#ifndef ESP32_COMMANDS_AT_COMMAND_H
#define ESP32_COMMANDS_AT_COMMAND_H

#include "../esp32.h"
#include "../types.h"
#include "at_callbacks.h"
#include "types.h"
#include "xil_printf.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

struct at_command *esp32_lookup_command(struct esp32state *inst,
                                        const char *       line);
bool               esp32_is_response_end(const char *line);
void               esp32_handle_line(struct esp32state *inst, const char *line);
void esp32_handle_unexpected(struct esp32state *inst, const char *line);

#endif
