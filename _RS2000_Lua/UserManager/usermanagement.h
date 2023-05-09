#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include <QDialog>
#include "ui_usermanagement.h"


class QTableWidgetItem;
class NewUser;
class ChangePassword;

class UserManagement : public QDialog, private Ui::UserManagement
{
    Q_OBJECT

public:
    explicit UserManagement(QWidget *parent = 0);
    ~UserManagement();

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_newuser_clicked();
    void on_deleteuser_clicked();
    void add_new_user();

    void on_password_clicked();

private:
    void init();
    void clear();
    void tableItemAdd(QString type);
    void tableItemRemove(QString type, QString name);

private:
    NewUser *m_newUser;
    ChangePassword *m_changePassword;
    QList<QTableWidgetItem *>_tableItem;
};

#endif // USERMANAGEMENT_H
