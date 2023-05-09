#include "changepassword.h"
#include "ui_changepassword.h"
#include "config.h"
#include <QMessageBox>
#include "Frame/mainwindow.h"

ChangePassword::ChangePassword(QWidget *parent) :
    QDialog(parent),_currentUser(Config::instance()->getCurUser()),
    _ui(new Ui::ChangePassword)
{
    _ui->setupUi(this);
    _ui->lineEdit->setText(_currentUser.name);
    _ui->lineEdit_2->setFocus();
}

void ChangePassword::setChangePasswordInfo(QString userType, QString userName, QString password)
{
    _currentUser.type = userType;
    _currentUser.name = userName;
    _currentUser.password = password;
}

ChangePassword::~ChangePassword()
{
    delete _ui;
}

void ChangePassword::on_pushButton_clicked()
{
    if (_ui->lineEdit_2->text() == _currentUser.password
            && _ui->lineEdit_3->text() == _ui->lineEdit_4->text()
            && !_ui->lineEdit_3->text().isEmpty())
    {
//        QList<UserInfo>& userList = Config::instance()->getUserList();

//        for (int i = 0; i < userList.count(); ++i)
//        {
//            if (userList.value(i).type == _currentUser.type &&
//                    userList.value(i).name == _currentUser.name)
//            {
//                userList.value(i).password = _ui->lineEdit_3->text();
//            }
//        }

//        UserInfo& curUser = Config::instance()->getCurUser();
//        curUser.password = _ui->lineEdit_3->text();
        for (int i = 0; i < Config::instance()->getUserList().count(); ++i)
        {
            if (Config::instance()->getUserList().at(i).type == _currentUser.type
                    && Config::instance()->getUserList().at(i).name == _currentUser.name)
            {
                UserInfo temp;
                temp.name = _currentUser.name;
                temp.type = _currentUser.type;
                temp.password = _ui->lineEdit_3->text();
//                Config::instance()->getUserList().replace(i, temp);
                Config::instance()->changUserInfo(i, temp);
                break;
            }
        }

        if (Config::instance()->getCurUser().type == _currentUser.type
                && Config::instance()->getCurUser().name == _currentUser.name)
        {
            Config::instance()->getCurUser().password = _ui->lineEdit_3->text();
        }
        close();
    }
    else if (_ui->lineEdit_2->text() != _currentUser.password)
    {
        MainWindow::instance()->showNotice(tr("Old Password is wrong!"));
    }
    else if (_ui->lineEdit_3->text() != _ui->lineEdit_4->text())
    {
        MainWindow::instance()->showNotice(tr("Password is not the same!"));
    }
}

void ChangePassword::on_pushButton_2_clicked()
{
    close();
}

void ChangePassword::closeEvent(QCloseEvent *event)
{
    _ui->lineEdit_2->clear();
    _ui->lineEdit_3->clear();
    _ui->lineEdit_4->clear();

    QDialog::closeEvent(event);
}

void ChangePassword::showEvent(QShowEvent *event)
{
    _ui->lineEdit->setText(_currentUser.name);
    _ui->lineEdit_2->setFocus();

    QDialog::showEvent(event);
}
