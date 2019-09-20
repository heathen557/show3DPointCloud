#ifndef OPENLOCALDIALOG_H
#define OPENLOCALDIALOG_H

#include <QDialog>

namespace Ui {
class openLocalDialog;
}

class openLocalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit openLocalDialog(QWidget *parent = 0);
    ~openLocalDialog();

private:
    Ui::openLocalDialog *ui;
};

#endif // OPENLOCALDIALOG_H
