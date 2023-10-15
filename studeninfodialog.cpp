#include "studeninfodialog.h"
#include "ui_studeninfodialog.h"

StudenInfoDialog::StudenInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudenInfoDialog)
{
    ui->setupUi(this);
}

StudenInfoDialog::~StudenInfoDialog()
{
    delete ui;
}
