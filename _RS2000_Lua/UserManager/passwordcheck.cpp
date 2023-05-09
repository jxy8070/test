#include "stable.h"

#include "passwordcheck.h"
#include "ui_passwordcheck.h"
#include "changepassword.h"
#include "Frame/mainwindow.h"


PasswordCheck::PasswordCheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordCheck)
{
    ui->setupUi(this);

    ui->lineEdit->setEchoMode(QLineEdit::Password);
    connect(ui->change, SIGNAL(clicked()), this, SLOT(change_clicked()));
    connect(ui->cancel, SIGNAL(clicked()), this, SLOT(cancel_clicked()));
    connect(ui->ok, SIGNAL(clicked()), this, SLOT(ok_clicked()));
}

PasswordCheck::~PasswordCheck()
{
//    qDebug("~PasswordCheck");
    delete ui;
}

void PasswordCheck::showEvent(QShowEvent *event)
{
    ui->lineEdit->setFocus();

    QDialog::showEvent(event);
}

void PasswordCheck::closeEvent(QCloseEvent *event)
{
    ui->lineEdit->clear();

    QDialog::closeEvent(event);
}

void PasswordCheck::change_clicked()
{
    ChangePassword passwordChange;
    UserInfo curUser = Config::instance()->getCurUser();
    passwordChange.setChangePasswordInfo(curUser.type,
        curUser.name, curUser.password);
    passwordChange.exec();
}

void PasswordCheck::ok_clicked()
{
    if (ui->lineEdit->text() ==
            Config::instance()->getCurUser().password)
    {
        this->accept();
    }
    else
    {
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
        MainWindow::instance()->showNotice(tr("The password is wrong, please retry!"));
    }
}

void PasswordCheck::cancel_clicked()
{
    this->close();
}

void PasswordCheck::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Enter:
        this->ok_clicked();
        break;
    case Qt::Key_Escape:
        close();
        break;
    default:        
        QDialog::keyPressEvent(event);
        break;
    }

}
