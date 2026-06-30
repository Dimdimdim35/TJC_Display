#include <iostream>
#include <string>
#include "parser/HMIParser.h"
#include "generator/CppGenerator.h"
#include "generator/JsonGenerator.h"

static int countType(const Project& project, ComponentType type) {
    int count = 0;
    for (const auto& page : project.pages)
        for (const auto& component : page.components)
            if (component.type == type) count++;
    return count;
}
static int countComponents(const Project& project) {
    int count = 0;
    for (const auto& page : project.pages) count += static_cast<int>(page.components.size());
    return count;
}

int main(int argc, char** argv)
{
    if (argc < 4) {
        std::cout << "Usage:\n  TJC_Studio.exe input.HMI ClassName outputDir\n";
        return 1;
    }
    try {
        std::string input = argv[1], className = argv[2], outputDir = argv[3];
        HMIParser parser;
        Project project = parser.parse(input, className);

        std::cout << "Project model:\n";
        std::cout << "  Pages      : " << project.pages.size() << "\n";
        std::cout << "  Components : " << countComponents(project) << "\n";
        std::cout << "  Text       : " << countType(project, ComponentType::Text) << "\n";
        std::cout << "  Number     : " << countType(project, ComponentType::Number) << "\n";
        std::cout << "  Button     : " << countType(project, ComponentType::Button) << "\n";
        std::cout << "  DualButton : " << countType(project, ComponentType::DualButton) << "\n";
        std::cout << "  Slider     : " << countType(project, ComponentType::Slider) << "\n";
        std::cout << "  Progress   : " << countType(project, ComponentType::ProgressBar) << "\n";
        std::cout << "  Picture    : " << countType(project, ComponentType::Picture) << "\n\n";

        CppGenerator cpp; cpp.generate(project, className, outputDir);
        JsonGenerator json; json.generate(project, className, outputDir);

        std::cout << "Generated:\n";
        std::cout << "  " << outputDir << "\\" << className << ".h\n";
        std::cout << "  " << outputDir << "\\" << className << ".cpp\n";
        std::cout << "  " << outputDir << "\\" << className << ".json\n";
        std::cout << "Done.\n";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
