#include "filesave.h"
#include "ui_filesave.h"
#include<QFileDialog>
#include<QDebug>
#include<QMessageBox>

fileSave::fileSave(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fileSave)
{
    ui->setupUi(this);
    ui->radioButton->setChecked(true);    //默认设置为不保存
    ui->lineEdit->setEnabled(false);
    ui->SelFilePath_pushButton->setEnabled(false);

    ui->radioButton_3->setChecked(true);  //默认设置为保存pcd(二进制格式)
    formatSelect = 0 ;

}

fileSave::~fileSave()
{
    delete ui;
}

//选择文件路径
void fileSave::on_SelFilePath_pushButton_clicked()
{
    file_path = QFileDialog::getExistingDirectory(this,QStringLiteral("请选择文件保存路径..."),"./");
    if(file_path.isEmpty())
    {
       qDebug()<<QStringLiteral("没有选择路径")<<endl;
       QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("保存路径不能为空"));
        return;
    }
    else
    {
        file_path.append("/");
        qDebug() << file_path << endl;
        ui->lineEdit->setText(file_path);
    }
}

//确定槽函数
void fileSave::on_pushButton_clicked()
{
    //如果选中且路径不能空，则发送保存信号
    if(ui->radioButton_2->isChecked() && (!file_path.isEmpty()))
    {
        emit isSaveFlagSignal(true,file_path,formatSelect);
    }else  //否则发送不保存信号
    {
        emit isSaveFlagSignal(false,file_path,formatSelect);
    }


    this->hide();
}

//取消槽函数
void fileSave::on_pushButton_2_clicked()
{

}


//radio1 clicked
void fileSave::on_radioButton_clicked()
{
    if(ui->radioButton->isChecked())
    {
        ui->SelFilePath_pushButton->setEnabled(false);
        ui->lineEdit->setEnabled(false);

    }

}

//radio2 clicked
void fileSave::on_radioButton_2_clicked()
{
    if(ui->radioButton_2->isChecked())
    {
        ui->lineEdit->setEnabled(true);
        ui->SelFilePath_pushButton->setEnabled(true);
    }
}



void fileSave::on_radioButton_3_clicked()
{
    formatSelect = 0;
}

void fileSave::on_radioButton_4_clicked()
{
    formatSelect = 1;
}

void fileSave::on_radioButton_5_clicked()
{
    formatSelect = 2;
}
