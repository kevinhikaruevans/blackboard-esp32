#include "http.h"

void http_init(HttpMessage *msg) {

}

void http_create_request(HttpMessage *msg, HttpMethod method, char *url) {
    char *protocol = strtok(url, "//");
    char *domain = strtok(NULL, "/");
    char *uri = strtok(NULL, "\0");

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
    memcpy(&msg->request.domain, domain, strlen(domain));

    msg->request.uri[0] = '/';
    if (uri != NULL) {
        memcpy(&msg->request.uri + 1, uri, strlen(uri));
    }
    msg->request.method = method;
    msg->request.headers[0][0] = '\0';
}

void http_resolve(ESP32 *inst, HttpMessage *msg) {
    char command[32];
    sprintf(command, "AT+CIPDOMAIN=\"%s\"", msg->request.domain);
    bq_enqueue(&inst->tx_queue, command);
}

void http_send(ESP32 *inst, HttpMessage *msg) {

}
