#include "snapshotsaveeqdata.h"

#include "configItem/configitemeq.h"

void SnapshotSaveEqData::saveEqData(ConfigItem *cfgItem, int type)
{
    ConfigItemEq *eqItem = qgraphicsitem_cast<ConfigItemEq *>(cfgItem);

    _moduleNum.insert(eqItem->customName(), eqItem->getModuleNum());

    EqModuleData body;
    body._eqList.clear();
    body._bypess = eqItem->getAllBypass() ? 0x80000000 : 0x00000000;

    for(int i = 0; i < eqItem->getEqDataList().length(); i++)
    {
        EqData eqData;
        eqData.freq = eqItem->getEqDataList().at(i)->freq;
        eqData.gain = eqItem->getEqDataList().at(i)->gain;
        eqData.q    = eqItem->getEqDataList().at(i)->q;
        body._bypess |= (eqItem->getSegBypass(i) << i);
        body._eqList.append(eqData);
    }
    qDebug() << __FUNCTION__ << __LINE__ << QString::number(body._bypess, 2);
    switch(eqItem->getCfgType()) {
    case CFG_TYPE_EQ_1:
        _Eq_1_DataList[type].insert(eqItem->customName(), body);
        break;
    case CFG_TYPE_EQ_3:
        _Eq_3_DataList[type].insert(eqItem->customName(), body);
        break;
    case CFG_TYPE_EQ_5:
        _Eq_5_DataList[type].insert(eqItem->customName(), body);
        break;
    case CFG_TYPE_EQ_7:
        _Eq_7_DataList[type].insert(eqItem->customName(), body);
        break;
    case CFG_TYPE_EQ_10:
        _Eq_10_DataList[type].insert(eqItem->customName(), body);
        break;
    case CFG_TYPE_EQ_16:
        _Eq_16_DataList[type].insert(eqItem->customName(), body);
        break;
    case CFG_TYPE_EQ_1X3:
        _Eq_1x3_DataList[type].insert(eqItem->customName(), body);
        break;
    case CFG_TYPE_EQ_2X3:
        _Eq_2x3_DataList[type].insert(eqItem->customName(), body);
        break;
    case CFG_TYPE_EQ_3X3:
        _Eq_3x3_DataList[type].insert(eqItem->customName(), body);
        break;
    default:
        break;
    }

}

void SnapshotSaveEqData::setEqData(ConfigItem *cfgItem, int type)
{
    ConfigItemEq *eqItem = qgraphicsitem_cast<ConfigItemEq *>(cfgItem);

    if(eqItem->getCfgType() == CFG_TYPE_EQ_1)
    {
        snapToConfig(_Eq_1_DataList[type], eqItem);
        eqItem->updateCfgSettingLoadedData();
    }
    if(eqItem->getCfgType() == CFG_TYPE_EQ_3)
    {
        snapToConfig(_Eq_3_DataList[type], eqItem);
        eqItem->updateCfgSettingLoadedData();
    }
    if(eqItem->getCfgType() == CFG_TYPE_EQ_5)
    {
        snapToConfig(_Eq_5_DataList[type], eqItem);
        eqItem->updateCfgSettingLoadedData();
    }
    if(eqItem->getCfgType() == CFG_TYPE_EQ_7)
    {
        snapToConfig(_Eq_7_DataList[type], eqItem);
        eqItem->updateCfgSettingLoadedData();
    }
    if(eqItem->getCfgType() == CFG_TYPE_EQ_10)
    {
        snapToConfig(_Eq_10_DataList[type], eqItem);
        eqItem->updateCfgSettingLoadedData();
    }
    if(eqItem->getCfgType() == CFG_TYPE_EQ_16)
    {
        snapToConfig(_Eq_16_DataList[type], eqItem);
        eqItem->updateCfgSettingLoadedData();
    }
    if(eqItem->getCfgType() == CFG_TYPE_EQ_1X3)
    {
        snapToConfig(_Eq_1x3_DataList[type], eqItem);
        eqItem->updateCfgSettingLoadedData();
    }
    if(eqItem->getCfgType() == CFG_TYPE_EQ_2X3)
    {
        snapToConfig(_Eq_2x3_DataList[type], eqItem);
        eqItem->updateCfgSettingLoadedData();
    }
    if(eqItem->getCfgType() == CFG_TYPE_EQ_3X3)
    {
        snapToConfig(_Eq_3x3_DataList[type], eqItem);
        eqItem->updateCfgSettingLoadedData();
    }

    quint32 moduleNum = _moduleNum.find(eqItem->customName()).value();
    eqItem->setModuleNum(moduleNum);
}

void SnapshotSaveEqData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    QList<QString> nameList = getEqDataList(type, pattern).keys();
    int unimoduleNum = getEqDataList(type, pattern).count();
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_EQ_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_EQ).arg(idx));
        quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO),moduleNum);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
        int eqListNum = getEqDataList(type, pattern)[nameList.at(idx)]._eqList.count();
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_EQLISTNUM), eqListNum);

        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_EQ_BYPASS),
                            getEqDataList(type, pattern)[nameList.at(idx)]._bypess);
        for (int i = 0; i < eqListNum; ++i)
        {
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_EQ_FREQ).arg(i),
                                getEqDataList(type, pattern)[nameList.at(idx)]._eqList[i].freq);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_EQ_GAIN).arg(i),
                                getEqDataList(type, pattern)[nameList.at(idx)]._eqList[i].gain);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_EQ_Q).arg(i),
                                getEqDataList(type, pattern)[nameList.at(idx)]._eqList[i].q);
        }

        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

void SnapshotSaveEqData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_EQ_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();
    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_EQ).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();
        int eqListNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_EQLISTNUM)).toInt();

        QList<EqData> eqDataList;
        bool bypass = ConfigIni->value(QString(CONFIG_DEVICE_DEV_EQ_BYPASS)).toBool();
        for(int i = 0; i < eqListNum; i++)
        {
            EqData eqData;
            eqData.freq = ConfigIni->value(QString(CONFIG_DEVICE_DEV_EQ_FREQ).arg(i)).toUInt();
            eqData.gain = ConfigIni->value(QString(CONFIG_DEVICE_DEV_EQ_GAIN).arg(i)).toInt();
            eqData.q    = ConfigIni->value(QString(CONFIG_DEVICE_DEV_EQ_Q).arg(i)).toInt();
            eqDataList.append(eqData);
        }
        setEqData(devName, type, pattern, bypass, eqDataList, moduleNum);
        ConfigIni->endGroup();
    }

    ConfigIni->endGroup();
}

void SnapshotSaveEqData::snapToConfig(QHash<QString, EqModuleData> data, ConfigItemEq *eqItem)
{
    if(!findCfgItemData<EqModuleData>(data, eqItem)) return;

    EqModuleData body = data.find(eqItem->customName()).value();

        qDebug()<<__FUNCTION__<<__LINE__<<QString::number(body._bypess, 2)<<body._smooth
               <<body._eqList.at(0).freq
               <<body._eqList.at(0).gain
              <<body._eqList.at(0).q;
    eqItem->setAllBypass(body._bypess & 0x80000000);
    for(int i = 0; i < eqItem->getEqDataList().count(); i++)
    {
        eqItem->setEqData(i, body._bypess & (0x1 << i), body._eqList.at(i).freq, body._eqList.at(i).gain, body._eqList.at(i).q, 0);
    }
}

void SnapshotSaveEqData::setEqData(QString devName, CfgType type, int pattern, bool bypass, QList<EqData> eqDataList, quint32 moduleNum)
{
    EqModuleData body;
    body._bypess = bypass;
    body._smooth = 0;

    for(int i = 0; i < eqDataList.count(); i++)
    {
        EqData eqData;
        eqData.freq = eqDataList.at(i).freq;
        eqData.gain = eqDataList.at(i).gain;
        eqData.q    = eqDataList.at(i).q;
        body._eqList.append(eqData);
    }

    switch (type) {
    case CFG_TYPE_EQ_1:
        _Eq_1_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    case CFG_TYPE_EQ_3:
        _Eq_3_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    case CFG_TYPE_EQ_5:
        _Eq_5_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    case CFG_TYPE_EQ_7:
        _Eq_7_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    case CFG_TYPE_EQ_10:
        _Eq_10_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    case CFG_TYPE_EQ_16:
        _Eq_16_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    case CFG_TYPE_EQ_1X3:
        _Eq_1x3_DataList[pattern].insert(devName,body);
        _moduleNum.insert(devName, moduleNum);
        break;
    case CFG_TYPE_EQ_2X3:
        _Eq_2x3_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    case CFG_TYPE_EQ_3X3:
        _Eq_3x3_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    default:
        break;
    }
}

QHash<QString, SnapshotSaveEqData::EqModuleData> SnapshotSaveEqData::getEqDataList(CfgType type, int pattern)
{
    QHash<QString, EqModuleData> list;
    switch(type)
    {
    case CFG_TYPE_EQ_1:
        list = _Eq_1_DataList[pattern];
        break;
    case CFG_TYPE_EQ_3:
        list = _Eq_3_DataList[pattern];
        break;
    case CFG_TYPE_EQ_5:
        list = _Eq_5_DataList[pattern];
        break;
    case CFG_TYPE_EQ_7:
        list = _Eq_7_DataList[pattern];
        break;
    case CFG_TYPE_EQ_10:
        list = _Eq_10_DataList[pattern];
        break;
    case CFG_TYPE_EQ_16:
        list = _Eq_16_DataList[pattern];
        break;
    case CFG_TYPE_EQ_1X3:
        list = _Eq_1x3_DataList[pattern];
        break;
    case CFG_TYPE_EQ_2X3:
        list = _Eq_2x3_DataList[pattern];
        break;
    case CFG_TYPE_EQ_3X3:
        list = _Eq_3x3_DataList[pattern];
        break;
    default:
        break;
    }
    return list;
}
