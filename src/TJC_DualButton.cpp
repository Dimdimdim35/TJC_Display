#include "TJC_DualButton.h"
#include "TJC_Display.h"
#include "TJC_Protocol.h"

TJC_DualButton::TJC_DualButton(TJC_Protocol& protocol, const char* name, TJC_Display* display, uint8_t id)
    : TJC_Component(protocol, name, display, id) {}

TJC_DualButton& TJC_DualButton::text(const char* v) { _protocol.sendAttribute(_name, "txt", v); return *this; }
TJC_DualButton& TJC_DualButton::text(const __FlashStringHelper* v) { _protocol.sendAttribute(_name, "txt", v); return *this; }
TJC_DualButton& TJC_DualButton::value(uint8_t v) { _protocol.sendAttribute(_name, "val", v ? 1 : 0); return *this; }
TJC_DualButton& TJC_DualButton::on() { return value(1); }
TJC_DualButton& TJC_DualButton::off() { return value(0); }
bool TJC_DualButton::get(uint32_t& value, uint32_t timeout) { if (!_display) return false; return _display->getNumber(_name, value, timeout); }
uint32_t TJC_DualButton::read(uint32_t def, uint32_t timeout) { uint32_t v; return get(v, timeout) ? v : def; }
