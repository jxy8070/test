#include "snapshotsaveinputdata.h"
#include "configItem/configiteminput.h"

void SnapshotSaveInputData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    switch(type) {
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        setDataToSetting<Input_NewData_t>(_input_4_DataList[pattern], ConfigIni, type);
        break;
    default:
        break;
    }
}

void SnapshotSaveInputData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_INPUT_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_INPUT).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
//        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();
        switch (type) {
        case CFG_TYPE_INPUT_2:
        case CFG_TYPE_INPUT_4:
        case CFG_TYPE_INPUT_8:
        case CFG_TYPE_INPUT_16:
        {
            Input_NewData_t body;
            getDataFromSetting<Input_NewData_t>(body, ConfigIni, pattern, type);
            _input_4_DataList[pattern].insert(devName, body);
            break;
        }
        default:
            break;
        }
        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

void SnapshotSaveInputData::saveInputData(ConfigItem *cfgItem, int pattern)
{
    ConfigItemInput *inputItem = qgraphicsitem_cast<ConfigItemInput *>(cfgItem);

    switch (inputItem->getCfgType()) {
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        saveInput_2_16_Data(inputItem, pattern);
        break;
    default:
        break;
    }
}

void SnapshotSaveInputData::setInputData(ConfigItem *cfgItem, int pattern)
{
    ConfigItemInput *inputItem = qgraphicsitem_cast<ConfigItemInput *>(cfgItem);

    switch (inputItem->getCfgType()) {
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        if(!findCfgItemData<Input_NewData_t>(_input_4_DataList[pattern], cfgItem)) return;
        setInput_2_16_data(inputItem, pattern);
        break;
    default:
        break;
    }
}

void SnapshotSaveInputData::saveInput_2_16_Data(ConfigItemInput *inputItem, int pattern)
{
    Input_NewData_t body4;
    for(int i = 0; i < inputItem->getMaxPort(); ++i) {
        body4.ch = inputItem->getRealChannel(i);
        if (i & 0x01) {
            body4.body_4.gain[i>>1].gainB = inputItem->getGain(i);
        }
        else {
            body4.body_4.gain[i>>1].gainA = inputItem->getGain(i);
        }
        body4.body_4.antiPhase &= ~(0x01 << i);
        body4.body_4.antiPhase |= (inputItem->isInvert(i) << i);

        body4.body_4.mute &= ~(0x01 << i);
        body4.body_4.mute |= (inputItem->isMute(i) << i);
        if(inputItem->getPortType() == PortType_MONAURAL) {
            body4.phantom[i] = inputItem->getPhantom(i);
            body4.preampGain[i] = inputItem->getPreampGain(i);
        }
        QByteArray array = inputItem->getUserLabel(i).toUtf8();
//        qDebug() << __FUNCTION__ << __LINE__ << array.length() << array.count() << sizeof(array);
        body4.userLab[i] = (char *)malloc(array.count() + 1);
        memcpy(body4.userLab[i], array, array.count());
        body4.userLab[i][array.count()] = '\0';
    }
    _input_4_DataList[pattern].insert(inputItem->customName(), body4);
}

void SnapshotSaveInputData::setInput_2_16_data(ConfigItemInput *inputItem, int pattern)
{
    Input_NewData_t body4 = _input_4_DataList[pattern].find(inputItem->customName()).value();
    for(int i = 0; i < inputItem->getMaxPort(); i++) {
        if (i & 0x01) {
            inputItem->setGain(i, body4.body_4.gain[i >> 1].gainB);
        }
        else {
            inputItem->setGain(i, body4.body_4.gain[i >> 1].gainA);
        }
        inputItem->setInvert(i, ((body4.body_4.antiPhase >> i) & 0x01));
        inputItem->setMute(i, ((body4.body_4.mute >> i) & 0x01));
        if(inputItem->getPortType() == PortType_MONAURAL) {
            inputItem->setPhantom(i, body4.phantom[i]);
            inputItem->setPreampGain(i, body4.preampGain[i]);
        }
        inputItem->setUserLabel(i, QString(body4.userLab[i]));
    }
    inputItem->updateCfgSettingLoadedData();
}

int SnapshotSaveInputData::getchannelNum(int type)
{
    switch(type) {
    case CFG_TYPE_INPUT_1:  return 1;
    case CFG_TYPE_INPUT_2:  return 2;
    case CFG_TYPE_INPUT_4:  return 4;
    case CFG_TYPE_INPUT_8:  return 8;
    case CFG_TYPE_INPUT_16: return 16;
    default: return 0;
    }
}

template<typename T_Value>
void SnapshotSaveInputData::setDataToSetting(QHash<QString, T_Value> dataList, QSettings *ConfigIni, int type)
{
    QList<QString> nameList = dataList.keys();
    int unimoduleNum = dataList.count();
    if(!unimoduleNum) return;
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_INPUT_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);
    for(int idx = 0; idx < unimoduleNum; idx++) {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_INPUT).arg(idx));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_CHANNEL), dataList[nameList.at(idx)].ch >> 2);
        for (int i = 0; i < getchannelNum(type); ++i) {
            if (i & 0x01) {
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_GAIN).arg(i), dataList[nameList.at(idx)].body_4.gain[i >> 1].gainB);
            }
            else {
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_GAIN).arg(i), dataList[nameList.at(idx)].body_4.gain[i >> 1].gainA);
            }
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_INVERT).arg(i),  ((dataList[nameList.at(idx)].body_4.antiPhase >> i) & 0x01));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_MUTE).arg(i),    ((dataList[nameList.at(idx)].body_4.mute >> i) & 0x01));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_Phantom).arg(i), dataList[nameList.at(idx)].phantom[i]);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_PreGain).arg(i), dataList[nameList.at(idx)].preampGain[i]);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_UserLabel).arg(i), QString(dataList[nameList.at(idx)].userLab[i]));
        }
        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

template<typename T_Value>
void SnapshotSaveInputData::getDataFromSetting(T_Value& body, QSettings *ConfigIni,int pattern, int type)
{
    body.ch = ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_CHANNEL)).toInt();
    for(int i = 0; i < getchannelNum(type); i++) {
        if (i & 0x01) {
            body.body_4.gain[i >> 1].gainB = ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_GAIN).arg(i)).toInt();
        }
        else {
            body.body_4.gain[i >> 1].gainA = ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_GAIN).arg(i)).toInt();
        }
        body.body_4.antiPhase &= ~(0x01 << i);
        body.body_4.antiPhase |= (ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_INVERT).arg(i)).toInt() << i);

        body.body_4.mute &= ~(0x01 << i);
        body.body_4.mute |= (ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_MUTE).arg(i)).toInt() << i);

        body.phantom[i]    = ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_Phantom).arg(i)).toBool();
        body.preampGain[i] = ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_PreGain).arg(i)).toInt();

        QString uLab = ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_UserLabel).arg(i)).toString();
        QByteArray array = uLab.toUtf8();
        body.userLab[i] = (char *)malloc(array.length()+1);
        memcpy(body.userLab[i], array, array.length());
        body.userLab[i][array.length()] = '\0';
    }
}
