#include "bq.h"

/**
 * Initializes a queue to an empty state.
 * @param bq The buffer queue to initialize
 */
void bq_init(struct bufferqueue *bq) {
  bq->buffer_index       = 0;
  bq->char_index         = 0;
  bq->queue_buffer_index = BQ_IDX_INACTIVE;
}

/**
 * Adds a single character to the queue. CRs and LFs will be treated
 * as null characters and will shift the buffer queue.
 * @param bq The target queue
 * @param c The character to append
 * @returns a non-NULL value if the command requires immediate processing.
 */
char *bq_putc(struct bufferqueue *bq, char c) {
  char *ret = NULL;

  if (bq == NULL)
    return ret;

  if (bq->char_index < BQ_BUFFER_SIZE - 1) {
    char *         line = bq->buffers[bq->buffer_index];
    volatile char *dest = &bq->buffers[bq->buffer_index][bq->char_index++];

    switch (c) {
    default:
      *dest = c;
      break;
    case '\r': // eh
    case '\n':
      *dest = '\0';
      break;
    }

    if (c == ':' && strstr(line, "+IPD") == line) {
      *dest = '\0';
      c     = '\n';
      ret   = line;
    }

    if (c == '\n') {
      bq->char_index = 0;
      bq->buffer_index++;

      // TODO check if buffer_index exceeds queue_buffer_index
      //     to ensure the ISR does not "lap" (or overwrite) the
      //     dequeueing loop...
      if (bq->buffer_index >= BQ_SCROLLBACK_SIZE) {
        bq->buffer_index = 0;
      }

      if (bq->queue_buffer_index == BQ_IDX_INACTIVE) {
        // this should be the only time that the ISR touches queue_buffer_index
        // sort-of acting as a once-changed semaphore (by the ISR to the normal
        // program)
        bq->queue_buffer_index = 0;
      }
    }
  } else {
    // TODO handle this later
    xil_printf("[ BQ] putc overflow!\n\r");
  }

  return ret;
}

/**
 * Appends a string to the queue and forces a newline.
 * @param bq The queue to add to
 * @param str The string to enqueue
 */
void bq_enqueue(struct bufferqueue *bq, const char *str) {
  if (bq == NULL || str == NULL) {
    return;
  }

  size_t         srclen = strlen(str) + 1; /* +1 for \0 */
  volatile char *dest   = NULL;

  /**
   * TODO
   * 1. consider whether or not to start at char_index or just to start at 0
   * 2. test out the limits and actually ensure it's within bounds
   */
  if ((bq->char_index + srclen) < BQ_BUFFER_SIZE - 1) {
    dest = &bq->buffers[bq->buffer_index][bq->char_index];

    memcpy((void *)dest, str, srclen);

    bq->char_index = 0;
    bq->buffer_index++;

    if (bq->buffer_index >= BQ_SCROLLBACK_SIZE) {
      bq->buffer_index = 0;
    }

    if (bq->queue_buffer_index == BQ_IDX_INACTIVE) {
      bq->queue_buffer_index = 0;
    }

  } else {
    // TODO handle this later
    xil_printf("[ BQ] enqueue overflow!\r\n");
    xil_printf("[ BQ] overflow data: [[%s]]\r\n", str);
  }
}

/**
 * Dequeues a line from a queue.
 * @param bq The queue to dequeue from
 * @returns the next line or NULL if the queue is empty
 */
char *bq_dequeue(struct bufferqueue *bq) {
  if (bq == NULL) {
    return NULL;
  }

  if (bq->queue_buffer_index == BQ_IDX_INACTIVE) {
    // no data has been received yet
    return NULL;
  }

  if (bq->buffer_index == bq->queue_buffer_index) {
    // caught up, hold off until new data has been received
    return NULL;
  }

  char *line = (char *)bq->buffers[bq->queue_buffer_index++];

  if (bq->queue_buffer_index >= BQ_SCROLLBACK_SIZE) {
    bq->queue_buffer_index = 0;
  }

  return line;
}

/**
 * Gets the previous line in the buffer, sort-of a reverse-`peek()` method.
 * @param bq The buffer to rev-peek on
 * @param buffer The current line, i.e. the line to offset from.
 * @returns Returns the last line.
 */
char *bq_prev_buffer(struct bufferqueue *bq, char *buffer) {
  if (buffer <= bq->buffers[0]) {
    return (char *)&bq->buffers[BQ_SCROLLBACK_SIZE - 1];
  }
  return buffer - BQ_BUFFER_SIZE;
}
