#include "mainwindow.h"
#include <QScrollBar>
QMutex mutex;
QImage tofImage;
QImage intensityImage;
bool isWriteSuccess;    //写入命令是否成功标识
extern bool  isShowPointCloud;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    isWriteSuccess = false;

    //把读取USB信息放到线程当中，并开启线程
    recvUsbMsg_obj = new ReceUSB_Msg();
    recvUsbThread = new QThread;
    recvUsbMsg_obj->moveToThread(recvUsbThread);
    recvUsbThread->start();
    connect(this,SIGNAL(readSignal()),recvUsbMsg_obj, SLOT(run()));
    connect(this,SIGNAL(closeLinkSignal()),recvUsbMsg_obj,SLOT(closeUSB()));
    connect(recvUsbMsg_obj,SIGNAL(linkInfoSignal(int)),this,SLOT(linkInfoSlot(int)));

    connect(this,SIGNAL(readSysSignal()),recvUsbMsg_obj,SLOT(readSysSlot()));
    connect(this,SIGNAL(writeSysSignal()),recvUsbMsg_obj,SLOT(writeSysSlot()));
    connect(this,SIGNAL(readDevSignal()),recvUsbMsg_obj,SLOT(readDevSlot()));
    connect(this,SIGNAL(writeDevSignal()),recvUsbMsg_obj,SLOT(writeDevSlot()));
    connect(this,SIGNAL(loadSettingSignal()),recvUsbMsg_obj,SLOT(loadSettingSlot()));
    connect(this,SIGNAL(saveSettingSignal()),recvUsbMsg_obj,SLOT(saveSettingSlot()));

    connect(&showTimer,SIGNAL(timeout()),this,SLOT(showImageSlot()));

    initGUI();
}

void MainWindow::initGUI()
{

    //**********************************************************

    ui->tableWidget_2->setColumnWidth(0,117);
    ui->tableWidget_2->setColumnWidth(1,117);
    ui->tableWidget_2->setRowHeight(0,35);
    ui->tableWidget_2->setRowHeight(1,35);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);   //禁止编辑
    ui->tableWidget_2->setItem(0,0,&tofMaxItem_value);
    ui->tableWidget_2->setItem(0,1,&tofMInItem_value);
    ui->tableWidget_2->setItem(1,0,&peakMaxItem_value);
    ui->tableWidget_2->setItem(1,1,&peakMInItem_value);


    ui->tableWidget_4->setColumnWidth(0,117);
    ui->tableWidget_4->setColumnWidth(1,117);
    ui->tableWidget_4->setRowHeight(0,23);
    ui->tableWidget_4->setRowHeight(1,23);
    ui->tableWidget_4->setRowHeight(0,23);
    ui->tableWidget_4->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中
    ui->tableWidget_4->setEditTriggers(QAbstractItemView::NoEditTriggers);   //禁止编辑
    ui->tableWidget_4->setItem(0,0,&xMInItem_value);
    ui->tableWidget_4->setItem(0,1,&xMaxItem_value);
    ui->tableWidget_4->setItem(1,0,&yMinItem_value);
    ui->tableWidget_4->setItem(1,1,&yMaxItem_value);
    ui->tableWidget_4->setItem(2,0,&zMinItem_value);
    ui->tableWidget_4->setItem(2,1,&zMaxItem_value);



}


//刷新显示的槽函数
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
 *  2、没有接收到数据
 */
void MainWindow::linkInfoSlot(int flagNum)
{
    if(0 == flagNum)
    {
        ui->pushButton->setText("关闭连接");
    }else if(1 == flagNum)
    {
        QMessageBox::information(NULL,"告警",QString::fromUtf8("未找到设备！"));
    }else if(2 == flagNum)
    {
        QMessageBox::information(NULL,"告警",QString::fromUtf8("未接收到数据，请检查设备链接或配置信息"));
    }else if(3 == flagNum)
    {
        QMessageBox::information(NULL,"告警",QString::fromUtf8("打开设备失败"));
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}

//打开设备
void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text() == "连接设备")
    {
        emit readSignal();

    }else if(ui->pushButton->text() == "关闭连接")
    {
        emit closeLinkSignal();
        ui->pushButton->setText("连接设备");
    }

}


//播放槽函数
void MainWindow::on_pushButton_2_clicked()
{
    if(ui->pushButton_2->text() == "播放")
    {
        if(isWriteSuccess)
        {
            showTimer.start(50);
            ui->widget->readFileTimer.start(100);
        }else
        {
            QMessageBox::information(NULL,"告警","未接收到数据，请检查设备连接！");
        }
        ui->pushButton_2->setText("暂停");

    }else
    {
        showTimer.stop();
        ui->widget->readFileTimer.stop();
        ui->pushButton_2->setText("播放");
    }




}

//读取系统寄存器
void MainWindow::on_readSys_pushButton_clicked()
{
    emit readSysSignal();
}

//写入系统寄存器
void MainWindow::on_writeSys_pushButton_clicked()
{
    emit writeSysSignal();
}

//读取设备寄存器
void MainWindow::on_readDev_pushButton_clicked()
{
    emit readDevSignal();
}

//写入设备寄存器
void MainWindow::on_writeDev_pushButton_clicked()
{
    emit writeDevSignal();
}

//加载配置集
void MainWindow::on_loadSetting_pushButton_clicked()
{
    emit loadSettingSignal();
}

//保存配置集
void MainWindow::on_saveSetting_pushButton_clicked()
{
    emit saveSettingSignal();
}
