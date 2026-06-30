/*
 * TJC_Display Library
 *
 * Example : 02_OutputTest
 * Arduino Mega + TJC4832
 */

#include <TJC_Display.h>

TJC_Display lcd(Serial2);

uint32_t counter = 0;

void setup()
{
    Serial.begin(115200);

    lcd.begin(115200);

    delay(500);

    lcd.page((uint8_t)0);

    lcd.text("t0") = "Test TJC";
    lcd.number("n0") = 0;
    lcd.progress("j0").value(0);

    Serial.println("Output test started");
}

void loop()
{
    lcd.update();

    lcd.number("n0") = counter;
    lcd.progress("j0").value(counter % 100);

    if ((counter % 2) == 0)
        lcd.text("t0") = "PAIR";
    else
        lcd.text("t0") = "IMPAIR";

    counter++;

    delay(500);
}