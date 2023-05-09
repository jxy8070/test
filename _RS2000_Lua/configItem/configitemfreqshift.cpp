#include "configitemfreqshift.h"
#include "configSetting/cfgsettingfreqshift.h"
#include "protocal/packet.h"

ConfigItemFreqShift::ConfigItemFreqShift(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    addSubType(CFG_TYPE_FREQSHIFT);
//    addSubType(CFG_TYPE_FREQSHIFT_PLUS);  //杨杰: 当时做测试用

    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        initPort(1, 1);
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        initPort(1, 2);
        break;
    default:
        break;
    }
    memset(&_body, 0, sizeof(IG_PARACTL_FREQSHIFT));
    memset(&_body_plus, 0, sizeof(IG_PARACTL_FREQSHIFTP));

    _body.shiftfreq_hz = 5;
    _body.gain = (0 + 100) * 100;

    _body_plus.shiftfreq_hz = 5;
    _body_plus.phase = (0 + 180);
    _body_plus.gain = (0 + 100) * 100;
    _body_plus.shiftfreqMode = DEV_FREQSHIFTP_WM_FIXFREQ;
    _body_plus.shiftfreqSpan_hz = 0;
    _body_plus.shiftfreqSpeed_hz = 1;

    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "Freq", 0x000A0001,  0x000A0002));
    pinList.append(new CLuaPinParam_PC(this, name, "Gain", 0x000A0003,  0x000A0004));
    initControlPins(pinList);
    _setting = NULL;
}

ConfigItemFreqShift::~ConfigItemFreqShift()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemFreqShift::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingFreqShift(this);
    }
    return _setting;
}


QByteArray ConfigItemFreqShift::getDetailData()
{
    QByteArray paraBuff;

    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        paraBuff.append((const char *)&_body, sizeof(IG_PARACTL_FREQSHIFT));
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        paraBuff.append((const char *)&_body_plus, sizeof(IG_PARACTL_FREQSHIFTP));
        break;
    default:
        break;
    }
    return paraBuff;
}


void ConfigItemFreqShift::setDetailData(unsigned int *d)
{
    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        memcpy(&_body, d, sizeof(IG_PARACTL_FREQSHIFT));
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        memcpy(&_body_plus, d, sizeof(IG_PARACTL_FREQSHIFTP));
        break;
    default:
        break;
    }

    if(_setting != NULL)
    {
        _setting->upDateUiData();
    }
}

void ConfigItemFreqShift::setRtnData(unsigned int *d)
{

}

void ConfigItemFreqShift::updateCfgSettingLoadedData()
{
    if(_setting != NULL)
    {
        _setting->upDateUiData();
    }
}

void ConfigItemFreqShift::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_FREQSHIFT_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    SetFreq(ConfigIni->value(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQ)).toUInt());
    SetGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_FREQSHIFT_GAIN)).toInt());
    SetPhase(ConfigIni->value(QString(CONFIG_DEVICE_DEV_FREQSHIFT_PHASE)).toUInt());
    SetFreqMode(ConfigIni->value(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQMODE)).toUInt());
    SetFreqSpan(ConfigIni->value(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQSPAN)).toUInt());
    SetFreqSpeed(ConfigIni->value(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQSPEED)).toUInt());
    ConfigIni->endGroup();
}

void ConfigItemFreqShift::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_FREQSHIFT_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQ), GetFreq());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_GAIN), GetGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_PHASE), GetPhase());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQMODE), GetFreqMode());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQSPAN), GetFreqSpan());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQSPEED), GetFreqSpeed());
    ConfigIni->endGroup();
}

QVariant ConfigItemFreqShift::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    switch(param->SetCmdKey())
    {
    case 0x000A0002:
        SetFreq(val.toInt());
        return QVariant(val.toInt());
    case 0x000A0004:
        SetGain(val.toDouble());
        return QVariant(val.toDouble());
    default:
        return QVariant();
    }
}

uint8_t ConfigItemFreqShift::GetFreq()
{
    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        return _body.shiftfreq_hz;
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        return _body_plus.shiftfreq_hz;
        break;
    default:
        break;
    }

    return 0;
}

int8_t ConfigItemFreqShift::GetGain()
{
    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        return _body.gain*0.01-100;
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        return _body_plus.gain*0.01-100;
        break;
    default:
        break;
    }

    return 0;
}

int16_t ConfigItemFreqShift::GetPhase()
{
    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        return _body_plus.phase - 180;
        break;
    default:
        break;
    }

    return 0;
}

uint32_t ConfigItemFreqShift::GetFreqMode()
{
    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        return _body_plus.shiftfreqMode;
        break;
    default:
        break;
    }

    return 0;
}

uint32_t ConfigItemFreqShift::GetFreqSpan()
{
    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        return _body_plus.shiftfreqSpan_hz;
        break;
    default:
        break;
    }

    return 0;
}

uint32_t ConfigItemFreqShift::GetFreqSpeed()
{
    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        return _body_plus.shiftfreqSpeed_hz;
        break;
    default:
        break;
    }

    return 0;
}

void ConfigItemFreqShift::SetFreq(uint8_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        _body.shiftfreq_hz = value;
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        _body_plus.shiftfreq_hz = value;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemFreqShift::SetGain(int8_t value)
{

    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        _body.gain = (value + 100) * 100;
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        _body_plus.gain = (value + 100) * 100;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemFreqShift::SetPhase(int16_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        _body_plus.phase = (value + 180);
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemFreqShift::SetFreqMode(uint32_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        _body_plus.shiftfreqMode = value;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemFreqShift::SetFreqSpan(uint32_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        _body_plus.shiftfreqSpan_hz = value;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemFreqShift::SetFreqSpeed(uint32_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_FREQSHIFT:
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        _body_plus.shiftfreqSpeed_hz = value;
        break;
    default:
        break;
    }

    saveParamToDevice();
}
