#include "TJC_Display.h"
#include <string.h>

TJC_Display::TJC_Display(HardwareSerial& serial)
    : _serial(serial),
      _protocol(serial),
      _parser()
{
}

void TJC_Display::begin(uint32_t baudrate)
{
    _serial.begin(baudrate);
}

void TJC_Display::update()
{
    while (_serial.available())
        _parser.process(static_cast<uint8_t>(_serial.read()));
}

bool TJC_Display::read(TJC_Event& event)
{
    return _parser.read(event);
}

void TJC_Display::page(uint8_t pageId)
{
    char command[16];
    snprintf(command, sizeof(command), "page %u", pageId);
    _protocol.sendCommand(command);
}

void TJC_Display::command(const char* command)
{
    _protocol.sendCommand(command);
}

bool TJC_Display::getNumber(const char* component, uint32_t& value, uint32_t timeout)
{
    char command[40];
    snprintf(command, sizeof(command), "get %s.val", component);
    _protocol.sendCommand(command);

    uint32_t start = millis();
    TJC_Event event;

    while (millis() - start < timeout)
    {
        update();

        while (read(event))
        {
            if (event.type == TJC_EventType::NumberReceived)
            {
                value = event.value;
                return true;
            }
        }
    }

    return false;
}

bool TJC_Display::getText(const char* component, char* buffer, size_t length, uint32_t timeout)
{
    if (buffer == nullptr || length == 0)
        return false;

    buffer[0] = '\0';

    char command[40];
    snprintf(command, sizeof(command), "get %s.txt", component);
    _protocol.sendCommand(command);

    uint32_t start = millis();
    TJC_Event event;

    while (millis() - start < timeout)
    {
        update();

        while (read(event))
        {
            if (event.type == TJC_EventType::StringReceived)
            {
                strncpy(buffer, event.text, length - 1);
                buffer[length - 1] = '\0';
                return true;
            }
        }
    }

    return false;
}

TJC_Text TJC_Display::text(const char* name, uint8_t id) { return TJC_Text(_protocol, name, this, id); }
TJC_Number TJC_Display::number(const char* name, uint8_t id) { return TJC_Number(_protocol, name, this, id); }
TJC_Button TJC_Display::button(const char* name, uint8_t id) { return TJC_Button(_protocol, name, this, id); }
TJC_DualButton TJC_Display::dualButton(const char* name, uint8_t id) { return TJC_DualButton(_protocol, name, this, id); }
TJC_Slider TJC_Display::slider(const char* name, uint8_t id) { return TJC_Slider(_protocol, name, this, id); }
TJC_ProgressBar TJC_Display::progress(const char* name, uint8_t id) { return TJC_ProgressBar(_protocol, name, this, id); }
TJC_Picture TJC_Display::picture(const char* name, uint8_t id) { return TJC_Picture(_protocol, name, this, id); }
