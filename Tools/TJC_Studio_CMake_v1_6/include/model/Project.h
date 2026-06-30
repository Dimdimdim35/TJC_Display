#ifndef TJC_STUDIO_PROJECT_H
#define TJC_STUDIO_PROJECT_H
#include <string>
#include <vector>
#include "model/Page.h"
struct Project { std::string name; std::vector<Page> pages; };
#endif
