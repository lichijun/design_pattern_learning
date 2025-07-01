#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
};

std::string LogLevelToString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::INFO:
        return "INFO";
        break;

    case LogLevel::DEBUG:
    return "DEBUG";
    break;

    case LogLevel::WARNING:
    return "WARNING";
    break;

    case LogLevel::ERROR:
    return "ERROR";
    break;
    
    default:
    return "UNKNOWN";
        break;
    }
}

class AbstractLogger
{
public:
    AbstractLogger(LogLevel level) : _lvl(level) {}

    void SetNextLogger(AbstractLogger* next)
    {
        _pNextLogger = next;
    }

    void LogMessage(LogLevel level, std::string msg)
    {
        if (_lvl >= level)
        {
            write(std::to_string(static_cast<int>(time(0))) + ", [" + LogLevelToString(level) + "]: " + msg);
        }
        else
        {
            if (_pNextLogger)
            {
                _pNextLogger->LogMessage(level, msg);
            }
        }
    }

    virtual void write(std::string msg) = 0;

private:
    LogLevel _lvl;
    AbstractLogger* _pNextLogger{nullptr};
};

class ConsoleLogger : public AbstractLogger
{
public:
    using AbstractLogger::AbstractLogger;
    
    void write(std::string msg) override
    {
        std::cout << msg << std::endl;
    }
};

class FileLogger : public AbstractLogger
{
public:
    FileLogger(LogLevel level, std::string filePath) : AbstractLogger(level)
    {
        _filePath = filePath;
    }

    void write(std::string msg) override
    {
        std::ofstream file(_filePath, std::ios::app);
        if (file.is_open())
        {
            file << msg << std::endl;
            file.close();
        }
        else
        {
            std::cout << "open file: " << _filePath << "failed!" << std::endl;
        }
    }

private:
    std::string _filePath;
};

// 客户端代码
int main() {
    // 1. 创建具体日志处理器
    AbstractLogger* debugLogger = new ConsoleLogger(LogLevel::DEBUG);
    AbstractLogger* infoLogger = new FileLogger(LogLevel::INFO, "app.log");
    AbstractLogger* warningLogger = new ConsoleLogger(LogLevel::WARNING);
    AbstractLogger* errorLogger = new FileLogger(LogLevel::ERROR, "error.log");
    
    // 2. 构建责任链
    debugLogger->SetNextLogger(infoLogger);
    infoLogger->SetNextLogger(warningLogger);
    warningLogger->SetNextLogger(errorLogger);
    
    std::cout << "===== 初始责任链测试 =====" << std::endl;
    AbstractLogger* loggerChain = debugLogger;
    loggerChain->LogMessage(LogLevel::DEBUG, "这是一个调试信息");
    loggerChain->LogMessage(LogLevel::INFO, "这是一个普通信息");
    loggerChain->LogMessage(LogLevel::WARNING, "这是一个警告信息");
    loggerChain->LogMessage(LogLevel::ERROR, "这是一个错误信息");
    
    // 3. 调整责任链顺序
    std::cout << "\n===== 调整后责任链测试 =====" << std::endl;
    warningLogger->SetNextLogger(debugLogger);
    debugLogger->SetNextLogger(errorLogger);
    errorLogger->SetNextLogger(infoLogger);
    infoLogger->SetNextLogger(nullptr);
    
    loggerChain = warningLogger;
    loggerChain->LogMessage(LogLevel::DEBUG, "新的调试信息");
    loggerChain->LogMessage(LogLevel::INFO, "新的普通信息");
    loggerChain->LogMessage(LogLevel::WARNING, "新的警告信息");
    loggerChain->LogMessage(LogLevel::ERROR, "新的错误信息");
    
    // 清理资源
    delete debugLogger;
    delete infoLogger;
    delete warningLogger;
    delete errorLogger;
    
    return 0;
}