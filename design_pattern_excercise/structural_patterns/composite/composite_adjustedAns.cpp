#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

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
        cout << string(depth * 2 + 1, '-') << " 员工" << _name << endl;
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
        std::cout << std::string(depth * 2 + 1, '-') << " " << _name << std::endl;
        for (const auto& child : _children)
        {
            child->display(depth+1);
        }
    }
    void add(std::unique_ptr<DepartmentComponent> component) override
    {
        _children.push_back(move(component));
    }
    void remove(DepartmentComponent* component) override 
    {
        auto it = std::remove_if(_children.begin(), _children.end(),
            [component](const std::unique_ptr<DepartmentComponent>& ptr) {
                return ptr.get() == component;
            });
        _children.erase(it, _children.end());
    }
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