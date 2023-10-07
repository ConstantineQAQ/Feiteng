#include "mainwindow.h"

static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("mainwindow");
Feiteng::Database::ptr g_database = FEITENG_DATABASE_NAME("mainwindow");
QMetaObject::Connection connection;
QMetaObject::Connection connection2;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , Ttimer(new QTimer(this))
    , Ftimer(new QTimer(this))
{
    ui->setupUi(this);
    g_logger->addAppender(Feiteng::LogAppender::ptr(new Feiteng::FileLogAppender("../log/face.log"))); // 添加日志输出器
    g_logger->addAppender(Feiteng::LogAppender::ptr(new Feiteng::StdoutLogAppender)); // 添加日志输出器
    serialConfigInit(); // 串口配置初始化
    faceConfigInit(); // 人脸配置初始化
    temperatureConfigInit(); // 体温配置初始化
    FEITENG_CAMERA_OPEN(); // 打开摄像头
    connect(Ftimer, SIGNAL(timeout()), this, SLOT(disPlayFrame()));
    connect(this, &MainWindow::faceRecognitionDone, this, &MainWindow::handleFaceRecognitionDone);
    Ftimer->start(33);
}

void MainWindow::serialConfigInit()
{
    m_serial = Feiteng::Serial::ptr(new Feiteng::Serial());
    m_serial_config = Feiteng::Config::Lookup<Feiteng::SerialConfig>
    (
        "serial.serial_config" 
        , Feiteng::SerialConfig("ttyUSB0")
        , "Serial config"
    );
    m_serial_config->addListener([this](const Feiteng::SerialConfig& old_value, const Feiteng::SerialConfig& new_value) {
        m_serial->setConfig(std::shared_ptr<Feiteng::SerialConfig>(
            new Feiteng::SerialConfig(new_value)
        ));
    });
    m_serial->setConfig(std::shared_ptr<Feiteng::SerialConfig>(new Feiteng::SerialConfig(m_serial_config->getValue())));
}

void MainWindow::faceConfigInit()
{
    m_face_config = Feiteng::Config::Lookup<Feiteng::FaceConfig>
    (
        "face_config" 
        , Feiteng::FaceConfig()
        , "Face config"
    );
    m_face = Feiteng::FaceInfo::ptr(new Feiteng::FaceInfo());
    m_face->setConfig(std::shared_ptr<Feiteng::FaceConfig>(new Feiteng::FaceConfig(m_face_config->getValue())));
    m_face_config->addListener([this](const Feiteng::FaceConfig& old_value, const Feiteng::FaceConfig& new_value) {
        m_face->setConfig(std::shared_ptr<Feiteng::FaceConfig>(
            new Feiteng::FaceConfig(new_value)
        ));
        FEITENG_LOG_INFO(g_logger) << "人脸配置已更新";
    });
}

void MainWindow::temperatureConfigInit()
{
    m_temperature_config = Feiteng::Config::Lookup<Feiteng::TemperatureConfig>
    (
        "serial.temperature_config" 
        , Feiteng::TemperatureConfig()
        , "Temperature config"
    );
    m_temperature_config->addListener([this](const Feiteng::TemperatureConfig& old_value, const Feiteng::TemperatureConfig& new_value) {
        Ttimer->setInterval(new_value.getInterval());
    });
    Ttimer->setInterval(m_temperature_config->getValue().getInterval());
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::disPlayFrame()
{
    cv::Mat tempframe;
    FEITENG_CAMERA() >> tempframe;
    QImage qimage = QImage(tempframe.data,tempframe.cols,tempframe.rows,QImage::Format_RGB888).rgbSwapped();
    ui->face_label->setPixmap(QPixmap::fromImage(qimage));
}

void MainWindow::handleFaceRecognitionDone(int label, double confidence)
{
    m_person = nullptr;
    if(confidence < 0) {
        QMessageBox::warning(this, "签到提醒", "未检测到人脸，请重新签到!");
        return;
    }
    m_person = Feiteng::Person::ptr(new Feiteng::Person("王亚宁", "20203760", "微电子与通信工程学院"));
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
       
    } else {
        // 用户返回，执行相应的逻辑，例如重新采集图像
    }
}

void MainWindow::processFaceRecognition()
{
    // m_face->setLabel("20203760");
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
    Ttimer->setInterval(1000);
    YAML::Node node = YAML::LoadFile("../conf/serial.yml");
    Feiteng::Config::LoadFromYaml(node);
    FEITENG_LOG_INFO(g_logger) << "串口配置已加载";
    // 断开现有的连接
    QObject::disconnect(connection);
    QObject::disconnect(connection2);
    if(m_person == nullptr) {
        QMessageBox::warning(this, "签到提醒", "还未签到，没有相关人员信息!");
    } else {
        // 人员已经签到，开始测量体温
        count = 0;// 测温次数
        bodyTempVec.clear(); // 体温数据
        m_serial->Sopen();
        Feiteng::Temperature::ptr bodyTemp = m_person->getTemperature();
        bodyTemp->setSerial(std::shared_ptr<Feiteng::Serial>(m_serial));
        Feiteng::BodyTemp* bodyTempPtr = dynamic_cast<Feiteng::BodyTemp*>(bodyTemp.get());
        // 测量十次体温求平均值，并且将不符合标准的数据剔除直到够十次
        QByteArray startMeasuringCmd = QByteArray::fromHex("FACAC4");
        QByteArray bodyTempModeCmd = QByteArray::fromHex("FAC5BF");
        connection = QObject::connect(Ttimer, &QTimer::timeout, this, [startMeasuringCmd, bodyTempPtr, this]() {
            if (count < 10) {  // 检查是否已经测量了十次
                FEITENG_LOG_INFO(g_logger) << "发送开始测量命令";
                m_serial->Swrite(startMeasuringCmd);  // 发送开始测量命令
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
                TemperDialog dialog(QString("测量到的体温平均值为：%1").arg(average), this);  // 传递测量到的体温平均值到弹窗
                dialog.setWindowTitle("确认测量温度信息");  // 设置弹窗标题
                dialog.setFixedSize(300, 200);    // 设置弹窗为固定大小
                if (dialog.exec() == QDialog::Accepted) {
                    // 用户确认签到，执行相应的逻辑，例如更新数据库
                } else {

                }
            }
        });
        connection2 = QObject::connect(m_serial->getSerial(), &QSerialPort::readyRead, this, [bodyTempPtr,this]() {
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
