#include "parser/HMIParser.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>
#include <map>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <vector>

const char* componentTypeToString(ComponentType type)
{
    switch (type) {
        case ComponentType::Text: return "Text";
        case ComponentType::Number: return "Number";
        case ComponentType::Button: return "Button";
        case ComponentType::DualButton: return "DualButton";
        case ComponentType::Slider: return "Slider";
        case ComponentType::ProgressBar: return "ProgressBar";
        case ComponentType::Picture: return "Picture";
        default: return "Unknown";
    }
}

const char* componentTypeToCppClass(ComponentType type)
{
    switch (type) {
        case ComponentType::Text: return "TJC_Text";
        case ComponentType::Number: return "TJC_Number";
        case ComponentType::Button: return "TJC_Button";
        case ComponentType::DualButton: return "TJC_DualButton";
        case ComponentType::Slider: return "TJC_Slider";
        case ComponentType::ProgressBar: return "TJC_ProgressBar";
        case ComponentType::Picture: return "TJC_Picture";
        default: return "TJC_Component";
    }
}

static std::string lowerFirst(std::string s) {
    if (!s.empty()) s[0] = static_cast<char>(std::tolower(static_cast<unsigned char>(s[0])));
    return s;
}
static bool isIdentifierChar(unsigned char c) { return std::isalnum(c) || c == '_'; }
static bool isLegacyDualButtonBt(const std::string& name) {
    return name.rfind("bt", 0) == 0 && name.size() >= 3 && std::isdigit(static_cast<unsigned char>(name[2]));
}
struct ObjRecord { std::string name; uint8_t id = 0; };

static std::string readStringAfterKey(const std::string& data, size_t keyPos, const std::string& key)
{
    size_t pos = keyPos + key.size();
    while (pos < data.size()) {
        unsigned char c = static_cast<unsigned char>(data[pos]);
        if (std::isalpha(c) || c == '_') break;
        ++pos;
    }
    size_t start = pos;
    while (pos < data.size()) {
        unsigned char c = static_cast<unsigned char>(data[pos]);
        if (!isIdentifierChar(c)) break;
        ++pos;
    }
    if (pos <= start) return {};
    return data.substr(start, pos - start);
}

// Observed HMI pattern:
// ... "id" ... 00 00 00 <id> <length> 00 00 00 "objname" ...
static uint8_t readIdBeforeObjname(const std::string& data, size_t objnamePos)
{
    if (objnamePos < 5) return 0;
    return static_cast<uint8_t>(static_cast<unsigned char>(data[objnamePos - 5]));
}

static std::vector<ObjRecord> extractObjRecords(const std::string& data)
{
    std::vector<ObjRecord> result;
    const std::string key = "objname";
    size_t pos = 0;
    while (true) {
        pos = data.find(key, pos);
        if (pos == std::string::npos) break;
        ObjRecord record;
        record.name = readStringAfterKey(data, pos, key);
        record.id = readIdBeforeObjname(data, pos);
        if (!record.name.empty()) result.push_back(record);
        pos += key.size();
    }
    return result;
}

ComponentType HMIParser::detectType(const std::string& name)
{
    if (name.rfind("page", 0) == 0 || name.empty()) return ComponentType::Unknown;
    if (isLegacyDualButtonBt(name)) return ComponentType::DualButton;
    if (name.rfind("bt", 0) == 0 && name.size() >= 3 && std::isupper(static_cast<unsigned char>(name[2]))) return ComponentType::DualButton;
    static const std::unordered_map<char, ComponentType> typeMap = {
        {'t', ComponentType::Text}, {'n', ComponentType::Number}, {'b', ComponentType::Button},
        {'h', ComponentType::Slider}, {'j', ComponentType::ProgressBar}, {'p', ComponentType::Picture}
    };
    auto it = typeMap.find(name[0]);
    return it == typeMap.end() ? ComponentType::Unknown : it->second;
}

std::string HMIParser::makeCppName(const std::string& hmiName, ComponentType type)
{
    if (type == ComponentType::DualButton && hmiName.rfind("bt", 0) == 0) {
        if (isLegacyDualButtonBt(hmiName)) return hmiName;
        return lowerFirst(hmiName.substr(2));
    }
    if (hmiName.size() >= 2 && std::isdigit(static_cast<unsigned char>(hmiName[1]))) return hmiName;
    if (hmiName.size() <= 1) return hmiName;
    return lowerFirst(hmiName.substr(1));
}

std::string HMIParser::normalizePageName(const std::string& pageName)
{
    if (pageName.rfind("page", 0) == 0) return pageName;
    if (pageName.size() > 1 && pageName[0] == 'p') return lowerFirst(pageName.substr(1));
    return pageName;
}

Project HMIParser::parse(const std::string& path, const std::string& projectName)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open HMI file: " + path);
    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    Project project; project.name = projectName;
    std::vector<ObjRecord> objRecords = extractObjRecords(data);
    std::set<std::string> pageNames;
    std::map<std::string, uint8_t> componentIdByName;

    for (const auto& record : objRecords) {
        const std::string& name = record.name;
        if (name.rfind("page", 0) == 0) { pageNames.insert(name); continue; }
        ComponentType type = detectType(name);
        if (type == ComponentType::Unknown) continue;
        bool legacy = isLegacyDualButtonBt(name) || (name.size() >= 2 && std::isdigit(static_cast<unsigned char>(name[1])));
        bool convention =
            (type == ComponentType::DualButton && name.size() >= 3 && std::isupper(static_cast<unsigned char>(name[2]))) ||
            (type != ComponentType::DualButton && name.size() >= 2 && std::isupper(static_cast<unsigned char>(name[1])));
        if (!legacy && !convention) continue;
        componentIdByName[name] = record.id;
    }

    if (pageNames.empty()) pageNames.insert("page0");
    for (const auto& pageName : pageNames) {
        Page page; page.hmiName = pageName; page.cppName = normalizePageName(pageName);
        project.pages.push_back(page);
    }

    std::set<ComponentType> typesWithConventionNames;
    for (const auto& item : componentIdByName) {
        const std::string& name = item.first;
        ComponentType type = detectType(name);
        bool convention =
            (type == ComponentType::DualButton && name.size() >= 3 && std::isupper(static_cast<unsigned char>(name[2]))) ||
            (type != ComponentType::DualButton && name.size() >= 2 && std::isupper(static_cast<unsigned char>(name[1])));
        if (convention) typesWithConventionNames.insert(type);
    }

    if (project.pages.empty()) return project;
    Page& firstPage = project.pages.front();

    for (const auto& item : componentIdByName) {
        const std::string& name = item.first;
        uint8_t id = item.second;
        ComponentType type = detectType(name);
        bool legacy = isLegacyDualButtonBt(name) || (name.size() >= 2 && std::isdigit(static_cast<unsigned char>(name[1])));
        if (legacy && typesWithConventionNames.count(type)) continue;
        Component component;
        component.hmiName = name;
        component.cppName = makeCppName(name, type);
        component.type = type;
        component.id = id;
        firstPage.components.push_back(component);
    }

    std::sort(firstPage.components.begin(), firstPage.components.end(),
              [](const Component& a, const Component& b) { return a.id < b.id; });
    return project;
}
