#pragma once

#include <stdlib.h>

// десодирует url
size_t urldecode(char *dest, const char *src);
// кодирует url
size_t urlndecode(char *dest, const char *src, ssize_t n);