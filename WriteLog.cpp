#include "WriteLog.h"
#include "globaldata.h"
#include<qdebug.h>
#include <qtextcodec.h>
#include <QMutex>

void WriteLogFile(QString filename ,int flag,QString msg)
{

     QString text;
//	 QString timenow=currenttime();
    // QTextCharFormat chfmt;

    switch (flag) {
    case 1:
        text = "INFO:";
        break;
    case 2:
        text = "WARN:";
       // chfmt.setForeground(QBrush(Qt::yellow));
        break;
    case 3:
        text = "ERROR:";
       // chfmt.setForeground(QBrush(Qt::red));
        break;
    default:
        break;
    }

     text.append("[");
 //   text.append(timenow);
    text.append("]");
    text.append(msg);
    QFile writefile(globaldata::current_path);
    if(writefile.open(QIODevice::WriteOnly|QIODevice::Append))
    {
        QTextStream out(&writefile);

        out<<text<<endl;
        writefile.close();
    }

}

void createLogFile()
{
    createlogdir("Log");
    QString timenow=currentdate();
    QString LogFile="Log/"+timenow+".txt";
    QFile file(LogFile);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text))
    {
        //qDebug()<<"Can't open the file!"<<endl;
    }
    file.close();
}

void createlogdir(QString logdir)
{
    QDir temp ;
    bool exist = temp.exists(logdir);
    if(!exist)
    {
        bool ok = temp.mkdir(logdir);
        if( !ok )
        {
            qDebug("create file failed");
        }
    }
}

QString  currentdate()
{
     QDateTime dt;
     QTime time;
     QDate date;
    // dt.setTime(time.currentTime());
     dt.setDate(date.currentDate());
     QString currenttime = dt.toString("yyyy-MM-dd");
     return currenttime;
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    static QMutex mutex;
    mutex.lock();



    QString strMsg("");
    switch(type)
    {
    case QtDebugMsg:
        strMsg = QString("info:");
        break;
    case QtWarningMsg:
        strMsg = QString("Warning:");
        break;
    /*case QtCriticalMsg:
        strMsg = QString("Critical:");
        break;*/
    /*case QtInfoMsg:
        strMsg = QString("Info:");
        break;*/
    case QtFatalMsg:
        strMsg = QString("Error:");
        break;
    }
    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);

//    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
//    QString strMessage = QString("%1%2,DateTime:%3,%4").arg(strMsg)
//            .arg(msg).arg(strDateTime).arg(context_info);

    QString strMessage = QString("%1%2").arg(strMsg)
            .arg(msg);


    createlogdir("ClientLog");
    QString timenow=currentdate();
    QString LogFile="ClientLog/"+timenow+".txt";

    QFile file(LogFile);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);

    stream << strMessage << "\r\n";
    file.flush();
    file.close();
    //QDIRINFO


    if(globaldata::oldLogFile!=LogFile)
    {
        globaldata::LogCount++;
    }

    if(globaldata::LogCount>30)
    {
         QDir dir;
         dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
         dir.setSorting(QDir::Size | QDir::Reversed);

         QFileInfoList list = dir.entryInfoList();
         QMap<QString, int> map;
         for (int i = 0; i < list.size(); ++i)
         {
             QFileInfo fileInfo = list.at(i);
             map.insert(fileInfo.fileName(),i);
         }
         QList<QString> kList = map.keys();
         QString msg1="delete Log file:"+kList[0];
         qDebug()<<msg1;

         QFile::remove(kList[1]);

    }
    globaldata::oldLogFile=LogFile;
    mutex.unlock();
}
