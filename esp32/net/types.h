#ifndef ESP32_NET_TYPES_H
#define ESP32_NET_TYPES_H

#include "../types.h"
typedef enum http_method {
    HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_UPDATE, HTTP_DELETE
} HttpMethod;

typedef struct http_message {
    struct {
        char domain[32];
        char ip[16];
        char headers[32][16];
        HttpMethod method;
        char uri[128];
    } request;

    struct {
        char headers[32][16];
        enum {
            HTTP_OK = 200, HTTP_NOT_FOUND = 404, HTTP_OTHER
        } responseCode;

        char body[1024]; /* something else should be used here */
    } response;

    void (*on_success)(struct esp32state *, struct http_message *);
    void (*on_error)(struct esp32state *, struct http_message *);
} HttpMessage;

#endif
