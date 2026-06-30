#ifndef TJC_STUDIO_COMPONENT_H
#define TJC_STUDIO_COMPONENT_H
#include <string>
#include <cstdint>
enum class ComponentType { Unknown, Text, Number, Button, DualButton, Slider, ProgressBar, Picture };
struct Component { std::string hmiName; std::string cppName; ComponentType type = ComponentType::Unknown; uint8_t id = 0; };
const char* componentTypeToString(ComponentType type);
const char* componentTypeToCppClass(ComponentType type);
#endif
