#include "stable.h"
#include "newuser.h"

#include "config.h"
//#include <QMessageBox>

NewUser::NewUser(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setModal(true);

    connect(type, SIGNAL(activated(int)), this, SLOT(TypeChange(int)));
}

NewUser::~NewUser()
{
//    qDebug()<<"~NewUser";
}

void NewUser::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
}

void NewUser::closeEvent(QCloseEvent *event)
{
    name->clear();
    password->clear();
    passwordconfirm->clear();

    QDialog::closeEvent(event);
}

void NewUser::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    default:
        break;
    }

    QDialog::keyPressEvent(event);
}

void NewUser::on_pushButtonok_clicked()
{
    UserInfo temp;

    switch (type->currentIndex()) {
    case 0:
        temp.type = "Viewer";
        break;
    case 1:
        temp.type = "Normal";
        break;
    case 2:
        temp.type = "Admin";
        break;
    }

//    temp.type = type->currentText();
    temp.name = name->text();
    temp.password = password->text();

    foreach (UserInfo comp, Config::instance()->getUserList())
    {
        if ((comp.type == temp.type) && (comp.name == temp.name))
        {
            Utils::MessageBoxWarning(tr("Warning"), tr("Have the same name and type!"));
            return;
        }
    }

    if (password->text() == passwordconfirm->text()
            && !password->text().isEmpty())
    {
        Config::instance()->getUserList().append(temp);
        emit addNewUser();
        close();
    }
}

void NewUser::on_pushButtoncancel_clicked()
{
    close();
}

void NewUser::showWidget(int i)
{
    type->setCurrentIndex(i);
    name->setFocus();

    show();
}

void NewUser::TypeChange(int index)
{
    UserInfo curUser = Config::instance()->getCurUser();

    if (curUser.type == "Viewer" && index > 0)
    {
        Utils::MessageBoxWarning(tr("Warning"), tr("You are Viewer, No permission to create the user for this Type!"));
        type->setCurrentIndex(0);
    }
    else if (curUser.type == "Normal" && index > 1)
    {
        Utils::MessageBoxWarning(tr("Warning"), tr("You are Normal, No permission to create the user for this Type"));
        type->setCurrentIndex(1);
    }
}
