# TJC_Display v2.0.0 Stable

Stable release of the TJC Display Arduino framework and TJC_Studio generator.

## Contents

```text
TJC_Display/     Arduino library
TJC_Studio/      CMake generator
Docs/            Documentation
Examples/        HMI test files and examples
CHANGELOG.md     Release notes
```

## Quick start

1. Copy `TJC_Display/` into:

```text
Documents/Arduino/libraries/
```

2. Build TJC_Studio:

```cmd
cd TJC_Studio
build_vs2026_x64.bat
```

3. Generate a full Arduino project:

```cmd
build\Release\TJC_Studio.exe examples\01_alias_test.HMI GeneratedUI generated
```

4. Open:

```text
generated/GeneratedUI/GeneratedUI.ino
```

No Arduino folder rename dialog should appear.

## Validated features

- Real HMI ID extraction
- `GeneratedUI_ID`
- Object API
- `ui.update()`
- Callbacks
- Generated controller
- Smart pages
- Slider data binding
- Complete Arduino project generation
