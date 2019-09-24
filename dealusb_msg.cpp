#include "dealusb_msg.h"
#include<qdebug.h>
#include<QTime>


extern QMutex mutex;
extern QImage tofImage;
extern QImage intensityImage;

extern pcl::PointCloud<pcl::PointXYZRGB> pointCloudRgb;
extern bool isShowPointCloud;
extern bool isWriteSuccess;    //写入命令是否成功标识

extern QMutex mouseShowMutex;
extern int mouseShowTOF[256][64];
extern int mouseShowPEAK[256][64];

/*保存用到的标识*/
extern bool isSaveFlag;        //是否进行存储
extern QString saveFilePath;   //保存的路径  E:/..../.../的形式
extern int saveFileIndex;      //文件标号；1作为开始
extern int formatFlag;         //0:二进制； 1：ASCII 2：TXT

/***tof/peak 切换标识****/
extern bool isTOF;
extern int gainImage;   //增益

/*******统计信息相关的变量***********/
extern QMutex statisticMutex;
extern vector<vector<int>> allStatisticTofPoints;   //用于统计 均值和方差的 容器  TOF
extern vector<vector<int>> allStatisticPeakPoints;   //用于统计 均值和方差的 容器  TOF





DealUsb_msg::DealUsb_msg(QObject *parent) : QObject(parent),
    microQimage(256,64, QImage::Format_RGB32),macroQimage(256,64, QImage::Format_RGB32)
{

    //    pointCloudRgb.width = 16384;
    //    pointCloudRgb.height = 1;
    //    pointCloudRgb.resize(pointCloudRgb.width);

    tempRgbCloud.width = 16384;
    tempRgbCloud.height = 1 ;
    tempRgbCloud.points.resize(tempRgbCloud.width);

    LSB = 0.015; //时钟频率
    isFirstLink = true;
    isFilterFlag = false ;    //初始化时不进行滤波
    isTOF = true;
    localFile_timer = NULL;

    //    linkServer();



    //总共有16384个点，针对每一个点开启一个独立的容器进行存储相关内容    统计相关
    statisticStartFlag = true;
    statisticFrameNumber = 10;
    vector<int> singlePoint;
    for(int i=0; i<16384; i++)
    {
        tempStatisticTofPoints.push_back(singlePoint);
        tempStatisticPeakPoints.push_back(singlePoint);
        allStatisticTofPoints.push_back(singlePoint);
        allStatisticPeakPoints.push_back(singlePoint);
    }

    //    readLocalPCDFile();
}

//修改统计帧数的槽函数
void DealUsb_msg::alterStatisticFrameNum_slot(int num)
{
    statisticFrameNumber = num ;

}

//修改是否进行滤波的槽函数
void DealUsb_msg::isFilter_slot(bool isFiter)
{
    //    readLocalPCDFile();
    if(true == isFiter)
    {
        isFilterFlag = true;
    }else{
        isFilterFlag = false;
    }


}

//260个字节的 解析协议的槽函数
void DealUsb_msg::recvMsgSlot(QByteArray array)
{
    int ret;
    char *MyBuffer;

    MyBuffer = array.data();

    isWriteSuccess = true;    //写入命令是否成功标识，成功以后才能点击“显示”按钮操作
    int imgRow,imgCol;
    int spadNum = (quint8)(MyBuffer[0]) +  (((quint8)(MyBuffer[1]))<<8);
    int line_number = (quint8)(MyBuffer[2]) +  (((quint8)(MyBuffer[3]))<<8);
    //    qDebug()<<"here111   spadNum = "<<spadNum<<"  line_number = "<<line_number<<endl;

    if(line_number!=0  && ( (lastLineNum+1) != line_number) )
    {
        qDebug()<<"lastLineNum="<<lastLineNum <<"   "<<line_number<<endl;
    }
    lastLineNum = line_number;


    if(spadNum==0 && lastSpadNum==7)  //此时说明上一帧数据已经接收完毕，把整帧数据付给其他线程，供其显示，数据可以显示了
    {

        //判断是否保存数据
        if(isSaveFlag)
        {
            if(formatFlag == 0)   //保存二进制pcd
            {
                emit savePCDSignal(tempRgbCloud,0);
            }else if(formatFlag == 1)      //保存ASCII码版本的 pcd文件
            {
                emit savePCDSignal(tempRgbCloud,1);
            }else if(formatFlag == 2)   //保存原始的TOF 和 PEAK 数据
            {
                bool flag = true;
                for(int i=0; i<16384; i++)
                {
                    if(tofPeakNum[i].isEmpty())
                        flag =false;
                    tofPeakToSave_string.append(tofPeakNum[i]);
                    tofPeakNum[i].clear();
                }

                if(flag)
                    emit saveTXTSignal(tofPeakToSave_string);
                tofPeakToSave_string.clear();
            }
        }


        //统计信息相关的 ，将统计信息的容器赋值给全局变量
        if(statisticStartFlag)
        {
            statisticMutex.lock();
            allStatisticTofPoints = tempStatisticTofPoints;
            allStatisticPeakPoints = tempStatisticPeakPoints;
            statisticMutex.unlock();
        }


        //向主线程中发送最大值、最小值等统计信息
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


        /*
        //将二维、三维数据存储到全局变量当中，主线程当中会调用全局变量进行二维、三维图像的显示
        mutex.lock();
        tofImage = microQimage;
        intensityImage = macroQimage;
        pcl::copyPointCloud(tempRgbCloud,pointCloudRgb);
        mutex.unlock();
        isShowPointCloud = true;
*/

        /*******************对Tof 图像进行过滤0的滤波处理*******************************/
        vector<int> numArray;
        QRgb tofColor;
        int gainIndex_tof;
        int cloudIndex;
        for(int imgX=0; imgX<256; imgX++)
            for(int imgY=1; imgY<64; imgY++)
            {
                if(0 == mouseShowTOF[imgX][imgY])
                {

                    if( imgX>=1 )
                        numArray.push_back(mouseShowTOF[imgX-1][imgY]);
                    if( imgX<=254 )
                        numArray.push_back(mouseShowTOF[imgX+1][imgY]);
                    if( imgY>=1)
                        numArray.push_back(mouseShowTOF[imgX][imgY-1]);
                    if( imgY<=62)
                        numArray.push_back(mouseShowTOF[imgX][imgY+1]);

                    sort(numArray.begin(),numArray.end());
                    int midNumTof = numArray[numArray.size()/2];

                    //更新该点的值（鼠标显示）；
                    mouseShowTOF[imgX][imgY] =midNumTof;

                    //更新TOF 图像的像素值
                    gainIndex_tof = midNumTof*gainImage;
                    if(gainIndex_tof<1024 && gainIndex_tof>=0)
                        tofColor = qRgb(colormap[gainIndex_tof * 3], colormap[gainIndex_tof * 3 + 1], colormap[gainIndex_tof * 3 + 2]);
                    else
                        tofColor = qRgb(colormap[1023 * 3], colormap[1023 * 3 + 1], colormap[1023 * 3 + 2]);
                    microQimage.setPixel(imgX,imgY,tofColor);         //TOF图像的赋值

                    //更新点云数据的值 以及颜色
                    cloudIndex = imgY*256 + imgX;
                    temp_x = midNumTof * x_Weight[cloudIndex] * LSB;
                    temp_y = midNumTof * y_Weight[cloudIndex] * LSB;
                    temp_z = midNumTof * z_Weight[cloudIndex] * LSB;
                    QColor mColor = QColor(tofColor);
                    r = mColor.red();
                    g = mColor.green();
                    b = mColor.blue();
                    rgb = ((int)r << 16 | (int)g << 8 | (int)b);
                    tempRgbCloud.points[cloudIndex].x = temp_x;
                    tempRgbCloud.points[cloudIndex].y = temp_y;
                    tempRgbCloud.points[cloudIndex].z = temp_z;
                    tempRgbCloud.points[cloudIndex].rgb = *reinterpret_cast<float*>(&rgb);
                    numArray.clear();
                }
            }







        // 1、将滤波功能放到这里进行实现，
        // 2、将滤波后的三维点云 同步到二维图像
        if(true == isFilterFlag)
        {
            /*******************开启滤波功能*********************************/
            //先用直通滤波把所有零点重复的零点过滤掉
            //            pcl::PassThrough<pcl::PointXYZRGB> pass;                      //创建滤波器对象
            //            pass.setInputCloud(tempRgbCloud.makeShared());                //设置待滤波的点云
            //            pass.setFilterFieldName("y");                                 //设置在Z轴方向上进行滤波
            //            pass.setFilterLimits(0, 0.10);                                //设置滤波范围(从最高点向下0.10米去除)
            //            pass.setFilterLimitsNegative(true);                           //保留
            //            pass.filter(tempRgbCloud_pass);                                   //滤波并存储
            //            if(tempRgbCloud_pass.size()<1)
            //                return;

            //  基于统计运算的滤波算法
            //        DealedCloud_rgb.clear();
            //        pcl::StatisticalOutlierRemoval<pcl::PointXYZRGB> sor;
            //        sor.setInputCloud(tempRgbCloud_pass.makeShared());
            //        sor.setMeanK(20);
            //        sor.setStddevMulThresh(0);
            //        //40  0.1 不见前面噪点
            //        sor.filter(tempRgbCloud_radius);
            //        qDebug()<<"after filter the points'Number = "<<DealedCloud_rgb.size()<<endl;

            //            QTime time;
            //            time.start();
            //            int len;
            tempRgbCloud_radius.resize(0);
            pcl::RadiusOutlierRemoval<pcl::PointXYZRGB> outrem(true);      //设置为true以后才能获取到滤出的噪点的 个数以及点的序列号
            outrem.setInputCloud(tempRgbCloud.makeShared());              //设置输入点云
            outrem.setRadiusSearch(0.25);              //设置在0.8半径的范围内找邻近点
            outrem.setMinNeighborsInRadius(15);       //设置查询点的邻近点集数小于2的删除  30
            outrem.filter (tempRgbCloud_radius);//执行条件滤波，存储结果到cloud_filtered
            int len = outrem.getRemovedIndices()->size();


            //条件滤波   设置半径 以及 圆周内的点数


            //            qDebug()<<"dealusb_msg    fileted size = "<<outrem.getRemovedIndices()->size();
            //            qDebug()<<" passThrough cost time = "<<time.elapsed()<<endl;
            /*************************以上为滤波处理部分************************************************************/

            /***********************接下来 根据点云的序号 去除二维图像中的噪声************************/

            /*            //此种方法采用取均值的方法，效果不是太好
            int index,pix_x,pix_y;
            QRgb clearTofCol,clearPeakCol;
            for(int i=0; i<len; i++)
            {
                index = outrem.getRemovedIndices()->at(i);


                pix_x = index % 256;
                pix_y = index / 256;

                //将像素去均值 后赋予噪点处的像素值
                if(pix_y+1 < 64 && pix_y-1 >0)
                {
                     clearTofCol =  (microQimage.pixel(pix_x,pix_y+1) + microQimage.pixel(pix_x,pix_y-1))/2.0;
                     clearPeakCol = (macroQimage.pixel(pix_x,pix_y+1) + macroQimage.pixel(pix_x,pix_y-1))/2.0;
                }
                else
                {
                     clearTofCol =  microQimage.pixel(pix_x,pix_y);
                     clearPeakCol = macroQimage.pixel(pix_x,pix_y);
                }
                microQimage.setPixel(pix_x,pix_y,clearTofCol);
                macroQimage.setPixel(pix_x,pix_y,clearPeakCol);
            }
*/

            int index,pix_x,pix_y;
            for(int i=0; i<len; i++)
            {
                index = outrem.getRemovedIndices()->at(i);
                pix_x = index % 256;
                pix_y = index / 256;
                mouseShowTOF[pix_x][pix_y] = 0;   //噪点赋值为黑色
            }

            vector<int> numArray;
            QRgb tofColor;
            int gainIndex_tof;
            for(int i=0;i<len; i++)
            {
                index = outrem.getRemovedIndices()->at(i);
                pix_x = index % 256;
                pix_y = index / 256;

                if( pix_x>=1 )
                    numArray.push_back(mouseShowTOF[pix_x-1][pix_y]);
                if( pix_x<=254 )
                    numArray.push_back(mouseShowTOF[pix_x+1][pix_y]);
                if( pix_y>=1)
                    numArray.push_back(mouseShowTOF[pix_x][pix_y-1]);
                if( pix_y<=62)
                    numArray.push_back(mouseShowTOF[pix_x][pix_y+1]);

                sort(numArray.begin(),numArray.end());
                int midNumTof = numArray[numArray.size()/2];
                //                qDebug()<<"midNumTof = "<<midNumTof<<endl;


                numArray.clear();

                //更新鼠标显示的TOF值
                mouseShowTOF[pix_x][pix_y] = midNumTof;

                //更新TOF 图像的像素值
                gainIndex_tof = midNumTof*gainImage;
                if(gainIndex_tof<1024 && gainIndex_tof>=0)
                    tofColor = qRgb(colormap[gainIndex_tof * 3], colormap[gainIndex_tof * 3 + 1], colormap[gainIndex_tof * 3 + 2]);
                else
                    tofColor = qRgb(colormap[1023 * 3], colormap[1023 * 3 + 1], colormap[1023 * 3 + 2]);
                microQimage.setPixel(pix_x,pix_y,tofColor);         //TOF图像的赋值
            }


            mutex.lock();
            tofImage = microQimage;
            intensityImage = macroQimage;
            pcl::copyPointCloud(tempRgbCloud_radius,pointCloudRgb);
            mutex.unlock();
            /***************************************************************/

        }else{                      //不进行滤波
            mutex.lock();
            tofImage = microQimage;
            intensityImage = macroQimage;
            pcl::copyPointCloud(tempRgbCloud,pointCloudRgb);
            mutex.unlock();
        }
        isShowPointCloud = true;

        //        tempRgbCloud.clear();
    }//以上为处理完整的一帧数据





    int line_offset = spadNum / 2;           //取值 0 1 2 3 ；
    int row_offset = (spadNum + 1) % 2;      //表示是在第一行 还是在第二行

    for(int i=0; i<64; i++)
    {

        int tof,intensity;
        if(isTOF == false)   //设置一个不可能的值
        {
            tof = quint8(MyBuffer[4 + i * 4]) + ((quint8(MyBuffer[4 + i * 4 +1]))<<8);
            intensity = quint8(MyBuffer[4 + i * 4 + 2]) + ((quint8(MyBuffer[4 + i * 4 + 3 ]))<<8);
        }else
        {
            intensity = quint8(MyBuffer[4 + i * 4]) + ((quint8(MyBuffer[4 + i * 4 +1]))<<8);
            tof = quint8(MyBuffer[4 + i * 4 + 2]) + ((quint8(MyBuffer[4 + i * 4 + 3 ]))<<8);
        }

        //        //行列以及颜色传递给图像
        //        //宏像素 颠倒一下位置 0...63 改为 1 0  3 2  5 4 ;
        //        //方法为 偶数时 1+i; 奇数时 i-1;
        //        int newi;
        //        if(0 == i%2)
        //        {
        //            newi = i+1;
        //        }else
        //        {
        //            newi = i-1;
        //        }


        imgRow = i * 4 + line_offset;
        imgCol = line_number * 2 + row_offset;
        cloudIndex = imgCol*256+imgRow;      //在点云数据中的标号

        /********************对TOF 两针做平均*************************/
        //        int tmpTof = tof;
        //        tof = (lastTOF[cloudIndex] + tof)/2;
        //        lastTOF[cloudIndex] = tmpTof;
        /************************************************/

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


        if(imgRow>=0 && imgRow<256 && imgCol>=0 && imgCol<64)
        {
            microQimage.setPixel(imgRow,imgCol,tofColor);         //TOF图像的赋值
            macroQimage.setPixel(imgRow,imgCol,intenColor);       //强度图像的赋值

            /************鼠标点击处显示信息相关*************/
            mouseShowMutex.lock();
            mouseShowTOF[imgRow][imgCol] = tof;
            mouseShowPEAK[imgRow][imgCol] = intensity;
            mouseShowMutex.unlock();

            /*********文件保存相关*****************/
            if(formatFlag ==2  && isSaveFlag == true)
            {
                tofPeakNum[cloudIndex] = QString::number(tof).append(", ").append(QString::number(intensity)).append("\n");
            }else
            {
                tofPeakNum[cloudIndex].clear();

            }


            /************点云数据相关************/
            //获取三维坐标
            temp_x = tof * x_Weight[cloudIndex] * LSB;
            temp_y = tof * y_Weight[cloudIndex] * LSB;
            temp_z = tof * z_Weight[cloudIndex] * LSB;

            QColor mColor = QColor(tofColor);
            r = mColor.red();
            g = mColor.green();
            b = mColor.blue();
            rgb = ((int)r << 16 | (int)g << 8 | (int)b);


            tempRgbCloud.points[cloudIndex].x = temp_x;
            tempRgbCloud.points[cloudIndex].y = temp_y;
            tempRgbCloud.points[cloudIndex].z = temp_z;
            tempRgbCloud.points[cloudIndex].rgb = *reinterpret_cast<float*>(&rgb);

            /***************统计均值 、方差相关***********************/
            if(statisticStartFlag == true)
            {
                //判断每个点已经储存的个数，如果已经超过设定的范围，则进行循环储存；
                int offset = tempStatisticPeakPoints[cloudIndex].size() - statisticFrameNumber;
                if(offset >= 0)
                {
                    tempStatisticTofPoints[cloudIndex].erase(tempStatisticTofPoints[cloudIndex].begin(),tempStatisticTofPoints[cloudIndex].begin() + offset + 1);
                    tempStatisticPeakPoints[cloudIndex].erase(tempStatisticPeakPoints[cloudIndex].begin(),tempStatisticPeakPoints[cloudIndex].begin() + offset + 1);
                }
                //向每个点的容器中添加一个新的点,完成循环存储
                tempStatisticTofPoints[cloudIndex].push_back(tof);
                tempStatisticPeakPoints[cloudIndex].push_back(intensity);
            }


            /******统计点云空间坐标最大值、最小值**********/
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
            qDebug()<<QStringLiteral("给像素赋值时出现异常 imgrow=")<<imgRow<<"   imgCol = "<<imgCol<<endl;

    }

    //    qDebug()<<QStringLiteral("处理时间为：")<<time.elapsed()<<endl;

    lastSpadNum = spadNum ;
}













/*************TCP 协议相关********************************************************************************************************************************/
void DealUsb_msg::linkServer()
{
    m_tcpSocket.connectToHost("127.0.0.1",6006);

    if(!m_tcpSocket.waitForConnected(300))
    {
        qDebug()<<"mTcpsocket  link error !"<<endl;
        return;
    }else
    {
        qDebug()<<"mTcpsocket  link success !"<<endl;
    }
    connect(&m_tcpSocket, SIGNAL(readyRead()), this, SLOT(ClientRecvData()),Qt::DirectConnection); //very important
}

//接收TCP 数据槽函数
void DealUsb_msg::ClientRecvData()  //接收点云数据的槽函数
{
    quint32 mesg_len=0;
    qint64  readbyte = m_tcpSocket.bytesAvailable();
    if(readbyte<=0)
    {
        qDebug()<<QString::fromLocal8Bit("接收到的数据长度为空!")<<endl;
        return;
    }
    QByteArray buffer;
    quint16 flag;   //包头的标识 2个字节
    quint32 len;    //包头报文长度，json字符串的长度，4个字节
    buffer = m_tcpSocket.readAll();
    m_buffer.append(buffer);
    int totallen = m_buffer.size();


    while(totallen)
    {
        if(totallen < 6)    //不足6个字节（头部）
        {
            qDebug()<<QString::fromLocal8Bit("bagHead less six byte!!!!!");
            break;
        }

        //先进行包头判断，0x5A 0x5A , 因为有可能接收到的不是完整的报文
        int index = m_buffer.indexOf("ZZ");
        if(index<0)   //没有找到包头
        {
            qDebug()<<" 没有找到包头 "<<endl;
            m_buffer.clear();
            totallen = m_buffer.size();
            break;
        }else if(index>0)   //包头前面存在冗余
        {
            qDebug()<<"index ="<<index<<endl;
            m_buffer = m_buffer.right(totallen - index);
            totallen = m_buffer.size();
        }


        QDataStream packet(m_buffer);
        packet>>flag>>len;       //获取长度 len  信息头暂时不用

        //        qDebug()<<" total ="<<totallen<<"   len="<<len<<endl;

        QByteArray json_Array;  //存储解析好的JSON 数据

        if(totallen-6>=len) //若是数据部分长度大于 或者等于 指定长度，说明后面可能有冗余数据
        {
            json_Array = m_buffer.mid(6,len);   //传递到下面做JSON数据解析
            m_buffer = m_buffer.right(totallen-6-len);
            totallen = m_buffer.size();
            //            qDebug()<<QString::fromLocal8Bit("data Receive is ok or too much , m_buffer = ")<<m_buffer.size()<<endl;

        }
        else        //说明此时数据不够,等待下一次处理
        {
            break;
        }


        //以上数据为数据预处理
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(json_Array, &jsonError);                     // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
        {                                                                                            // 解析未发生错误
            if (doucment.isObject()) {
                QJsonObject object = doucment.object();                                              // 转化为对象
                if (object.contains("table"))
                {                                                                                    // 包含指定的 key
                    QJsonValue val_flag = object.value("table");                                    // 获取指定 key 对应的 value
                    int flag = val_flag.toInt();
                    //                    qDebug() << "table " << flag;
                    if(1 == flag)              //query机型基本参数返回表
                    {
                        QJsonValue value_msg = object.value("msg");
                        if(value_msg.isArray())
                        {
                            QJsonArray msgArr = value_msg.toArray();
                            int size_ = msgArr.size();

                            //                            qDebug()<<" size  = "<<size_<<endl;

                            for(int i=0; i<size_; i++)
                            {

                                QJsonObject pointObject =  msgArr[i].toObject();
                                if (pointObject.contains("position") && pointObject.contains("counter"))
                                {
                                    int tof = pointObject.value("position").toInt();             // 获取指定 key 对应的 value
                                    int intensity = pointObject.value("counter").toInt();             // 获取指定 key 对应的 value
                                    //                                    int cloudIndex = i%64*256 + i/64;
                                    int cloudIndex = i;


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

                                    int imgRow = cloudIndex/256;
                                    int imgCol = cloudIndex%256;
                                    if(imgRow>=0 && imgRow<64 && imgCol>=0 && imgCol<256)
                                    {
                                        microQimage.setPixel(imgCol,imgRow,tofColor);         //TOF图像的赋值 x,y
                                        macroQimage.setPixel(imgCol,imgRow,intenColor);       //强度图像的赋值 x.y

                                        //鼠标显示tof peak的变量赋值   2019-8-21
                                        mouseShowMutex.lock();
                                        mouseShowTOF[imgCol][imgRow] = tof;
                                        mouseShowPEAK[imgCol][imgRow] = intensity;
                                        mouseShowMutex.unlock();


                                        //文件保存   2019-8-21
                                        if(formatFlag==2 && isSaveFlag == true)
                                        {
                                            tofPeakNum[cloudIndex] = QString::number(tof).append(", ").append(QString::number(intensity)).append("\n");
                                        }else
                                        {
                                            tofPeakNum[cloudIndex].clear();
                                        }


                                        /********点云相关*********获取三维坐标***************/
                                        if(isTOF)
                                        {
                                            temp_x = tof * x_Weight[cloudIndex] * LSB;
                                            temp_y = tof * y_Weight[cloudIndex] * LSB;
                                            //                                            temp_y = tof * LSB;
                                            temp_z = tof * z_Weight[cloudIndex] * LSB;

                                            QColor mColor = QColor(tofColor);
                                            r = mColor.red();
                                            g = mColor.green();
                                            b = mColor.blue();
                                            rgb = ((int)r << 16 | (int)g << 8 | (int)b);

                                        }else
                                        {
                                            temp_x = intensity * x_Weight[cloudIndex] * LSB;
                                            temp_y = intensity * y_Weight[cloudIndex] * LSB;
                                            //                                            temp_y = intensity * LSB;
                                            temp_z = intensity * z_Weight[cloudIndex] * LSB;


                                            QColor mColor = QColor(intenColor);
                                            r = mColor.red();
                                            g = mColor.green();
                                            b = mColor.blue();
                                            rgb = ((int)r << 16 | (int)g << 8 | (int)b);
                                        }
                                        //                                        cloutPoint.x = temp_x;
                                        //                                        cloutPoint.y = temp_y;
                                        //                                        cloutPoint.z = temp_z;

                                        //                                        //点云颜色
                                        //                                        QColor mColor = QColor(tofColor);
                                        //                                        r = mColor.red();
                                        //                                        g = mColor.green();
                                        //                                        b = mColor.blue();
                                        //                                        rgb = ((int)r << 16 | (int)g << 8 | (int)b);
                                        //                                        cloutPoint.rgb = *reinterpret_cast<float*>(&rgb);;
                                        //                                        tempRgbCloud.push_back(cloutPoint);

                                        tempRgbCloud.points[cloudIndex].x = temp_x;
                                        tempRgbCloud.points[cloudIndex].y = temp_y;
                                        tempRgbCloud.points[cloudIndex].z = temp_z;
                                        tempRgbCloud.points[cloudIndex].rgb = *reinterpret_cast<float*>(&rgb);



                                        //                                        qDebug()<<" cloud 's size ="<<tempRgbCloud.size()<<endl;

                                        /***************统计均值 、方差相关***********************/
                                        if(statisticStartFlag == true)
                                        {
                                            //判断每个点已经储存的个数，如果已经超过设定的范围，则进行循环储存；
                                            int offset = tempStatisticPeakPoints[cloudIndex].size() - statisticFrameNumber;
                                            if(offset >= 0)
                                            {
                                                tempStatisticTofPoints[cloudIndex].erase(tempStatisticTofPoints[cloudIndex].begin(),tempStatisticTofPoints[cloudIndex].begin() + offset + 1);
                                                tempStatisticPeakPoints[cloudIndex].erase(tempStatisticPeakPoints[cloudIndex].begin(),tempStatisticPeakPoints[cloudIndex].begin() + offset + 1);
                                            }
                                            //向每个点的容器中添加一个新的点,完成循环存储
                                            tempStatisticTofPoints[cloudIndex].push_back(tof);
                                            tempStatisticPeakPoints[cloudIndex].push_back(intensity);
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

                                    }else{
                                        qDebug()<<QStringLiteral("给像素赋值时出现异常 imgrow=")<<imgRow<<" imgCol = "<<imgCol<<endl;
                                    }


                                }//一组tof & peak值

                            } //for 多少个点

                            // m_data.resize(size_ * 6);
                        }     //if(value_msg.isArray())



                        /**********************2019-8-21  add********************************************/
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


                        //统计信息相关的 ，将统计信息的容器赋值给全局变量
                        if(statisticStartFlag)
                        {
                            statisticMutex.lock();
                            allStatisticTofPoints = tempStatisticTofPoints;
                            allStatisticPeakPoints = tempStatisticPeakPoints;
                            statisticMutex.unlock();
                        }


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
                        /************************************************************/


                        /**********一帧数据接收完毕**********/
                        /*                        mutex.lock();
                        tofImage = microQimage;
                        intensityImage = macroQimage;
                        pcl::copyPointCloud(tempRgbCloud,pointCloudRgb);
                        mutex.unlock();
                        isShowPointCloud = true;
*/
                        // 1、将滤波功能放到这里进行实现，
                        // 2、将滤波后的三维点云 同步到二维图像
                        if(true == isFilterFlag)
                        {
                            /*******************开启滤波功能*********************************/
                            //先用直通滤波把所有零点重复的零点过滤掉
                            pcl::PassThrough<pcl::PointXYZRGB> pass;                      //创建滤波器对象
                            pass.setInputCloud(tempRgbCloud.makeShared());                //设置待滤波的点云
                            pass.setFilterFieldName("y");                             //设置在Z轴方向上进行滤波
                            pass.setFilterLimits(0, 0.10);                               //设置滤波范围(从最高点向下0.10米去除)
                            pass.setFilterLimitsNegative(true);                       //保留
                            pass.filter(tempRgbCloud_pass);                                   //滤波并存储
                            if(tempRgbCloud_pass.size()<1)
                                return;

                            //  基于统计运算的滤波算法
                            //        DealedCloud_rgb.clear();
                            //        pcl::StatisticalOutlierRemoval<pcl::PointXYZRGB> sor;
                            //        sor.setInputCloud(tempRgbCloud_pass.makeShared());
                            //        sor.setMeanK(20);
                            //        sor.setStddevMulThresh(0);
                            //        //40  0.1 不见前面噪点
                            //        sor.filter(tempRgbCloud_radius);
                            //        qDebug()<<"after filter the points'Number = "<<DealedCloud_rgb.size()<<endl;


                            //条件滤波   设置半径 以及 圆周内的点数
                            tempRgbCloud_radius.resize(0);
                            pcl::RadiusOutlierRemoval<pcl::PointXYZRGB> outrem(true);      //设置为true以后才能获取到滤出的噪点的 个数以及点的序列号
                            outrem.setInputCloud(tempRgbCloud_pass.makeShared());              //设置输入点云
                            outrem.setRadiusSearch(0.25);              //设置在0.8半径的范围内找邻近点
                            outrem.setMinNeighborsInRadius(30);       //设置查询点的邻近点集数小于2的删除
                            outrem.filter (tempRgbCloud_radius);//执行条件滤波，存储结果到cloud_filtered
                            int len = outrem.getRemovedIndices()->size();
                            qDebug()<<"dealusb_msg    fileted size = "<<outrem.getRemovedIndices()->size();
                            /*************************以上为滤波处理部分************************************************************/

                            /***********************接下来 根据点云的序号 去除二维图像中的噪声************************/

                            int index,pix_x,pix_y;
                            QRgb clearTofCol,clearPeakCol;
                            for(int i=0; i<len; i++)
                            {
                                index = outrem.getRemovedIndices()->at(i);
                                pix_x = index % 256;
                                pix_y = index / 256;

                                //将像素去均值 后赋予噪点处的像素值
                                if(pix_y+1 < 64 && pix_y-1 >0)
                                {
                                    clearTofCol =  (microQimage.pixel(pix_x,pix_y+1) + microQimage.pixel(pix_x,pix_y-1))/2.0;
                                    clearPeakCol = (macroQimage.pixel(pix_x,pix_y+1) + macroQimage.pixel(pix_x,pix_y-1))/2.0;
                                }
                                else
                                {
                                    clearTofCol =  microQimage.pixel(pix_x,pix_y);
                                    clearPeakCol = macroQimage.pixel(pix_x,pix_y);
                                }
                                microQimage.setPixel(pix_x,pix_y,clearTofCol);
                                macroQimage.setPixel(pix_x,pix_y,clearPeakCol);


                            }



                            mutex.lock();
                            tofImage = microQimage;
                            intensityImage = macroQimage;
                            pcl::copyPointCloud(tempRgbCloud_radius,pointCloudRgb);
                            mutex.unlock();
                            /***************************************************************/

                        }else{                      //不进行滤波
                            mutex.lock();
                            tofImage = microQimage;
                            intensityImage = macroQimage;
                            pcl::copyPointCloud(tempRgbCloud,pointCloudRgb);
                            mutex.unlock();
                        }
                        isShowPointCloud = true;






                        //                        tempRgbCloud.clear();



                        //                        update();    //刷新OPENGL 显示
                    }   //if(1 == flag)

                }  //if (object.contains("@table"))

            }else     //不是Json对象
            {
                qDebug()<<QString::fromLocal8Bit("the data is not a json object");

                //                break;
            }
        }else        //documnt is not null
        {
            qDebug()<<QString::fromLocal8Bit("data analyze error,can't translate to QJsonDocument file ~~~ ");
            break;
        }

    }//while


}











/***************** read local_file *************************************/

void DealUsb_msg::selectLocalFile_slot(QString sPath)
{
    filePath = sPath;
    //    readLocalPCDFile();

    if( NULL == localFile_timer)
    {
        localFile_timer = new QTimer();
        connect(localFile_timer,SIGNAL(timeout()),this,SLOT(readLocalPCDFile()));
    }
    fileIndex = 1;
    localFile_timer->start(100);

}

void DealUsb_msg::readLocalPCDFile()
{
    //    int fileIndex = 1;
    QString fileName;
    QString line[17000];
    QStringList tofPeakList;
    int imgRow,imgCol;



    fileName = filePath + QString::number(fileIndex)+".txt";
    fileIndex++;

    QFile file(fileName);

    int countNum = 0;
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            line[countNum] = in.readLine();
            countNum++;
        }
        file.close();
    }else
    {
        qDebug()<<"read file error!"<<endl;
        //            localFile_timer->stop();
        fileIndex = 1;
        return;
    }
    for(int i=0; i<countNum-5; i++)            //去掉空的数据
    {
        int tof,intensity;
        tofPeakList = line[i].split(",");
        if(tofPeakList.size()<2)
            return;
        tof = tofPeakList[0].toInt();
        intensity = tofPeakList[1].toInt();

        //行列以及颜色传递给图像
        imgRow = i%256;
        imgCol = i/256;
        cloudIndex = i;      //在点云数据中的标号

        //            qDebug()<<"imgRow="<<imgRow<<"  imgCol="<<imgCol<<endl;

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


        if(imgRow>=0 && imgRow<256 && imgCol>=0 && imgCol<64)
        {
            microQimage.setPixel(imgRow,imgCol,tofColor);         //TOF图像的赋值
            macroQimage.setPixel(imgRow,imgCol,intenColor);       //强度图像的赋值

            /************鼠标点击处显示信息相关*************/
            mouseShowMutex.lock();
            mouseShowTOF[imgRow][imgCol] = tof;
            mouseShowPEAK[imgRow][imgCol] = intensity;
            mouseShowMutex.unlock();


            /************点云数据相关************/
            //获取三维坐标

            temp_x = tof * x_Weight[cloudIndex] * LSB;
            temp_y = tof * y_Weight[cloudIndex] * LSB;
            temp_z = tof * z_Weight[cloudIndex] * LSB;

            QColor mColor = QColor(tofColor);
            r = mColor.red();
            g = mColor.green();
            b = mColor.blue();
            rgb = ((int)r << 16 | (int)g << 8 | (int)b);


            tempRgbCloud.points[cloudIndex].x = temp_x;
            tempRgbCloud.points[cloudIndex].y = temp_y;
            tempRgbCloud.points[cloudIndex].z = temp_z;
            tempRgbCloud.points[cloudIndex].rgb = *reinterpret_cast<float*>(&rgb);

//            qDebug()<<" cloudIndex = "<<cloudIndex<<endl;

            /***************统计均值 、方差相关***********************/
            if(statisticStartFlag == true)
            {
                //判断每个点已经储存的个数，如果已经超过设定的范围，则进行循环储存；
                int offset = tempStatisticPeakPoints[cloudIndex].size() - statisticFrameNumber;
                if(offset >= 0)
                {
                    tempStatisticTofPoints[cloudIndex].erase(tempStatisticTofPoints[cloudIndex].begin(),tempStatisticTofPoints[cloudIndex].begin() + offset + 1);
                    tempStatisticPeakPoints[cloudIndex].erase(tempStatisticPeakPoints[cloudIndex].begin(),tempStatisticPeakPoints[cloudIndex].begin() + offset + 1);
                }
                //向每个点的容器中添加一个新的点,完成循环存储
                tempStatisticTofPoints[cloudIndex].push_back(tof);
                tempStatisticPeakPoints[cloudIndex].push_back(intensity);
            }


            /******统计点云空间坐标最大值、最小值**********/
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
            qDebug()<<QStringLiteral("给像素赋值时出现异常 imgrow=")<<imgRow<<"   imgCol = "<<imgCol<<endl;

    }  //一帧数据已经读取完成




    //统计信息相关的 ，将统计信息的容器赋值给全局变量
    if(statisticStartFlag)
    {
        statisticMutex.lock();
        allStatisticTofPoints = tempStatisticTofPoints;
        allStatisticPeakPoints = tempStatisticPeakPoints;
        statisticMutex.unlock();
    }


    //向主线程中发送最大值、最小值等统计信息
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






    /*******************对Tof 图像进行过滤0的滤波处理*******************************/
//    vector<int> numArray;
//    QRgb tofColor;
//    int gainIndex_tof;
//    int cloudIndex;
//    for(int imgX=0; imgX<256; imgX++)
//        for(int imgY=1; imgY<64; imgY++)
//        {
//            if(0 == mouseShowTOF[imgX][imgY])
//            {

//                if( imgX>=1 )
//                    numArray.push_back(mouseShowTOF[imgX-1][imgY]);
//                if( imgX<=254 )
//                    numArray.push_back(mouseShowTOF[imgX+1][imgY]);
//                if( imgY>=1)
//                    numArray.push_back(mouseShowTOF[imgX][imgY-1]);
//                if( imgY<=62)
//                    numArray.push_back(mouseShowTOF[imgX][imgY+1]);

//                sort(numArray.begin(),numArray.end());
//                int midNumTof = numArray[numArray.size()/2];                //存在bug  size=2时，  numArray[2]函数越界

//                //更新该点的值（鼠标显示）；
//                mouseShowTOF[imgX][imgY] =midNumTof;

//                //更新TOF 图像的像素值
//                gainIndex_tof = midNumTof*gainImage;
//                if(gainIndex_tof<1024 && gainIndex_tof>=0)
//                    tofColor = qRgb(colormap[gainIndex_tof * 3], colormap[gainIndex_tof * 3 + 1], colormap[gainIndex_tof * 3 + 2]);
//                else
//                    tofColor = qRgb(colormap[1023 * 3], colormap[1023 * 3 + 1], colormap[1023 * 3 + 2]);
//                microQimage.setPixel(imgX,imgY,tofColor);         //TOF图像的赋值

//                //更新点云数据的值 以及颜色
//                cloudIndex = imgY*256 + imgX;
//                temp_x = midNumTof * x_Weight[cloudIndex] * LSB;
//                temp_y = midNumTof * y_Weight[cloudIndex] * LSB;
//                temp_z = midNumTof * z_Weight[cloudIndex] * LSB;
//                QColor mColor = QColor(tofColor);
//                r = mColor.red();
//                g = mColor.green();
//                b = mColor.blue();
//                rgb = ((int)r << 16 | (int)g << 8 | (int)b);
//                tempRgbCloud.points[cloudIndex].x = temp_x;
//                tempRgbCloud.points[cloudIndex].y = temp_y;
//                tempRgbCloud.points[cloudIndex].z = temp_z;
//                tempRgbCloud.points[cloudIndex].rgb = *reinterpret_cast<float*>(&rgb);
//                numArray.clear();
//            }
//        }




    // 1、将滤波功能放到这里进行实现，
    // 2、将滤波后的三维点云 同步到二维图像
    if(true == isFilterFlag)
    {
        /*******************开启滤波功能*********************************/
        //先用直通滤波把所有零点重复的零点过滤掉
        //        pcl::PassThrough<pcl::PointXYZRGB> pass;                      //创建滤波器对象
        //        pass.setInputCloud(tempRgbCloud.makeShared());                //设置待滤波的点云
        //        pass.setFilterFieldName("y");                                 //设置在Z轴方向上进行滤波
        //        pass.setFilterLimits(0, 0.10);                                //设置滤波范围(从最高点向下0.10米去除)
        //        pass.setFilterLimitsNegative(true);                           //保留
        //        pass.filter(tempRgbCloud_pass);                                   //滤波并存储
        //        if(tempRgbCloud_pass.size()<1)
        //                return;

        //  基于统计运算的滤波算法
        tempRgbCloud_radius.clear();
        pcl::StatisticalOutlierRemoval<pcl::PointXYZRGB> outrem(true);
        outrem.setInputCloud(tempRgbCloud.makeShared());
        outrem.setMeanK(40);
        outrem.setStddevMulThresh(0.25);
        //40  0.1 不见前面噪点
        outrem.filter(tempRgbCloud_radius);
        int len = outrem.getRemovedIndices()->size();
        qDebug()<<"after filter the points'Number = "<<tempRgbCloud_radius.size()<<endl;


//        tempRgbCloud_radius.resize(0);
//        pcl::RadiusOutlierRemoval<pcl::PointXYZRGB> outrem(true);      //设置为true以后才能获取到滤出的噪点的 个数以及点的序列号
//        outrem.setInputCloud(tempRgbCloud.makeShared());              //设置输入点云
//        outrem.setRadiusSearch(0.25);              //设置在0.8半径的范围内找邻近点
//        outrem.setMinNeighborsInRadius(15);       //设置查询点的邻近点集数小于2的删除  30
//        outrem.filter (tempRgbCloud_radius);//执行条件滤波，存储结果到cloud_filtered
//        int len = outrem.getRemovedIndices()->size();

        /*************************以上为滤波处理部分************************************************************/

        /***********************接下来 根据点云的序号 去除二维图像中的噪声************************/

        int index,pix_x,pix_y;
        for(int i=0; i<len; i++)
        {
            index = outrem.getRemovedIndices()->at(i);
            pix_x = index % 256;
            pix_y = index / 256;
            mouseShowTOF[pix_x][pix_y] = 0;   //噪点赋值为黑色
        }

        //对噪点进行取中值的滤波处理
        vector<int> numArray;
        QRgb tofColor;
        int gainIndex_tof;
        for(int i=0;i<len; i++)
        {
            index = outrem.getRemovedIndices()->at(i);
            pix_x = index % 256;
            pix_y = index / 256;

            if( pix_x>=1 )
                numArray.push_back(mouseShowTOF[pix_x-1][pix_y]);
            if( pix_x<=254 )
                numArray.push_back(mouseShowTOF[pix_x+1][pix_y]);
            if( pix_y>=1)
                numArray.push_back(mouseShowTOF[pix_x][pix_y-1]);
            if( pix_y<=62)
                numArray.push_back(mouseShowTOF[pix_x][pix_y+1]);

            sort(numArray.begin(),numArray.end());
            int midNumTof = numArray[numArray.size()/2];
            //                qDebug()<<"midNumTof = "<<midNumTof<<endl;
            numArray.clear();

            //更新鼠标显示的TOF值
            mouseShowTOF[pix_x][pix_y] = midNumTof;

            //更新TOF 图像的像素值
            gainIndex_tof = midNumTof*gainImage;
            if(gainIndex_tof<1024 && gainIndex_tof>=0)
                tofColor = qRgb(colormap[gainIndex_tof * 3], colormap[gainIndex_tof * 3 + 1], colormap[gainIndex_tof * 3 + 2]);
            else
                tofColor = qRgb(colormap[1023 * 3], colormap[1023 * 3 + 1], colormap[1023 * 3 + 2]);
            microQimage.setPixel(pix_x,pix_y,tofColor);         //TOF图像的赋值
        }


        mutex.lock();
        tofImage = microQimage;
        intensityImage = macroQimage;
        pcl::copyPointCloud(tempRgbCloud_radius,pointCloudRgb);
        mutex.unlock();
        /***************************************************************/

    }else{                      //不进行滤波
        mutex.lock();
        tofImage = microQimage;
        intensityImage = macroQimage;
        pcl::copyPointCloud(tempRgbCloud,pointCloudRgb);
        mutex.unlock();
    }
    isShowPointCloud = true;


}




