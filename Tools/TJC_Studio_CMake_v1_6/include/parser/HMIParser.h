#ifndef TJC_STUDIO_HMIPARSER_H
#define TJC_STUDIO_HMIPARSER_H
#include <string>
#include "model/Project.h"
class HMIParser {
public:
    Project parse(const std::string& path, const std::string& projectName);
private:
    static ComponentType detectType(const std::string& name);
    static std::string makeCppName(const std::string& hmiName, ComponentType type);
    static std::string normalizePageName(const std::string& pageName);
};
#endif
