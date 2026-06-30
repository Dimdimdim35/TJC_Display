# API

## Generated UI

```cpp
TJC_Display lcd(Serial2);
GeneratedUI ui(lcd);

void loop()
{
    ui.update();
}
```

## Callbacks

```cpp
ui.page0.start.onRelease(onStart);
ui.page0.slider.onRelease(onSlider);
ui.page0.bt0.onRelease(onDual);
```

## Controller

```cpp
class MyController : public GeneratedUIController
{
public:
    void onStartRelease() override {}
    void onSliderRelease() override {}
    void onBt0Release() override {}
};
```

## Data binding

```cpp
ui.page0.slider.bind(ui.page0.value);
ui.page0.slider.bind(ui.page0.progress);
```

## Pages

```cpp
ui.page0.show();
ui.showPage(GeneratedUI_ID::Page0);
```
