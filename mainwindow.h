#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QImage>
//#include"receusb_msg.h"
#include<QThread>
#include "ui_mainwindow.h"
#include<QMutex>
#include<QMessageBox>
#include"receusb_msg.h"
#include<QTableWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTimer showTimer;
    QTimer oneSecondTimer;

    ReceUSB_Msg *recvUsbMsg_obj;
    QThread *recvUsbThread;

    QTableWidgetItem tofMinItem_value;
    QTableWidgetItem tofMaxItem_value;
    QTableWidgetItem peakMinItem_value;
    QTableWidgetItem peakMaxItem_value;

    QTableWidgetItem xMinItem_value;
    QTableWidgetItem xMaxItem_value;
    QTableWidgetItem yMinItem_value;
    QTableWidgetItem yMaxItem_value;
    QTableWidgetItem zMinItem_value;
    QTableWidgetItem zMaxItem_value;

//    int framePerSecond;
    float tofMin_,tofMax_,peakMin_,peakMax_,xMin_,xMax_,yMin_,yMax_,zMin_,zMax_;

    QImage resImage;         //用来显示缩放后的tof图像
    QImage resIntenImage;    //用来显示缩放后的intensity图像

    bool isLinkSuccess;      //是否连接成功的标识，若是没有连接，则不能进行后续操作


private slots:
    void on_pushButton_clicked();    //测试用的槽函数
    void showImageSlot();           //显示二维图像的槽函数
    void on_pushButton_2_clicked();
    void linkInfoSlot(int );      //接收线程发过来的链接信息（告警、错误）
    void initGUI();


    void on_readSys_pushButton_clicked();

    void on_writeSys_pushButton_clicked();

    void on_readDev_pushButton_clicked();

    void on_writeDev_pushButton_clicked();

    void on_loadSetting_pushButton_clicked();

    void on_saveSetting_pushButton_clicked();

    void recvStaticValueSlot(float tofMin,float tofMax,float peakMin,float peakMax,float xMin,float xMax,float yMin,float yMax,float zMin,float zMax);

    void oneSecondSlot();   //每秒的槽函数

    void queryPixSlot(int x, int y);     //接收label空间坐标的槽函数

    void reReadSysSlot(QString str);  //读取系统指令 返回槽函数

    void reReadDevSlot(QString str); //读取设备指令 返回槽函数

private:
    Ui::MainWindow *ui;

signals:
    void readSignal();
    void closeLinkSignal();   //关闭连接

    void readSysSignal();
    void writeSysSignal(int,QString);
    void readDevSignal();
    void writeDevSignal();
    void loadSettingSignal();
    void saveSettingSignal();


};

#endif // MAINWINDOW_H
