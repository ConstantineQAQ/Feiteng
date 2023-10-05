#include "mainwindow.h"

static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("mainwindow");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
{
    ui->setupUi(this);
    g_logger->addAppender(Feiteng::LogAppender::ptr(new Feiteng::FileLogAppender("../log/face.log"))); // 添加日志输出器
    FEITENG_CAMERA_OPEN(); // 打开摄像头
    Feiteng::ConfigVar<Feiteng::FaceConfig>::ptr face_config =
        Feiteng::Config::Lookup<Feiteng::FaceConfig>(
            "face_config" 
            , Feiteng::FaceConfig()
            , "Face config"); // 人脸配置
    m_face = Feiteng::FaceInfo::ptr(new Feiteng::FaceInfo()); // 人脸信息
    m_face->setConfig(std::shared_ptr<Feiteng::FaceConfig>(new Feiteng::FaceConfig(face_config->getValue()))); // 设置人脸配置
    face_config->addListener([this](const Feiteng::FaceConfig& old_value, const Feiteng::FaceConfig& new_value) {
        m_face->setConfig(std::shared_ptr<Feiteng::FaceConfig>(
            new Feiteng::FaceConfig(new_value)
        ));
        FEITENG_LOG_INFO(g_logger) << "人脸配置已更新";
    }); // 使用回调函数监听人脸配置
    YAML::Node root = YAML::LoadFile("../conf/face.yml");
    Feiteng::Config::LoadFromYaml(root); // 从配置文件加载人脸配置
    connect(timer, SIGNAL(timeout()), this, SLOT(disPlayFrame()));
    timer->start(30);
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

void MainWindow::on_pushButton_clicked()
{
    FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "测试用";
    QtConcurrent::run(this, &MainWindow::processFaceRecognition);
}

void MainWindow::processFaceRecognition()
{
    FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "测试用";
    m_face->setLabel("20203760");
    m_face->faceRecorde();
    Feiteng::FaceRecognizer::ptr face_recognizer = Feiteng::FaceRecognizer::ptr(new Feiteng::FaceRecognizer());
    face_recognizer->addFaceInfo(m_face);
    face_recognizer->train();
    cv::Mat test_face = m_face->getFaceROI();
    Feiteng::FacePredict::ptr face_predict = Feiteng::FacePredict::ptr(new Feiteng::FacePredict(face_recognizer));
    face_predict->predict(test_face);
    
    // 在主线程中更新 UI
    QMetaObject::invokeMethod(this, "updateFaceResultLabel", Q_ARG(int, face_predict->getLabel()));
}

void MainWindow::updateFaceResultLabel(int label)
{
    ui->faceResult_label->setText(QString::fromStdString(std::to_string(label)));
}
