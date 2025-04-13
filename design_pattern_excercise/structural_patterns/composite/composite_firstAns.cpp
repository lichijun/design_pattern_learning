#include <iostream>
#include <memory>
#include <vector>

using namespace std;

/**
 * 基本正确，display格式有点问题，remove函数没有实现，不建议用using namespace std可能导致命名冲突
 * remove_if函数并不会直接删除元素，而是会把符合条件的元素放到容器的末尾，最后反馈的迭代器指向第一个符合删除条件的元素，
 * 需要配合erase才能真正删除元素
 * string(3, "-")可以构造出三个小短横的字符串
 * 
 * 关于组合模式：组合模式通过树状结构组合单个对象，从而实现对单个对象(Leaf)和组合对象(Composite)的统一操作，
 *             单个对象和组合对象都继承于抽象类component
 */

class DepartmentComponent
{
public:
    virtual int calculateSalary() = 0;
    virtual void display(int depth = 0) = 0;
    virtual void add(std::unique_ptr<DepartmentComponent> component) {}
    virtual void remove(DepartmentComponent* component) {}
};

class Employee : public DepartmentComponent
{
public:
    Employee(string name, int salary) : _name(name), _salary(salary) {}
    int calculateSalary() override
    {
        return _salary;
    }
    void display(int depth) override
    {
        for (int i = 0; i < depth * 2 + 1; ++i)
        {
            cout << "-";
        }
        cout << "员工" << _name <<endl;
    }

private:
    string _name;
    int _salary;
};

class Department : public DepartmentComponent
{
public:
    Department(string name) : _name(name) {}
    int calculateSalary() override
    {
        int totalSalary = 0;
        for (const auto& child : _children)
        {
            totalSalary += child->calculateSalary();
        }
        return totalSalary;
    }
    void display(int depth) override
    {
        for (int i = 0; i < depth * 2 + 1; ++i)
        {
            cout << "-";
        }
        cout << _name << endl;
        for (const auto& child : _children)
        {
            child->display(depth+1);
        }
    }
    void add(std::unique_ptr<DepartmentComponent> component) override
    {
        _children.push_back(move(component));
    }
    // void remove(DepartmentComponent* component)
private:
    string _name;
    vector<std::unique_ptr<DepartmentComponent>> _children;
};

int main()
{
    unique_ptr<DepartmentComponent> auditGroup = make_unique<Department>("子部门审计组");
    auditGroup->add(make_unique<Employee>("赵六", 6000));
    auditGroup->add(make_unique<Employee>("陈七", 6000));
    unique_ptr<DepartmentComponent> financialDep = make_unique<Department>("财务部");
    financialDep->add(make_unique<Employee>("王五", 8000));
    financialDep->add(move(auditGroup));
    unique_ptr<DepartmentComponent> techDep = make_unique<Department>("技术部");
    techDep->add(make_unique<Employee>("张三", 10000));
    techDep->add(make_unique<Employee>("李四", 12000));
    unique_ptr<DepartmentComponent> company = make_unique<Department>("总公司");
    company->add(move(techDep));
    company->add(move(financialDep));
    company->display();
    cout << "总公司总薪资：" << to_string(company->calculateSalary()) << endl;

    return 0;
}