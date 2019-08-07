#include "mainwindow.h"
#include <QScrollBar>
#include<QToolTip>
#include<QFileDialog>
QMutex mutex;
QImage tofImage;
QImage intensityImage;

int tofInfo[16384];
int peakInfo[16384];

bool isWriteSuccess;    //写入命令是否成功标识
bool isRecvFlag;
int framePerSecond;

/*保存用到的标识*/
bool isSaveFlag;        //是否进行存储
QString saveFilePath;   //保存的路径  E:/..../.../的形式
int saveFileIndex;      //文件标号；1作为开始
int formatFlag;         //0:二进制； 1：ASCII 2：TXT

extern bool  isShowPointCloud;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    isWriteSuccess = false;
    framePerSecond = 0;   //统计帧率，初始化为0
    isLinkSuccess = false;

    isSaveFlag = false;
    saveFileIndex = 1;

    //USB数据粗粒线程
     dealUsbMsg_obj = new DealUsb_msg();
     dealUsbThread = new QThread;
     dealUsbMsg_obj->moveToThread(dealUsbThread);
     dealUsbThread->start();

    //把读取USB信息放到线程当中，并开启线程
    recvUsbMsg_obj = new ReceUSB_Msg();
    recvUsbThread = new QThread;
    recvUsbMsg_obj->moveToThread(recvUsbThread);
    recvUsbThread->start();

    //开启保存pcd文件的线程
    savePCD_obj = new savePCDThread;
    saveThread = new QThread;
    savePCD_obj->moveToThread(saveThread);
    saveThread->start();

    qRegisterMetaType<pcl::PointCloud<pcl::PointXYZRGB>>("pcl::PointCloud<pcl::PointXYZRGB>");   //注册函数

    //接收数据线程、处理数据线程
    connect(recvUsbMsg_obj,SIGNAL(recvMsgSignal(QByteArray)),dealUsbMsg_obj,SLOT(recvMsgSlot(QByteArray)));

    connect(this,SIGNAL(read_usb_signal()),recvUsbMsg_obj,SLOT(read_usb()));
    connect(this,SIGNAL(readSignal(int,int)),recvUsbMsg_obj, SLOT(run(int,int)));
    connect(this,SIGNAL(closeLinkSignal()),recvUsbMsg_obj,SLOT(closeUSB()));
    connect(recvUsbMsg_obj,SIGNAL(linkInfoSignal(int)),this,SLOT(linkInfoSlot(int)));

    connect(this,SIGNAL(readSysSignal(int,bool)),recvUsbMsg_obj,SLOT(readSysSlot(int,bool)));
    connect(this,SIGNAL(writeSysSignal(int,QString,bool)),recvUsbMsg_obj,SLOT(writeSysSlot(int,QString,bool)));
    connect(this,SIGNAL(readDevSignal(int,int,bool)),recvUsbMsg_obj,SLOT(readDevSlot(int,int,bool)));
    connect(this,SIGNAL(writeDevSignal(int,int,QString,bool)),recvUsbMsg_obj,SLOT(writeDevSlot(int,int,QString,bool)));
    connect(this,SIGNAL(loadSettingSignal(QString,bool)),recvUsbMsg_obj,SLOT(loadSettingSlot(QString,bool)));
    connect(this,SIGNAL(saveSettingSignal(QString,int,bool)),recvUsbMsg_obj,SLOT(saveSettingSlot(QString,int,bool)));
    connect(recvUsbMsg_obj,SIGNAL(reReadSysSignal(QString)),this,SLOT(reReadSysSlot(QString)));
    connect(recvUsbMsg_obj,SIGNAL(reReadDevSignal(QString)),this,SLOT(reReadDevSlot(QString)));
    connect(dealUsbMsg_obj,SIGNAL(staticValueSignal(float,float,float,float,float,float,float,float,float,float)),this,SLOT(recvStaticValueSlot(float,float,float,float,float,float,float,float,float,float)));


    connect(&showTimer,SIGNAL(timeout()),this,SLOT(showImageSlot()));
    connect(&oneSecondTimer,SIGNAL(timeout()),this,SLOT(oneSecondSlot()));
    connect(ui->showTOF_label,SIGNAL(queryPixSignal(int,int)),this,SLOT(queryPixSlot(int,int)));
    connect(ui->showIntensity_label,SIGNAL(queryPixSignal(int,int)),this,SLOT(queryPixSlot(int,int)));
    connect(ui->action,SIGNAL(triggered()),this,SLOT(showSaveFileDialog())); //文件保存 窗口打开

    connect(&fileSaveDia,SIGNAL(isSaveFlagSignal(bool,QString,int)),this,SLOT(isSaveFlagSlot(bool,QString,int)));
    connect(dealUsbMsg_obj,SIGNAL(savePCDSignal(pcl::PointCloud<pcl::PointXYZRGB>,int)),savePCD_obj,SLOT(savePCDSlot(pcl::PointCloud<pcl::PointXYZRGB>,int)));
    connect(dealUsbMsg_obj,SIGNAL(saveTXTSignal(QString)),savePCD_obj,SLOT(saveTXTSlot(QString)));

    initGUI();
}

void MainWindow::initGUI()
{

    //**********************************************************

    ui->tableWidget_2->setColumnWidth(0,117);
    ui->tableWidget_2->setColumnWidth(1,117);
    ui->tableWidget_2->setRowHeight(0,35);
    ui->tableWidget_2->setRowHeight(1,35);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);   //禁止编辑
    ui->tableWidget_2->setItem(0,0,&tofMinItem_value);
    ui->tableWidget_2->setItem(0,1,&tofMaxItem_value);
    ui->tableWidget_2->setItem(1,0,&peakMinItem_value);
    ui->tableWidget_2->setItem(1,1,&peakMaxItem_value);


    ui->tableWidget_4->setColumnWidth(0,130);
    ui->tableWidget_4->setColumnWidth(1,130);
    ui->tableWidget_4->setRowHeight(0,20);
    ui->tableWidget_4->setRowHeight(1,25);
    ui->tableWidget_4->setRowHeight(2,25);
    ui->tableWidget_4->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中
    ui->tableWidget_4->setEditTriggers(QAbstractItemView::NoEditTriggers);   //禁止编辑
    ui->tableWidget_4->setItem(0,0,&xMinItem_value);
    ui->tableWidget_4->setItem(0,1,&xMaxItem_value);
    ui->tableWidget_4->setItem(1,0,&yMinItem_value);
    ui->tableWidget_4->setItem(1,1,&yMaxItem_value);
    ui->tableWidget_4->setItem(2,0,&zMinItem_value);
    ui->tableWidget_4->setItem(2,1,&zMaxItem_value);


    tofMinItem_value.setTextAlignment(Qt::AlignCenter);
    tofMaxItem_value.setTextAlignment(Qt::AlignCenter);
    peakMinItem_value.setTextAlignment(Qt::AlignCenter);
    peakMaxItem_value.setTextAlignment(Qt::AlignCenter);
    xMinItem_value.setTextAlignment(Qt::AlignCenter);
    xMaxItem_value.setTextAlignment(Qt::AlignCenter);
    yMinItem_value.setTextAlignment(Qt::AlignCenter);
    yMaxItem_value.setTextAlignment(Qt::AlignCenter);
    zMinItem_value.setTextAlignment(Qt::AlignCenter);
    zMaxItem_value.setTextAlignment(Qt::AlignCenter);



}


//刷新显示的槽函数
void MainWindow::showImageSlot()
{
    if(!isShowPointCloud)
        return;


    if(!tofImage.isNull() && !intensityImage.isNull())
    {
        mutex.lock();

        mutex.try_lock();
        resImage = tofImage.scaled(tofImage.width()*1.5, tofImage.height()*3.5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        resIntenImage = intensityImage.scaled(intensityImage.width()*1.5, intensityImage.height()*3.5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        mutex.unlock();

        QPixmap pixmap2(QPixmap::fromImage (resImage));
        ui->showTOF_label->setPixmap(pixmap2);

        QPixmap pixmap3(QPixmap::fromImage (resIntenImage));
        ui->showIntensity_label->setPixmap(pixmap3);

    }
}


//接收线程发过来的错误信息

// 0：连接正常 1没找到设备
// 2:没有接收到数据  3打开设备失败
// 4：读取系统成功；5：读取系统失败；
// 6：读取设备成功；7：读取设备失败
// 8：加载配置信息成功；9：加载配置信息失败
// 10：保存配置信息成功； 11：保存配置信息失败
// 12：写入系统成功      13：写入系统失败
// 14：写入设备成功      15：写入设备失败
void MainWindow::linkInfoSlot(int flagNum)
{
    QString strLog;
    QString tempStr;
    QTime t1 = QTime::currentTime();
    QString tempstr_1;
    QString str;

    switch (flagNum) {
    case 0:
        tempStr = QStringLiteral("设备连接成功");
        tempStr.append("                           ");
        ui->pushButton->setText(QStringLiteral("关闭连接"));
        isLinkSuccess = true;
        break;
    case 1:
        tempStr = QStringLiteral("未找到设备！");
        tempStr.append("                   ");
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("未找到设备！"));
        break;
    case 2:
        /*****打印到运行日志*****/
        tempstr_1 = QStringLiteral("接收数据异常，请检查设备！");
        t1 = QTime::currentTime();
        str = tempstr_1 + "           " +t1.toString("hh:mm:ss");
        ui->textEdit_2->append(str);



        isRecvFlag = false ;
        tempStr = QStringLiteral("未接收到数据，请检查设备！");
        tempStr.append("             ");
//        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("未接收到数据，请检查设备，"));
        break;
    case 3:
        tempStr = QStringLiteral("打开设备失败！");
        tempStr.append("                   ");
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("打开设备失败"));
        break;
    case 4:
        tempStr = QStringLiteral("读取系统寄存器成功！");
        tempStr.append("                   ");
        break;
    case 5:
        tempStr = QStringLiteral("读取系统寄存器失败！");
        tempStr.append("                   ");
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("读取系统寄存器失败,"));
        break;
    case 6:
        tempStr = QStringLiteral("读取设备寄存器成功！");
        tempStr.append("                   ");
        break;
    case 7:
        tempStr = QStringLiteral("读取设备寄存器失败！");
        tempStr.append("                   ");
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("读取设备寄存器失败."));
        break;
    case 8:
        tempStr = QStringLiteral("加载配置信息成功！");
        tempStr.append("                     ");
        break;
    case 9:
        tempStr = QStringLiteral("加载配置信息失败！");
        tempStr.append("                   ");
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("加载配置信息失败."));
        break;
    case 10:
        tempStr = QStringLiteral("保存配置信息成功！");
        tempStr.append("                   ");
        break;
    case 11:
        tempStr = QStringLiteral("保存配置信息失败！");
        tempStr.append("                   ");
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("保存配置信息失败."));
        break;
    case 12:
        tempStr = QStringLiteral("写入系统寄存器成功！");
        tempStr.append("                   ");
        break;
    case 13:
        tempStr = QStringLiteral("写入系统寄存器失败！");
        tempStr.append("                   ");
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("写入系统寄存器失败的"));
        break;
    case 14:
        tempStr = QStringLiteral("写入设备寄存器成功！");
        tempStr.append("                   ");
        break;
    case 15:
        tempStr = QStringLiteral("写入设备寄存器失败！");
        tempStr.append("                   ");
        break;

    default:
        break;
    }

    str = tempStr  +t1.toString("hh:mm:ss");
    ui->textEdit->append(str);
}

//没接收到一帧会进入一次，故可以统计 帧率
void MainWindow::recvStaticValueSlot(float tofMin,float tofMax,float peakMin,float peakMax,float xMin,float xMax,float yMin,float yMax,float zMin,float zMax)
{
    framePerSecond++;
    tofMin_ = tofMin;
    tofMax_ = tofMax;
    peakMin_ = peakMin;
    peakMax_ = peakMax;
    xMin_ = xMin;
    xMax_ = xMax;
    yMin_ = yMin;
    yMax_ = yMax;
    zMin_ = zMin;
    zMax_ = zMax;

    //     tofMinItem_value.setText(QString::number(tofMin));
    //     tofMaxItem_value.setText(QString::number(tofMax));
    //     peakMinItem_value.setText(QString::number(peakMin));
    //     peakMaxItem_value.setText(QString::number(peakMax));

    //     xMinItem_value.setText(QString::number(xMin));
    //     xMaxItem_value.setText(QString::number(xMax));
    //     yMinItem_value.setText(QString::number(yMin));
    //     yMaxItem_value.setText(QString::number(yMax));
    //     zMinItem_value.setText(QString::number(zMin));
    //     zMaxItem_value.setText(QString::number(zMax));



}

MainWindow::~MainWindow()
{
    delete ui;
}

//打开设备
void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text() ==QStringLiteral("连接设备"))
    {

        int vid = ui->VID_lineEdit->text().toInt(NULL,16);
        int pid = ui->PID_lineEdit->text().toInt(NULL,16);
        emit readSignal(vid,pid);

        if(isWriteSuccess)
        {
           isRecvFlag = true;
           emit read_usb_signal();
        }

    }else if(ui->pushButton->text() == QStringLiteral("关闭连接"))
    {
        //        oneSecondTimer.stop();
        isRecvFlag = false;
        emit closeLinkSignal();
        ui->pushButton->setText(QStringLiteral("连接设备"));
        isLinkSuccess = false;


        /*****打印到运行日志*****/
        QString tempstr = QStringLiteral("已经关闭USB的连接！");
        QTime t1 = QTime::currentTime();
        QString str = tempstr + "               " +t1.toString("hh:mm:ss");
        ui->textEdit_2->append(str);
    }

}


//播放槽函数
void MainWindow::on_pushButton_2_clicked()
{
    if(ui->pushButton_2->text() == QStringLiteral("播放"))
    {
        if(isWriteSuccess)
        {
            showTimer.start(90);
            ui->widget->readFileTimer.start(90);
            oneSecondTimer.start(1000);

            QString tempstr = QStringLiteral("数据接收正常,开始播放~");
            QTime t1 = QTime::currentTime();
            QString str = tempstr + "               " +t1.toString("hh:mm:ss");
            ui->textEdit_2->append(str);


        }else
        {
            //QMessageBox::information(NULL,"告警","未接收到数据，请检查设备连接！");
        }
        ui->pushButton_2->setText(QStringLiteral("暂停"));

    }else
    {
        showTimer.stop();
        ui->widget->readFileTimer.stop();
        ui->pushButton_2->setText(QStringLiteral("播放"));

        /*****打印到运行日志*****/
        QString tempstr = QStringLiteral("播放暂停！");
        QTime t1 = QTime::currentTime();
        QString str = tempstr + "                           " +t1.toString("hh:mm:ss");
        ui->textEdit_2->append(str);
    }




}

//读取系统寄存器
void MainWindow::on_readSys_pushButton_clicked()
{
    if(!isLinkSuccess)
    {
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("设备未连接"));
        return;
    }

    int address = ui->lineEdit->text().toInt(NULL,16);

//    qDebug()<<" the address = "<<address<<endl;
//    emit readSysSignal();


    if(isRecvFlag)
    {
        isRecvFlag = false;
        emit readSysSignal(address,true);
    }else
    {
        emit readSysSignal(address,false);
    }


}

//写入系统寄存器
void MainWindow::on_writeSys_pushButton_clicked()
{
    if(!isLinkSuccess)
    {
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("设备未连接"));
        return;
    }

    int address = ui->lineEdit->text().toInt(NULL,16);
    QString data = ui->sysData_lineEdit->text();

//    emit writeSysSignal(address,data);
    if(isRecvFlag)
    {
        isRecvFlag = false;
        emit writeSysSignal(address,data,true);
    }else
    {
        emit writeSysSignal(address,data,false);
    }
    ui->sysData_lineEdit->clear();

}

//读取设备寄存器
void MainWindow::on_readDev_pushButton_clicked()
{
    if(!isLinkSuccess)
    {
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("设备未连接"));
        return;
    }

    int hardWareAddress = ui->lineEdit_3->text().toInt(NULL,16);
    int registerAddress = ui->lineEdit_4->text().toInt(NULL,16);
//    emit readDevSignal(hardWareAddress,registerAddress);

    if(isRecvFlag)
    {
        isRecvFlag = false;
        emit readDevSignal(hardWareAddress,registerAddress,true);
    }else
    {
        emit readDevSignal(hardWareAddress,registerAddress,false);
    }


}

//写入设备寄存器
void MainWindow::on_writeDev_pushButton_clicked()
{
    if(!isLinkSuccess)
    {
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("设备未连接"));
        return;
    }

    int hardWareAddress = ui->lineEdit_3->text().toInt(NULL,16);
    int registerAddress = ui->lineEdit_4->text().toInt(NULL,16);
    QString data = ui->lineEdit_5->text();

//    emit writeDevSignal(hardWareAddress,registerAddress,data);
//    ui->lineEdit_5->clear();



    //如果接收线程正在运行，先关闭接收线程（while循环），否则线程接收不到信号
    //线程处理完数据以后，再次打开while循环，即另isRecvFlag = true;
    if(isRecvFlag)
    {
        isRecvFlag = false;
        emit writeDevSignal(hardWareAddress,registerAddress,data,true);
    }else
    {
        emit writeDevSignal(hardWareAddress,registerAddress,data,false);
    }
    ui->lineEdit_5->clear();

}

//加载配置集
void MainWindow::on_loadSetting_pushButton_clicked()
{
    if(!isLinkSuccess)
    {
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("设备未连接"));
        return;
    }

    QString file_path;
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(QStringLiteral("请选择配置文件"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径

    QStringList mimeTypeFilters;
    mimeTypeFilters <<QStringLiteral("芯片配置文件(*.para)|*.para") ;
    fileDialog->setNameFilters(mimeTypeFilters);


    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }else
    {
        return;
    }
    ////////////////////////////////////////////////////////////////////////////////////////

    file_path = fileNames[0];
    qDebug()<<" file_path = "<<fileNames[0]<<endl;

    QString checkStr = file_path.right(4);
    if("para" != checkStr)
    {
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("请选择正确的配置文件！"));
        return ;
    }

    if(isRecvFlag)
    {
        isRecvFlag = true;
        emit loadSettingSignal(file_path,true);
    }else
    {
        emit loadSettingSignal(file_path,false);
    }




}

//保存配置集
void MainWindow::on_saveSetting_pushButton_clicked()
{
    if(!isLinkSuccess)
    {
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("设备未连接"));
        return;
    }

    QString filePath;

    QFileDialog *fileDialog = new QFileDialog(this);//创建一个QFileDialog对象，构造函数中的参数可以有所添加。
    fileDialog->setWindowTitle(tr("Save As"));//设置文件保存对话框的标题
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);//设置文件对话框为保存模式
    fileDialog->setFileMode(QFileDialog::AnyFile);//设置文件对话框弹出的时候显示任何文件，不论是文件夹还是文件
    fileDialog->setViewMode(QFileDialog::Detail);//文件以详细的形式显示，显示文件名，大小，创建日期等信息；
    fileDialog->setGeometry(10,30,300,200);//设置文件对话框的显示位置
    fileDialog->setDirectory(".");//设置文件对话框打开时初始打开的位置
    QStringList mimeTypeFilters;
    mimeTypeFilters <<QStringLiteral("芯片配置文件(*.para)|*.para") ;
    fileDialog->setNameFilters(mimeTypeFilters);


    if(fileDialog->exec() == QDialog::Accepted)
    {
        filePath = fileDialog->selectedFiles()[0];//得到用户选择的文件名
        qDebug()<<" filePath = "<<filePath<<endl;
    }else
    {
        return ;
    }

    int deviceId = ui->lineEdit_3->text().toInt(NULL,16);


    //如果接收线程正在运行，先关闭接收线程（while循环），否则线程接收不到信号
    //线程处理完数据以后，再次打开while循环，即另isRecvFlag = true;
    if(isRecvFlag)
    {
        isRecvFlag = false;
        emit saveSettingSignal(filePath, deviceId, true);
    }else
    {
        emit saveSettingSignal(filePath, deviceId, false);
    }

}

//读取系统指令 返回槽函数,str是十进制的数字
void MainWindow::reReadSysSlot(QString str)
{
//    QByteArray ba = str.toLatin1();
//    const char *c_str = ba.data();  //为何要使用const 应该跟使用Qt版本有关
//    int m = uint8_t(c_str[0]);
//    qDebug()<<" the data =  "<<m<<endl;

    int m = str.toInt();
    qDebug()<<" the data =  "<<m<<endl;
    ui->sysData_lineEdit->setText(QString::number(m,16));
}

//读取设备指令 返回槽函数,str是十进制的数字
void MainWindow::reReadDevSlot(QString str)
{
//    QByteArray ba = str.toLatin1();
//    const char *c_str = ba.data();  //为何要使用const 应该跟使用Qt版本有关
//    int m = uint8_t(c_str[0]);
    int m = str.toInt();
    qDebug()<<" the data =  "<<m<<endl;
    ui->lineEdit_5->setText(QString::number(m,16));
}

void MainWindow::oneSecondSlot()
{
    //   qDebug()<<"帧率 = "<<framePerSecond<<endl;
    framePerSecond = 0;


    tofMinItem_value.setText(QString::number(tofMin_));
    tofMaxItem_value.setText(QString::number(tofMax_));
    peakMinItem_value.setText(QString::number(peakMin_));
    peakMaxItem_value.setText(QString::number(peakMax_));

    xMinItem_value.setText(QString::number(xMin_));
    xMaxItem_value.setText(QString::number(xMax_));
    yMinItem_value.setText(QString::number(yMin_));
    yMaxItem_value.setText(QString::number(yMax_));
    zMinItem_value.setText(QString::number(zMin_));
    zMaxItem_value.setText(QString::number(zMax_));
}


//鼠标停靠处显示TOF 和 peak信息
void MainWindow::queryPixSlot(int x,int y)
{
    int index = 256*y/3.5 +x/1.5 ;
//    QString str = "tof="+QString::number(tofInfo[index])+",peak="+QString::number(peakInfo[index]);

    QString str ="x="+QString::number(int(x/1.5)) + ",y="+QString::number(int(y/3.5)) + ",tof="+QString::number(tofInfo[index])+",peak="+QString::number(peakInfo[index]);

    QToolTip::showText(QCursor::pos(),str);
    //    qDebug()<<"x="<<x<<"  y="<<y<<" tof="<<tofInfo[index]<<" peak="<<peakInfo[index]<<endl;
}


//文件保存界面的槽函数
void MainWindow::showSaveFileDialog()
{
    fileSaveDia.show();
}

//接收是否保存pcd文件的槽函数
void MainWindow::isSaveFlagSlot(bool saveFlag, QString filePath,int formatSelect)
{
    if(saveFlag)
        saveFileIndex = 1;

    saveFilePath = filePath; 
    formatFlag = formatSelect ;
    isSaveFlag = saveFlag;
}

//三维点云的正视图
void MainWindow::on_pushButton_3_clicked()
{
    ui->widget->frontView_slot();
}

//三维点云的侧视图
void MainWindow::on_pushButton_4_clicked()
{
    ui->widget->endView_slot();
}

//三维点云的仰视图
void MainWindow::on_pushButton_5_clicked()
{
    ui->widget->verticalView_slot();
}
