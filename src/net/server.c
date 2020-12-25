#include "net/server.h"

void server_create(struct esp32state *inst, int port, bool ssl) {
  char buffer[64];
  if (ssl) {
    sprintf(buffer, "AT+CIPSERVER=1,%d,\"SSL\",0", port);
  } else {
    sprintf(buffer, "AT+CIPSERVER=1,%d", port);
  }
  bq_enqueue(&inst->tx_queue, buffer);
}


