#include "stable.h"
#include <QDebug>
#include "logindialog.h"
#include "device/devicemanager.h"
#include "device/AES70device.h"
#include "ui_logindialog.h"
#include "Frame/mainwindow.h"


LoginDialog::LoginDialog(bool isLockSys, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::LoginDialog)
{
    _ui->setupUi(this);
    _isLockSys = isLockSys;
    _ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    connect(_ui->comboBox_type, SIGNAL(currentIndexChanged(int)), this, SLOT(TypeChanged(int)));
    connect(_ui->comboBox_name, SIGNAL(currentIndexChanged(int)), this, SLOT(NameChanged(int)));
    connect(_ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(_ui->pushButton_ok, SIGNAL(clicked()), this, SLOT(CheckPassword()));

    if(!isLockSys)
    {
        int idx = 0;
        _ui->comboBox_type->setCurrentIndex(2);
        foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
        {
            QList<QNetworkAddressEntry>entryList = netInterface.addressEntries();
            foreach(QNetworkAddressEntry entry, entryList)
            {
                if (entry.ip().toString().contains(".")
                        && netInterface.hardwareAddress() != NULL)
                {
                    _ipList.append(entry.ip().toString());
                    _itrList.append(netInterface);
                    QString itm = QString(netInterface.humanReadableName() + "(" + entry.ip().toString() + ")");
                    _ui->networkselect->addItem(itm);
                    if(Config::instance()->getCurIp() == entry.ip().toString())
                        idx = _ui->networkselect->findText(itm, Qt::MatchCaseSensitive);
                }
            }
        }
        _ui->networkselect->setCurrentIndex(idx);
    }
    else
    {
        setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint); //隐藏按钮X

        _ui->label_4->hide();
        _ui->networkselect->hide();

        _ui->comboBox_type->setEnabled(false);
        _ui->comboBox_name->setEnabled(false);

        UserInfo curUser = Config::instance()->getCurUser();
        _ui->comboBox_type->setCurrentText(curUser.type);
        _ui->comboBox_name->clear();
        _ui->comboBox_name->addItem(curUser.name);
        _ui->pushButton_cancel->hide();
//        _ui->pushButton_ok->move(190, 170);
    }

    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
    {
        setWindowFlags(Qt::FramelessWindowHint);
        installEventFilter(this);
        this->layout()->setSpacing(15);
        this->layout()->setContentsMargins(20, 10, 20, 50);
        _ui->gridLayout->setContentsMargins(80, 30, 80, 0);

        _ui->pushButton_ok->setFixedSize(70, 25);
        _ui->pushButton_cancel->setFixedSize(70, 25);

        QPixmap pixmap(":/Image/Fangtu/LoginImage_1.png");
        float pixRatio = 0.7;
        int w = pixRatio * pixmap.width();
        int h = pixRatio * pixmap.height();
        QPixmap pixEctype =  pixmap.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        _ui->widget_image->setFixedSize(w, h);

        QLabel *label_logo = new QLabel;
        label_logo->setParent(_ui->widget_image);
        label_logo->setFixedSize(_ui->widget_image->size());
        label_logo->setPixmap(pixEctype);
        setFixedSize(w+20*2, 60+h+30+160+30+60);
        setWindowTitle(tr("FionTu"));
    }
    else {
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); //隐藏按钮?
        _ui->widget_image->setVisible(false);
        resize(357, 160);
    }
    _ui->lineEdit_password->setFocus();
}

LoginDialog::~LoginDialog()
{
//    qDebug("~LoginDialog");
    delete _ui;
}

QString LoginDialog::getSelectUserType()
{
//    _ui->comboBox_type->setCurrentIndex(1);
    QString userType =  "Viewer";
    switch (_ui->comboBox_type->currentIndex()) {
    case LOGINDLG_VIEWER:
        userType =  "Viewer";
        break;
    case LOGINDLG_NORMAL:
        userType =  "Normal";
        break;
    case LOGINDLG_ADMIN:
        userType =  "Admin";
        break;
    }
    return userType;
}

bool LoginDialog::eventFilter(QObject *watched, QEvent *event)//hrp
{
    static QPoint mousePoint;
    static bool mousePressed = false;
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (!mouseEvent) return QWidget::eventFilter(watched, event);
//    qDebug() << __FUNCTION__ << __LINE__ << mouseEvent->globalPos() << mousePoint << mousePressed << &mousePoint << &mousePressed;
    if (mouseEvent->type() == QEvent::MouseButtonPress) {
        if (mouseEvent->button() == Qt::LeftButton) {
            mousePressed = true;
            mousePoint = mouseEvent->globalPos() - this->pos();
            return true;
        }
    } else if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        mousePressed = false;
        return true;
    } else if (mouseEvent->type() == QEvent::MouseMove) {
        if (mousePressed && (mouseEvent->buttons() & Qt::LeftButton)) {
            this->move(mouseEvent->globalPos() - mousePoint);
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}


void LoginDialog::TypeChanged(int i)
{
    Q_UNUSED(i)
    _ui->comboBox_name->clear();

    QString userType = getSelectUserType();

    foreach (UserInfo temp, Config::instance()->getUserList())
    {
        if (temp.type == userType)
        {
            _ui->comboBox_name->addItem(temp.name);
        }
    }
    _ui->comboBox_name->setCurrentIndex(0);
}


void LoginDialog::NameChanged(int i)
{
    Q_UNUSED(i)
    _ui->lineEdit_password->clear();
    _ui->lineEdit_password->setFocus();

    QString userType = getSelectUserType();

    foreach (UserInfo temp, Config::instance()->getUserList())
    {
        if ((temp.type == userType) && (temp.name == _ui->comboBox_name->currentText()))
        {
            Config::instance()->getCurUser().type     = temp.type;
            Config::instance()->getCurUser().name     = temp.name;
            Config::instance()->getCurUser().password = temp.password;
            break;
        }
    }
}

void LoginDialog::CheckPassword(void)
{
    UserInfo curUser = Config::instance()->getCurUser();
    if(_ui->lineEdit_password->text() == curUser.password)
    {
        curUser.type = getSelectUserType();
        curUser.name = _ui->comboBox_name->currentText();
        QString curIp = _ipList.value(_ui->networkselect->currentIndex());
        QNetworkInterface curNet = _itrList.value(_ui->networkselect->currentIndex());
        Config::instance()->setCurNetInfo(curIp, curNet);
        if(curUser.type == "Normal") {
            this->done(2);
        }else {
            this->accept();
        }

    }
    else
    {
        _ui->lineEdit_password->clear();
        _ui->lineEdit_password->setFocus();
        Utils::MessageBoxWarning(tr("Warning"), tr("The password is wrong, please retry!"));
    }
}

void LoginDialog::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void LoginDialog::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Escape:
        _ui->pushButton_cancel->click();
        break;
    case Qt::Key_Return:        //大键区
    case Qt::Key_Enter:         //小键盘区
        _ui->pushButton_ok->click();
        break;
    default:
        QDialog::keyPressEvent(event);
        break;
    }    
}
