#ifndef TJC_EVENT_H
#define TJC_EVENT_H

#include <Arduino.h>

enum class TJC_EventType : uint8_t
{
    None = 0,
    TouchPress,
    TouchRelease,
    PageChanged,
    NumberReceived,
    StringReceived,
    Sleep,
    Wakeup,
    Startup,
    Error
};

struct TJC_Event
{
    static constexpr size_t TEXT_SIZE = 64;

    TJC_EventType type = TJC_EventType::None;
    uint8_t header = 0;
    uint8_t page = 0;
    uint8_t component = 0;
    uint32_t value = 0;
    char text[TEXT_SIZE] = {0};
    uint8_t errorCode = 0;

    void clear()
    {
        type = TJC_EventType::None;
        header = 0;
        page = 0;
        component = 0;
        value = 0;
        errorCode = 0;
        text[0] = '\0';
    }

    bool isPress() const { return type == TJC_EventType::TouchPress; }
    bool isRelease() const { return type == TJC_EventType::TouchRelease; }
    bool isTouch() const { return isPress() || isRelease(); }
    bool isComponent(uint8_t id) const { return component == id; }
    bool isPage(uint8_t id) const { return page == id; }
    bool isError() const { return type == TJC_EventType::Error; }
    bool isNumber() const { return type == TJC_EventType::NumberReceived; }
    bool isString() const { return type == TJC_EventType::StringReceived; }
};

#endif
