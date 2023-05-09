#include "deviceitem_trn.h"
#include "device/AES70device.h"
#include "Frame/projectscene.h"
#include "Frame/projectview.h"
#include "deviceSetting/devsetting_trn.h"
#include <QMdiSubWindow>
#include "Frame/mainwindow.h"
#include "Frame/configview.h"
#include "Frame/configscene.h"
#include "configSetting/cfgsettingnetworkstreammodeselect.h"
#include "device/device_trn.h"
#include "ItemPort.h"
#include "configItem/configitemfactory.h"
#include "configItem/configiteminput.h"
#include "configItem/configitemoutput.h"


DeviceItem_Trn::DeviceItem_Trn(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 130, 473,/*设备宽长*/  parent)
{
    _textLength = 49;

    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) {
        foreach(ItemPort *port, _inPortList)
            port->setEnableMute(true);
        foreach(ItemPort *port, _outPortList)
            port->setEnableMute(true);
    }
}

void DeviceItem_Trn::showSettingDialog()
{
    Device_Trn* trnDev = qobject_cast<Device_Trn*>(_device);
    Q_ASSERT(trnDev !=NULL);
    QString cfgFileName = QString("%1.nmu").arg(_device->getDeviceName());
    QString cfgFilePath = MainWindow::instance()->getSolutionPath(cfgFileName);
    //如果不存在，先增加
    if(!QFile::exists(cfgFilePath)) {
        Device_Trn* trnDev = qobject_cast<Device_Trn*>(_device);
        ConfigScene::createSceneIni(cfgFilePath, trnDev);
        MainWindow::instance()->reflashSolution();
    }

    MainWindow::instance()->openSubWindow(cfgFilePath);
}


void DeviceItem_Trn::showNetworkStreamModeSelect()
{
    CfgSettingNetworkStreamModeSelect form(this);
    form.exec();
}

bool DeviceItem_Trn::canCustomAlgorithm()
{
    if(isBind()){
        return true;
    }
    else {
        return false;
    }
}

void DeviceItem_Trn::initAfterAdd()
{
    DeviceItem::initAfterAdd();
}


void DeviceItem_Trn::clearBeforeDelete()
{
    DeviceItem::clearBeforeDelete();
}

