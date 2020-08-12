#ifndef ESP32_NET_HTTP_H
#define ESP32_NET_HTTP_H

#include <stdio.h>
#include "types.h"
#include "../types.h"

void http_init(HttpMessage *);
void http_create_request(ESP32 *, const HttpMethod, char *url);
void http_resolve(ESP32 *);
void http_send(ESP32 *);

#endif
