#ifndef STUDENINFODIALOG_H
#define STUDENINFODIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QTableWidgetItem>
#include <QTableWidget>

#include "database.h"
#include "log.h"
#include "addstudentdialog.h"

#include "ui_studeninfodialog.h"
namespace Ui {
class StudenInfoDialog;
}

class StudenInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StudenInfoDialog(QWidget *parent = nullptr);
    ~StudenInfoDialog();
    void queryAndDisplay();  // 用于查询和显示结果

private slots:
    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void on_add_PushButton_clicked();

    void on_train_pushButton_clicked();

    void on_delete_PushButton_clicked();

    void on_yaml_pushButton_clicked();

private:
    Ui::StudenInfoDialog *ui;
};

#endif // STUDENINFODIALOG_H
