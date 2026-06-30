#include <TJC_Display.h>
#include "GeneratedUI.h"

TJC_Display lcd(Serial2);
GeneratedUI ui(lcd);

void setup()
{
    Serial.begin(115200);
    lcd.begin(115200);

    delay(500);

    lcd.page((uint8_t)0);

    ui.page0.title = "Object Events";
    ui.page0.value = 50;
    ui.page0.slider.value(50);
    ui.page0.progress.value(50);
    ui.page0.start.text("START");
    ui.page0.bt0.off();

    Serial.println("===== Object Event Helpers Test =====");
}

void loop()
{
    lcd.update();

    TJC_Event event;

    while (lcd.read(event))
    {
        if (ui.page0.start.isRelease(event))
        {
            Serial.println("START release");

            uint32_t value = ui.page0.start.read();

            Serial.print("Start = ");
            Serial.println(value);
        }

        else if (ui.page0.slider.isRelease(event))
        {
            Serial.println("SLIDER release");

            uint32_t value = ui.page0.slider.read();

            Serial.print("Slider = ");
            Serial.println(value);

            ui.page0.value = value;
            ui.page0.progress.value(value);
        }

        else if (ui.page0.bt0.isRelease(event))
        {
            Serial.println("DUAL release");

            uint32_t state = ui.page0.bt0.read();

            Serial.print("Dual = ");
            Serial.println(state);

            if (state)
                ui.page0.title = "DUAL ON";
            else
                ui.page0.title = "DUAL OFF";
        }
    }
}