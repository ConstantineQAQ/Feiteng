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
#include "database.h"
#include "admindialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class FrameProcessor;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void faceRecognitionDone(int label, double confidence);
private slots:
    void updateFaceLabel(const QImage &image);
    void on_work_pushButton_clicked();
    void handleFaceRecognitionDone(int label, double confidence);
    void on_temper_pushButton_clicked();
    void on_admin_pushButton_clicked();

private:
    void processFaceRecognition();
    void processTemperature();
    // Q_INVOKABLE void updateFaceResultLabel(int label);
    int count; // 测温次数
    std::vector<double> bodyTempVec; // 体温数据
    Ui::MainWindow *ui;
    Feiteng::FaceInfo::ptr m_face; // 人脸信息
    Feiteng::Person::ptr m_person; // 人员信息
    QTimer *Ttimer; // 温度定时器
    QTimer *Ftimer; // 人脸定时器
    QThread *m_thread; // 线程
    FrameProcessor *m_frameProcessor; // 帧处理器
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

class FrameProcessor : public QObject
{
    Q_OBJECT

public:
    explicit FrameProcessor(QObject *parent = nullptr) : QObject(parent) {}
    void setFaceInfo(const Feiteng::FaceInfo::ptr& faceInfo) { m_face = faceInfo; }

signals:
    void frameProcessed(const QImage &image);

public slots:
    void processFrame();

private:
    Feiteng::FaceInfo::ptr m_face;
};


#endif // MAINWINDOW_H
