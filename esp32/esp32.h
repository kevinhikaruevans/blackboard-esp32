#ifndef ESP32_ESP32_H
#define ESP32_ESP32_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "xil_printf.h"
#include "xuartps_hw.h"
#include "commands/at_command.h"
#include "types.h"

/**
 * TODO: make these consistent
 * 	-> naming
 * 	-> style (addr vs macro ptr)
 */
#define UART0_CTRL_ADDR        0xE0000000
#define UART0_MODE_ADDR        0xE0000004
#define UART0_FIFO_STATUS_ADDR 0xE000002C
#define UART0_BAUDGEN_ADDR     0xE0000018
#define UART0_BAUDDIV_ADDR     0xE0000034
#define UART0_FIFO_ADDR        0xE0000030
#define UART0_IER              *((uint32_t *) 0xE0000008)
#define UART0_IDR              *((uint32_t *) 0xE000000C)
#define UART0_ISR              *((uint32_t *) 0xE0000014)
#define UART0_RXTG             *((uint32_t *) 0xE0000020)

#define SLCR_LOCK              *((uint32_t *) 0xF8000004)
#define SLCR_UNLOCK            *((uint32_t *) 0xF8000008)
#define UNLOCK_KEY             0xDF0D
#define LOCK_KEY               0x767B

#define MIO_CONF_BASEADDR      0xF8000700
#define MIO_OFFSET_ESP32_BOOT0 0x4 * 25
#define MIO_OFFSET_ESP32_EN    0x4 * 26

#define MIO_ESP32_BOOT0        (MIO_CONF_BASEADDR + MIO_OFFSET_ESP32_BOOT0)
#define MIO_ESP32_EN           (MIO_CONF_BASEADDR + MIO_OFFSET_ESP32_EN)
#define ESP32_MASK             ((1 << 25) | (1 << 26))

#define GPIO_DATA(n)           *(((uint32_t*) 0xE000A040) + n)
#define GPIO_DATA_RO(n)        *(((uint32_t*) 0xE000A060) + n)
#define GPIO_DIRM(n)           *(((uint32_t*) 0xE000A204) + 16*n)
#define GPIO_OEN(n)            *(((uint32_t*) 0xE000A208) + 16*n)


void esp32_enable_module(ESP32 *);
void esp32_enable_uart(ESP32 *);
void esp32_state_init(ESP32 *);
void esp32_char_recv(ESP32 *inst);
void esp32_set_ready(ESP32 *inst, void (*on_ready)(ESP32 *));

void esp32_clear_interrupts();
void esp32_run_queue(ESP32 *inst);
int esp32_println(ESP32 *inst, char *buffer);
char *esp32_prev_buffer(ESP32 *, char *buffer);

#endif
