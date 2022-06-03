#pragma once
#include "hash_table.h"
#include "strings.h"

DEFINE_HASH_TABLE(strings_hash_table, string_t, string_t)

strings_hash_table_t make_strings_hash_table(size_t allocated, size_t load_factor);