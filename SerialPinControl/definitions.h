
#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#include <SerialCommands.h>

#define CMD_ON    "ON"
#define CMD_OFF   "OFF"
#define CMD_READ  "READ"
#define CMD_ALL   "ALL"
#define CMD_MODE  "MODE"
#define ERROR_RESP  "ERROR"
#define CMD       "cmd"

#define HELP  'H'
#define WRITE 'w'
#define ON    HIGH
#define OFF   LOW
#define READ  'R'
#define MODE  'M'
#define PWM   'P'
#define ALL   'A'


#define PRINT(s) (sender->GetSerial()->print((s)))
#define PRINTLN(s) (sender->GetSerial()->println((s)))
#define IS_PWM(p) (((p)==3||(p)==5||(p)==6||p==9||p==10||p==11))

const int testIndPin = 13;
const int relay3 = 12;
const int ACRelay = 9;
const int QWRelay = 10;
const int battRelay = 11;
const int lowBattEn = 2;
const int relayIn = 7;
const int padSignal = 3;
const int okc = 6;
const int remoteSignal = 5;


inline bool isValid(const char* cmd)
{
    return cmd != NULL;
}

/**
 * Default command handler.
 */
void cmd_unrecognized(SerialCommands* sender, const char* cmd)
{
    PRINT(F("Unrecognized command ["));
    PRINT(cmd);
    PRINTLN(']');
    PRINTLN(F("Usage: cmd <action> <value>\r\n Actions: Write(w), Read(r), PWM(p), Mode(m), AllPins(a)\r\n Values: 1=Write High/Set Input, 0=Write Low/Set Output"));
}

/**
 * Handles the serial commands.
 */
void handleCommand(SerialCommands* sender, const char* cmd)
{
    String pin, val;

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
            pin = String(sender->Next());
            val = String(sender->Next());

            PRINTLN(" pin=" + pin + "\r\n mode=" + (val.toInt() == 0 ? "INPUT" : "OUTPUT") );
            pinMode(pin.toInt(), (val.toInt() == 0 ? INPUT : OUTPUT ));
            break;

        case 'W':
        case WRITE:
            pin = String(sender->Next());
            val = String(sender->Next());
            PRINTLN("pin=" + pin);
            PRINTLN("val=" + val);
            digitalWrite(pin.toInt(), val.toInt());
            break;

        case 'r':
        case READ:
            pin = String(sender->Next());
            PRINTLN("\r\nReading pin " + pin);
            val = String(digitalRead(pin.toInt()));
            PRINTLN(" Value = " + val);
            break;

        case 'a':
        case ALL:
            val = String(sender->Next());
            for(int i = 2; i < 14; i++) {
                if(i != 7) {
                    if(val.equals("off"))
                        digitalWrite(i, LOW);
                    else if(val.equals("on"))
                        digitalWrite(i, HIGH);
                    else {
                        PRINTLN(" Invalid parameter: " + val);
                        break;
                    }
                }
            }
            break;

        case PWM:
        case 'p':
            pin = String(sender->Next());
            if(IS_PWM(pin.toInt())) {
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
    const char* str = sender->Next();
    if(!isValid(str))
        PRINTLN(F("NULL Command Recieved."));
    else {
        String pin = String(str);
        uint8_t p = (uint8_t)pin.toInt();
        String resp = (digitalRead(p) == HIGH) ? "HIGH" : "LOW";
        PRINTLN("Pin" + pin + " is " + resp);
    }
}

/**
 * Sets all the digital outputs at once.
 */
void cmd_all(SerialCommands* sender)
{
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

#endif
