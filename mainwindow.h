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

    ReceUSB_Msg *recvUsbMsg_obj;
    QThread *recvUsbThread;

    QTableWidgetItem tofMInItem_value;
    QTableWidgetItem tofMaxItem_value;
    QTableWidgetItem peakMInItem_value;
    QTableWidgetItem peakMaxItem_value;

    QTableWidgetItem xMInItem_value;
    QTableWidgetItem xMaxItem_value;
    QTableWidgetItem yMinItem_value;
    QTableWidgetItem yMaxItem_value;
    QTableWidgetItem zMinItem_value;
    QTableWidgetItem zMaxItem_value;



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

private:
    Ui::MainWindow *ui;

signals:
    void readSignal();
    void closeLinkSignal();   //关闭连接

    void readSysSignal();
    void writeSysSignal();
    void readDevSignal();
    void writeDevSignal();
    void loadSettingSignal();
    void saveSettingSignal();


};

#endif // MAINWINDOW_H
