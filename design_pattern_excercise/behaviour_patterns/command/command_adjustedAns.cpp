#include <iostream>
#include <vector>
#include <string>
#include <memory>
/***
 * 1. 补充重做逻辑：新命令执行后要清空重做栈
 * 2. 历史记录不合理，应该记录每一次的操作+命令，也不应该通过命令指针做历史记录，历史记录应该类似于一份日志，是静态的
 * 3. 使用unique_ptr保存命令，防止内存泄漏。用unique_ptr是因为命令在每个阶段的所有权是单一的
 *    (客户端->Invoker->重做栈/撤销栈)
 */

// receiver definition
class Light
{
public:
    void On()
    {
        std::cout << "客厅灯开启" << std::endl;
    }
    void Off()
    {
        std::cout << "客厅灯关闭" << std::endl;
    }
};

class AC
{
public:
    void On()
    {
        std::cout << "卧室空调开启（26℃）" << std::endl;
    }
    void Off()
    {
        std::cout << "卧室空调关闭" << std::endl;
    }
};

class Speaker
{
public:
    void On()
    {
        std::cout << "厨房音响开启（音量15）" << std::endl;
    }
    void Off()
    {
        std::cout << "厨房音响关闭" << std::endl;
    }
};

// command definition
class Command
{
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string getInfo() = 0;
};

class LightOnCommand : public Command
{
public:
    LightOnCommand(Light& light) : _light(light) {}
    void execute() override
    {
        _light.On();
    }
    void undo() override
    {
        _light.Off();
    }
    std::string getInfo()
    {
        return "客厅灯打开";
    }
private:
    Light& _light;
};

class LightOffCommand : public Command
{
public:
    LightOffCommand(Light& light) : _light(light) {}
    void execute() override
    {
        _light.Off();
    }
    void undo() override
    {
        _light.On();
    }
    std::string getInfo()
    {
        return "客厅灯关闭";
    }
private:
    Light& _light;
};

class ACOpenCommand : public Command
{
public:
    ACOpenCommand(AC& AC) : _AC(AC) {}
    void execute() override
    {
        _AC.On();
    }
    void undo() override
    {
        _AC.Off();
    }
    std::string getInfo()
    {
        return "卧室空调打开";
    }
private:
    AC& _AC;
};

class SpeakerOnCommand : public Command
{
public:
    SpeakerOnCommand(Speaker& speaker) : _speaker(speaker) {}
    void execute() override
    {
        _speaker.On();
    }
    void undo() override
    {
        _speaker.Off();
    }
    std::string getInfo()
    {
        return "厨房音响打开";
    }
private:
    Speaker& _speaker;
};

class MacroCommand : public Command
{
public:
    void execute() override
    {
        for (auto& cmd : _pCommands)
        {
            cmd->execute();
        }
    }
    void undo() override
    {
        for (auto iter = _pCommands.rbegin(); iter != _pCommands.rend(); iter++)
        {
            iter->get()->undo();
        }
    }
    std::string getInfo()
    {
        return "[Macro] 回家模式";
    }
    void addCommand(std::unique_ptr<Command> cmd)
    {
        _pCommands.push_back(std::move(cmd));
    }
    void clearCommands()
    {
        _pCommands.clear();
    }
private:
    std::vector<std::unique_ptr<Command>> _pCommands{};
};

// invoker definition
struct operationRecord
{
    operationRecord(int seq, std::string opt) : sequence(seq), operation(opt) {}
    int sequence;
    std::string operation;
};

class RemoteControl
{
public:
    void executeCommand(std::unique_ptr<Command> cmd)
    {
        cmd->execute();
        _history.push_back(operationRecord(++operationSequence, cmd->getInfo()));
        _executedCmds.push_back(std::move(cmd));
        _undoedCmds.clear();
    }
    void showHistory()
    {
        std::cout << "\n===== 操作历史 =====" << std::endl;
        for (const auto& record : _history)
        {
            std::cout << record.sequence << ". " << record.operation <<std::endl;
        }
        std::cout << "=================\n" << std::endl;
    }
    void undo()
    {
        if (_executedCmds.size() > 0)
        {
            _executedCmds.back()->undo();
            _history.push_back(operationRecord(++operationSequence, "UNDO:" + _executedCmds.back()->getInfo()));
            _undoedCmds.push_back(std::move(_executedCmds.back()));
            _executedCmds.pop_back();
        }
    }
    void redo()
    {
        if (_undoedCmds.size() > 0)
        {
            _undoedCmds.back()->execute();
            _history.push_back(operationRecord(++operationSequence, "REDO:" + _undoedCmds.back()->getInfo()));
            _executedCmds.push_back(std::move(_undoedCmds.back()));
            _undoedCmds.pop_back();
        }
    }
private:
    std::vector<std::unique_ptr<Command>> _executedCmds{};
    std::vector<std::unique_ptr<Command>> _undoedCmds{};
    std::vector<operationRecord> _history{};
    int operationSequence{0};
};

int main() {
    // 创建设备
    Light livingRoomLight;
    AC bedroomAC;
    Speaker kitchenSpeaker;
    
    // 创建命令
    std::unique_ptr<Command> upLightOn = std::make_unique<LightOnCommand>(livingRoomLight);
    std::unique_ptr<Command> upLightOff = std::make_unique<LightOffCommand>(livingRoomLight);
    std::unique_ptr<Command> upACOpen = std::make_unique<ACOpenCommand>(bedroomAC);
    std::unique_ptr<Command> upspeakerOn = std::make_unique<SpeakerOnCommand>(kitchenSpeaker);

    
    // 创建宏命令：回家模式（开灯+开空调+开音响）
    std::unique_ptr<MacroCommand> upHomeMode = std::make_unique<MacroCommand>();
    upHomeMode->addCommand(std::move(upLightOn));
    upHomeMode->addCommand(std::move(upACOpen));
    upHomeMode->addCommand(std::move(upspeakerOn));
    
    // 遥控器操作
    RemoteControl remote;
    remote.executeCommand(std::move(upHomeMode));   // 执行宏命令
    remote.executeCommand(std::move(upLightOff));   // 关闭客厅灯
    
    remote.showHistory(); // 显示历史记录
    
    remote.undo(); // 撤销关灯
    remote.undo(); // 撤销回家模式
    
    remote.redo(); // 重做回家模式
    remote.showHistory();
}