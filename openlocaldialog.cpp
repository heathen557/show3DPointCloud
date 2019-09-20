#include "openlocaldialog.h"
#include "ui_openlocaldialog.h"

openLocalDialog::openLocalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openLocalDialog)
{
    ui->setupUi(this);
//    ui->filePath_lineEdit->setEnabled(false);
    ui->filePath_lineEdit->setReadOnly(true);

}

openLocalDialog::~openLocalDialog()
{
    delete ui;
}

//路径选择
void openLocalDialog::on_select_pushButton_clicked()
{
    filePath = QFileDialog::getExistingDirectory(this,QStringLiteral("选择文件夹路径"),"/");
    if(filePath.isEmpty())
    {
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("路径无效！"));
        return;
    }
    filePath.append("/");
    ui->filePath_lineEdit->setText(filePath);

}

//确定
void openLocalDialog::on_ok_pushButton_clicked()
{
    if(!filePath.isEmpty())
        emit selectLocalFile_signal(filePath);

    this->hide();
}

//取消
void openLocalDialog::on_cancel_pushButton_clicked()
{
    this->hide();
}

