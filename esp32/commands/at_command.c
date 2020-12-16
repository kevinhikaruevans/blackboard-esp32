#include "at_command.h"

/**
 * A slow, linear search of ESP32 commands.
 *
 * IDEALLY: this should be a hashtable but I'm not sure if I should just
 * redo the project as C++ and use an unordered_map...
 */
struct at_command *esp32_lookup_command(struct esp32state *inst,
                                        const char *       line) {
  const int size = sizeof(esp32_commands) / sizeof(ATCommand);
  char *    eq   = strchr(line, '=');

  if (eq != NULL && *(eq + 1) != '\0') {
    /**
     * lil bit questionable, but this will search for a '='
     * i.e. for a set command
     * and replace the next char with the end of string
     *
     * this makes a command like `AT+CWJAP="myssid",...`
     * map to `AT+CWJAP=`
     */
    *(eq + 1) = '\0';
  }

  for (int i = 0; i < size; i++) {
    ATCommand *cmd = &esp32_commands[i];

    if (strcmp(cmd->name, line) == 0) {
      return cmd;
    }
  }

  return NULL;
}

// maybe unneeded:
bool esp32_is_response_end(const char *line) {
  return (strcmp("OK", line) == 0) || (strcmp("ERROR", line) == 0);
}

/**
 * Calls the corresponding handler for a line received (or
 * displays an error if no matching handler is found).
 * @param inst The ESP32 instance
 * @param line The line received
 */
void esp32_handle_line(struct esp32state *inst, const char *line) {
  if (line == NULL || line[0] == '\0') {
    // ignore empty strings
    return;
  }
  // xil_printf(">> %s\r\n", line);

  if (inst->current_command == NULL) {
    if (inst->boot_status == ESP32_STATUS_READY) {
      if (strstr(line, "+IPD") == line) {
        // esp32_handle_socket_recv(inst, line);
        // esp32_handle_ipd(inst, line);

      } else {
        esp32_handle_unexpected(inst, line);
      }
    } else {
      xil_printf("[BOOT] '%s'\r\n", line);

      if (strcmp("ready", line) == 0) {
        // esp32 printed "ready", update status
        inst->boot_status = ESP32_STATUS_READY;
        xil_printf("[ ESP] module is ready!\r\n");

        if (inst->on_ready != NULL) {
          inst->on_ready(inst);
        }
      }
    }
  } else {
    xil_printf("[RECV] (cb:%s) '%s'\r\n", inst->current_command->name, line);
    ATCommandStatus status = inst->current_command->callback(inst, line);

    switch (status) {
    default:
    case COMMAND_DONE:
      /* unset the current command if it's done */
      inst->current_command = NULL;
      break;

    case COMMAND_WAITING:
      /* do nothing if it's still waiting */
      break;
    }
  }
}

/**
 * A function to handle an unexpected command response
 * @param inst The ESP32 instance
 * @param line The unexpected line
 */
void esp32_handle_unexpected(struct esp32state *inst, const char *line) {
  xil_printf("[ ERR] Unexpected line: '%s'\r\n", line);
}
