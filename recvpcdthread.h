#ifndef RECVPCDTHREAD_H
#define RECVPCDTHREAD_H

#include <QObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include<QTimer>
#include<QtNetwork/QTcpSocket>

extern QVector<GLfloat> m_data;

class recvPcdThread : public QObject
{
    Q_OBJECT
public:
    explicit recvPcdThread(QObject *parent = 0);

    QTimer readTimer;   //test
    int index;          //test


    QTcpSocket m_tcpSocket;
    QByteArray m_buffer;     //存储的所有接收到的内容

signals:

public slots:
    void readDataSlot();
    char *strsep(char **s, const char *ct);
    void run();


    void ClientRecvData();  //接收点云数据的槽函数

};

#endif // RECVPCDTHREAD_H
