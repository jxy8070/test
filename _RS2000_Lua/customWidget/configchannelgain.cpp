#include "configchannelgain.h"
#include "ui_configchannelgain.h"
#include <QMessageBox>
#include <QDebug>
#include "device/AES70device.h"
#include "device/device_trn.h"
#include "deviceItem/deviceitem_trn.h"

ConfigChannelGain::ConfigChannelGain(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::ConfigChannelGain)
{
    ui->setupUi(this);

    _channel = 0;
    _lastCheckId = 20;
    _btnGroup = new QButtonGroup;
    _btnGroup->addButton(ui->radioButton_1, 0);
    _btnGroup->addButton(ui->radioButton_2, 1);
    _btnGroup->addButton(ui->radioButton_3, 2);
    _btnGroup->addButton(ui->radioButton_4, 3);
    _btnGroup->addButton(ui->radioButton_5, 4);
    _btnGroup->addButton(ui->radioButton_6, 5);
    _btnGroup->addButton(ui->radioButton_7, 6);
    _btnGroup->addButton(ui->radioButton_8, 7);
    connect(_btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(onBtnGroupClicked(int)));
}

ConfigChannelGain::~ConfigChannelGain()
{
    delete ui;
}

void ConfigChannelGain::init(DeviceItem *devItemBase, int channel)
{
    _channel = channel;
    _devItemBase  = devItemBase;

//    if (_devItemBase->getDanteDevice()->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devItemBase->getAES70Device());
        ui->pushButton->setChecked(trn->getConfigData().InMic48V[_channel]);
//        qDebug() << __FUNCTION__ << __LINE__ << trn->getConfigData().InMicGain[_channel];
        _btnGroup->button(trn->getConfigData().InMicGain[_channel])->setChecked(true);
    }
    _lastCheckId = _btnGroup->checkedId();
    setTitle(QString("In %1").arg(channel+1));
}

void ConfigChannelGain::onBtnGroupClicked(int i)
{
    Q_UNUSED(i)
    QMessageBox msgBox(QMessageBox::Question, tr("Save"),
                       tr("The Gain of %1 channel will be set to %2, are you sure continue?").arg(_channel+1).arg(_btnGroup->checkedButton()->text()),
                       NULL);
    msgBox.setWindowFlags(Qt::FramelessWindowHint);
    QPushButton *okButton = msgBox.addButton(tr("Yes"), QMessageBox::AcceptRole);
    QPushButton *cancelButton = msgBox.addButton(tr("No"), QMessageBox::RejectRole);
    msgBox.exec();
    if ((QPushButton*)msgBox.clickedButton() == cancelButton)
    {
        _btnGroup->button(_lastCheckId)->setChecked(true);
        return;
    }

//    if (_devItemBase->getDanteDevice()->getType() == DEV_TYPE_DNTE_TRN)
	{
        Device_Trn *dev = qobject_cast<Device_Trn*>(_devItemBase->getAES70Device());
	    if (dev != NULL)
	    {
	        dev->sendMicGainPack(_channel, _btnGroup->checkedId());
	    }
	}
//    qDebug() << __FUNCTION__ << __LINE__ << _btnGroup->checkedId();
    _lastCheckId = _btnGroup->checkedId();
}

void ConfigChannelGain::on_pushButton_clicked(bool checked)
{
    if (checked == true)
    {
        QMessageBox msgBox(QMessageBox::Question, tr("Save"),
                           tr("The Gain of %1 channel will be set to %2, are you sure continue?").arg(_channel+1).arg(_btnGroup->checkedButton()->text()),
                           NULL);
        msgBox.setWindowFlags(Qt::FramelessWindowHint);
        QPushButton *okButton = msgBox.addButton(tr("Yes"), QMessageBox::AcceptRole);
        QPushButton *cancelButton = msgBox.addButton(tr("No"), QMessageBox::RejectRole);
        msgBox.exec();
        if ((QPushButton*)msgBox.clickedButton() == cancelButton)
        {
            ui->pushButton->setChecked(false);
            return;
        }
    }

//    if (_devItemBase->getDanteDevice()->getType() == DEV_TYPE_DNTE_TRN)
	{
        Device_Trn *dev = qobject_cast<Device_Trn*>(_devItemBase->getAES70Device());
	    if (dev != NULL)
	    {
	        dev->sendPhantomPack(_channel, checked);
	    }
	}
}
