#include "studeninfodialog.h"
extern Feiteng::MySQL::ptr g_mysql;
extern Feiteng::FaceRecognizer::ptr g_face_recognizer;
extern YAML::Node facenode;
extern YAML::Node serialnode;
extern YAML::Node databasenode;
extern YAML::Node lognode;
extern YAML::Node tempernode;
static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("studeninfodialog");


StudenInfoDialog::StudenInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudenInfoDialog)
{
    ui->setupUi(this);
    g_mysql->connect(); // 连接数据库
    queryAndDisplay();  // 查询并显示结果
}

// 实现查询和显示结果的函数
void StudenInfoDialog::queryAndDisplay()
{
    // 清除旧数据
    ui->tableWidget->setRowCount(0);
    // 查询
    Feiteng::ISQLData::ptr data =  g_mysql->query("SELECT Student.ID, Student.Name, Student.Department, Student.PhotoPath, Attendance.SignedToday, Attendance.LastSignDate, Attendance.Temperature FROM Student LEFT JOIN Attendance ON Student.ID = Attendance.StudentID");
    int row = 0;
    // 显示新数据
    if (data) {
        while (data->next()) {
            ui->tableWidget->insertRow(row);  // 使用行索引变量
            for (int col = 0; col < 7; ++col) {
                ui->tableWidget->setItem(row, col, new QTableWidgetItem(data->getString(col).c_str()));
            }
            row++;
        }
    } else {
        FEITENG_LOG_ERROR(g_logger) << "查询结果为空";
    }
}


StudenInfoDialog::~StudenInfoDialog()
{
    delete ui;
}

void StudenInfoDialog::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    // 获取当前选中的行号
    int currentRow = ui->tableWidget->currentRow();
    // 获取要删除的学生ID
    QString studentID = ui->tableWidget->item(currentRow, 0)->text();  // 假设ID是表格的第一列
    FEITENG_LOG_INFO(g_logger) << "要删除的学生ID为：" << studentID.toStdString();
    ui->delete_LineEdit->setText(studentID);
}

void StudenInfoDialog::on_add_PushButton_clicked()
{
    AddStudentDialog *addStudentDialog = new AddStudentDialog(this);
    addStudentDialog->show();
}

void StudenInfoDialog::on_train_pushButton_clicked()
{
    g_face_recognizer->loadFromDir("/home/kylin/Feiteng/face");
    g_face_recognizer->train();
    QMessageBox::information(nullptr, "训练提醒", "训练完成!");
}

void StudenInfoDialog::on_delete_PushButton_clicked()
{
    Feiteng::MySQLTransaction::ptr temptransaction = Feiteng::MySQLTransaction::Create(g_mysql, true);
    temptransaction->begin();
    // 获取要删除的员工ID
    std::string StudentID = ui->delete_LineEdit->text().toStdString();
    Feiteng::MySQLStmt::ptr delAttendance = Feiteng::MySQLStmt::Create(g_mysql, "delete from Attendance where StudentID = ?");
    Feiteng::MySQLStmt::ptr delStudent = Feiteng::MySQLStmt::Create(g_mysql, "delete from Student where ID = ?");
    FEITENG_LOG_INFO(g_logger) << "要删除的学生ID为：" << StudentID;
    delStudent->bindString(0, StudentID);
    delAttendance->bindString(0, StudentID);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"删除确认","确定要删除这条记录吗？",QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        delAttendance->execute();
        delStudent->execute();
        temptransaction->commit();
        if(temptransaction->isFinished()) {
           std::filesystem::path dir_path("/home/kylin/Feiteng/face/" + StudentID);
           std::filesystem::remove_all(dir_path);
           FEITENG_LOG_INFO(g_logger) << "人脸照片删除成功";
           QMessageBox::information(this,"删除成功","用户已成功删除");
           FEITENG_LOG_INFO(g_logger) << "删除成功 " << StudentID;
        }else{
            QMessageBox::information(this,"删除失败","无法删除用户");
            FEITENG_LOG_ERROR(g_logger) << "删除失败 " << StudentID;
        }
    }
    // 删除后刷新表格
    queryAndDisplay();
}

void StudenInfoDialog::on_yaml_pushButton_clicked()
{
    Feiteng::Config::LoadFromYaml(facenode);
    Feiteng::Config::LoadFromYaml(serialnode);
    Feiteng::Config::LoadFromYaml(databasenode);
    Feiteng::Config::LoadFromYaml(lognode);
    Feiteng::Config::LoadFromYaml(tempernode);
}
