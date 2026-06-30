#include "generator/JsonGenerator.h"
#include <filesystem>
#include <fstream>

void JsonGenerator::generate(const Project& project, const std::string& className, const std::string& outputDir)
{
    std::filesystem::create_directories(outputDir);
    std::ofstream out(outputDir + "/" + className + ".json");
    out << "{\n";
    out << "  \"generator\": \"TJC_Studio v1.6.0\",\n";
    out << "  \"project\": \"" << project.name << "\",\n";
    out << "  \"pages\": [\n";
    for (size_t p = 0; p < project.pages.size(); ++p) {
        const auto& page = project.pages[p];
        out << "    {\n";
        out << "      \"hmiName\": \"" << page.hmiName << "\",\n";
        out << "      \"cppName\": \"" << page.cppName << "\",\n";
        out << "      \"components\": [\n";
        for (size_t c = 0; c < page.components.size(); ++c) {
            const auto& component = page.components[c];
            out << "        { \"hmiName\": \"" << component.hmiName
                << "\", \"cppName\": \"" << component.cppName
                << "\", \"type\": \"" << componentTypeToString(component.type)
                << "\", \"id\": " << static_cast<unsigned int>(component.id) << " }";
            if (c + 1 < page.components.size()) out << ",";
            out << "\n";
        }
        out << "      ]\n";
        out << "    }";
        if (p + 1 < project.pages.size()) out << ",";
        out << "\n";
    }
    out << "  ]\n}\n";
}
