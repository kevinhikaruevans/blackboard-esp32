#ifndef ESP32_TYPES_H
#define ESP32_TYPES_H

#include <stdbool.h>

#define ESP32_BUFFER_SIZE     256
#define ESP32_SCROLLBACK_SIZE 32
#define ESP32_IDX_INACTIVE    -1

typedef struct esp32state {
    volatile char buffers[ESP32_SCROLLBACK_SIZE][ESP32_BUFFER_SIZE];
    volatile  int char_index;
    volatile  int buffer_index;
    volatile  int queue_buffer_index;

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
