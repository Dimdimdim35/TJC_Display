#ifndef TJC_PROTOCOL_H
#define TJC_PROTOCOL_H

#include <Arduino.h>

class TJC_Protocol
{
public:
    explicit TJC_Protocol(HardwareSerial& serial);

    void sendCommand(const char* command);
    void sendAttribute(const char* component, const char* attribute, uint32_t value);
    void sendAttribute(const char* component, const char* attribute, const char* value);
    void sendAttribute(const char* component, const char* attribute, const __FlashStringHelper* value);
    void sendFunction(const char* function, const char* component);
    void sendFunction(const char* function, const char* component, uint32_t value);

private:
    HardwareSerial& _serial;
    void endCommand();
};

#endif
