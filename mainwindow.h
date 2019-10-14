#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QImage>
//#include"receusb_msg.h"
#include<QThread>
#include "ui_mainwindow.h"
#include<QMutex>
#include<QMessageBox>
#include"receusb_msg.h"
#include"dealusb_msg.h"
#include<QTableWidgetItem>
#include<filesave.h>
#include<savepcdthread.h>
#include<statisticsdialog.h>
#include"calmeanstdthread.h"
#include<QSignalMapper>
#include<QLabel>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include<openlocaldialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    void init3DShow();

    void initTreeWidget();          //设备寄存器读写的界面的初始化函数

    void closeEvent(QCloseEvent *event);

    void resizeEvent(QResizeEvent *event);

    QTimer showTimer;
    QTimer oneSecondTimer;

    ReceUSB_Msg *recvUsbMsg_obj;
    QThread *recvUsbThread;

    DealUsb_msg *dealUsbMsg_obj;
    QThread *dealUsbThread;

    savePCDThread *savePCD_obj;
    QThread *saveThread;

    calMeanStdThread *calMeanStd_obj;
    QThread *calThread;

    QTableWidgetItem tofMinItem_value;
    QTableWidgetItem tofMaxItem_value;
    QTableWidgetItem peakMinItem_value;
    QTableWidgetItem peakMaxItem_value;

    QTableWidgetItem xMinItem_value;
    QTableWidgetItem xMaxItem_value;
    QTableWidgetItem yMinItem_value;
    QTableWidgetItem yMaxItem_value;
    QTableWidgetItem zMinItem_value;
    QTableWidgetItem zMaxItem_value;

//    int framePerSecond;
    float tofMin_,tofMax_,peakMin_,peakMax_,xMin_,xMax_,yMin_,yMax_,zMin_,zMax_;

    QImage resImage;         //用来显示缩放后的tof图像
    QImage resIntenImage;    //用来显示缩放后的intensity图像

    bool isLinkSuccess;      //是否连接成功的标识，若是没有连接，则不能进行后续操作
    fileSave fileSaveDia;

    statisticsDialog  statisticsDia_;

    openLocalDialog openLocalDia_;

    QString logStr;


    /***********单个寄存器配置**********/
    QLineEdit TDC_lineEdit[13];
    QPushButton TDC_read_pushButton[13];
    QPushButton TDC_write_pushButton[13];
    QSignalMapper *TDC_read_signalMapper;
    QSignalMapper *TDC_write_signalMapper;

    QLineEdit Integration_lineEdit[4];
    QPushButton Integration_read_pushButton[4];
    QPushButton Integration_write_pushButton[4];
    QSignalMapper *Integration_read_signalMapper;
    QSignalMapper *Integration_write_signalMapper;

    QLineEdit MA_lineEdit[16];
    QPushButton MA_read_pushButton[16];
    QPushButton MA_write_pushButton[16];
    QSignalMapper *MA_read_signalMapper;
    QSignalMapper *MA_write_signalMapper;

    QLineEdit Digital_lineEdit[10];
    QPushButton Digital_read_pushButton[10];
    QPushButton Digital_write_pushButton[10];
    QSignalMapper *Digital_read_signalMapper;
    QSignalMapper *Digital_write_signalMapper;

    QLineEdit Analog_lineEdit[26];
    QPushButton Analog_read_pushButton[26];
    QPushButton Analog_write_pushButton[26];
    QSignalMapper *Analog_read_signalMapper;
    QSignalMapper *Analog_write_signalMapper;

    QLineEdit Pixel_lineEdit[17];
    QPushButton Pixel_read_pushButton[17];
    QPushButton Pixel_write_pushButton[17];
    QSignalMapper *Pixel_read_signalMapper;
    QSignalMapper *Pixel_write_signalMapper;

    QLineEdit Top_lineEdit[13];
    QPushButton Top_read_pushButton[13];
    QPushButton Top_write_pushButton[13];
    QSignalMapper *Top_read_signalMapper;
    QSignalMapper *Top_write_signalMapper;

    QLineEdit Delayline_lineEdit[7];
    QPushButton Delayline_read_pushButton[7];
    QPushButton Delayline_write_pushButton[7];
    QSignalMapper *Delayline_read_signalMapper;
    QSignalMapper *Delayline_write_signalMapper;

    QLineEdit MISC_lineEdit[8];
    QPushButton MISC_read_pushButton[8];
    QPushButton MISC_write_pushButton[8];
    QSignalMapper *MISC_read_signalMapper;
    QSignalMapper *MISC_write_signalMapper;

    QLineEdit Others_lineEdit[6];
    QPushButton Others_read_pushButton[6];
    QPushButton Others_write_pushButton[6];
    QSignalMapper *Others_read_signalMapper;
    QSignalMapper *Others_write_signalMapper;

    QLabel explainLabel;  //显示注释
    int expandItem_index;

     QCamera *camera;

     bool isShowCamera;


private slots:

    void on_pushButton_clicked();    //测试用的槽函数

    void showImageSlot();           //显示二维图像的槽函数

    void on_pushButton_2_clicked();   //播放的槽函数

    void linkInfoSlot(int );      //接收线程发过来的链接信息（告警、错误）

    void showRunInfoSlot(QString msgStr);     //显示运行日志的槽函数

    void showWarnInfoSlot(QString warnStr);  //显示告警信息日志的槽函数

    void initGUI();

    void on_readSys_pushButton_clicked();

    void on_writeSys_pushButton_clicked();

//    void on_readDev_pushButton_clicked();

//    void on_writeDev_pushButton_clicked();

    void on_loadSetting_pushButton_clicked();

    void on_saveSetting_pushButton_clicked();

    void recvStaticValueSlot(float tofMin,float tofMax,float peakMin,float peakMax,float xMin,float xMax,float yMin,float yMax,float zMin,float zMax);

    void oneSecondSlot();   //每秒的槽函数

    void queryPixSlot(int x, int y);     //接收label空间坐标的槽函数

    void reReadSysSlot(QString str);  //读取系统指令 返回槽函数

    void reReadDevSlot(int regesiterAddress,QString str); //读取设备指令 返回槽函数

    /****************/
    void showSaveFileDialog();     //打开保存文件窗口的槽函数

    void isSaveFlagSlot(bool saveFlag, QString filePath,int formatSelect);    //接收是否保存pcd文件的槽函数
    //正视图
    void on_pushButton_3_clicked();
    //侧视图
    void on_pushButton_4_clicked();
    //仰视图
    void on_pushButton_5_clicked();

    void on_change_pushButton_clicked();

    void on_radioButton_clicked();

    void on_gainImage_lineEdit_returnPressed();

    /***************数据分析****************************/
    void showStatisticDia_slot();

    /***************读取本地文件**********************************/
    void showOpenLocalDia_slot();

    /****************单个寄存器配置相关槽函数*********************************/
    void TDC_read_slot(int);

    void TDC_write_slot(int);

    void Integration_read_slot(int);

    void Integration_write_slot(int);

    void MA_read_slot(int);

    void MA_write_slot(int);

    void Digital_read_slot(int);

    void Digital_write_slot(int);

    void Analog_read_slot(int);

    void Analog_write_slot(int);

    void Pixel_read_slot(int);

    void Pixel_write_slot(int);

    void Top_read_slot(int);

    void Top_write_slot(int);

    void Delayline_read_slot(int);

    void Delayline_write_slot(int);

    void MISC_read_slot(int);

    void MISC_write_slot(int);

    void Others_read_slot(int);

    void Others_write_slot(int);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemExpanded(QTreeWidgetItem *item);

//    void showStatisticDia_slot();

    void on_getALL_pushButton_clicked();

    void on_setAll_pushButton_clicked();

    void on_toolBox_currentChanged(int index);

    void on_tabWidget_2_currentChanged(int index);

    void on_guideLineOffset_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;

signals:
    void readSignal(int ,int);

    void closeLinkSignal();   //关闭连接

    void readSysSignal(int,bool);

    void writeSysSignal(int,QString,bool);

    void readDevSignal(int,int,bool);

    void writeDevSignal(int,int,QString,bool);

    void loadSettingSignal(QString,bool);

    void saveSettingSignal(QString,int, bool );

    void read_usb_signal();

    void isFilter_signal(bool);
};

#endif // MAINWINDOW_H
