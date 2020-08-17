#include <stdio.h>
#include "xil_io.h"
#include "xil_types.h"
#include "xparameters.h"
#include "xil_io.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "esp32/gic.h"
#include "esp32/esp32.h"
#include "esp32/net/http.h"
#include "esp32/net/wifi.h"
#include "sleep.h"

#include "esp32/net/socket.h"



void on_success(struct esp32state *inst, struct http_message *client) {
    xil_printf("Received data: '%s'\r\n", client->response.body);
}

static struct http_message msg;

void on_connect(struct esp32state *inst) {
    http_create_request(inst, &msg, HTTP_GET, "http://worldtimeapi.org/api/ip.txt");
    msg.on_success = &on_success;
    http_execute(inst, &msg);
}


void esp32_ready(ESP32 *inst) {
    inst->wifi_state.on_connect = &on_connect;
    wifi_connect(inst, "dd-wrt", "");

}

int main()
{
    ESP32 esp32;
    esp32_state_init(&esp32);
    esp32_interrupts_config(&esp32);
    esp32_enable_uart(&esp32);
    esp32_enable_module(&esp32);
    esp32_set_ready(&esp32, &esp32_ready);

    while(1) {
        esp32_run_queue(&esp32);
    }
    return 0;
}



