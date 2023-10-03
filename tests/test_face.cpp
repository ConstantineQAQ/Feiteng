#include "../include/face.h"
#include "../include/config.h"
#include "../include/log.h"
#include <time.h>
#include <iostream>

void test_face() {
    std::cout << "请输入人脸标签：";
    std::string label;
    std::cin >> label;
    Feiteng::FaceInfo *face = new Feiteng::FaceInfo(label);
    face->faceRecorde();
}

void test_predict() {
    FEITENG_CAMERA_OPEN();
    Feiteng::FaceRecognizer *recognizer = new Feiteng::FaceRecognizer();
    recognizer->getRecognizer()->read("../face_recognizer.xml");
    Feiteng::FaceInfo *face = new Feiteng::FaceInfo();
    face->faceRecorde();
    cv::Mat test_face = face->getFaceROI();
    recognizer->predict(test_face);
}

void config_face(){
    Feiteng::ConfigVar<Feiteng::FaceConfig>::ptr face_config =
        Feiteng::Config::Lookup<Feiteng::FaceConfig>(
            "face_config" 
            , Feiteng::FaceConfig()
            , "Face config");
    Feiteng::FaceInfo *face = new Feiteng::FaceInfo();
    face_config->addListener([face](const Feiteng::FaceConfig& old_value, const Feiteng::FaceConfig& new_value) {
        face->setConfig(std::shared_ptr<Feiteng::FaceConfig>(
            new Feiteng::FaceConfig(new_value)
        ));
    });
    face->setConfig(std::shared_ptr<Feiteng::FaceConfig>(new Feiteng::FaceConfig(face_config->getValue())));
    #define XX(m) \
        FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << #m << " = " << face->getConfig()->toString();
    XX(before);
    YAML::Node root = YAML::LoadFile("../conf/face.yml");
    Feiteng::Config::LoadFromYaml(root);
    XX(after);
}

void test_train() {
    Feiteng::ConfigVar<Feiteng::FaceConfig>::ptr face_config =
        Feiteng::Config::Lookup<Feiteng::FaceConfig>(
            "face_config" 
            , Feiteng::FaceConfig()
            , "Face config");
    FEITENG_CAMERA_OPEN();
    Feiteng::FaceRecognizer *recognizer = new Feiteng::FaceRecognizer(); // 人脸识别器
    std::cout << "请输入人脸标签：";
    std::string label;
    std::cin >> label;
    Feiteng::FaceInfo *face = new Feiteng::FaceInfo(label);
    face->setConfig(std::shared_ptr<Feiteng::FaceConfig>(new Feiteng::FaceConfig(face_config->getValue())));
    face->faceRecorde();
    recognizer->addFaceInfo(std::shared_ptr<Feiteng::FaceInfo>(face));
    FEITENG_CAMERA_CLOSE();
    FEITENG_CAMERA_OPEN();
    std::cout << "请输入第二个人的人脸标签：";
    std::string label2;
    std::cin >> label2;
    Feiteng::FaceInfo *face2 = new Feiteng::FaceInfo(label2);
    face2->faceRecorde();
    recognizer->addFaceInfo(std::shared_ptr<Feiteng::FaceInfo>(face2));
    recognizer->train();
    FEITENG_CAMERA_CLOSE();
}

int main() {
    config_face();
    return 0;
}
