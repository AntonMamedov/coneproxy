#include "details/string_hash_map.h"

static size_t hash_impl(const char* str, size_t table_size, size_t key, size_t str_len) {
    unsigned long hash_result = 0;
    for (size_t i = 0; i < str_len; i++) {
        hash_result = (key * hash_result + str[i]);
    }
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}

static size_t hash1(const char* str, size_t table_size) {
    return hash_impl(str, table_size, table_size + 1, strlen(str));
}

static size_t hash2(const char* str, size_t table_size) {
    return hash_impl(str, table_size, table_size - 1, strlen(str));
}

int string_hash_map_allocate_buffer(string_hash_map_t* dst) {
    if (dst == NULL)
        return -1;
    size_t map_size = dst->map_size;
    size_t buffer_size = map_size * sizeof(string_hash_map_node_t)
                         + map_size * sizeof(hash_map_node_state_t);
    byte_t* buffer = (byte_t*) calloc(buffer_size, sizeof(byte_t));
    if (buffer == NULL)
        return -1;
    dst->buffer = buffer;
    dst->map = (string_hash_map_node_t*)buffer;
    dst->states = (hash_map_node_state_t*)(buffer + map_size * sizeof(string_hash_map_node_t));
    return 0;
}

int string_hash_map_init(string_hash_map_t* dst, size_t map_size, size_t load_factor) {
    if (dst == NULL) {
        return -1;
    }
    if ((map_size != 0) && ((map_size & (map_size - 1)) == 0)) // проверка на степень двойки
        map_size += 2;

    dst->map_size = map_size;
    dst->load_factor = load_factor;
    dst->size = 0;
    if (string_hash_map_allocate_buffer(dst) < 0)
        return -1;
    return 0;
}

void string_hash_map_release(string_hash_map_t* dst) {
    free(dst->buffer);
}

size_t string_hash_map_get_index(const string_hash_map_t* src, const char* key, size_t* h1, size_t i /* счетчик коллизий */) {
    if (i == 0)
        return *h1 = hash1(key, src->map_size);

    return *h1 + (hash2(key, src->map_size) * i) % src->map_size;
}

ssize_t string_hash_map_search_impl(const string_hash_map_t* src, const char* key) {
    if (src == NULL || key == NULL)
        return -1;

    size_t h1;
    for (size_t i = 0; i < src->size; i++)
    {
        size_t index = string_hash_map_get_index(src, key, &h1, i);
        hash_map_node_state_t state = src->states[index];
        switch (state) {
            case EMPTY:
                return -1;
            case DELETED:
                break;
            case TAKEN:
                if (strcmp(key, string_data(&src->map[index].key)) != 0)
                    break;
                return index;
        }
    }
    return -1;
}

value_type* string_hash_map_search(string_hash_map_t* src, const char* key) {
    ssize_t index = string_hash_map_search_impl(src, key);

    if (index < 0) {
        return NULL;
    }

    return &src->map[index].value;
}

ssize_t string_hash_map_delete(string_hash_map_t* dst, const char* key) {
    ssize_t index = string_hash_map_search_impl(dst, key);
    if (index < 0)
        return -1;
    dst->states[index] = DELETED;
    string_release(&dst->map[index].key);
    string_release(&dst->map[index].value);
    dst->size--;
    return index;
}

ssize_t string_hash_map_insert_for_extend(string_hash_map_t* dst, string_t * key, string_t* value) {
    size_t h1;
    for (size_t i = 0; i < dst->map_size; i++) {
        size_t index = string_hash_map_get_index(dst, string_data(key), &h1, i);
        hash_map_node_state_t state = dst->states[index];
        switch(state) {
            case EMPTY:
                dst->states[index] = TAKEN;
                string_share(&dst->map[index].key, key);
                string_share(&dst->map[index].value, value);
                return (ssize_t)index;
            case DELETED:
                assert(0 && "the new table should not have deleted fields");
            case TAKEN:
                break;
        }
    }

    return -1;
}

ssize_t string_hash_map_extend(string_hash_map_t* dst) {
    string_hash_map_t new_hash_table;
    string_hash_map_init(&new_hash_table, dst->map_size * dst->load_factor, dst->load_factor);

    for (size_t i = 0; i < dst->map_size; i++) {
        hash_map_node_state_t state = dst->states[i];
        switch (state) {
            case EMPTY:
            case DELETED:
                break;
            case TAKEN:
                if (string_hash_map_insert_for_extend(&new_hash_table, &dst->map[i].key, &dst->map[i].value) < 0) {
                    string_hash_map_release(&new_hash_table);
                    return -1;
                }
                break;
        }
    }
    string_hash_map_release(dst);
    *dst = new_hash_table;
    return (ssize_t)new_hash_table.map_size;
}

ssize_t string_hash_map_insert(string_hash_map_t* dst, const char* key, const char* value) {
    size_t h1;
    for (size_t i = 0; i < dst->map_size; i++) {
        size_t index = string_hash_map_get_index(dst, key, &h1, i);
        hash_map_node_state_t state = dst->states[index];
        switch(state) {
            case EMPTY:
                dst->states[index] = TAKEN;
                string_copy_c_str(&dst->map[index].key, key);
                string_copy_c_str(&dst->map[index].value, value);
                dst->size++;
                return (ssize_t)index;
            case DELETED:
                assert(0 && "the new table should not have deleted fields");
            case TAKEN:
                break;
        }
    }

    return -1;
}