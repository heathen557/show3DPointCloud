/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glwidget.h"
#include "mylabel.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QAction *action_2;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QFrame *frame_4;
    QGridLayout *gridLayout_7;
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    myLabel *showTOF_label;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    myLabel *showIntensity_label;
    QFrame *frame_3;
    QGridLayout *gridLayout_5;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_6;
    GLWidget *widget;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_6;
    QLineEdit *gainImage_lineEdit;
    QHBoxLayout *horizontalLayout_14;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *change_pushButton;
    QRadioButton *radioButton;
    QLabel *label_10;
    QFrame *frame_2;
    QGridLayout *gridLayout_12;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_8;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *VID_lineEdit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *PID_lineEdit;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_9;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QLineEdit *sysData_lineEdit;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *readSys_pushButton;
    QPushButton *writeSys_pushButton;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_10;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_5;
    QLineEdit *lineEdit_3;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QLineEdit *lineEdit_4;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_8;
    QLineEdit *lineEdit_5;
    QHBoxLayout *horizontalLayout_11;
    QPushButton *readDev_pushButton;
    QPushButton *writeDev_pushButton;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_11;
    QVBoxLayout *verticalLayout_2;
    QPushButton *loadSetting_pushButton;
    QPushButton *saveSetting_pushButton;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *pushButton_2;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_16;
    QHBoxLayout *horizontalLayout_13;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_13;
    QTextEdit *textEdit;
    QWidget *tab_2;
    QGridLayout *gridLayout_17;
    QTextEdit *textEdit_2;
    QFrame *frame_6;
    QGridLayout *gridLayout_15;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_11;
    QTableWidget *tableWidget_2;
    QFrame *frame_5;
    QGridLayout *gridLayout_14;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_9;
    QTableWidget *tableWidget_4;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1335, 806);
        MainWindow->setMinimumSize(QSize(1330, 806));
        QIcon icon;
        icon.addFile(QStringLiteral("icon_1.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QStringLiteral(""));
        MainWindow->setTabShape(QTabWidget::Rounded);
        action = new QAction(MainWindow);
        action->setObjectName(QStringLiteral("action"));
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QStringLiteral("action_2"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        frame_4 = new QFrame(centralWidget);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setStyleSheet(QLatin1String("QFrame#frame_4{\n"
"border:1px solid grey;}"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        gridLayout_7 = new QGridLayout(frame_4);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame = new QFrame(frame_4);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setMaximumSize(QSize(420, 523));
        frame->setStyleSheet(QStringLiteral(""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, -1, -1);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("font:10pt \"\351\273\221\344\275\223\";"));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(-1, 2, -1, -1);
        showTOF_label = new myLabel(groupBox);
        showTOF_label->setObjectName(QStringLiteral("showTOF_label"));
        showTOF_label->setMinimumSize(QSize(384, 224));
        showTOF_label->setMaximumSize(QSize(384, 224));
        showTOF_label->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));

        gridLayout_3->addWidget(showTOF_label, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(frame);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(0, 260));
        groupBox_2->setStyleSheet(QString::fromUtf8("font:10pt \"\351\273\221\344\275\223\";"));
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(-1, 2, -1, -1);
        showIntensity_label = new myLabel(groupBox_2);
        showIntensity_label->setObjectName(QStringLiteral("showIntensity_label"));
        showIntensity_label->setMinimumSize(QSize(384, 224));
        showIntensity_label->setMaximumSize(QSize(384, 224));
        showIntensity_label->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));

        gridLayout_4->addWidget(showIntensity_label, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBox_2);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);


        horizontalLayout->addWidget(frame);

        frame_3 = new QFrame(frame_4);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setStyleSheet(QStringLiteral(""));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(frame_3);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, -1, -1);
        groupBox_3 = new QGroupBox(frame_3);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(0, 520));
        groupBox_3->setMaximumSize(QSize(16777215, 520));
        groupBox_3->setStyleSheet(QString::fromUtf8("font:10pt \"\351\273\221\344\275\223\";"));
        gridLayout_6 = new QGridLayout(groupBox_3);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(-1, -1, -1, 0);
        widget = new GLWidget(groupBox_3);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(0, 472));
        widget->setMaximumSize(QSize(16777215, 500));
        widget->setStyleSheet(QLatin1String("border:1px solid black;\n"
"background-color: rgb(0, 0, 0);"));

        gridLayout_6->addWidget(widget, 0, 0, 1, 1);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_15->addWidget(label_6);

        gainImage_lineEdit = new QLineEdit(groupBox_3);
        gainImage_lineEdit->setObjectName(QStringLiteral("gainImage_lineEdit"));

        horizontalLayout_15->addWidget(gainImage_lineEdit);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        pushButton_3 = new QPushButton(groupBox_3);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setMaximumSize(QSize(60, 20));

        horizontalLayout_14->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(groupBox_3);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setMaximumSize(QSize(60, 20));

        horizontalLayout_14->addWidget(pushButton_4);

        pushButton_5 = new QPushButton(groupBox_3);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setMaximumSize(QSize(60, 20));

        horizontalLayout_14->addWidget(pushButton_5);


        horizontalLayout_15->addLayout(horizontalLayout_14);

        change_pushButton = new QPushButton(groupBox_3);
        change_pushButton->setObjectName(QStringLiteral("change_pushButton"));

        horizontalLayout_15->addWidget(change_pushButton);

        radioButton = new QRadioButton(groupBox_3);
        radioButton->setObjectName(QStringLiteral("radioButton"));

        horizontalLayout_15->addWidget(radioButton);

        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_15->addWidget(label_10);


        gridLayout_6->addLayout(horizontalLayout_15, 1, 0, 1, 1);


        gridLayout_5->addWidget(groupBox_3, 0, 0, 1, 1);


        horizontalLayout->addWidget(frame_3);

        frame_2 = new QFrame(frame_4);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy);
        frame_2->setMinimumSize(QSize(200, 0));
        frame_2->setMaximumSize(QSize(200, 500));
        frame_2->setStyleSheet(QStringLiteral(""));
        gridLayout_12 = new QGridLayout(frame_2);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox_4 = new QGroupBox(frame_2);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setMaximumSize(QSize(16777215, 107));
        groupBox_4->setStyleSheet(QString::fromUtf8("font:10pt \"\351\273\221\344\275\223\";"));
        gridLayout_8 = new QGridLayout(groupBox_4);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(groupBox_4);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        VID_lineEdit = new QLineEdit(groupBox_4);
        VID_lineEdit->setObjectName(QStringLiteral("VID_lineEdit"));

        horizontalLayout_2->addWidget(VID_lineEdit);


        gridLayout_8->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        PID_lineEdit = new QLineEdit(groupBox_4);
        PID_lineEdit->setObjectName(QStringLiteral("PID_lineEdit"));

        horizontalLayout_3->addWidget(PID_lineEdit);


        gridLayout_8->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        pushButton = new QPushButton(groupBox_4);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8("font:10pt \"\351\273\221\344\275\223\";"));

        horizontalLayout_7->addWidget(pushButton);


        gridLayout_8->addLayout(horizontalLayout_7, 2, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox_4);

        groupBox_6 = new QGroupBox(frame_2);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setMaximumSize(QSize(16777215, 106));
        groupBox_6->setStyleSheet(QString::fromUtf8("font:10pt \"\351\273\221\344\275\223\";\n"
""));
        gridLayout_9 = new QGridLayout(groupBox_6);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(groupBox_6);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        lineEdit = new QLineEdit(groupBox_6);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout_4->addWidget(lineEdit);


        gridLayout_9->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_4 = new QLabel(groupBox_6);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_5->addWidget(label_4);

        sysData_lineEdit = new QLineEdit(groupBox_6);
        sysData_lineEdit->setObjectName(QStringLiteral("sysData_lineEdit"));

        horizontalLayout_5->addWidget(sysData_lineEdit);


        gridLayout_9->addLayout(horizontalLayout_5, 1, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        readSys_pushButton = new QPushButton(groupBox_6);
        readSys_pushButton->setObjectName(QStringLiteral("readSys_pushButton"));

        horizontalLayout_6->addWidget(readSys_pushButton);

        writeSys_pushButton = new QPushButton(groupBox_6);
        writeSys_pushButton->setObjectName(QStringLiteral("writeSys_pushButton"));

        horizontalLayout_6->addWidget(writeSys_pushButton);


        gridLayout_9->addLayout(horizontalLayout_6, 2, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox_6);

        groupBox_7 = new QGroupBox(frame_2);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setMinimumSize(QSize(0, 0));
        groupBox_7->setStyleSheet(QString::fromUtf8("font:10pt \"\351\273\221\344\275\223\";"));
        gridLayout_10 = new QGridLayout(groupBox_7);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_5 = new QLabel(groupBox_7);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_8->addWidget(label_5);

        lineEdit_3 = new QLineEdit(groupBox_7);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        horizontalLayout_8->addWidget(lineEdit_3);


        gridLayout_10->addLayout(horizontalLayout_8, 0, 0, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_7 = new QLabel(groupBox_7);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_9->addWidget(label_7);

        lineEdit_4 = new QLineEdit(groupBox_7);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));

        horizontalLayout_9->addWidget(lineEdit_4);


        gridLayout_10->addLayout(horizontalLayout_9, 1, 0, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_8 = new QLabel(groupBox_7);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_10->addWidget(label_8);

        lineEdit_5 = new QLineEdit(groupBox_7);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));

        horizontalLayout_10->addWidget(lineEdit_5);


        gridLayout_10->addLayout(horizontalLayout_10, 2, 0, 1, 1);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        readDev_pushButton = new QPushButton(groupBox_7);
        readDev_pushButton->setObjectName(QStringLiteral("readDev_pushButton"));
        readDev_pushButton->setMinimumSize(QSize(0, 20));

        horizontalLayout_11->addWidget(readDev_pushButton);

        writeDev_pushButton = new QPushButton(groupBox_7);
        writeDev_pushButton->setObjectName(QStringLiteral("writeDev_pushButton"));
        writeDev_pushButton->setMinimumSize(QSize(0, 20));

        horizontalLayout_11->addWidget(writeDev_pushButton);


        gridLayout_10->addLayout(horizontalLayout_11, 3, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox_7);

        groupBox_8 = new QGroupBox(frame_2);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setMaximumSize(QSize(16777215, 82));
        gridLayout_11 = new QGridLayout(groupBox_8);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        loadSetting_pushButton = new QPushButton(groupBox_8);
        loadSetting_pushButton->setObjectName(QStringLiteral("loadSetting_pushButton"));

        verticalLayout_2->addWidget(loadSetting_pushButton);

        saveSetting_pushButton = new QPushButton(groupBox_8);
        saveSetting_pushButton->setObjectName(QStringLiteral("saveSetting_pushButton"));

        verticalLayout_2->addWidget(saveSetting_pushButton);


        gridLayout_11->addLayout(verticalLayout_2, 0, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox_8);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        pushButton_2 = new QPushButton(frame_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setStyleSheet(QString::fromUtf8("font:10pt \"\351\273\221\344\275\223\";"));

        horizontalLayout_12->addWidget(pushButton_2);


        verticalLayout_3->addLayout(horizontalLayout_12);


        gridLayout_12->addLayout(verticalLayout_3, 0, 0, 1, 1);


        horizontalLayout->addWidget(frame_2);


        gridLayout_7->addLayout(horizontalLayout, 0, 0, 1, 1);


        gridLayout->addWidget(frame_4, 0, 0, 1, 1);

        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setMaximumSize(QSize(16777215, 180));
        groupBox_5->setStyleSheet(QString::fromUtf8("\n"
"\n"
"#groupBox_5{\n"
"font:10pt \"\351\273\221\344\275\223\";\n"
"}"));
        gridLayout_16 = new QGridLayout(groupBox_5);
        gridLayout_16->setSpacing(6);
        gridLayout_16->setContentsMargins(11, 11, 11, 11);
        gridLayout_16->setObjectName(QStringLiteral("gridLayout_16"));
        gridLayout_16->setContentsMargins(-1, -1, -1, 3);
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        tabWidget = new QTabWidget(groupBox_5);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setMaximumSize(QSize(16777215, 130));
        tabWidget->setStyleSheet(QString::fromUtf8("font:9pt \"\351\273\221\344\275\223\";"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_13 = new QGridLayout(tab);
        gridLayout_13->setSpacing(6);
        gridLayout_13->setContentsMargins(11, 11, 11, 11);
        gridLayout_13->setObjectName(QStringLiteral("gridLayout_13"));
        textEdit = new QTextEdit(tab);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        gridLayout_13->addWidget(textEdit, 0, 1, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_17 = new QGridLayout(tab_2);
        gridLayout_17->setSpacing(6);
        gridLayout_17->setContentsMargins(11, 11, 11, 11);
        gridLayout_17->setObjectName(QStringLiteral("gridLayout_17"));
        textEdit_2 = new QTextEdit(tab_2);
        textEdit_2->setObjectName(QStringLiteral("textEdit_2"));

        gridLayout_17->addWidget(textEdit_2, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());

        horizontalLayout_13->addWidget(tabWidget);

        frame_6 = new QFrame(groupBox_5);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        frame_6->setMaximumSize(QSize(300, 16777215));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        gridLayout_15 = new QGridLayout(frame_6);
        gridLayout_15->setSpacing(6);
        gridLayout_15->setContentsMargins(11, 11, 11, 11);
        gridLayout_15->setObjectName(QStringLiteral("gridLayout_15"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_11 = new QLabel(frame_6);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setStyleSheet(QString::fromUtf8("font:10pt \"\351\273\221\344\275\223\";"));

        verticalLayout_5->addWidget(label_11);

        tableWidget_2 = new QTableWidget(frame_6);
        if (tableWidget_2->columnCount() < 2)
            tableWidget_2->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (tableWidget_2->rowCount() < 2)
            tableWidget_2->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(1, __qtablewidgetitem3);
        tableWidget_2->setObjectName(QStringLiteral("tableWidget_2"));
        tableWidget_2->setStyleSheet(QString::fromUtf8("font:10pt \"\351\273\221\344\275\223\";\n"
"border:0px"));

        verticalLayout_5->addWidget(tableWidget_2);


        gridLayout_15->addLayout(verticalLayout_5, 0, 0, 1, 1);


        horizontalLayout_13->addWidget(frame_6);

        frame_5 = new QFrame(groupBox_5);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setMaximumSize(QSize(300, 16777215));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        gridLayout_14 = new QGridLayout(frame_5);
        gridLayout_14->setSpacing(6);
        gridLayout_14->setContentsMargins(11, 11, 11, 11);
        gridLayout_14->setObjectName(QStringLiteral("gridLayout_14"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_9 = new QLabel(frame_5);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setStyleSheet(QString::fromUtf8("font:10pt \"\351\273\221\344\275\223\";"));

        verticalLayout_4->addWidget(label_9);

        tableWidget_4 = new QTableWidget(frame_5);
        if (tableWidget_4->columnCount() < 2)
            tableWidget_4->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        if (tableWidget_4->rowCount() < 3)
            tableWidget_4->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_4->setVerticalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_4->setVerticalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_4->setVerticalHeaderItem(2, __qtablewidgetitem8);
        tableWidget_4->setObjectName(QStringLiteral("tableWidget_4"));
        tableWidget_4->setStyleSheet(QString::fromUtf8("font:10pt \"\351\273\221\344\275\223\";\n"
"border:0px"));

        verticalLayout_4->addWidget(tableWidget_4);


        gridLayout_14->addLayout(verticalLayout_4, 0, 0, 1, 1);


        horizontalLayout_13->addWidget(frame_5);


        gridLayout_16->addLayout(horizontalLayout_13, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_5, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1335, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QStringLiteral("menu_4"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menu->addAction(action);
        menu_3->addAction(action_2);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\350\212\257\350\247\206\347\225\214\351\235\242\351\230\265\350\256\276\345\244\207\346\265\213\350\257\225\347\250\213\345\272\217", Q_NULLPTR));
        action->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\344\277\235\345\255\230", Q_NULLPTR));
        action_2->setText(QApplication::translate("MainWindow", "\347\273\237\350\256\241\350\256\276\347\275\256", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "Tof\345\233\276\345\203\217\357\274\232", Q_NULLPTR));
        showTOF_label->setText(QString());
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Peak\345\233\276\345\203\217\357\274\232", Q_NULLPTR));
        showIntensity_label->setText(QString());
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\347\202\271\344\272\221\345\233\276\345\203\217\357\274\232", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "-------------------", Q_NULLPTR));
        gainImage_lineEdit->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MainWindow", "\346\255\243\350\247\206\345\233\276", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("MainWindow", "\344\276\247\350\247\206\345\233\276", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("MainWindow", "\344\273\260\350\247\206\345\233\276", Q_NULLPTR));
        change_pushButton->setText(QApplication::translate("MainWindow", "\345\210\207\346\215\242tof/peak", Q_NULLPTR));
        radioButton->setText(QString());
        label_10->setText(QApplication::translate("MainWindow", "--------------------", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "\350\256\276\345\244\207\351\223\276\346\216\245\357\274\232", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "VID:", Q_NULLPTR));
        VID_lineEdit->setText(QApplication::translate("MainWindow", "8888", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "PID ", Q_NULLPTR));
        PID_lineEdit->setText(QApplication::translate("MainWindow", "0160", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245\350\256\276\345\244\207", Q_NULLPTR));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "\347\263\273\347\273\237\345\257\204\345\255\230\345\231\250\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "\345\234\260\345\235\200\357\274\232", Q_NULLPTR));
        lineEdit->setText(QApplication::translate("MainWindow", "13", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\357\274\232", Q_NULLPTR));
        readSys_pushButton->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226", Q_NULLPTR));
        writeSys_pushButton->setText(QApplication::translate("MainWindow", "\345\206\231\345\205\245", Q_NULLPTR));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "\350\256\276\345\244\207\345\257\204\345\255\230\345\231\250\357\274\232", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\345\234\260\345\235\200\357\274\232", Q_NULLPTR));
        lineEdit_3->setText(QApplication::translate("MainWindow", "D8", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "    \345\234\260\345\235\200\357\274\232", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "    \346\225\260\346\215\256\357\274\232", Q_NULLPTR));
        readDev_pushButton->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226", Q_NULLPTR));
        writeDev_pushButton->setText(QApplication::translate("MainWindow", "\345\206\231\345\205\245", Q_NULLPTR));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "\351\205\215\347\275\256\351\233\206\357\274\232", Q_NULLPTR));
        loadSetting_pushButton->setText(QApplication::translate("MainWindow", "\345\212\240\350\275\275\351\205\215\347\275\256\351\233\206", Q_NULLPTR));
        saveSetting_pushButton->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\351\205\215\347\275\256\351\233\206", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "\346\222\255\346\224\276", Q_NULLPTR));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "\347\233\221\346\216\247\347\252\227\345\217\243\357\274\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\350\277\236\346\216\245\344\277\241\346\201\257", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\350\277\220\350\241\214\346\227\245\345\277\227", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "           TOF/PEAK\344\277\241\346\201\257\347\273\237\350\256\241", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "minValue", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "maxValue", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_2->verticalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "tof", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_2->verticalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "peak", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "             \347\251\272\351\227\264\344\277\241\346\201\257\347\273\237\350\256\241", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_4->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "minValue", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_4->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "maxValue", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_4->verticalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "x", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_4->verticalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "y", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_4->verticalHeaderItem(2);
        ___qtablewidgetitem8->setText(QApplication::translate("MainWindow", "z", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("MainWindow", "\347\233\270\346\234\272\345\261\236\346\200\247", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("MainWindow", "\350\256\276\347\275\256", Q_NULLPTR));
        menu_4->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
