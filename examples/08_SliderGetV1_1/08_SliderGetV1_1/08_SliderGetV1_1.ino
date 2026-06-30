/*
 * TJC_Display Library
 *
 * Example : Slider Get V1.1
 * Arduino Mega + TJC4832
 */

#include <TJC_Display.h>

TJC_Display lcd(Serial2);

void setup()
{
    Serial.begin(115200);

    lcd.begin(115200);

    delay(500);

    lcd.page((uint8_t)0);

    lcd.text("t0") = "Slider V1.1";
    lcd.number("n0") = 0;

    lcd.slider("h0")
        .minimum(0)
        .maximum(100)
        .value(50);

    Serial.println("=== Slider get() V1.1 test ===");
}

void loop()
{
    lcd.update();

    TJC_Event event;

    while (lcd.read(event))
    {
        if (event.type == TJC_EventType::TouchRelease &&
            event.component == 5)   // ID du slider h0
        {
            uint32_t value;

            if (lcd.slider("h0").get(value))
            {
                Serial.print("Slider = ");
                Serial.println(value);

                lcd.number("n0") = value;
            }
            else
            {
                Serial.println("Erreur lecture slider");
            }
        }
    }
}
