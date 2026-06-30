#include <TJC_Display.h>

TJC_Display lcd(Serial2);
uint32_t counter = 0;

void setup()
{
    Serial.begin(115200);
    lcd.begin(115200);
    lcd.page((uint8_t)0);
    lcd.text("t0") = "Output test";
}

void loop()
{
    lcd.update();
    lcd.number("n0") = counter;
    lcd.progress("j0").value(counter % 101);
    counter++;
    delay(500);
}
