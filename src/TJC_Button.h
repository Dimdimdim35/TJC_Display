#ifndef TJC_BUTTON_H
#define TJC_BUTTON_H

#include <Arduino.h>
#include "TJC_Component.h"
#include "TJC_Types.h"

class TJC_Display;

class TJC_Button : public TJC_Component
{
public:
    TJC_Button(TJC_Protocol& protocol,
          const char* name,
          TJC_Display* display = nullptr,
          uint8_t id = 0);


    TJC_Button& text(const char* text);
    TJC_Button& text(const __FlashStringHelper* text);
    TJC_Button& value(uint8_t value);
    TJC_Button& font(uint8_t id);
    TJC_Button& hAlign(TJC_HAlign value);
    TJC_Button& valign(TJC_VAlign value);
    bool get(uint32_t& value, uint32_t timeout = TJC_DEFAULT_TIMEOUT);
    uint32_t read(uint32_t defaultValue = 0, uint32_t timeout = TJC_DEFAULT_TIMEOUT);

};

#endif
