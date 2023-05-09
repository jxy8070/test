#include "snapshotsavemeterdata.h"

#include "configItem/configitemmeter.h"

void SnapshotSaveMeterData::saveMeterData(ConfigItem *cfgItem, int type)
{
    ConfigItemMeter *meterItem = qgraphicsitem_cast<ConfigItemMeter *>(cfgItem);

    _moduleNum.insert(meterItem->customName(), meterItem->getMeterNum());
    MeterData mData;
    memset(&mData, 0, sizeof(MeterData));

    mData._meterNum = meterItem->getMeterNum();
    mData._rowcol   = meterItem->getRowCol();
    for(int i = 0; i < meterItem->getMeterNum(); i++)
    {
        mData._holdTime[i]     = meterItem->getHoldTime(i);
        mData._holdEnable[i]   = meterItem->getHoldEnable(i);
        mData._infiniteHold[i] = meterItem->getInfiniteHold(i);
        QByteArray array = meterItem->getUserName(i).toUtf8();
//        array.append(meterItem->getUserName(i));
        mData._userName[i]     = (char *)malloc(array.length() + 1);
        memset(mData._userName[i], 0, sizeof(array.length()));
        memcpy(mData._userName[i], array, array.length());
        mData._userName[i][array.length()] = '\0';
//        qDebug()<<__FUNCTION__<<__LINE__<<mData._userName[i]<<mData._userName[i];
    }

    if(meterItem->getCfgType() == CFG_TYPE_METER_1)
    {
        _meter_1_DataList[type].insert(meterItem->customName(), mData);
    }
    if(meterItem->getCfgType() == CFG_TYPE_METER_2)
    {
        _meter_2_DataList[type].insert(meterItem->customName(), mData);
    }
    if(meterItem->getCfgType() == CFG_TYPE_METER_4)
    {
        _meter_4_DataList[type].insert(meterItem->customName(), mData);
    }
    if(meterItem->getCfgType() == CFG_TYPE_METER_8)
    {
        _meter_8_DataList[type].insert(meterItem->customName(), mData);
    }

}

void SnapshotSaveMeterData::setMeterData(ConfigItem *cfgItem, int type)
{
    ConfigItemMeter *meterItem = qgraphicsitem_cast<ConfigItemMeter *>(cfgItem);

    if(meterItem->getCfgType() == CFG_TYPE_METER_1)
    {
        snapToConfig(_meter_1_DataList[type], meterItem);
    }
    if(meterItem->getCfgType() == CFG_TYPE_METER_4)
    {
        snapToConfig(_meter_4_DataList[type], meterItem);
    }
    if(meterItem->getCfgType() == CFG_TYPE_METER_8)
    {
        snapToConfig(_meter_8_DataList[type], meterItem);
    }

    quint32 moduleNum = _moduleNum.find(meterItem->customName()).value();
    meterItem->setModuleNum(moduleNum);

    meterItem->updateCfgSettingLoadedData();

}

void SnapshotSaveMeterData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    QList<QString> nameList = getMeterDataList(type, pattern).keys();
    int unimoduleNum = getMeterDataList(type, pattern).count();
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_METER_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_METER).arg(idx));
        quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);

        for (int i = 0; i < 8; ++i)
        {
            QString userName(getMeterDataList(type, pattern)[nameList.at(idx)]._userName[i]);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_METER_USERNAME).arg(i), userName);
        }
        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

void SnapshotSaveMeterData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_METER_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_METER).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();

        QString userName[8];
        for(int i = 0; i < 8; ++i)
        {
            userName[i] = ConfigIni->value(QString(CONFIG_DEVICE_DEV_METER_USERNAME).arg(i)).toString();
        }

        setMeterData(devName, type, pattern, userName, moduleNum);
        ConfigIni->endGroup();
    }

    ConfigIni->endGroup();
}

void SnapshotSaveMeterData::snapToConfig(QHash<QString, MeterData> data, ConfigItemMeter *meterItem)
{
    if(!findCfgItemData<MeterData>(data, meterItem)) return;

    MeterData mdata;
    memset(&mdata, 0, sizeof(MeterData));
    mdata = data.find(meterItem->customName()).value();
    for(int i = 0; i < meterItem->getMeterNum(); i++)
    {
        meterItem->setHoldTime(i, mdata._holdTime[i]);
        meterItem->setHoldEnable(i, mdata._holdEnable[i]);
        meterItem->setInfiniteHole(i, mdata._infiniteHold[i]);
        QString name(mdata._userName[i]);
        meterItem->setUserName(i, name);
//        qDebug()<<__FUNCTION__<<__LINE__<<mdata._meterNum<<mdata._userName[i]<<name;
    }
}

QHash<QString, SnapshotSaveMeterData::MeterData> SnapshotSaveMeterData::getMeterDataList(CfgType type, int pattern)
{
    QHash<QString, MeterData> list;
    switch (type) {
    case CFG_TYPE_METER_1:
        list = _meter_1_DataList[pattern];
        break;
    case CFG_TYPE_METER_2:
        list = _meter_2_DataList[pattern];
        break;
    case CFG_TYPE_METER_4:
        list = _meter_4_DataList[pattern];
        break;
    case CFG_TYPE_METER_8:
        list = _meter_8_DataList[pattern];
        break;
    default:
        break;
    }
    return list;
}

void SnapshotSaveMeterData::setMeterData(QString devName, CfgType type, int pattern, QString *userName, quint32 moduleNum)
{
//    qDebug()<<__FUNCTION__<<__LINE__<<devName;
    MeterData body;
    body._rowcol = 0xFFFFFFFF;
    body._meterNum = getMeterNum(type);
    for(int i = 0; i < 8; i++)
    {
        body._holdTime[i] = 100;
        body._holdEnable[i] = true;
        body._infiniteHold[i] = false;

//        qDebug()<<__FUNCTION__<<__LINE__<<userName[i];
        QByteArray array = userName[i].toUtf8();
        body._userName[i] = (char *)malloc(array.length() + 1);
        memset(body._userName[i], 0, sizeof(array.length()));
        memcpy(body._userName[i], array, array.length());
        body._userName[i][array.length()] = '\0';
    }

    _moduleNum.insert(devName, moduleNum);

    if(type == CFG_TYPE_METER_1)
    {
        _meter_1_DataList[pattern].insert(devName, body);
    }
    if(type == CFG_TYPE_METER_2)
    {
        _meter_2_DataList[pattern].insert(devName, body);
    }
    if(type == CFG_TYPE_METER_4)
    {
        _meter_4_DataList[pattern].insert(devName, body);
    }
    if(type == CFG_TYPE_METER_8)
    {
        _meter_8_DataList[pattern].insert(devName, body);
    }
}

int SnapshotSaveMeterData::getMeterNum(CfgType type)
{
    switch (type) {
    case CFG_TYPE_METER_1:
        return 1;
    case CFG_TYPE_METER_2:
        return 2;
    case CFG_TYPE_METER_4:
        return 4;
    case CFG_TYPE_METER_8:
        return 8;
    default:
        return 0;
    }
}


