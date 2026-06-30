#ifndef TJC_PROGRESSBAR_H
#define TJC_PROGRESSBAR_H

#include <Arduino.h>
#include "TJC_Component.h"
#include "TJC_Types.h"

class TJC_Display;

class TJC_ProgressBar : public TJC_Component
{
public:
    TJC_ProgressBar(TJC_Protocol& protocol,
          const char* name,
          TJC_Display* display = nullptr,
          uint8_t id = 0);


    TJC_ProgressBar& operator=(uint32_t value);
    TJC_ProgressBar& value(uint32_t value);
    bool get(uint32_t& value, uint32_t timeout = TJC_DEFAULT_TIMEOUT);
    uint32_t read(uint32_t defaultValue = 0, uint32_t timeout = TJC_DEFAULT_TIMEOUT);

};

#endif
