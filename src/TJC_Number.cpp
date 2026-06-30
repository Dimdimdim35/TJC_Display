#include "TJC_Number.h"
#include "TJC_Display.h"
#include "TJC_Protocol.h"

TJC_Number::TJC_Number(TJC_Protocol& protocol, const char* name, TJC_Display* display, uint8_t id)
    : TJC_Component(protocol, name, display, id) {}

TJC_Number& TJC_Number::operator=(uint32_t v) { return value(v); }
TJC_Number& TJC_Number::value(uint32_t v) { _protocol.sendAttribute(_name, "val", v); return *this; }
TJC_Number& TJC_Number::font(uint8_t id) { _protocol.sendAttribute(_name, "font", id); return *this; }
TJC_Number& TJC_Number::hAlign(TJC_HAlign v) { _protocol.sendAttribute(_name, "xcen", static_cast<uint8_t>(v)); return *this; }
TJC_Number& TJC_Number::valign(TJC_VAlign v) { _protocol.sendAttribute(_name, "ycen", static_cast<uint8_t>(v)); return *this; }
bool TJC_Number::get(uint32_t& value, uint32_t timeout) { if (!_display) return false; return _display->getNumber(_name, value, timeout); }
uint32_t TJC_Number::read(uint32_t def, uint32_t timeout) { uint32_t v; return get(v, timeout) ? v : def; }
