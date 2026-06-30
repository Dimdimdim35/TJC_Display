#ifndef TJC_TYPES_H
#define TJC_TYPES_H

#include <Arduino.h>

#ifndef TJC_DEFAULT_TIMEOUT
#define TJC_DEFAULT_TIMEOUT 1000
#endif

enum class TJC_HAlign : uint8_t { Left = 0, Center = 1, Right = 2 };
enum class TJC_VAlign : uint8_t { Top = 0, Center = 1, Bottom = 2 };

#endif
