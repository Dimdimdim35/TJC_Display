#include <TJC_Display.h>
#include "GeneratedUI.h"

TJC_Display lcd(Serial2);
GeneratedUI ui(lcd);

void onStart()
{
    Serial.println("Callback START");
}

void onSlider()
{
    uint32_t v = ui.page0.slider.read();

    Serial.print("Callback SLIDER = ");
    Serial.println(v);

    // v1.9: value and progress are updated automatically by binding.
}

void onDual()
{
    uint32_t state = ui.page0.bt0.read();

    Serial.print("Callback DUAL = ");
    Serial.println(state);

    if (state)
        ui.page0.title = "DUAL ON";
    else
        ui.page0.title = "DUAL OFF";
}

class MyController : public GeneratedUIController
{
public:
    void onStartRelease() override
    {
        Serial.println("Controller START");
    }

    void onSliderRelease() override
    {
        Serial.println("Controller SLIDER");
    }

    void onBt0Release() override
    {
        Serial.println("Controller DUAL");
    }
};

MyController controller;

void setup()
{
    Serial.begin(115200);
    lcd.begin(115200);

    delay(500);

    ui.page0.show();

    ui.page0.title = "TJC v1.9";
    ui.page0.value = 50;
    ui.page0.slider.value(50);
    ui.page0.progress.value(50);
    ui.page0.start.text("START");
    ui.page0.bt0.off();

    ui.page0.slider.bind(ui.page0.value);
    ui.page0.slider.bind(ui.page0.progress);

    ui.page0.start.onRelease(onStart);
    ui.page0.slider.onRelease(onSlider);
    ui.page0.bt0.onRelease(onDual);

    ui.setController(&controller);

    Serial.println("===== TJC v1.9 data binding validation =====");
}

void loop()
{
    ui.update();
}
