#include "details/strings_hash_table.h"
#include "stdio.h"
IMPLEMENT_HASH_TABLE(strings_hash_table, string_t, string_t);

static size_t hash_impl(const string_t* str, size_t table_size, size_t key) {
    unsigned long hash_result = 0;
    const char* c_str = string_data(str);
    for (size_t i = 0; i < str->len; i++) {
        hash_result = (key * hash_result + c_str[i]);
    }
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}

static size_t hash1(const void* str, size_t table_size, const void* hash_data) {
    (void)hash_data;
    return hash_impl((const string_t *)str, table_size, table_size + 1);
}

static size_t hash2(const void* str, size_t table_size, const void* hash_data) {
    (void)hash_data;
    size_t s = hash_impl((const string_t *)str, table_size, table_size - 1);
    printf("string: %s hash: %lu\n", string_data(str), s);
    return hash_impl((const string_t *)str, table_size, table_size - 1);
}

static int cmp(const void* left, const void* right) {
    return (string_is_eq((const string_t*)left, (const string_t*)right));
}

static void release(void* dst) {
    string_release((string_t*)dst);
}

static void string_assignment(void *dst, const void * src) {
    string_t * str_dst = (string_t*) dst;
    const string_t * str_src = (const string_t *) src;
    string_share(str_dst, (string_t*)str_src);
}

static void gen_hash_data(const void* a, const void* b) {
    (void)a;
    (void)b;
}

strings_hash_table_t make_strings_hash_table(size_t allocated, size_t load_factor) {
    hash_table_tool_funcs_t funcs = {hash1, hash2, cmp, release, string_assignment, string_assignment, gen_hash_data};
    return strings_hash_table_new(allocated, load_factor, &funcs);
}