
#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#include <SerialCommands.h>

#define CMD_ON    "ON"
#define CMD_OFF   "OFF"
#define CMD_READ  "READ"
#define CMD_ALL   "ALL"
#define CMD_MODE  "MODE"
#define ERROR_RESP  "ERROR"


void cmd_unrecognized(SerialCommands* sender, const char* cmd) {
    sender->GetSerial()->print(F("Unrecognized command ["));
    sender->GetSerial()->print(cmd);
    sender->GetSerial()->println(']');
}

#endif
