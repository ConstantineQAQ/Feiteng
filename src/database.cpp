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
                FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "on_database_conf_changed";
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

static DatabaseInit __database_init;

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
    rt->m_query = query;
    return rt;
}
MySQLStmtRes::~MySQLStmtRes(){}
MySQLStmt::ptr MySQLStmt::Create(MySQL::ptr db, const QString &stmt)
{
    MySQLStmt::ptr rt(new MySQLStmt(db));
    QSqlQuery query = QSqlQuery(db->getDB());

    // 准备SQL语句
    if (!query.prepare(stmt)) {
        FEITENG_LOG_ERROR(g_logger) << "stmt= " << stmt.toStdString() << " error: " << query.lastError().text().toStdString();
        return nullptr;
    }
    rt->m_query = query;
    return rt;
}
int MySQLStmt::execute()
{
    bool success = m_query.exec();
    if (!success) {
        FEITENG_LOG_ERROR(g_logger) << "stmt= " << m_query.lastQuery().toStdString() << " error: " << m_query.lastError().text().toStdString();
        return -1;
    }
    return 0;
}
int64_t MySQLStmt::getLastInsertId()
{
    return m_query.lastInsertId().toLongLong();
}
ISQLData::ptr MySQLStmt::query()
{
    return MySQLStmtRes::Create(shared_from_this());
}
MySQLTransaction::ptr MySQLTransaction::Create(MySQL::ptr mysql, bool auto_commit)
{
    MySQLTransaction::ptr rt(new MySQLTransaction(mysql,auto_commit));
    if(rt->begin()) {
        return rt;
    }
    return nullptr;
}
MySQLTransaction::~MySQLTransaction()
{
    if(m_autoCommit) {
        commit();
    } else {
        rollback();
    }
}

bool MySQLTransaction::begin()
{
    int rt = execute("BEGIN");
    return rt == 0;
}

bool MySQLTransaction::commit()
{
    int rt = execute("COMMIT");
    if(rt == 0) {
        m_isFinished = true;
    }
    return rt == 0;
}

bool MySQLTransaction::rollback()
{
    if(m_isFinished) return true;
    int rt = execute("ROLLBACK");
    if(rt == 0) {
        m_isFinished = true;
    }
    return rt == 0;
}

int MySQLTransaction::execute(const std::string &sql)
{
    if(m_isFinished) {
        FEITENG_LOG_ERROR(g_logger) << "transaction is finished, sql: " << sql;
        return -1;
    }
    int rt = m_mysql->execute(sql);
    return rt;
}

int MySQLTransaction::execute(const char *format, va_list ap)
{
    if(m_isFinished) {
        FEITENG_LOG_ERROR(g_logger) << "transaction is finished, sql: " << format;
        return -1;
    }
    int rt = m_mysql->execute(format, ap);
    return rt;
}

int MySQLTransaction::execute(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    return execute(format, ap);
}

int64_t MySQLTransaction::getLastInsertId()
{
    return m_mysql->getLastInsertId();
}
std::shared_ptr<MySQL> MySQLTransaction::getMySQL()
{
    return m_mysql;
}
MySQL::MySQL(const std::string &name):m_name(name)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL", QString::fromStdString(name));
}
void MySQL::init(const std::string &host, int port, const std::string &user, const std::string &password, const std::string &dbName)
{
    m_db.setHostName(QString::fromStdString(host));
    m_db.setPort(port);
    m_db.setUserName(QString::fromStdString(user));
    m_db.setPassword(QString::fromStdString(password));
    m_db.setDatabaseName(QString::fromStdString(dbName));
}
bool MySQL::connect()
{
    if(m_db.open()) {
        FEITENG_LOG_INFO(g_logger) << "数据库连接成功";
        return true;
    } else {
        FEITENG_LOG_ERROR(g_logger) << "数据库连接失败" << m_db.lastError().text().toStdString();
        return false;
    }
    return false;
}
int MySQL::execute(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int rt = execute(format, ap);
    va_end(ap);
    return rt;
}
int MySQL::execute(const char *format, va_list ap)
{
    char* buf = nullptr;
    int len = vasprintf(&buf, format, ap);
    if(len == -1) {
        FEITENG_LOG_ERROR(g_logger) << "vasprintf error format=" << format;
        return -1;
    }
    std::string sql;
    sql.assign(buf, len);
    free(buf);
    return execute(sql);
}
int MySQL::execute(const std::string &sql)
{
    QSqlQuery query(m_db);
    if (!query.exec(QString::fromStdString(sql))) {
        FEITENG_LOG_ERROR(g_logger) << "sql= " << sql << " error: " << query.lastError().text().toStdString();
        return -1;
    }
    return 0;
}
ISQLData::ptr MySQL::query(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    ISQLData::ptr rt = query(format, ap);
    va_end(ap);
    return rt;
}
ISQLData::ptr MySQL::query(const char *format, va_list ap)
{
    char* buf = nullptr;
    int len = vasprintf(&buf, format, ap);
    if(len == -1) {
        FEITENG_LOG_ERROR(g_logger) << "vasprintf error format=" << format;
        return nullptr;
    }
    std::string sql;
    sql.assign(buf, len);
    free(buf);
    return query(sql);
}
ISQLData::ptr MySQL::query(const std::string &sql)
{
    QSqlQuery query(m_db);
    if (!query.exec(QString::fromStdString(sql))) {
        FEITENG_LOG_ERROR(g_logger) << "sql= " << sql << " error: " << query.lastError().text().toStdString();
        return nullptr;
    }
    MySQLRes::ptr rt(new MySQLRes(query));
    return rt;
}
IStmt::ptr MySQL::prepare(const QString &stmt)
{
    return MySQLStmt::Create(shared_from_this(), stmt);
}
int MySQL::getErrno()
{
    return m_db.lastError().number();
}
QString MySQL::getErrStr()
{
    return m_db.lastError().text();
}
ITransaction::ptr MySQL::openTransaction(bool auto_commit)
{
    return MySQLTransaction::Create(shared_from_this(), auto_commit);
}
int64_t MySQL::getLastInsertId()
{
    return m_query.lastInsertId().toLongLong();
}
}