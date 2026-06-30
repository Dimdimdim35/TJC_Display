#include "TJC_Text.h"
#include "TJC_Display.h"
#include "TJC_Protocol.h"

TJC_Text::TJC_Text(TJC_Protocol& protocol, const char* name, TJC_Display* display, uint8_t id)
    : TJC_Component(protocol, name, display, id) {}

TJC_Text& TJC_Text::operator=(const char* v) { return text(v); }
TJC_Text& TJC_Text::operator=(const __FlashStringHelper* v) { return text(v); }
TJC_Text& TJC_Text::text(const char* v) { _protocol.sendAttribute(_name, "txt", v); return *this; }
TJC_Text& TJC_Text::text(const __FlashStringHelper* v) { _protocol.sendAttribute(_name, "txt", v); return *this; }
TJC_Text& TJC_Text::font(uint8_t id) { _protocol.sendAttribute(_name, "font", id); return *this; }
TJC_Text& TJC_Text::hAlign(TJC_HAlign v) { _protocol.sendAttribute(_name, "xcen", static_cast<uint8_t>(v)); return *this; }
TJC_Text& TJC_Text::valign(TJC_VAlign v) { _protocol.sendAttribute(_name, "ycen", static_cast<uint8_t>(v)); return *this; }

bool TJC_Text::get(char* buffer, size_t length, uint32_t timeout)
{
    if (_display == nullptr) return false;
    return _display->getText(_name, buffer, length, timeout);
}
