#ifndef ESP32_COMMANDS_AT_COMMAND_H
#define ESP32_COMMANDS_AT_COMMAND_H

#include <ctype.h>
#include <string.h>
#include "xil_printf.h"
#include "at_callbacks.h"
#include "types.h"
#include "../types.h"
#include <stdbool.h>

struct at_command* esp32_lookup_command(struct esp32state *inst, const char *line);
bool esp32_is_response_end(const char *line);
void esp32_handle_line(struct esp32state *inst, const char *line);
void esp32_handle_unexpected(struct esp32state *inst, const char *line);

#endif
