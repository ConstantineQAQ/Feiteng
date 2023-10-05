#include "database.h"

namespace Feiteng {
Database::ptr DatabaseManager::getDatabase(const std::string &name)
{
    MutexType::Lock lock(m_mutex);
    auto it = m_databases.find(name);
    if(it != m_databases.end()) {
        return it->second;
    }
    Database::ptr database(new Database(name));
    database->m_root = m_root;
    m_databases[name] = database;
    return database;
}

DatabaseManager::DatabaseManager()
{
    m_root.reset(new Database());
    m_databases[m_root->getName()] = m_root;
}

Database::Database(const std::string &name) : m_name(name)
{
    m_config.reset(new DatabaseConfig("QMYSQL","127.0.0.1","kylin","kylin","admindatabase"));
}

void Database::connect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase(QString::fromStdString(m_config->getType()));
    db.setHostName(QString::fromStdString(m_config->getConn()));
    db.setUserName(QString::fromStdString(m_config->getUserName()));
    db.setPassword(QString::fromStdString(m_config->getPassword()));
    db.setDatabaseName(QString::fromStdString(m_config->getDatabaseName()));
    if(!db.open()) {
        FEITENG_LOG_ERROR(FEITENG_LOG_ROOT()) << "连接数据库失败： " << m_name;
        return;
    }
    FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "成功连接数据库：  " << m_name;
}

DatabaseConfig::DatabaseConfig(const std::string &type, const std::string &conn, 
                               const std::string &user_name, const std::string &password,
                               const std::string &databasename)
: m_type(type)
, m_conn(conn)
, m_user_name(user_name)
, m_password(password)
, m_databasename(databasename)
{
}
std::string DatabaseConfig::toString() const
{
    std::stringstream ss;
    ss << "[DatabaseConfig type=" << m_type
        << " conn=" << m_conn
        << " user_name=" << m_user_name
        << " password=" << m_password
        << " databasename=" << m_databasename
        << "]";
    return ss.str();
}
}