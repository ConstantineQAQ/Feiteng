#include "admindialog.h"

Feiteng::MySQL::ptr a_mysql = FEITENG_MYSQL_NAME("feiteng");
static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("admindialog");

AdminDialog::AdminDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminDialog)
{
    ui->setupUi(this);
    a_mysql->connect(); // 连接数据库
}

AdminDialog::~AdminDialog()
{
    emit backButtonClicked();
    delete ui;
}

void AdminDialog::on_commit_pushButton_clicked()
{
    a_mysql->connect(); // 连接数据库
    QString account = ui->account_lineEdit->text();
    QString password = ui->passwd_lineEdit->text();
    FEITENG_LOG_INFO(g_logger) << account.toStdString() << " " << password.toStdString();
    Feiteng::MySQLStmt::ptr tempstmt = Feiteng::MySQLStmt::Create(a_mysql, "select * from admin where username = ? and password = ?");
    tempstmt->bindString(0, account.toStdString());
    tempstmt->bindString(1, password.toStdString());
    Feiteng::ISQLData::ptr tempdata = tempstmt->query();
    if(!tempdata->next()) {
        QMessageBox::warning(this, "登录提醒", "账号或密码错误，请重新登录!");
        return;
    } else {
        StudenInfoDialog *info = new StudenInfoDialog();
        info->exec();
    }
}

void AdminDialog::on_back_pushButton_clicked()
{
    emit backButtonClicked();
    this->close();
}
