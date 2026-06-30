# TJC_Studio CMake v1.6.0

Génère maintenant les composants avec leur ID réel :

```cpp
slider(lcd.slider("hSlider", GeneratedUI_ID::Slider))
start(lcd.button("bStart", GeneratedUI_ID::Start))
bt0(lcd.dualButton("bt0", GeneratedUI_ID::Bt0))
```

Cela permet côté Arduino :

```cpp
if (ui.page0.slider.isRelease(event))
{
}
```

Build:

```cmd
build_vs2026_x64.bat
```

Run:

```cmd
build\Release\TJC_Studio.exe examples\01_alias_test.HMI GeneratedUI generated
```
