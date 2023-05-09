#include "cfgsettingnetworkstreammodeselect.h"
#include "ui_cfgsettingnetworkstreammodeselect.h"
#include "device/device_trn.h"
#include "deviceItem/deviceitem_trn.h"

CfgSettingNetworkStreamModeSelect::CfgSettingNetworkStreamModeSelect(DeviceItem *devItemBase, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::CfgSettingNetworkStreamModeSelect)
{
    ui->setupUi(this);
    _devItemBase = devItemBase;
    Device_Trn *dev = qobject_cast<Device_Trn*>(_devItemBase->getAES70Device());
    if (dev)
    {
        ui->input1_8->setCurrentIndex(dev->getConfigData().NetworkMode[DeviceItem_Trn::Port_Network_Input_Bank1]);
        ui->input9_16->setCurrentIndex(dev->getConfigData().NetworkMode[DeviceItem_Trn::Port_Network_Input_Bank2]);
        ui->output1_8->setCurrentIndex(dev->getConfigData().NetworkMode[DeviceItem_Trn::Port_Network_Output_Bank1]);
        ui->output9_16->setCurrentIndex(dev->getConfigData().NetworkMode[DeviceItem_Trn::Port_Network_Output_Bank2]);
    }
}

CfgSettingNetworkStreamModeSelect::~CfgSettingNetworkStreamModeSelect()
{
    delete ui;
}

void CfgSettingNetworkStreamModeSelect::on_pushButton_ok_clicked()
{
    _devItemBase->replacePort(DeviceItem_Trn::Port_Network_Input_Bank1, ui->input1_8->currentIndex());
    _devItemBase->replacePort(DeviceItem_Trn::Port_Network_Input_Bank2, ui->input9_16->currentIndex());
    _devItemBase->replacePort(DeviceItem_Trn::Port_Network_Output_Bank1, ui->output1_8->currentIndex());
    _devItemBase->replacePort(DeviceItem_Trn::Port_Network_Output_Bank2, ui->output9_16->currentIndex());

    Device_Trn *dev = qobject_cast<Device_Trn*>(_devItemBase->getAES70Device());
    if (dev != NULL)
    {
        dev->sendNetworkStreamModeSelect(DeviceItem_Trn::Port_Network_Input_Bank1, ui->input1_8->currentIndex());
        dev->sendNetworkStreamModeSelect(DeviceItem_Trn::Port_Network_Input_Bank2, ui->input9_16->currentIndex());
        dev->sendNetworkStreamModeSelect(DeviceItem_Trn::Port_Network_Output_Bank1, ui->output1_8->currentIndex());
        dev->sendNetworkStreamModeSelect(DeviceItem_Trn::Port_Network_Output_Bank2, ui->output9_16->currentIndex());
    }

    close();
}

void CfgSettingNetworkStreamModeSelect::on_pushButton_cancel_clicked()
{
    close();
}
