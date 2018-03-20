#define SERIAL_COMMANDS_DEBUG

#include <Arduino.h>
#include "definitions.h"
#include <SerialCommands.h>


const int kLedPin = 13;
const int relay3 = 12;
const int relay1 = 9;
const int relay2 = 10;
const int relay4 = 11;
const int low_batt = 2;
const int batt_en = 4;
const int relay_in = 7;
const int door = 3;
const int okc = 6;



char serial_command_buffer_[32];
//int pin_modes[] = {OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, INPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT};

SerialCommands serial_commands(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");

void cmd_on(SerialCommands* sender, const char* cmd);
void cmd_off(SerialCommands* sender, const char* cmd);
void cmd_mode(SerialCommands* sender, const char* cmd);
void cmd_read(SerialCommands* sender, const char* cmd);
void cmd_all(SerialCommands* sender, const char* cmd);

void cmd_on(SerialCommands* sender, const char* cmd)
{
    String port_str = String(sender->Next());
    if (port_str == NULL) {
        sender->GetSerial()->println("ERROR NO_PORT");
        return;
    }
    int i = port_str.toInt();
    if (i < 2 || i > 19)
        return;
    Serial.println(port_str);
    digitalWrite(i, HIGH);
    sender->GetSerial()->println(port_str + " is on");
}

void cmd_off(SerialCommands* sender, const char* cmd) {
    String port_str = String(sender->Next());
    if (port_str == NULL) {
        sender->GetSerial()->println("ERROR NO_PORT");
        return;
    }
    int i = port_str.toInt();
    if (i < 2 || i > 19)
        return;
    digitalWrite(i, LOW);
    sender->GetSerial()->println(port_str + " is off");
}

void cmd_mode(SerialCommands* sender, const char* cmd) {
    String str_pin = String(sender->Next());
    String str_mode = String(sender->Next());
    if (str_pin == NULL || str_mode == NULL) {
        sender->GetSerial()->println(ERROR_RESP);
        return;
    }
    int p = str_pin.toInt();
    int m = str_mode.toInt();
    if (p > 1 && p < 20 && (m == INPUT || m == OUTPUT)) {
        pinMode(p, m);
        sender->GetSerial()->println("Setting pin" + str_pin + " to" + (m == 0 ? " INPUT" : " OUTPUT"));
    }
}

void cmd_read(SerialCommands* sender, const char* cmd)
{
    String pin = String(sender->Next());
    int p = pin.toInt();
    String resp = (digitalRead(p) == HIGH) ? "HIGH" : "LOW";
    sender->GetSerial()->println("Pin" + pin + " is " + resp);
}

void cmd_all(SerialCommands* sender)
{
    String on_off = String(sender->Next());
    if (on_off == NULL) return;
    int value = (on_off.equals("ON") ? HIGH : LOW);
    for (int i = 2; i < 14; i++)
        if(i != 7) digitalWrite(i, value);
}

SerialCommand cmd_on_(CMD_ON, cmd_on);
SerialCommand cmd_off_(CMD_OFF, cmd_off);
SerialCommand cmd_mode_(CMD_MODE, cmd_mode);
SerialCommand cmd_read_(CMD_READ, cmd_read);
SerialCommand cmd_all_(CMD_ALL, cmd_all);

void setup(){
    for (int i = 2; i < 14; i++)
        pinMode(i, (i == 7 ? INPUT : OUTPUT));
    Serial.begin(9600);
    serial_commands.SetDefaultHandler(cmd_unrecognized);
    serial_commands.AddCommand(&cmd_on_);
    serial_commands.AddCommand(&cmd_off_);
    serial_commands.AddCommand(&cmd_mode_);
    serial_commands.AddCommand(&cmd_read_);
    serial_commands.AddCommand(&cmd_all_);
    Serial.println(F("Ready!"));
}

void loop(){
    serial_commands.ReadSerial();
}

