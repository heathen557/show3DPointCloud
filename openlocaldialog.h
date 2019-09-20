#ifndef OPENLOCALDIALOG_H
#define OPENLOCALDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include<QDebug>
#include<QMessageBox>


namespace Ui {
class openLocalDialog;
}

class openLocalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit openLocalDialog(QWidget *parent = 0);
    ~openLocalDialog();

    QString filePath;

private slots:

    void on_ok_pushButton_clicked();

    void on_cancel_pushButton_clicked();

    void on_select_pushButton_clicked();

private:
    Ui::openLocalDialog *ui;

signals:
    void selectLocalFile_signal(QString);
};

#endif // OPENLOCALDIALOG_H
