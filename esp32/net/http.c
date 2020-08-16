#include "http.h"

void http_init(HttpMessage *msg) {

}

void http_create_request(ESP32 *inst, HttpMethod method, char *url) {
    HttpMessage *msg = inst->current_msg;
    char *protocol = strtok(url, "://");
    char *domain = strtok(NULL, "/");
    char *uri = strtok(NULL, "\0");

    /******
     * TODO strtok on domain, check for :
     * if : exists, set port
     */
    if (protocol == NULL || domain == NULL) {
        xil_printf("[HTTP] error: url is malformed\r\n");
        return;
    }

    if (strcmp("http", protocol) != 0) {
        xil_printf("[HTTP] error: only http is supported, not '%s'\r\n",
                protocol);
        return;
    }

    // could probably use ptr arith instead of strlen here
    memcpy(&msg->request.domain, domain, strlen(domain) + 1);
    msg->request.port = 80;
    msg->request.uri[0] = '/';

    if (uri != NULL) {
        memcpy(&msg->request.uri + 1, uri, strlen(uri) + 1);
    }
    msg->request.method = method;
    msg->request.headers[0][0] = '\0';
    msg->request.cip_started = false;
}

void http_resolve(ESP32 *inst) {
    char command[64];
    sprintf(command, "AT+CIPDOMAIN=\"%s\"", inst->current_msg->request.domain);
    bq_enqueue(&inst->tx_queue, command);
}

void http_send(ESP32 *inst) {
    /*
     * AT+CIPSTART
     * AT+CIPMODE
     * AT+CIPSEND
     * AT+CIPCLOSE
     */
    HttpMessage *msg = inst->current_msg;
    char buffer[64];
    char largeBuffer[256];

    sprintf(buffer, "AT+CIPSTART=\"TCP\",\"%s\",%d", /*msg->request.ip*/msg->request.domain, msg->request.port);

    bq_enqueue(&inst->tx_queue, buffer);
    bq_enqueue(&inst->tx_queue, "AT+CIPMODE=0");

    int length = sprintf(largeBuffer, "%s %s HTTP/1.1\r\nUser-Agent: ESP32\r\nHost: %s\r\n\r\n",
            HTTP_METHODS[msg->request.method],
            msg->request.uri,
            msg->request.domain);

    sprintf(buffer, "AT+CIPSEND=%d", length);
    bq_enqueue(&inst->tx_queue, buffer);
    bq_enqueue(&inst->tx_queue, largeBuffer); // will exceed buffer length. will need to do a raw send
    //bq_enqueue(&inst->tx_queue, "AT+CIPCLOSE");
    //bq_enqueue(&inst->tx_queue, "AT+CIPSEND=")
}

void http_recv();
