#include <algorithm>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

enum class Type { Text, Number, Button, DualButton, Slider, Progress, Picture, Unknown };

struct Component
{
    std::string hmi;
    std::string cpp;
    Type type;
    uint8_t id;
};

struct Page
{
    std::string hmi;
    std::string cpp;
    uint8_t id;
    std::vector<Component> components;
};

static bool isIdChar(unsigned char c) { return std::isalnum(c) || c == '_'; }

static std::string lowerFirst(std::string s)
{
    if (!s.empty()) s[0] = static_cast<char>(std::tolower(static_cast<unsigned char>(s[0])));
    return s;
}

static std::string upperFirst(std::string s)
{
    if (!s.empty()) s[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
    return s;
}

static bool isBtLegacy(const std::string& n)
{
    return n.rfind("bt", 0) == 0 && n.size() >= 3 && std::isdigit(static_cast<unsigned char>(n[2]));
}

static Type detect(const std::string& n)
{
    if (n.rfind("page", 0) == 0 || n.empty()) return Type::Unknown;
    if (isBtLegacy(n)) return Type::DualButton;
    if (n.rfind("bt", 0) == 0 && n.size() >= 3 && std::isupper(static_cast<unsigned char>(n[2]))) return Type::DualButton;

    switch (n[0])
    {
        case 't': return Type::Text;
        case 'n': return Type::Number;
        case 'b': return Type::Button;
        case 'h': return Type::Slider;
        case 'j': return Type::Progress;
        case 'p': return Type::Picture;
        default: return Type::Unknown;
    }
}

static const char* typeName(Type t)
{
    switch (t)
    {
        case Type::Text: return "Text";
        case Type::Number: return "Number";
        case Type::Button: return "Button";
        case Type::DualButton: return "DualButton";
        case Type::Slider: return "Slider";
        case Type::Progress: return "ProgressBar";
        case Type::Picture: return "Picture";
        default: return "Unknown";
    }
}

static const char* cppClass(Type t)
{
    switch (t)
    {
        case Type::Text: return "TJC_Text";
        case Type::Number: return "TJC_Number";
        case Type::Button: return "TJC_Button";
        case Type::DualButton: return "TJC_DualButton";
        case Type::Slider: return "TJC_Slider";
        case Type::Progress: return "TJC_ProgressBar";
        case Type::Picture: return "TJC_Picture";
        default: return "TJC_Component";
    }
}

static const char* factory(Type t)
{
    switch (t)
    {
        case Type::Text: return "text";
        case Type::Number: return "number";
        case Type::Button: return "button";
        case Type::DualButton: return "dualButton";
        case Type::Slider: return "slider";
        case Type::Progress: return "progress";
        case Type::Picture: return "picture";
        default: return "";
    }
}

static std::string makeCppName(const std::string& n, Type t)
{
    if (t == Type::DualButton && n.rfind("bt", 0) == 0)
    {
        if (isBtLegacy(n)) return n;
        return lowerFirst(n.substr(2));
    }

    if (n.size() >= 2 && std::isdigit(static_cast<unsigned char>(n[1]))) return n;
    if (n.size() <= 1) return n;
    return lowerFirst(n.substr(1));
}

static std::string readAfterObjname(const std::string& data, size_t pos)
{
    const std::string key = "objname";
    pos += key.size();

    while (pos < data.size())
    {
        unsigned char c = static_cast<unsigned char>(data[pos]);
        if (std::isalpha(c) || c == '_') break;
        ++pos;
    }

    size_t start = pos;

    while (pos < data.size() && isIdChar(static_cast<unsigned char>(data[pos]))) ++pos;

    if (pos <= start) return {};
    return data.substr(start, pos - start);
}

static uint8_t idBeforeObjname(const std::string& data, size_t pos)
{
    if (pos < 5) return 0;
    return static_cast<uint8_t>(static_cast<unsigned char>(data[pos - 5]));
}

static std::vector<Page> parseHmi(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open HMI file");

    std::string data((std::istreambuf_iterator<char>(file)), {});

    std::map<std::string, uint8_t> pageIds;
    std::map<std::string, uint8_t> componentIds;

    size_t pos = 0;

    while ((pos = data.find("objname", pos)) != std::string::npos)
    {
        std::string name = readAfterObjname(data, pos);
        uint8_t id = idBeforeObjname(data, pos);

        pos += 7;

        if (name.empty()) continue;

        if (name.rfind("page", 0) == 0)
        {
            pageIds[name] = id;
            continue;
        }

        Type type = detect(name);
        if (type == Type::Unknown) continue;

        bool legacy = isBtLegacy(name) || (name.size() >= 2 && std::isdigit(static_cast<unsigned char>(name[1])));
        bool convention =
            (type == Type::DualButton && name.size() >= 3 && std::isupper(static_cast<unsigned char>(name[2]))) ||
            (type != Type::DualButton && name.size() >= 2 && std::isupper(static_cast<unsigned char>(name[1])));

        if (!legacy && !convention) continue;

        componentIds[name] = id;
    }

    if (pageIds.empty()) pageIds["page0"] = 0;

    std::vector<Page> pages;

    for (const auto& p : pageIds)
    {
        Page page;
        page.hmi = p.first;
        page.cpp = p.first;
        page.id = p.second;
        pages.push_back(page);
    }

    std::sort(pages.begin(), pages.end(), [](const Page& a, const Page& b) { return a.id < b.id; });

    std::set<Type> conventionTypes;

    for (const auto& item : componentIds)
    {
        const std::string& name = item.first;
        Type type = detect(name);

        bool convention =
            (type == Type::DualButton && name.size() >= 3 && std::isupper(static_cast<unsigned char>(name[2]))) ||
            (type != Type::DualButton && name.size() >= 2 && std::isupper(static_cast<unsigned char>(name[1])));

        if (convention) conventionTypes.insert(type);
    }

    for (const auto& item : componentIds)
    {
        const std::string& name = item.first;
        Type type = detect(name);

        bool legacy = isBtLegacy(name) || (name.size() >= 2 && std::isdigit(static_cast<unsigned char>(name[1])));

        if (legacy && conventionTypes.count(type)) continue;

        Component component;
        component.hmi = name;
        component.cpp = makeCppName(name, type);
        component.type = type;
        component.id = item.second;

        pages.front().components.push_back(component);
    }

    std::sort(pages.front().components.begin(), pages.front().components.end(),
              [](const Component& a, const Component& b) { return a.id < b.id; });

    return pages;
}

static bool hasComponent(const std::vector<Page>& pages, const std::string& cppName)
{
    for (const auto& page : pages)
        for (const auto& component : page.components)
            if (component.cpp == cppName)
                return true;

    return false;
}

static void generateIno(const std::string& dir, const std::string& className, const std::vector<Page>& pages)
{
    std::ofstream ino(dir + "/" + className + ".ino");

    ino << "#include <TJC_Display.h>\n";
    ino << "#include \"" << className << ".h\"\n\n";
    ino << "TJC_Display lcd(Serial2);\n";
    ino << className << " ui(lcd);\n\n";

    if (hasComponent(pages, "start"))
    {
        ino << "void onStart()\n{\n";
        ino << "    Serial.println(\"Callback START\");\n";
        ino << "}\n\n";
    }

    if (hasComponent(pages, "slider"))
    {
        ino << "void onSlider()\n{\n";
        ino << "    uint32_t v = ui.page0.slider.read();\n";
        ino << "    Serial.print(\"Callback SLIDER = \");\n";
        ino << "    Serial.println(v);\n";
        ino << "}\n\n";
    }

    if (hasComponent(pages, "bt0"))
    {
        ino << "void onDual()\n{\n";
        ino << "    uint32_t state = ui.page0.bt0.read();\n";
        ino << "    Serial.print(\"Callback DUAL = \");\n";
        ino << "    Serial.println(state);\n\n";
        if (hasComponent(pages, "title"))
        {
            ino << "    if (state)\n";
            ino << "        ui.page0.title = \"DUAL ON\";\n";
            ino << "    else\n";
            ino << "        ui.page0.title = \"DUAL OFF\";\n";
        }
        ino << "}\n\n";
    }

    ino << "class MyController : public " << className << "Controller\n{\npublic:\n";

    if (hasComponent(pages, "start"))
        ino << "    void onStartRelease() override { Serial.println(\"Controller START\"); }\n";

    if (hasComponent(pages, "slider"))
        ino << "    void onSliderRelease() override { Serial.println(\"Controller SLIDER\"); }\n";

    if (hasComponent(pages, "bt0"))
        ino << "    void onBt0Release() override { Serial.println(\"Controller DUAL\"); }\n";

    ino << "};\n\n";
    ino << "MyController controller;\n\n";

    ino << "void setup()\n{\n";
    ino << "    Serial.begin(115200);\n";
    ino << "    lcd.begin(115200);\n";
    ino << "    delay(500);\n\n";
    ino << "    ui.page0.show();\n\n";

    if (hasComponent(pages, "title"))
        ino << "    ui.page0.title = \"TJC v2.0\";\n";

    if (hasComponent(pages, "value"))
        ino << "    ui.page0.value = 50;\n";

    if (hasComponent(pages, "slider"))
        ino << "    ui.page0.slider.value(50);\n";

    if (hasComponent(pages, "progress"))
        ino << "    ui.page0.progress.value(50);\n";

    if (hasComponent(pages, "start"))
        ino << "    ui.page0.start.text(\"START\");\n";

    if (hasComponent(pages, "bt0"))
        ino << "    ui.page0.bt0.off();\n";

    ino << "\n";

    if (hasComponent(pages, "slider") && hasComponent(pages, "value"))
        ino << "    ui.page0.slider.bind(ui.page0.value);\n";

    if (hasComponent(pages, "slider") && hasComponent(pages, "progress"))
        ino << "    ui.page0.slider.bind(ui.page0.progress);\n";

    ino << "\n";

    if (hasComponent(pages, "start"))
        ino << "    ui.page0.start.onRelease(onStart);\n";

    if (hasComponent(pages, "slider"))
        ino << "    ui.page0.slider.onRelease(onSlider);\n";

    if (hasComponent(pages, "bt0"))
        ino << "    ui.page0.bt0.onRelease(onDual);\n";

    ino << "\n";
    ino << "    ui.setController(&controller);\n\n";
    ino << "    Serial.println(\"===== TJC v2.0 generated Arduino project =====\");\n";
    ino << "}\n\n";

    ino << "void loop()\n{\n";
    ino << "    ui.update();\n";
    ino << "}\n";
}

static void generateReadme(const std::string& dir, const std::string& className)
{
    std::ofstream out(dir + "/README.md");

    out << "# " << className << " Arduino Project\n\n";
    out << "Generated by TJC_Studio v2.0.0.\n\n";
    out << "Open `" << className << ".ino` in Arduino IDE and compile.\n\n";
    out << "Requires `TJC_Display` v2.0.0 or newer.\n";
}

static void generatePlatformIO(const std::string& dir)
{
    std::ofstream out(dir + "/platformio.ini");

    out << "[env:megaatmega2560]\n";
    out << "platform = atmelavr\n";
    out << "board = megaatmega2560\n";
    out << "framework = arduino\n";
    out << "monitor_speed = 115200\n";
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::cout << "Usage:\n";
        std::cout << "  TJC_Studio.exe input.HMI ClassName outputDir\n\n";
        std::cout << "Example:\n";
        std::cout << "  TJC_Studio.exe examples\\01_alias_test.HMI GeneratedUI generated\n";
        return 1;
    }

    try
    {
        std::string input = argv[1];
        std::string className = argv[2];
        std::string outputRoot = argv[3];
        std::string outputDir = outputRoot + "/" + className;

        auto pages = parseHmi(input);

        std::filesystem::create_directories(outputDir);

        std::ofstream h(outputDir + "/" + className + ".h");
        std::ofstream cpp(outputDir + "/" + className + ".cpp");
        std::ofstream json(outputDir + "/" + className + ".json");

        h << "// Generated by TJC_Studio v2.0.0\n";
        h << "#ifndef " << className << "_H\n";
        h << "#define " << className << "_H\n\n";
        h << "#include <TJC_Display.h>\n\n";

        h << "namespace " << className << "_ID\n{\n";
        for (const auto& page : pages)
        {
            h << "    constexpr uint8_t " << upperFirst(page.cpp) << " = " << static_cast<int>(page.id) << ";\n";
            for (const auto& component : page.components)
                h << "    constexpr uint8_t " << upperFirst(component.cpp) << " = " << static_cast<int>(component.id) << ";\n";
        }
        h << "}\n\n";

        for (const auto& page : pages)
        {
            std::string pageClass = className + "_" + page.cpp;

            h << "class " << pageClass << "\n{\npublic:\n";
            h << "    explicit " << pageClass << "(TJC_Display& lcd);\n\n";
            h << "    void show();\n";
            h << "    void handleEvent(const TJC_Event& event);\n\n";

            for (const auto& component : page.components)
                h << "    " << cppClass(component.type) << " " << component.cpp << ";\n";

            h << "\nprivate:\n";
            h << "    TJC_Display& _lcd;\n";
            h << "};\n\n";
        }

        h << "class " << className << "Controller\n{\npublic:\n";
        h << "    virtual ~" << className << "Controller() = default;\n";
        h << "    virtual void onEvent(const TJC_Event& event) {}\n";

        for (const auto& page : pages)
        {
            for (const auto& component : page.components)
            {
                h << "    virtual void on" << upperFirst(component.cpp) << "Press() {}\n";
                h << "    virtual void on" << upperFirst(component.cpp) << "Release() {}\n";
            }
        }

        h << "};\n\n";

        h << "class " << className << "\n{\npublic:\n";
        h << "    explicit " << className << "(TJC_Display& lcd);\n\n";
        h << "    void update();\n";
        h << "    void processEvents();\n";
        h << "    void setController(" << className << "Controller* controller);\n";
        h << "    void showPage(uint8_t pageId);\n\n";

        for (const auto& page : pages)
            h << "    " << className << "_" << page.cpp << " " << page.cpp << ";\n";

        h << "\nprivate:\n";
        h << "    TJC_Display& _lcd;\n";
        h << "    " << className << "Controller* _controller;\n";
        h << "};\n\n";
        h << "#endif\n";

        cpp << "// Generated by TJC_Studio v2.0.0\n";
        cpp << "#include \"" << className << ".h\"\n\n";

        for (const auto& page : pages)
        {
            std::string pageClass = className + "_" + page.cpp;

            cpp << pageClass << "::" << pageClass << "(TJC_Display& lcd)\n    : _lcd(lcd)";

            for (const auto& component : page.components)
            {
                cpp << ",\n      " << component.cpp
                    << "(lcd." << factory(component.type)
                    << "(\"" << component.hmi << "\", "
                    << className << "_ID::" << upperFirst(component.cpp)
                    << "))";
            }

            cpp << "\n{\n}\n\n";

            cpp << "void " << pageClass << "::show()\n{\n";
            cpp << "    _lcd.page(" << className << "_ID::" << upperFirst(page.cpp) << ");\n";
            cpp << "}\n\n";

            cpp << "void " << pageClass << "::handleEvent(const TJC_Event& event)\n{\n";
            for (const auto& component : page.components)
                cpp << "    " << component.cpp << ".handleEvent(event);\n";
            cpp << "}\n\n";
        }

        cpp << className << "::" << className << "(TJC_Display& lcd)\n    : _lcd(lcd),\n      _controller(nullptr)";

        for (const auto& page : pages)
            cpp << ",\n      " << page.cpp << "(lcd)";

        cpp << "\n{\n}\n\n";

        cpp << "void " << className << "::update()\n{\n";
        cpp << "    processEvents();\n";
        cpp << "}\n\n";

        cpp << "void " << className << "::setController(" << className << "Controller* controller)\n{\n";
        cpp << "    _controller = controller;\n";
        cpp << "}\n\n";

        cpp << "void " << className << "::showPage(uint8_t pageId)\n{\n";
        cpp << "    _lcd.page(pageId);\n";
        cpp << "}\n\n";

        cpp << "void " << className << "::processEvents()\n{\n";
        cpp << "    _lcd.update();\n";
        cpp << "    TJC_Event event;\n\n";
        cpp << "    while (_lcd.read(event))\n";
        cpp << "    {\n";

        for (const auto& page : pages)
            cpp << "        " << page.cpp << ".handleEvent(event);\n";

        cpp << "\n";
        cpp << "        if (_controller)\n";
        cpp << "        {\n";
        cpp << "            _controller->onEvent(event);\n";

        for (const auto& page : pages)
        {
            for (const auto& component : page.components)
            {
                cpp << "            if (" << page.cpp << "." << component.cpp
                    << ".isPress(event)) _controller->on" << upperFirst(component.cpp) << "Press();\n";

                cpp << "            if (" << page.cpp << "." << component.cpp
                    << ".isRelease(event)) _controller->on" << upperFirst(component.cpp) << "Release();\n";
            }
        }

        cpp << "        }\n";
        cpp << "    }\n";
        cpp << "}\n";

        json << "{\n";
        json << "  \"generator\": \"TJC_Studio v2.0.0\",\n";
        json << "  \"pages\": [\n";

        for (size_t pi = 0; pi < pages.size(); ++pi)
        {
            const auto& page = pages[pi];

            json << "    {\n";
            json << "      \"hmiName\": \"" << page.hmi
                 << "\", \"cppName\": \"" << page.cpp
                 << "\", \"id\": " << static_cast<int>(page.id)
                 << ",\n";
            json << "      \"components\": [\n";

            for (size_t ci = 0; ci < page.components.size(); ++ci)
            {
                const auto& component = page.components[ci];

                json << "        { \"hmiName\": \"" << component.hmi
                     << "\", \"cppName\": \"" << component.cpp
                     << "\", \"type\": \"" << typeName(component.type)
                     << "\", \"id\": " << static_cast<int>(component.id)
                     << " }";

                if (ci + 1 < page.components.size()) json << ",";
                json << "\n";
            }

            json << "      ]\n";
            json << "    }";

            if (pi + 1 < pages.size()) json << ",";
            json << "\n";
        }

        json << "  ]\n";
        json << "}\n";

        generateIno(outputDir, className, pages);
        generateReadme(outputDir, className);
        generatePlatformIO(outputDir);

        std::cout << "Generated Arduino project:\n";
        std::cout << "  " << outputDir << "\\" << className << ".ino\n";
        std::cout << "  " << outputDir << "\\" << className << ".h\n";
        std::cout << "  " << outputDir << "\\" << className << ".cpp\n";
        std::cout << "  " << outputDir << "\\" << className << ".json\n";
        std::cout << "  " << outputDir << "\\README.md\n";
        std::cout << "  " << outputDir << "\\platformio.ini\n";
        std::cout << "Done.\n";

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
