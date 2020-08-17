#ifndef ESP32_NET_TYPES_H
#define ESP32_NET_TYPES_H

#include <stdbool.h>
//#include "../types.h"

#define SOCKET_HOST_SIZE 32

typedef enum at_socket_protocol {
    SOCKET_TCP,
    SOCKET_UDP,
    SOCKET_SSL
} ATSocketProtocol;


typedef struct at_socket {
    enum at_socket_protocol protocol;

    char host[32];

    int port;

    char buffer[1024];

    int buffer_size;

    int link_id;

    int receivedBytesRemaining;

    void *parent;

    void (*on_receive)(void *state, struct at_socket *);

} ATSocket;


typedef enum http_method {
    HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_UPDATE, HTTP_DELETE
} HttpMethod;

extern const char HTTP_METHODS[5][10];

typedef struct http_message {
    struct at_socket * socket;

    struct {
        char headers[8][64];
        HttpMethod method;
        char uri[128];
    } request;

    struct {
        char headers[8][64];
        int response_code;
        char * body;
    } response;

    void (*on_success)(struct esp32state *, struct http_message *);
//    void (*on_error)(struct esp32state *, struct http_message *);
} HttpMessage;



#endif
