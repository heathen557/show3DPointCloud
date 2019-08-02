#ifndef FILESAVE_H
#define FILESAVE_H

#include <QDialog>

namespace Ui {
class fileSave;
}

class fileSave : public QDialog
{
    Q_OBJECT

public:
    explicit fileSave(QWidget *parent = 0);
    ~fileSave();

    QString file_path;

    int formatSelect;   //0:二进制  1：ASCII    2：txt

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_SelFilePath_pushButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_5_clicked();

signals:
    void isSaveFlagSignal(bool,QString,int);  //是否保存pcd文件,路径、格式

private:
    Ui::fileSave *ui;
};

#endif // FILESAVE_H
