#include "snapshotsaveoutputdata.h"
#include "configItem/configitemoutput.h"

void SnapshotSaveOutputData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    switch(type) {
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        setDataToSetting<Output_NewData_t>(_output_4_DataList[pattern], ConfigIni, type);
        break;
    default:
        break;
    }
}

void SnapshotSaveOutputData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUT_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUT).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
//        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();
        switch (type) {
        case CFG_TYPE_OUTPUT_2:
        case CFG_TYPE_OUTPUT_4:
        case CFG_TYPE_OUTPUT_8:
        case CFG_TYPE_OUTPUT_16:
        {
            Output_NewData_t body;
            getDataFromSetting<Output_NewData_t>(body, ConfigIni, pattern, type);
            _output_4_DataList[pattern].insert(devName, body);
            break;
        }
        default:
            break;
        }
        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

void SnapshotSaveOutputData::saveOutputData(ConfigItem *cfgItem, int pattern)
{
    ConfigItemOutput *outputItem = qgraphicsitem_cast<ConfigItemOutput *>(cfgItem);
    switch (outputItem->getCfgType()) {
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        {
            Output_NewData_t body;
            for(int i= 0; i < outputItem->getMaxPort(); i++) {
                body.s_ch = outputItem->getRealChannel(i);
                if (i & 0x01) {
                    body.s_body.gain[i>>1].gainB = (outputItem->getGain(i) + 100) * 100;
                }
                else {
                    body.s_body.gain[i>>1].gainA = (outputItem->getGain(i) + 100) * 100;
                }
                body.s_body.mute &= ~(0x01 << i);
                body.s_body.mute |= (outputItem->isMute(i) << i);
                body.s_body.antiPhase &= ~(0x01 << i);
                body.s_body.antiPhase |= (outputItem->isInvert(i) << i);

                QByteArray array = outputItem->getUserLabel(i).toUtf8();
                body.s_userLab[i] = (char *)malloc(array.length()+1);
                memcpy(body.s_userLab[i], array, array.length());
                body.s_userLab[i][array.length()] = '\0';
            }
            _output_4_DataList[pattern].insert(outputItem->customName(), body);
        }
        break;
    default:
        break;
    }
}

void SnapshotSaveOutputData::setOutputData(ConfigItem *cfgItem, int pattern)
{
    ConfigItemOutput *outputItem = qgraphicsitem_cast<ConfigItemOutput *>(cfgItem);
    switch (outputItem->getCfgType()) {
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        {
            if(!findCfgItemData<Output_NewData_t>(_output_4_DataList[pattern], outputItem)) return;
            Output_NewData_t body = _output_4_DataList[pattern].find(outputItem->customName()).value();
            for(int i = 0; i < outputItem->getMaxPort(); i++) {
                if (i & 0x01) {
                    outputItem->setGain(i, body.s_body.gain[i >> 1].gainB * 0.01 - 100);
                }
                else {
                    outputItem->setGain(i, body.s_body.gain[i >> 1].gainA * 0.01 - 100);
                }
                outputItem->setMute(i, ((body.s_body.mute >> i) & 0x01));
                outputItem->setInvert(i, ((body.s_body.antiPhase >> i) & 0x01));

                outputItem->setUserLabel(i, QString(body.s_userLab[i]));
            }
            outputItem->updateCfgSettingLoadedData();
        }
        break;
    default:
        break;
    }
}

int SnapshotSaveOutputData::getchannelNum(int type)
{
    switch(type) {
    case CFG_TYPE_OUTPUT_1:  return 1;
    case CFG_TYPE_OUTPUT_2:  return 2;
    case CFG_TYPE_OUTPUT_4:  return 4;
    case CFG_TYPE_OUTPUT_8:  return 8;
    case CFG_TYPE_OUTPUT_16: return 16;
    default: return 0;
    }
}


template<typename T_Value>
void SnapshotSaveOutputData::setDataToSetting(QHash<QString, T_Value> dataList, QSettings *ConfigIni, int type)
{
    QList<QString> nameList = dataList.keys();
    int unimoduleNum = dataList.count();
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUT_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);
    for(int idx = 0; idx < unimoduleNum; idx++) {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUT).arg(idx));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_CHANNEL), dataList[nameList.at(idx)].s_ch >> 2);
        for (int i = 0; i < getchannelNum(type); ++i) {
            if (i & 0x01) {
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_GAIN).arg(i), dataList[nameList.at(idx)].s_body.gain[i >> 1].gainB);
            }
            else {
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_GAIN).arg(i), dataList[nameList.at(idx)].s_body.gain[i >> 1].gainA);
            }
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_INVERT).arg(i),  ((dataList[nameList.at(idx)].s_body.antiPhase >> i) & 0x01));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_MUTE).arg(i),    ((dataList[nameList.at(idx)].s_body.mute >> i) & 0x01));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_UserLabel).arg(i), QString(dataList[nameList.at(idx)].s_userLab[i]));
        }
        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

template<typename T_Value>
void SnapshotSaveOutputData::getDataFromSetting(T_Value &body, QSettings *ConfigIni, int pattern, int type)
{
    body.s_ch = ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_CHANNEL)).toInt();
    for(int i = 0; i < getchannelNum(type); i++) {
        if (i & 0x01) {
            body.s_body.gain[i >> 1].gainB = ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_GAIN).arg(i)).toInt();
        }
        else {
            body.s_body.gain[i >> 1].gainA = ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_GAIN).arg(i)).toInt();
        }
        body.s_body.antiPhase &= ~(0x01 << i);
        body.s_body.antiPhase |= (ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_INVERT).arg(i)).toInt() << i);

        body.s_body.mute &= ~(0x01 << i);
        body.s_body.mute |= (ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_MUTE).arg(i)).toInt() << i);

        QString uLab = ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_UserLabel).arg(i)).toString();
        QByteArray array = uLab.toUtf8();
        body.s_userLab[i] = (char *)malloc(array.length()+1);
        memcpy(body.s_userLab[i], array, array.length());
        body.s_userLab[i][array.length()] = '\0';
    }
}
