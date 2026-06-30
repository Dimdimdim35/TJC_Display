#ifndef TJC_COMPONENT_H
#define TJC_COMPONENT_H

#include <Arduino.h>
#include "TJC_Event.h"

class TJC_Protocol;
class TJC_Display;

class TJC_Component
{
public:
    typedef void (*EventCallback)();

    TJC_Component(TJC_Protocol& protocol,
                  const char* name,
                  TJC_Display* display = nullptr,
                  uint8_t id = 0);

    virtual ~TJC_Component() = default;

    const char* name() const;
    uint8_t id() const;

    bool isComponent(const TJC_Event& event) const;
    bool isPress(const TJC_Event& event) const;
    bool isRelease(const TJC_Event& event) const;

    void onPress(EventCallback callback);
    void onRelease(EventCallback callback);

    virtual void handleEvent(const TJC_Event& event);

    TJC_Component& show();
    TJC_Component& hide();
    TJC_Component& enable();
    TJC_Component& disable();
    TJC_Component& refresh();
    TJC_Component& textColor(uint16_t color);
    TJC_Component& backgroundColor(uint16_t color);

protected:
    TJC_Protocol& _protocol;
    TJC_Display* _display;
    const char* _name;
    uint8_t _id;

private:
    EventCallback _onPress;
    EventCallback _onRelease;
};

#endif
