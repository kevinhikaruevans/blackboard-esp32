#ifndef ESP32_NET_WIFI_H
#define ESP32_NET_WIFI_H

#include <string.h>
#include "xil_printf.h"
#include "../types.h"
#include "../bq.h"

void wifi_init(ESP32 *inst);
void wifi_connect(ESP32 *inst, const char *ssid, const char *psk);
void wifi_disconnect(ESP32 *);

#endif
