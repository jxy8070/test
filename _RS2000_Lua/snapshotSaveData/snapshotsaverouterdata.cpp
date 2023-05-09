#include "snapshotsaverouterdata.h"
#include "configItem/configitemrouter.h"

void SnapshotSaveRouterData::saveRouterData(ConfigItem *cfgItem, int type)
{
    ConfigItemRouter *routerItem = qgraphicsitem_cast<ConfigItemRouter *>(cfgItem);
    if(routerItem->getCfgType() == CFG_TYPE_ROUTER_4)
    {
        IG_PARACTL_VROUTER4 body;
        for(int col = 0; col < routerItem->getPortNum(); col++)
        {
            body.routerch[col].itoo = routerItem->getRouterInfo(col);
//            qDebug()<<__FUNCTION__<<__LINE__<<routerItem->getRouterInfo(col)<<body.routerch[col].itoo;
        }
        _router_4_DataList[type].insert(routerItem->customName(), body);
        _moduleNum.insert(routerItem->customName(), routerItem->getModuleNum());
    }
    if(routerItem->getCfgType() == CFG_TYPE_ROUTER_8)
    {
        IG_PARACTL_VROUTER8 body;
        for(int row = 0; row < routerItem->getPortNum(); row++)
        {
            body.routerch[row].itoo = routerItem->getRouterInfo(row);
        }
        _router_8_DataList[type].insert(routerItem->customName(), body);
        _moduleNum.insert(routerItem->customName(), routerItem->getModuleNum());
    }
    if(routerItem->getCfgType() == CFG_TYPE_ROUTER_16)
    {
        IG_PARACTL_VROUTER16 body;
        for(int row = 0; row < routerItem->getPortNum(); row++)
        {
            body.routerch[row].itoo = routerItem->getRouterInfo(row);
        }
        _router_16_DataList[type].insert(routerItem->customName(), body);
        _moduleNum.insert(routerItem->customName(), routerItem->getModuleNum());
    }
    if(routerItem->getCfgType() == CFG_TYPE_ROUTER_32)
    {
        //待实现
    }
    if(routerItem->getCfgType() == CFG_TYPE_ROUTER_64)
    {
        //待实现
    }
}

void SnapshotSaveRouterData::setRouterData(ConfigItem *cfgItem, int type)
{
    ConfigItemRouter *routerItem = qgraphicsitem_cast<ConfigItemRouter *>(cfgItem);

    quint32 moduleNum = _moduleNum.find(routerItem->customName()).value();
    routerItem->setModuleNum(moduleNum);

    if(routerItem->getCfgType() == CFG_TYPE_ROUTER_4)
    {
        if(!findCfgItemData<IG_PARACTL_VROUTER4>(_router_4_DataList[type], cfgItem)) return;

        IG_PARACTL_VROUTER4 body = _router_4_DataList[type].find(routerItem->customName()).value();
        for(int col = 0; col < routerItem->getPortNum(); col++)
        {
            routerItem->setRouterInfo(col, body.routerch[col].itoo);
//            qDebug()<<__FUNCTION__<<__LINE__<<body.routerch[col].itoo;
        }
        routerItem->updateCfgSettingLoadedData();
    }
    if(routerItem->getCfgType() == CFG_TYPE_ROUTER_8)
    {
        if(!findCfgItemData<IG_PARACTL_VROUTER8>(_router_8_DataList[type], cfgItem)) return;

        IG_PARACTL_VROUTER8 body = _router_8_DataList[type].find(routerItem->customName()).value();
        for(int row = 0; row < routerItem->getPortNum(); row++)
        {
            routerItem->setRouterInfo(row, body.routerch[row].itoo);
        }
        routerItem->updateCfgSettingLoadedData();
    }
    if(routerItem->getCfgType() == CFG_TYPE_ROUTER_16)
    {
        if(!findCfgItemData<IG_PARACTL_VROUTER16>(_router_16_DataList[type], cfgItem)) return;

        IG_PARACTL_VROUTER16 body = _router_16_DataList[type].find(routerItem->customName()).value();
        for(int row = 0; row < routerItem->getPortNum(); row++)
        {
            routerItem->setRouterInfo(row, body.routerch[row].itoo);
        }
        routerItem->updateCfgSettingLoadedData();
    }
}

void SnapshotSaveRouterData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    if(!_router_4_DataList[pattern].isEmpty() && type == CFG_TYPE_ROUTER_4)
    {
        QList<QString> nameList = _router_4_DataList[pattern].keys();
        int unimoduleNum = _router_4_DataList[pattern].count();
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_ROUTER_TYPE).arg(type));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVOUTCOUNT), 4);

        for(int idx = 0; idx < unimoduleNum; idx++)
        {
            ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_ROUTER).arg(idx));
            quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
            for(int j = 0; j < 4; j++)
            {
                quint16 vlu = _router_4_DataList[pattern][nameList.at(idx)].routerch[j].itoo;
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_ROUTER_ROUTERINFO).arg(j), vlu);
            }
            ConfigIni->endGroup();
        }
        ConfigIni->endGroup();
    }
    if(!_router_8_DataList[pattern].isEmpty() && type == CFG_TYPE_ROUTER_8)
    {
        QList<QString> nameList = _router_8_DataList[pattern].keys();
        int unimoduleNum = _router_8_DataList[pattern].count();
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_ROUTER_TYPE).arg(type));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVOUTCOUNT), 8);

        for(int idx = 0; idx < unimoduleNum; idx++)
        {
            ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_ROUTER).arg(idx));
            quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
            for(int j = 0; j < 8; j++)
            {
                quint16 vlu = _router_8_DataList[pattern][nameList.at(idx)].routerch[j].itoo;
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_ROUTER_ROUTERINFO).arg(j), vlu);
            }
            ConfigIni->endGroup();
        }
        ConfigIni->endGroup();
    }
    if(!_router_16_DataList[pattern].isEmpty() && type == CFG_TYPE_ROUTER_16)
    {
        QList<QString> nameList = _router_16_DataList[pattern].keys();
        int unimoduleNum = _router_16_DataList[pattern].count();
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_ROUTER_TYPE).arg(type));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVOUTCOUNT), 16);

        for(int idx = 0; idx < unimoduleNum; idx++)
        {
            ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_ROUTER).arg(idx));
            quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
            for(int j = 0; j < 16; j++)
            {
                quint16 vlu = _router_16_DataList[pattern][nameList.at(idx)].routerch[j].itoo;
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_ROUTER_ROUTERINFO).arg(j), vlu);
            }
            ConfigIni->endGroup();
        }
        ConfigIni->endGroup();
    }
//    if(routerItem->getCfgType() == CFG_TYPE_ROUTER_32)
//    {
//        //待实现
//    }
//    if(routerItem->getCfgType() == CFG_TYPE_ROUTER_64)
//    {
//        //待实现
//    }
}

void SnapshotSaveRouterData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_ROUTER_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();
    int outCount = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVOUTCOUNT)).toInt();

    for(int idx = 0; idx <unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_ROUTER).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toInt();
        QList<quint16> itooList;
        for(int j = 0; j < outCount; j++)
        {
            itooList.append(ConfigIni->value(QString(CONFIG_DEVICE_DEV_ROUTER_ROUTERINFO).arg(j)).toUInt());
        }
//        qDebug()<<__FUNCTION__<<__LINE__<<itooList.length()<<itooList.count()<<devName<<moduleNum;
        ConfigIni->endGroup();

        if(type == CFG_TYPE_ROUTER_4)
        {
            IG_PARACTL_VROUTER4 body;
            for(int i = 0; i < itooList.length(); i++)
            {
                body.routerch[i].itoo = itooList.at(i);
            }
            _router_4_DataList[pattern].insert(devName, body);
            _moduleNum.insert(devName, moduleNum);
        }
        if(type == CFG_TYPE_ROUTER_8)
        {
            IG_PARACTL_VROUTER8 body;
            for(int i = 0; i < itooList.length(); i++)
            {
                body.routerch[i].itoo = itooList.at(i);
            }
            _router_8_DataList[pattern].insert(devName, body);
            _moduleNum.insert(devName, moduleNum);
        }
        if(type == CFG_TYPE_ROUTER_16)
        {
            IG_PARACTL_VROUTER16 body;
            for(int i = 0; i < itooList.length(); i++)
            {
                body.routerch[i].itoo = itooList.at(i);
            }
            _router_16_DataList[pattern].insert(devName, body);
            _moduleNum.insert(devName, moduleNum);
        }

    }
    ConfigIni->endGroup();
}


