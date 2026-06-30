#include <TJC_Display.h>

TJC_Display lcd(Serial2);

void setup()
{
    Serial.begin(115200);
    lcd.begin(115200);
    lcd.page((uint8_t)0);
    lcd.number("n0") = 12345;
    delay(300);
    uint32_t value;
    if (lcd.number("n0").get(value))
    {
        Serial.print("Valeur = ");
        Serial.println(value);
    }
}

void loop() {}
