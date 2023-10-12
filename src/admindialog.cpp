#include "admindialog.h"

AdminDialog::AdminDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminDialog)
{
    ui->setupUi(this);
}

AdminDialog::~AdminDialog()
{
    FEITENG_CAMERA_OPEN();
    delete ui;
}
