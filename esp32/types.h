#ifndef ESP32_TYPES_H
#define ESP32_TYPES_H

#include <stdbool.h>
#include "bq.h"

typedef struct esp32state {
	BufferQueue rx_queue;

    enum esp32bootstatus {
        ESP32_STATUS_OFF,
        ESP32_STATUS_INIT,
        ESP32_STATUS_READY
    } boot_status;

    void (*char_recv)(struct esp32state *);

    void (*on_ready)(struct esp32state *);

    struct at_command *current_command;
} ESP32;



#endif
