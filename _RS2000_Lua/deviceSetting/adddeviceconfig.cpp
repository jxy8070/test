#include "stable.h"
#include "adddeviceconfig.h"
#include "device/devicemanager.h"
#include "sysdbutils.h"
#include "device/devicemodel.h"
#include "device/devicemanager.h"
#include "device/dantedevice.h"
//wj 需要删除
//#include "config.h"

AddDeviceConfig::AddDeviceConfig(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setModal(true);

    ok->setEnabled(false);
    setAttribute(Qt::WA_DeleteOnClose);
//    QRegExp limit("[0-9a-fA-F]");
//    QRegExpValidator *limitV = new QRegExpValidator(limit, this);
//    lineEdit->setValidator(limitV);

    lineEdit->setMaxLength(12);
//    lineEdit_2->setMaxLength(4);
//    lineEdit_3->setMaxLength(8);
    lineEdit->setFocus();

    connect(lineEdit, SIGNAL(textEdited(QString)), this, SLOT(line_edit(QString)));
//    connect(lineEdit_2, SIGNAL(textEdited(QString)), this, SLOT(line_edit(QString)));
//    connect(lineEdit_3, SIGNAL(textEdited(QString)), this, SLOT(line_edit(QString)));

    connect(ok, SIGNAL(clicked()), this, SLOT(ok_clicked()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

    comboBox->addItem(Utils::getDevTypeName(DEV_TYPE_DNTE_SPK));
    comboBox->addItem(Utils::getDevTypeName(DEV_TYPE_DNTE_AMP));
    comboBox->addItem(Utils::getDevTypeName(DEV_TYPE_DNTE_AMP_ONEOUT));
    comboBox->addItem(Utils::getDevTypeName(DEV_TYPE_DNTE_AMP4_4));
    comboBox->addItem(Utils::getDevTypeName(DEV_TYPE_DNTE_TRN));
}

AddDeviceConfig::~AddDeviceConfig()
{
//    qDebug()<<"~AddDeviceConfig";
}

//void AddDeviceConfig::clear()
//{
//    lineEdit->clear();
//    lineEdit_2->clear();
//    lineEdit_3->clear();
//}

//void AddDeviceConfig::showEvent(QShowEvent *event)
//{

//    QDialog::showEvent(event);
//}

//void AddDeviceConfig::closeEvent(QCloseEvent *event)
//{
////    clear();

//    QDialog::closeEvent(event);
//}

void AddDeviceConfig::keyPressEvent(QKeyEvent *event)
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

void AddDeviceConfig::line_edit(QString string)
{
    if (sender() == lineEdit)
    {
        if (string.length() == 12)
        {
            ok->setEnabled(true);
        }
        else
        {
            ok->setEnabled(false);
        }
    }
//    else if (sender() == lineEdit_2)
//    {
//        if (string.length() == 4)
//        {
//            lineEdit_3->setFocus();
//        }
//    }
//    else if (sender() == lineEdit_3)
//    {
//        if (string.length() == 8)
//        {

//        }
//        else
//        {
//            if (ok->isEnabled())
//            {
//                ok->setEnabled(false);
//            }
//        }
//    }
}

void AddDeviceConfig::ok_clicked()
{
//    //如果序列号相同，则不能添加
//    QString mac = QString("001DC1%1").arg(lineEdit->text().toUpper());
//    QString serials = QString("%1-%2-%3").arg(lineEdit->text()).
//            arg(lineEdit_2->text()).arg(lineEdit_3->text());
//    QString deviceType = comboBox->currentText();
//    if(DeviceManager::instance()->existBySerials(serials))
//    {
//        QMessageBox::information(NULL, tr("Information"),
//            tr("Have the same serials(Type=%1)").arg(deviceType));
//        return;
//    }
//    else{
//        DeviceManager::instance()->addDevice(mac, deviceType, serials);
//    }
//    emit addNewConfig();
//    close();
    DevType devNum = DEV_TYPE_DNTE_SPK;
    switch (comboBox->currentIndex()) {
    case 0:
        devNum = DEV_TYPE_DNTE_SPK;
        break;
    case 1:
        devNum = DEV_TYPE_DNTE_AMP;
        break;
    case 2:
        devNum = DEV_TYPE_DNTE_AMP_ONEOUT;
        break;
    case 3:
        devNum = DEV_TYPE_DNTE_AMP4_4;
        break;
    case 4:
        devNum = DEV_TYPE_DNTE_TRN;
        break;
    default:
        devNum = DEV_TYPE_DNTE_SPK;
        break;
    }

    DanteDevice* newDev = DanteDevice::createDevice(lineEdit->text(), devNum, "", "");
    newDev->setDeviceIp("0.0.0.0");
    newDev->switchStatus(Dev_Offline);
    DeviceManager::instance()->getDeviceModel()->insertDevice(0, newDev);

    close();
}
