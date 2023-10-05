#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include "face.h"
#include "config.h"
#include "log.h"
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void disPlayFrame();
    void on_pushButton_clicked();
    
private:
    void processFaceRecognition();
    Q_INVOKABLE void updateFaceResultLabel(int label);
    Ui::MainWindow *ui;
    Feiteng::FaceInfo::ptr m_face;
    QTimer *timer;
};
#endif // MAINWINDOW_H
