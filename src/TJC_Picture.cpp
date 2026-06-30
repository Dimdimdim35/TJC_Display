#include "TJC_Picture.h"
#include "TJC_Display.h"
#include "TJC_Protocol.h"

TJC_Picture::TJC_Picture(TJC_Protocol& protocol, const char* name, TJC_Display* display, uint8_t id)
    : TJC_Component(protocol, name, display, id) {}

TJC_Picture& TJC_Picture::operator=(uint32_t v) { return picture(v); }
TJC_Picture& TJC_Picture::picture(uint32_t v) { _protocol.sendAttribute(_name, "pic", v); return *this; }
bool TJC_Picture::get(uint32_t& value, uint32_t timeout) { if (!_display) return false; return _display->getNumber(_name, value, timeout); }
uint32_t TJC_Picture::read(uint32_t def, uint32_t timeout) { uint32_t v; return get(v, timeout) ? v : def; }
