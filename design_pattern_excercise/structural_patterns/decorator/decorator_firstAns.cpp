#include <iostream>
#include <string>

/** 代码问题总结
 * 1. 保持封装性。尽量不要直接访问成员，应该通过成员方法去访问
 * 2. 保持抽象基类的抽象性。尽量不要在抽象基类中定义具体的成员变量和成员方法
 * 3. 保证装饰器的动态组合与递归委托。装饰器应该在程序运行过程中能够动态地运行，因此应该在render方法中实现装饰效果
 *    而不是在构造函数中实现装饰效果。装饰器应该能被递归调用，因此应该在render方法中先调用被包裹对象的render方法
 * 4. 避免内存泄漏。调用装饰器的析构函数应该释放被包裹对象，从而实现递归地释放内存
 */

// 你的代码在这里
class Text
{
public:
    Text() {}
    Text(const std::string& content) : textContent(content) {}
    ~Text() {}
    virtual std::string render() = 0;

    // 不应该直接访问基类的成员变量，破坏了面向对象的封闭性！应该通过基类方法访问
    // 抽象基类不应该包含具体成员变量textContent
    std::string textContent = std::string(); 
};

class PlainText : public Text
{
public:
    using Text::Text;
    std::string render() override
    {
        return textContent;
    }
};

class TextDecrator : public Text
{
public:
    TextDecrator() {}
    TextDecrator(Text* text) : pText(text) {}
    // 不应该在基类中实现具体的方法，应该保持基类的抽象性！
    std::string render() override
    {
        return textContent;
    }
    // 存在内存泄漏，应该定义虚析构释放被包裹的对象！
protected:
    Text* pText;
};

class UnderlineDecorator : public TextDecrator
{
public:
    // 装饰器模式不应该通过构造函数实现，因为构造函数只能被调用一次，应该通过render函数实现！
    UnderlineDecorator(Text* text) : TextDecrator(text)
    {
        textContent = "<u>" + pText->textContent + "</u>";
    }
};

class ItalicDecorator : public TextDecrator
{
public:
    ItalicDecorator(Text* text) : TextDecrator(text)
    {
        textContent = "<i>" + pText->textContent + "</i>";
    }
};

class BoldDecorator : public TextDecrator
{
public:
    BoldDecorator(Text* text) : TextDecrator(text)
    {
        textContent = "<b>" + pText->textContent + "</b>";
    }
};

int main() {
    Text* text = new PlainText("Hello World");
    
    // 测试用例1：Bold -> Italic -> Underline
    Text* decorated1 = new BoldDecorator(
                        new ItalicDecorator(
                        new UnderlineDecorator(text)));
    std::cout << decorated1->render() << std::endl; // 应输出：<b><i><u>Hello World</u></i></b>
    
    // 测试用例2：Underline -> Italic -> Bold
    Text* decorated2 = new UnderlineDecorator(
                        new ItalicDecorator(
                        new BoldDecorator(
                        new PlainText("Hello World"))));
    std::cout << decorated2->render() << std::endl; // 应输出：<u><i><b>Hello World</b></i></u>

    // 释放内存（可选）
    delete decorated1;
    delete decorated2;
    return 0;
}