#ifndef ESP32_COMMANDS_TYPES_H
#define ESP32_COMMANDS_TYPES_H

#include "../types.h"

#define AT_CMD_NAME_SIZE 24
#define AT_CMD_DESC_SIZE 32

typedef enum {
    ESP32_AT_AT,
    ESP32_AT_RST,
    ESP32_AT_GMR
} AT_COMMAND_TYPE;

typedef struct at_command {
    AT_COMMAND_TYPE type;
    char name[AT_CMD_NAME_SIZE];
    char desc[AT_CMD_DESC_SIZE];
    void (*callback)(ESP32 *, const char *);
} ESP32_AT_Command;

#endif
