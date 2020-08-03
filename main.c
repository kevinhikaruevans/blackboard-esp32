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
#include "sleep.h"

void esp32_ready(ESP32 *inst) {
    xil_printf("Gonna send an AT+GMR\r\n");
    esp32_println(inst, "AT+GMR");
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



