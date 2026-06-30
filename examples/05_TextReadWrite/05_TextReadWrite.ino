#include <TJC_Display.h>

TJC_Display lcd(Serial2);

void setup()
{
    Serial.begin(115200);
    lcd.begin(115200);
    lcd.page((uint8_t)0);
    lcd.text("t0") = "Bonjour V1.1";
    delay(300);
    char buffer[64];
    if (lcd.text("t0").get(buffer, sizeof(buffer)))
    {
        Serial.print("Texte lu = ");
        Serial.println(buffer);
    }
}

void loop()
{
    lcd.update();
}
