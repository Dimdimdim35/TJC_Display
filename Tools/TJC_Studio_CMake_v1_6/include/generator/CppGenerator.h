#ifndef TJC_STUDIO_CPPGENERATOR_H
#define TJC_STUDIO_CPPGENERATOR_H
#include <string>
#include "model/Project.h"
class CppGenerator { public: void generate(const Project& project,const std::string& className,const std::string& outputDir); };
#endif
