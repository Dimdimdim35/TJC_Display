#ifndef TJC_SLIDER_H
#define TJC_SLIDER_H

#include <Arduino.h>

#include "TJC_Component.h"
#include "TJC_Types.h"

class TJC_Display;
class TJC_Number;
class TJC_ProgressBar;

class TJC_Slider : public TJC_Component
{
public:
    static constexpr uint8_t MAX_BINDINGS = 4;

    TJC_Slider(TJC_Protocol& protocol,
               const char* name,
               TJC_Display* display = nullptr,
               uint8_t id = 0);

    TJC_Slider& operator=(uint32_t value);

    TJC_Slider& value(uint32_t value);

    TJC_Slider& minimum(uint32_t value);

    TJC_Slider& maximum(uint32_t value);

    TJC_Slider& bind(TJC_Number& target);

    TJC_Slider& bind(TJC_ProgressBar& target);

    void handleEvent(const TJC_Event& event) override;

    bool get(uint32_t& value,
             uint32_t timeout = TJC_DEFAULT_TIMEOUT);

    uint32_t read(uint32_t defaultValue = 0,
                  uint32_t timeout = TJC_DEFAULT_TIMEOUT);

private:
    TJC_Number* _numberBindings[MAX_BINDINGS];
    TJC_ProgressBar* _progressBindings[MAX_BINDINGS];
    uint8_t _numberBindingCount;
    uint8_t _progressBindingCount;

    void updateBindings(uint32_t value);
};

#endif
