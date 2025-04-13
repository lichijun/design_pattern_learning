#include <iostream>
#include <string>
#include <unordered_map>

class TreeType
{
public:
    TreeType(const std::string& name, const std::string& color, const std::string& texture) :
    _name(name), _color(color), _texture(texture) {}

    std::string GetName() const {return _name;}
    std::string GetColor() const {return _color;}
    std::string GetTexture() const {return _texture;}

private:
    std::string _name;
    std::string _color;
    std::string _texture;
};

class Tree
{
public:
    Tree(int x, int y, int age, TreeType* treeType) :
    _x(x), _y(y), _age(age), _treeType(treeType) {}

    void display()
    {
        //Tree: Oak, Color: Green, Position: (10, 20), Age: 5
        std::cout << "Tree: " << _treeType->GetName() << ", "
                  << "Color: " << _treeType->GetColor() << ", "
                  << "Position: (" << _x << ", " << _y << "), Age: " << _age << std::endl; 
    }

private:
    int _x;
    int _y;
    int _age;
    TreeType* _treeType;
};

class TreeFactory
{
public:
    ~TreeFactory()
    {
        for (auto treeType : _treeTypes)
        {
            delete treeType.second;
        }
    }

    TreeType* getTreeType(const std::string& name, const std::string& color, const std::string& texture)
    {
        std::string key = GenerateKey(name, color, texture);
        auto searchRes = _treeTypes.find(key);
        if (searchRes != _treeTypes.end())
        {
            return searchRes->second;
        }
        else
        {
            auto pTreeType = new TreeType(name, color, texture);
            _treeTypes.insert(std::make_pair(key, pTreeType));
            return pTreeType;
        }
    }

private:
    std::unordered_map<std::string, TreeType*> _treeTypes;

    std::string GenerateKey(const std::string& name, const std::string& color, const std::string& texture)
    {
        return name + color + texture;
    }
};


// 客户端测试代码
int main() {
    TreeFactory factory;

    // 创建多棵树，验证享元模式
    TreeType* oakType = factory.getTreeType("Oak", "Green", "OakTexture");
    Tree oak1(10, 20, 5, oakType);
    Tree oak2(30, 40, 5, oakType);
    Tree oak3(50, 60, 5, factory.getTreeType("Oak", "Green", "OakTexture")); 

    TreeType* pineType = factory.getTreeType("Pine", "DarkGreen", "PineTexture");
    Tree pine(100, 200, 10, pineType);

    oak1.display();
    oak2.display();
    oak3.display();
    pine.display();

    // 输出地址验证共享
    std::cout << "Oak Type Address: " << oakType << std::endl;
    std::cout << "Pine Type Address: " << pineType << std::endl;

    return 0;
}