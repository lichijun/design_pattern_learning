#include <iostream>
#include <string>
#include <stdexcept>

/**
 * parseRole写成私有静态方法的原因
 * 1. 核心逻辑：parseRole 的功能是将字符串转换为枚举值，这一过程仅依赖输入参数（如 "admin"），
 * 无需访问类的非静态成员变量（如 real_file_ 或 role_）。
 * 2. ​静态方法特性：static 方法属于类本身，而非类的实例，因此它无法访问非静态成员，但这也确保了它的独立性。
*/

enum class UserRole
{
    Admin,
    User,
    Guest,
    Unknown
};

class File
{
public:
    virtual ~File() {}

    virtual std::string read() = 0;
    virtual void write(std::string content) = 0;
private:
};

class RealFile : public File
{
public:
    RealFile(std::string fileName) : _fileName(fileName) {}

    std::string read() override
    {
        return "File Content";
    }

    void write(std::string content) override
    {
        std::cout << content << std::endl;
    }

private:
    std::string _fileName;
};

class ProxyFile : public File
{
public:
    ProxyFile(std::string fileName, std::string userLevel)
     : _fileName(fileName)
    {
        pRealFile = new RealFile(fileName);
        _userRole = parseRole(userLevel);
    }

    ~ProxyFile()
    {
        delete pRealFile;
    }

    std::string read() override
    {
        if (_userRole == UserRole::Admin)
        {
            return pRealFile->read();
        }
        else if (_userRole == UserRole::User)
        {
            return pRealFile->read();
        }
        else if (_userRole == UserRole::Guest)
        {
            throw std::runtime_error("Permission denied");
        }
        else
        {
            throw std::runtime_error("Permission denied");
        }
    }

    void write(std::string content) override
    {
        if (_userRole == UserRole::Admin)
        {
            pRealFile->write(content);
        }
        else if (_userRole == UserRole::User)
        {
            throw std::runtime_error("Permission denied");
        }
        else if (_userRole == UserRole::Guest)
        {
            throw std::runtime_error("Permission denied");
        }
        else
        {
            throw std::runtime_error("Permission denied");
        }
    }

private:
    std::string _fileName;
    UserRole _userRole;
    RealFile* pRealFile;

    static UserRole parseRole(const std::string& role_str) {
        if (role_str == "admin") return UserRole::Admin;
        else if (role_str == "user") return UserRole::User;
        else if (role_str == "guest") return UserRole::Guest;
        else return UserRole::Unknown;
    }
};


int main() {
    try {
        // 管理员正常读写
        File* adminFile = new ProxyFile("data.txt", "admin");
        std::cout << adminFile->read() << std::endl; // 输出 File Content
        adminFile->write("New data");      // 输出 Write: New data

        // 普通用户只读
        File* userFile = new ProxyFile("data.txt", "user");
        std::cout << userFile->read() << std::endl;  // 正常读取
        userFile->write("test");           // 抛出异常

        // 访客无权限
        File* guestFile = new ProxyFile("data.txt", "guest");
        guestFile->read();                 // 抛出异常
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}