#ifndef NEWUSER_H
#define NEWUSER_H

#include <QDialog>
#include "ui_newuser.h"


class NewUser : public QDialog, private Ui::NewUser
{
    Q_OBJECT

public:
    explicit NewUser(QWidget *parent = 0);
    ~NewUser();
    void showWidget(int i);

private slots:
    void on_pushButtonok_clicked();
    void on_pushButtoncancel_clicked();
    void TypeChange(int index);

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void addNewUser();

private:
};

#endif // NEWUSER_H
