#pragma once
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "details/strings.h"

typedef enum hash_table_node_state {
    EMPTY = 0,
    DELETED,
    TAKEN
} hash_map_node_state_t;

typedef string_t key_type;
typedef string_t value_type;
typedef unsigned char byte_t;

typedef struct string_hash_map_node {
    key_type                key;
    value_type              value;
} string_hash_map_node_t;

typedef struct string_hash_map {
    byte_t*                        buffer;
    string_hash_map_node_t*        map;
    hash_map_node_state_t*         states;
    size_t                         map_size;
    size_t                         size;
    size_t                         load_factor;
} string_hash_map_t;

int string_hash_map_init(string_hash_map_t* dst, size_t map_size, size_t load_factor);
void string_hash_map_release(string_hash_map_t* dst);
value_type* string_hash_map_search(string_hash_map_t* src, const char* key);
ssize_t string_hash_map_delete(string_hash_map_t* dst, const char* key);
ssize_t string_hash_map_insert(string_hash_map_t* dst, const char* key, const char* value);

