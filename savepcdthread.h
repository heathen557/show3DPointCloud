#ifndef SAVEPCDTHREAD_H
#define SAVEPCDTHREAD_H
#include<pcl/io/pcd_io.h>//pcd 读写类相关的头文件。
#include<pcl/io/ply_io.h>
#include<pcl/point_types.h> //PCL中支持的点类型头文件。
#include <QObject>

class savePCDThread : public QObject
{
    Q_OBJECT
public:
    explicit savePCDThread(QObject *parent = 0);

signals:

public slots:
    void savePCDSlot();

    void writeTXT(QString text,int index);

    void saveTXTSlot(QString msgStr);
};

#endif // SAVEPCDTHREAD_H
