
#pragma once

#include <SerialCommands.h>
#include "Arduino.h"

class Pins {
    public:
        uint8_t mode, pin;
        const char* name;
        Pins() {
            mode = -1;
            pin = -1;
            name = '\0';
        }
        Pins(uint8_t m, uint8_t p, const char* n) {
            mode = m;
            pin = p;
            name = n;
            pinMode(p, m);
            if(m) digitalWrite(p, HIGH);
        }
};

extern Pins pins[11];

Pins getPin(const Pins (*ptr)[11], uint8_t _pin) {
    int z = sizeof(*ptr) / (sizeof(ptr) * 2);
    for(int i = 0; i <= z; i++) {
        if( _pin == ((*ptr)[i]).pin) {
            return ((*ptr)[i]);
        }
    }
}

Pins findPin(const Pins (*ptr)[11], const char* n) {
    int z = sizeof(*ptr) / (sizeof(ptr) * 2);
    for(int i = 0; i <= z; i++) {
        if( n == ((*ptr)[i]).name)
            return ((*ptr)[i]);
    }
}

void printAll(const Pins (*ptr)[11]) {
    int z = sizeof(*ptr) / (sizeof(ptr) * 2);
    for(int i = 2; i <= 13; i++) {
        Pins p = getPin(ptr, i);
        if(p.name == '\0') continue;
        Serial.print(p.name);
        //Serial.print((*ptr)[i].name);
        Serial.print(", ");
        Serial.println(p.pin);
    }
}

#define null

#define CMD_ON      "ON"
#define CMD_OFF     "OFF"
#define CMD_READ    "READ"
#define CMD_ALL     "ALL"
#define CMD_MODE    "MODE"
#define ERROR_RESP  "ERROR"
#define CMD          "cmd"

#define HELP    'H'
#define WRITE   'w'
#define ON      HIGH
#define OFF     LOW
#define READ    'R'
#define MODE    'M'
#define PWM     'P'
#define ALL     'A'

#define PRINT(s) (sender->GetSerial()->print((s)))
#define PRINTLN(s) (sender->GetSerial()->println((s)))
#define IS_PWM(p) (((p)==3||(p)==5||(p)==6||p==9||p==10||p==11))

inline bool isValid(const char*);

/**
 * Default command handler.
 */
void cmd_unrecognized(SerialCommands* sender, const char* cmd)
{
    if(!isValid(cmd)) {
        PRINTLN("Invlid cmd.");
        return;
    }
    String pin_s = String(sender->Next());
    String val = String(sender->Next());
    switch(cmd[0])
    {
        case HELP:
        case 'h':
        default:
            PRINTLN(F("Usage: cmd <action> <value>\r\n Actions: Write(w), Read(r), PWM(p), Mode(m), AllPins(a)\r\n Values: 1=Write High/Set Input, 0=Write Low/Set Output"));
            break;

        case 'm':
        case 'M':
            PRINTLN(" pin=" + pin_s + "\r\n mode=" + (val.toInt() == 0 ? "INPUT" : "OUTPUT") );
            pinMode(pin_s.toInt(), (val.toInt() == 0 ? INPUT : OUTPUT ));
            break;

        case 'W':
        case WRITE:
            PRINTLN("Writing to pin " + pin_s + ", Value = " + val);
            digitalWrite(pin_s.toInt(), val.toInt());
            break;

        case 'r':
        case READ:
            PRINTLN("\r\nReading pin " + pin_s);
            val = String(digitalRead(pin_s.toInt()));
            PRINTLN(" Value = " + val);
            break;

        case 'a':
        case ALL:
            PRINTLN("Setting all pins " + val);
            for(int i = 2; i < 14; i++) {
                if(i == 7) return;
                if(val.equals("off"))
                    digitalWrite(i, LOW);
                else if(val.equals("on"))
                    digitalWrite(i, HIGH);
                else {
                    PRINTLN(" Invalid parameter: " + val);
                    break;
                }
            }
            break;

    }
}

/**
 * Handles the serial commands.
 */
void handleCommand(SerialCommands* sender, const char* cmd)
{
    String pin_s, val;
    int v;
    PRINTLN("HandleCommand");
    // Switch the proceeding char
    switch(sender->Next()[0])
    {
        case HELP:
        case 'h':
        default:
            PRINTLN(F("Usage: cmd <action> <value>\r\n Actions: Write(w), Read(r), PWM(p), Mode(m), AllPins(a)\r\n Values: 1=Write High/Set Input, 0=Write Low/Set Output"));
            break;

        case 'm':
        case 'M':
            pin_s = String(sender->Next());
            val = String(sender->Next());

            PRINTLN(" pin=" + pin_s + "\r\n mode=" + (val.toInt() == 0 ? "INPUT" : "OUTPUT") );
            pinMode(pin_s.toInt(), (val.toInt() == 0 ? INPUT : OUTPUT ));
            break;

        case 'W':
        case WRITE:
            pin_s = String(sender->Next());
            val = String(sender->Next());
            PRINTLN("pin=" + pin_s);
            PRINTLN("val=" + val);
            digitalWrite(pin_s.toInt(), val.toInt());
            break;

        case 'r':
        case READ:
            pin_s = String(sender->Next());
            PRINTLN("\r\nReading pin " + pin_s);
            val = String(digitalRead(pin_s.toInt()));
            PRINTLN(" Value = " + val);
            break;

        case 'a':
        case ALL:
            // val = String(sender->Next());
            // const char* s = sender->Next();
            v = (sender->Next()[1] == 'f' ? LOW : HIGH);
            PRINTLN("Command All.");
            // PRINTLN("Value = " + val);
            for(int i = 2; i < 14; i++) {
                if(i == 7) return;
                // if(val.indexOf("off") > 0)
                digitalWrite(i, v);
                //else if(val.equals("on"))
                //    digitalWrite(i, HIGH);
                //else {
                //    PRINTLN(" Invalid parameter: " + val);
                //    break;
                // }
            }
            break;

        case PWM:
        case 'p':
            pin_s = String(sender->Next());
            if(IS_PWM(pin_s.toInt())) {
                PRINTLN("Is a PWM pin.");
            } else {
                PRINTLN("Is not a PWM pin.");
            }
            break;
    }
    PRINTLN("");
}

/**
 * Handles ON commands.
 */
void cmd_on(SerialCommands* sender, const char* cmd)
{
    const char* port_str = sender->Next();
    if (!isValid(cmd) && !isValid(port_str)) {
        PRINTLN(F("NULL Command Recieved."));
        return;
    }
    PRINT("cmd=");
    PRINTLN(cmd);
    int i = String(port_str).toInt();
    if (i < 2 || i > 19)
        return;
    PRINTLN(port_str);
    digitalWrite(i, HIGH);
    PRINTLN(String(port_str) + " is on");
}

/**
 * Handles OFF commands.
 */
void cmd_off(SerialCommands* sender, const char* cmd)
{
    const char* str = sender->Next();
    if (!isValid(str)) {
        PRINTLN(F("NULL Command Recieved."));
    } else {
        String port_str = String(str);
        uint8_t i = port_str.toInt();
        if (i > 1 && i < 20) {
            digitalWrite(i, LOW);
            PRINTLN(port_str + " is off");
        }
    }
}

/**
 * Handles PinMode commands.
 */
void cmd_mode(SerialCommands* sender, const char* cmd)
{
    String str_pin = String(sender->Next());
    String str_mode = String(sender->Next());
    if (str_pin == NULL || str_mode == NULL) {
        PRINTLN(F("NULL Command Recieved."));
        return;
    }
    int p = str_pin.toInt();
    int m = str_mode.toInt();
    if (p > 1 && p < 20 && (m == INPUT || m == OUTPUT)) {
        pinMode(p, m);
        PRINTLN("Setting pin" + str_pin + " to" + (m == 0 ? " INPUT" : " OUTPUT"));
    }
}

/**
 * Handles pin reading commands.
 */
void cmd_read(SerialCommands* sender, const char* cmd)
{
    handleCommand(sender, cmd);
    /*const char* str = sender->Next();
    if(!isValid(str))
        PRINTLN(F("NULL Command Recieved."));
    else {
        String pin = String(str);
        uint8_t p = (uint8_t)pin.toInt();
        String resp = (digitalRead(p) == HIGH) ? "HIGH" : "LOW";
        PRINTLN("Pin" + pin + " is " + resp);
    }*/
}

/**
 * Sets all the digital outputs at once.
 */
void cmd_all(SerialCommands* sender, const char* cmd)
{
    //handleCommand(sender, cmd);
    const char* str = sender->Next();
    if (!isValid(str))
        PRINTLN(F("NULL Command Recieved."));
    else {
        String on_off = String(str);
        uint8_t value = (on_off.equals("ON") ? HIGH : LOW);
        for (int i = 2; i < 14; i++)
            if(i != 7) digitalWrite(i, value);
    }
}

inline bool isValid(const char* cmd)
{
    return (cmd != NULL);
}
