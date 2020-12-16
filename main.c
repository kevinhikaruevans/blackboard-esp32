#include <xil_printf.h>

#include "esp32/esp32.h"
#include "esp32/net/http.h"
#include "esp32/net/wifi.h"

#include "esp32/net/socket.h"
#include "wifi_config.h"

void on_success(struct esp32state *inst, struct http_message *client) {
  xil_printf("Received data: '%s'\r\n", client->response.body);
}

void on_connect(struct esp32state *inst) {
  static struct http_message msg;

  http_create_request(inst, &msg, HTTP_GET,
                      "http://worldtimeapi.org/api/ip.txt");

  msg.on_success = &on_success;
  http_execute(inst, &msg);
}

void esp32_ready(struct esp32state *inst) {
  inst->wifi_state.on_connect = &on_connect;
  wifi_connect(inst, WIFI_SSID, WIFI_PSK);
}

int main() {
  struct esp32state esp32;
  esp32_init(&esp32);
  esp32_set_ready(&esp32, &esp32_ready);

  while (1) {
    esp32_run_queue(&esp32);
  }
  return 0;
}
