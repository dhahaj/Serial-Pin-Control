/*--------------------------------------------------------------------
  Author		: Pedro Pereira
  License		: BSD
  Repository	: https://github.com/ppedro74/Arduino-SerialCommands

  SerialCommands Simple Demo

  --------------------------------------------------------------------*/
#include <Arduino.h>
#include <SerialCommands.h>

// Pin 13 has an LED connected on most Arduino boards.
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
int pin_modes[] = {OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, INPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT};

SerialCommands serial_commands_(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");

void cmd_unrecognized(SerialCommands* sender, const char* cmd)
{
  sender->GetSerial()->print("Unrecognized command [");
  sender->GetSerial()->print(cmd);
  sender->GetSerial()->println("]");
}

//called for ON command
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

//called for OFF command
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
    sender->GetSerial()->println("ERROR");
    return;
  }
  int p = str_pin.toInt();
  int m = str_mode.toInt();
  pin_modes[p] = m; //== 0 ? 'I' : 'O';
  sender->GetSerial()->print("modes = {");
  for (int j = 0; j < 12; j++) {
    sender->GetSerial()->print(pin_modes[j], DEC);
    sender->GetSerial()->print(", ");
  }
  sender->GetSerial()->println('}');
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

//Note: Commands are case sensitive
SerialCommand cmd_on_("ON", cmd_on);
SerialCommand cmd_off_("OFF", cmd_off);
SerialCommand cmd_mode_("MODE", cmd_mode);
SerialCommand cmd_read_("READ", cmd_read);

void setup()
{
  Serial.begin(9600);

  //Configure the LED for output and sets the intial state to off
  for (int i = 2; i < 14; i++) {
    if (i == 6) {
      pinMode(7, INPUT);
    } else {
      pinMode(i, OUTPUT);
      // digitalWrite(i, LOW);
    }
  }

  serial_commands_.SetDefaultHandler(cmd_unrecognized);
  serial_commands_.AddCommand(&cmd_on_);
  serial_commands_.AddCommand(&cmd_off_);
  serial_commands_.AddCommand(&cmd_mode_);
  serial_commands_.AddCommand(&cmd_read_);

  Serial.println("Ready!");
}

void loop()
{
  serial_commands_.ReadSerial();
}
