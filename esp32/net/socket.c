#include "socket.h"

const char AT_SOCKET_PROTOCOLS_STR[3][4] = {
    "TCP",
    "UDP",
    "SSL"
};

void socket_init(
        struct esp32state *inst,
        struct at_socket * sock,
        enum at_socket_protocol type,
        char host[SOCKET_HOST_SIZE],
        int port)
{
    memcpy(sock->host, host, SOCKET_HOST_SIZE);
    sock->protocol = type;
    sock->port = port;
    sock->link_id = 1; // TODO: round robin
    sock->buffer_size = 0;
    sock->receivedBytesRemaining = 0;
}

void socket_open(ESP32 *inst, struct at_socket *sock) {
    char buffer[64];
    sprintf(buffer,
            "AT+CIPSTART=%d,\"%s\",\"%s\",%d",
            sock->link_id,
            AT_SOCKET_PROTOCOLS_STR[sock->protocol],
            sock->host,
            sock->port
            );
    bq_enqueue(&inst->tx_queue, buffer);
}

void socket_close(ESP32 *inst, struct at_socket *sock) {
    char buffer[32];
    sprintf(buffer, "AT+CIPCLOSE=%d", sock->link_id);
    bq_enqueue(&inst->tx_queue, buffer);
}

void socket_send(ESP32 *inst, struct at_socket *sock, char *data, int length) {
    char buffer[32];
    sprintf(buffer, "AT+CIPSEND=%d,%d", sock->link_id, length);
    bq_enqueue(&inst->tx_queue, buffer);
    bq_enqueue(&inst->tx_queue, data);
}

void socket_set_receive(struct at_socket *sock, void (*cb)(void *, struct at_socket *)) {
    sock->on_receive = cb;
}
