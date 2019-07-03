#ifndef WRITELOG_H
#define WRITELOG_H

#include<QString>
#include<QDateTime>
#include<QTextCharFormat>
#include<QFile>
#include<QTextStream>
#include<QDir>
//QString kstimerecord();
void createLogFile();
void createlogdir(QString logdir);
void WriteLogFile(QString filename ,int flag,QString msg);
QString  currentdate();
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
#endif //LISTENSET_H