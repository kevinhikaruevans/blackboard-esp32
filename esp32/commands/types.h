#ifndef ESP32_COMMANDS_TYPES_H
#define ESP32_COMMANDS_TYPES_H

#include "../types.h"

#define AT_CMD_NAME_SIZE 24
#define AT_CMD_DESC_SIZE 32

typedef enum {
    ESP32_AT_AT,
    ESP32_AT_RST,
    ESP32_AT_GMR,
    ESP32_AT_CWMODE_QUERY,
	ESP32_AT_CWMODE_SET,
	ESP32_AT_CWJAP_QUERY,
	ESP32_AT_CWJAP_SET,
} AT_COMMAND_TYPE;

/*
 * Things to consider:
 * - merging set/query commands to a single at_command instance
 * - have 2 or 3 callback functions in at_command
 * - handler depends on whether or not the line contains a `=` or `?`
 *      - e.g. if you do `AT+CWJAP`, it'll call the 'set' callback in `at_command`
 * - DUNNO
 */

/**
 * Container for commands and their associated callback functions
 */
typedef struct at_command {
    AT_COMMAND_TYPE type; /*!< The command type */
    char name[AT_CMD_NAME_SIZE]; /*!< The command itself, e.g. AT or AT+CWJAP */
    char desc[AT_CMD_DESC_SIZE]; /*!< Probably unneeded, but a basic description of what the command does */
    void (*callback)(ESP32 *, const char *); /*!< The callback function for the command */
} ESP32_AT_Command;

#endif
