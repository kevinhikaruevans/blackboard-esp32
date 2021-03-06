#ifndef ESP32_COMMANDS_AT_CALLBACKS_H
#define ESP32_COMMANDS_AT_CALLBACKS_H

#include <ctype.h>

#include "../types.h"
#include "types.h"

extern struct at_command esp32_commands[10];

ATCommandStatus esp32_callback_default(struct esp32state *inst,
                                       const char *       resp);
ATCommandStatus esp32_callback_at(struct esp32state *, const char *);
ATCommandStatus esp32_callback_gmr(struct esp32state *, const char *);
ATCommandStatus esp32_callback_cwjap_set(struct esp32state *inst,
                                         const char *       resp);
ATCommandStatus esp32_callback_cipdomain_set(struct esp32state *inst,
                                             const char *       resp);
ATCommandStatus esp32_callback_cipstart_set(struct esp32state *inst,
                                            const char *       resp);
ATCommandStatus esp32_callback_cipmode_set(struct esp32state *inst,
                                           const char *       resp);
ATCommandStatus esp32_callback_cipsend_set(struct esp32state *inst,
                                           const char *       resp);

#endif
