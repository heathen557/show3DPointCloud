#include "recvpcdthread.h"
#include<QFile>
#include<QMessageBox>
#include<QJsonParseError>
#include<QJsonObject>
#include<QJsonArray>


recvPcdThread::recvPcdThread(QObject *parent) : QObject(parent)
{

//    readTimer.start(100);
//    connect(&readTimer,SIGNAL(timeout()),this,SLOT(readDataSlot()));
//    readTimer.start(100);




    //tcp
//    m_tcpSocket.connectToHost("10.0.1.221",6000);
  m_tcpSocket.connectToHost("127.0.0.1",6000);

    if(!m_tcpSocket.waitForConnected(300))
    {
//        QMessageBox::information(NULL,NULL "QT网络通信", "连接服务端失败！");
         QMessageBox::information(NULL,"TCP link","tcp link error");
//        return;
    }else
    {
        QMessageBox::information(NULL,"TCP link","tcp link OK");
    }
     connect(&m_tcpSocket, SIGNAL(readyRead()), this, SLOT(ClientRecvData()));

}


void recvPcdThread::ClientRecvData()
{

//    quint32 mesg_len=0;
//    qint64  readbyte = m_tcpSocket.bytesAvailable();
//    if(readbyte<=0)
//    {
//        qDebug()<<QString::fromLocal8Bit(" the msg is NULL ")<<endl;
//        return;
//    }
    QByteArray buffer;
    quint16 flag;   //包头的标识 2个字节
    quint32 len;    //包头报文长度，json字符串的长度，4个字节
    buffer = m_tcpSocket.readAll();
    m_buffer.append(buffer);
    int totallen = m_buffer.size();

    qDebug()<<"data ="<<m_buffer<<endl;

//    while(totallen)
//    {

//        if(totallen < 6)    //不足6个字节（头部）
//        {
//            qDebug()<<QString::fromLocal8Bit("bagHead less six byte!!!!!");
//            break;
//        }

//        //先进行包头判断，0x5A 0x5A , 因为有可能接收到的不是完整的报文
//        int index = m_buffer.indexOf("ZZ");
//        qDebug()<<"find the char index = "<< index<<endl;
//        if(index<0)   //没有找到包头
//        {
//            m_buffer.clear();
//            totallen = m_buffer.size();
//            break;
//        }else if(index>0)   //包头前面存在冗余
//        {
//            m_buffer = m_buffer.right(totallen - index);
//            totallen = m_buffer.size();
//        }

//        QDataStream packet(m_buffer);
//        packet>>flag>>len;       //获取长度 len  信息头暂时不用
////        qDebug()<<QString::fromLocal8Bit("json's data length = ")<<len  ;
//        QByteArray json_Array;  //存储解析好的JSON 数据

//        if(totallen-6>=len) //若是数据部分长度大于 或者等于 指定长度，说明后面可能有冗余数据
//        {
//            json_Array = m_buffer.mid(6,len);   //传递到下面做JSON数据解析
//            m_buffer = m_buffer.right(totallen-6-len);
//            totallen = m_buffer.size();
//            qDebug()<<QString::fromLocal8Bit("data Receive is ok or too much , m_buffer = ")<<m_buffer.size()<<endl;
//        }
//        else        //说明此时数据不够,等待下一次处理
//        {
//            qDebug()<<QString::fromLocal8Bit("data Receive is less ");
//            break;
//        }
//        //以上数据为数据预处理




//        QJsonParseError jsonError;
//        QJsonDocument doucment = QJsonDocument::fromJson(json_Array, &jsonError);                     // 转化为 JSON 文档
//        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
//        {                                                                                            // 解析未发生错误
//            if (doucment.isObject()) {
//                QJsonObject object = doucment.object();                                              // 转化为对象
//                if (object.contains("table"))
//                {                                                                                    // 包含指定的 key
//                    QJsonValue val_flag = object.value("table");                                    // 获取指定 key 对应的 value
//                    int flag = val_flag.toInt();
//                    qDebug() << "table " << flag;
//                    if(1 == flag)              //query机型基本参数返回表
//                    {
//                        QJsonValue value_msg = object.value("msg");
//                        if(value_msg.isArray())
//                        {
//                            QJsonArray msgArr = value_msg.toArray();
//                            int size_ = msgArr.size();

//                            int j = 0;
//                            for(int i=0; i<size_; i++)
//                            {

//                                  QJsonObject pointObject =  msgArr[i].toObject();
//                                  if (pointObject.contains("x") && pointObject.contains("y") && pointObject.contains("z"))
//                                  {
//                                      QJsonValue val_x = pointObject.value("x");             // 获取指定 key 对应的 value
//                                      float value_x = val_x.toDouble();

//                                      QJsonValue val_y = pointObject.value("y");             // 获取指定 key 对应的 value
//                                      float value_y= val_y.toDouble();

//                                      QJsonValue val_z = pointObject.value("z");             // 获取指定 key 对应的 value
//                                      float value_z= val_z.toDouble();

//                                      qDebug()<<"x="<<value_x<<" y="<<value_x<<"  z="<<value_z<<endl;

////                                      m_data[j] = value_x/10.0;
////                                      m_data[j+1] = value_y/10.0;
////                                      m_data[j+2] = value_z/10.0;

//                                      j += 6;

//                                  }

//                            }

////                            m_data.resize(size_ * 6);
//                        }     //if(value_msg.isArray())

////                        update();    //刷新OPENGL 显示
//                    }   //if(1 == flag)

//                }  //if (object.contains("@table"))

//            }else     //不是Json对象
//            {
//                qDebug()<<QString::fromLocal8Bit("the data is not a json object");

//                break;
//            }
//        }else        //documnt is not null
//        {
//            qDebug()<<QString::fromLocal8Bit("data analyze error,can't translate to QJsonDocument file ~~~ ");
//            break;
//        }

//    }//while
}




void recvPcdThread::run()
{
    qDebug()<<"read pcd has come in  run() "<<endl;
//     connect(&readTimer,SIGNAL(timeout()),this,SLOT(readDataSlot()));
//     readTimer.start(100);
}

void recvPcdThread::readDataSlot()
{

//    qDebug()<<"read pcd has come in "<<endl;
    QString fileName = "PCD2/position" + QString::number(index) + ".pcd";
    index++;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"readPCDFile read over!"<<endl;
         return;
    }

    uchar* fpr = file.map(0, file.size());
    int cnt = 0;
    int subcnt = 0;
    char *substr;
    char *s = strdup((char*)fpr);

    int i = 0;
    while(substr= strsep(&s, "\n"))
    {
        cnt++;
        if(cnt<12)
            continue;

        char *lineSubStr;
        subcnt = 0;
        while(subcnt < 3)
        {
            lineSubStr = strsep(&substr, " ");
            QString str = QString(lineSubStr);
//            m_data[i+subcnt] = str.toFloat()/100.0;
            subcnt++;
        }

        i += 6;
    }
     m_data.resize(cnt * 6);
     qDebug()<<"m_data's size  = "<<m_data.size()<<endl;
}


//字符串解析，读取pcd文件时候用
 char *recvPcdThread::strsep(char **s, const char *ct)
{
    char *sbegin = *s;
    char *end;

    if (sbegin == NULL)
        return NULL;

    end = strpbrk(sbegin, ct);
    if (end)
        *end++ = '\0';
    *s = end;
    return sbegin;
}
