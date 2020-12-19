#ifndef ESP32_NET_WIFI_H
#define ESP32_NET_WIFI_H

#include "../bq.h"
#include "../types.h"
#include "xil_printf.h"
#include <string.h>

void wifi_init(struct esp32state *inst);
void wifi_connect(struct esp32state *inst, const char *ssid, const char *psk);
void wifi_disconnect(struct esp32state *);

#endif
