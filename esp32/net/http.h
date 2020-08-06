#ifndef ESP32_NET_HTTP_H
#define ESP32_NET_HTTP_H

#include <string.h>
#include "types.h"
#include "../types.h"

void http_init(HttpMessage *);
void http_create_request(HttpMessage *, const HttpMethod, char *url);
void http_resolve(ESP32 *, HttpMessage *);
void http_send(ESP32 *, HttpMessage *);

#endif
