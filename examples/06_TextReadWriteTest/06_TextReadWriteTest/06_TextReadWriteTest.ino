/*
 * TJC_Display Library
 *
 * Example : 06_TextReadWriteTest
 *
 * Test d'écriture / lecture d'un champ texte
 * Arduino Mega 2560 + TJC4832
 */

#include <TJC_Display.h>

TJC_Display lcd(Serial2);

void setup()
{
    Serial.begin(115200);

    while (!Serial)
        ;

    Serial.println();
    Serial.println("====================================");
    Serial.println(" TJC_Display V1.1 Text Test");
    Serial.println("====================================");

    lcd.begin(115200);

    delay(500);

    lcd.page((uint8_t)0);

    delay(500);

    //-------------------------------------------------
    // Ecriture
    //-------------------------------------------------

    Serial.println("Ecriture de t0...");

    lcd.text("t0") = "XYZ.987";

    delay(300);

    //-------------------------------------------------
    // Lecture
    //-------------------------------------------------

    char buffer[64];

    if (lcd.text("t0").get(buffer, sizeof(buffer)))
    {
        Serial.println();
        Serial.println("Lecture OK");

        Serial.print("Texte lu = [");
        Serial.print(buffer);
        Serial.println("]");

        Serial.print("Longueur = ");
        Serial.println(strlen(buffer));
    }
    else
    {
        Serial.println("Erreur getText()");
    }
}

void loop()
{
  
}