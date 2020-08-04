#ifndef ESP32_TYPES_H
#define ESP32_TYPES_H

#include "bq.h"

/**
 * A container to hold the state of the ESP32 and
 * the associated callback functions.
 */
typedef struct esp32state {
    BufferQueue rx_queue; /*!< queue for data received */
    BufferQueue tx_queue; /*!< queue for commands to send */

    enum esp32bootstatus {
        ESP32_STATUS_OFF,
        ESP32_STATUS_INIT,
        ESP32_STATUS_READY
    } boot_status; /*!< the approx. status of the esp32 */

    void (*char_recv)(struct esp32state *); /*!< function called when a char has been received */

    void (*on_ready)(struct esp32state *); /*!< function called when the esp32 has booted */

    struct at_command *current_command; /*!< the last command sent or NULL if the esp32 is not busy */
} ESP32;



#endif
