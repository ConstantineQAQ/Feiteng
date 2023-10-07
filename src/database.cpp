#include "database.h"

static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("database");

namespace Feiteng {
struct DatabaseDefine {
    std::string name; // 数据库器名称
    std::string host; // 数据库ip地址
    std::string type; // 数据库类型
    std::string user; // 数据库用户名
    std::string passwd; // 数据库密码
    std::string database; // 数据库名称
    int port; // 数据库端口
    
    bool operator<(const DatabaseDefine& oth) const {
        return name < oth.name;
    }

    bool operator==(const DatabaseDefine& oth) const {
        return name == oth.name
            && host == oth.host
            && type == oth.type
            && user == oth.user
            && passwd == oth.passwd
            && database == oth.database
            && port == oth.port;
    }
};

template<>
class LexicalCast<std::string, DatabaseDefine> {
public:
    DatabaseDefine operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        DatabaseDefine def;
        if(!node["name"].IsDefined()) {
            FEITENG_LOG_ERROR(g_logger) << "database config error: name is null, " << node;
            throw std::logic_error("database config error: name is null, ");
        }
        def.name = node["name"].as<std::string>();
        def.host = node["host"].IsDefined() ? node["host"].as<std::string>() : "";
        if(node["type"].IsDefined()) {
            def.type = node["type"].as<std::string>();
        } else {
            FEITENG_LOG_ERROR(g_logger) << "database config error: type is null, " << node;
            throw std::logic_error("database config error: type is null, ");
        }
        def.user = node["user"].IsDefined() ? node["user"].as<std::string>() : "";
        def.passwd = node["passwd"].IsDefined() ? node["passwd"].as<std::string>() : "";
        def.database = node["database"].IsDefined() ? node["database"].as<std::string>() : "";
        def.port = node["port"].IsDefined() ? node["port"].as<int>() : 0;
        return def;
    }
};

template<>
class LexicalCast<DatabaseDefine, std::string> {
public:
    std::string operator()(const DatabaseDefine& v) {
        YAML::Node node;
        node["name"] = v.name;
        node["host"] = v.host;
        node["type"] = v.type;
        node["user"] = v.user;
        node["passwd"] = v.passwd;
        node["database"] = v.database;
        node["port"] = v.port;
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

Feiteng::ConfigVar<std::set<DatabaseDefine> >::ptr g_database_defines =
    Feiteng::Config::Lookup("databases", std::set<DatabaseDefine>(), "databases config");

struct DatabaseInit {
    DatabaseInit() {
        g_database_defines->addListener([](const std::set<DatabaseDefine>& old_value
                    , const std::set<DatabaseDefine>& new_value){
                FEITENG_LOG_INFO(g_logger) << "on_database_conf_changed";
                for(auto& i : new_value) {
                    auto it = old_value.find(i);
                    Feiteng::MySQL::ptr mysql;
                    if(it == old_value.end()) {
                        // 新增
                        mysql = FEITENG_MYSQL_NAME(i.name);
                    } else {
                        if(!(i == *it)) {
                            // 修改
                            mysql = FEITENG_MYSQL_NAME(i.name);
                        } else {
                            continue; 
                        }
                    }
                    mysql->init(i.host, i.port, i.user, i.passwd, i.database);
                }
                for(auto& i : old_value) {
                    auto it = new_value.find(i);
                    if(it == new_value.end()) {
                        // 删除
                        auto mysql = FEITENG_MYSQL_NAME(i.name);
                        FEITENG_MYSQL_DELETE(i.name);
                        // 存在内存泄漏问题: mysql
                    }
                }
        });
    }
};
MySQLManager::MySQLManager()
{
    m_root.reset(new MySQL);
    m_databases[m_root->getName()] = m_root;
}
MySQL::ptr MySQLManager::getMySQL(const std::string &name)
{
    MutexType::Lock lock(m_mutex);
    auto it = m_databases.find(name);
    if(it != m_databases.end()) {
        return it->second;
    }
    MySQL::ptr mysql(new MySQL(name));
    mysql->m_root = m_root;
    return m_databases[name] = mysql;

}
void MySQLManager::del(const std::string &name)
{
    MutexType::Lock lock(m_mutex);
    m_databases.erase(name);
}
MySQLStmtRes::ptr MySQLStmtRes::Create(std::shared_ptr<MySQLStmt> stmt)
{
    MySQLStmtRes::ptr rt(new MySQLStmtRes(stmt));
    QSqlQuery& query = stmt->getQuery();
    if (!query.exec()) {
        FEITENG_LOG_ERROR(g_logger) << "stmt= " << query.lastQuery().toStdString() << " error: " << query.lastError().text().toStdString();
        return nullptr;
    }

    int numFields = query.record().count();
    rt->m_datas.resize(numFields);
    // 获取字段名
    while (query.next()) {
        QSqlRecord record = query.record();
        for (int i = 0; i < numFields; i++) {
            QSqlField field = record.field(i);
            Data data;
            data.is_null = field.isNull();
            data.error = false;  // Set error flag based on your error handling logic
            data.value = field.value();
            rt->m_datas[i] = data;
        }
    }
    return rt;
}
MySQLStmt::ptr MySQLStmt::Create(MySQL::ptr db, const QString &stmt)
{
    MySQLStmt::ptr rt(new MySQLStmt(db));
    QSqlQuery query(db->getDB());

    // 准备SQL语句
    if (!query.prepare(stmt)) {
        FEITENG_LOG_ERROR(g_logger) << "stmt= " << stmt.toStdString() << " error: " << query.lastError().text().toStdString();
        return nullptr;
    }
    rt->m_query = query;
    return rt;
}
}