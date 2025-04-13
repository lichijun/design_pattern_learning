#include <iostream>
#include <string>
#include <stdexcept>

/**
 * 基本正确，传入的用户等级，建议用枚举代替string类型的硬编码，提高代码可维护性
*/

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
     : _fileName(fileName), _userLevel(userLevel) 
    {
        pRealFile = new RealFile(fileName);
    }

    ~ProxyFile()
    {
        delete pRealFile;
    }

    std::string read() override
    {
        if (_userLevel == "admin")
        {
            return pRealFile->read();
        }
        else if (_userLevel == "user")
        {
            return pRealFile->read();
        }
        else if (_userLevel == "guest")
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
        if (_userLevel == "admin")
        {
            pRealFile->write(content);
        }
        else if (_userLevel == "user")
        {
            throw std::runtime_error("Permission denied");
        }
        else if (_userLevel == "guest")
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
    std::string _userLevel;
    RealFile* pRealFile;
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