#ifndef TJC_NUMBER_H
#define TJC_NUMBER_H

#include <Arduino.h>
#include "TJC_Component.h"
#include "TJC_Types.h"

class TJC_Display;

class TJC_Number : public TJC_Component
{
public:
    TJC_Number(TJC_Protocol& protocol,
          const char* name,
          TJC_Display* display = nullptr,
          uint8_t id = 0);


    TJC_Number& operator=(uint32_t value);
    TJC_Number& value(uint32_t value);
    TJC_Number& font(uint8_t id);
    TJC_Number& hAlign(TJC_HAlign value);
    TJC_Number& valign(TJC_VAlign value);
    bool get(uint32_t& value, uint32_t timeout = TJC_DEFAULT_TIMEOUT);
    uint32_t read(uint32_t defaultValue = 0, uint32_t timeout = TJC_DEFAULT_TIMEOUT);

};

#endif
