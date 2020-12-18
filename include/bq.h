#ifndef ESP32_BQ_H
#define ESP32_BQ_H

#include "ctype.h"
#include "xil_printf.h"
#include <string.h>

#define BQ_BUFFER_SIZE     256
#define BQ_SCROLLBACK_SIZE 32
#define BQ_IDX_INACTIVE    -1

/**
 * A simple rx/tx queue using a circular buffer of strings.
 */
struct bufferqueue {
  volatile char buffers[BQ_SCROLLBACK_SIZE]
                       [BQ_BUFFER_SIZE]; /*!< Arrays of strings */
  volatile int char_index;   /*!< The index of the character in a line buffer,
                                i.e. the column. */
  volatile int buffer_index; /*!< The index of the enqueing */
  volatile int queue_buffer_index; /*!< The index of the dequeuing */
};

void  bq_init(struct bufferqueue *);
char *bq_putc(struct bufferqueue *, char);
void  bq_enqueue(struct bufferqueue *, const char *);
char *bq_dequeue(struct bufferqueue *);

#endif
