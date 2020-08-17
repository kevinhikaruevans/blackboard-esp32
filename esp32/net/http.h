#ifndef ESP32_NET_HTTP_H
#define ESP32_NET_HTTP_H

#include "../types.h"
#include "./types.h"
#include "./socket.h"
#include <stdio.h>

void http_create_request(struct esp32state *inst, struct http_message *msg, HttpMethod method, char *url);
void http_execute(struct esp32state *inst, struct http_message *msg);
void http_on_receive(struct esp32state *inst, struct at_socket *socket);


#endif
