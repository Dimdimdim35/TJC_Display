#include <TJC_Display.h>

TJC_Display lcd(Serial2);

void setup()
{
    Serial.begin(115200);
    lcd.begin(115200);

    lcd.page((uint8_t)0);
    lcd.text("t0") = "Hello TJC";
    lcd.number("n0") = 1234;
    lcd.progress("j0").value(50);
}

void loop()
{
    lcd.update();
}
