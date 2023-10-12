#include <iostream>
#include <string>
#include "../include/config.h"
#include "../include/face.h"
#include "../include/temperature.h"
#include "../include/log.h"
#include "../include/database.h"

extern Feiteng::FaceConfig::ptr g_face_config;

int main() {
    FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << g_face_config->toString();
    static Feiteng::Serial::ptr g_serial = FEITENG_SERIAL_NAME("wendu");
    FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << g_serial->getConfig()->toString();
    static Feiteng::MySQL::ptr g_mysql = FEITENG_MYSQL_NAME("feiteng");
    FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << g_mysql->connect();
    static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("database");
    FEITENG_LOG_INFO(g_logger) << "hello world";
    YAML::Node facenode = YAML::LoadFile("../conf/face.yml");
    YAML::Node serialnode = YAML::LoadFile("../conf/serial.yml");
    YAML::Node databasenode = YAML::LoadFile("../conf/database.yml");
    YAML::Node lognode = YAML::LoadFile("../conf/log.yml");
    Feiteng::Config::LoadFromYaml(facenode);
    Feiteng::Config::LoadFromYaml(serialnode);
    Feiteng::Config::LoadFromYaml(databasenode);
    Feiteng::Config::LoadFromYaml(lognode);
    FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "============================================";
    FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << g_face_config->toString();
    FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << g_serial->getConfig()->toString();
    FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << g_mysql->connect();
    FEITENG_LOG_INFO(g_logger) << "hello world";
}