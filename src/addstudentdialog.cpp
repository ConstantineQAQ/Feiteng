#include "addstudentdialog.h"

static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("AddStudentDialog");
extern Feiteng::FaceConfig::ptr g_face_config;
extern Feiteng::FaceRecognizer::ptr g_face_recognizer;
extern Feiteng::MySQL::ptr g_mysql;

AddStudentDialog::AddStudentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStudentDialog),
    Ftimer(new QTimer(this))
    
{
    ui->setupUi(this);
    m_person = Feiteng::Person::ptr(new Feiteng::Person());
    g_mysql->connect(); // 连接数据库
    Ftimer->start(33);
    connect(Ftimer, SIGNAL(timeout()), this, SLOT(displayFrame()));
    connect(this, &AddStudentDialog::faceInputCompleted, this,[]
    {
        QMessageBox::information(nullptr, "添加学生提醒", "学生信息录入成功!");
    });
    FEITENG_CAMERA_OPEN(); // 打开摄像头
}

AddStudentDialog::~AddStudentDialog()
{
    delete ui;
}

void AddStudentDialog::closeEvent(QCloseEvent *event)
{
    FEITENG_CAMERA_CLOSE();
    Ftimer->stop();
    QDialog::closeEvent(event);
}

void AddStudentDialog::displayFrame() {
    cv::Mat tempframe;
    FEITENG_CAMERA() >> tempframe;
    Feiteng::FaceInfo::ptr tempface = m_person->getFaceInfo();
    std::vector<cv::Rect> faces = tempface->getFaces();
    tempface->getFaceCascade()->detectMultiScale(tempframe, faces,1.2,3,0|cv::CASCADE_SCALE_IMAGE,cv::Size(90,90));
    std::string text = m_person->getId().empty() ? "unknow" : m_person->getId();
    for(size_t i=0;i<faces.size();i++){
        cv::rectangle(tempframe,faces[i],cv::Scalar(255,0,0),2);
        cv::putText(tempframe,text,cv::Point(faces[i].x,faces[i].y),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(0,255,0));
    }
    QImage qimage = QImage(tempframe.data,tempframe.cols,tempframe.rows,QImage::Format_RGB888).rgbSwapped();
    ui->face_label->setPixmap(QPixmap::fromImage(qimage));
}

void AddStudentDialog::on_add_pushButton_clicked()
{
    if(m_person->getId().empty() || m_person->getName() == "unknow" || m_person->getDepartment().empty()) {
        QMessageBox::warning(this, "添加学生提醒", "学生信息不完全!");
        FEITENG_LOG_ERROR(g_logger) << "学生信息不完全!";
        return;
    } else {
        QtConcurrent::run(this, &AddStudentDialog::FaceInput);
    }
}

void AddStudentDialog::FaceInput() {
    Feiteng::FaceInfo::ptr tempface = m_person->getFaceInfo();
    tempface->setLabel(m_person->getId());
    tempface->faceRecorde();
    // 数据库操作
    Feiteng::MySQLTransaction::ptr temptransaction = Feiteng::MySQLTransaction::Create(g_mysql, true);
    temptransaction->begin();
    Feiteng::MySQLStmt::ptr tempstmt = Feiteng::MySQLStmt::Create(g_mysql, 
    "insert into Student(ID, Name, Department, PhotoPath) values(?, ?, ?, ?)");
    Feiteng::MySQLStmt::ptr tempstmt1 = Feiteng::MySQLStmt::Create(g_mysql,
    "insert into Attendance(StudentID) values(?)");
    tempstmt->bindString(0, m_person->getId());
    tempstmt->bindString(1, m_person->getName());
    tempstmt->bindString(2, m_person->getDepartment());
    tempstmt->bindString(3, "/home/kylin/Feiteng/face/" + m_person->getId());
    tempstmt->execute();
    tempstmt1->bindString(0, m_person->getId());
    tempstmt1->execute();
    temptransaction->commit();
    emit faceInputCompleted();  // 发射信号
}


void AddStudentDialog::on_saveButton_clicked()
{
    m_person->setId(ui->id_lineEdit->text().toStdString());
    m_person->setName(ui->name_lineEdit->text().toStdString());
    m_person->setDepartment(ui->department_lineEdit->text().toStdString());
    QMessageBox::information(this, "添加学生提醒", "学生信息保存成功!");
}
