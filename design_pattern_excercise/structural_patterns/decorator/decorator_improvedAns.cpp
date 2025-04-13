#include <iostream>
#include <memory>
#include <mutex>

class Text {
public:
    virtual ~Text() = default;
    virtual std::string render() const = 0; // 添加const限定，定义为常成员函数，确保函数不会修改对象内部状态
};

class PlainText final : public Text {
public:
    explicit PlainText(std::string content) 
        : content_(std::move(content)) {}

    std::string render() const override {
        std::lock_guard<std::mutex> lock(mtx_);
        return content_;
    }

private:
    mutable std::mutex mtx_;  // 保证线程安全
    std::string content_;
};

class SafeDecorator : public Text {    
public:
    explicit SafeDecorator(std::unique_ptr<Text> t) // 使用智能指针，解决对象所有权模糊的问题。explicit避免隐式类型转换
        : wrapped(std::move(t)) 
    {
        if(!wrapped) throw std::invalid_argument("Invalid wrapped");
    }

    std::string render() const override {
        std::lock_guard<std::mutex> lock(mtx); // 避免多线程访问render可能出现的问题，当前程序不存在问题
        return wrapped->render();
    }

    // 禁用拷贝
    SafeDecorator(const SafeDecorator&) = delete; // 避免拷贝构造函数的浅拷贝，造成悬空指针
    SafeDecorator& operator=(const SafeDecorator&) = delete;
    
    // 允许移动
    SafeDecorator(SafeDecorator&&) = default; // 声明了拷贝构造函数/析构函数的情况下，需要显式地声明移动构造函数
    SafeDecorator& operator=(SafeDecorator&&) = default;

protected:
    std::unique_ptr<Text> wrapped;
    mutable std::mutex mtx;
};

class BoldDecorator : public SafeDecorator
{
public:
    BoldDecorator(std::unique_ptr<Text> t) : SafeDecorator(std::move(t)) {}
    std::string render() const override
    {
        std::lock_guard<std::mutex> lock(mtx);
        return "<b>" + wrapped->render() + "</b>";
    }
};

class ItalicDecorator : public SafeDecorator
{
public:
    ItalicDecorator(std::unique_ptr<Text> t) : SafeDecorator(std::move(t)) {}
    std::string render() const override
    {
        return "<i>" + wrapped->render() + "</i>";
    }
};

class UnderlineDecorator : public SafeDecorator
{
public:
    UnderlineDecorator(std::unique_ptr<Text> t) : SafeDecorator(std::move(t)) {}
    std::string render() const override
    {
        return "<u>" + wrapped->render() + "</u>";
    }
};

int main() {
    auto text = std::make_unique<BoldDecorator>(std::make_unique<ItalicDecorator>
                                                (std::make_unique<PlainText>("Hello World")));
    std::cout << text.get()->render();
    return 0;
}