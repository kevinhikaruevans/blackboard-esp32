#ifndef ESP32_GIC_H
#define ESP32_GIC_H

#include "esp32.h"
#include "xil_exception.h"
#include <ctype.h>

#define ICCPMR     *((uint32_t *)0xF8F00104) // Priority mask reg
#define ICCICR     *((uint32_t *)0xF8F00100) // CPU interface control reg
#define ICDDCR     *((uint32_t *)0xF8F01000) // Distributor Control reg
#define ICCIAR     *((uint32_t *)0xF8F0010C)
#define ICCEOIR    *((uint32_t *)0xF8F00110)
#define ICDISER(n) *(((uint32_t *)0xF8F01100) + n) // Int. set enable reg
#define ICDICER(n) *(((uint32_t *)0xF8F01180) + n) // Int. Clear Enable reg
#define ICDIPR(n)  *(((uint32_t *)0xF8F01400) + n) // Int. Priority reg
#define ICDIPTR(n) *(((uint32_t *)0xF8F01800) + n) // Int. Processor Targets reg
#define ICDICFR(n) *(((uint32_t *)0xF8F01C00) + n) // Int. Configuration reg

void arm_disable_irq();
void arm_enable_irq();
void esp32_interrupts_config(struct esp32state *);
void irq_handler();

#endif
