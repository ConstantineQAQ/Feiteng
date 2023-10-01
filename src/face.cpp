#include "face.h"


namespace Feiteng {

static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("face");

void Feiteng::Face::detectFace()
{
    *m_cap >> m_frame;    // 获取当前帧并将其转换为灰度图
    cv::flip(m_frame, m_frame, 1); // 水平翻转
    cv::cvtColor(m_frame, m_grayframe, cv::COLOR_BGR2GRAY); // 转换为灰度图
    cv::fastNlMeansDenoising(m_grayframe, m_grayframe, 3, 7, 21); // 去噪
    cv::equalizeHist(m_grayframe, m_grayframe); // 直方图均衡化
    m_face_cascade->detectMultiScale(m_grayframe, m_faces, 1.1, 3, 0, cv::Size(100, 100)); // 检测人脸
}


void Feiteng::Face::resizeFace()
{
    
}

void FaceRecognizer::train()
{   
    m_txtfile = std::make_shared<txtFile>();
    m_recognizer = cv::face::LBPHFaceRecognizer::create(); // 创建人脸识别器
    m_txtfile->read("/home/kylin/Feiteng/face.txt"); // 读取txt文件
    if (!m_txtfile->pathEmpty() && !m_txtfile->labelEmpty()) {
        m_images.push_back(cv::imread(m_txtfile->getPath(), cv::IMREAD_GRAYSCALE)); // 读取人脸图像
        m_labels.push_back(std::stoi(m_txtfile->getLabel())); // 读取人脸标签
    }
    if (m_images.empty() || m_labels.empty()) {
        FEITENG_LOG_ERROR(g_logger) << "读取人脸图像或标签失败";
        return;
    }
    m_recognizer->train(m_images, m_labels); // 训练
    m_recognizer->save("face.xml"); // 保存训练结果
    FEITENG_LOG_INFO(g_logger) << "训练完成";
}
}
