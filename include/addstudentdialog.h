#ifndef ADDSTUDENTDIALOG_H
#define ADDSTUDENTDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QThread>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/QtConcurrentRun>

#include "face.h"
#include "log.h"
#include "person.h"
#include "database.h"


#include "ui_addstudentdialog.h"


namespace Ui {
class AddStudentDialog;
}

class AddStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddStudentDialog(QWidget *parent = nullptr);
    ~AddStudentDialog();
    
protected:
    void closeEvent(QCloseEvent *event) override;
    
signals:
    void faceInputCompleted();  

private slots:
    void on_add_pushButton_clicked();
    void on_saveButton_clicked();
    void displayFrame();

private:
    void FaceInput();
    QTimer *Ftimer; // 人脸定时器
    Feiteng::Person::ptr m_person;
    Ui::AddStudentDialog *ui;
};

#endif // ADDSTUDENTDIALOG_H
