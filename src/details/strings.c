#include "details/strings.h"

#include <stdlib.h>
#include <string.h>

string_t string_new(const char* str) {
    string_t new_str;
    string_init(&new_str);
    string_copy_c_str(&new_str, str);
    return new_str;
}

void string_init(string_t* dst) {
    memset(dst, 0, sizeof (string_t));
}

void string_release(string_t* dst) {
    if (dst->len > DEFAULT_STRING_SIZE)
        free(dst->str.dynamic_str.str);
    dst->len = 0;
}

const char* string_data(const string_t* src) {
    if (src->len < DEFAULT_STRING_SIZE) {
        return src->str.static_str;
    }

    return src->str.dynamic_str.str;
}

ssize_t string_copy_c_str(string_t* dst, const char* src) {
    char* data = NULL;

    if (dst->len > DEFAULT_STRING_SIZE) {
        string_release(dst);
        string_init(dst);
    }
    dst->len = 0;
    data = dst->str.static_str;


    for (size_t i = 0; src[i] != '\0'; i++) {
        if (dst->len == DEFAULT_STRING_SIZE && data != dst->str.dynamic_str.str) {
            char tmp[DEFAULT_STRING_SIZE + 1];
            strcpy(tmp, data);
            dst->str.dynamic_str.str = malloc(sizeof(char) * (DEFAULT_STRING_SIZE + 1) * 3);
            if (dst->str.dynamic_str.str == NULL)
                return -1;
            dst->str.dynamic_str.allocated_size = DEFAULT_STRING_SIZE * 3;
            data = dst->str.dynamic_str.str;
            strcpy(data, tmp);
        }

        if (dst->len > DEFAULT_STRING_SIZE && dst->len == dst->str.dynamic_str.allocated_size - 1) {
            data = realloc(data, sizeof(char) * dst->str.dynamic_str.allocated_size * 2);
            if (data == NULL) {
                free(dst->str.dynamic_str.str);
                return -1;
            }
            dst->str.dynamic_str.allocated_size *= 2;
            dst->str.dynamic_str.str = data;
        }
        data[i] = src[i];
        dst->len++;
    }
    data[dst->len] = '\0';
}

void string_share(string_t* dst, string_t* src) {
    if (dst->len > 0) {
        string_release(dst);
        string_init(dst);
    }
    if (src->len > DEFAULT_STRING_SIZE) {
        dst->str.dynamic_str.str = src->str.dynamic_str.str;
        dst->len = src->len;
    } else
        memcpy(dst, src, sizeof (string_t));
}

bool string_is_eq(const string_t* left, const string_t* right) {
    if (left->len != right->len) {
        return false;
    }

    const size_t* left_str_num = (const size_t*)string_data(left);
    const size_t* right_str_num = (const size_t*)string_data(right);

    size_t size = left->len / sizeof(size_t);
    for (size_t i = 0; i < size; i++) {
        if (left_str_num[i] != right_str_num[i]) {
            return false;
        }
    }

    size_t shift = sizeof(size_t) - left->len % sizeof(size_t);

    if (shift != 0) {
        return (left_str_num[size] >> shift) == (right_str_num[size] >> shift);
    }
    return true;
}

bool string_is_eq2(const string_t* left, const string_t* right) {
    return !strcmp(string_data(left), string_data(right));
}

char *strcpy_not_yet_determining_zero(char *dest, const char *src) {
    while (*src != '\0') {
        *dest++ = *src++;
    }
    return dest;
}