#include "TJC_Component.h"
#include "TJC_Protocol.h"

TJC_Component::TJC_Component(TJC_Protocol& protocol,
                             const char* name,
                             TJC_Display* display,
                             uint8_t id)
    : _protocol(protocol),
      _display(display),
      _name(name),
      _id(id),
      _onPress(nullptr),
      _onRelease(nullptr)
{
}

const char* TJC_Component::name() const { return _name; }
uint8_t TJC_Component::id() const { return _id; }

bool TJC_Component::isComponent(const TJC_Event& event) const
{
    return event.component == _id;
}

bool TJC_Component::isPress(const TJC_Event& event) const
{
    return event.isPress() && isComponent(event);
}

bool TJC_Component::isRelease(const TJC_Event& event) const
{
    return event.isRelease() && isComponent(event);
}

void TJC_Component::onPress(EventCallback callback)
{
    _onPress = callback;
}

void TJC_Component::onRelease(EventCallback callback)
{
    _onRelease = callback;
}

void TJC_Component::handleEvent(const TJC_Event& event)
{
    if (isPress(event) && _onPress)
        _onPress();

    if (isRelease(event) && _onRelease)
        _onRelease();
}

TJC_Component& TJC_Component::show()
{
    _protocol.sendFunction("vis", _name, 1);
    return *this;
}

TJC_Component& TJC_Component::hide()
{
    _protocol.sendFunction("vis", _name, 0);
    return *this;
}

TJC_Component& TJC_Component::enable()
{
    _protocol.sendFunction("tsw", _name, 1);
    return *this;
}

TJC_Component& TJC_Component::disable()
{
    _protocol.sendFunction("tsw", _name, 0);
    return *this;
}

TJC_Component& TJC_Component::refresh()
{
    _protocol.sendFunction("ref", _name);
    return *this;
}

TJC_Component& TJC_Component::textColor(uint16_t color)
{
    _protocol.sendAttribute(_name, "pco", color);
    return *this;
}

TJC_Component& TJC_Component::backgroundColor(uint16_t color)
{
    _protocol.sendAttribute(_name, "bco", color);
    return *this;
}
