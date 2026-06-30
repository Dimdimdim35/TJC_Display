#include "TJC_Button.h"
#include "TJC_Display.h"
#include "TJC_Protocol.h"

TJC_Button::TJC_Button(TJC_Protocol& protocol, const char* name, TJC_Display* display, uint8_t id)
    : TJC_Component(protocol, name, display, id) {}

TJC_Button& TJC_Button::text(const char* v) { _protocol.sendAttribute(_name, "txt", v); return *this; }
TJC_Button& TJC_Button::text(const __FlashStringHelper* v) { _protocol.sendAttribute(_name, "txt", v); return *this; }
TJC_Button& TJC_Button::value(uint8_t v) { _protocol.sendAttribute(_name, "val", v); return *this; }
TJC_Button& TJC_Button::font(uint8_t id) { _protocol.sendAttribute(_name, "font", id); return *this; }
TJC_Button& TJC_Button::hAlign(TJC_HAlign v) { _protocol.sendAttribute(_name, "xcen", static_cast<uint8_t>(v)); return *this; }
TJC_Button& TJC_Button::valign(TJC_VAlign v) { _protocol.sendAttribute(_name, "ycen", static_cast<uint8_t>(v)); return *this; }
bool TJC_Button::get(uint32_t& value, uint32_t timeout) { if (!_display) return false; return _display->getNumber(_name, value, timeout); }
uint32_t TJC_Button::read(uint32_t def, uint32_t timeout) { uint32_t v; return get(v, timeout) ? v : def; }
