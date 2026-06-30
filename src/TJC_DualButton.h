#ifndef TJC_DUALBUTTON_H
#define TJC_DUALBUTTON_H

#include <Arduino.h>
#include "TJC_Component.h"
#include "TJC_Types.h"

class TJC_Display;

class TJC_DualButton : public TJC_Component
{
public:
    TJC_DualButton(TJC_Protocol& protocol,
          const char* name,
          TJC_Display* display = nullptr,
          uint8_t id = 0);


    TJC_DualButton& text(const char* text);
    TJC_DualButton& text(const __FlashStringHelper* text);
    TJC_DualButton& value(uint8_t value);
    TJC_DualButton& on();
    TJC_DualButton& off();
    bool get(uint32_t& value, uint32_t timeout = TJC_DEFAULT_TIMEOUT);
    uint32_t read(uint32_t defaultValue = 0, uint32_t timeout = TJC_DEFAULT_TIMEOUT);

};

#endif
