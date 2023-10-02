#ifndef FACE_H
#define FACE_H

#include "file.h"
#include <vector>
#include <string>
#include <memory>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/objdetect/objdetect.hpp>
#include <opencv4/opencv2/face.hpp>
#include <opencv4/opencv2/face/facerec.hpp>
#include <opencv4/opencv2/core/types.hpp>


namespace Feiteng {

// 摄像头对象
class Camera {
public:
    typedef std::shared_ptr<Camera> ptr;
    void openCamera(); // 打开摄像头
    cv::VideoCapture getCamera() const { return m_cap; } 
private:
    cv::VideoCapture m_cap; // 摄像头
};

// 图像原始数据
class Face {
public:
    typedef std::shared_ptr<Face> ptr;
    void getRawFrame(); // 检测人脸
    cv::Mat getFace() const { return m_frame; }
    cv::Mat getGrayFace() const { return m_grayframe; }
    virtual ~Face() {}
    virtual void detectFace() {}
    Face(); 
protected:
    cv::Mat m_frame ,m_grayframe; // 图像，从摄像头采集的数据
    Camera::ptr m_camera; // 摄像头
};


// 人脸详细信息
class FaceInfo : public Face{ 
public:
    typedef std::shared_ptr<FaceInfo> ptr;
    void faceRecorde(int number = 10); // 人脸录入
    virtual void detectFace(); // 人脸检测
    FaceInfo(std::string label = "unknown");
    virtual ~FaceInfo() {}
private:
    std::string m_label; // 标签
    std::vector<cv::Rect> m_faces; // 人脸矩形框
    cv::Mat m_faceROI; // 人脸
    std::shared_ptr<cv::CascadeClassifier> m_face_cascade; // 人脸级联器
};

// 人脸识别器
class FaceRecognizer {
public:
    typedef std::shared_ptr<FaceRecognizer> ptr;
    void setTxtFile(txtFile::ptr txtfile) { m_txtfile = txtfile; }
    void train(); // 训练

private:
    Face::ptr m_face; // 人脸信息
    cv::Ptr<cv::face::LBPHFaceRecognizer> m_recognizer; // 人脸识别器
    std::vector<cv::Mat> m_images; // 人脸图像
    std::vector<int> m_labels; // 人脸标签
    txtFile::ptr m_txtfile; // txt文件
};

}

#endif // FACE_H