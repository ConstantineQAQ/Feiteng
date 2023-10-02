#include "../include/face.h"
#include <iostream>

int main() {
    std::cout << "请输入人脸标签：";
    std::string label;
    std::cin >> label;
    Feiteng::FaceInfo *face = new Feiteng::FaceInfo(label);
    face->faceRecorde();
    return 0;
}
