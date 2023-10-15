#include "studeninfodialog.h"
extern Feiteng::MySQL::ptr g_mysql;
extern Feiteng::FaceRecognizer::ptr g_face_recognizer;
static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("studeninfodialog");

StudenInfoDialog::StudenInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudenInfoDialog)
{
    ui->setupUi(this);
    g_mysql->connect(); // 连接数据库
    queryAndDisplay();  // 查询并显示结果
}

void StudenInfoDialog::on_deleteButton_clicked()
{
    Feiteng::MySQLStmt::ptr delStudent = Feiteng::MySQLStmt::Create(g_mysql, "delete from Student where ID = ?");
    Feiteng::MySQLStmt::ptr delAttendance = Feiteng::MySQLStmt::Create(g_mysql, "delete from Attendance where StudentID = ?");
    int currentRow = ui->tableWidget->currentRow();  // 获取当前选中的行号
    // 获取要删除的员工ID
    QString studentID = ui->tableWidget->item(currentRow, 0)->text();  // 假设ID是表格的第一列
    FEITENG_LOG_INFO(g_logger) << "要删除的学生ID为：" << studentID.toStdString();
    delStudent->bindString(0, studentID.toStdString());
    delAttendance->bindString(0, studentID.toStdString());
    delStudent->execute();
    delAttendance->execute();
    // 删除后刷新表格
    queryAndDisplay();
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
            // 添加删除按钮
            QPushButton *deleteButton = new QPushButton("删除");
            // QWidget *actionWidget = new QWidget();
            // QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
            // actionLayout->addWidget(deleteButton);
            // actionWidget->setLayout(actionLayout);
            // ui->tableWidget->setCellWidget(row,7,actionWidget);
            ui->tableWidget->setCellWidget(row, 7, deleteButton);
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
