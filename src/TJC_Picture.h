#ifndef TJC_PICTURE_H
#define TJC_PICTURE_H

#include <Arduino.h>
#include "TJC_Component.h"
#include "TJC_Types.h"

class TJC_Display;

class TJC_Picture : public TJC_Component
{
public:
    TJC_Picture(TJC_Protocol& protocol,
          const char* name,
          TJC_Display* display = nullptr,
          uint8_t id = 0);


    TJC_Picture& operator=(uint32_t pictureId);
    TJC_Picture& picture(uint32_t pictureId);
    bool get(uint32_t& pictureId, uint32_t timeout = TJC_DEFAULT_TIMEOUT);
    uint32_t read(uint32_t defaultValue = 0, uint32_t timeout = TJC_DEFAULT_TIMEOUT);

};

#endif
