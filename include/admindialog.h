#ifndef ADMINDIALOG_H
#define ADMINDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QString>

#include "face.h"
#include "log.h"
#include "ui_admindialog.h"
#include "studeninfodialog.h"

namespace Ui {
class AdminDialog;
}

class AdminDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdminDialog(QWidget *parent = nullptr);
    ~AdminDialog();

signals:
    void backButtonClicked();
    
private slots:
    void on_commit_pushButton_clicked();

    void on_back_pushButton_clicked();

private:
    Ui::AdminDialog *ui;
};

#endif // ADMINDIALOG_H
