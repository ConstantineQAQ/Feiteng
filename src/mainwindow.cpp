#include "mainwindow.h"

static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("mainwindow");
Feiteng::MySQL::ptr g_mysql = FEITENG_MYSQL_NAME("info");
Feiteng::Serial::ptr g_serial = FEITENG_SERIAL_NAME("wendu");
Feiteng::FaceRecognizer::ptr g_face_recognizer = std::make_shared<Feiteng::FaceRecognizer>();
extern Feiteng::FaceConfig::ptr g_face_config;
QMetaObject::Connection connection;
QMetaObject::Connection connection2;
YAML::Node facenode = YAML::LoadFile("../conf/face.yml");
YAML::Node serialnode = YAML::LoadFile("../conf/serial.yml");
YAML::Node databasenode = YAML::LoadFile("../conf/database.yml");
YAML::Node lognode = YAML::LoadFile("../conf/log.yml");
YAML::Node tempernode = YAML::LoadFile("../conf/temperature.yml");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , Ttimer(new QTimer(this))
    , Ftimer(new QTimer(this))
    , m_face(Feiteng::FaceInfo::ptr(new Feiteng::FaceInfo()))
    , m_thread(new QThread(this))
    , m_frameProcessor(new FrameProcessor())
    , adminDialog(new AdminDialog(this))
{
    ui->setupUi(this);
    Feiteng::Config::LoadFromYaml(facenode);
    Feiteng::Config::LoadFromYaml(serialnode);
    Feiteng::Config::LoadFromYaml(databasenode);
    Feiteng::Config::LoadFromYaml(lognode);
    Feiteng::Config::LoadFromYaml(tempernode);
    if (g_mysql->connect()) {
        FEITENG_LOG_INFO(g_logger) << "数据库连接成功";
    } else {
        FEITENG_LOG_ERROR(g_logger) << "数据库连接失败";
    }
    connect(m_frameProcessor, &FrameProcessor::frameProcessed, this, &MainWindow::updateFaceLabel);
    connect(this, &MainWindow::faceRecognitionDone, this, &MainWindow::handleFaceRecognitionDone);
    connect(this, &MainWindow::TemperDone, this, &MainWindow::handleTemperatureDone);
    connect(adminDialog, &AdminDialog::backButtonClicked, this, [this]{
        FEITENG_CAMERA_OPEN();
        m_thread->start();
    });
    Ftimer->start(50);
    m_frameProcessor->setFaceInfo(m_face);
    m_frameProcessor->moveToThread(m_thread);
    connect(Ftimer, SIGNAL(timeout()), m_frameProcessor, SLOT(processFrame()));
    m_thread->start();
    FEITENG_CAMERA_OPEN(); // 打开摄像头
}

MainWindow::~MainWindow()
{
    m_thread->quit();
    m_thread->wait();
    delete ui;
}

void MainWindow::updateFaceLabel(const QImage &image)
{
    ui->face_label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::handleFaceRecognitionDone(int label, double confidence)
{
    m_person = nullptr;
    if(confidence < 0) {
        QMessageBox::warning(this, "签到提醒", "未检测到人脸，请重新签到!");
        FEITENG_LOG_ERROR(g_logger) << "未检测到人脸，请重新签到!";
        return;
    }
    Feiteng::MySQLStmt::ptr tempstmt = Feiteng::MySQLStmt::Create(g_mysql, "select * from Student where id = ?");
    tempstmt->bindString(0, std::to_string(label));
    Feiteng::ISQLData::ptr tempdata = tempstmt->query();
    FEITENG_LOG_INFO(g_logger) << "查询到的数据条数为：" << tempdata->getDataCount();
    if(tempdata->getDataCount() > 0) {
        if (tempdata->next()) {
            FEITENG_LOG_INFO(g_logger) << "查询到的数据为：" << tempdata->getValue(0).toString().toStdString() << " " << tempdata->getValue(1).toString().toStdString() << " " << tempdata->getValue(2).toString().toStdString() << " " << tempdata->getValue(3).toString().toStdString();
            m_person = Feiteng::Person::ptr(new Feiteng::Person());
            m_person->setId(tempdata->getValue(0).toString().toStdString());
            m_person->setName(tempdata->getValue(1).toString().toStdString());
            m_person->setDepartment(tempdata->getValue(2).toString().toStdString());
            m_person->setPhotoPath(tempdata->getValue(3).toString().toStdString());
        }
    } else {
        QMessageBox::warning(this, "签到提醒", "未查询到相关人员信息，请重新签到!");
        FEITENG_LOG_ERROR(g_logger) << "未查询到相关人员信息，请重新签到!";
        return;
    }
    ConfirmDialog dialog(
        QString("签到成功，签到人员信息为：\n姓名：%1\n学号：%2\n学院：%3")
        .arg(QString::fromStdString(m_person->getName()))
        .arg(QString::fromStdString(m_person->getId()))
        .arg(QString::fromStdString(m_person->getDepartment())), 
        this
    );  // 传递签到人员信息到弹窗
    dialog.setWindowTitle("确认签到信息");  // 设置弹窗标题
    dialog.setFixedSize(300, 200);    // 设置弹窗为固定大小
    if (dialog.exec() == QDialog::Accepted) {
        // 用户确认签到，执行相应的逻辑，例如更新数据库
        g_mysql->execute("update Attendance set SignedToday = 1 , LastSignDate = now() where StudentID = " + std::to_string(label));
    } else {
        // 用户返回，执行相应的逻辑，例如重新采集图像
        return;
    }
}

void MainWindow::handleTemperatureDone(double temper)
{
    TemperDialog dialog(QString("测量到的体温平均值为：%1").arg(temper), this);  // 传递测量到的体温平均值到弹窗
    dialog.setWindowTitle("确认测量温度信息");  // 设置弹窗标题
    dialog.setFixedSize(300, 200);    // 设置弹窗为固定大小
    if (dialog.exec() == QDialog::Accepted) {
        // 用户确认签到，执行相应的逻辑，例如更新数据库
        Feiteng::MySQLStmt::ptr tempstmt = Feiteng::MySQLStmt::Create(g_mysql, "update Attendance set Temperature = ? where StudentID = ?");
        tempstmt->bindDouble(0, temper);
        tempstmt->bindString(1, m_person->getId());
        tempstmt->execute();
    } else {

    }
}

void MainWindow::processFaceRecognition()
{
    m_face->detectFace();
    Feiteng::FaceRecognizer::ptr face_recognizer = Feiteng::FaceRecognizer::ptr(new Feiteng::FaceRecognizer());
    face_recognizer->getRecognizer()->read("../model/face_recognizer.xml");
    // face_recognizer->train();
    cv::Mat test_face = m_face->getFaceROI();
    Feiteng::FacePredict::ptr face_predict = Feiteng::FacePredict::ptr(new Feiteng::FacePredict(face_recognizer));
    face_predict->predict(test_face);
    emit faceRecognitionDone(face_predict->getLabel(), face_predict->getConfidence());
}

void MainWindow::on_work_pushButton_clicked()
{
    QtConcurrent::run(this, &MainWindow::processFaceRecognition);
}

void MainWindow::on_temper_pushButton_clicked()
{
    // 断开现有的连接
    QObject::disconnect(connection);
    QObject::disconnect(connection2);
    if(m_person == nullptr) {
        QMessageBox::warning(this, "签到提醒", "还未签到，没有相关人员信息!");
    } else {
        // 人员已经签到，开始测量体温
        Ttimer->setInterval(m_person->getTemperature()->getConfig()->getInterval());  // 设置计时器的间隔
        count = 0;// 测温次数
        bodyTempVec.clear(); // 体温数据
        g_serial->Sopen();
        Feiteng::Temperature::ptr bodyTemp = m_person->getTemperature();
        bodyTemp->setSerial(std::shared_ptr<Feiteng::Serial>(g_serial));
        Feiteng::BodyTemp* bodyTempPtr = dynamic_cast<Feiteng::BodyTemp*>(bodyTemp.get());
        // 测量十次体温求平均值，并且将不符合标准的数据剔除直到够十次
        QByteArray startMeasuringCmd = QByteArray::fromHex("FACAC4");
        QByteArray bodyTempModeCmd = QByteArray::fromHex("FAC5BF");
        connection = QObject::connect(Ttimer, &QTimer::timeout, this, [startMeasuringCmd, bodyTempPtr, this]() {
            if (count < 10) {  // 检查是否已经测量了十次
                FEITENG_LOG_INFO(g_logger) << "发送开始测量命令";
                g_serial->Swrite(startMeasuringCmd);  // 发送开始测量命令
            } else {
                Ttimer->stop();  // 停止计时器，结束测量
                // 检查 bodyTempVec 是否包含十个有效的体温数据
                if(bodyTempVec.size() < 10) {
                    QMessageBox::warning(this, "测量提醒", "测量体温次数不足10次，请重新测量!");
                    return;
                }
                // 如果不是，需要重新开始测量或采取其他措施
                // 计算平均值
                double sum = 0;
                for (auto& i : bodyTempVec) {
                    sum += i;
                }
                double average = sum / bodyTempVec.size();
                FEITENG_LOG_INFO(g_logger) << "测量到的体温平均值为：" << average;
                emit TemperDone(average);
            }
        });
        connection2 = QObject::connect(g_serial->getSerial(), &QSerialPort::readyRead, this, [bodyTempPtr,this]() {
            bodyTempPtr->fetchTemperature();  // 获取体温数据
            double temperature = bodyTempPtr->getBodyTemperature();
            if (temperature != 0 && !(temperature > 42)) {  // 检查温度值是否有效
                count++;  // 增加测量计数
                bodyTempVec.push_back(temperature);  // 将测量到的体温数据存入容器
                FEITENG_LOG_INFO(g_logger) << "测量到的体温数据为：" << temperature;
            }
        });
        Ttimer->start();  // 开始计时器
    }
}

void MainWindow::on_admin_pushButton_clicked()
{
    FEITENG_CAMERA_CLOSE();
    m_thread->quit();
    adminDialog->setWindowTitle("管理员登录");
    adminDialog->exec();
}

void FrameProcessor::processFrame() {
    cv::Mat tempframe;
    FEITENG_CAMERA() >> tempframe;
    std::vector<cv::Rect> faces = m_face->getFaces();
    m_face->getFaceCascade()->detectMultiScale(tempframe, faces,1.2,3,0|cv::CASCADE_SCALE_IMAGE,cv::Size(90,90));
    for(size_t i=0;i<faces.size();i++){
        cv::rectangle(tempframe,faces[i],cv::Scalar(255,0,0),2);
        cv::putText(tempframe,"Test_test",cv::Point(faces[i].x,faces[i].y),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(0,255,0));
    }
    QImage qimage = QImage(tempframe.data,tempframe.cols,tempframe.rows,QImage::Format_RGB888).rgbSwapped();
    emit frameProcessed(qimage);
}
