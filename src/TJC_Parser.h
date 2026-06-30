#ifndef TJC_PARSER_H
#define TJC_PARSER_H

#include <Arduino.h>
#include "TJC_Event.h"

class TJC_Parser
{
public:
    TJC_Parser();

    void process(uint8_t byte);
    bool read(TJC_Event& event);

private:
    static constexpr uint8_t BUFFER_SIZE = 96;

    uint8_t _buffer[BUFFER_SIZE];
    uint8_t _index;
    uint8_t _ffCount;
    bool _hasEvent;
    TJC_Event _event;

    void parseFrame(const uint8_t* data, uint8_t length);
};

#endif
