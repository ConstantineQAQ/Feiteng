#include "mainwindow.h"
#include "face.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Feiteng::FaceRecognizer* facerecognizer = new Feiteng::FaceRecognizer();
    facerecognizer->train();
    w.show();
    return a.exec();
}
