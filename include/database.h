#ifndef DATABASE_H
#define DATABASE_H

#include "log.h"
#include "thread.h"
#include "util.h"
#include "singleton.h"

#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <memory>
#include <string>

#define FEITENG_DATABASE_ROOT() Feiteng::DatabaseMgr::GetInstance()->getRoot()
#define FEITENG_DATABASE_NAME(name) Feiteng::DatabaseMgr::GetInstance()->getDatabase(name)

namespace Feiteng {

// 数据库配置类
class DatabaseConfig {
public:
    typedef std::shared_ptr<DatabaseConfig> ptr;
    DatabaseConfig(const std::string& type = "",
                   const std::string& conn = "",
                   const std::string& user_name = "",
                   const std::string& password = "");
    std::string getType() const { return m_type; }
    std::string getConn() const { return m_conn; }
    std::string getUserName() const { return m_user_name; }
    std::string getPassword() const { return m_password; }
    void setType(const std::string& type) { m_type = type; }
    void setConn(const std::string& conn) { m_conn = conn; }
    void setUserName(const std::string& user_name) { m_user_name = user_name; }
    void setPassword(const std::string& password) { m_password = password; }
    std::string toString() const; // 将数据库配置转换为字符串
    std::string toYamlString(); // 将数据库配置转换为YAML字符串

    bool operator==(const DatabaseConfig& rhs) const {
        return m_type == rhs.m_type
            && m_conn == rhs.m_conn
            && m_user_name == rhs.m_user_name
            && m_password == rhs.m_password;
    }
private:
    std::string m_type; // 数据库类型
    std::string m_conn; // 数据库连接
    std::string m_user_name; // 数据库用户名
    std::string m_password; // 数据库密码
};

// 数据库类
class Database {
friend class DatabaseManager;
public:
    typedef std::shared_ptr<Database> ptr;
    Database(const std::string& name = "root");
    std::string getName() const { return m_name; } // 获取数据库名称
    DatabaseConfig::ptr getConfig() const { return m_config; } // 获取数据库配置
    void setConfig(const DatabaseConfig::ptr& config) { m_config = config; } // 设置数据库配置
    void connect(); // 连接数据库
private:
    std::string m_name; // 数据库名称
    Database::ptr m_root; // 默认数据库
    DatabaseConfig::ptr m_config; // 数据库配置
};

// 数据库管理类
class DatabaseManager {
public:
    typedef std::shared_ptr<DatabaseManager> ptr;
    typedef Spinlock MutexType;
    Database::ptr getDatabase(const std::string& name); // 获取数据库对象
    Database::ptr getRoot() const { return m_root; } // 获取默认数据库对象
    DatabaseManager();
private:
    std::map<std::string, Database::ptr> m_databases;
    MutexType m_mutex;
    Database::ptr m_root;
};

typedef Feiteng::Singleton<DatabaseManager> DatabaseMgr;
}

#endif // DATABASE_H