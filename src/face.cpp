#include "face.h"

static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("face");
Feiteng::FaceConfig::ptr g_face_config = std::make_shared<Feiteng::FaceConfig>();

namespace Feiteng {

template<>
class LexicalCast<Feiteng::FaceConfig, std::string> {
public:
    std::string operator()(const FaceConfig& v) {
        YAML::Node node;
        node["confidence"] = v.getConfidence();
        node["ssim"] = v.getSSIM();
        node["face_sum"] = v.getFaceSum();
        node["face_size"] = v.getFaceSize();
        node["scale_factor"] = v.getScaleFactor();
        node["min_neighbors"] = v.getMinNeighbors();
        node["face_path"] = v.getFacePath();
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<>
class LexicalCast<std::string, Feiteng::FaceConfig> {
public:
    FaceConfig operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        FaceConfig config;
        config.setConfidence(node["confidence"].as<double>());
        config.setSSIM(node["ssim"].as<double>());
        config.setFaceSum(node["face_sum"].as<int>());
        config.setFaceSize(node["face_size"].as<int>());
        config.setScaleFactor(node["scale_factor"].as<double>());
        config.setMinNeighbors(node["min_neighbors"].as<int>());
        config.setFacePath(node["face_path"].as<std::string>());
        return config;
    }
};

Feiteng::ConfigVar<FaceConfig>::ptr g_face_define =
    Feiteng::Config::Lookup("face", FaceConfig(), "face config");

struct FaceIniter {
    FaceIniter() {
        g_face_define->addListener([](const FaceConfig& old_value, const FaceConfig& new_value){
            FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "face config changed";
            g_face_config = std::make_shared<FaceConfig>(new_value);
        });
    }
};

static FaceIniter __face_initer;

Face::Face()
{
    m_config.reset(new FaceConfig());
}

void Feiteng::Face::getRawFrame()
{
    FEITENG_CAMERA() >> m_frame; // 采集数据
    cv::flip(m_frame, m_frame, 1); // 水平翻转
    cv::cvtColor(m_frame, m_grayframe, cv::COLOR_BGR2GRAY); // 转换为灰度图
    cv::fastNlMeansDenoising(m_grayframe, m_grayframe, 3, 7, 21); // 去噪
    cv::equalizeHist(m_grayframe, m_grayframe); // 直方图均衡化
}


FaceInfo::FaceInfo(std::string label) : m_label(label), Face() {
    m_face_cascade = std::make_shared<cv::CascadeClassifier>();
    m_face_cascade->load("/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml"); // 加载人脸级联器
}

double FaceInfo::getSSIM(cv::Mat img1, cv::Mat img2)
{
	// 常数，用于确保分母不为零
	const double C1 = 6.5025, C2 = 58.5225;

	// 将图像转换为浮点型
	cv::Mat I1, I2;
	img1.convertTo(I1, CV_32F);
	img2.convertTo(I2, CV_32F);

	// 计算图像的平方
	cv::Mat I2_2 = I2.mul(I2);
	cv::Mat I1_2 = I1.mul(I1);
	cv::Mat I1_I2 = I1.mul(I2);

	// 计算均值
	cv::Mat mu1, mu2;
	cv::GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
	cv::GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);

	// 计算均值的平方
	cv::Mat mu1_2 = mu1.mul(mu1);
	cv::Mat mu2_2 = mu2.mul(mu2);
	cv::Mat mu1_mu2 = mu1.mul(mu2);

	// 计算方差
	cv::Mat sigma1_2, sigma2_2, sigma12;
	cv::GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;
	cv::GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
	sigma2_2 -= mu2_2;
	cv::GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
	sigma12 -= mu1_mu2;

	// 计算SSIM的分子和分母
	cv::Mat t1, t2, t3;
	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigma12 + C2;
	t3 = t1.mul(t2);

	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigma2_2 + C2;
	t1 = t1.mul(t2);

	// 计算SSIM
	cv::Mat ssim_map;
	cv::divide(t3, t1, ssim_map);

	// 计算SSIM的均值
	cv::Scalar mssim = cv::mean(ssim_map);

	// 返回SSIM值
	return mssim.val[0];
}

void FaceInfo::faceRecorde()
{
    int number = m_config->getFaceSum();
    int count = 0;
    while (count < number) {
        try
        {
            detectFace(); // 检测人脸
            cv::resize(m_faceROI, m_faceROI, cv::Size(100, 100)); // 人脸图像大小归一化
            if (m_faceROIs.empty()) {
                m_faceROIs.push_back(m_faceROI);
                FEITENG_LOG_INFO(g_logger) << "录入成功";
                count++;
                cv::imwrite("/home/kylin/Feiteng/face" + m_label + "_" + std::to_string(count) + ".jpg", m_faceROI); // 测试用
            } else {
                // 判断新输入的照片与之前照片的相似度
                for(int a = 0; a < m_faceROIs.size(); a++) {
                    if (getSSIM(m_faceROI, m_faceROIs[a]) > m_config->getSSIM()) {
                        FEITENG_LOG_ERROR(g_logger) << "人脸相似度过高";
                        break;
                    }
                    if(a == m_faceROIs.size() - 1) {
                        m_faceROIs.push_back(m_faceROI);
                        FEITENG_LOG_INFO(g_logger) << "录入成功";
                        count++;
                        cv::imwrite("/home/kylin/Feiteng/face" + m_label + "_" + std::to_string(count) + ".jpg", m_faceROI); // 测试用
                    }
                }
            }
        }
        catch(const std::exception& e)
        {
            FEITENG_LOG_ERROR(g_logger) << e.what();
        }
        
        
    }
}

void FaceInfo::detectFace()
{
    getRawFrame(); // 获取原始图像
    m_face_cascade->detectMultiScale(m_grayframe, m_faces, m_config->getScaleFactor()
                                    , m_config->getMinNeighbors(), 0, cv::Size(m_config->getFaceSize(), m_config->getFaceSize())); // 检测人脸
    if (m_faces.empty()) {
        FEITENG_LOG_ERROR(g_logger) << "未检测到人脸";
        m_faceROI = cv::Mat();
        return;
    }
    m_faceROI = m_grayframe(m_faces[0]); // 获取人脸
}

FaceRecognizer::FaceRecognizer()
{
    m_recognizer = cv::face::LBPHFaceRecognizer::create();
}

void FaceRecognizer::train()
{   
    std::vector<cv::Mat> images;
    std::vector<int> labels;
    if(m_faceinfos.empty()) {
        FEITENG_LOG_ERROR(g_logger) << "未录入人脸";
        return;
    }
    for(const auto& faceinfo : m_faceinfos) {
        for(int i = 0; i < faceinfo->getFaceROIs().size(); i++) {
            images.push_back(faceinfo->getFaceROIs()[i]);
            labels.push_back(std::stoi(faceinfo->getLabel()));
        }
    }
    m_recognizer->train(images, labels);
    m_recognizer->save("/home/kylin/Feiteng/face_recognizer.xml");
    FEITENG_LOG_INFO(g_logger) << "训练成功";
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

void Camera::closeCamera()
{
    if (m_cap.isOpened()) {
        m_cap.release();
        FEITENG_LOG_INFO(g_logger) << "摄像头已关闭";
    } else {
        FEITENG_LOG_INFO(g_logger) << "摄像头已关闭";
    }
}

Camera::~Camera()
{
    if (m_cap.isOpened()) {
        m_cap.release();
        FEITENG_LOG_INFO(g_logger) << "摄像头已关闭";
    } 
}
std::string FaceConfig::toString() const
{
    std::stringstream ss;
    ss << "[FaceConfig faceSum=" << m_faceSum
        << " faceSize=" << m_faceSize
        << " confidence=" << m_confidence
        << " SSIM=" << m_SSIM
        << " scaleFactor=" << m_scaleFactor
        << " minNeighbors=" << m_minNeighbors
        << " facePath=" << m_facePath
        << "]";
    return ss.str();
}

void FacePredict::predict(cv::Mat face_test)
{
    if(!face_test.empty()) {
        m_recognizer->m_recognizer->predict(face_test, m_label, m_confidence);
        FEITENG_LOG_INFO(g_logger) << "label: " << m_label << " confidence: " << m_confidence;
    } else {
        m_confidence = -1;
    }
}
}
