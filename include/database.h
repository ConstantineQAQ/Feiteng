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
    MySQLStmtRes(std::shared_ptr<MySQLStmt> stmt);
    struct Data {
        Data();
        ~Data();

        bool is_null;
        bool error;
        QVariant value;
    };
    QSqlQuery m_query;
    std::shared_ptr<MySQLStmt> m_stmt;
    std::vector<Data> m_datas;
};

class MySQLTransaction : public ITransaction {
};

class MySQLStmt : public IStmt {
public:
    typedef std::shared_ptr<MySQLStmt> ptr;
    static MySQLStmt::ptr Create(MySQL::ptr db, const QString& stmt);
    QSqlQuery& getQuery() { return m_query; }
private:
    MySQLStmt(MySQL::ptr db);
    MySQL::ptr m_mysql;
    QSqlQuery m_query;
};

class MySQL : public IDB , public std::enable_shared_from_this<MySQL>{
friend class MySQLManager;
public:
    typedef std::shared_ptr<MySQL> ptr;
    MySQL(const std::string& name = "root") {
        m_db = QSqlDatabase::addDatabase("QMYSQL", QString::fromStdString(name));
    }
    void init(const std::string& host, int port, const std::string& user, const std::string& password, const std::string& dbName) {
        m_db.setHostName(QString::fromStdString(host));
        m_db.setPort(port);
        m_db.setUserName(QString::fromStdString(user));
        m_db.setPassword(QString::fromStdString(password));
        m_db.setDatabaseName(QString::fromStdString(dbName));
    }
    bool connect(const QString& host, int port, const QString& user, const QString& password, const QString& dbName) {
        m_db.setHostName(host);
        m_db.setPort(port);
        m_db.setUserName(user);
        m_db.setPassword(password);
        m_db.setDatabaseName(dbName);
        return m_db.open();
    }
    std::string getName() const { return m_name; }

    int execute(const std::string& sql) override {
        QSqlQuery query(m_db);
        if (!query.exec(QString::fromStdString(sql))) {
            return -1;  // Error
        }
        return query.numRowsAffected();
    }

    ISQLData::ptr query(const std::string& sql) override {
        QSqlQuery qsqlQuery(m_db);
        if (!qsqlQuery.exec(QString::fromStdString(sql))) {
            return nullptr;  // Error
        }
        return std::make_shared<MySQLRes>(qsqlQuery);
    }

    QSqlDatabase& getDB() { return m_db; }
    IStmt::ptr prepare(const QString& stmt) override { /* Implementation */ }
    int getErrno() override { /* Implementation */ }
    QString getErrStr() override { /* Implementation */ }
    ITransaction::ptr openTransaction(bool auto_commit = false) override { /* Implementation */ }

protected:
    QSqlDatabase m_db;
    MySQL::ptr m_root;
    std::string m_name;
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