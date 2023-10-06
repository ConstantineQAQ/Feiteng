#include "temperature.h"

static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("temperature");

namespace Feiteng {

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
    if (m_temperature > 37.3 || m_temperature < 36.1) {
        return false;
    }
    return false;
}
void ObjectTemp::fetchTemperature() {
    MutexType::Lock lock(m_mutex);
    QByteArray data = m_serial->Sread();
    if (data.size() == 0) {
        FEITENG_LOG_ERROR(g_logger) << "温度数据为空";
        return;
    }
    int objectTempInt = static_cast<unsigned char>(data.at(2));
    int objectTempDec = static_cast<unsigned char>(data.at(3));
    double objectTemp = objectTempInt + objectTempDec * 0.01;
    FEITENG_LOG_INFO(g_logger) << QString("物体温度 %1C").arg(objectTemp,0,'f',1).toStdString(); 
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

Serial::Serial()
{
    m_config.reset(new SerialConfig());
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

}
