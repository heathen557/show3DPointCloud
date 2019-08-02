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
#include<QFile>


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
    QImage saveTofIntenImage;
    int lastSpadNum;

    pcl::PointCloud<pcl::PointXYZRGB> tempRgbCloud;
//    pcl::PointCloud<pcl::PointXYZI> tempcloud_XYZI;
//    pcl::PointCloud<pcl::PointXYZ>  tempcloud_RGB;
    int cloudIndex;

    float  LSB ; //时钟频率
    bool isFirstLink;   //因为USB驱动存在问题，故设次函数

    float tofMin,tofMax,peakMin,peakMax,xMin,xMax,yMin,yMax,zMin,zMax;
    float temp_x,temp_y,temp_z;

    int r,g,b,rgb;

    int tmp_tofInfo[16384];
    int tmp_peakInfo[16384];

    int idVendor_,idProduct_;

    QString tofPeakToSave_string;



signals:
    void linkInfoSignal(int );      //向主线程发送链接信息（错误警告）
                                    // 0：连接正常 1没找到设备
                                    // 2:没有接收到数据  3打开设备失败
                                    // 4：读取系统成功；5：读取系统失败；
                                    // 6：读取设备成功；7：读取设备失败
                                    // 8：加载配置信息成功；9：加载配置信息失败
                                    // 10：保存配置信息成功； 11：保存配置信息失败
                                    // 12：写入系统成功      13：写入系统失败
                                    // 14：写入设备成功      15：写入设备失败
    void staticValueSignal(float,float,float,float,float,float,float,float,float,float);

    void reReadSysSignal(QString);  //读取系统指令 返回信号

    void reReadDevSignal(QString); //读取设备指令 返回信号

    void savePCDSignal();

    void saveTXTSignal(QString );

public slots:
    void read_usb();                 //读取USB内容的槽函数
    void openLinkDevSlot();          //打开设备连接的槽函数
    void run(int vId, int pId);
    void closeUSB();

    void readSysSlot();
    void writeSysSlot(int addr,QString data);
    void readDevSlot(int id,int address);
    void writeDevSlot(int slavId,int addr,QString data);
    void loadSettingSlot(QString filePath);
    void saveSettingSlot(QString filePath,int deviceId,bool  recvFlag);
};

#endif // RECEUSB_MSG_H
