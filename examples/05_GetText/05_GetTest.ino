/*
 * TJC_Display Library
 *
 * Example : 05_GetText
 */

#include <TJC_Display.h>

TJC_Display lcd(Serial2);

void setup()
{
    Serial.begin(115200);
    lcd.begin(115200);

    lcd.page((uint8_t)0);

    lcd.text("t0") = "Bonjour TJC";

    Serial.println("=== getText() test ===");
}

void loop()
{
    lcd.update();

    char buffer[64];

    if (lcd.getText("t0", buffer, sizeof(buffer)))
    {
        Serial.print("Texte lu = ");
        Serial.println(buffer);
    }
    else
    {
        Serial.println("Lecture texte impossible");
    }

    delay(2000);
}