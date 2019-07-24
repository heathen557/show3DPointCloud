#ifndef RECEUSB_MSG_H
#define RECEUSB_MSG_H
#include<pcl/io/pcd_io.h>//pcd 读写类相关的头文件。
#include<pcl/io/ply_io.h>
#include<pcl/point_types.h> //PCL中支持的点类型头文件。
#include <QObject>
#include<QDebug>
#include<QTimer>
#include<QImage>
#include<Windows.h>
#include"lusb0_usb.h"


class ReceUSB_Msg : public QObject
{
    Q_OBJECT
public:
    explicit ReceUSB_Msg(QObject *parent = 0);

    struct usb_device *findUSBDev(const unsigned short idVendor,
                                  const unsigned short idProduct);

    bool openUSB(struct usb_device *dev);


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

    pcl::PointCloud<pcl::PointXYZI> tempcloud_XYZI;
    pcl::PointCloud<pcl::PointXYZ>  tempcloud_RGB;
    int cloudIndex;

    float  LSB ; //时钟频率
    bool isFirstLink;   //因为USB驱动存在问题，故设次函数

    float tofMin,tofMax,peakMin,peakMax,xMin,xMax,yMin,yMax,zMin,zMax;
    float temp_x,temp_y,temp_z;




signals:
    void linkInfoSignal(int );      //向主线程发送链接信息（错误警告）
                                    // 0：连接正常 1没找到设备
                                    // 2:没有接收到数据  3打开设备失败
                                    // 4：读取系统成功；5：读取系统失败；
                                    // 6：读取设备成功；7：读取设备失败
                                    // 8：加载配置信息成功；9：加载配置信息失败
                                    // 10：保存配置信息成功； 11：保存配置信息失败
    void staticValueSignal(float,float,float,float,float,float,float,float,float,float);

public slots:
    void read_usb();                 //读取USB内容的槽函数
    void openLinkDevSlot();          //打开设备连接的槽函数
    void run();
    void closeUSB();

    void readSysSlot();
    void writeSysSlot();
    void readDevSlot();
    void writeDevSlot();
    void loadSettingSlot();
    void saveSettingSlot();
};

#endif // RECEUSB_MSG_H
