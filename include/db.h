#ifndef DB_H
#define DB_H

#include <memory>
#include <string>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

namespace Feiteng {

class ISQLData {
public:
    typedef std::shared_ptr<ISQLData> ptr;
    virtual ~ISQLData() {}

    virtual bool isValid() const = 0;
    virtual QString lastError() const = 0;

    virtual int getDataCount() = 0;
    virtual int getColumnCount() = 0;
    virtual int getColumnBytes(int idx) = 0;
    virtual QVariant::Type getColumnType(int idx) = 0;
    virtual QString getColumnName(int idx) = 0;

    virtual bool isNull(int idx) = 0;
    virtual QVariant getValue(int idx) = 0;
    virtual std::string getString(int idx) = 0;
    virtual std::string getBlob(int idx) = 0;
    virtual time_t getTime(int idx) = 0;
    virtual bool next() = 0;
};

class ISQLUpdate {
public:
    virtual ~ISQLUpdate() {}
    virtual int execute(const char* format, ...) = 0;
    virtual int execute(const std::string& sql) = 0;
    virtual int64_t getLastInsertId() = 0;
};

class ISQLQuery {
public:
    virtual ~ISQLQuery() {}
    virtual ISQLData::ptr query(const char* format, ...) = 0;
    virtual ISQLData::ptr query(const std::string& sql) = 0;
};

class IStmt {
public:
    typedef std::shared_ptr<IStmt> ptr;
    virtual ~IStmt(){}
    virtual void bindInt(int idx, const int& value) = 0;
    virtual void bindFloat(int idx, const float& value) = 0;
    virtual void bindDouble(int idx, const double& value) = 0;
    virtual void bindString(int idx, const char* value) = 0;
    virtual void bindString(int idx, const std::string& value) = 0;
    virtual void bindBlob(int idx, const void* value, int64_t size) = 0;
    virtual void bindBlob(int idx, const std::string& value) = 0;
    virtual void bindTime(int idx, const time_t& value) = 0;
    virtual void bindNull(int idx) = 0;

    virtual int execute() = 0;
    virtual int64_t getLastInsertId() = 0;
    virtual ISQLData::ptr query() = 0;

    virtual std::string getErrStr() = 0;
};

class ITransaction : public ISQLUpdate {
public:
    typedef std::shared_ptr<ITransaction> ptr;
    virtual ~ITransaction() {};
    virtual bool begin() = 0;
    virtual bool commit() = 0;
    virtual bool rollback() = 0;
};

class IDB : public ISQLUpdate, public ISQLQuery {
public:
    typedef std::shared_ptr<IDB> ptr;
    virtual ~IDB() {}

    virtual IStmt::ptr prepare(const QString& stmt) = 0;  
    virtual int getErrno() = 0;
    virtual QString getErrStr() = 0;  
    virtual ITransaction::ptr openTransaction(bool auto_commit = false) = 0;

protected:
    QSqlDatabase m_db;  
};

}

#endif // DB_H