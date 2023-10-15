#ifndef STUDENINFODIALOG_H
#define STUDENINFODIALOG_H

#include <QDialog>

namespace Ui {
class StudenInfoDialog;
}

class StudenInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StudenInfoDialog(QWidget *parent = nullptr);
    ~StudenInfoDialog();

private:
    Ui::StudenInfoDialog *ui;
};

#endif // STUDENINFODIALOG_H
