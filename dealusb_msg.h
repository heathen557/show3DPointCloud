#ifndef DEALUSB_MSG_H
#define DEALUSB_MSG_H
#include<pcl/io/pcd_io.h>//pcd 读写类相关的头文件。
#include<pcl/io/ply_io.h>
#include<pcl/point_types.h> //PCL中支持的点类型头文件。
#include <QObject>
#include<QImage>

class DealUsb_msg : public QObject
{
    Q_OBJECT
public:
    explicit DealUsb_msg(QObject *parent = 0);

    QByteArray recvArray;

    int lastSpadNum;

    QImage microQimage;
    QImage macroQimage;

    pcl::PointCloud<pcl::PointXYZRGB> tempRgbCloud;

    int cloudIndex;

    float  LSB ; //时钟频率
    bool isFirstLink;   //因为USB驱动存在问题，故设次函数

    float tofMin,tofMax,peakMin,peakMax,xMin,xMax,yMin,yMax,zMin,zMax;
    float temp_x,temp_y,temp_z;

    int r,g,b,rgb;

    int tmp_tofInfo[16384];
    int tmp_peakInfo[16384];

    QString tofPeakToSave_string;   //存储文件所需的tof和peak;
    QString tofPeakNum[16384];



signals:
    void staticValueSignal(float,float,float,float,float,float,float,float,float,float);

    void savePCDSignal(pcl::PointCloud<pcl::PointXYZRGB>,int);    // int 0:二进制  1：ASCII

    void saveTXTSignal(QString );

public slots:
    void recvMsgSlot(QByteArray array);


};

#endif // DEALUSB_MSG_H
