#include "../include/database.h"
#include "../include/log.h"
#include "../include/config.h"

YAML::Node databasenode = YAML::LoadFile("../conf/database.yml");
Feiteng::MySQL::ptr g_mysql = FEITENG_MYSQL_NAME("feiteng");

void testquery() {
    Feiteng::ISQLData::ptr res = g_mysql->query("select * from students_face_recognition");
    if (res) {
        while (res->next()) {
            FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << res->getValue(0).toString().toStdString() << " "
                                                << res->getString(1) << " "
                                                << res->getString(2) << " "
                                                << res->getString(3) << " ";
        }
    }
}

void testexec() {
    Feiteng::MySQLTransaction::ptr g_mysqltransaction = Feiteng::MySQLTransaction::Create(g_mysql, true);
    int res = g_mysqltransaction->execute("insert into students_face_recognition values(1, 'test', 'test', 'test')");
    if (res) {
        FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "insert success";
    } else {
        FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "insert failed";
    }
}

int main() {
    Feiteng::Config::LoadFromYaml(databasenode);
    g_mysql->connect();
    testexec();
}