#pragma once
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

typedef enum hash_table_node_state {
    EMPTY = 0,
    DELETED,
    TAKEN
} hash_table_node_state_t;


typedef size_t (*hash_func_p)(const void * key, size_t table_size, const void* /* для случаев когда нужны доп данные для хеш-фунции */);
typedef int (*key_cmp_func_p)(const void * left, const void * right);
typedef void (*hash_table_node_release_func_p)(void* dst);
typedef void (*assignment_func_p)(void *dst, const void * src);
typedef void (*generate_hash_data_func_p)(const void* dst, const void* src);



#define DEFINE_HASH_TABLE_WITH_ADDITIONAL_HASH_DATA(struct_name, key_type, value_type, hash_data_length) \
\
\
typedef struct struct_name##_node {\
    hash_table_node_state_t state;\
    key_type                key;\
    value_type              value;\
} struct_name##_node_t;\
\
typedef struct hash_table_tool_funcs {\
    hash_func_p                    hash1;\
    hash_func_p                    hash2;\
    key_cmp_func_p                 cmp;\
    hash_table_node_release_func_p release;\
    assignment_func_p              key_assignment;\
    assignment_func_p              value_assignment;\
    generate_hash_data_func_p      generate_hash_data;\
} hash_table_tool_funcs_t;\
\
typedef struct struct_name {\
    struct_name##_node_t*         table;\
    size_t                   allocated;\
    size_t                   size;\
    size_t                   load_factor;\
    hash_table_tool_funcs_t  funcs;\
} struct_name##_t;\
\
\
struct_name##_t struct_name##_new(size_t allocated, size_t load_factor, const hash_table_tool_funcs_t* funcs);\
value_type* struct_name##_search(const key_type* key, struct_name##_t* src);\
ssize_t struct_name##_delete(const key_type* key, struct_name##_t* src);\
struct_name##_t struct_name##_release(struct_name##_t* dst);\
ssize_t struct_name##_insert(struct_name##_t* dst, const key_type* key, const value_type* value);




#define IMPLEMENT_HASH_TABLE_WITH_ADDITIONAL_HASH_DATA(struct_name, key_type, value_type, hash_data_length) \
struct_name##_t struct_name##_new(size_t allocated, size_t load_factor, const hash_table_tool_funcs_t* funcs) {\
    if ((allocated != 0) && ((allocated & (allocated - 1)) == 0))\
        allocated += 2;\
\
    struct_name##_node_t* table = (struct_name##_node_t*)calloc(allocated, sizeof(struct_name##_node_t));\
    struct_name##_t hash_table = {table, allocated, 0, load_factor, *funcs};\
    return hash_table;\
}\
\
static size_t struct_name##_get_index(const key_type* key, const struct_name##_t* src, const void* hash_data, size_t* h1, size_t i) {\
    if (i == 0)\
        return *h1 = src->funcs.hash1(key, src->allocated, hash_data);\
\
    return *h1 + (src->funcs.hash2(key, src->allocated, hash_data) * i) % src->allocated;\
}\
\
static struct_name##_node_t * struct_name##_search_impl(const key_type* key, struct_name##_t* src) {\
    unsigned char hash_data[hash_data_length];\
    src->funcs.generate_hash_data(hash_data, key);\
    size_t h1;\
    for (size_t i = 0; i < src->size; i++)\
    {\
        size_t index = struct_name##_get_index(key, src, hash_data, &h1, i);\
        hash_table_node_state_t state = src->table[index].state;\
        switch (state) {\
            case EMPTY:\
                return NULL;\
            case DELETED:\
                break;\
            case TAKEN:\
                if (!src->funcs.cmp(&src->table[index].key, key))\
                    break;\
                return &src->table[index];\
        }\
    }\
    return NULL;\
}\
\
value_type* struct_name##_search(const key_type* key, struct_name##_t* src) {\
    struct_name##_node_t* res = struct_name##_search_impl(key, src);\
    if (res == NULL)\
        return NULL;\
    return &res->value;\
}                                                               \
                                                                \
ssize_t struct_name##_delete(const key_type* key, struct_name##_t* src) {\
    struct_name##_node_t* res = struct_name##_search_impl(key, src);\
    if (res == NULL)\
        return -1;\
    res->state = DELETED;\
    src->funcs.release(&res->key);\
    src->funcs.release(&res->value);\
    return 0;\
}\
\
struct_name##_t struct_name##_release(struct_name##_t* dst) {\
    for (size_t i = 0; i < dst->size; i++) {\
        dst->funcs.release(&dst->table[i]);\
    }\
\
    free(dst->table);\
}\
\
static ssize_t struct_name##_extend(struct_name##_t* dst) {\
    struct_name##_t new_hash_table = struct_name##_new(dst->allocated * dst->load_factor, dst->load_factor, &dst->funcs);\
\
    for (size_t i = 0; i < dst->allocated; i++) {\
        hash_table_node_state_t state = dst->table[i].state;\
        switch (state) {\
            case EMPTY:\
            case DELETED:\
                break;\
            case TAKEN:\
                if (struct_name##_insert(&new_hash_table, &dst->table[i].key, &dst->table[i].value) < 0) {\
                    return -1;\
                }\
                break;\
        }\
    }\
    struct_name##_release(dst);\
    *dst = new_hash_table;\
    return (ssize_t)new_hash_table.allocated;\
}\
\
ssize_t struct_name##_insert(struct_name##_t* dst, const key_type* key, const value_type* value) {\
    if (dst->size >= dst->allocated / dst->load_factor) {\
        if (struct_name##_extend(dst) < 0) {\
            return -1;\
        }\
    }\
    unsigned char hash_data[hash_data_length];\
    dst->funcs.generate_hash_data(key, hash_data);\
    size_t h1;\
    for (size_t i = 0; i < dst->allocated; i++) {\
\
        size_t index = struct_name##_get_index(key, dst, hash_data, &h1, i);\
        hash_table_node_state_t state = dst->table[index].state;\
        switch(state) {\
            case EMPTY:\
            case DELETED:\
                dst->table[index].state = TAKEN;\
                dst->funcs.key_assignment(&dst->table[index].key, key);\
                dst->funcs.value_assignment(&dst->table[index].value, value);\
                dst->size++;\
                return index;\
            case TAKEN:\
                break;\
        }\
    }\
\
    return -1;\
}


#define DEFINE_HASH_TABLE(struct_name, key_type, value_type) \
    DEFINE_HASH_TABLE_WITH_ADDITIONAL_HASH_DATA(struct_name, key_type, value_type, 0)

#define IMPLEMENT_HASH_TABLE(struct_name, key_type, value_type) \
    IMPLEMENT_HASH_TABLE_WITH_ADDITIONAL_HASH_DATA(struct_name, key_type, value_type, 0)