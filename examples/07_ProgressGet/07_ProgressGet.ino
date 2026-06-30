#include <TJC_Display.h>

TJC_Display lcd(Serial2);
uint32_t progressValue = 0;

void setup()
{
    Serial.begin(115200);
    lcd.begin(115200);
    lcd.page((uint8_t)0);
    lcd.text("t0") = "Progress";
    lcd.number("n0") = 0;
}

void loop()
{
    lcd.update();
    lcd.progress("j0").value(progressValue);
    delay(200);
    uint32_t value;
    if (lcd.progress("j0").get(value))
    {
        Serial.print("Progress = "); Serial.println(value);
        lcd.number("n0") = value;
    }
    progressValue += 5;
    if (progressValue > 100) progressValue = 0;
}
