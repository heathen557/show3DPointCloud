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
extern float gainImage;   //增益

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
    gainImage = 1;

    haveIndex = 0;

    //    linkServer();

    peakOffset = 0;   //设置为阈值，小于这个值的认为是无效数据，将接收到的tof值设置为0  ::此功能预留，面阵_1028效果较好，但是对其他数据会滤掉大部分有效数据
    isOnlyCenterShow_flag = false;   //是否只显示中心区域的标识，设置为true则只显示中心光较强的区域（超过范围的点xyz坐标全部设置为0），设置为false则显示全部点云数据；默认false;



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



void DealUsb_msg::loadLocalArray()
{
    //加载数据的配置四个相关的配置矩阵的部分

    //加载tofOffsetArray.txt配置集
    QFile tofOffsetArray_file("tofOffsetArray.txt");
    QString tofOffsetArray_line[16400];
    QString str;
    if (tofOffsetArray_file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&tofOffsetArray_file);
        int i = 0;
        while (!in.atEnd())
        {
            tofOffsetArray_line[i] = in.readLine();
            i++;
        }
        tofOffsetArray_file.close();

        if(i<16383)
        {
            str = "load the 'tofOffsetArray.txt' error!";
            emit loadArrayFileSignal(false,str);
        }else{
            str = "load the 'tofOffsetArray.txt' success!";
            emit loadArrayFileSignal(true,str);
        }
    }else{
        str = "load the 'tofOffsetArray.txt' error!";
        emit loadArrayFileSignal(false,str);
    }



    //加载thetaArray.txt配置集
    QFile thetaArray_file("thetaArray.txt");
    QString thetaArray_line[16400];
    if (thetaArray_file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&thetaArray_file);
        int i = 0;
        while (!in.atEnd())
        {
            thetaArray_line[i] = in.readLine();
            i++;
        }
        thetaArray_file.close();

        if(i<16383)
        {
            str = "load the 'thetaArray.txt' error!";
            emit loadArrayFileSignal(false,str);
        }else{
            str = "load the 'thetaArray.txt success!'";
            emit loadArrayFileSignal(true,str);
        }
    }else{
        str = "load the 'thetaArray.txt' error!";
        emit loadArrayFileSignal(false,str);
    }


    //加载betaArray.txt配置集
    QFile betaArray_file("betaArray.txt");
    QString betaArray_line[16400];
    if (betaArray_file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&betaArray_file);
        int i = 0;
        while (!in.atEnd())
        {
            betaArray_line[i] = in.readLine();
            i++;
        }
        betaArray_file.close();

        if(i<16383)
        {
            str = "load the 'betaArray.txt' error!";
            emit loadArrayFileSignal(false,str);
        }else {
            str = "load the 'betaArray.txt' success!";
            emit loadArrayFileSignal(true,str);
        }
    }else{
        str = "load the 'betaArray.txt' error!";
        emit loadArrayFileSignal(false,str);
    }



    for(int i=0;i<16384;i++)
    {
        tofOffsetArray[i] = tofOffsetArray_line[i].toFloat();
        thetaArray[i] = thetaArray_line[i].toFloat();
        betaArray[i] = betaArray_line[i].toFloat();

//        qDebug()<<"thetaArray["<<i<<"] = "<<thetaArray[i]<<endl;
    }
}




//修改统计帧数的槽函数
void DealUsb_msg::alterStatisticFrameNum_slot(int num)
{
    statisticFrameNumber = num ;

}

//修改是否进行滤波的槽函数
void DealUsb_msg::isFilter_slot(bool isFiter)
{
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
//     qDebug()<<"spadNum = "<<spadNum<<"  line_number = "<<line_number<<endl;     //打包发布会导致打包发布后的程序卡顿

    if(line_number!=0  && ( (lastLineNum+1) != line_number) )
    {
        qDebug()<<"lastLineNum="<<lastLineNum <<"   "<<line_number<<endl;
    }
    lastLineNum = line_number;


    if(spadNum==0 && lastSpadNum==7)  //此时说明上一帧数据已经接收完毕，把整帧数据付给其他线程，供其显示，数据可以显示了
    {

        haveIndex++;    //满足 足够一帧的数据了

        //判断是否保存数据部分
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
                    {
                        flag =false;
//                        tofPeakNum[i] = QString::number(0).append(", ").append(QString::number(0)).append("\n");    //把没有接收到的数据设置为0 并且保存  2019-10-16 华为需求更改
                        tofPeakNum[i] = QString("%1").arg(0, 5, 10, QChar('0')).append(",").append(QString("%1").arg(0, 5, 10, QChar('0'))).append("\n");    //把没有接收到的数据设置为0 并且保存  2019-10-16 华为需求更改
                    }
                    tofPeakToSave_string.append(tofPeakNum[i]);
                    tofPeakNum[i].clear();
                }

//                if(flag)
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



        // 1、将滤波功能放到这里进行实现，
        // 2、将滤波后的三维点云 同步到二维图像
        if(true == isFilterFlag)
        {
            /*******************开启滤波功能*********************************/
            //先用直通滤波把所有零点重复的零点过滤掉
            pcl::PassThrough<pcl::PointXYZRGB> pass;                      //创建滤波器对象
            pass.setInputCloud(tempRgbCloud.makeShared());                //设置待滤波的点云
            pass.setFilterFieldName("y");                                 //设置在Z轴方向上进行滤波
            pass.setFilterLimits(0, 0.10);                                //设置滤波范围(从最高点向下0.10米去除)
            pass.setFilterLimitsNegative(true);                           //保留
            pass.filter(tempRgbCloud_pass);                                   //滤波并存储
            if(tempRgbCloud_pass.size()<1)
                return;

            //统计
            tempRgbCloud_radius.clear();
            pcl::StatisticalOutlierRemoval<pcl::PointXYZRGB> outrem(true);
            outrem.setInputCloud(tempRgbCloud_pass.makeShared());
            outrem.setMeanK(40);
            outrem.setStddevMulThresh(0.25);
            //40  0.1 不见前面噪点
            outrem.filter(tempRgbCloud_radius);
            int len = outrem.getRemovedIndices()->size();
//            qDebug()<<"after filter the points'Number = "<<tempRgbCloud_radius.size()<<endl;

            //            QTime time;
            //            time.start();
            //            int len;
//            tempRgbCloud_radius.resize(0);
//            pcl::RadiusOutlierRemoval<pcl::PointXYZRGB> outrem(true);      //设置为true以后才能获取到滤出的噪点的 个数以及点的序列号
//            outrem.setInputCloud(tempRgbCloud.makeShared());              //设置输入点云
//            outrem.setRadiusSearch(0.25);              //设置在0.8半径的范围内找邻近点
//            outrem.setMinNeighborsInRadius(15);       //设置查询点的邻近点集数小于2的删除  30
//            outrem.filter (tempRgbCloud_radius);//执行条件滤波，存储结果到cloud_filtered
//            int len = outrem.getRemovedIndices()->size();


            //条件滤波   设置半径 以及 圆周内的点数
            //            qDebug()<<"dealusb_msg    fileted size = "<<outrem.getRemovedIndices()->size();
            //            qDebug()<<" passThrough cost time = "<<time.elapsed()<<endl;
            /*************************以上为滤波处理部分************************************************************/


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
    }  //以上为处理完整的一帧数据*********************************************************





    int line_offset = spadNum / 2;           //取值 0 1 2 3 ；
    int row_offset = (spadNum + 1) % 2;      //表示是在第一行 还是在第二行

    for(int i=0; i<64; i++)
    {

        imgRow = i * 4 + line_offset;
        imgCol = line_number * 2 + row_offset;
        cloudIndex = imgCol*256+imgRow;      //在点云数据中的标号
        int tof,intensity ,tmpTof;     //tof：是用来做显示（二维、三维、最大最小值）（因为涉及到要进行校正）    tmpTof：用来存储本地数据 以及统计界面时候用

        if(isTOF == false)   //设置一个不可能的值
        {
            tof = quint8(MyBuffer[4 + i * 4]) + ((quint8(MyBuffer[4 + i * 4 +1]))<<8);
            intensity = quint8(MyBuffer[4 + i * 4 + 2]) + ((quint8(MyBuffer[4 + i * 4 + 3 ]))<<8);
        }else
        {
            intensity = quint8(MyBuffer[4 + i * 4]) + ((quint8(MyBuffer[4 + i * 4 +1]))<<8);
            tof = quint8(MyBuffer[4 + i * 4 + 2]) + ((quint8(MyBuffer[4 + i * 4 + 3 ]))<<8);
        }

        //这个是和90度直角矫正相关的  减去一个偏移量70  ；把处理之后小于0的值都过滤掉
//        tof = tof -70;
        tmpTof = tof;

//        tof =tof +32;
        tof = tof + tofOffsetArray[cloudIndex];




        //设置TOF图像、强度图像的颜色
        QRgb tofColor,intenColor;
        int gainIndex_tof = tof*gainImage;
        gainIndex_tof = gainIndex_tof>0 ?gainIndex_tof:0;
        int gainIndex_intensity =intensity * gainImage;
        gainIndex_intensity = gainIndex_intensity>0?gainIndex_intensity:0;
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
//                tofPeakNum[cloudIndex] = QString::number(tof).append(", ").append(QString::number(intensity)).append("\n");
                tofPeakNum[cloudIndex] = QString("%1").arg(tmpTof, 5, 10, QChar('0')).append(",").append(QString("%1").arg(intensity, 5, 10, QChar('0'))).append("\n");

            }else
            {
                tofPeakNum[cloudIndex].clear();

            }


            /************点云数据相关************/
            //获取三维坐标
//            temp_x = tof * x_Weight[cloudIndex] * LSB;
//            temp_y = tof * y_Weight[cloudIndex] * LSB;
//            temp_z = tof * z_Weight[cloudIndex] * LSB;

            //这部分是tof到三维点云的转换
            Lr =  (tof*tof - (5/1.5)*(5/1.5))/(2*(tof + (5/1.5)*sin(thetaArray[cloudIndex]))) * LSB;      //
            Lr = Lr<0?0:Lr;
            temp_x = Lr *  sin(thetaArray[cloudIndex]);                                   //  x坐标值
            temp_z = Lr *  cos(thetaArray[cloudIndex]) * sin(betaArray[cloudIndex]);     //  y坐标值
            temp_y = Lr *  cos(thetaArray[cloudIndex]) * cos(betaArray[cloudIndex]);      // z坐标值
//            if(imgRow>=78 && imgRow<=178)
//            {
//                temp_y = temp_y + B_Array[imgRow-78]*LSB;
//            }

            if(tofOffsetArray[cloudIndex] ==tof)     //tof 原始值为0 处的位置会 显示成为一个弧度,所以将这里的三维点云坐标置为0
            {
                temp_x = 0;
                temp_z = 0;
                temp_y = 0;
            }

            if(intensity <peakOffset)
            {
                temp_x = 0;
                temp_z = 0;
                temp_y = 0;
            }


//            //这里是只显示中间光强度比较大的区域 显示行数：12-52   显示列数：78-178
            if(isOnlyCenterShow_flag)
            {
                if(imgCol<12 || imgCol>52 || imgRow<78 || imgRow>178)
                {
                    temp_x = 0;
                    temp_y = 0;
                    temp_z = 0;
                }
            }


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
                tempStatisticTofPoints[cloudIndex].push_back(tmpTof);
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
//                                            temp_x = tof * x_Weight[cloudIndex] * LSB;
//                                            temp_y = tof * y_Weight[cloudIndex] * LSB;
//                                                                                        temp_y = tof * LSB;
//                                            temp_z = tof * z_Weight[cloudIndex] * LSB;

                                            QColor mColor = QColor(tofColor);
                                            r = mColor.red();
                                            g = mColor.green();
                                            b = mColor.blue();
                                            rgb = ((int)r << 16 | (int)g << 8 | (int)b);

                                        }else
                                        {
//                                            temp_x = intensity * x_Weight[cloudIndex] * LSB;
//                                            temp_y = intensity * y_Weight[cloudIndex] * LSB;
                                            //                                            temp_y = intensity * LSB;
//                                            temp_z = intensity * z_Weight[cloudIndex] * LSB;


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











/********************************* **************** ****************  read local_file ************************ **************** **************** **************** *****************************/

void DealUsb_msg::selectLocalFile_slot(QString sPath)
{
    filePath = sPath;

    if( NULL == localFile_timer)
    {
        localFile_timer = new QTimer();
        connect(localFile_timer,SIGNAL(timeout()),this,SLOT(readLocalPCDFile()));
    }
    fileIndex = 1;
    localFile_timer->start(80);

}

void DealUsb_msg::readLocalPCDFile()
{
    //    int fileIndex = 1;
    QString fileName;
    QString line[17000];
    QStringList tofPeakList;
    int imgRow,imgCol;


    haveIndex++;

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
    for(int i=0; i<countNum; i++)            //去掉空的数据
    {
        int tof,intensity,tmpTof;   //tof:用来显示（二维、三维、鼠标点击、最大最小值）,因为涉及到要减去一个偏移；     tmpTof：用来存储原始数据、以及统计界面的显示
        if(line[i].isEmpty())
            continue;

        tofPeakList = line[i].split(",");
        if(tofPeakList.size()<2)
            return;

        if(isTOF)
        {
            tof = tofPeakList[0].toInt();


//            //循环赋值    100帧数据取平均值
//            for(int n=0; n<99; n++)
//            {
//                lastTOF[n][i] = lastTOF[n+1][i];
//            }
//            lastTOF[99][i] = tof;

//            if(haveIndex >100)
//            {
//                float zeroNum = 0;
//                haveIndex = 120;
//                float allTof_100 = 0;
//                for(int k=0; k<100; k++)     //100帧取平均   ，如果有0的数据则不进行平均处理
//                {
//                    if(lastTOF[k][i] == 0)
//                    {
//                        zeroNum = zeroNum+1;
//                    }
//                    allTof_100 += lastTOF[k][i];
//                }

//               tof = allTof_100/(100.0-zeroNum);
//            }



//            qDebug()<<"here------------"<<endl;
            intensity = tofPeakList[1].toInt();
        }else
        {
           intensity = tofPeakList[0].toInt();
           tof = tofPeakList[1].toInt();
        }




        tmpTof = tof;
//        tof = tof - 70;

        tof = tof + tofOffsetArray[cloudIndex];
//        tof = tof<0? 0:tof;


        //行列以及颜色传递给图像
        imgRow = i%256;
        imgCol = i/256;
        cloudIndex = i;      //在点云数据中的标号

        //            qDebug()<<"imgRow="<<imgRow<<"  imgCol="<<imgCol<<endl;

        //设置TOF图像、强度图像的颜色
        QRgb tofColor,intenColor;
        int gainIndex_tof = tof*gainImage;
        int gainIndex_intensity =intensity * gainImage;

//        int gainIndex_tof = tof *0.5;
//        int gainIndex_intensity = intensity * 0.5;

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

//            temp_x = tof * x_Weight[cloudIndex] * LSB;
//            temp_y = tof * y_Weight[cloudIndex] * LSB;
//            temp_z = tof * z_Weight[cloudIndex] * LSB;


            Lr =  (tof*tof - (5/1.5)*(5/1.5))/(2*(tof + (5/1.5)*sin(thetaArray[cloudIndex]))) *LSB ;      //
            Lr = Lr<0?0:Lr;
            temp_x = Lr * sin(thetaArray[cloudIndex]);                                   //  x坐标值
            temp_z = Lr *  cos(thetaArray[cloudIndex]) * sin(betaArray[cloudIndex]);     //  y坐标值
            temp_y = Lr *  cos(thetaArray[cloudIndex]) * cos(betaArray[cloudIndex]);      // z坐标值
//            if(imgRow>=78 && imgRow<=178)
//            {
//                temp_y = temp_y + B_Array[imgRow-78]*LSB;
//            }

            if(isOnlyCenterShow_flag)
            {
                if(imgCol<12 || imgCol>52 || imgRow<78 || imgRow>178)
                {
                    temp_x = 0;
                    temp_y = 0;
                    temp_z = 0;
                }
            }


            QColor mColor = QColor(tofColor);
            r = mColor.red();
            g = mColor.green();
            b = mColor.blue();
            rgb = ((int)r << 16 | (int)g << 8 | (int)b);


            if(intensity < peakOffset)
            {
                temp_x = 0;
                temp_z = 0;
                temp_y = 0;
            }

            if(tofOffsetArray[cloudIndex] == tof)
            {
                temp_x = 0;
                temp_z = 0;
                temp_y = 0;
            }

            tempRgbCloud.points[cloudIndex].x = temp_x;
            tempRgbCloud.points[cloudIndex].y = temp_y;
            tempRgbCloud.points[cloudIndex].z = temp_z;
            tempRgbCloud.points[cloudIndex].rgb = *reinterpret_cast<float*>(&rgb);

//            qDebug()<<" cloudIndex = "<<cloudIndex<<endl;




            /*********文件保存相关*****************/
            if(formatFlag ==2  && isSaveFlag == true)
            {
//                tofPeakNum[cloudIndex] = QString::number(tof).append(", ").append(QString::number(intensity)).append("\n");
                tofPeakNum[cloudIndex] = QString("%1").arg(tmpTof, 5, 10, QChar('0')).append(",").append(QString("%1").arg(intensity, 5, 10, QChar('0'))).append("\n");
            }else
            {
                tofPeakNum[cloudIndex].clear();

            }




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
                tempStatisticTofPoints[cloudIndex].push_back(tmpTof);
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




    /*********************test saveFile HUAWEI ***********************************************/

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
                {
                    flag =false;
//                    tofPeakNum[i] = QString::number(0).append(", ").append(QString::number(0)).append("\n");    //把没有接收到的数据设置为0 并且保存  2019-10-16 华为需求更改

                    tofPeakNum[i] = QString("%1").arg(0, 5, 10, QChar('0')).append(",").append(QString("%1").arg(0, 5, 10, QChar('0'))).append("\n");   //把没有接收到的数据设置为0 并且保存  2019-10-16 华为需求更改


                }
                tofPeakToSave_string.append(tofPeakNum[i]);
                tofPeakNum[i].clear();
            }

//                if(flag)
                emit saveTXTSignal(tofPeakToSave_string);
            tofPeakToSave_string.clear();
        }
    }

    /**********************************************************************/







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





    // 1、将滤波功能放到这里进行实现，
    // 2、将滤波后的三维点云 同步到二维图像
    if(true == isFilterFlag)
    {
        /*******************开启滤波功能*********************************/
        //先用直通滤波把所有零点重复的零点过滤掉
        pcl::PassThrough<pcl::PointXYZRGB> pass;                      //创建滤波器对象
        pass.setInputCloud(tempRgbCloud.makeShared());                //设置待滤波的点云
        pass.setFilterFieldName("y");                                 //设置在Z轴方向上进行滤波
        pass.setFilterLimits(0, 0.10);                                //设置滤波范围(从最高点向下0.10米去除)
        pass.setFilterLimitsNegative(true);                           //保留
        pass.filter(tempRgbCloud_pass);                                   //滤波并存储
        if(tempRgbCloud_pass.size()<1)
                return;
////        qDebug()<<"after PassThrough filter the points'Number = "<<tempRgbCloud_radius.size()<<endl;

//        //  统计
//        tempRgbCloud_radius.clear();
//        pcl::StatisticalOutlierRemoval<pcl::PointXYZRGB> outrem(false);
//        outrem.setInputCloud(tempRgbCloud_pass.makeShared());
//        outrem.setMeanK(40);
//        outrem.setStddevMulThresh(0.25);
//        //40  0.1 不见前面噪点
//        outrem.filter(tempRgbCloud_radius);
//        int len = outrem.getRemovedIndices()->size();
////        qDebug()<<"after filter the points'Number = "<<tempRgbCloud_radius.size()<<endl;
///
        tempRgbCloud_radius.clear();
        pcl::StatisticalOutlierRemoval<pcl::PointXYZRGB> outrem(true);
        outrem.setInputCloud(tempRgbCloud_pass.makeShared());
        outrem.setMeanK(10);
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


        mutex.lock();
        tofImage = microQimage;
        intensityImage = macroQimage;

//        tofImage = *blur(&microQimage);
//        intensityImage = *blur(&macroQimage);

        pcl::copyPointCloud(tempRgbCloud_radius,pointCloudRgb);
        mutex.unlock();




//        pcl::getMinMax3D(*cloud,min,max);

        /***************************************************************/

    }else{                      //不进行滤波
        mutex.lock();
        tofImage = microQimage;
        intensityImage = macroQimage;



        pcl::copyPointCloud(tempRgbCloud,pointCloudRgb);
        mutex.unlock();


//        pcl::PointXYZRGB tmp_min;
//        pcl::PointXYZRGB tmp_max;
//        pcl::getMinMax3D(pointCloudRgb,tmp_min,tmp_max);

//        qDebug()<<"y_max ="<<tmp_max.y<<endl;
    }
    isShowPointCloud = true;


}



QImage *DealUsb_msg::blur(QImage *origin)
{
//    QImage * newImage = new QImage(*origin);
//    int kernel [3][3]= {{5,4,3},
//                        {4,0,0},
//                        {3,0,0}};
//    int kernelSize = 3;
//    int sumKernel = 19;
//    int r,g,b;
//    QColor color;
//    for(int x=kernelSize/2; x<newImage->width()-(kernelSize/2); x++){
//        for(int y=kernelSize/2; y<newImage->height()-(kernelSize/2); y++){

//            r = 0;
//            g = 0;
//            b = 0;

//            for(int i = -kernelSize/2; i<= kernelSize/2; i++)
//            {
//                for(int j = -kernelSize/2; j<= kernelSize/2; j++)
//                {
//                    color = QColor(origin->pixel(x+i, y+j));
//                    r += color.red()*kernel[kernelSize/2+i][kernelSize/2+j];
//                    g += color.green()*kernel[kernelSize/2+i][kernelSize/2+j];
//                    b += color.blue()*kernel[kernelSize/2+i][kernelSize/2+j];
//                }
//            }
//            r = qBound(0, r/sumKernel, 255);
//            g = qBound(0, g/sumKernel, 255);
//            b = qBound(0, b/sumKernel, 255);
//            newImage->setPixel(x,y, qRgb(r,g,b));
//        }
//    }
//    return newImage;


    QImage * newImage = new QImage(*origin);
    vector<int> numArray;
    QRgb tofColor;
    int gainIndex_tof;
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
//                    int midNumTof = numArray[numArray.size()/2];                //存在bug  size=2时，  numArray[2]函数越界
                int midNumTof = numArray[1];                //存在bug  size=2时，  numArray[2]函数越界

                //更新该点的值（鼠标显示）；
                mouseShowTOF[imgX][imgY] =midNumTof;

                //更新TOF 图像的像素值
                gainIndex_tof = midNumTof*gainImage;
                if(gainIndex_tof<1024 && gainIndex_tof>=0)
                    tofColor = qRgb(colormap[gainIndex_tof * 3], colormap[gainIndex_tof * 3 + 1], colormap[gainIndex_tof * 3 + 2]);
                else
                    tofColor = qRgb(colormap[1023 * 3], colormap[1023 * 3 + 1], colormap[1023 * 3 + 2]);
                newImage->setPixel(imgX,imgY,tofColor);         //TOF图像的赋值
                numArray.clear();
            }
        }
    return newImage;

}



