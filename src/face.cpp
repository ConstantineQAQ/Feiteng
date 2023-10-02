#include "face.h"

namespace Feiteng {

static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("face");

void Feiteng::Face::getRawFrame()
{
    m_camera->openCamera(); // 打开摄像头
    m_camera->getCamera() >> m_frame; // 从摄像头采集数据
    cv::imwrite("/home/kylin/Feiteng/frame.jpg", m_frame); // 测试用
    FEITENG_LOG_INFO(g_logger) << "采集数据成功";
    cv::flip(m_frame, m_frame, 1); // 水平翻转
    cv::cvtColor(m_frame, m_grayframe, cv::COLOR_BGR2GRAY); // 转换为灰度图
    cv::fastNlMeansDenoising(m_grayframe, m_grayframe, 3, 7, 21); // 去噪
    cv::equalizeHist(m_grayframe, m_grayframe); // 直方图均衡化
}


Face::Face()
{
    m_camera = std::make_shared<Camera>();
}

FaceInfo::FaceInfo(std::string label) : m_label(label) 
                                                  ,Face(){
    m_face_cascade = std::make_shared<cv::CascadeClassifier>();
    m_face_cascade->load("/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml"); // 加载人脸级联器
}

void FaceInfo::faceRecorde(int number)
{
    int count = 0;
    while (count < number) {
        detectFace(); // 检测人脸
        cv::resize(m_faceROI, m_faceROI, cv::Size(100, 100)); // 人脸图像大小归一化
        cv::imwrite("/home/kylin/Feiteng/face" + m_label + "_" + std::to_string(count) + ".jpg", m_faceROI); // 测试用
        count++;
    }
}
void FaceInfo::detectFace()
{
    getRawFrame(); // 获取原始图像
    m_face_cascade->detectMultiScale(m_grayframe, m_faces, 1.1, 3, 0, cv::Size(100, 100)); // 检测人脸
    if (m_faces.empty()) {
        FEITENG_LOG_ERROR(g_logger) << "未检测到人脸";
        return;
    }
    m_faceROI = m_grayframe(m_faces[0]); // 获取人脸
    cv::imwrite("/home/kylin/Feiteng/face.jpg", m_faceROI); // 测试用
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

void Camera::openCamera()
{
    if (m_cap.isOpened()) {
        FEITENG_LOG_INFO(g_logger) << "摄像头已打开";
        return;
    }
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        FEITENG_LOG_ERROR(g_logger) << "摄像头打开失败";
        return;
    } else {
        FEITENG_LOG_INFO(g_logger) << "摄像头打开成功";
        m_cap = cap;
    }
}

}
