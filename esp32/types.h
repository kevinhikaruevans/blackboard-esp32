#ifndef ESP32_TYPES_H
#define ESP32_TYPES_H

#include "bq.h"
#include "net/types.h"

enum esp32_subsystem { ESP32_GLOBAL, ESP32_WIFI };
/**
 * A container to hold the state of the ESP32 and
 * the associated callback functions.
 */
struct esp32state {
  struct bufferqueue rx_queue; /*!< queue for data received */
  struct bufferqueue tx_queue; /*!< queue for commands to send */

  enum esp32bootstatus {
    ESP32_STATUS_OFF,
    ESP32_STATUS_INIT,
    ESP32_STATUS_READY
  } boot_status; /*!< the approx. status of the esp32 */

  struct {
    enum esp32wifistatus { WIFI_DISCONNECTED, WIFI_CONNECTED } wifi_status;

    char ssid[32];
    char psk[32];

    void (*on_connect)(struct esp32state *);
  } wifi_state;

  struct at_socket sockets[5];

  struct at_socket *active_socket;

  bool is_raw_transmit; /*!< need to rename this, but true if we're doing a raw
                           (non-command) send */

  void (*char_recv)(struct esp32state *); /*!< function called when a char has
                                             been received */

  void (*on_receive)(struct esp32state *, char);

  void (*on_error)(struct esp32state *, enum esp32_subsystem, int);

  // void (*line_recv)(struct esp32state *);

  void (*on_ready)(
      struct esp32state *); /*!< function called when the esp32 has booted */

  void (*on_netchange)(struct esp32state *); /*!< function called when there's
                                                been a change to the network */

  struct at_command *current_command; /*!< the current/last command sent or NULL
                                         if the esp32 is not busy */

  struct http_message
      *current_msg; /*!< the current/last http msg or NULL if there is none */
};

#endif
