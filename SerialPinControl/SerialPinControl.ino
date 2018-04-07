#define SERIAL_COMMANDS_DEBUG

#include <Arduino.h>
#include "definitions.h"
#include <SerialCommands.h>

Pins pins[] = {
    Pins(1, 2, "LOW_BATT"),
    Pins(1, 3, "TEST_MODE"),
    Pins(1, 5, "REMOTE"),
    Pins(1, 6, "PAD"),
    Pins(0, 7, "RELAY_IN"),
    Pins(1, 8, "OKC"),
    Pins(1, 9, "AC_EN"),
    Pins(1, 10, "QW_EN"),
    Pins(1, 13, "TEST_IND"),
    Pins(1, 11, "BATT_EN"),
    Pins(1, 12, "RELAY4")
};

char serial_command_buffer_[32];

SerialCommands command(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");

void cmd_on(SerialCommands* sender, const char* cmd);
void cmd_off(SerialCommands* sender, const char* cmd);
void cmd_mode(SerialCommands* sender, const char* cmd);
void cmd_read(SerialCommands* sender, const char* cmd);
void cmd_all(SerialCommands* sender, const char* cmd);
void handleCommand(SerialCommands* sender, const char* cmd);

SerialCommand cmd_on_(CMD_ON, cmd_on);
SerialCommand cmd_off_(CMD_OFF, cmd_off);
SerialCommand cmd_mode_(CMD_MODE, cmd_mode);
SerialCommand cmd_read_(CMD_READ, cmd_read);
SerialCommand cmd_all_(CMD_ALL, cmd_all);
SerialCommand handleCommand_(CMD, handleCommand);

void setup() {
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
    Serial.println(findPin("cool").name);
    printAll(&pins);

    Pins p = findPin(&pins, "QW_EN");
    Serial.println(String(p.name) + ", " + p.pin);
}

void loop() {
    command.ReadSerial();
}

void checkPins(char* p) {

}



Pins findPin(const char* n)
{
    for(int i = 0; i <= 11; i++)
        if( pins[i].name == n)
            return pins[i];
    return Pins();
}




