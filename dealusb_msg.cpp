#include "dealusb_msg.h"
#include<qdebug.h>


extern QMutex mutex;
extern QImage tofImage;
extern QImage intensityImage;

extern pcl::PointCloud<pcl::PointXYZRGB> pointCloudRgb;
extern bool isShowPointCloud;
extern bool isWriteSuccess;    //写入命令是否成功标识

extern int tofInfo[16384];
extern int peakInfo[16384];

/*保存用到的标识*/
extern bool isSaveFlag;        //是否进行存储
extern QString saveFilePath;   //保存的路径  E:/..../.../的形式
extern int saveFileIndex;      //文件标号；1作为开始
extern int formatFlag;         //0:二进制； 1：ASCII 2：TXT

extern bool isTOF;
extern int gainImage;






DealUsb_msg::DealUsb_msg(QObject *parent) : QObject(parent),
    microQimage(256,64, QImage::Format_RGB32),macroQimage(256,64, QImage::Format_RGB32)
{

//    pointCloudRgb.width = 16384;
//    pointCloudRgb.height = 1;
//    pointCloudRgb.resize(pointCloudRgb.width);

//    tempRgbCloud.width = 16384;
//    tempRgbCloud.height = 1 ;
//    tempRgbCloud.points.resize(tempRgbCloud.width);

    LSB = 0.015; //时钟频率
    isFirstLink = true;

}

void DealUsb_msg::recvMsgSlot(QByteArray array)
{
    int ret;
    char *MyBuffer;

    MyBuffer = array.data();

    isWriteSuccess = true;    //写入命令是否成功标识，成功以后才能点击“显示”按钮操作
    int imgRow,imgCol;
    int spadNum = MyBuffer[0] + (((ushort)MyBuffer[1]) << 8);
    int line_number = MyBuffer[2] + (((ushort)MyBuffer[3]) << 8);
//    qDebug()<<"here   spadNum = "<<spadNum<<"  line_number = "<<line_number<<endl;


    if(spadNum==0 && lastSpadNum==7)  //此时说明上一帧数据已经接收完毕，把整帧数据付给其他线程，供其显示，数据可以显示了
    {
        mutex.lock();
        tofImage = microQimage;
        intensityImage = macroQimage;

        pcl::copyPointCloud(tempRgbCloud,pointCloudRgb);
        mutex.unlock();
        isShowPointCloud = true;

        //判断是否保存数据
        if(isSaveFlag)
        {
            if(formatFlag == 0)   //保存二进制pcd
            {
                emit savePCDSignal(tempRgbCloud,0);
            }else if(formatFlag == 1)
            {
                emit savePCDSignal(tempRgbCloud,1);
            }else if(formatFlag == 2)
            {
                for(int i=0; i<16384; i++)
                {
                     tofPeakToSave_string.append(tofPeakNum[i]);
                     tofPeakNum[i].clear();
                }
                emit saveTXTSignal(tofPeakToSave_string);
                tofPeakToSave_string.clear();
            }
        }


        memcpy(tofInfo, tmp_tofInfo, 16384 * sizeof(int));
        memcpy(peakInfo, tmp_peakInfo, 16384 * sizeof(int));

        emit staticValueSignal(tofMin,tofMax,peakMin,peakMax,xMin,xMax,yMin,yMax,zMin,zMax);
        //重置变量
        tofMin = 10000;
        tofMax = -10000;
        peakMin = 10000;
        peakMax = -10000;
        xMin = 10000;
        xMax = -10000;
        yMin = 10000;
        yMax = -10000;
        zMin = 10000;
        zMax = -10000;

        tempRgbCloud.clear();
    }

    int line_offset = spadNum / 2;
    int row_offset = (spadNum + 1) % 2;      //表示是在第一行 还是在第二行

    for(int i=0; i<64; i++)
    {

        int tof = (ushort)((MyBuffer[4 + i * 4]) + (((ushort)MyBuffer[4 + i * 4 + 1]) << 8));
        int intensity = (ushort)((MyBuffer[4 + i * 4 + 2]) + (((ushort)MyBuffer[4 + i * 4 + 3]) << 8));

//        if(tof>1024)
//            tof = 0;

//        if(intensity>300)
//            intensity = 0;


        //设置TOF图像、强度图像的颜色
        QRgb tofColor,intenColor;
        int gainIndex_tof = tof*gainImage;
        int gainIndex_intensity =intensity * gainImage;
        if(gainIndex_tof<1024 && gainIndex_tof>=0)
            tofColor = qRgb(colormap[gainIndex_tof * 3], colormap[gainIndex_tof * 3 + 1], colormap[gainIndex_tof * 3 + 2]);
        else
            tofColor = qRgb(colormap[1023 * 3], colormap[1023 * 3 + 1], colormap[1023 * 3 + 2]);

        if(gainIndex_intensity<1024 && gainIndex_intensity>=0)
            intenColor = qRgb(colormap[gainIndex_intensity * 3], colormap[gainIndex_intensity * 3 + 1], colormap[gainIndex_intensity * 3 + 2]);
        else
            intenColor = qRgb(colormap[1023 * 3], colormap[1023 * 3 + 1], colormap[1023 * 3 + 2]);


        //行列以及颜色传递给图像
        imgRow = i * 4 + line_offset;
        imgCol = line_number * 2 + row_offset;

//        qDebug()<<"index =="<< imgCol*256+imgRow<<endl;
        cloudIndex = imgCol*256+imgRow;      //在点云数据中的标号

        if(imgRow>=0 && imgRow<256 && imgCol>=0 && imgCol<64)
        {
            microQimage.setPixel(imgRow,imgCol,tofColor);         //TOF图像的赋值
            macroQimage.setPixel(imgRow,imgCol,intenColor);       //强度图像的赋值

            tmp_tofInfo[cloudIndex] = tof ;
            tmp_peakInfo[cloudIndex] = intensity ;

            if(formatFlag ==2  && isSaveFlag == true)
            {
//                tofPeakToSave_string.append(QString::number(tof)).append(", ").append(QString::number(intensity)).append("\n");
//                tofPeakToSave_string[cloudIndex] =
                tofPeakNum[cloudIndex] = QString::number(tof).append(", ").append(QString::number(intensity)).append("\n");

            }else
            {
//                tofPeakToSave_string.clear();
                tofPeakNum[cloudIndex].clear();

            }


            //获取三维坐标
            if(isTOF)
            {
                temp_x = tof * x_Weight[cloudIndex] * LSB;
//                temp_y = tof * y_Weight[cloudIndex] * LSB;
                temp_y = tof * LSB;
                temp_z = tof * z_Weight[cloudIndex] * LSB;
            }else
            {
                temp_x = intensity * x_Weight[cloudIndex] * LSB;
//                temp_y = intensity * y_Weight[cloudIndex] * LSB;
                temp_y = intensity * LSB;
                temp_z = intensity * z_Weight[cloudIndex] * LSB;
            }



//            if(temp_y<4.5)    //第三层
            {
                cloutPoint.x = temp_x;
                cloutPoint.y = temp_y;
                cloutPoint.z = temp_z;
                        //点云颜色
                QColor mColor = QColor(tofColor);
                r = mColor.red();
                g = mColor.green();
                b = mColor.blue();
                rgb = ((int)r << 16 | (int)g << 8 | (int)b);

//                tempRgbCloud.points[cloudIndex].x = temp_x;
//                tempRgbCloud.points[cloudIndex].y = temp_y;
//                tempRgbCloud.points[cloudIndex].z = temp_z;
//                tempRgbCloud.points[cloudIndex].rgb = *reinterpret_cast<float*>(&rgb);

                cloutPoint.rgb = *reinterpret_cast<float*>(&rgb);


                tempRgbCloud.push_back(cloutPoint);
            }



            //统计点云空间坐标最大值、最小值
            xMax = (temp_x>xMax) ? temp_x : xMax;
            xMin = (temp_x<xMin) ? temp_x : xMin;
            yMax = (temp_y>yMax) ? temp_y : yMax;
            yMin = (temp_y<yMin) ? temp_y : yMin;
            zMax = (temp_z>zMax) ? temp_z : zMax;
            zMin = (temp_z<zMin) ? temp_z : zMin;

            //统计二维图像
            tofMax = (tof>tofMax) ? tof : tofMax;
            tofMin = (tof<tofMin) ? tof : tofMin;
            peakMax = (intensity>peakMax) ? intensity : peakMax;
            peakMin = (intensity<peakMin) ? intensity : peakMin;
        }
        else
            qDebug()<<"给像素赋值时出现异常 imgrow="<<imgRow<<"   imgCol = "<<imgCol<<endl;
    }
    lastSpadNum = spadNum ;



}
