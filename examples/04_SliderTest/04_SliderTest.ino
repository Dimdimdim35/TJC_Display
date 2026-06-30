/*
 * TJC_Display Library
 *
 * Example : 04_SliderGetNumber
 */

#include <TJC_Display.h>

TJC_Display lcd(Serial2);

void setup()
{
    Serial.begin(115200);

    lcd.begin(115200);

    lcd.page((uint8_t)0);

    lcd.text("t0") = "Move slider";

    lcd.number("n0") = 0;

    Serial.println("=== Slider getNumber() test ===");
}

void loop()
{
    lcd.update();

    TJC_Event event;

    while (lcd.read(event))
    {
        if (event.type == TJC_EventType::TouchRelease &&
            event.component == 5)      // h0
        {
            uint32_t value;

            if (lcd.getNumber("h0", value))
            {
                Serial.print("Slider = ");
                Serial.println(value);

                lcd.number("n0") = value;
            }
            else
            {
                Serial.println("Lecture impossible");
            }
        }
    }
}