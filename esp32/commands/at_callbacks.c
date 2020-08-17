#include "at_callbacks.h"

struct at_command esp32_commands[9] = {
    { "AT", &esp32_callback_at },
    { "AT+GMR", &esp32_callback_gmr },
    { "AT+CWMODE=", &esp32_callback_default },
    { "AT+CWJAP=", &esp32_callback_cwjap_set },
    { "AT+CIPDOMAIN=", &esp32_callback_cipdomain_set },
    { "AT+CIPSTART=", &esp32_callback_cipstart_set },
    { "AT+CIPMODE=", &esp32_callback_default },
    { "AT+CIPSEND=", &esp32_callback_cipsend_set },
    { "AT+CIPMUX=", &esp32_callback_default }
};

ATCommandStatus esp32_callback_default(struct esp32state *inst, const char *resp) {
    if (strcmp("OK", resp) == 0 || strcmp("ERROR", resp) == 0) {
        xil_printf("[ CB] end default command\r\n");
        return COMMAND_DONE;
    }

    return COMMAND_WAITING;
}

ATCommandStatus esp32_callback_cipmode_set(struct esp32state *inst, const char *resp) {
    return esp32_callback_default(inst, resp);
}

ATCommandStatus esp32_callback_cipsend_set(struct esp32state *inst, const char *resp) {
    //return esp32_callback_default(inst, resp);
    /*if (strcmp("SEND OK", resp) == 0
            || strcmp("ERROR", resp) == 0
            || strcmp("SEND FAIL", resp) == 0)
                    {
        xil_printf("[ CB] end CIPSEND command\r\n");
        return COMMAND_DONE;
    }

     return COMMAND_WAITING;*/
}

ATCommandStatus esp32_callback_cipstart_set(struct esp32state *inst, const char *resp) {
    if (strcmp("OK", resp) == 0 || strcmp("ERROR", resp) == 0) {
        //inst->current_msg->request.cip_started = true;

        return COMMAND_DONE;
    }
    return COMMAND_WAITING;
}

ATCommandStatus esp32_callback_cipdomain_set(struct esp32state *inst, const char *resp) {
    if (strcmp("OK", resp) == 0 || strcmp("ERROR", resp) == 0) {
        xil_printf("[ CB] end CIPDOMAIN command\r\n");
        return COMMAND_DONE;
    }

    if (strstr(resp, "+CIPDOMAIN:") == resp) {
        const char *ip = resp + sizeof("+CIPDOMAIN:") - 1; // hopefully this is optimized out, lol
        xil_printf("Resolved as '%s'\r\n", ip);
        //memcpy(inst->current_msg->request.ip, ip, strlen(ip));
    }

    return COMMAND_WAITING;
}
ATCommandStatus esp32_callback_at(struct esp32state *inst, const char *resp) {
    if (strcmp("OK", resp) == 0) {
        char *prev = bq_prev_buffer(inst->rx_queue, resp);

        xil_printf("prev='%s'\r\n", prev);

        return COMMAND_DONE;
    }
    return COMMAND_WAITING;
}

ATCommandStatus esp32_callback_cwjap_set(struct esp32state *inst, const char *resp) {
    if (strcmp("WIFI GOT IP", resp) == 0) {
        inst->wifi_state.wifi_status = WIFI_CONNECTED;

        if (inst->wifi_state.on_connect != NULL) {
            inst->wifi_state.on_connect(inst);
        }
    }
    return esp32_callback_default(inst, resp);
}

ATCommandStatus esp32_callback_gmr(struct esp32state *inst, const char *resp) {
    if (strcmp("OK", resp) == 0) {
        char *prev = bq_prev_buffer(inst->rx_queue, resp);

        prev = bq_prev_buffer(inst->rx_queue, prev);
        xil_printf("[RECV] AT+GMR '%s'\r\n", prev);

        prev = bq_prev_buffer(inst->rx_queue, prev);
        xil_printf("[RECV] AT+GMR '%s'\r\n", prev);

        prev = bq_prev_buffer(inst->rx_queue, prev);
        xil_printf("[RECV] AT+GMR '%s'\r\n", prev);

        return COMMAND_DONE;
    }

    return COMMAND_WAITING;
}
