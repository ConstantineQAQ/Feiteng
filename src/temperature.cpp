#include "temperature.h"

static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("temperature");
Feiteng::BodyTemp::ptr g_body_temp = Feiteng::BodyTemp::create();
Feiteng::ObjectTemp::ptr g_object_temp = Feiteng::ObjectTemp::create();

namespace Feiteng {

template<>
class LexicalCast<Feiteng::TemperatureConfig, std::string> {
public:
    std::string operator()(const TemperatureConfig& v) {
        YAML::Node node;
        node["interval"] = v.getInterval();
        node["threshold"] = v.getThreshold();
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<>
class LexicalCast<std::string, Feiteng::TemperatureConfig> {
public:
    TemperatureConfig operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        TemperatureConfig config;
        config.setInterval(node["interval"].as<int>());
        config.setThreshold(node["threshold"].as<double>());
        return config;
    }
};

template<>
class LexicalCast<std::string, std::set<Feiteng::SerialConfig>> {
public:
    std::set<Feiteng::SerialConfig> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        std::set<Feiteng::SerialConfig> configs;
        for (size_t i = 0; i < node.size(); ++i) {
            auto n = node[i];
            if (!n["name"].IsDefined()) {
                FEITENG_LOG_ERROR(g_logger) << "serial config error: name is null, " << n;
                continue;
            }
            Feiteng::SerialConfig sc;
            sc.setName(n["name"].as<std::string>());
            sc.setPortName(n["port"].IsDefined() ? n["port"].as<std::string>() : "");
            sc.setBaudRate(n["baud_rate"].IsDefined() ? n["baud_rate"].as<int>() : 0);
            sc.setDataBits(n["data_bits"].IsDefined() ? n["data_bits"].as<int>() : 0);
            sc.setParity(n["parity"].IsDefined() ? n["parity"].as<int>() : 0);
            sc.setStopBits(n["stop_bits"].IsDefined() ? n["stop_bits"].as<int>() : 0);
            sc.setFlowControl(n["flow_control"].IsDefined() ? n["flow_control"].as<int>() : 0);
            configs.insert(sc);
        }
        return configs;
    }
};

template<>
class LexicalCast<std::set<Feiteng::SerialConfig>, std::string> {
public:
    std::string operator()(const std::set<Feiteng::SerialConfig>& v) {
        YAML::Node node;
        for(auto& i : v) {
            YAML::Node n;
            n["name"] = i.getName();
            n["port"] = i.getPortName();
            n["baud_rate"] = i.getBaudRate();
            n["data_bits"] = i.getDataBits();
            n["parity"] = i.getParity();
            n["stop_bits"] = i.getStopBits();
            n["flow_control"] = i.getFlowControl();
            node.push_back(n);
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

Feiteng::ConfigVar<std::set<Feiteng::SerialConfig>>::ptr g_serial_config =
    Feiteng::Config::Lookup("serial", std::set<Feiteng::SerialConfig>(), "serial config");

Feiteng::ConfigVar<Feiteng::TemperatureConfig>::ptr g_temperature_config =
    Feiteng::Config::Lookup("temperature", Feiteng::TemperatureConfig(), "temperature config");

struct SerialConfigIniter {
    SerialConfigIniter(){
        g_serial_config->addListener([](const std::set<Feiteng::SerialConfig>& old_value,
                    const std::set<Feiteng::SerialConfig>& new_value) {
                FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "serial config changed: " << new_value.size();
                for (auto& i : new_value) {
                    auto it = old_value.find(i);
                    Serial::ptr serial;
                    if (it == old_value.end()) {
                        serial = FEITENG_SERIAL_NAME(i.getName());
                    } else {
                        if (!(i == *it)) {
                            serial = FEITENG_SERIAL_NAME(i.getName());
                        } else {
                            continue;
                        }
                    }
                    serial->setConfig(std::shared_ptr<SerialConfig>(new SerialConfig(i)));
                }
                for (auto& i : old_value) {
                    auto it = new_value.find(i);
                    if (it == new_value.end()) {
                        FEITENG_SERIAL_EARSE(i.getName());
                    }
                }
            });
    }
};

struct  TemperatureConfigIniter {
    TemperatureConfigIniter(){
        g_temperature_config->addListener([](const Feiteng::TemperatureConfig& old_value,
                    const Feiteng::TemperatureConfig& new_value) {
                FEITENG_LOG_INFO(g_logger) << "temperature config changed: " << new_value.toString();
                g_body_temp->setConfig(std::shared_ptr<TemperatureConfig>(new TemperatureConfig(new_value)));
            });
    }
};

static SerialConfigIniter __serial_config_initer;
static TemperatureConfigIniter __temperature_config_initer;

void BodyTemp::fetchTemperature() {
    MutexType::Lock lock(m_mutex);
    QByteArray data = m_serial->Sread();
    if (data.size() == 0) {
        FEITENG_LOG_ERROR(g_logger) << "温度数据为空";
        return;
    }
    int bodyTempInt = static_cast<unsigned char>(data.at(2));
    int bodyTempDec = static_cast<unsigned char>(data.at(3));
    double bodyTemp = bodyTempInt + bodyTempDec * 0.01;
    FEITENG_LOG_INFO(g_logger) << QString("人体温度 %1C").arg(bodyTemp,0,'f',1).toStdString();
    m_bodyTemperature = bodyTemp;
}

bool BodyTemp::isTemperatureNormal() {
    if (m_bodyTemperature > 37.3 || m_bodyTemperature < 36.1) {
        return false;
    }
    return false;
}
void ObjectTemp::fetchTemperature() {
}

bool ObjectTemp::isTemperatureNormal()
{
    return false;
}

void Serial::Swrite(const QByteArray &data)
{
    MutexType::Lock lock(m_mutex);
    if(!m_serial->isOpen()) {
        FEITENG_LOG_ERROR(g_logger) << "串口未打开";
        return;
    }
    qint64 status = m_serial->write(data);
    if (status == -1) {
        FEITENG_LOG_ERROR(g_logger) << "串口写入失败";
        return;
    } else {
        FEITENG_LOG_INFO(g_logger) << "串口写入成功";
    }
}

QByteArray Serial::Sread()
{
    MutexType::Lock lock(m_mutex);
    if(!m_serial->isOpen()) {
        FEITENG_LOG_ERROR(g_logger) << "串口未打开";
        return QByteArray();
    }
    if (m_serial->bytesAvailable() == 0) {
        FEITENG_LOG_ERROR(g_logger) << "串口数据为空";
        return QByteArray();
    }
    QByteArray temp = m_serial->readAll();
    return temp;
}

void Serial::Sopen()
{
    MutexType::Lock lock(m_mutex);
    if(m_serial->isOpen()) {
        FEITENG_LOG_ERROR(g_logger) << "串口已打开";
        return;
    }
    if(!m_serial->open(QIODevice::ReadWrite)) {
        FEITENG_LOG_ERROR(g_logger) << "无法打开串口： " << m_config->getPortName() + " " << m_serial->errorString().toStdString(); 
        return;
    }
    FEITENG_LOG_INFO(g_logger) << "串口打开成功";
}

void Serial::Sclose()
{
    MutexType::Lock lock(m_mutex);
    if(!m_serial->isOpen()) {
        FEITENG_LOG_ERROR(g_logger) << "串口未打开";
        return;
    }
    m_serial->close();
}

Serial::Serial(const std::string& name) : m_name(name)
{
    m_config.reset(new SerialConfig());
    m_config->setName(name);
    m_serial = new QSerialPort();
}

std::string SerialConfig::toString() const
{
    std::stringstream ss;
    ss << "[SerialConfig port=" << m_portName
       << " baudRate=" << m_baudRate
       << " dataBits=" << m_dataBits
       << " parity=" << m_parity
       << " stopBits=" << m_stopBits
       << " flowControl=" << m_flowControl
       << "]";
    return ss.str();
}
std::string TemperatureConfig::toString() const
{
    std::stringstream ss;
    ss << "[TemperatureConfig interval=" << m_interval
       << " threshold=" << m_threshold
       << "]]";
    return ss.str();
}
Temperature::Temperature()
{
    m_config.reset(new TemperatureConfig());

}

SerialManager::SerialManager()
{
    m_root.reset(new Serial);
    m_serials[m_root->getName()] = m_root;
}

Serial::ptr SerialManager::getSerial(const std::string &name)
{
    MutexType::Lock lock(m_mutex);
    auto it = m_serials.find(name);
    if (it != m_serials.end()) {
        return it->second;
    }
    Serial::ptr serial(new Serial(name));
    m_serials[name] = serial;
    return serial;
}
void SerialManager::delSerila(const std::string &name)
{
    MutexType::Lock lock(m_mutex);
    auto it = m_serials.find(name);
    if (it == m_serials.end()) {
        return;
    }
    delete it->second.get();
    m_serials.erase(name);
}
}
