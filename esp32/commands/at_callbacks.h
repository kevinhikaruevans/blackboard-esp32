#ifndef ESP32_COMMANDS_AT_CALLBACKS_H
#define ESP32_COMMANDS_AT_CALLBACKS_H

#include <ctype.h>

#include "types.h"
#include "../types.h"

extern struct at_command esp32_commands[4];
void esp32_callback_default(struct esp32state *inst, const char *resp);
void esp32_callback_at(struct esp32state *, const char *);
void esp32_callback_gmr(struct esp32state *, const char *);
void esp32_callback_cwjap_set(struct esp32state *inst, const char *resp);
#endif
