#ifndef TJC_STUDIO_PAGE_H
#define TJC_STUDIO_PAGE_H
#include <string>
#include <vector>
#include "model/Component.h"
struct Page { std::string hmiName; std::string cppName; std::vector<Component> components; };
#endif
