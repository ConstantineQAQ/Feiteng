#include <iostream>
#include <string>
#include <memory>
#include "../include/config.h"
#include "../include/database.h"
#include "../include/log.h"

Feiteng::Database::ptr g_database = FEITENG_DATABASE_NAME("test");

void print_yaml(const YAML::Node& node, int level) {
    if(node.IsScalar()){
        FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << std::string(level*4, ' ') << node.Scalar() << " - " << node.Type() << " - " << level;
    }else if(node.IsNull()){
        FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << std::string(level*4, ' ') << "NULL - " << node.Type() << " - " << level;
    }else if(node.IsMap()){
        for(auto it = node.begin(); it != node.end(); ++it){
            FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << std::string(level*4, ' ') << it->first << " - " << it->second.Type() << " - " << level;
            print_yaml(it->second, level + 1);
        }
    }else if(node.IsSequence()){
        for(size_t i = 0; i < node.size(); ++i){
            FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << std::string(level*4, ' ') << i << " - " << node[i].Type() << " - " << level;
            print_yaml(node[i], level + 1);
        }
    }
}

void testDatabaseYaml() {
    auto db_config_var = Feiteng::Config::Lookup<Feiteng::DatabaseConfig>(
        "db_config",
        Feiteng::DatabaseConfig{"MySQL", "127.0.0.1", "test", "test"},
        "Database configuration"
    );

    // 从配置文件加载配置信息
    YAML::Node root = YAML::LoadFile("../conf/database.yml");
    print_yaml(root,0);
    Feiteng::Config::LoadFromYaml(root);

    // 获取数据库配置信息
    Feiteng::DatabaseConfig db_config = db_config_var->getValue();

    // 输出数据库配置信息
    std::cout << "Username: " << db_config.getUserName() << std::endl;
    std::cout << "Password: " << db_config.getPassword() << std::endl;
    std::cout << "IP: " << db_config.getConn() << std::endl;
    std::cout << "Type: " << db_config.getType() << std::endl;
}

void connectDatabase() {
    auto db_config_var = Feiteng::Config::Lookup<Feiteng::DatabaseConfig>(
        "db_config",
        Feiteng::DatabaseConfig{"MySQL", "127.0.0.1", "test", "test"},
        "Database configuration"
    );
    YAML::Node root = YAML::LoadFile("../conf/database.yml");
    Feiteng::Config::LoadFromYaml(root);
    Feiteng::DatabaseConfig db_config = db_config_var->getValue();
    g_database->setConfig(std::shared_ptr<Feiteng::DatabaseConfig>(
        new Feiteng::DatabaseConfig(db_config)
    ));
    FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << g_database->getConfig()->toString();
    g_database->connect();
}

void test_select() {
    connectDatabase();
    std::string sql = "select name,student_id,id from students_face_recognition";
    QSqlQuery query;
    if(!query.exec(QString::fromStdString(sql))) {
        FEITENG_LOG_ERROR(FEITENG_LOG_ROOT()) << "查询失败";
        return;
    }
    while(query.next()) {
        std::string name = query.value(0).toString().toStdString();
        std::string student_id = query.value(1).toString().toStdString();
        int id = query.value(2).toInt();
        FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "name: " << name << " student_id: " << student_id << " id: " << id;
    }
}

int main() {
    test_select();
    return 0;
}