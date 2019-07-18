#ifndef RECEUSB_MSG_H
#define RECEUSB_MSG_H
#include<pcl/io/pcd_io.h>//pcd 读写类相关的头文件。
#include<pcl/io/ply_io.h>
#include<pcl/point_types.h> //PCL中支持的点类型头文件。
#include <QObject>
#include<QDebug>
#include<QTimer>
#include<QImage>

#include"lusb0_usb.h"


class ReceUSB_Msg : public QObject
{
    Q_OBJECT
public:
    explicit ReceUSB_Msg(QObject *parent = 0);

    struct usb_device *findUSBDev(const unsigned short idVendor,
                                  const unsigned short idProduct);

    bool openUSB(struct usb_device *dev);

    void closeUSB();

    bool System_Register_Read(int Address, QString &Data);

    bool System_Register_Write(int Address, QString &Data);

    bool Device_Register_Read(int slavedId,int Address,QString &Data);

    bool Device_Register_Write(int slavedId,int Address,QString &Data);


    bool devOpenFlg;

    usb_dev_handle *devHandle;

    struct usb_device * dev;

    QTimer *readTimer;               //此处采用轮询的方式读取USB串口数据，1ms读取一次
    QTimer showTimer;
    QImage microQimage;
    QImage macroQimage;

    int lastSpadNum;



signals:
    void linkInfoSignal(int );      //向主线程发送链接信息（错误警告） 1没找到设备

public slots:
    void read_usb();                 //读取USB内容的槽函数
    void on_pushButton_clicked();
    void run();

};

#endif // RECEUSB_MSG_H
