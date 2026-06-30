#include "TJC_Protocol.h"

TJC_Protocol::TJC_Protocol(HardwareSerial& serial)
    : _serial(serial)
{
}

void TJC_Protocol::endCommand()
{
    _serial.write(0xFF);
    _serial.write(0xFF);
    _serial.write(0xFF);
}

void TJC_Protocol::sendCommand(const char* command)
{
    _serial.print(command);
    endCommand();
}

void TJC_Protocol::sendAttribute(const char* component, const char* attribute, uint32_t value)
{
    _serial.print(component);
    _serial.print('.');
    _serial.print(attribute);
    _serial.print('=');
    _serial.print(value);
    endCommand();
}

void TJC_Protocol::sendAttribute(const char* component, const char* attribute, const char* value)
{
    _serial.print(component);
    _serial.print('.');
    _serial.print(attribute);
    _serial.print("=\"");
    _serial.print(value);
    _serial.print('"');
    endCommand();
}

void TJC_Protocol::sendAttribute(const char* component, const char* attribute, const __FlashStringHelper* value)
{
    _serial.print(component);
    _serial.print('.');
    _serial.print(attribute);
    _serial.print("=\"");
    _serial.print(value);
    _serial.print('"');
    endCommand();
}

void TJC_Protocol::sendFunction(const char* function, const char* component)
{
    _serial.print(function);
    _serial.print(' ');
    _serial.print(component);
    endCommand();
}

void TJC_Protocol::sendFunction(const char* function, const char* component, uint32_t value)
{
    _serial.print(function);
    _serial.print(' ');
    _serial.print(component);
    _serial.print(',');
    _serial.print(value);
    endCommand();
}
