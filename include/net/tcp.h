#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "types.h"

// тип создаваемого дескриптора (блокирующий / не блокирующий)
typedef enum {
    BLOCKING = 0,
    NOT_BLOCKING = 1
} descriptor_work_mod_t;

// ip хоста
typedef enum {
    ALL       = INADDR_ANY,      // 0.0.0.0 прослушивать все ip хоста
    LOCAL     = INADDR_LOOPBACK, // 127.0.0.1 только localhost
    BROADCAST = INADDR_BROADCAST // 255.255.255.255 широковещательный
} host_ip_t;

// опции сокетоа
typedef struct tcp_options {
    descriptor_work_mod_t work_mod; // режим прослушивания
    bool tcp_cork;                  // включить/отключить tcp корк
} tcp_options_t;

// создать дескриптор для прослушивания заданного порта
descriptor_t tcp_new_acceptor(host_ip_t host_ip, uint16_t port, const tcp_options_t* tcp_options);
// начать порослушивание порта
// возвращает дескриптор нового соединения
descriptor_t tcp_listen(descriptor_t server_fd, descriptor_work_mod_t work_mod);
// создает точку подключения
// заполняет информацию о сервере необходимую для создания подключений
int tcp_new_point(sockaddr_in_t *dst, const char *host, uint16_t port);
// создает новое подключение
descriptor_t tcp_new_connect(const sockaddr_in_t *src, descriptor_work_mod_t work_mod);