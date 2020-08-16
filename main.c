#include <stdio.h>
#include "xil_io.h"
#include "xil_types.h"
#include "xparameters.h"
#include "xil_io.h"
#include "xil_exception.h"
#include "xscugic.h"

//#include "esp32.h"
#include "esp32/gic.h"
#include "esp32/esp32.h"
#include "esp32/net/http.h"
#include "sleep.h"

#include "esp32/net/socket.h"

void recv(ESP32 *inst, struct at_socket *sock) {
    xil_printf("Received data: '%s'\r\n", sock->buffer);

    socket_close(inst, sock);
}

void esp32_ready(ESP32 *inst) {
	bq_enqueue(&inst->tx_queue, "AT+CWMODE=3");
    bq_enqueue(&inst->tx_queue, "AT+CIPMODE=0");
    bq_enqueue(&inst->tx_queue, "AT+CIPMUX=1");
    bq_enqueue(&inst->tx_queue, "AT+CWJAP=\"dd-wrt\",\"\"");
    //bq_enqueue(&inst->tx_queue, "AT+CIPDOMAIN=\"www.khevans.com\"");

    //http_create_request(inst, HTTP_GET, "http://direct.khevans.com/test");
    //http_resolve(inst);
    //http_send(inst);


    //return;
    ATSocket *socket = &inst->sockets[1];

    socket_init(inst, socket, SOCKET_TCP, "direct.khevans.com", 80);

    char *data = "GET /test HTTP/1.1\r\nUser-Agent: esp32\r\nHost: direct.khevans.com\r\n\r\n";
    socket_set_receive(socket, recv);
    socket_open(inst, socket);
    socket_send(inst, socket, data, strlen(data));

}

int main()
{
    ESP32 esp32;
    esp32_state_init(&esp32);
    esp32_interrupts_config(&esp32);
    esp32_enable_uart(&esp32);
    esp32_enable_module(&esp32);
    esp32_set_ready(&esp32, &esp32_ready);

    while(1) {
        esp32_run_queue(&esp32);
    }
    return 0;
}



