#include "esp32.h"

/**
 * Initializes the ESP32 state.
 */
void esp32_state_init(ESP32 *inst) {
    inst->boot_status        = ESP32_STATUS_OFF;
    inst->char_recv          = &esp32_char_recv;
    inst->on_ready           = NULL;
    inst->current_command    = NULL;

    bq_init(&inst->rx_queue);
}

/**
 * Sets the callback function, for when the ESP32 is fully booted.
 */
void esp32_set_ready(ESP32 *inst, void (*on_ready)(ESP32 *)) {
    inst->on_ready = on_ready;
}

/**
 * Called by the ISR when a character is received.
 */
void esp32_char_recv(ESP32 *inst) {
    char c = *((uint32_t *)UART0_FIFO_ADDR);

    bq_putc(&inst->rx_queue, c);

    esp32_clear_interrupts();
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
 */
void esp32_run_queue(ESP32 *inst) {
	char *line = bq_dequeue(&inst->rx_queue);

	if (line != NULL) {
		esp32_handle_line(inst, line);
	}
}

/**
 * Prints a line to the ESP32 over UART0.
 *
 * @param inst
 * @param buffer The buffer to write
 */
int esp32_println(ESP32 *inst, char *buffer) {
    int i = 0;
    int length = strlen(buffer);
    char c;

    for(; i < length; i++) {
        c = buffer[i];
        XUartPs_SendByte(UART0_CTRL_ADDR, c);
    }

    // TODO: swap out the Xilinx functions and just write to the TX FIFO addr
    XUartPs_SendByte(UART0_CTRL_ADDR, '\r');
    XUartPs_SendByte(UART0_CTRL_ADDR, '\n');
    inst->current_command = esp32_lookup_command(inst, buffer);

    if (inst->current_command == NULL) {
        xil_printf("[ ERR] failed to map '%s' to a command!\r\n", buffer);
    }

    return i;
}

/**
 * Configure and enable the ESP32 module
 */
void esp32_enable_module(ESP32 *inst) {
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
 */
void esp32_enable_uart(ESP32 *inst) {
    /*
     * could use Xilinx's libs for this bit, or we could
     * just use the normal ptr things
     */

    // reset rx/tx
    *(uint32_t *)UART0_CTRL_ADDR = 0b11;

    while((*(uint32_t *)UART0_CTRL_ADDR) == 0b11)
    { /* wait for reset */ }

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

