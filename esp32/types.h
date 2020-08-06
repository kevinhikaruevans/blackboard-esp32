#ifndef ESP32_TYPES_H
#define ESP32_TYPES_H

#include "bq.h"
#include "net/types.h"

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

    struct {
        enum esp32wifistatus {
            WIFI_DISCONNECTED, WIFI_CONNECTED
        } wifi_status;

        char ssid[32];
        char psk[32];
    } wifi_state;

    void (*char_recv)(struct esp32state *); /*!< function called when a char has been received */

    void (*on_ready)(struct esp32state *); /*!< function called when the esp32 has booted */

    void (*on_netchange)(struct esp32state *); /*!< function called when there's been a change to the network */

    struct at_command *current_command; /*!< the current/last command sent or NULL if the esp32 is not busy */

    struct http_message *current_msg; /*!< the current/last http msg or NULL if there is none */
} ESP32;



#endif
