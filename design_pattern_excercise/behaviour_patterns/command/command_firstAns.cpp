#include <iostream>
#include <vector>
#include <string>

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
        for (auto cmd : _pCommands)
        {
            cmd->execute();
        }
    }
    void undo() override
    {
        for (auto cmd : _pCommands)
        {
            cmd->undo();
        }
    }
    std::string getInfo()
    {
        return "[Macro] 回家模式";
    }
    void addCommand(Command* cmd)
    {
        _pCommands.push_back(cmd);
    }
    void clearCommands()
    {
        _pCommands.clear();
    }
private:
    std::vector<Command*> _pCommands{};
};

// invoker definition
class RemoteControl
{
public:
    void executeCommand(Command* cmd)
    {
        cmd->execute();
        _executedCmds.push_back(cmd);
    }
    void showHistory()
    {
        for (int i = 0; i < _executedCmds.size(); i++)
        {
            std::cout << std::to_string(i + 1) << ". " << _executedCmds[i]->getInfo() << std::endl;
        }
    }
    void undo()
    {
        if (_executedCmds.size() > 0)
        {
            _executedCmds.back()->undo();
            _undoedCmds.push_back(_executedCmds.back());
            _executedCmds.pop_back();
        }
    }
    void redo()
    {
        if (_undoedCmds.size() > 0)
        {
            executeCommand(_undoedCmds.back());
            _undoedCmds.pop_back();
        }
    }
private:
    std::vector<Command*> _executedCmds{};
    std::vector<Command*> _undoedCmds{};
};

int main() {
    // 创建设备
    Light livingRoomLight;
    AC bedroomAC;
    Speaker kitchenSpeaker;
    
    // 创建命令
    LightOnCommand lightOn(livingRoomLight);
    LightOffCommand lightOff(livingRoomLight);
    ACOpenCommand acOpen(bedroomAC);
    SpeakerOnCommand speakerOn(kitchenSpeaker);
    
    // 创建宏命令：回家模式（开灯+开空调+开音响）
    MacroCommand homeMode;
    homeMode.addCommand(&lightOn);
    homeMode.addCommand(&acOpen);
    homeMode.addCommand(&speakerOn);
    
    // 遥控器操作
    RemoteControl remote;
    remote.executeCommand(&homeMode);   // 执行宏命令
    remote.executeCommand(&lightOff);   // 关闭客厅灯
    
    remote.showHistory(); // 显示历史记录
    
    remote.undo(); // 撤销关灯
    remote.undo(); // 撤销回家模式
    
    remote.redo(); // 重做回家模式
}