#ifndef ESP32_NET_SOCKET_H
#define ESP32_NET_SOCKET_H

#include <stdio.h>
#include "../bq.h"
#include "./types.h"
#include "../types.h"


extern const char AT_SOCKET_PROTOCOLS_STR[3][4];

void socket_init(
        struct esp32state *inst,
        struct at_socket * sock,
        enum at_socket_protocol type,
        char host[SOCKET_HOST_SIZE],
        int port);

void socket_open(struct esp32state *inst, struct at_socket *sock);
void socket_close(struct esp32state *inst, struct at_socket *sock);
void socket_send(struct esp32state *inst, struct at_socket *sock, char *data, int length);
void socket_set_receive(struct at_socket *, void (*cb)(void *, struct at_socket *));


#endif
