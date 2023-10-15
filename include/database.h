#ifndef DATABASE_H
#define DATABASE_H

#include "util.h"
#include "singleton.h"
#include "config.h"
#include "db.h"

#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QtSql/QSqlRecord>
#include <QDateTime>
#include <QSqlDriver>
#include <QSqlField>
#include <QSqlResult>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>
#include <memory>
#include <string>
#include <functional>

#define FEITENG_MYSQL_ROOT() Feiteng::MySQLMgr::GetInstance()->getRoot()
#define FEITENG_MYSQL_NAME(name) Feiteng::MySQLMgr::GetInstance()->getMySQL(name)
#define FEITENG_MYSQL_DELETE(name) Feiteng::MySQLMgr::GetInstance()->del(name)

namespace Feiteng {

class MySQL;
class MySQLStmt;

class MySQLRes : public ISQLData {
public:
    typedef std::shared_ptr<MySQLRes> ptr;
    MySQLRes(const QSqlQuery& query) : m_query(query) {}
    bool isValid() const override { return m_query.isValid(); }
    QString lastError() const override { return m_query.lastError().text(); }
    int getDataCount() override { return m_query.size(); }
    int getColumnCount() override { return m_query.record().count(); }
    int getColumnBytes(int idx) override { return m_query.record().field(idx).length(); }
    QVariant::Type getColumnType(int idx) override { return m_query.record().field(idx).type(); }
    QString getColumnName(int idx) override { return m_query.record().fieldName(idx); }
    bool isNull(int idx) override { return m_query.isNull(idx); }
    QVariant getValue(int idx) override { return m_query.value(idx); }
    std::string getString(int idx) override { return m_query.value(idx).toString().toStdString(); }
    std::string getBlob(int idx) override { return m_query.value(idx).toByteArray().toStdString(); }
    time_t getTime(int idx) override { return m_query.value(idx).toDateTime().toTime_t(); }
    bool next() override { return m_query.next(); }
private:
    QSqlQuery m_query;
};

class MySQLStmtRes : public ISQLData {
friend class MySQLStmt;
public:
    typedef std::shared_ptr<MySQLStmtRes> ptr;
    static MySQLStmtRes::ptr Create(std::shared_ptr<MySQLStmt> stmt);
    ~MySQLStmtRes();
    bool isValid() const override { return m_query.isValid(); }
    QString lastError() const override { return m_query.lastError().text(); }
    
    int getDataCount() override { return m_query.size(); }
    int getColumnCount() override { return m_query.record().count(); }
    int getColumnBytes(int idx) override { return m_query.record().field(idx).length(); }
    QVariant::Type getColumnType(int idx) override { return m_query.record().field(idx).type(); }
    QString getColumnName(int idx) override { return m_query.record().fieldName(idx); }
    bool isNull(int idx) override { return m_query.isNull(idx); }
    QVariant getValue(int idx) override { return m_query.value(idx); }
    std::string getString(int idx) override { return m_query.value(idx).toString().toStdString(); }
    std::string getBlob(int idx) override { return m_query.value(idx).toByteArray().toStdString(); }
    time_t getTime(int idx) override { return m_query.value(idx).toDateTime().toTime_t(); }
    bool next() override { return m_query.next(); }
private:
    MySQLStmtRes(std::shared_ptr<MySQLStmt> stmt): m_stmt(stmt) {}
    QSqlQuery m_query;
    std::shared_ptr<MySQLStmt> m_stmt;
};

class MySQLManager;
class MySQL : public IDB , public std::enable_shared_from_this<MySQL>{
friend class MySQLManager;
public:
    typedef std::shared_ptr<MySQL> ptr;
    MySQL(const std::string& name = "root");
    void init(const std::string& host, int port, const std::string& user, const std::string& password, const std::string& dbName);
    bool connect();
    std::string getName() const { return m_name; }

    virtual int execute(const char* format, ...) override;
    int execute(const char* format, va_list ap);
    virtual int execute(const std::string& sql) override;

    ISQLData::ptr query(const char* format, ...) override;
    ISQLData::ptr query(const char* format, va_list ap); 
    ISQLData::ptr query(const std::string& sql) override;

    QSqlDatabase& getDB() { return m_db; }
    QSqlQuery& getQuery() { return m_query; }
    IStmt::ptr prepare(const QString& stmt) override;
    int getErrno() override;
    QString getErrStr() override;
    ITransaction::ptr openTransaction(bool auto_commit = false) override;
    int64_t getLastInsertId() override;

protected:
    QSqlDatabase m_db; // QT数据库连接
    QSqlQuery m_query; // QT数据库查询
    MySQL::ptr m_root; // 主数据库
    std::string m_name; // 数据库连接名称
};

class MySQLTransaction : public ITransaction {
public:
    typedef std::shared_ptr<MySQLTransaction> ptr;
    static MySQLTransaction::ptr Create(MySQL::ptr mysql, bool auto_commit);
    virtual ~MySQLTransaction();

    bool begin() override;
    bool commit() override;
    bool rollback() override;

    virtual int execute(const std::string& sql) override;
    int execute(const char* format, va_list ap);
    virtual int execute(const char* format, ...) override;
    int64_t getLastInsertId() override;
    std::shared_ptr<MySQL> getMySQL();
    bool isAutoCommit() const { return m_autoCommit;}
    bool isFinished() const { return m_isFinished;}
private:
    MySQLTransaction(MySQL::ptr mysql, bool auto_commit): m_mysql(mysql), m_autoCommit(auto_commit), m_isFinished(false) {};
private:
    MySQL::ptr m_mysql;
    bool m_autoCommit;
    bool m_isFinished;
};

class MySQLStmt : public IStmt 
    ,public std::enable_shared_from_this<MySQLStmt>{
public:
    typedef std::shared_ptr<MySQLStmt> ptr;
    static MySQLStmt::ptr Create(MySQL::ptr db, const QString& stmt);
    QSqlQuery& getQuery() { return m_query; }
    void bindInt(int idx, const int& value) override { m_query.bindValue(idx, value); }
    void bindFloat(int idx, const float& value) override { m_query.bindValue(idx, value); }
    void bindDouble(int idx, const double& value) override { m_query.bindValue(idx, value); }
    void bindString(int idx, const char* value) override { m_query.bindValue(idx, value); }
    void bindString(int idx, const std::string& value) override { m_query.bindValue(idx, value.c_str()); }
    void bindBlob(int idx, const void* value, int64_t size) override { m_query.bindValue(idx, QByteArray(static_cast<const char*>(value), size)); }
    void bindBlob(int idx, const std::string& value) override { m_query.bindValue(idx, QByteArray(value.c_str(), value.size())); }
    void bindTime(int idx, const time_t& value) override { m_query.bindValue(idx, QDateTime::fromTime_t(value)); }
    void bindNull(int idx) override { m_query.bindValue(idx, QVariant::String); }

    std::string getErrStr() override { return m_query.lastError().text().toStdString(); }

    int execute() override;
    int64_t getLastInsertId() override;
    ISQLData::ptr query() override;
private:
    MySQLStmt(MySQL::ptr db) : m_mysql(db) {}
    MySQL::ptr m_mysql;
    QSqlQuery m_query;
};

class MySQLManager {
public:
    MySQLManager();
    typedef std::shared_ptr<MySQLManager> ptr;
    typedef Mutex MutexType;

    MySQL::ptr getMySQL(const std::string& name);
    void del(const std::string& name);
    MySQL::ptr getRoot() const { return m_root; }
private:
    std::map<std::string, MySQL::ptr> m_databases;
    MySQL::ptr m_root;
    MutexType m_mutex;
};

typedef Feiteng::Singleton<MySQLManager> MySQLMgr;

}
#endif // DATABASE_H