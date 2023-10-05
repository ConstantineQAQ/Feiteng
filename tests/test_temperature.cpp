#include "../include/temperature.h"
#include "../include/log.h"
#include "../include/config.h"
#include <memory>
#include <QApplication>

void test_yaml() {
    Feiteng::ConfigVar<Feiteng::SerialConfig>::ptr serial_config =
        Feiteng::Config::Lookup<Feiteng::SerialConfig>(
            "serial_config" 
            , Feiteng::SerialConfig()
            , "Serial config");
    Feiteng::Serial *serial = new Feiteng::Serial();
    serial_config->addListener([serial](const Feiteng::SerialConfig& old_value, const Feiteng::SerialConfig& new_value) {
        serial->setConfig(std::shared_ptr<Feiteng::SerialConfig>(
            new Feiteng::SerialConfig(new_value)
        ));
    });
    serial->setConfig(std::shared_ptr<Feiteng::SerialConfig>(new Feiteng::SerialConfig(serial_config->getValue())));
    #define XX(m) \
        FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << #m << " = " << serial->getConfig()->toString();
    XX(before);
    YAML::Node root = YAML::LoadFile("../conf/serial.yml");
    Feiteng::Config::LoadFromYaml(root);
    XX(after);
}

int test_serial(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Feiteng::ConfigVar<Feiteng::SerialConfig>::ptr serial_config =
        Feiteng::Config::Lookup<Feiteng::SerialConfig>(
            "serial.serial_config" 
            , Feiteng::SerialConfig()
            , "Serial config");
    Feiteng::Serial *serial = new Feiteng::Serial();
    serial->setConfig(std::shared_ptr<Feiteng::SerialConfig>(new Feiteng::SerialConfig(serial_config->getValue()))); // 测试用
    serial->Sopen(); // 测试用
    serial_config->addListener([serial](const Feiteng::SerialConfig& old_value, const Feiteng::SerialConfig& new_value) {
        serial->setConfig(std::shared_ptr<Feiteng::SerialConfig>(
            new Feiteng::SerialConfig(new_value)
        ));
    });
    YAML::Node root = YAML::LoadFile("../conf/serial.yml");
    Feiteng::Config::LoadFromYaml(root);
    serial->Sopen();
    Feiteng::Temperature::ptr bodyTemp = Feiteng::Temperature::create(Feiteng::TempMode::Body);
    bodyTemp->setSerial(std::shared_ptr<Feiteng::Serial>(serial));
    Feiteng::BodyTemp* bodyTempPtr = dynamic_cast<Feiteng::BodyTemp*>(bodyTemp.get());
    QByteArray startMeasuringCmd = QByteArray::fromHex("FACAC4");
    QByteArray bodyTempModeCmd = QByteArray::fromHex("FAC5BF");
    QTimer* timer = new QTimer();
    timer->setInterval(1000);
    bool connection2 = QObject::connect(serial->getSerial(), &QSerialPort::readyRead, bodyTempPtr, &Feiteng::BodyTemp::fetchTemperature);
    bool connection1 = QObject::connect(timer,&QTimer::timeout,serial,[startMeasuringCmd,serial](){
        FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "发送开始测量命令";
        serial->Swrite(startMeasuringCmd);
    });
    timer->start();
    return app.exec();
}

void test_temper_config() {
    auto temperature_config =
        Feiteng::Config::Lookup<Feiteng::TemperatureConfig>(
            "serial.temperature_config" 
            , Feiteng::TemperatureConfig()
            , "Temperature config");
    Feiteng::Temperature::ptr bodyTemp = Feiteng::Temperature::create(Feiteng::TempMode::Body);
    temperature_config->addListener([&bodyTemp](const Feiteng::TemperatureConfig& old_value, const Feiteng::TemperatureConfig& new_value) {
        bodyTemp->setConfig(std::shared_ptr<Feiteng::TemperatureConfig>(
            new Feiteng::TemperatureConfig(new_value)
        ));
    });
    FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "before = " << bodyTemp->getConfig()->toString();
    YAML::Node root = YAML::LoadFile("../conf/serial.yml");
    Feiteng::Config::LoadFromYaml(root);
    FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "after = " << bodyTemp->getConfig()->toString();
}

int main(int argc, char* argv[]) {
    test_temper_config();
}
