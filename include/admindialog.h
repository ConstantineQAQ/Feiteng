#ifndef ADMINDIALOG_H
#define ADMINDIALOG_H

#include <QDialog>

#include "face.h"
#include "ui_admindialog.h"

namespace Ui {
class AdminDialog;
}

class AdminDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdminDialog(QWidget *parent = nullptr);
    ~AdminDialog();

private:
    Ui::AdminDialog *ui;
};

#endif // ADMINDIALOG_H
