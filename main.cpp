﻿#include "mainwindow.h"
#include <QApplication>
#include "WriteLog.h"


int main(int argc, char *argv[])
{
//    qInstallMessageHandler(myMessageOutput);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
