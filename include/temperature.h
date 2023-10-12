#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QPushButton>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include <vector>
#include <cstdio>
#include <QByteArray>
#include <memory>

#include "log.h"
#include "thread.h"
#include "util.h"
#include "config.h"

#define FEITENG_SERIAL_ROOT() Feiteng::SerialMgr::GetInstance()->getRoot()
#define FEITENG_SERIAL_NAME(name) Feiteng::SerialMgr::GetInstance()->getSerial(name)
#define FEITENG_SERIAL_OPEN(name) Feiteng::SerialMgr::GetInstance()->getSerial(name)->Sopen()
#define FEITENG_SERIAL_EARSE(name) Feiteng::SerialMgr::GetInstance()->delSerila(name)
#define FEITENG_SERIAL_CLOSE(name) Feiteng::SerialMgr::GetInstance()->getSerial(name)->Sclose()

namespace Feiteng
{
/**
 * @brief The Temperature class
 * 
*/

class SerialConfig {
public:
    typedef std::shared_ptr<SerialConfig> ptr;
    SerialConfig(const std::string& portName = "COM1", int baudRate = 9600,
                 int dataBits = 8, int parity = 0, int stopBits = 1, int flowControl = 0, 
                 const std::string& name = "root")
        : m_portName(portName)
        , m_baudRate(baudRate)
        , m_dataBits(dataBits)
        , m_parity(parity)
        , m_stopBits(stopBits)
        , m_flowControl(flowControl) 
        , m_name(name){
    }
    const std::string getName() const { return m_name; }
    const std::string& getPortName() const { return m_portName; }
    const int getBaudRate() const { return m_baudRate; }
    const int getDataBits() const { return m_dataBits; }
    const int getParity() const { return m_parity; }
    const int getStopBits() const { return m_stopBits; }
    const int getFlowControl() const { return m_flowControl; }
    void setName(const std::string& v) { m_name = v; }
    void setPortName(const std::string& v) { m_portName = v; }
    void setBaudRate(int v) { m_baudRate = v; }
    void setDataBits(int v) { m_dataBits = v; }
    void setParity(int v) { m_parity = v; }
    void setStopBits(int v) { m_stopBits = v; }
    void setFlowControl(int v) { m_flowControl = v; }
    std::string toString() const;
    bool operator==(const SerialConfig& rhs) const {
        return m_portName == rhs.m_portName
            && m_baudRate == rhs.m_baudRate
            && m_dataBits == rhs.m_dataBits
            && m_parity == rhs.m_parity
            && m_stopBits == rhs.m_stopBits
            && m_flowControl == rhs.m_flowControl
            && m_name == rhs.m_name;
    }
    bool operator<(const SerialConfig& rhs) const {
        return m_name < rhs.m_name;
    }
private:
    std::string m_name; // 串口配置名称
    std::string m_portName; // 串口名称
    int m_baudRate; // 波特率
    int m_dataBits; // 数据位
    int m_parity; // 校验位
    int m_stopBits; // 停止位
    int m_flowControl; // 流控制
};

class Serial : public QObject , public std::enable_shared_from_this<Serial>{
Q_OBJECT
friend class SerialManager;
public:
    typedef std::shared_ptr<Serial> ptr;
    typedef Mutex MutexType;
    void Swrite(const QByteArray& data);
    void setConfig(SerialConfig::ptr config){
        m_config = config;
        applyConfig();
        }
    SerialConfig::ptr getConfig() const { return m_config; }
    QByteArray Sread();
    void Sopen();
    void Sclose();
    QSerialPort* getSerial() const { return m_serial; }
    Serial(const std::string& name = "root");
    const std::string getName() const { return m_name; }
    // 将配置应用于串口
    void applyConfig() {
        if(m_config) {
            m_serial->setPortName(QString::fromStdString(m_config->getPortName()));
            m_serial->setBaudRate(m_config->getBaudRate());
            m_serial->setDataBits(static_cast<QSerialPort::DataBits>(m_config->getDataBits()));
            m_serial->setParity(static_cast<QSerialPort::Parity>(m_config->getParity()));
            m_serial->setStopBits(static_cast<QSerialPort::StopBits>(m_config->getStopBits()));
            m_serial->setFlowControl(static_cast<QSerialPort::FlowControl>(m_config->getFlowControl()));
            FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "串口配置应用成功";
        }
    }

private:
    std::string m_name; // 串口名称
    SerialConfig::ptr m_config; // 串口配置
    QSerialPort *m_serial; // 串口
    MutexType m_mutex; // 互斥锁
    Serial::ptr m_root; // 主串口
};

class TemperatureConfig {
public:
    typedef std::shared_ptr<TemperatureConfig> ptr;
    TemperatureConfig(int interval = 1000, double threshold = 37.3)
        : m_interval(interval)
        , m_threshold(threshold) {
    }
    const int getInterval() const { return m_interval; }
    const double getThreshold() const { return m_threshold; }
    void setInterval(int v) { m_interval = v; }
    void setThreshold(double v) { m_threshold = v; }
    std::string toString() const;
    bool operator==(const TemperatureConfig& rhs) const {
        return m_interval == rhs.m_interval
            && m_threshold == rhs.m_threshold;
    }
private:
    int m_interval; // 采集间隔
    double m_threshold; // 阈值
};

class BodyTemp;
class ObjectTemp;

class Temperature : public QObject
{
Q_OBJECT
public:
    typedef std::shared_ptr<Temperature> ptr;
    typedef Mutex MutexType;
    virtual void fetchTemperature() {}
    virtual bool isTemperatureNormal() { return true; }
    virtual ~Temperature() {}
    void setSerial(Serial::ptr serial) { m_serial = serial; }
    void setConfig(TemperatureConfig::ptr config) { m_config = config; }
    Serial::ptr getSerial() const { return m_serial; }
    TemperatureConfig::ptr getConfig() const { return m_config; }
    Temperature();

protected:
    MutexType m_mutex; // 互斥锁
    TemperatureConfig::ptr m_config; // 温度配置
    Serial::ptr m_serial; // 串口
};

class BodyTemp : public Temperature
{
Q_OBJECT
public:
    typedef std::shared_ptr<BodyTemp> ptr;
    bool isTemperatureNormal();
    double getBodyTemperature() const { return m_bodyTemperature; }
    static ptr create() { return std::make_shared<BodyTemp>(); }
public slots:
    void fetchTemperature();
private:
    double m_bodyTemperature; // 人体温度
};

class ObjectTemp : public Temperature
{
Q_OBJECT
public:
    typedef std::shared_ptr<ObjectTemp> ptr;
    bool isTemperatureNormal();
    static ptr create() { return std::make_shared<ObjectTemp>();}
public slots:
    void fetchTemperature();
};

class SerialManager {
public:
    SerialManager();
    typedef std::shared_ptr<SerialManager> ptr;
    typedef Spinlock MutexType;
    Serial::ptr getSerial(const std::string& name);
    Serial::ptr getRoot() const { return m_root; }
    void delSerila(const std::string& name);
    std::string toYamlString();
private:
    MutexType m_mutex;
    std::map<std::string, Serial::ptr> m_serials;
    Serial::ptr m_root;
};

typedef Singleton<SerialManager> SerialMgr;

} // namespace Feiteng

#endif // TEMPERATURE_H