#include "wifi.h"

void wifi_init(struct esp32state *inst) {
    inst->wifi_state.wifi_status = WIFI_DISCONNECTED;
    inst->wifi_state.on_connect = NULL;
}

void wifi_connect(struct esp32state *inst, const char *ssid, const char *psk) {
    char command[BQ_BUFFER_SIZE];

    bq_enqueue(&inst->tx_queue, "AT+CWMODE=3");
    bq_enqueue(&inst->tx_queue, "AT+CIPMODE=0");
    bq_enqueue(&inst->tx_queue, "AT+CIPMUX=1");

    sprintf(command, "AT+CWJAP=\"%s\",\"%s\"", ssid, psk);
    bq_enqueue(&inst->tx_queue, command);

    xil_printf("[WIFI] trying to connect to network '%s'\r\n", ssid);
}

void wifi_disconnect(struct esp32state *inst) {
    char command[BQ_BUFFER_SIZE];

    bq_enqueue(&inst->tx_queue, "AT+CWQAP");

    xil_printf("[WIFI] disconnecting from network...\r\n");
}
