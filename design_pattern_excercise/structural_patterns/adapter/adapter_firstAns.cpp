#include <iostream>
#include <string>
#include <algorithm>

class JSONParser
{
public:
    virtual ~JSONParser() = default;
    virtual std::string GetJSONData() const = 0;
};

class LegacyXMLParser
{
public:
    std::string ParseXML()
    {
        return R"(<data><value>42</value><message>Hello, Adapter!</message></data>)";
    }
};

class XMLToJSONAdapter : public JSONParser
{
public:
    XMLToJSONAdapter(LegacyXMLParser* legacyXMLParser) : _legacyXMLParser(legacyXMLParser) {}
    std::string GetJSONData() const override
    {
        auto xml = _legacyXMLParser->ParseXML();
        std::string json = xml;
        replaceAll(json, "<data>", R"({"data": {)");
        replaceAll(json, "</data>", "}}");
        replaceAll(json, "<value>", R"("value": )");
        replaceAll(json, "</value>", ","); // 添加逗号分隔
        replaceAll(json, "<message>", R"("message": ")");
        replaceAll(json, "</message>", "\""); // 闭合引号

        return json;
    }
private:
    LegacyXMLParser* _legacyXMLParser;

    void replaceAll(std::string& str, const std::string& from, const std::string& to) const {
        size_t pos = 0;
        while ((pos = str.find(from, pos)) != std::string::npos) {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }
    }
};

int main() {
    LegacyXMLParser xmlParser;
    // TODO: 通过适配器调用，输出JSON数据
    JSONParser* pAdapter = new XMLToJSONAdapter(&xmlParser);
    std::cout << pAdapter->GetJSONData() << std::endl;
    delete pAdapter;

    return 0;
}