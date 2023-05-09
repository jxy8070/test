#include "configitemuci.h"
#include "device/device_trn.h"
#include <QHash>

//static QHash<QString, ProcessorData_t> g_processorDataList;

ConfigItemUCI::ConfigItemUCI(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{

}

QWidget *ConfigItemUCI::getSettingForm()
{
    return new QWidget;
}

QByteArray ConfigItemUCI::getDetailData()
{
    QByteArray array;
    return array;
}

QByteArray ConfigItemUCI::getOnlineData()
{
    QByteArray array;
    return array;
}

void ConfigItemUCI::setDetailData(unsigned int *d)
{

}

void ConfigItemUCI::setRtnData(unsigned int *d)
{

}

void ConfigItemUCI::updateCfgSettingLoadedData()
{

}

void ConfigItemUCI::loadParaFromSetting(QSettings *ConfigIni)
{
    _uciTabWidgetItemList.clear();
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_UCI_GROUP));
    this->setSceneTitle(ConfigIni->value(QString("SceneTitle")).toString());
    this->setCustomName(ConfigIni->value(QString(UCI_DEV_NAME)).toString());
    int count = ConfigIni->value(QString(UCI_CTRLWID_COUNT)).toInt();

    for(int idx = 0; idx < count; idx++) {
        ConfigIni->beginGroup(QString(UCI_CTRLWID_NO).arg(idx));
        _uciTabWidgetItemList.append(new Uci_data_t(
                                         ConfigIni->value(QString(UCI_CTRLWID_TYPE)).toInt(),
                                         ConfigIni->value(QString(UCI_CTRLWID_TYPENAME)).toString().toUtf8(),
                                         ConfigIni->value(QString(UCI_CTRLWID_DEVNAME)).toString().toUtf8(),
                                         ConfigIni->value(QString(UCI_CTRLWID_USERLAB)).toString().toUtf8(),
                                         ConfigIni->value(QString(UCI_CTRLWID_CH)).toInt(),
                                         ConfigIni->value(QString(UCI_CTRLWID_PAGE)).toInt(),
                                         ConfigIni->value(QString(UCI_CTRLWID_POS)).toInt(),
                                         ConfigIni->value(QString(UCI_CTRLWID_VISIBLE)).toBool()
                                         ));
        if((_uciTabWidgetItemList.last()->s_cfgType & FCT_FAMILY_MASK) == CFG_TYPE_INPUT_1
                || (_uciTabWidgetItemList.last()->s_cfgType & FCT_FAMILY_MASK) == CFG_TYPE_OUTPUT_1) {
            _uciTabWidgetItemList.last()->setGainRange(ConfigIni->value(QString(UCI_CTRLWID_MINGAIN)).toInt(),
                                                       ConfigIni->value(QString(UCI_CTRLWID_MAXGAIN)).toInt());
        }else if((_uciTabWidgetItemList.last()->s_cfgType & FCT_FAMILY_MASK) == CFG_TYPE_MIXER_4X1) {
            _uciTabWidgetItemList.last()->setMixerNodesSwitch(ConfigIni->value(QString(UCI_CTRLWID_MIXERNODESSWITCH)).toBool());
        }
        ConfigIni->endGroup();
    }
}

void ConfigItemUCI::saveParaToSetting(QSettings *ConfigIni)
{

    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_UCI_GROUP));
    ConfigIni->setValue(QString("SceneTitle"), getSceneTitle());
    ConfigIni->setValue(QString(UCI_DEV_NAME), customName());
    ConfigIni->setValue(QString(UCI_DEV_AES70DEV_IP), _trnDev->getDeviceIp());
    ConfigIni->setValue(QString(UCI_DEV_AES70DEV_DEVTYPE), _trnDev->getDeviceType());
    ConfigIni->setValue(QString(UCI_DEV_AES70DEV_CNAME), _trnDev->getDeviceName());
    ConfigIni->setValue(QString(UCI_CTRLWID_COUNT), getTabWidgetItemList().count());
    int idx = 0;
    foreach(Uci_data_t *item, _uciTabWidgetItemList) {
        ConfigIni->beginGroup(QString(UCI_CTRLWID_NO).arg(idx++));
        ConfigIni->setValue(QString(UCI_CTRLWID_TYPE), item->s_cfgType);
        ConfigIni->setValue(QString(UCI_CTRLWID_TYPENAME), item->s_modelType);
        ConfigIni->setValue(QString(UCI_CTRLWID_DEVNAME), item->s_name);
        ConfigIni->setValue(QString(UCI_CTRLWID_USERLAB), item->s_userLab);
        if((item->s_cfgType & FCT_FAMILY_MASK) == CFG_TYPE_INPUT_1
                || (item->s_cfgType & FCT_FAMILY_MASK) == CFG_TYPE_OUTPUT_1) {
            ConfigIni->setValue(QString(UCI_CTRLWID_MAXGAIN), item->s_gainRange.maxValue);
            ConfigIni->setValue(QString(UCI_CTRLWID_MINGAIN), item->s_gainRange.minValue);
        }else if((item->s_cfgType & FCT_FAMILY_MASK) == CFG_TYPE_MIXER_4X1) {
            ConfigIni->setValue(QString(UCI_CTRLWID_MIXERNODESSWITCH), item->s_mixerNodesSwitch);
        }

        ConfigIni->setValue(QString(UCI_CTRLWID_CH), item->s_channel);
        ConfigIni->setValue(QString(UCI_CTRLWID_VISIBLE), item->s_visible);
        ConfigIni->setValue(QString(UCI_CTRLWID_PAGE), item->s_page);
        ConfigIni->setValue(QString(UCI_CTRLWID_POS), item->s_pos);

        ConfigIni->endGroup();
    }
}

QVariant ConfigItemUCI::setLuaPinValue(CLuaPinParam_PC *param, QVariant val)
{
    return QVariant();
}

#include "Frame/mainwindow.h"
#include <QMdiSubWindow>
#include "Frame/projectview.h"
#include "Frame/projectscene.h"
#include "deviceItem/deviceItem.h"
void ConfigItemUCI::saveDataToUciTable(QString fileName)
{
    QSettings *ConfigIni = new QSettings(fileName, QSettings::IniFormat, 0);

    //外围设备
    ProjectScene *proScene = NULL;
    foreach(QMdiSubWindow *subWin, MainWindow::instance()->getMdiArea()->subWindowList()) {
        AbsGraphView *absView = qobject_cast<AbsGraphView *>(subWin->widget());
        if(!absView) continue;
        if(absView->getType() == VIEW_PROJECT &&
           absView->windowTitle() == getAES70Dev()->getCurRoomName()) {
            proScene = qobject_cast<ProjectView *>(absView)->scene();
            break;
        }
    }
    if(proScene) {
        ConfigIni->remove(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_GROUP));
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_GROUP));
        int idx = 0;
        foreach(QGraphicsItem *item, proScene->items()) {
            if(item->type() == DeviceItem::Type) {
                DeviceItem *devItem = qgraphicsitem_cast<DeviceItem *>(item);
                ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DATA).arg(idx++));
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DEVNAME),
                                    devItem->getAES70Device()->getDeviceName());
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DEVTYPE),
                                    devItem->getType());
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DEVIP),
                                    devItem->getAES70Device()->getDeviceIp());
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DEVMAC),
                                    devItem->getAES70Device()->getAES70Mac());
                ConfigIni->endGroup();
            }
        }
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_COUNT), idx);
        ConfigIni->endGroup();
    }
}

QList<QString> ConfigItemUCI::openDatafromUciFile(QString fileName)
{
    QList<QString> MacList;
    QSettings *ConfigIni = new QSettings(fileName, QSettings::IniFormat, 0);
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_UCI_PROC_GROUP));
    int count = ConfigIni->value(QString(CONFIG_DEVICE_DEV_UCI_PROC_COUNT)).toInt();
    for(int idx = 0; idx < count; idx++) {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_UCI_PROC_DATA).arg(idx));
        MacList.append(ConfigIni->value(QString(CONFIG_DEVICE_DEV_UCI_PROC_MAC)).toString());
        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
    return MacList;
}

QString ConfigItemUCI::getAES70DevMac()
{
    return _trnDev->getAES70Mac();
}

QMap<int, ConfigItemUCI::Uci_data_t *> ConfigItemUCI::getTabWidgetItem(CfgType cfgType)
{
    QMap<int, Uci_data_t *> data;
//    qDebug() << __FUNCTION__ << __LINE__ << QString::number(cfgType, 16) << _uciTabWidgetItemList.count();
    foreach(Uci_data_t *item, _uciTabWidgetItemList) {
        if(item->s_cfgType == cfgType) {
            data.insert(item->s_pos, item);
        }
    }
    return data;
}

QMap<int, ConfigItemUCI::Uci_data_t *> ConfigItemUCI::getTabWidgetItem(int page)
{
    QMap<int, Uci_data_t *> data;
//    qDebug() << __FUNCTION__ << __LINE__ << QString::number(cfgType, 16) << _uciTabWidgetItemList.count();
    foreach(Uci_data_t *item, _uciTabWidgetItemList) {
        if(item->s_page == page) {
            data.insert(item->s_pos, item);
        }
    }
    return data;
}


