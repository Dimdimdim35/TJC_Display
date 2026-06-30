#include <TJC_Display.h>

TJC_Display lcd(Serial2);

void setup()
{
    Serial.begin(115200);
    lcd.begin(115200);
    lcd.page((uint8_t)0);
    Serial.println("Touch event test");
}

void loop()
{
    lcd.update();
    TJC_Event event;
    while (lcd.read(event))
    {
        Serial.print("Type=");
        Serial.print((int)event.type);
        Serial.print(" Page=");
        Serial.print(event.page);
        Serial.print(" Component=");
        Serial.println(event.component);
    }
}
