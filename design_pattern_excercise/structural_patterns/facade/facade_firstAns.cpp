#include <iostream>
#include <memory>

using namespace std;

/**
 * 没问题，有个建议是：题目未要求子系统对象动态创建，可直接将子系统对象作为 SmartHomeFacade 的普通成员变量，
 * 无需使用 shared_ptr。非要用智能指针的话，unique_ptr也比shared_ptr好。
 * 子系统对象的创建和销毁完全由 SmartHomeFacade 控制，无需共享。unique_ptr性能更好，无额外开销，仅包装原始指针
 */

// 灯光系统
class LightSystem {
public:
    void turnOn() { cout << "灯光已开启\n"; }
    void turnOff() { cout << "灯光已关闭\n"; }
    void setBrightness(int level) { cout << "灯光亮度设置为 " << level << "%\n"; }
};

// 空调系统
class AirConditioner {
public:
    void turnOn() { cout << "空调已开启\n"; }
    void turnOff() { cout << "空调已关闭\n"; }
    void setTemperature(int temp) { cout << "空调温度设置为 " << temp << "°C\n"; }
    void setFanSpeed(int speed) { cout << "空调风速设置为 " << speed << "档\n"; }
};

// 音响系统
class AudioSystem {
public:
    void playMusic(string song) { cout << "正在播放音乐: " << song << "\n"; }
    void stopMusic() { cout << "音乐已停止\n"; }
};

// 安全系统
class SecuritySystem {
public:
    void activateMonitoring() { cout << "安全监控已启动\n"; }
    void deactivateMonitoring() { cout << "安全监控已关闭\n"; }
};

class SmartHomeFacade
{
public:
    SmartHomeFacade()
    {
        spLightSys = make_shared<LightSystem>();
        spAirSys = make_shared<AirConditioner>();
        spAudioSys = make_shared<AudioSystem>();
        spSecuritySys = make_shared<SecuritySystem>();
    }

    void leaveHome()
    {
        spLightSys->turnOff();
        spAirSys->turnOff();
        spAudioSys->stopMusic();
        spSecuritySys->activateMonitoring();
    }

    void arriveHome()
    {
        spLightSys->turnOn();
        spLightSys->setBrightness(70);
        spAirSys->turnOn();
        spAirSys->setTemperature(25);
        spAirSys->setFanSpeed(2);
        spAudioSys->playMusic("Welcome Home");
        spSecuritySys->deactivateMonitoring();
    }

private:
    shared_ptr<LightSystem> spLightSys;
    shared_ptr<AirConditioner> spAirSys;
    shared_ptr<AudioSystem> spAudioSys;
    shared_ptr<SecuritySystem> spSecuritySys;
};

int main() {
    SmartHomeFacade smartHome;
    
    cout << "===== 离家模式 =====" << endl;
    smartHome.leaveHome();
    
    cout << "\n===== 回家模式 =====" << endl;
    smartHome.arriveHome();
    
    return 0;
}