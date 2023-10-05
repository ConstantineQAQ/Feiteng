#ifndef FACE_H
#define FACE_H

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

#include "singleton.h"
#include "log.h"


#define FEITENG_CAMERA_OPEN() Feiteng::CameraMgr::GetInstance()->openCamera()
#define FEITENG_CAMERA() Feiteng::CameraMgr::GetInstance()->getCamera()
#define FEITENG_CAMERA_CLOSE() Feiteng::CameraMgr::GetInstance()->closeCamera()
#define FEITENG_CAMERA_CLEAR_FRAME() Feiteng::CameraMgr::GetInstance()->clearFrame()

namespace Feiteng {

// 摄像头对象
class Camera {
public:
    typedef std::shared_ptr<Camera> ptr;
    void openCamera(); // 打开摄像头
    cv::VideoCapture getCamera() const { return m_cap; } 
    void closeCamera(); // 关闭摄像头
    void clearFrame() { m_cap.grab(); } // 清除帧缓存
    ~Camera();
private:
    cv::VideoCapture m_cap; // 摄像头
};

typedef Singleton<Camera> CameraMgr;

// 人脸参数配置类
class FaceConfig {
public:
    typedef std::shared_ptr<FaceConfig> ptr;
    FaceConfig(double confidence = 0.8, double SSIM = 0.8, int faceSum = 10
                , int faceSize = 100, double scaleFactor = 1.1, int minNeighbors = 3
                , std::string facePath = "./face/"): m_confidence(confidence)
                , m_SSIM(SSIM), m_faceSum(faceSum), m_faceSize(faceSize)
                , m_scaleFactor(scaleFactor), m_minNeighbors(minNeighbors)
                , m_facePath(facePath) {}
    double getConfidence() const { return m_confidence; }
    double getSSIM() const { return m_SSIM; }
    int getFaceSum() const { return m_faceSum; }
    int getFaceSize() const { return m_faceSize; }
    double getScaleFactor() const { return m_scaleFactor; }
    int getMinNeighbors() const { return m_minNeighbors; }
    std::string getFacePath() const { return m_facePath; }
    void setConfidence(double confidence) { m_confidence = confidence; }
    void setSSIM(double SSIM) { m_SSIM = SSIM; }
    void setFaceSum(int faceSum) { m_faceSum = faceSum; }
    void setFaceSize(int faceSize) { m_faceSize = faceSize; }
    void setScaleFactor(double scaleFactor) { m_scaleFactor = scaleFactor; }
    void setMinNeighbors(int minNeighbors) { m_minNeighbors = minNeighbors; }
    void setFacePath(std::string facePath) { m_facePath = facePath; }
    std::string toString() const; // 将人脸参数配置转换为字符串
    std::string toYamlString(); // 将人脸参数配置转换为YAML字符串
    bool operator==(const FaceConfig& rhs) const {
        return m_confidence == rhs.m_confidence
            && m_SSIM == rhs.m_SSIM
            && m_faceSum == rhs.m_faceSum
            && m_faceSize == rhs.m_faceSize
            && m_scaleFactor == rhs.m_scaleFactor
            && m_minNeighbors == rhs.m_minNeighbors
            && m_facePath == rhs.m_facePath;
    }

private:
    double m_confidence; // 置信度
    double m_SSIM; // 相似度
    int m_faceSum; // 录入人脸总数
    int m_faceSize; // 人脸大小
    double m_scaleFactor; // 缩放因子
    int m_minNeighbors; // 最小邻居数
    std::string m_facePath; // 人脸照片存储路径
};


// 图像原始数据
class Face {
public:
    typedef std::shared_ptr<Face> ptr;
    cv::Mat getFace() const { return m_frame; }
    cv::Mat getGrayFace() const { return m_grayframe; }
    void setConfig(FaceConfig::ptr config) { m_config = config; }
    FaceConfig::ptr getConfig() const { return m_config; }
    Face();
    virtual ~Face() {}
protected:
    void getRawFrame(); // 检测人脸
    cv::Mat m_frame ,m_grayframe; // 图像，从摄像头采集的数据
    FaceConfig::ptr m_config; // 人脸参数配置
};


// 人脸详细信息
class FaceInfo : public Face{ 
public:
    typedef std::shared_ptr<FaceInfo> ptr;
    void faceRecorde(); // 人脸录入
    FaceInfo(std::string label = "unknown");
    void setLabel(std::string label) { m_label = label; }
    std::string getLabel() const { return m_label; }
    std::vector<cv::Rect> getFaces() const { return m_faces; } 
    std::vector<cv::Mat> getFaceROIs() const { return m_faceROIs; }
    cv::Mat getFaceROI() const { return m_faceROI; } 

    virtual ~FaceInfo() {}
private:
    double getSSIM(cv::Mat img1, cv::Mat img2); // 计算两张图片的相似度
    void detectFace(); // 人脸检测
    std::string m_label; // 标签
    std::vector<cv::Rect> m_faces; // 人脸矩形框
    cv::Mat m_faceROI; // 一张人脸照片
    std::vector<cv::Mat> m_faceROIs; // 所有合法录入的人脸照片（用来判断新输入的照片与前一张照片的相似度）
    std::shared_ptr<cv::CascadeClassifier> m_face_cascade; // 人脸级联器
};

// 人脸训练器
class FaceRecognizer {
friend class FacePredict;
public:
    typedef std::shared_ptr<FaceRecognizer> ptr;
    FaceRecognizer();
    void addFaceInfo(FaceInfo::ptr faceinfo) { m_faceinfos.push_back(std::move(faceinfo)); }
    cv::Ptr<cv::face::LBPHFaceRecognizer> getRecognizer() const { return m_recognizer; }
    void train(); // 训练
private:
    cv::Ptr<cv::face::LBPHFaceRecognizer> m_recognizer; // 人脸识别器
    std::vector<FaceInfo::ptr> m_faceinfos; // 人脸信息
};

// 人脸预测类
class FacePredict {
public:
    typedef std::shared_ptr<FacePredict> ptr;
    FacePredict(FaceRecognizer::ptr recognizer): m_recognizer(recognizer) {}
    void predict(cv::Mat face_test); // 预测
    int getLabel() const { return m_label; }
private:
    int m_label; // 标签
    double m_confidence; // 置信度
    FaceRecognizer::ptr m_recognizer; // FaceRecognizer 实例的引用
};

}

#endif // FACE_H
