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
    volatile char buffers[BQ_SCROLLBACK_SIZE][BQ_BUFFER_SIZE]; /*!< Arrays of strings */
    volatile int  char_index;                                  /*!< The index of the character in a line buffer, i.e. the column. */
    volatile int  buffer_index;                                /*!< The index of the enqueing */
    volatile int  queue_buffer_index;                          /*!< The index of the dequeuing */
} BufferQueue;

void bq_init(BufferQueue *);
char * bq_putc(BufferQueue *bq, char c);
void bq_enqueue(BufferQueue *bq, const char *str);
char *bq_dequeue(BufferQueue *bq);

#endif
