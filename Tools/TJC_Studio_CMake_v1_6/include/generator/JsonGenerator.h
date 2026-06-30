#ifndef TJC_STUDIO_JSONGENERATOR_H
#define TJC_STUDIO_JSONGENERATOR_H
#include <string>
#include "model/Project.h"
class JsonGenerator { public: void generate(const Project& project,const std::string& className,const std::string& outputDir); };
#endif
