#include "net/tcp.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

descriptor_t tcp_new_acceptor(host_ip_t host_ip, uint16_t port, const tcp_options_t* tcp_options) {
    descriptor_t sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }

    sockaddr_in_t serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(host_ip);
    serv_addr.sin_port = htons(port);

    if (bind(sock, (sockaddr_t *) &serv_addr, sizeof(serv_addr)) < 0) {
        return -1;
    }

    if (listen(sock, INT_MAX) < 0) {
        return -1;
    }

    if (!tcp_options) {
        return sock;
    }

    if (tcp_options->work_mod == NOT_BLOCKING) {
        if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) {
            close(sock);
            return -1;
        }
    }

    if (tcp_options->tcp_cork) {
        int state = 1;
        if (setsockopt(sock, IPPROTO_TCP, TCP_CORK, &state, sizeof(state)) > 0) {
            close(sock);
            return -1;
        }
    }


    return sock;
}

descriptor_t tcp_listen(descriptor_t server_fd, descriptor_work_mod_t work_mod) {
    sockaddr_storage_t session_addr;
    socklen_t s_size = sizeof(session_addr);

    descriptor_t session = accept(server_fd, (sockaddr_t *) &session_addr, &s_size);
    if (session < 0) {
        return -1;
    }

    if (work_mod == NOT_BLOCKING) {
        if (fcntl(session, F_SETFL, O_NONBLOCK) < 0) {
            return -1;
        }
    }

    return session;
}

int tcp_new_point(sockaddr_in_t *dst, const char *host, uint16_t port) {
    memset(dst, 0, sizeof(sockaddr_in_t));
    dst->sin_family = AF_INET;
    dst->sin_port = htons(port);
    if (inet_pton(AF_INET, host, &dst->sin_addr) <= 0) {
        return -1;
    }

    return 0;
}

descriptor_t tcp_new_connect(const sockaddr_in_t *src, descriptor_work_mod_t work_mod) {
    descriptor_t fd = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(fd, (sockaddr_t*)src, sizeof(sockaddr_in_t)) < 0)
        return -1;

    if (work_mod == NOT_BLOCKING) {
        if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
            return -1;
        }
    }

    return fd;
}