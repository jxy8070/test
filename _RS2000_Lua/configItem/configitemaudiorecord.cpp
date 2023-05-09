#include "configitemaudiorecord.h"
#include "configSetting/cfgsettingaudiorecord.h"

int ConfigItemAudioRecord::GlobalRecordIdx = 0;

ConfigItemAudioRecord::ConfigItemAudioRecord(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    int inNum = 2, outNum = 0;
    memset(&_body, 0, sizeof(IG_PARACTL_RECORD));

    addSubType(CFG_TYPE_AUDIO_RECORD1);
    addSubType(CFG_TYPE_AUDIO_RECORD2);
    addSubType(CFG_TYPE_AUDIO_RECORD4);
    if(cfgType == CFG_TYPE_AUDIO_RECORD1)  inNum = 1;
    else if(cfgType == CFG_TYPE_AUDIO_RECORD2)  inNum = 2;
    else if(cfgType == CFG_TYPE_AUDIO_RECORD4)  inNum = 4;
    initPort(inNum, outNum);
    initData();

    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "Record", 0xFFFFFFFF, 0xFFFFFFFF)); //wj
    pinList.append(new CLuaPinParam_PC(this, name, "Stop", 0xFFFFFFFF, 0xFFFFFFFF));
    initControlPins(pinList);

    _setting = NULL;
}

ConfigItemAudioRecord::~ConfigItemAudioRecord()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

void ConfigItemAudioRecord::initData()
{
    _recordIdx = GlobalRecordIdx++;
    _recordCmd = 0;
    _sampleRate = 0;
    memset(_recordPath, 0, sizeof(_recordPath));
    memset(_recordPerfix, 0, sizeof(_recordPerfix));
}

QWidget *ConfigItemAudioRecord::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingAudioRecord(this);
    }
    return _setting;
}


QByteArray ConfigItemAudioRecord::getDetailData()
{
    QByteArray paraBuff;
    _body.recordIdx = _recordIdx;
    _body.recordCmd = _recordCmd;
    _body.sampleRate = _sampleRate;
    memcpy(_body.recordPath, _recordPath, sizeof(_recordPath));
    memcpy(_body.recordPerfix, _recordPerfix, sizeof(_recordPerfix));
    paraBuff.append((const char*)&_body, sizeof(IG_PARACTL_RECORD));

    return paraBuff;
}


void ConfigItemAudioRecord::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(IG_PARACTL_RECORD));
//    if(_setting != NULL)
//    {
//        _setting->updateUiData();
//    }
}

void ConfigItemAudioRecord::setRtnData(unsigned int *d)
{

}

void ConfigItemAudioRecord::updateCfgSettingLoadedData()
{
//    if(_setting != NULL)
//    {
//        _setting->updateUiData();
//    }
}


DesignItem* ConfigItemAudioRecord::clone(DesignItem* toItem)
{
    ConfigItemAudioRecord *item = NULL;
    return item;
}


void ConfigItemAudioRecord::loadParaFromSetting(QSettings *ConfigIni)
{
//    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SIG_GROUP));
//    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
//    setMute(ConfigIni->value(QString(CONFIG_DEVICE_DEV_SIG_MUTE)).toBool());
//    setLevel(ConfigIni->value(QString(CONFIG_DEVICE_DEV_SIG_LEVEL)).toDouble());
//    setFreq(ConfigIni->value(QString(CONFIG_DEVICE_DEV_SIG_FREQ)).toDouble());
//    ConfigIni->endGroup();
}


void ConfigItemAudioRecord::saveParaToSetting(QSettings *ConfigIni)
{
//    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SIG_GROUP));
//    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
//    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SIG_MUTE), getMute());
//    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SIG_LEVEL), getLevel());
//    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SIG_FREQ), getFreq());
//    ConfigIni->endGroup();
}

QVariant ConfigItemAudioRecord::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    assert(false); //wj 需要实现
//    switch(param->SetCmdKey())
//    {
//    case 0x0000FFFF:
//        break;
//    }
    return QVariant();
}

void ConfigItemAudioRecord::start()
{
    _recordCmd = 0x01;
    saveParamToDevice();
}

void ConfigItemAudioRecord::stop()
{
     _recordCmd = 0x00;
     saveParamToDevice();
}

