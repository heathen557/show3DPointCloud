#include "mainwindow.h"
QMutex mutex;
QImage tofImage;
QImage intensityImage;
extern bool  isShowPointCloud;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //把读取USB信息放到线程当中，并开启线程
    recvUsbMsg_obj = new ReceUSB_Msg();
    recvUsbThread = new QThread;
    recvUsbMsg_obj->moveToThread(recvUsbThread);
    recvUsbThread->start();
    connect(this,SIGNAL(readSignal()),recvUsbMsg_obj, SLOT(run()));
    connect(recvUsbMsg_obj,SIGNAL(linkInfoSignal(int)),this,SLOT(linkInfoSlot(int)));
    connect(&showTimer,SIGNAL(timeout()),this,SLOT(showImageSlot()));
}


//测试按钮
void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_pushButton_2_clicked()
{
    showTimer.start(50);
//    recvUsbThread->start();

    emit readSignal();
}

void MainWindow::showImageSlot()
{
    if(!isShowPointCloud)
        return;

    QImage resImage;
    QImage resIntenImage;
    if(!tofImage.isNull() && !intensityImage.isNull())
    {
        mutex.lock();
        resImage = tofImage.scaled(tofImage.width()*3, tofImage.height()*5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        resIntenImage = intensityImage.scaled(intensityImage.width()*3, intensityImage.height()*5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        mutex.unlock();

        QPixmap pixmap2(QPixmap::fromImage (resImage));
        ui->showTOF_label->setPixmap(pixmap2);

        QPixmap pixmap3(QPixmap::fromImage (resIntenImage));
        ui->showIntensity_label->setPixmap(pixmap3);

    }
}


//接收线程发过来的错误信息；
/*  1、没有查找到设备
 *
 */
void MainWindow::linkInfoSlot(int flagNum)
{
    if(1 == flagNum)
    {
        QMessageBox::information(NULL,"warn",QString::fromUtf8("NO Device"));
    }else if(2 == flagNum)
    {
        QMessageBox::information(NULL,"warn",QString::fromUtf8("RECEIVE NO DATA"));
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}
