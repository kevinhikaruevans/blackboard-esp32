#include "gic.h"

void arm_disable_irq() {
  uint32_t cpsr_val = 0;
  asm("mrs %0, cpsr\n" : "=r"(cpsr_val));  // get current cpsr
  cpsr_val |= 0x80;                        // set the I bit (bit7)
  asm("msr cpsr, %0\n" : : "r"(cpsr_val)); // writeback modified value
}

void arm_enable_irq() {
  uint32_t cpsr_val = 0;
  asm("mrs %0, cpsr\n" : "=r"(cpsr_val));  // get current cpsr
  cpsr_val &= 0xFFFFFF7F;                  // clear the I bit (bit7)
  asm("msr cpsr, %0\n" : : "r"(cpsr_val)); // writeback modified value
}

void disable_interrupt59(void) {
  ICDIPTR(14) &= ~0x03000000;    // remove processors from interrupt
  ICDICER(1) = (1 << (59 - 32)); // disable interrupts
}

// sets interrupt sensitivity of interrupt52 to ‘sens’
void set_interrupt59_sensitivity(uint8_t sens) {
  ICDICFR(3) &= ~(0x3 << 22);
  ICDICFR(3) |= (sens & 0x3) << 22;
}

// sets the interrupt priority of 59 to ‘priority_val’
void set_interrupt59_priority(uint8_t priority_val) {
  ICDIPR(14) &= ~0xFF000000; // clear priority bits for interrupt 59
  ICDIPR(14) |= ((priority_val)&0xF8)
                << 24; // set top 5 bits based on passed value
}

// enables interrupt 59
void enable_interrupt59() {
  ICDIPTR(14) |= (1 << 24); // set bit 1 of ICDIPTR13 (enable for cpu0)
  ICDISER(1) = (1 << (59 - 32));
}

void esp32_interrupts_config(struct esp32state *instance) {
  arm_disable_irq();

  ICDDCR = 0;   // disable GIC distributor
  ICCICR = 0x3; // disable IRQ passthrough (use the GIC?)

  ICCPMR = 0xFF; // set GIC priority to the highest (0xFF)

  // UART0 IRQ ID = 59
  disable_interrupt59();
  set_interrupt59_priority(0xA0);
  set_interrupt59_sensitivity(1);
  enable_interrupt59();

  ICDDCR = 1; // reenable GIC distributor
  // XIL_EXCEPTION_ID_IRQ_INT = 5
  Xil_ExceptionRegisterHandler(5, irq_handler, instance);

  arm_enable_irq();
  // esp32_clear_interrupts();
}

void irq_handler(void *data) {
  uint32_t interrupt_ID = ICCIAR;

  if (interrupt_ID == 59) {
    struct esp32state *inst = data;
    if (data != NULL && inst->char_recv != NULL) {
      inst->char_recv(inst);
    }
  }
  ICCEOIR = interrupt_ID;
}
