#include "TJC_ProgressBar.h"
#include "TJC_Display.h"
#include "TJC_Protocol.h"

TJC_ProgressBar::TJC_ProgressBar(TJC_Protocol& protocol, const char* name, TJC_Display* display, uint8_t id)
    : TJC_Component(protocol, name, display, id) {}

TJC_ProgressBar& TJC_ProgressBar::operator=(uint32_t v) { return value(v); }
TJC_ProgressBar& TJC_ProgressBar::value(uint32_t v) { _protocol.sendAttribute(_name, "val", v); return *this; }
bool TJC_ProgressBar::get(uint32_t& value, uint32_t timeout) { if (!_display) return false; return _display->getNumber(_name, value, timeout); }
uint32_t TJC_ProgressBar::read(uint32_t def, uint32_t timeout) { uint32_t v; return get(v, timeout) ? v : def; }
