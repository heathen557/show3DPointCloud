
#include "receusb_msg.h"
#include<QDataStream>


#define MY_CONFIG 1
#define MY_INTF 0



extern bool isShowPointCloud;
extern bool isWriteSuccess;    //写入命令是否成功标识
extern bool isRecvFlag;
extern int framePerSecond;




ReceUSB_Msg::ReceUSB_Msg(QObject *parent) : QObject(parent),
    microQimage(256,64, QImage::Format_RGB32),macroQimage(256,64, QImage::Format_RGB32),saveTofIntenImage(256,64, QImage::Format_RGB32)
{
    isFirstLink = true;
    isRecvFlag = false;

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

}

//查找是不是存在想要链接的USB设备
struct usb_device *ReceUSB_Msg::findUSBDev(const unsigned short idVendor,
                                           const unsigned short idProduct)
{
    struct usb_bus *bus;
    struct usb_device *dev;

    usb_find_busses(); /* find all busses */
    usb_find_devices(); /* find all connected devices */

    for(bus = usb_get_busses(); bus; bus = bus->next)
    {
        for(dev = bus->devices; dev; dev = dev->next)
        {
            if((dev->descriptor.idVendor == idVendor_)
                    && (dev->descriptor.idProduct == idProduct_))
            {
                return dev;
            }else
            {
                emit linkInfoSignal(1);  //没找到设备
            }
        }
    }
    return NULL;

}

//打开已经存在的USB设备
bool ReceUSB_Msg::openUSB(struct usb_device *dev)
{
    devOpenFlg = false;

    devHandle = usb_open(dev);
    if(!devHandle)
    {
        qDebug() << "error opening device: ";
        qDebug() << usb_strerror();
        return false;
    }
    else
    {
        qDebug() << "success: device " << "MY_VID" << " : "<<  "MY_PID" << " opened";
        devOpenFlg = true;
    }

    if (usb_set_configuration(devHandle, MY_CONFIG) < 0)
    {
        qDebug() << "error setting config #" << MY_CONFIG << " : " << usb_strerror();
        usb_close(devHandle);
        return false;
    }
    else
    {
        qDebug() << "success: set configuration #" << MY_CONFIG;
    }

    if (usb_claim_interface(devHandle, MY_INTF) < 0)
    {
        qDebug() << "error claiming interface #" << MY_INTF;
        qDebug() << usb_strerror();
        usb_close(devHandle);
        return false;
    }
    else
    {
        qDebug() <<  "success: claim_interface #" << MY_INTF;
    }

    return true;
}


//关闭USB链接
void ReceUSB_Msg::closeUSB()
{
    if(devHandle)
    {
        readTimer->stop();   //先关闭数据接收
        devOpenFlg = false;
        //        UsbListener::quit();
        int ret = usb_close(devHandle); // Exit Thread
        qDebug() << "Close USB Device [" << ret << "]";
        devHandle = NULL;
        isFirstLink = false;   //以后就不是第一次连接了
    }

}



//系统FPGA注册 读取
bool ReceUSB_Msg::System_Register_Read(int Address, QString &Data)
{

    bool res = true;
    int transLen = 0;
    struct usb_ctrl_setup Cmd;
    Cmd.bRequestType = 0xC0;
    Cmd.bRequest = 0x01;
    Cmd.wValue = 0x0000;
    Cmd.wIndex = Address;  //need change
    Cmd.wLength = 0x0001;
    char data[1];
    QString arr;
    res = res && usb_control_msg(devHandle,Cmd.bRequestType,Cmd.bRequest,Cmd.wValue,Cmd.wIndex,data,1,transLen);
//    Data = QString(data[0]);

    quint8 tmp = quint8(data[0]);
    Data = QString::number(tmp);

    return res;
}


//系统FPGA注册 写入
bool ReceUSB_Msg::System_Register_Write(int Address, QString &Data)
{
    bool res = true;
    int transLen = 0;
    char data[1];
    struct usb_ctrl_setup Cmd;
    Cmd.bRequestType = 0x40;
    Cmd.bRequest = 0x01;
    Cmd.wValue = 0x0000;
    Cmd.wIndex = Address;  // need change
    Cmd.wLength = 0x0001;
    data[0] = Data.toInt(NULL,16);    //need modify
    res = res && usb_control_msg(devHandle,Cmd.bRequestType,Cmd.bRequest,Cmd.wValue,Cmd.wIndex,data,1,transLen);

    return res;
}

//设备注册 读取
bool ReceUSB_Msg::Device_Register_Read(int slavedId,int Address,QString &Data)
{
    bool res = true;
    int transLen = 0;
    char data[1];
    struct usb_ctrl_setup Cmd;
    Cmd.bRequestType = 0x40;
    Cmd.bRequest = 0x01;
    Cmd.wValue = 0x0000;
    Cmd.wIndex = 0x00f1;
    Cmd.wLength = 0x0001;
    data[0]= slavedId;   //need modify
    res = res && usb_control_msg(devHandle,Cmd.bRequestType,Cmd.bRequest,Cmd.wValue,Cmd.wIndex,data,1,transLen);


    Cmd.wIndex = 0x00f5;
    data[0] = 0x33;
    res = res && usb_control_msg(devHandle,Cmd.bRequestType,Cmd.bRequest,Cmd.wValue,Cmd.wIndex,data,1,transLen);


    Cmd.wIndex = 0x00f2;
    data[0] = Address;      //need modify
    res = res && usb_control_msg(devHandle,Cmd.bRequestType,Cmd.bRequest,Cmd.wValue,Cmd.wIndex,data,1,transLen);


    Cmd.wIndex = 0x00f5;
    data[0] = 0xf9;
    res = res && usb_control_msg(devHandle,Cmd.bRequestType,Cmd.bRequest,Cmd.wValue,Cmd.wIndex,data,1,transLen);


    Cmd.bRequestType = 0xC0;
    Cmd.wIndex = 0x00f4;
    res = res && usb_control_msg(devHandle,Cmd.bRequestType,Cmd.bRequest,Cmd.wValue,Cmd.wIndex,data,1,transLen);

    quint8 tmp = quint8(data[0]);
    Data = QString::number(tmp);

    //此处返回的是  十进制数字
    return res;
}


//设备注册 写入
bool ReceUSB_Msg::Device_Register_Write(int slavedId,int Address,QString &Data)
{
    bool res = true;
    int transLen = 0;
    uchar data[1];
    struct usb_ctrl_setup Cmd;
    Cmd.bRequestType = 0x40;
    Cmd.bRequest = 0x01;
    Cmd.wValue = 0x0000;
    Cmd.wIndex = 0x00f1;
    Cmd.wLength = 0x0001;
    data[0] = slavedId;      //need modify
    res = res && usb_control_msg(devHandle,Cmd.bRequestType,Cmd.bRequest,Cmd.wValue,Cmd.wIndex,(char*)(&data[0]),1,transLen);

    Cmd.wIndex = 0x00f5;
    data[0] = 0x37;
    res = res && usb_control_msg(devHandle,Cmd.bRequestType,Cmd.bRequest,Cmd.wValue,Cmd.wIndex,(char*)(&data[0]),1,transLen);


    Cmd.wIndex = 0x00f2;
    data[0] = Address;      //need modify
    res = res && usb_control_msg(devHandle,Cmd.bRequestType,Cmd.bRequest,Cmd.wValue,Cmd.wIndex,(char*)(&data[0]),1,transLen);


    Cmd.wIndex = 0x00f3;

    int tmp = Data.toInt(NULL,16);
    data[0] = tmp;


    res = res && usb_control_msg(devHandle,Cmd.bRequestType,Cmd.bRequest,Cmd.wValue,Cmd.wIndex,(char*)(&data[0]),1,transLen);

    return res;
}




//读取USB设备数据
void ReceUSB_Msg::read_usb()
{
    int ret;
    char MyBuffer[4096];
    //批量读(同步)
    QByteArray mArray;

    while(isRecvFlag)
    {
        ret = usb_interrupt_read(devHandle,129,MyBuffer,sizeof(MyBuffer),3000);
        //    ret = usb_bulk_read(devHandle, 129, MyBuffer, sizeof(MyBuffer), 3000);       //此处延迟设置为3000，经过测试设置为1的时候，ret<0,程序报错退出

        if (ret < 0) {
            qDebug("**************************************************error reading:%s", usb_strerror());
            emit linkInfoSignal(2);  //  2:没有接收到数据
            readTimer->stop();
        }

        if(260 == ret)
        {
            mArray = QByteArray(MyBuffer,260);
            emit recvMsgSignal(mArray);




//            QDataStream out(&mArray,QIODevice::ReadWrite);
//            QString strHex;
//            while (!out.atEnd())
//            {
//                qint8 outChar=0;
//                out>>outChar;
//                QString str=QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));

//                if (str.length()>1)
//                {
//                    strHex+=str+" ";
//                }
//                else
//                {
//                    strHex+="0"+str+" ";
//                }
//            }
//            strHex = strHex.toUpper();

//            qDebug()<<QStringLiteral("原始数据为：")<<strHex<<endl;

            emit recvMsgSignal(mArray);


        }

    }


}

//接收主函数信号，开启连接
void ReceUSB_Msg::run(int vId, int pId)
{
    idVendor_ = vId;
    idProduct_ = pId;

    readTimer = new QTimer();
    connect(readTimer, SIGNAL(timeout()),this,SLOT(read_usb()));
    openLinkDevSlot();

}

//打开设备连接
void ReceUSB_Msg::openLinkDevSlot()
{
    usb_init();                                  /* initialize the library */


    //下面代码屏蔽，重新打开时会查询不到设备
    int numBus = usb_find_busses();              /* find all busses */
    qDebug()<<"thread NUMbUS = "<<numBus<<endl;

    int numDevs = usb_find_devices();           /* find all connected devices */
    qDebug()<<"numDevs = "<<numDevs<<endl;

    if(isFirstLink)
    {
        if(numBus <= 0 || numDevs<=0)
        {
            emit linkInfoSignal(1);                //没有发现设备
            return;
        }
    }


    dev = findUSBDev(0,0);                      //查找usb设备

    if(NULL == dev)
    {
        isFirstLink = false;
        return ;
    }

    if(true ==openUSB(dev))                               //打开USB设备
    {
        emit linkInfoSignal(0);                //打开设备成功
    }else{
        emit linkInfoSignal(3);                //打开设备失败
    }

}


//读取系统寄存器槽函数
void ReceUSB_Msg::readSysSlot(int address,bool recvFlag)
{
    QString array;
    //系统注册 读取  0x13 = 19,array返回值
    //    bool res = System_Register_Read(19,array);

    bool res = System_Register_Read(address,array);
    qDebug()<<"[R]sys Read array="<<array<<"   res="<<res<<endl;
    //系统注册 写入测试

    if(res)
    {
        emit reReadSysSignal(array);
        emit linkInfoSignal(4);
    }else
    {
        emit linkInfoSignal(5);
    }

    isRecvFlag = recvFlag;
    read_usb();

}

//写入系统寄存器槽函数
// 12：写入系统成功      13：写入系统失败
void ReceUSB_Msg::writeSysSlot(int addr,QString data,bool recvFlag)
{
    if(System_Register_Write(addr, data))
    {
        emit linkInfoSignal(12);
    }
    else
    {
        emit linkInfoSignal(13);
    }

    isRecvFlag = recvFlag;
    read_usb();
}

//读取设备寄存器槽函数
void ReceUSB_Msg::readDevSlot(int id,int address,bool recvFlag)
{
    QString data;

    if(true == Device_Register_Read(id, address, data))
    {
        emit reReadDevSignal(data);
        emit linkInfoSignal(6);
    }else
    {
        emit linkInfoSignal(7);
    }

    isRecvFlag = recvFlag;
    read_usb();


}

//写入设备寄存器槽函数
void ReceUSB_Msg::writeDevSlot(int slavId,int addr,QString data,bool recvFlag)
{
    if(true == Device_Register_Write(slavId,addr,data))
    {
        emit linkInfoSignal(14);
    }else
    {
        emit linkInfoSignal(15);
    }

    isRecvFlag = recvFlag;
    read_usb();

}

//加载配置集槽函数
void ReceUSB_Msg::loadSettingSlot(QString filePath,bool recvFlag)
{
    QFile file(filePath);
    QString line[20];

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        int i = 0;
        while (!in.atEnd())
        {
            line[i] = in.readLine();
            qDebug()<<"line["<<i<<"] = "<<line[i];
            i++;
        }
        file.close();
    }
    //line[0] 对应 str1;
    //line[1] 对应 str2;
    //line[2] 对应 str3;

    bool res = true;
    QString array;
    /**********************测试STR2******************************************/
    // 1、 0x11= 17  0x41=65
    //    QString str2 = "41 01 00";
    QString str2 = line[1];
    res = System_Register_Write(17,str2.mid(0,2));
    qDebug()<<"[w]sys write str2="<<str2.mid(0,2)<<"   res="<<res<<endl;

    // 0x12 = 18
    res = System_Register_Write(18,str2.mid(3,2));
    qDebug()<<"[w]sys write str2="<<str2.mid(3,2)<<"   res="<<res<<endl;

    //0xe2 = 226
    res = System_Register_Write(226,str2.mid(6,2));
    qDebug()<<"[w]sys write str2="<<str2.mid(6,2)<<"   res="<<res<<endl;

    //0x13 = 19
    res = System_Register_Read(19,array);
    qDebug()<<"[R]sys Read array="<<array<<"   res="<<res<<endl;


    /************************测试STR3********************************************/
    //    QString str3 = "00 00 26 0A 00 64 00 14 00 01 00 00";   //len = 12
    QString str3 = line[2];

    //起始位置从32 开始
    for(int i=0 ; i<12; i++)
    {
        res = System_Register_Write(32+i, str3.mid(i*3,2));
        qDebug()<<"[w]sys write str3="<<str3.mid(i*3,2)<<"   res="<<res<<endl;
    }


    /*************************测试STR1******************************************************/
    //    QString str1 = "00 44 1F 44 45 44 EE 02 64 11 22 44 88 88 44 22 11 03 40 00 1F E0 81 4A 84 08 00 00 CC 01 00 00 00 00 00 00 00 0A 06 06 06 06 06 34 FF FF FF FF 04 1E";
    QString str1 = line[0];


    //0xd8 = 216,
    for(int k=1; k<50; k++)
    {
        res = Device_Register_Write(216,k,str1.mid(3*k,2));
        qDebug()<<"[w]Device write str1="<<str1.mid(3*k,2)<<"   res="<<res<<str1.mid(3*k,2).toInt(NULL,16) <<endl;
    }

    QString str = "00";                                       //2019-8-15
    res = Device_Register_Write(216,0,str);
    qDebug()<<"[w]Device write str1="<<"00"<<"   res="<<res<<endl;

    if(res)
    {
        emit linkInfoSignal(8);
    }else
    {
        emit linkInfoSignal(9);
    }

    /************************开始接受数据****************************************************/
    //    readTimer->start(1);
    isRecvFlag =true;
    read_usb();
}

//保存配置集槽函数
// deviceId转换过来的是十进制数  D8->216
void ReceUSB_Msg::saveSettingSlot(QString filePath,int deviceId,bool recvFlag)
{
    qDebug()<<" setting fileNamePath = "<< filePath<<"  deviceId ="<<deviceId<<endl;

    int i=0;
    bool res;
    QString dataStr;
    QString textString;

    QByteArray ba;
    const char *c_str;
    int m;
    /***************文本中写入str1******************************************/
    for(; i<50 ; i++)
    {
        res = Device_Register_Read(deviceId, i, dataStr);
        m = dataStr.toInt();
        QString tmpData = QString("%1 ").arg(m,2,16,QLatin1Char('0')).toUpper();
        textString.append(tmpData);
        qDebug()<<" the data =  "<<m<<"    sixteen's num ="<< tmpData<<endl;

    }
    QFile file(filePath);
    file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
    QTextStream out(&file);
    out<<textString.toLocal8Bit()<<endl;


    /***************文本中写入str2*****************************************************/
    res = System_Register_Read(17, dataStr);
//    ba = dataStr.toLatin1();
//    c_str = ba.data();
//    m = uint8_t(c_str[0]);
    m = dataStr.toInt();
    QString tmpData = QString("%1 ").arg(m,2,16,QLatin1Char('0')).toUpper();
    textString = tmpData;

    res = System_Register_Read(18, dataStr);
//    ba = dataStr.toLatin1();
//    c_str = ba.data();
//    m = uint8_t(c_str[0]);
    m = dataStr.toInt();
    tmpData = QString("%1 ").arg(m,2,16,QLatin1Char('0')).toUpper();
    textString.append(tmpData);


    res = System_Register_Read(226, dataStr);
//    ba = dataStr.toLatin1();
//    c_str = ba.data();
//    m = uint8_t(c_str[0]);
    m = dataStr.toInt();
    tmpData = QString("%1 ").arg(m,2,16,QLatin1Char('0')).toUpper();
    textString.append(tmpData);

    out<<textString.toLocal8Bit()<<endl;


    /***************文本中写入str3*****************************************************/
    textString.clear();
    for (i = 0; i < 12; i++)
    {
        res = System_Register_Read((32+i), dataStr);
//        ba = dataStr.toLatin1();
//        c_str = ba.data();
//        m = uint8_t(c_str[0]);
        m = dataStr.toInt();
        tmpData = QString("%1 ").arg(m,2,16,QLatin1Char('0')).toUpper();
        textString.append(tmpData);
    }
    out<<textString.toLocal8Bit()<<endl;



    file.close();
    if(recvFlag)
    {
        isRecvFlag = true;
        qDebug()<<"saveSettingSlot has set the recvFlag =  true"<<endl;
        read_usb();


    }

    if(res)
    {
        linkInfoSignal(10);
    }else
    {
        linkInfoSignal(11);
    }

}






