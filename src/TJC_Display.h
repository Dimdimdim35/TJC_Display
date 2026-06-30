#ifndef TJC_DISPLAY_H
#define TJC_DISPLAY_H

#include <Arduino.h>

#include "TJC_Types.h"
#include "TJC_Event.h"
#include "TJC_Protocol.h"
#include "TJC_Parser.h"
#include "TJC_Text.h"
#include "TJC_Number.h"
#include "TJC_Button.h"
#include "TJC_DualButton.h"
#include "TJC_Slider.h"
#include "TJC_ProgressBar.h"
#include "TJC_Picture.h"

class TJC_Display
{
public:
    explicit TJC_Display(HardwareSerial& serial);

    void begin(uint32_t baudrate = 9600);
    void update();
    bool read(TJC_Event& event);

    void page(uint8_t pageId);
    void command(const char* command);

    bool getNumber(const char* component,
                   uint32_t& value,
                   uint32_t timeout = TJC_DEFAULT_TIMEOUT);

    bool getText(const char* component,
                 char* buffer,
                 size_t length,
                 uint32_t timeout = TJC_DEFAULT_TIMEOUT);

    TJC_Text text(const char* name, uint8_t id = 0);
    TJC_Number number(const char* name, uint8_t id = 0);
    TJC_Button button(const char* name, uint8_t id = 0);
    TJC_DualButton dualButton(const char* name, uint8_t id = 0);
    TJC_Slider slider(const char* name, uint8_t id = 0);
    TJC_ProgressBar progress(const char* name, uint8_t id = 0);
    TJC_Picture picture(const char* name, uint8_t id = 0);

private:
    HardwareSerial& _serial;
    TJC_Protocol _protocol;
    TJC_Parser _parser;
};

#endif
