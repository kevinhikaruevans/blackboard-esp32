#ifndef ESP32_COMMANDS_TYPES_H
#define ESP32_COMMANDS_TYPES_H

#include "../types.h"

#define AT_CMD_NAME_SIZE 24
#define AT_CMD_DESC_SIZE 32

/*
 * Things to consider:
 * - merging set/query commands to a single at_command instance
 * - have 2 or 3 callback functions in at_command
 * - handler depends on whether or not the line contains a `=` or `?`
 *      - e.g. if you do `AT+CWJAP`, it'll call the 'set' callback in `at_command`
 * - DUNNO
 */

typedef enum at_command_status {
    COMMAND_WAITING,
    COMMAND_DONE
} ATCommandStatus;

/**
 * Container for commands and their associated callback functions
 */
typedef struct at_command {
    char name[AT_CMD_NAME_SIZE]; /*!< The command itself, e.g. AT or AT+CWJAP */
    ATCommandStatus (*callback)(struct esp32state *, const char *); /*!< The callback function for the command */
} ATCommand;

#endif
