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

class Face {

public:
    typedef std::shared_ptr<Face> ptr;
    void detectFace(); // 检测人脸
    void resizeFace(); // 调整人脸大小
    cv::Mat getFace() const { return m_frame; }
    cv::Mat getGrayFace() const { return m_grayframe; }
    std::vector<cv::Rect> getFaces() const { return m_faces; }
    ~Face();
private:
    cv::Mat m_frame ,m_grayframe; // 人脸图像，从摄像头采集的数据
    std::shared_ptr<cv::VideoCapture> m_cap; // 摄像头
    std::shared_ptr<cv::CascadeClassifier> m_face_cascade; // 人脸检测器
    std::vector<cv::Rect> m_faces; // 人脸矩形框
};

class FaceInfo { 
public:
    typedef std::shared_ptr<FaceInfo> ptr;
private:
    Face::ptr m_face; // 人脸信息
    std::string label; // 标签
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