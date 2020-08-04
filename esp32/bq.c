#include "bq.h"

void bq_init(BufferQueue *bq) {
	bq->buffer_index = 0;
	bq->char_index = 0;
	bq->queue_buffer_index = BQ_IDX_INACTIVE;
}

void bq_putc(BufferQueue *bq, const char c) {
	if (bq == NULL)
		return;

    if (bq->char_index < BQ_BUFFER_SIZE - 1) {
        volatile char* dest = &bq->buffers[bq->buffer_index][bq->char_index++];

        switch(c) {
        default:
            *dest = c;
            break;
        case '\r':
        case '\n':
            *dest = '\0';
            break;
        }

        if (c == '\n') {
            bq->char_index = 0;
            bq->buffer_index++;

            //TODO check if buffer_index exceeds queue_buffer_index
            //     to ensure the ISR does not "lap" (or overwrite) the
            //     dequeueing loop...
            if (bq->buffer_index >= BQ_SCROLLBACK_SIZE) {
                bq->buffer_index = 0;
            }

            if (bq->queue_buffer_index == BQ_IDX_INACTIVE) {
                // this should be the only time that the ISR touches queue_buffer_index
                // sort-of acting as a once-changed semaphore (by the ISR to the normal program)
                bq->queue_buffer_index = 0;
            }
        }
    } else {
        // TODO handle this later
        xil_printf("[ BQ] overflow!\n\r");
    }
}

void bq_enqueue(BufferQueue *bq, const char *str) {
	if (bq == NULL || str == NULL) {
		return;
	}

	size_t srclen       = strlen(str) + 1; /* +1 for \0 */
	volatile char* dest = NULL;

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
        xil_printf("[ BQ] overflow!\n\r");
    }
}

char* bq_dequeue(BufferQueue *bq) {
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

    if(bq->queue_buffer_index >= BQ_SCROLLBACK_SIZE) {
        bq->queue_buffer_index = 0;
    }

    return line;
}

/**
 * Gets the previous line in the buffer.
 *
 * @param bq The buffer to rev-peek on
 * @param buffer The current line, i.e. the line to offset from.
 */
char *bq_prev_buffer(BufferQueue *bq, char *buffer) {
    if (buffer <= bq->buffers[0]) {
        return (char *)&bq->buffers[BQ_SCROLLBACK_SIZE - 1];
    }
    return buffer - BQ_BUFFER_SIZE;
}
