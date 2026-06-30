#include "TJC_Parser.h"
#include <string.h>

TJC_Parser::TJC_Parser()
    : _index(0),
      _ffCount(0),
      _hasEvent(false)
{
}

void TJC_Parser::process(uint8_t byte)
{
    if (_index < BUFFER_SIZE)
        _buffer[_index++] = byte;

    if (byte == 0xFF)
        _ffCount++;
    else
        _ffCount = 0;

    if (_ffCount >= 3)
    {
        if (_index >= 3)
            parseFrame(_buffer, _index - 3);

        _index = 0;
        _ffCount = 0;
    }
}

bool TJC_Parser::read(TJC_Event& event)
{
    if (!_hasEvent)
        return false;

    event = _event;
    _event.clear();
    _hasEvent = false;
    return true;
}

void TJC_Parser::parseFrame(const uint8_t* data, uint8_t length)
{
    if (length == 0)
        return;

    _event.clear();
    _event.header = data[0];

    switch (data[0])
    {
        case 0x65:
            if (length >= 4)
            {
                _event.page = data[1];
                _event.component = data[2];
                _event.type = data[3] ? TJC_EventType::TouchPress : TJC_EventType::TouchRelease;
                _hasEvent = true;
            }
            break;

        case 0x66:
            if (length >= 2)
            {
                _event.page = data[1];
                _event.type = TJC_EventType::PageChanged;
                _hasEvent = true;
            }
            break;

        case 0x71:
            if (length >= 5)
            {
                _event.value = ((uint32_t)data[1]) |
                               ((uint32_t)data[2] << 8) |
                               ((uint32_t)data[3] << 16) |
                               ((uint32_t)data[4] << 24);
                _event.type = TJC_EventType::NumberReceived;
                _hasEvent = true;
            }
            break;

        case 0x70:
            _event.type = TJC_EventType::StringReceived;
            {
                uint8_t n = (length - 1 < TJC_Event::TEXT_SIZE - 1)
                            ? (length - 1)
                            : (TJC_Event::TEXT_SIZE - 1);
                memcpy(_event.text, data + 1, n);
                _event.text[n] = '\0';
                _hasEvent = true;
            }
            break;

        default:
            _event.type = TJC_EventType::Error;
            _event.errorCode = data[0];
            _hasEvent = true;
            break;
    }
}
