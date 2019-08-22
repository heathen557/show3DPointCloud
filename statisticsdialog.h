#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <QDialog>


namespace Ui {
class statisticsDialog;
}

class statisticsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit statisticsDialog(QWidget *parent = 0);
    ~statisticsDialog();


private slots:
    void on_start_pushButton_clicked();

    void on_start_pushButton_2_clicked();

     void statistic_MeanStdSlot(QString,QString,QString,QString);

signals:
     void alterStatisticFrameNum_signal(int);

private:
    Ui::statisticsDialog *ui;
};

#endif // STATISTICSDIALOG_H
