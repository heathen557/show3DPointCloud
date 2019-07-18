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

private slots:
    void on_pushButton_clicked();    //测试用的槽函数
    void showImageSlot();           //显示二维图像的槽函数
    void on_pushButton_2_clicked();
    void linkInfoSlot(int );      //接收线程发过来的链接信息（告警、错误）


private:
    Ui::MainWindow *ui;

signals:
    void readSignal();

};

#endif // MAINWINDOW_H
