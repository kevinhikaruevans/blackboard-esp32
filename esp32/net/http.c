#include "http.h"

void http_create_request(struct esp32state *inst, struct http_message *msg, HttpMethod method, char *url) {
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

    msg->socket = &inst->sockets[1];

    socket_init(inst, msg->socket, SOCKET_TCP, domain, 80);

    msg->socket->parent = (void *) msg;

    // could probably use ptr arith instead of strlen here
    //memcpy(&msg->request.domain, domain, strlen(domain) + 1);
    //msg->request.port = 80; //TODO
    msg->request.uri[0] = '/';
    msg->request.uri[1] = '\0';

    if (uri != NULL) {
        memcpy(&msg->request.uri[1], uri, strlen(uri) + 1);
    }

    msg->request.method = method;
    msg->request.headers[0][0] = '\0';
    msg->on_success = NULL;
}

void http_execute(struct esp32state *inst, struct http_message *msg) {
    char buffer[256];
    socket_set_receive(msg->socket, &http_on_receive);
    socket_open(inst, msg->socket);
    //TODO: add headers
    int length = sprintf(buffer,
            "%s %s HTTP/1.1\r\n"
                    "Host: %s\r\n"
                    "User-Agent: blackboard-esp32\r\n"
                    "\r\n",
            HTTP_METHODS[msg->request.method],
            msg->request.uri,
            msg->socket->host);
    socket_send(inst, msg->socket, buffer, length);
}

void http_on_receive(struct esp32state *inst, struct at_socket *socket) {
    socket_close(inst, socket);

    struct http_message * msg = (struct http_message *) socket->parent;
    char *line = strtok(socket->buffer, "\n");

    // hopefully optimized out:
    char *responseCode = line + sizeof("HTTP/1.1");
    responseCode[3] = '\0';

    msg->response.response_code = atoi(responseCode);

    line = strtok(NULL, "\n");
    int headerIndex = 0;

    while (line != NULL) {
        int headerLineLength = strlen(line);

        // just break if it's hit the \r\n\r\n:
        if (headerLineLength <= 1)
            break;

        if (headerIndex < 8) {
            if (line[headerLineLength - 1] == '\r')
                line[headerLineLength - 1] = '\0';

            memcpy(&msg->response.headers[headerIndex++], line, headerLineLength + 1);
        }

        line = strtok(NULL, "\n");
    }

    msg->response.headers[headerIndex][0] = '\0';
    msg->response.body = strtok(NULL, "\0");

    if (msg->on_success != NULL) {
        msg->on_success(inst, msg);
    }
}
