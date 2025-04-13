#include <iostream>
#include <string>

/**
 * 基本没有问题，需要注意的是构造函数初始化中，成员变量应该在基类初始化后声明
 */

// 实现化接口：渲染API
class Renderer {
public:
    virtual void renderCircle(int x, int y, int r) = 0;
    virtual void renderRect(int x, int y, int w, int h) = 0;
    virtual ~Renderer() = default;
};

// TODO: 实现 OpenGLRenderer 和 VulkanRenderer
class OpenGLRenderer : public Renderer
{
public:
    void renderCircle(int x, int y, int r) override
    {
        std::cout << "OpenGL渲染圆形..." << std::endl;
        std::cout << "圆心位置(" << x << ", " << y << "), 半径：" << r << std::endl;
    }
    void renderRect(int x, int y, int w, int h) override
    {
        std::cout << "OpenGL渲染矩形..." << std::endl;
        std::cout << "中心位置(" << x << ", " << y << "), 宽度：" << w << " 高度：" << h << std::endl;
    }
};

class VulkanRenderer : public Renderer
{
public:
    void renderCircle(int x, int y, int r) override
    {
        std::cout << "Vulkan渲染圆形..." << std::endl;
        std::cout << "圆心位置(" << x << ", " << y << "), 半径：" << r << std::endl;
    }
    void renderRect(int x, int y, int w, int h) override
    {
        std::cout << "Vulkan渲染矩形..." << std::endl;
        std::cout << "中心位置(" << x << ", " << y << "), 宽度：" << w << " 高度：" << h << std::endl;
    }
};

// 抽象化类：图形
class Shape {
protected:
    Renderer* renderer;
    std::string name;
public:
    Shape(Renderer* r, const std::string& n) : renderer(r), name(n) {}
    virtual void draw() = 0; 
    virtual ~Shape() = default;
};

// TODO: 实现 Circle 和 Rectangle 类
class Circle : public Shape
{
public:
    Circle(Renderer* r, int x, int y, int radius) : 
    Shape(r, "Circle"), _x(x), _y(y), _radius(radius) {};
    void draw() override
    {
        renderer->renderCircle(_x, _y, _radius);
    };
private:
    int _x;
    int _y;
    int _radius;
};

class Rectangle : public Shape
{
public:
    Rectangle(Renderer* r, int x, int y, int w, int h) : 
    Shape(r, "Rectangle"), _x(x), _y(y), _w(w), _h(h) {};
    void draw() override
    {
        renderer->renderRect(_x, _y, _w, _h);
    };
private:
    int _x;
    int _y;
    int _w;
    int _h;
};

int main() {
    Renderer* opengl = new OpenGLRenderer();
    Renderer* vulkan = new VulkanRenderer();

    Shape* circle = new Circle(vulkan, 10, 20, 5);
    Shape* rect = new Rectangle(opengl, 30, 40, 8, 6);

    circle->draw();  // 输出：Vulkan渲染圆形...
    rect->draw();    // 输出：OpenGL渲染矩形...

    // 可选：动态切换渲染API
    delete circle;
    circle = new Circle(opengl, 0, 0, 3);
    circle->draw();  // 输出：OpenGL渲染圆形...

    delete opengl;
    delete vulkan;
    delete circle;
    delete rect;
    return 0;
}