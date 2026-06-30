# TJC_Studio CMake v1.9.0

Version Data Binding.

Génère un sketch de test où le slider est lié automatiquement:

```cpp
ui.page0.slider.bind(ui.page0.value);
ui.page0.slider.bind(ui.page0.progress);
```

Le callback du slider affiche la valeur, mais la synchronisation `Number` et `ProgressBar` est faite par la bibliothèque.

Build:

```cmd
build_vs2026_x64.bat
```

Run:

```cmd
build\Release\TJC_Studio.exe examples\01_alias_test.HMI GeneratedUI generated
```
