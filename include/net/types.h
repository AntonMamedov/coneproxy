#pragma once
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

typedef int descriptor_t;
typedef struct epoll_event epoll_event_t;
typedef struct stat stat_t;
typedef struct tm tm_t;
typedef unsigned char byte_t;
typedef struct sockaddr_in sockaddr_in_t;
typedef struct sockaddr_storage sockaddr_storage_t;
typedef struct sockaddr sockaddr_t;