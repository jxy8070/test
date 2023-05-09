#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "config.h"


namespace Ui {
class LoginDialog;
}

enum {
    LOGINDLG_VIEWER = 0,
    LOGINDLG_NORMAL,
    LOGINDLG_ADMIN,
};

class AES70Device;
class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(bool isLookSys, QWidget *parent = 0);
    ~LoginDialog();

protected:
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *event);
    QString getSelectUserType();

    bool eventFilter(QObject *watched, QEvent *event);

public slots:
    void TypeChanged(int i);
    void NameChanged(int i);
    void CheckPassword(void);

private:
    Ui::LoginDialog *_ui;
    bool _isLockSys;
    QStringList _ipList;
    QList<QNetworkInterface> _itrList;
};

#endif // LOGINWIDGET_H
