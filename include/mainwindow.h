#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtConcurrent/QtConcurrent>

#include "face.h"
#include "config.h"
#include "log.h"
#include "person.h"
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
signals:
    void faceRecognitionDone(int label, double confidence);
public slots:
    void disPlayFrame();
private slots:
    void on_work_pushButton_clicked();
    void handleFaceRecognitionDone(int label, double confidence);
    void on_temper_pushButton_clicked();

private:
    void processFaceRecognition();
    // Q_INVOKABLE void updateFaceResultLabel(int label);
    int count; // 测温次数
    std::vector<double> bodyTempVec; // 体温数据
    Ui::MainWindow *ui;
    Feiteng::FaceInfo::ptr m_face; // 人脸信息
    Feiteng::Person::ptr m_person; // 人员信息
    Feiteng::Serial::ptr m_serial; // 串口
    Feiteng::ConfigVar<Feiteng::SerialConfig>::ptr m_serial_config; // 串口配置
    Feiteng::ConfigVar<Feiteng::FaceConfig>::ptr m_face_config; // 人脸配置
    Feiteng::ConfigVar<Feiteng::TemperatureConfig>::ptr m_temperature_config; // 体温配置
    QTimer *Ttimer; // 温度定时器
    QTimer *Ftimer; // 人脸定时器
};

// 人脸识别确认弹窗
class ConfirmDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfirmDialog(const QString &info, QWidget *parent = nullptr)
        : QDialog(parent),
          infoLabel(new QLabel(info, this)),
          confirmButton(new QPushButton("确认", this)),
          cancelButton(new QPushButton("返回", this))
    {
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(infoLabel);
        layout->addWidget(confirmButton);
        layout->addWidget(cancelButton);
        setLayout(layout);

        connect(confirmButton, &QPushButton::clicked, this, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    }

private:
    QLabel *infoLabel;
    QPushButton *confirmButton;
    QPushButton *cancelButton;
};

// 体温测量确认弹窗
class TemperDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TemperDialog(const QString &info, QWidget *parent = nullptr)
        : QDialog(parent),
          infoLabel(new QLabel(info, this)),
          confirmButton(new QPushButton("确认", this)),
          cancelButton(new QPushButton("返回", this))
    {
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(infoLabel);
        layout->addWidget(confirmButton);
        layout->addWidget(cancelButton);
        setLayout(layout);

        connect(confirmButton, &QPushButton::clicked, this, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    }

private:
    QLabel *infoLabel;
    QPushButton *confirmButton;
    QPushButton *cancelButton;
};
#endif // MAINWINDOW_H
