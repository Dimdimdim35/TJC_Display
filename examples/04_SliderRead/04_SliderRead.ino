#include <TJC_Display.h>

TJC_Display lcd(Serial2);

void setup()
{
    Serial.begin(115200);
    lcd.begin(115200);
    lcd.page((uint8_t)0);
    lcd.text("t0") = "Slider read";
    lcd.slider("h0").minimum(0).maximum(100).value(50);
    Serial.println("Slider read test");
}

void loop()
{
    lcd.update();
    TJC_Event event;
    while (lcd.read(event))
    {
        if (event.type == TJC_EventType::TouchRelease)
        {
            uint32_t value = lcd.slider("h0").read();
            Serial.print("Slider=");
            Serial.println(value);
            lcd.number("n0") = value;
        }
    }
}
