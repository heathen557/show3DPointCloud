#ifndef CALMEANSTDTHREAD_H
#define CALMEANSTDTHREAD_H

#include <QObject>
#include<numeric>
#include<vector>
#include<QMutex>
#include<QTimer>
using namespace std;

class calMeanStdThread : public QObject
{
    Q_OBJECT
public:
    explicit calMeanStdThread(QObject *parent = 0);

    QTimer updateTimer;

    float tofMean;
    float tofAccum;
    float tofStd;

    float peakMean;
    float peakAccum;
    float peakStd;

    int frameSize;

    vector<vector<int>> useStatisticTofPoints;
    vector<vector<int>> useStatisticPeakPoints;

    QString tofMean_string;
    QString tofStd_string;

    QString peakMean_string;
    QString peakStd_string;

signals:

    void statistic_MeanStdSignal(QString,QString,QString,QString);

public slots:
    void updateSlot();      //刷新均值和方差的槽函数
};

#endif // CALMEANSTDTHREAD_H
