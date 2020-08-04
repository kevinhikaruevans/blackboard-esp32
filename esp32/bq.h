#ifndef ESP32_BQ_H
#define ESP32_BQ_H

#include "xil_printf.h"
#include "ctype.h"
#include <string.h>

#define BQ_BUFFER_SIZE     256
#define BQ_SCROLLBACK_SIZE 32
#define BQ_IDX_INACTIVE    -1

/**
 * A simple rx/tx queue using a circular buffer of strings.
 */
typedef struct bufferqueue {
    volatile char buffers[BQ_SCROLLBACK_SIZE][BQ_BUFFER_SIZE];
    volatile int  char_index;
    volatile int  buffer_index;
    volatile int  queue_buffer_index;
} BufferQueue;

void bq_init(BufferQueue *);
void bq_putc(BufferQueue *bq, const char c);
void bq_enqueue(BufferQueue *bq, const char *str);
char *bq_dequeue(BufferQueue *bq);

#endif
