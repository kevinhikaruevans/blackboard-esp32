#include <stdio.h>
#include "xil_io.h"
#include "xil_types.h"
#include "xparameters.h"
#include "xil_io.h"
#include "xil_exception.h"
#include "xscugic.h"

//#include "esp32.h"
#include "esp32/gic.h"
#include "esp32/esp32.h"
#include "esp32/net/http.h"
#include "sleep.h"

void esp32_ready(ESP32 *inst) {
	bq_enqueue(&inst->tx_queue, "AT+CWMODE=3");
    bq_enqueue(&inst->tx_queue, "AT+CWJAP=\"dd-wrt\",\"3606922899\"");
    //bq_enqueue(&inst->tx_queue, "AT+CIPDOMAIN=\"www.khevans.com\"");

    http_create_request(inst, HTTP_GET, "http://direct.khevans.com/test");
    http_resolve(inst);
    http_send(inst);
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



