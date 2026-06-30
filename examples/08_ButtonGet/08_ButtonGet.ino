#include <TJC_Display.h>

TJC_Display lcd(Serial2);

void setup()
{
    Serial.begin(115200);
    lcd.begin(115200);
    lcd.page((uint8_t)0);
    lcd.text("t0") = "Button";
    lcd.number("n0") = 0;
    lcd.button("b0").text("TEST");
}

void loop()
{
    lcd.update();
    TJC_Event event;
    while (lcd.read(event))
    {
        if (event.type == TJC_EventType::TouchRelease)
        {
            uint32_t value;
            if (lcd.button("b0").get(value))
            {
                Serial.print("Button value = "); Serial.println(value);
                lcd.number("n0") = value;
            }
        }
    }
}
