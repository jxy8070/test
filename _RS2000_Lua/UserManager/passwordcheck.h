#ifndef PASSWORDCHECK_H
#define PASSWORDCHECK_H

#include <QDialog>
#include "config.h"


namespace Ui {
class PasswordCheck;
}

class PasswordCheck : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordCheck(QWidget *parent = 0);
    ~PasswordCheck();

public slots:
    void change_clicked();
    void ok_clicked();
    void cancel_clicked();
protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::PasswordCheck *ui;
};

#endif // PASSWORDCHECK_H
