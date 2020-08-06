#include "wifi.h"

void wifi_init(ESP32 *inst) {
    inst->wifi_state.wifi_status = WIFI_DISCONNECTED;
}
void wifi_connect(ESP32 *inst, const char *ssid, const char *psk) {
    char command[BQ_BUFFER_SIZE];

    sprintf(command, "AT+CWJAP=\"%s\",\"%s\"", ssid, psk);
    bq_enqueue(&inst->tx_queue, command);

    xil_printf("[WIFI] trying to connect to network '%s'\r\n", ssid);
}

void wifi_disconnect(ESP32 *inst) {
    char command[BQ_BUFFER_SIZE];

    bq_enqueue(&inst->tx_queue, "AT+CWQAP");

    xil_printf("[WIFI] disconnecting from network...\r\n");
}
