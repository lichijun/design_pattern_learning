#include <iostream>
#include <string>

/** 评价
 * 程序正确地实现了装饰器模式，还有以下几点提升建议：
 * 1. 基类中定义虚析构，避免父类指针指向子类对象导致的析构不完整
 * 2. 保持防御性编程，考虑传参和预期不一致导致的错误
 * 3. 使用智能指针，避免裸指针手动delete造成的重复释放问题，核心是对象所有权模糊的问题
*/
// 你的代码在这里
class Text
{
public:
    virtual ~Text() = default;
    virtual std::string render() = 0;
};

class PlainText : public Text
{
public:
    PlainText(std::string text) : textContent(text) {}
    std::string render() override
    {
        return textContent;
    }

private:
    std::string textContent;
};

class Decorator : public Text
{
public:
    Decorator(Text* t) : wrapped(t) {}
    // 建议防御性编程
    // explicit Decorator(Text* t) : wrapped(t) {
    // if(t == nullptr) throw std::invalid_argument("wrapped不能为空");
    // }
    virtual ~Decorator()
    {
        delete wrapped;
    }

protected:
    Text* wrapped;
};

class BoldDecorator : public Decorator
{
public:
    BoldDecorator(Text* t) : Decorator(t) {}
    std::string render() override
    {
        return "<b>" + wrapped->render() + "</b>";
    }
};

class ItalicDecorator : public Decorator
{
public:
    ItalicDecorator(Text* t) : Decorator(t) {}
    std::string render() override
    {
        return "<i>" + wrapped->render() + "</i>";
    }
};

class UnderlineDecorator : public Decorator
{
public:
    UnderlineDecorator(Text* t) : Decorator(t) {}
    std::string render() override
    {
        return "<u>" + wrapped->render() + "</u>";
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