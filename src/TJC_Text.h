#ifndef TJC_TEXT_H
#define TJC_TEXT_H

#include <Arduino.h>
#include "TJC_Component.h"
#include "TJC_Types.h"

class TJC_Display;

class TJC_Text : public TJC_Component
{
public:
    TJC_Text(TJC_Protocol& protocol,
          const char* name,
          TJC_Display* display = nullptr,
          uint8_t id = 0);


    TJC_Text& operator=(const char* text);
    TJC_Text& operator=(const __FlashStringHelper* text);
    TJC_Text& text(const char* text);
    TJC_Text& text(const __FlashStringHelper* text);
    TJC_Text& font(uint8_t id);
    TJC_Text& hAlign(TJC_HAlign value);
    TJC_Text& valign(TJC_VAlign value);
    bool get(char* buffer, size_t length, uint32_t timeout = TJC_DEFAULT_TIMEOUT);

};

#endif
