#include "openlocaldialog.h"
#include "ui_openlocaldialog.h"

openLocalDialog::openLocalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openLocalDialog)
{
    ui->setupUi(this);
}

openLocalDialog::~openLocalDialog()
{
    delete ui;
}
