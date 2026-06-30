#include "TJC_Slider.h"
#include "TJC_Display.h"
#include "TJC_Number.h"
#include "TJC_ProgressBar.h"
#include "TJC_Protocol.h"

TJC_Slider::TJC_Slider(TJC_Protocol& protocol,
                       const char* name,
                       TJC_Display* display,
                       uint8_t id)
    : TJC_Component(protocol, name, display, id),
      _numberBindingCount(0),
      _progressBindingCount(0)
{
    for (uint8_t i = 0; i < MAX_BINDINGS; ++i)
    {
        _numberBindings[i] = nullptr;
        _progressBindings[i] = nullptr;
    }
}

TJC_Slider& TJC_Slider::operator=(uint32_t value)
{
    return this->value(value);
}

TJC_Slider& TJC_Slider::value(uint32_t value)
{
    _protocol.sendAttribute(_name, "val", value);
    return *this;
}

TJC_Slider& TJC_Slider::minimum(uint32_t value)
{
    _protocol.sendAttribute(_name, "minval", value);
    return *this;
}

TJC_Slider& TJC_Slider::maximum(uint32_t value)
{
    _protocol.sendAttribute(_name, "maxval", value);
    return *this;
}

TJC_Slider& TJC_Slider::bind(TJC_Number& target)
{
    if (_numberBindingCount < MAX_BINDINGS)
        _numberBindings[_numberBindingCount++] = &target;

    return *this;
}

TJC_Slider& TJC_Slider::bind(TJC_ProgressBar& target)
{
    if (_progressBindingCount < MAX_BINDINGS)
        _progressBindings[_progressBindingCount++] = &target;

    return *this;
}

void TJC_Slider::handleEvent(const TJC_Event& event)
{
    TJC_Component::handleEvent(event);

    if (isRelease(event))
    {
        uint32_t currentValue = read();

        updateBindings(currentValue);
    }
}

void TJC_Slider::updateBindings(uint32_t value)
{
    for (uint8_t i = 0; i < _numberBindingCount; ++i)
    {
        if (_numberBindings[i])
            _numberBindings[i]->value(value);
    }

    for (uint8_t i = 0; i < _progressBindingCount; ++i)
    {
        if (_progressBindings[i])
            _progressBindings[i]->value(value);
    }
}

bool TJC_Slider::get(uint32_t& value, uint32_t timeout)
{
    if (_display == nullptr)
        return false;

    return _display->getNumber(_name, value, timeout);
}

uint32_t TJC_Slider::read(uint32_t defaultValue, uint32_t timeout)
{
    uint32_t value;

    if (get(value, timeout))
        return value;

    return defaultValue;
}
