#define SERIAL_COMMANDS_DEBUG

#include <Arduino.h>
#include "definitions.h"
#include <SerialCommands.h>

char serial_command_buffer_[32];
byte pinModes[19];
//int pin_modes[] = {OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, INPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT};

SerialCommands command(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");

void cmd_on(SerialCommands* sender, const char* cmd);
void cmd_off(SerialCommands* sender, const char* cmd);
void cmd_mode(SerialCommands* sender, const char* cmd);
void cmd_read(SerialCommands* sender, const char* cmd);
void cmd_all(SerialCommands* sender);
void handleCommand(SerialCommands* sender, const char* cmd);

SerialCommand cmd_on_(CMD_ON, cmd_on);
SerialCommand cmd_off_(CMD_OFF, cmd_off);
SerialCommand cmd_mode_(CMD_MODE, cmd_mode);
SerialCommand cmd_read_(CMD_READ, cmd_read);
SerialCommand cmd_all_(CMD_ALL, cmd_all);
SerialCommand handleCommand_(CMD, handleCommand);

void setup(){
    for (int i = 2; i < 14; i++)
        pinMode(i, (i == 7 ? INPUT : OUTPUT));
    Serial.begin(9600);
    command.SetDefaultHandler(cmd_unrecognized);
    command.AddCommand(&cmd_on_);
    command.AddCommand(&cmd_off_);
    command.AddCommand(&cmd_mode_);
    command.AddCommand(&cmd_read_);
    command.AddCommand(&cmd_all_);
    command.AddCommand(&handleCommand_);
    Serial.println(F("Ready!"));
}

void loop(){
    command.ReadSerial();
}

