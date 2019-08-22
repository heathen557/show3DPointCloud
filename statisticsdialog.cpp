#include "statisticsdialog.h"
#include "ui_statisticsdialog.h"
#include<QDebug>



statisticsDialog::statisticsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::statisticsDialog)
{
    ui->setupUi(this);

}

statisticsDialog::~statisticsDialog()
{
    delete ui;
}


//开始统计的槽函数
void statisticsDialog::on_start_pushButton_clicked()
{
    int frameNumber = ui->frame_lineEdit->text().toInt();
    emit alterStatisticFrameNum_signal(frameNumber);

}



void statisticsDialog::on_start_pushButton_2_clicked()
{
//    updateTimer.stop();
}

//接收统计均值和方差的槽函数
void statisticsDialog::statistic_MeanStdSlot(QString tofMean, QString tofStd, QString peakMean, QString peakStd)
{
    ui->tofMean_textEdit->setText(tofMean);
    ui->tofStd_textEdit->setText(tofStd);

    ui->peakMean_textEdit->setText(peakMean);
    ui->peakStd_textEdit->setText(peakStd);
}
