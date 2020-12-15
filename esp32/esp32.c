#include "esp32.h"

/**
 * Fully initializes the ESP32 and enables it.
 * @param inst The esp32state instance
 */
void esp32_init(struct esp32state *inst) {
    esp32_state_init(inst);
    esp32_interrupts_config(inst);
    esp32_enable_uart(inst);
    esp32_enable_module(inst);
}

/**
 * Resets the ESP32 state to an empty one, defaulting to `off'.
 * @param inst The instance to initialize.
 */
void esp32_state_init(struct esp32state *inst) {
    inst->boot_status = ESP32_STATUS_OFF;
    inst->char_recv = &esp32_char_recv;
    inst->on_receive = &esp32_on_receive;
    inst->on_ready = NULL;
    inst->on_error = NULL;
    inst->on_netchange = NULL;
    inst->current_command = NULL;
    inst->active_socket = NULL;
    inst->is_raw_transmit = false;

    bq_init(&inst->rx_queue);
    bq_init(&inst->tx_queue);

    wifi_init(inst);
}

/**
 * Sets the callback function, called when the ESP32 is fully booted.
 * @param inst The ESP32 state instance
 * @param on_ready The callback function called when the ESP32 is booted.
 */
void esp32_set_ready(struct esp32state *inst, void (*on_ready)(struct esp32state *)) {
    inst->on_ready = on_ready;
}

void esp32_set_receive(struct esp32state *inst, void (*on_receive)(struct esp32state *, char)) {
    inst->on_receive = on_receive;
}

/**
 * Called by the ISR when a character is received.
 * @param inst The ESP32 state instance
 */
void esp32_char_recv(struct esp32state *inst) {
    char c = *((uint32_t *)UART0_FIFO_ADDR);

    //esp32_on_receive(inst, c);
    inst->on_receive(inst, c);

    esp32_clear_interrupts();
}

void esp32_on_receive(struct esp32state *inst, char c) {
    char *line = bq_putc(&inst->rx_queue, c);

    if (line != NULL && strstr(line, "+IPD") == line) {
        esp32_handle_ipd(inst, line);
    }


}
/**
 * Clears ALL interrupts (on UART0)
 */
void esp32_clear_interrupts() {
    UART0_ISR = 0xFFFFFFFF; // clear all
}

/**
 * Processes the queue
 * TODO rename the function? `run' implies too much, whereas process is more apt?
 * @param inst The ESP32 state instance
 */
void esp32_run_queue(struct esp32state *inst) {
	// handle receiving:
	char *line = bq_dequeue(&inst->rx_queue);

	if (line != NULL) {
		esp32_handle_line(inst, line);
	}

	// handle transmitting:
    if (inst->current_command == NULL || inst->is_raw_transmit) {
		// only transmit when there's not a command waiting
		line = bq_dequeue(&inst->tx_queue);

		if (line != NULL) {
			esp32_println(inst, line);
		}
	}

    // handle active socket:
    if (inst->active_socket != NULL) {
        if (inst->active_socket->receivedBytesRemaining == 0) {
            // done receiving, remove from being the active socket and call a receive() function
            if (inst->active_socket->on_receive != NULL) {
                inst->active_socket->on_receive(inst, inst->active_socket);
            }

            inst->active_socket = NULL;
        }
    }
}

/**
 * Prints a line to the ESP32 over UART0.
 *
 * @param inst The ESP32 state instance
 * @param buffer The buffer to write
 * @returns The number of bytes transmitted
 */
int esp32_println(struct esp32state *inst, char *buffer) {
    int i = 0;
    int length = strlen(buffer);
    char c;

    xil_printf("[ TX] '%s'\r\n", buffer);

    for(; i < length; i++) {
        c = buffer[i];
        XUartPs_SendByte(UART0_CTRL_ADDR, c);
    }

    // TODO: swap out the Xilinx functions and just write to the TX FIFO addr
    XUartPs_SendByte(UART0_CTRL_ADDR, '\r');
    XUartPs_SendByte(UART0_CTRL_ADDR, '\n');

    if (inst->is_raw_transmit) {
        xil_printf("[ TX] raw send, length %d\r\n", length);
    } else {
        if (inst->current_command == NULL) {
            inst->current_command = esp32_lookup_command(inst, buffer);

            if (inst->current_command == NULL) {
                xil_printf("[ ERR] failed to map '%s' to a command!\r\n", buffer);
            }
        } else {
            xil_printf("[ ERR] attempt to write over current_command with '%s'\r\n", buffer);
            xil_printf("[ ERR] +-> current command = %s\r\n", inst->current_command->name);
        }
    }
    return i + 2;
}

/**
 * Configure and enable the ESP32 module
 * @param inst The ESP32 state instance
 */
void esp32_enable_module(struct esp32state *inst) {
    // unlock system level control
    SLCR_UNLOCK = UNLOCK_KEY;

    // config MIO pins for BOOT0/EN
    // - unbuffered
    //     -> not actually sure what unbuffered means in this context
    //        does it mean there's no buffer `amp'?
    // - lvcmos 3.3
    *((uint32_t*) MIO_ESP32_BOOT0) = 0x00001600;
    *((uint32_t*) MIO_ESP32_EN)    = 0x00001600;

    // relock SCLR
    SLCR_LOCK = LOCK_KEY;

    // set GPIO direction and output enable for those two MIO pins
    GPIO_DIRM(0) = ESP32_MASK;
    GPIO_OEN(0) |= ESP32_MASK;

    // assert EN <= high
    GPIO_DATA(0) |= (1 << 26);

    inst->boot_status = ESP32_STATUS_INIT;
}

/**
 * Configure and enable UART0
 * @param inst The ESP32 state instance
 */
void esp32_enable_uart(struct esp32state *inst) {
    /*
     * could use Xilinx's libs for this bit, or we could
     * just use the normal ptr things
     */

    // reset rx/tx
    *(uint32_t *)UART0_CTRL_ADDR = 0b11;

    while((*(uint32_t *)UART0_CTRL_ADDR) == 0b11)
    { /* wait for reset, should implement a counter here for a timeout */ }

    /**
     * mode
     * 00  - normal uart mode
     * 00  - 1 stop bit
     * 1xx - no parity
     * 0x  - 8 bit chars
     * 0   - clk src is uart_ref_clk
     */
    *(uint32_t *)UART0_MODE_ADDR = 0b0000100000;

    // enable rx/tx
    *(uint32_t *)UART0_CTRL_ADDR = 0b010100;

    // set baudrate to 115200
    *(uint32_t *)UART0_BAUDGEN_ADDR = 0x7C;
    *(uint32_t *)UART0_BAUDDIV_ADDR = 6;

    UART0_RXTG = 1;   // set trigger to 1 byte
    UART0_IER  = 0x1; // enable
    UART0_ISR  = 0x1; // clear rx trig interrupt:
    //UART0_IDR = 1;
}

void esp32_handle_ipd(struct esp32state *inst, const char *line) {
    /* +IPD,<link_id>,<len> */
    strtok(line, ",");
    int linkId = atoi(strtok(NULL, ","));
    int length = atoi(strtok(NULL, ":"));

    xil_printf("[ IPD] socket data received, link: %d, length: %d\r\n", linkId, length);
    struct at_socket * socket = &inst->sockets[linkId];

    socket->receivedBytesRemaining = length;

    inst->active_socket = socket;
    esp32_set_receive(inst, &esp32_handle_socket_recv);
}

void esp32_handle_socket_recv(struct esp32state *inst, char c) {
    struct at_socket * socket = inst->active_socket;

    socket->buffer[socket->buffer_size++] = c;

    //TODO check for overflow
    if (--socket->receivedBytesRemaining == 0) {
        socket->buffer[socket->buffer_size] = '\0';
        esp32_set_receive(inst, &esp32_on_receive);

        //xil_printf("received all data: %s\r\n", socket->buffer);

    } else if (socket->receivedBytesRemaining < 50) {
        //xil_printf("%d.", socket->receivedBytesRemaining);
    }

}

