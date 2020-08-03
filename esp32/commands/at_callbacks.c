#include "at_callbacks.h"

struct at_command esp32_commands[2] = {
    {ESP32_AT_AT,  "AT",     "Tests AT Startup", &esp32_callback_at},
    //{ESP32_AT_RST, "AT+RST", "Restarts the ESP32", &esp32_callback_at},
    {ESP32_AT_GMR, "AT+GMR", "Checks version information", esp32_callback_gmr},
};


void esp32_callback_at(struct esp32state *inst, const char *resp) {
    xil_printf("Hit AT callback\r\n");

    if (strcmp("OK", resp) == 0) {
        inst->current_command = NULL;
        char *prev = esp32_prev_buffer(inst, resp);

        xil_printf("prev='%s'\r\n", prev);
    }
}

void esp32_callback_gmr(struct esp32state *inst, const char *resp) {
    if (strcmp("OK", resp) == 0) {
        inst->current_command = NULL;

        char *prev = esp32_prev_buffer(inst, resp);

        prev = esp32_prev_buffer(inst, prev);
        xil_printf("[RECV] AT+GMR '%s'\r\n", prev);

        prev = esp32_prev_buffer(inst, prev);
        xil_printf("[RECV] AT+GMR '%s'\r\n", prev);

        prev = esp32_prev_buffer(inst, prev);
        xil_printf("[RECV] AT+GMR '%s'\r\n", prev);
    }
}
