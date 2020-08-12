#include "at_callbacks.h"

struct at_command esp32_commands[7] = {
    { "AT", &esp32_callback_at },
    { "AT+GMR", &esp32_callback_gmr },
    { "AT+CWMODE=", &esp32_callback_default },
    { "AT+CWJAP=", &esp32_callback_cwjap_set },
    { "AT+CIPDOMAIN=", &esp32_callback_cipdomain_set },
    { "AT+CIPSTART=", &esp32_callback_cipstart_set },
    { "AT+CIPMODE=", &esp32_callback_cipmode_set }
};

void esp32_callback_default(struct esp32state *inst, const char *resp) {
    if (strcmp("OK", resp) == 0 || strcmp("ERROR", resp) == 0) {
        inst->current_command = NULL;
        xil_printf("[ CB] end default command\r\n");

        //xil_printf("prev='%s'\r\n", prev);
    }
}

void esp32_callback_cipmode_set(struct esp32state *inst, const char *resp) {
    if (strcmp("OK", resp) == 0 || strcmp("ERROR", resp) == 0) {
        inst->current_command = NULL;
    }
}
void esp32_callback_cipstart_set(struct esp32state *inst, const char *resp) {
    if (strcmp("OK", resp) == 0 || strcmp("ERROR", resp) == 0) {
        inst->current_command = NULL;
        inst->current_msg->request.cip_started = true;
    }
}

void esp32_callback_cipdomain_set(struct esp32state *inst, const char *resp) {
    if (strstr(resp, "+CIPDOMAIN:") == resp) {
        const char *ip = resp + sizeof("+CIPDOMAIN:") - 1; // hopefully this is optimized out, lol
        xil_printf("Resolved as '%s'\r\n", ip);
        memcpy(inst->current_msg->request.ip, ip, strlen(ip));
    }
    if (strcmp("OK", resp) == 0 || strcmp("ERROR", resp) == 0) {
        inst->current_command = NULL;
        xil_printf("[ CB] end CIPDOMAIN command\r\n");
    }
}
void esp32_callback_at(struct esp32state *inst, const char *resp) {
    if (strcmp("OK", resp) == 0) {
        inst->current_command = NULL;
        char *prev = bq_prev_buffer(inst->rx_queue, resp);

        xil_printf("prev='%s'\r\n", prev);
    }
}

void esp32_callback_cwjap_set(struct esp32state *inst, const char *resp) {
    //xil_printf("Hit AT+CWJAP= callback\r\n");


    if (strcmp("OK", resp) == 0) {
        inst->current_command = NULL;
        //char *prev = esp32_prev_buffer(inst, resp);

        //xil_printf("prev='%s'\r\n", prev);
    }
}
void esp32_callback_gmr(struct esp32state *inst, const char *resp) {
    if (strcmp("OK", resp) == 0) {
        inst->current_command = NULL;

        char *prev = bq_prev_buffer(inst->rx_queue, resp);

        prev = bq_prev_buffer(inst->rx_queue, prev);
        xil_printf("[RECV] AT+GMR '%s'\r\n", prev);

        prev = bq_prev_buffer(inst->rx_queue, prev);
        xil_printf("[RECV] AT+GMR '%s'\r\n", prev);

        prev = bq_prev_buffer(inst->rx_queue, prev);
        xil_printf("[RECV] AT+GMR '%s'\r\n", prev);
    }
}
