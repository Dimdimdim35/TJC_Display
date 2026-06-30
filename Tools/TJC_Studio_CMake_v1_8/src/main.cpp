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

struct Component { std::string hmi; std::string cpp; Type type; uint8_t id; };
struct Page { std::string hmi; std::string cpp; uint8_t id; std::vector<Component> components; };

static bool isIdChar(unsigned char c) { return std::isalnum(c) || c == '_'; }
static std::string lowerFirst(std::string s) { if (!s.empty()) s[0] = std::tolower((unsigned char)s[0]); return s; }
static std::string upperFirst(std::string s) { if (!s.empty()) s[0] = std::toupper((unsigned char)s[0]); return s; }

static bool isBtLegacy(const std::string& n)
{
    return n.rfind("bt", 0) == 0 && n.size() >= 3 && std::isdigit((unsigned char)n[2]);
}

static Type detect(const std::string& n)
{
    if (n.rfind("page", 0) == 0 || n.empty()) return Type::Unknown;
    if (isBtLegacy(n)) return Type::DualButton;
    if (n.rfind("bt", 0) == 0 && n.size() >= 3 && std::isupper((unsigned char)n[2])) return Type::DualButton;

    switch(n[0])
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
    switch(t)
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
    switch(t)
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
    switch(t)
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

    if (n.size() >= 2 && std::isdigit((unsigned char)n[1])) return n;
    if (n.size() <= 1) return n;
    return lowerFirst(n.substr(1));
}

static std::string readAfter(const std::string& data, size_t pos)
{
    const std::string key = "objname";
    pos += key.size();

    while (pos < data.size())
    {
        unsigned char c = data[pos];
        if (std::isalpha(c) || c == '_') break;
        ++pos;
    }

    size_t start = pos;

    while (pos < data.size() && isIdChar((unsigned char)data[pos]))
        ++pos;

    if (pos <= start) return {};
    return data.substr(start, pos - start);
}

static uint8_t idBefore(const std::string& data, size_t pos)
{
    if (pos < 5) return 0;
    return (uint8_t)(unsigned char)data[pos - 5];
}

static std::vector<Page> parse(const std::string& path)
{
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("Cannot open HMI");

    std::string data((std::istreambuf_iterator<char>(f)), {});

    std::map<std::string, uint8_t> pageIds;
    std::map<std::string, uint8_t> compIds;

    size_t pos = 0;

    while ((pos = data.find("objname", pos)) != std::string::npos)
    {
        std::string name = readAfter(data, pos);
        uint8_t id = idBefore(data, pos);
        pos += 7;

        if (name.empty()) continue;

        if (name.rfind("page", 0) == 0)
        {
            pageIds[name] = id;
            continue;
        }

        Type t = detect(name);
        if (t == Type::Unknown) continue;

        bool legacy = isBtLegacy(name) || (name.size() >= 2 && std::isdigit((unsigned char)name[1]));
        bool conv = (t == Type::DualButton && name.size() >= 3 && std::isupper((unsigned char)name[2])) ||
                    (t != Type::DualButton && name.size() >= 2 && std::isupper((unsigned char)name[1]));

        if (!legacy && !conv) continue;

        compIds[name] = id;
    }

    if (pageIds.empty()) pageIds["page0"] = 0;

    std::vector<Page> pages;

    for (auto& p: pageIds)
    {
        Page page;
        page.hmi = p.first;
        page.cpp = p.first;
        page.id = p.second;
        pages.push_back(page);
    }

    std::sort(pages.begin(), pages.end(), [](const Page& a, const Page& b){ return a.id < b.id; });

    std::set<Type> convTypes;

    for (auto& kv: compIds)
    {
        Type t = detect(kv.first);
        bool conv = (t == Type::DualButton && kv.first.size() >= 3 && std::isupper((unsigned char)kv.first[2])) ||
                    (t != Type::DualButton && kv.first.size() >= 2 && std::isupper((unsigned char)kv.first[1]));

        if (conv) convTypes.insert(t);
    }

    for (auto& kv: compIds)
    {
        Type t = detect(kv.first);
        bool legacy = isBtLegacy(kv.first) || (kv.first.size() >= 2 && std::isdigit((unsigned char)kv.first[1]));

        if (legacy && convTypes.count(t))
            continue;

        Component c;
        c.hmi = kv.first;
        c.cpp = makeCppName(kv.first, t);
        c.type = t;
        c.id = kv.second;

        pages.front().components.push_back(c);
    }

    std::sort(pages.front().components.begin(), pages.front().components.end(),
        [](const Component& a, const Component& b){ return a.id < b.id; });

    return pages;
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::cout << "Usage: TJC_Studio.exe input.HMI ClassName outputDir\n";
        return 1;
    }

    std::string input = argv[1];
    std::string cls = argv[2];
    std::string outdir = argv[3];

    auto pages = parse(input);
    std::filesystem::create_directories(outdir);

    std::ofstream h(outdir + "/" + cls + ".h");
    std::ofstream cpp(outdir + "/" + cls + ".cpp");
    std::ofstream json(outdir + "/" + cls + ".json");

    h << "// Generated by TJC_Studio v1.8.0\n";
    h << "#ifndef " << cls << "_H\n#define " << cls << "_H\n\n";
    h << "#include <TJC_Display.h>\n\n";

    h << "namespace " << cls << "_ID\n{\n";
    for (auto& p: pages)
    {
        h << "    constexpr uint8_t " << upperFirst(p.cpp) << " = " << (int)p.id << ";\n";
        for (auto& c: p.components)
            h << "    constexpr uint8_t " << upperFirst(c.cpp) << " = " << (int)c.id << ";\n";
    }
    h << "}\n\n";

    for (auto& p: pages)
    {
        std::string pc = cls + "_" + p.cpp;

        h << "class " << pc << "\n{\npublic:\n";
        h << "    explicit " << pc << "(TJC_Display& lcd);\n\n";
        h << "    void show();\n";
        h << "    void handleEvent(const TJC_Event& event);\n\n";

        for (auto& c: p.components)
            h << "    " << cppClass(c.type) << " " << c.cpp << ";\n";

        h << "\nprivate:\n    TJC_Display& _lcd;\n};\n\n";
    }

    h << "class " << cls << "Controller\n{\npublic:\n";
    h << "    virtual ~" << cls << "Controller() = default;\n";
    h << "    virtual void onEvent(const TJC_Event& event) {}\n";

    for (auto& p: pages)
        for (auto& c: p.components)
        {
            h << "    virtual void on" << upperFirst(c.cpp) << "Press() {}\n";
            h << "    virtual void on" << upperFirst(c.cpp) << "Release() {}\n";
        }

    h << "};\n\n";

    h << "class " << cls << "\n{\npublic:\n";
    h << "    explicit " << cls << "(TJC_Display& lcd);\n\n";
    h << "    void update();\n";
    h << "    void processEvents();\n";
    h << "    void setController(" << cls << "Controller* controller);\n";
    h << "    void showPage(uint8_t pageId);\n\n";

    for (auto& p: pages)
        h << "    " << cls << "_" << p.cpp << " " << p.cpp << ";\n";

    h << "\nprivate:\n    TJC_Display& _lcd;\n    " << cls << "Controller* _controller;\n};\n\n";
    h << "#endif\n";

    cpp << "// Generated by TJC_Studio v1.8.0\n";
    cpp << "#include \"" << cls << ".h\"\n\n";

    for (auto& p: pages)
    {
        std::string pc = cls + "_" + p.cpp;

        cpp << pc << "::" << pc << "(TJC_Display& lcd)\n    : _lcd(lcd)";

        for (auto& c: p.components)
        {
            cpp << ",\n      " << c.cpp
                << "(lcd." << factory(c.type)
                << "(\"" << c.hmi << "\", "
                << cls << "_ID::" << upperFirst(c.cpp) << "))";
        }

        cpp << "\n{\n}\n\n";

        cpp << "void " << pc << "::show()\n{\n";
        cpp << "    _lcd.page(" << cls << "_ID::" << upperFirst(p.cpp) << ");\n";
        cpp << "}\n\n";

        cpp << "void " << pc << "::handleEvent(const TJC_Event& event)\n{\n";
        for (auto& c: p.components)
            cpp << "    " << c.cpp << ".handleEvent(event);\n";
        cpp << "}\n\n";
    }

    cpp << cls << "::" << cls << "(TJC_Display& lcd)\n    : _lcd(lcd),\n      _controller(nullptr)";

    for (auto& p: pages)
        cpp << ",\n      " << p.cpp << "(lcd)";

    cpp << "\n{\n}\n\n";

    cpp << "void " << cls << "::update()\n{\n    processEvents();\n}\n\n";

    cpp << "void " << cls << "::setController(" << cls << "Controller* controller)\n{\n";
    cpp << "    _controller = controller;\n";
    cpp << "}\n\n";

    cpp << "void " << cls << "::showPage(uint8_t pageId)\n{\n";
    cpp << "    _lcd.page(pageId);\n";
    cpp << "}\n\n";

    cpp << "void " << cls << "::processEvents()\n{\n";
    cpp << "    _lcd.update();\n";
    cpp << "    TJC_Event event;\n\n";
    cpp << "    while (_lcd.read(event))\n    {\n";

    for (auto& p: pages)
        cpp << "        " << p.cpp << ".handleEvent(event);\n";

    cpp << "\n        if (_controller)\n        {\n";
    cpp << "            _controller->onEvent(event);\n";

    for (auto& p: pages)
        for (auto& c: p.components)
        {
            cpp << "            if (" << p.cpp << "." << c.cpp << ".isPress(event)) _controller->on" << upperFirst(c.cpp) << "Press();\n";
            cpp << "            if (" << p.cpp << "." << c.cpp << ".isRelease(event)) _controller->on" << upperFirst(c.cpp) << "Release();\n";
        }

    cpp << "        }\n";
    cpp << "    }\n";
    cpp << "}\n";

    json << "{\n  \"generator\": \"TJC_Studio v1.8.0\",\n  \"pages\": [\n";
    for (size_t pi = 0; pi < pages.size(); ++pi)
    {
        auto& p = pages[pi];
        json << "    {\n";
        json << "      \"hmiName\": \"" << p.hmi << "\", \"cppName\": \"" << p.cpp << "\", \"id\": " << (int)p.id << ",\n";
        json << "      \"components\": [\n";

        for (size_t ci = 0; ci < p.components.size(); ++ci)
        {
            auto& c = p.components[ci];
            json << "        { \"hmiName\": \"" << c.hmi << "\", \"cppName\": \"" << c.cpp << "\", \"type\": \"" << typeName(c.type) << "\", \"id\": " << (int)c.id << " }";
            if (ci + 1 < p.components.size()) json << ",";
            json << "\n";
        }

        json << "      ]\n";
        json << "    }";
        if (pi + 1 < pages.size()) json << ",";
        json << "\n";
    }
    json << "  ]\n}\n";

    std::cout << "Generated:\n";
    std::cout << "  " << outdir << "\\" << cls << ".h\n";
    std::cout << "  " << outdir << "\\" << cls << ".cpp\n";
    std::cout << "  " << outdir << "\\" << cls << ".json\n";
    std::cout << "Done.\n";

    return 0;
}
