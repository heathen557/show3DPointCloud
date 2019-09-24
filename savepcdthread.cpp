#include "savepcdthread.h"
#include<QDebug>
#include<QMutex>
#include<QFile>

QMutex saveMutex;
QString saveTofPeak_string;
extern QString saveFilePath;   //保存的路径  E:/..../.../的形式
extern int saveFileIndex;      //文件标号；1作为开始

savePCDThread::savePCDThread(QObject *parent) : QObject(parent)
{
    qDebug()<<"save pcd thread start"<<endl;
}


//保存二进制效率会高的多，但不利于查看
void savePCDThread::savePCDSlot(pcl::PointCloud<pcl::PointXYZRGB> cloud,int formatFlag)
{
    //第一包数据丢掉不完整
    if(1 == saveFileIndex)
    {
        saveFileIndex++;
        return;
    }


    QString filePathName = saveFilePath + QString::number(saveFileIndex-1)+".pcd";
    if(0 == formatFlag)
    {
        pcl::io::savePCDFileBinary(filePathName.toLatin1().toStdString(),cloud);
    }
    else if(1 == formatFlag)
    {
        pcl::io::savePCDFileASCII(filePathName.toLatin1().toStdString(),cloud);
    }
    saveFileIndex++;
}



void savePCDThread::saveTXTSlot(QString msgStr)
{
    if(1 == saveFileIndex)
    {
        saveFileIndex++;
        return;
    }

    writeTXT(msgStr,saveFileIndex);

}



//按照标识写文件
// input:text:写入文本的内容
// numOfFile：第几个文件
void savePCDThread::writeTXT(QString text, int index)
{
    QString sFilePath = saveFilePath + QString::number(saveFileIndex-1)+".txt";
    saveFileIndex++;
    QFile file(sFilePath);
    file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);

    QTextStream out(&file);
    out<<text.toLocal8Bit()<<endl;
    file.close();
}
