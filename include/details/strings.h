#pragma once
#include <stdlib.h>
#include <stdbool.h>

#define DEFAULT_STRING_SIZE 15

typedef struct dynamic_string {
    char* str;
    size_t allocated_size;
} dynamic_string_t;

typedef union {
    dynamic_string_t dynamic_str;
    char             static_str[DEFAULT_STRING_SIZE + 1];
} string_payload_t;

typedef struct string {
    string_payload_t str;
    size_t len;
} string_t;

string_t string_new(const char* str);
void string_init(string_t* dst);
void string_release(string_t* dst);
bool string_is_eq(const string_t* left, const string_t* right);
bool string_is_eq2(const string_t* left, const string_t* right);
const char* string_data(const string_t* src);
ssize_t string_copy_c_str(string_t* dst, const char* src);
void string_share(string_t* dst, string_t* src);

char *strcpy_not_yet_determining_zero(char *dest, const char *src);