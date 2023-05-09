#include "configitemsnapshot.h"
#include "configSetting/cfgsettingsnapshotbank.h"

#include "snapshotSaveData/snapshotsaveobject.h"
#include "snapshotSaveData/snapshotsavedelaydata.h"
#include "snapshotSaveData/snapshotsaverouterdata.h"
#include "snapshotSaveData/snapshotsavemixerdata.h"
#include "snapshotSaveData/snapshotsavedyndata.h"
#include "snapshotSaveData/snapshotsavepriorityduckerdata.h"
#include "snapshotSaveData/snapshotsaveeqdata.h"
#include "snapshotSaveData/snapshotsavesplitdata.h"
#include "snapshotSaveData/snapshotsavefiltedata.h"
#include "snapshotSaveData/snapshotsavesignaldata.h"
#include "snapshotSaveData/snapshotsavemeterdata.h"
#include "snapshotSaveData/snapshotsavegpidata.h"
#include "snapshotSaveData/snapshotsavefreqshiftdata.h"
#include "snapshotSaveData/snapshotsaveautomixdata.h"
#include "snapshotSaveData/snapshotsaveaecdata.h"
#include "snapshotSaveData/snapshotsavenfcdata.h"
#include "snapshotSaveData/snapshotsavegainrampdata.h"
#include "snapshotSaveData/snapshotsavegaindata.h"
#include "snapshotSaveData/snapshotsavecrossfaderdata.h"
#include "snapshotSaveData/snapshotsaveinputdata.h"
#include "snapshotSaveData/snapshotsaveoutputdata.h"

#include <QMessageBox>
#include <QByteArray>

ConfigItemSnapshot::ConfigItemSnapshot(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    : ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    _cfgSettingSnapshot = NULL;

    memset(_buttonPattern, 0, sizeof(int) * 24);
    _currentPattern = -1;
    _fromSel_CtrlsList.clear();
    for(int i = 0; i < DataCount; i++) {
        _LabelList.append(QString::number(i + 1));
    }
    ///hrp 待完善
    if(cfgType == CFG_TYPE_SNAPSHOT) {
        _snapNum = 4;
    }else if(cfgType == CFG_TYPE_SNAPSHOT_BANK) {
        _snapNum = 8;
    }

    addSubType(cfgType);
    initPort(0, 0);
    connect(this, SIGNAL(snapCountChanged_Signal(int)), this, SLOT(onSnapCountChanged_slots(int)));
}

QWidget *ConfigItemSnapshot::getSettingForm()
{
    if(_proxyItem == NULL) _cfgSettingSnapshot = NULL;
    if(_cfgSettingSnapshot == NULL)
    {
        _cfgSettingSnapshot = new CfgSettingSnapshotBank(this);
    }
    return _cfgSettingSnapshot;
}


QByteArray ConfigItemSnapshot::getDetailData()
{
    QByteArray array;
    return array;
}


void ConfigItemSnapshot::setDetailData(unsigned int *d)
{

}

void ConfigItemSnapshot::setRtnData(unsigned int *d)
{

}

void ConfigItemSnapshot::updateCfgSettingLoadedData()
{

}

void ConfigItemSnapshot::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_GROUP));
    int patternNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SHAPSHOT_CURRENTPATTERN)).toInt();
    int snapNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SHAPSHOT_SNAPNUM)).toInt();
    setSnapCount(snapNum);
    setCurrentPattern(patternNum);

    for(int i = 0; i < DataCount; i++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_PATTERN).arg(i));

        //delay
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_DELAY_TYPE, _delayData, i, CFG_TYPE_DELAY_5);
        //router
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_ROUTER_TYPE, _routerData, i, CFG_TYPE_ROUTER_4 );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_ROUTER_TYPE, _routerData, i, CFG_TYPE_ROUTER_8 );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_ROUTER_TYPE, _routerData, i, CFG_TYPE_ROUTER_16);
        //mixer
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_MIXER_TYPE, _mixerData, i, CFG_TYPE_MIXER_4X1);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_MIXER_TYPE, _mixerData, i, CFG_TYPE_MIXER_4X2);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_MIXER_TYPE, _mixerData, i, CFG_TYPE_MIXER_4X4);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_MIXER_TYPE, _mixerData, i, CFG_TYPE_MIXER_8X1);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_MIXER_TYPE, _mixerData, i, CFG_TYPE_MIXER_8X2);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_MIXER_TYPE, _mixerData, i, CFG_TYPE_MIXER_8X4);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_MIXER_TYPE, _mixerData, i, CFG_TYPE_MIXER_8X8);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_MIXER_TYPE, _mixerData, i, CFG_TYPE_MIXER_16X16);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_MIXER_TYPE, _mixerData, i, CFG_TYPE_MIXER_32X32);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_MIXER_TYPE, _mixerData, i, CFG_TYPE_MIXER_64X64);

        //dyn
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_DYN_TYPE, _dynData, i, CFG_TYPE_DYN_AUTO  );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_DYN_TYPE, _dynData, i, CFG_TYPE_DYN_COMP  );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_DYN_TYPE, _dynData, i, CFG_TYPE_DYN_EXPAND);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_DYN_TYPE, _dynData, i, CFG_TYPE_DYN_LIMIT );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_DYN_TYPE, _dynData, i, CFG_TYPE_DYN_NOISE );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_DYN_TYPE, _dynData, i, CFG_TYPE_DYN_PEAK  );
        //eq
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_EQ_TYPE, _eqData, i, CFG_TYPE_EQ_1  );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_EQ_TYPE, _eqData, i, CFG_TYPE_EQ_3  );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_EQ_TYPE, _eqData, i, CFG_TYPE_EQ_5  );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_EQ_TYPE, _eqData, i, CFG_TYPE_EQ_7  );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_EQ_TYPE, _eqData, i, CFG_TYPE_EQ_10 );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_EQ_TYPE, _eqData, i, CFG_TYPE_EQ_16 );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_EQ_TYPE, _eqData, i, CFG_TYPE_EQ_1X3);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_EQ_TYPE, _eqData, i, CFG_TYPE_EQ_2X3);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_EQ_TYPE, _eqData, i, CFG_TYPE_EQ_3X3);
        //split
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_SPLIT_TYPE, _splitData, i, CFG_TYPE_SPLIT_2);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_SPLIT_TYPE, _splitData, i, CFG_TYPE_SPLIT_3);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_SPLIT_TYPE, _splitData, i, CFG_TYPE_SPLIT_4);
        //Filter
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_FILTER_TYPE, _filteData, i, CFG_TYPE_FILTE_HP);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_FILTER_TYPE, _filteData, i, CFG_TYPE_FILTE_HS);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_FILTER_TYPE, _filteData, i, CFG_TYPE_FILTE_LP);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_FILTER_TYPE, _filteData, i, CFG_TYPE_FILTE_LS);
        //Signal
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_SIGNAL_TYPE, _signalData, i, CFG_TYPE_SIG_NOISE);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_SIGNAL_TYPE, _signalData, i, CFG_TYPE_SIG_SINE);
        //Meter
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_METER_TYPE, _meterData, i, CFG_TYPE_METER_1);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_METER_TYPE, _meterData, i, CFG_TYPE_METER_2);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_METER_TYPE, _meterData, i, CFG_TYPE_METER_4);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_METER_TYPE, _meterData, i, CFG_TYPE_METER_8);

        //Gpi
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_GPI_TYPE, _gpiData, i, CFG_TYPE_GPI);
        //FreqShift
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_FREQSHIFT_TYPE, _freqShiftData, i, CFG_TYPE_FREQSHIFT);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_FREQSHIFT_TYPE, _freqShiftData, i, CFG_TYPE_FREQSHIFT_PLUS);

        //AutoMix
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE, _autoMixData, i, CFG_TYPE_AUTOMIX_GS_4 );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE, _autoMixData, i, CFG_TYPE_AUTOMIX_GS_8 );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE, _autoMixData, i, CFG_TYPE_AUTOMIX_GS_16);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE, _autoMixData, i, CFG_TYPE_AUTOMIX_GS_32);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE, _autoMixData, i, CFG_TYPE_AUTOMIXNGAT_4 );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE, _autoMixData, i, CFG_TYPE_AUTOMIXNGAT_8 );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE, _autoMixData, i, CFG_TYPE_AUTOMIXNGAT_16);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE, _autoMixData, i, CFG_TYPE_AUTOMIXNGAT_32);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE, _autoMixData, i, CFG_TYPE_AUTOMIXNGRT_4 );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE, _autoMixData, i, CFG_TYPE_AUTOMIXNGRT_8 );
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE, _autoMixData, i, CFG_TYPE_AUTOMIXNGRT_16);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE, _autoMixData, i, CFG_TYPE_AUTOMIXNGRT_32);

        //AEC
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AEC_TYPE, _aecData, i, CFG_TYPE_AEC_2_2);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_AEC_TYPE, _aecData, i, CFG_TYPE_SOFTAEC_8K128);

        //NFC
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_NFC_TYPE, _nfcData, i, CFG_TYPE_NFC1_8NF);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_NFC_TYPE, _nfcData, i, CFG_TYPE_NFC1_16NF);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_NFC_TYPE, _nfcData, i, CFG_TYPE_NFC1_24NF);
        upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_NFC_TYPE, _nfcData, i, CFG_TYPE_NFC1_32NF);

        //Input
        switch (Config::instance()->getPortGroupNum()) {
        case 1:  upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_INPUT_TYPE, _inputData, i, CFG_TYPE_INPUT_1); break;
        case 2:  upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_INPUT_TYPE, _inputData, i, CFG_TYPE_INPUT_2); break;
        case 4:  upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_INPUT_TYPE, _inputData, i, CFG_TYPE_INPUT_4); break;
        case 8:  upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_INPUT_TYPE, _inputData, i, CFG_TYPE_INPUT_8); break;
        case 16: upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_INPUT_TYPE, _inputData, i, CFG_TYPE_INPUT_16); break;
        default:
            break;
        }

        //OutPut
        switch (Config::instance()->getPortGroupNum()) {
        case 1:  upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUT_TYPE, _outputData, i, CFG_TYPE_OUTPUT_1); break;
        case 2:  upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUT_TYPE, _outputData, i, CFG_TYPE_OUTPUT_2); break;
        case 4:  upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUT_TYPE, _outputData, i, CFG_TYPE_OUTPUT_4); break;
        case 8:  upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUT_TYPE, _outputData, i, CFG_TYPE_OUTPUT_8); break;
        case 16: upLoalModuleData(ConfigIni, CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUT_TYPE, _outputData, i, CFG_TYPE_OUTPUT_16); break;
        default:
            break;
        }

        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

void ConfigItemSnapshot::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SHAPSHOT_CURRENTPATTERN), getCurrentPattern());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SHAPSHOT_SNAPNUM), getSnapCount());
    for(int i = 0 ; i < DataCount; i++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_PATTERN).arg(i));

        //delay
        if(_delayData) { _delayData->saveToSetting(ConfigIni, i, CFG_TYPE_DELAY_5); }
        //Router
        if(_routerData) {
            _routerData->saveToSetting(ConfigIni, i, CFG_TYPE_ROUTER_4 );
            _routerData->saveToSetting(ConfigIni, i, CFG_TYPE_ROUTER_8 );
            _routerData->saveToSetting(ConfigIni, i, CFG_TYPE_ROUTER_16);
        }
        //mixer
        if(_mixerData) {
            _mixerData->saveToSetting(ConfigIni, i, CFG_TYPE_MIXER_4X1);
            _mixerData->saveToSetting(ConfigIni, i, CFG_TYPE_MIXER_4X2);
            _mixerData->saveToSetting(ConfigIni, i, CFG_TYPE_MIXER_4X1);
            _mixerData->saveToSetting(ConfigIni, i, CFG_TYPE_MIXER_8X1);
            _mixerData->saveToSetting(ConfigIni, i, CFG_TYPE_MIXER_8X2);
            _mixerData->saveToSetting(ConfigIni, i, CFG_TYPE_MIXER_8X4);
            _mixerData->saveToSetting(ConfigIni, i, CFG_TYPE_MIXER_8X8);
            _mixerData->saveToSetting(ConfigIni, i, CFG_TYPE_MIXER_16X16);
            _mixerData->saveToSetting(ConfigIni, i, CFG_TYPE_MIXER_32X32);
            _mixerData->saveToSetting(ConfigIni, i, CFG_TYPE_MIXER_64X64);
        }
        //dyn
        if(_dynData) {
            _dynData->saveToSetting(ConfigIni, i, CFG_TYPE_DYN_AUTO  );
            _dynData->saveToSetting(ConfigIni, i, CFG_TYPE_DYN_COMP  );
            _dynData->saveToSetting(ConfigIni, i, CFG_TYPE_DYN_LIMIT );
            _dynData->saveToSetting(ConfigIni, i, CFG_TYPE_DYN_NOISE );
            _dynData->saveToSetting(ConfigIni, i, CFG_TYPE_DYN_EXPAND);
            _dynData->saveToSetting(ConfigIni, i, CFG_TYPE_DYN_PEAK  );
        }
        //Eq
        if(_eqData) {
            _eqData->saveToSetting(ConfigIni, i, CFG_TYPE_EQ_1  );
            _eqData->saveToSetting(ConfigIni, i, CFG_TYPE_EQ_3  );
            _eqData->saveToSetting(ConfigIni, i, CFG_TYPE_EQ_5  );
            _eqData->saveToSetting(ConfigIni, i, CFG_TYPE_EQ_7  );
            _eqData->saveToSetting(ConfigIni, i, CFG_TYPE_EQ_10 );
            _eqData->saveToSetting(ConfigIni, i, CFG_TYPE_EQ_16 );
            _eqData->saveToSetting(ConfigIni, i, CFG_TYPE_EQ_1X3);
            _eqData->saveToSetting(ConfigIni, i, CFG_TYPE_EQ_2X3);
            _eqData->saveToSetting(ConfigIni, i, CFG_TYPE_EQ_3X3);
        }
        //split
        if(_splitData) {
            _splitData->saveToSetting(ConfigIni, i, CFG_TYPE_SPLIT_2);
            _splitData->saveToSetting(ConfigIni, i, CFG_TYPE_SPLIT_3);
            _splitData->saveToSetting(ConfigIni, i, CFG_TYPE_SPLIT_4);
        }
        //Filter
        if(_filteData) {
            _filteData->saveToSetting(ConfigIni, i, CFG_TYPE_FILTE_HP);
            _filteData->saveToSetting(ConfigIni, i, CFG_TYPE_FILTE_HS);
            _filteData->saveToSetting(ConfigIni, i, CFG_TYPE_FILTE_LP);
            _filteData->saveToSetting(ConfigIni, i, CFG_TYPE_FILTE_LS);
        }
        //Signal
        if(_signalData) {
            _signalData->saveToSetting(ConfigIni, i, CFG_TYPE_SIG_NOISE);
            _signalData->saveToSetting(ConfigIni, i, CFG_TYPE_SIG_SINE);
        }
        //meter
        if(_meterData) {
            _meterData->saveToSetting(ConfigIni, i, CFG_TYPE_METER_1);
            _meterData->saveToSetting(ConfigIni, i, CFG_TYPE_METER_2);
            _meterData->saveToSetting(ConfigIni, i, CFG_TYPE_METER_4);
            _meterData->saveToSetting(ConfigIni, i, CFG_TYPE_METER_8);
        }
        //Gpi
        if(_gpiData) {
            _gpiData->saveToSetting(ConfigIni, i, CFG_TYPE_GPI);
        }
        //FreqShift / FreqShiftPlus
        if(_freqShiftData) {
            _freqShiftData->saveToSetting(ConfigIni, i, CFG_TYPE_FREQSHIFT);
            _freqShiftData->saveToSetting(ConfigIni, i, CFG_TYPE_FREQSHIFT_PLUS);
        }

        //AutoMix
        if(_autoMixData) {
            _autoMixData->saveToSetting(ConfigIni, i, CFG_TYPE_AUTOMIX_GS_4 );
            _autoMixData->saveToSetting(ConfigIni, i, CFG_TYPE_AUTOMIX_GS_8 );
            _autoMixData->saveToSetting(ConfigIni, i, CFG_TYPE_AUTOMIX_GS_16);
            _autoMixData->saveToSetting(ConfigIni, i, CFG_TYPE_AUTOMIX_GS_32);
            _autoMixData->saveToSetting(ConfigIni, i, CFG_TYPE_AUTOMIXNGAT_4 );
            _autoMixData->saveToSetting(ConfigIni, i, CFG_TYPE_AUTOMIXNGAT_8 );
            _autoMixData->saveToSetting(ConfigIni, i, CFG_TYPE_AUTOMIXNGAT_16);
            _autoMixData->saveToSetting(ConfigIni, i, CFG_TYPE_AUTOMIXNGAT_32);
            _autoMixData->saveToSetting(ConfigIni, i, CFG_TYPE_AUTOMIXNGRT_4 );
            _autoMixData->saveToSetting(ConfigIni, i, CFG_TYPE_AUTOMIXNGRT_8 );
            _autoMixData->saveToSetting(ConfigIni, i, CFG_TYPE_AUTOMIXNGRT_16);
            _autoMixData->saveToSetting(ConfigIni, i, CFG_TYPE_AUTOMIXNGRT_32);
        }
        //AEC
        if(_aecData) {
            _aecData->saveToSetting(ConfigIni, i, CFG_TYPE_AEC_2_2);
            _aecData->saveToSetting(ConfigIni, i, CFG_TYPE_SOFTAEC_8K128);
        }
        //NFC
        if(_nfcData) {
            _nfcData->saveToSetting(ConfigIni, i, CFG_TYPE_NFC1_8NF);
            _nfcData->saveToSetting(ConfigIni, i, CFG_TYPE_NFC1_16NF);
            _nfcData->saveToSetting(ConfigIni, i, CFG_TYPE_NFC1_24NF);
            _nfcData->saveToSetting(ConfigIni, i, CFG_TYPE_NFC1_32NF);
        }
        //Input
        if(_inputData) {
            switch (Config::instance()->getPortGroupNum()) {
            case 1:  _inputData->saveToSetting(ConfigIni, i, CFG_TYPE_INPUT_1); break;
            case 2:  _inputData->saveToSetting(ConfigIni, i, CFG_TYPE_INPUT_2); break;
            case 4:  _inputData->saveToSetting(ConfigIni, i, CFG_TYPE_INPUT_4); break;
            case 8:  _inputData->saveToSetting(ConfigIni, i, CFG_TYPE_INPUT_8); break;
            case 16: _inputData->saveToSetting(ConfigIni, i, CFG_TYPE_INPUT_16); break;
            default:
                break;
            }
        }
        if(_outputData) {
            switch (Config::instance()->getPortGroupNum()) {
            case 1:  _outputData->saveToSetting(ConfigIni, i, CFG_TYPE_OUTPUT_1); break;
            case 2:  _outputData->saveToSetting(ConfigIni, i, CFG_TYPE_OUTPUT_2); break;
            case 4:  _outputData->saveToSetting(ConfigIni, i, CFG_TYPE_OUTPUT_4); break;
            case 8:  _outputData->saveToSetting(ConfigIni, i, CFG_TYPE_OUTPUT_8); break;
            case 16: _outputData->saveToSetting(ConfigIni, i, CFG_TYPE_OUTPUT_16); break;
            default:
                break;
            }
        }
        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

QVariant ConfigItemSnapshot::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    return QVariant();
}

void ConfigItemSnapshot::uploadData(int type)
{
    _currentPattern = type; // 1~24
    type -= 1; //0~23
    foreach (QGraphicsItem *item, scene()->items(Qt::AscendingOrder)) {
        if(item->type() != ConfigItem::Type) continue;
        ConfigItem *configitem = qgraphicsitem_cast<ConfigItem *>(item);

        if(getCreateType() != Create_snap_Global) {
           if(!getFromSel_CtrlsList().contains(configitem)) continue;
           if(configitem->scene() == NULL) continue;
        }
        switch (configitem->getCfgType()) {
        case CFG_TYPE_DELAY_5:
        case CFG_TYPE_DELAY_10:
        case CFG_TYPE_DELAY_20:
        case CFG_TYPE_DELAY_50:
        case CFG_TYPE_DELAY_100:
        case CFG_TYPE_DELAY_500:
        case CFG_TYPE_DELAY_1000:
            if(_delayData) {
                _delayData->setDelayData(configitem, type);
            }
            break;
        case CFG_TYPE_ROUTER_4:
        case CFG_TYPE_ROUTER_8:
        case CFG_TYPE_ROUTER_16:
        case CFG_TYPE_ROUTER_32:
        case CFG_TYPE_ROUTER_64:
            if(_routerData) {
                _routerData->setRouterData(configitem, type);
            }
        case CFG_TYPE_MIXER_4X1:
        case CFG_TYPE_MIXER_4X2:
        case CFG_TYPE_MIXER_4X4:
        case CFG_TYPE_MIXER_8X1:
        case CFG_TYPE_MIXER_8X2:
        case CFG_TYPE_MIXER_8X4:
        case CFG_TYPE_MIXER_8X8:
        case CFG_TYPE_MIXER_16X16:
        case CFG_TYPE_MIXER_32X32:
        case CFG_TYPE_MIXER_64X64:
            if(_mixerData) {
                _mixerData->setMixerData(configitem, type);
            }
            break;
        case CFG_TYPE_DYN_AUTO:
        case CFG_TYPE_DYN_COMP:
        case CFG_TYPE_DYN_LIMIT:
        case CFG_TYPE_DYN_NOISE:
        case CFG_TYPE_DYN_EXPAND:
        case CFG_TYPE_DYN_PEAK:
            if(_dynData) {
                _dynData->setDynData(configitem, type);
            }
            break;
        case CFG_TYPE_PDUCKER_4:
        case CFG_TYPE_PDUCKER_8:
        case CFG_TYPE_PDUCKER_16:
        case CFG_TYPE_PDUCKER_32:
            if(_priorityDuckerData) {
                _priorityDuckerData->setPriorityDuckerData(configitem, type);
            }
            break;
        case CFG_TYPE_EQ_1:
        case CFG_TYPE_EQ_3:
        case CFG_TYPE_EQ_5:
        case CFG_TYPE_EQ_7:
        case CFG_TYPE_EQ_10:
        case CFG_TYPE_EQ_16:
        case CFG_TYPE_EQ_1X3:
        case CFG_TYPE_EQ_2X3:
        case CFG_TYPE_EQ_3X3:
            if(_eqData) {
                _eqData->setEqData(configitem, type);
            }
            break;
        case CFG_TYPE_SPLIT_2:
        case CFG_TYPE_SPLIT_3:
        case CFG_TYPE_SPLIT_4:
            if(_splitData) {
                _splitData->setSplitData(configitem, type);
            }
            break;
        case CFG_TYPE_FILTE_LP:
        case CFG_TYPE_FILTE_HP:
        case CFG_TYPE_FILTE_LS:
        case CFG_TYPE_FILTE_HS:
            if(_filteData) {
                _filteData->setFilteData(configitem, type);
            }
            break;
        case CFG_TYPE_SIG_SINE:
        case CFG_TYPE_SIG_NOISE:
        case CFG_TYPE_SIG_PINKNOISE:
            if(_signalData) {
                _signalData->setSignalData(configitem, type);
            }
        case CFG_TYPE_METER_1:
        case CFG_TYPE_METER_2:
        case CFG_TYPE_METER_4:
        case CFG_TYPE_METER_8:
            if(_meterData) {
                _meterData->setMeterData(configitem, type);
            }
            break;
        case CFG_TYPE_GPI:
            if(_gpiData) {
                _gpiData->setGpiData(configitem, type);
            }
            break;
        case CFG_TYPE_FREQSHIFT:
        case CFG_TYPE_FREQSHIFT_PLUS:
            if(_freqShiftData) {
                _freqShiftData->setFreqShiftData(configitem, type);
            }
            break;
        case CFG_TYPE_AUTOMIX_GS_4:
        case CFG_TYPE_AUTOMIX_GS_8:
        case CFG_TYPE_AUTOMIX_GS_16:
        case CFG_TYPE_AUTOMIX_GS_32:
        case CFG_TYPE_AUTOMIXNGRT_4:
        case CFG_TYPE_AUTOMIXNGRT_8:
        case CFG_TYPE_AUTOMIXNGRT_16:
        case CFG_TYPE_AUTOMIXNGRT_32:
        case CFG_TYPE_AUTOMIXNGAT_4:
        case CFG_TYPE_AUTOMIXNGAT_8:
        case CFG_TYPE_AUTOMIXNGAT_16:
        case CFG_TYPE_AUTOMIXNGAT_32:
            if(_autoMixData) {
                _autoMixData->setAutoMixData(configitem, type);
            }
            break;
        case CFG_TYPE_AEC_2_2:
        case CFG_TYPE_SOFTAEC_8K128:
            if(_aecData) {
                _aecData->setAecData(configitem, type);
            }
            break;
        case CFG_TYPE_NFC1_8NF:
        case CFG_TYPE_NFC1_16NF:
        case CFG_TYPE_NFC1_24NF:
        case CFG_TYPE_NFC1_32NF:
            if(_nfcData) {
                _nfcData->setNfcData(configitem, type);
            }
            break;
        case CFG_TYPE_GAINRAMP_1:
        case CFG_TYPE_GAINRAMP_2:
        case CFG_TYPE_GAINRAMP_4:
        case CFG_TYPE_GAINRAMP_8:
        case CFG_TYPE_GAINRAMP_16:
        case CFG_TYPE_GAINRAMP_32:
            if(_gainRampData) {
                _gainRampData->setGainRampData(configitem, type);
            }
            break;
        case CFG_TYPE_GAIN_1:
        case CFG_TYPE_GAIN_2:
        case CFG_TYPE_GAIN_4:
        case CFG_TYPE_GAIN_8:
        case CFG_TYPE_GAIN_16:
        case CFG_TYPE_GAIN_32:
            if(_gainData) {
                _gainData->setGainData(configitem, type);
            }
            break;
        case CFG_TYPE_CROSSFADER_2X1:
        case CFG_TYPE_CROSSFADER_4X2:
        case CFG_TYPE_CROSSFADER_8X4:
        case CFG_TYPE_CROSSFADER_16X8:
        case CFG_TYPE_CROSSFADER_32X16:
            if(_crossfaderData) {
                _crossfaderData->setCrossfaderData(configitem, type);
            }
            break;
        case CFG_TYPE_INPUT_1:
        case CFG_TYPE_INPUT_2:
        case CFG_TYPE_INPUT_4:
        case CFG_TYPE_INPUT_8:
        case CFG_TYPE_INPUT_16:
            if(_inputData) {
                _inputData->setInputData(configitem, type);
            }
            break;
        case CFG_TYPE_OUTPUT_1:
        case CFG_TYPE_OUTPUT_2:
        case CFG_TYPE_OUTPUT_4:
        case CFG_TYPE_OUTPUT_8:
        case CFG_TYPE_OUTPUT_16:
            if(_outputData) {
                _outputData->setOutputData(configitem, type);
            }
            break;
        default:
            break;
        }
    }
}

void ConfigItemSnapshot::saveData(int type)
{
    _currentPattern = type;
    type -= 1;
    foreach (QGraphicsItem *item, scene()->items(Qt::AscendingOrder)) {
        if(item->type() != ConfigItem::Type) continue;
        ConfigItem *configitem = qgraphicsitem_cast<ConfigItem *>(item);
        if(getCreateType() != Create_snap_Global) {
           if(!getFromSel_CtrlsList().contains(configitem)) continue;
           if(configitem->scene() == NULL) continue;
        }

        switch (configitem->getCfgType()) {
        case CFG_TYPE_DELAY_5:
        case CFG_TYPE_DELAY_10:
        case CFG_TYPE_DELAY_20:
        case CFG_TYPE_DELAY_50:
        case CFG_TYPE_DELAY_100:
        case CFG_TYPE_DELAY_500:
        case CFG_TYPE_DELAY_1000:
            if(!_delayData) {
                _delayData = new SnapshotSaveDelayData;
            }
            _delayData->saveDelayData(configitem, type);
            break;
        case CFG_TYPE_ROUTER_4:
        case CFG_TYPE_ROUTER_8:
        case CFG_TYPE_ROUTER_16:
            if(!_routerData) {
                _routerData = new SnapshotSaveRouterData;
            }
            _routerData->saveRouterData(configitem, type);
            break;
        case CFG_TYPE_MIXER_4X1:
        case CFG_TYPE_MIXER_4X2:
        case CFG_TYPE_MIXER_4X4:
        case CFG_TYPE_MIXER_8X1:
        case CFG_TYPE_MIXER_8X2:
        case CFG_TYPE_MIXER_8X4:
        case CFG_TYPE_MIXER_32X32:
        case CFG_TYPE_MIXER_8X8:
        case CFG_TYPE_MIXER_16X16:
        case CFG_TYPE_MIXER_64X64:
            if(!_mixerData) {
                _mixerData = new SnapshotSaveMixerData;
            }
            _mixerData->saveMixerData(configitem, type);
            break;
        case CFG_TYPE_DYN_AUTO:
        case CFG_TYPE_DYN_COMP:
        case CFG_TYPE_DYN_LIMIT:
        case CFG_TYPE_DYN_NOISE:
        case CFG_TYPE_DYN_EXPAND:
        case CFG_TYPE_DYN_PEAK:
            if(!_dynData) {
                _dynData = new SnapshotSaveDynData;
            }
            _dynData->saveDynData(configitem, type);
            break;
        case CFG_TYPE_PDUCKER_4:
        case CFG_TYPE_PDUCKER_8:
        case CFG_TYPE_PDUCKER_16:
        case CFG_TYPE_PDUCKER_32:
            if(!_priorityDuckerData) {
                _priorityDuckerData = new SnapshotSavePriorityDuckerData;
            }
            _priorityDuckerData->savePriorityDuckerData(configitem, type);
            break;
        case CFG_TYPE_EQ_1:
        case CFG_TYPE_EQ_3:
        case CFG_TYPE_EQ_5:
        case CFG_TYPE_EQ_7:
        case CFG_TYPE_EQ_10:
        case CFG_TYPE_EQ_16:
        case CFG_TYPE_EQ_1X3:
        case CFG_TYPE_EQ_2X3:
        case CFG_TYPE_EQ_3X3:
            if(!_eqData) {
                _eqData = new SnapshotSaveEqData;
            }
            _eqData->saveEqData(configitem, type);
            break;
        case CFG_TYPE_SPLIT_2:
        case CFG_TYPE_SPLIT_3:
        case CFG_TYPE_SPLIT_4:
            if(!_splitData) {
                _splitData = new SnapshotSaveSplitData;
            }
            _splitData->saveSplitData(configitem, type);
            break;
        case CFG_TYPE_FILTE_LP:
        case CFG_TYPE_FILTE_HP:
        case CFG_TYPE_FILTE_LS:
        case CFG_TYPE_FILTE_HS:
            if(!_filteData) {
                _filteData = new SnapshotSaveFilteData;
            }
            _filteData->saveFilteData(configitem, type);
            break;
        case CFG_TYPE_SIG_SINE:
        case CFG_TYPE_SIG_NOISE:
        case CFG_TYPE_SIG_PINKNOISE:
            if(!_signalData) {
                _signalData = new SnapshotSaveSignalData;
            }
            _signalData->saveSignalData(configitem, type);
            break;
        case CFG_TYPE_METER_1:
        case CFG_TYPE_METER_2:
        case CFG_TYPE_METER_4:
        case CFG_TYPE_METER_8:
            if(!_meterData) {
                _meterData = new SnapshotSaveMeterData;
            }
            _meterData->saveMeterData(configitem, type);
            break;
        case CFG_TYPE_GPI:
            if(!_gpiData) {
                _gpiData = new SnapshotSaveGpiData;
            }
            _gpiData->saveGpiData(configitem, type);
            break;
        case CFG_TYPE_FREQSHIFT:
        case CFG_TYPE_FREQSHIFT_PLUS:
            if(!_freqShiftData) {
                _freqShiftData = new SnapshotSaveFreqShiftData;
            }
            _freqShiftData->saveFreqShiftData(configitem, type);
            break;
        case CFG_TYPE_AUTOMIX_GS_4:
        case CFG_TYPE_AUTOMIX_GS_8:
        case CFG_TYPE_AUTOMIX_GS_16:
        case CFG_TYPE_AUTOMIX_GS_32:
        case CFG_TYPE_AUTOMIXNGRT_4:
        case CFG_TYPE_AUTOMIXNGRT_8:
        case CFG_TYPE_AUTOMIXNGRT_16:
        case CFG_TYPE_AUTOMIXNGRT_32:
        case CFG_TYPE_AUTOMIXNGAT_4:
        case CFG_TYPE_AUTOMIXNGAT_8:
        case CFG_TYPE_AUTOMIXNGAT_16:
        case CFG_TYPE_AUTOMIXNGAT_32:
            if(!_autoMixData) {
                _autoMixData = new SnapshotSaveAutoMixData;
            }
            _autoMixData->saveAutoMixData(configitem, type);
            break;
        case CFG_TYPE_AEC_2_2:
        case CFG_TYPE_SOFTAEC_8K128:
            if(!_aecData) {
                _aecData = new SnapshotSaveAecData;
            }
            _aecData->saveAecData(configitem, type);
            break;
        case CFG_TYPE_NFC1_8NF:
        case CFG_TYPE_NFC1_16NF:
        case CFG_TYPE_NFC1_24NF:
        case CFG_TYPE_NFC1_32NF:
            if(!_nfcData) {
                _nfcData = new SnapshotSaveNfcData;
            }
            _nfcData->saveNfcData(configitem, type);
            break;
        case CFG_TYPE_GAINRAMP_1:
        case CFG_TYPE_GAINRAMP_2:
        case CFG_TYPE_GAINRAMP_4:
        case CFG_TYPE_GAINRAMP_8:
        case CFG_TYPE_GAINRAMP_16:
        case CFG_TYPE_GAINRAMP_32:
            if(!_gainRampData) {
                _gainRampData = new SnapshotSaveGainRampData;
            }
            _gainRampData->saveGainRampData(configitem, type);
            break;
        case CFG_TYPE_GAIN_1:
        case CFG_TYPE_GAIN_2:
        case CFG_TYPE_GAIN_4:
        case CFG_TYPE_GAIN_8:
        case CFG_TYPE_GAIN_16:
        case CFG_TYPE_GAIN_32:
            if(!_gainData) {
                _gainData = new SnapshotSaveGainData;
            }
            _gainData->saveGainData(configitem, type);
            break;
        case CFG_TYPE_CROSSFADER_2X1:
        case CFG_TYPE_CROSSFADER_4X2:
        case CFG_TYPE_CROSSFADER_8X4:
        case CFG_TYPE_CROSSFADER_16X8:
        case CFG_TYPE_CROSSFADER_32X16:
            if(!_crossfaderData) {
                _crossfaderData = new SnapshotSaveCrossfaderData;
            }
            _crossfaderData->saveCrossfaderData(configitem, type);
            break;
        case CFG_TYPE_INPUT_1:
        case CFG_TYPE_INPUT_2:
        case CFG_TYPE_INPUT_4:
        case CFG_TYPE_INPUT_8:
        case CFG_TYPE_INPUT_16:
            if(!_inputData) {
                _inputData = new SnapshotSaveInputData;
            }
            _inputData->saveInputData(configitem, type);
            break;
        case CFG_TYPE_OUTPUT_1:
        case CFG_TYPE_OUTPUT_2:
        case CFG_TYPE_OUTPUT_4:
        case CFG_TYPE_OUTPUT_8:
        case CFG_TYPE_OUTPUT_16:
            if(!_outputData) {
                _outputData = new SnapshotSaveOutputData;
            }
            _outputData->saveOutputData(configitem, type);
            break;
        default:
            break;
        }
        _buttonPattern[type] = true;
    }

}

void ConfigItemSnapshot::onSnapCountChanged_slots(int value)
{
    if(value > DataCount) setSnapCount(DataCount);
    if(value < 1) setSnapCount(1);
    if(_cfgSettingSnapshot) {
        _cfgSettingSnapshot->disconnected();
        _cfgSettingSnapshot->initUI();
        _cfgSettingSnapshot->initData();
        _cfgSettingSnapshot->connected();
    }
}

void ConfigItemSnapshot::upLoalModuleData(QSettings *ConfigIni, QString SettingType,
                                           SnapshotSaveObject *dataObj, int pattern, CfgType moduleType)
{
    ConfigIni->beginGroup(QString(SettingType).arg(moduleType));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();
    ConfigIni->endGroup();
    if(unimoduleNum > 0)
    {
        if(!dataObj) {
             switch (moduleType) {
             case CFG_TYPE_DELAY_5:
             case CFG_TYPE_DELAY_10:
             case CFG_TYPE_DELAY_20:
             case CFG_TYPE_DELAY_50:
             case CFG_TYPE_DELAY_100:
             case CFG_TYPE_DELAY_500:
             case CFG_TYPE_DELAY_1000:
                 _delayData = new SnapshotSaveDelayData;
                 dataObj = _delayData; break;
             case CFG_TYPE_ROUTER_4:
             case CFG_TYPE_ROUTER_8:
             case CFG_TYPE_ROUTER_16:
                 _routerData = new SnapshotSaveRouterData;
                 dataObj = _routerData; break;
             case CFG_TYPE_MIXER_4X1:
             case CFG_TYPE_MIXER_4X2:
             case CFG_TYPE_MIXER_4X4:
             case CFG_TYPE_MIXER_8X1:
             case CFG_TYPE_MIXER_8X2:
             case CFG_TYPE_MIXER_8X4:
             case CFG_TYPE_MIXER_8X8:
             case CFG_TYPE_MIXER_16X16:
             case CFG_TYPE_MIXER_32X32:
             case CFG_TYPE_MIXER_64X64:
                 _mixerData = new SnapshotSaveMixerData;
                 dataObj = _mixerData; break;
             case CFG_TYPE_DYN_AUTO:
             case CFG_TYPE_DYN_COMP:
             case CFG_TYPE_DYN_EXPAND:
             case CFG_TYPE_DYN_LIMIT:
             case CFG_TYPE_DYN_NOISE:
             case CFG_TYPE_DYN_PEAK:
                 _dynData = new SnapshotSaveDynData;
                 dataObj = _dynData; break;
             case CFG_TYPE_PDUCKER_4:
             case CFG_TYPE_PDUCKER_8:
             case CFG_TYPE_PDUCKER_16:
             case CFG_TYPE_PDUCKER_32:
                 _priorityDuckerData = new SnapshotSavePriorityDuckerData;
                 dataObj = _priorityDuckerData; break;
             case CFG_TYPE_EQ_1:
             case CFG_TYPE_EQ_3:
             case CFG_TYPE_EQ_5:
             case CFG_TYPE_EQ_7:
             case CFG_TYPE_EQ_10:
             case CFG_TYPE_EQ_16:
             case CFG_TYPE_EQ_1X3:
             case CFG_TYPE_EQ_2X3:
             case CFG_TYPE_EQ_3X3:
                 _eqData = new SnapshotSaveEqData;
                 dataObj = _eqData; break;
             case CFG_TYPE_SPLIT_2:
             case CFG_TYPE_SPLIT_3:
             case CFG_TYPE_SPLIT_4:
                 _splitData = new SnapshotSaveSplitData;
                 dataObj = _splitData; break;
             case CFG_TYPE_FILTE_HP:
             case CFG_TYPE_FILTE_HS:
             case CFG_TYPE_FILTE_LP:
             case CFG_TYPE_FILTE_LS:
                 _filteData = new SnapshotSaveFilteData;
                 dataObj = _filteData; break;
             case CFG_TYPE_SIG_NOISE:
             case CFG_TYPE_SIG_PINKNOISE:
             case CFG_TYPE_SIG_SINE:
                 _signalData = new SnapshotSaveSignalData;
                 dataObj = _signalData; break;
             case CFG_TYPE_METER_1:
             case CFG_TYPE_METER_2:
             case CFG_TYPE_METER_4:
             case CFG_TYPE_METER_8:
                 _meterData = new SnapshotSaveMeterData;
                 dataObj = _meterData; break;
             case CFG_TYPE_GPI:
                 _gpiData = new SnapshotSaveGpiData;
                 dataObj = _gpiData; break;
             case CFG_TYPE_FREQSHIFT:
             case CFG_TYPE_FREQSHIFT_PLUS:
                 _freqShiftData = new SnapshotSaveFreqShiftData;
                 dataObj = _freqShiftData; break;
             case CFG_TYPE_AUTOMIX_GS_4:
             case CFG_TYPE_AUTOMIX_GS_8:
             case CFG_TYPE_AUTOMIX_GS_16:
             case CFG_TYPE_AUTOMIX_GS_32:
             case CFG_TYPE_AUTOMIXNGAT_4:
             case CFG_TYPE_AUTOMIXNGAT_8:
             case CFG_TYPE_AUTOMIXNGAT_16:
             case CFG_TYPE_AUTOMIXNGAT_32:
             case CFG_TYPE_AUTOMIXNGRT_4:
             case CFG_TYPE_AUTOMIXNGRT_8:
             case CFG_TYPE_AUTOMIXNGRT_16:
             case CFG_TYPE_AUTOMIXNGRT_32:
                 _autoMixData = new SnapshotSaveAutoMixData;
                 dataObj = _autoMixData; break;
             case CFG_TYPE_AEC_2_2:
             case CFG_TYPE_SOFTAEC_8K128:
                 _aecData = new SnapshotSaveAecData;
                 dataObj = _aecData; break;
             case CFG_TYPE_NFC1_8NF:
             case CFG_TYPE_NFC1_16NF:
             case CFG_TYPE_NFC1_24NF:
             case CFG_TYPE_NFC1_32NF:
                 _nfcData = new SnapshotSaveNfcData;
                 dataObj = _nfcData; break;
             case CFG_TYPE_INPUT_1:
             case CFG_TYPE_INPUT_2:
             case CFG_TYPE_INPUT_4:
             case CFG_TYPE_INPUT_8:
             case CFG_TYPE_INPUT_16:
                 _inputData = new SnapshotSaveInputData;
                 dataObj = _inputData; break;
             case CFG_TYPE_OUTPUT_1:
             case CFG_TYPE_OUTPUT_2:
             case CFG_TYPE_OUTPUT_4:
             case CFG_TYPE_OUTPUT_8:
             case CFG_TYPE_OUTPUT_16:
                 _outputData = new SnapshotSaveOutputData;
                 dataObj = _outputData; break;
             default:
                 return;
             }
        }
        dataObj->SettingToSnap(ConfigIni, pattern, moduleType);
        _buttonPattern[pattern] = true;
    }
}
