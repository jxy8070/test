#include "configitemfilte.h"
#include "configSetting/cfgsettingfilte.h"
#include "protocal/packet.h"
#include "customWidget/eqgraph.h"

ConfigItemFilte::ConfigItemFilte(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    int inNum = 1, outNum = 1;
    _bypass = true;
    memset(&_body, 0, sizeof(IG_PARACTL_FILTER));
    memset(&_body_BandPass, 0, sizeof(IG_PARACTL_BPFILTER));
    memset(&_body_AllPass, 0, sizeof(IG_PARACTL_APFILTER));
    _smooth = 0;

    _filterType   = 5;
    _eqData = new EqData;
    _eqData->freq = 20;
    _eqData->gain = 0;
    _eqData->q    = 100;

#if Logo_FangTu /* Function */
    addSubType(cfgType);
#else
    addSubType(CFG_TYPE_FILTE_LP);
    addSubType(CFG_TYPE_FILTE_HP);
    addSubType(CFG_TYPE_FILTE_LS);
    addSubType(CFG_TYPE_FILTE_HS);
    addSubType(CFG_TYPE_FILTER_BP);
#endif

    switch(getCfgType()) {
    case CFG_TYPE_FILTE_LP:   //低通
        _eqData->freq = 1000;
        _eqType = EQ_LOWPASS_CONFIG;
        break;
    case CFG_TYPE_FILTE_HP:   //高通
        _eqData->freq = 1000;
        _eqType = EQ_HIGHPASS_CONFIG;
        break;
    case CFG_TYPE_FILTE_LS:   //低通斜坡
        _eqData->freq = 16000;
        _eqType = EQ_LOWSHELF_CONFIG;
        break;
    case CFG_TYPE_FILTE_HS:   //高通斜坡
        _eqData->freq = 80;
        _eqType = EQ_HIGHSHELF_CONFIG;
        break;
    case CFG_TYPE_FILTER_BP:
        _eqData->freq = 1000;
        _eqType = EQ_BANDPASS_CONFIG;
        break;
    case CFG_TYPE_FILTER_AP:
        _eqData->freq = 1000;
        _eqData->q    = 70;//0.707
        _eqType = EQ_ALLPASS_CONFIG;
        _body_AllPass.antiphase = 0;
        _body_AllPass.mute      = 0;
        break;
    }

    initPort(inNum, outNum);

    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "Type",  0x00090001,  0x00090002));
    pinList.append(new CLuaPinParam_PC(this, name, "Freq",  0x00090003,  0x00090004));
//    pinList.append(new CLuaPinParam_PC(this, name, "Gain",  0x00090005,  0x00090006));   //软件中Gain及Q值不可调整，暂时隐藏
//    pinList.append(new CLuaPinParam_PC(this, name, "Bandwidth",  0x00090009,  0x0009000A));
    pinList.append(new CLuaPinParam_PC(this, name, "Bypass",  0x00090007,  0x00090008));
    initControlPins(pinList);
    _setting = NULL;
}

ConfigItemFilte::~ConfigItemFilte()
{
    delete _eqData;
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemFilte::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingFilte(this);
    }
    return _setting;
}


QByteArray ConfigItemFilte::getDetailData()
{
    quint8 order[12] = {
        2, 4, 6, 8, 1, 2, 3, 4, 5, 6, 7, 8
    };
    QByteArray paraBuff;

    switch(getCfgType())
    {
    case CFG_TYPE_FILTE_LP:   //低通
    case CFG_TYPE_FILTE_HP:   //高通
    case CFG_TYPE_FILTE_LS:   //低通斜坡
    case CFG_TYPE_FILTE_HS:   //高通斜坡
        _body.smooth = _smooth;
        _body.bypass = _bypass ? 0xFFFFFFFF : 0;
        _body.filterType = _filterType > 3 ? FCT_FILTER_BUTTERWORTH : FCT_FILTER_LINKWITZ;
        _body.orders = order[_filterType];
        _body.freq_hz = _eqData->freq;
        _body.gain = (100 + _eqData->gain / 100.) * 100;

        paraBuff.append((const char*)&_body, sizeof(IG_PARACTL_FILTER));
        break;
    case CFG_TYPE_FILTER_BP:
        _body_BandPass.smooth = _smooth;
        _body_BandPass.bypass = _bypass ? 0xFFFFFFFF : 0;
        _body_BandPass.freq_hz = _eqData->freq;
        _body_BandPass.bandWidth = _eqData->q * 10;

        paraBuff.append((const char*)&_body_BandPass, sizeof(IG_PARACTL_BPFILTER));
        break;
    case CFG_TYPE_FILTER_AP:
        _body_AllPass.freq_hz = _eqData->freq;
        _body_AllPass.q = _eqData->q;
        _body_AllPass.bypass = _bypass ? 0xFFFFFFFF : 0;
        _body_AllPass.smooth = _smooth;

        paraBuff.append((const char*)&_body_AllPass, sizeof(_body_AllPass));
    }


    _smooth = 0;
    return paraBuff;
}


void ConfigItemFilte::setDetailData(unsigned int *d)
{
    switch(getCfgType()) {
    case CFG_TYPE_FILTE_LP:   //低通
    case CFG_TYPE_FILTE_HP:   //高通
    case CFG_TYPE_FILTE_LS:   //低通斜坡
    case CFG_TYPE_FILTE_HS:   //高通斜坡
        memcpy(&_body, d, sizeof(IG_PARACTL_FILTER));
        _smooth = _body.smooth;
        _bypass = (_body.bypass == 0xFFFFFFFF) ? 1 : 0;
        if (_body.filterType == FCT_FILTER_BUTTERWORTH) {
            _filterType = _body.orders + 3;
        }
        else {
            _filterType = (_body.orders >> 1) - 1;
        }

        _eqData->freq = _body.freq_hz;
        _eqData->gain = ((_body.gain / 100) - 100) * 100;
        break;
    case CFG_TYPE_FILTER_BP:
        memcpy(&_body_BandPass, d, sizeof(IG_PARACTL_BPFILTER));
        _smooth = _body_BandPass.smooth;
        _bypass = (_body_BandPass.bypass == 0xFFFFFFFF) ? 1 : 0;
        _eqData->freq = _body_BandPass.freq_hz;
        _eqData->q    = _body_BandPass.bandWidth / 10;
        break;
    case CFG_TYPE_FILTER_AP:
        memcpy(&_body_AllPass, d, sizeof(IG_PARACTL_APFILTER));
        _smooth = _body_AllPass.smooth;
        _bypass = (_body_AllPass.bypass == 0xFFFFFFFF) ? 1 : 0;
        _eqData->freq = _body_AllPass.freq_hz;
        _eqData->q    = _body_AllPass.q;
        break;
    }

    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemFilte::setRtnData(unsigned int *d)
{

}

void ConfigItemFilte::updateCfgSettingLoadedData()
{
    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemFilte::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_FILTER_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    setBypass(ConfigIni->value(QString(CONFIG_DEVICE_DEV_FILTER_BYPASS)).toBool());
    setFilterType(ConfigIni->value(QString(CONFIG_DEVICE_DEV_FILTER_FILTERTYPE)).toUInt());
    setEqPara(ConfigIni->value(QString(CONFIG_DEVICE_DEV_FILTER_FREQ)).toInt(),
                   ConfigIni->value(QString(CONFIG_DEVICE_DEV_FILTER_GAIN)).toInt(), getEqData()->q);
    ConfigIni->endGroup();
}

void ConfigItemFilte::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_FILTER_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FILTER_BYPASS), getBypass());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FILTER_FILTERTYPE), getFilterType());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FILTER_FREQ), getEqData()->freq);
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FILTER_GAIN), getEqData()->gain);
    ConfigIni->endGroup();
}

QVariant ConfigItemFilte::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    EqData* eData = getEqData();
    switch(param->SetCmdKey())
    {
    case 0x00090002:
        setFilterType(val.toInt());
        return QVariant(val.toInt());
    case 0x00090004:
        setEqPara(val.toInt(), eData->gain, eData->q);
        return QVariant(val.toInt());
//    case 0x00090006:
//        setEqPara(eData->freq, val.toDouble()* 100, eData->q);
//        return QVariant(val.toDouble());
    case 0x00090008:
        setBypass(val.toBool());
        return QVariant(val.toBool());
//    case 0x0009000A:
//        setEqPara(eData->freq, eData->gain, val.toDouble()* 100);
//        return QVariant(val.toDouble());
    default:
        return QVariant();
    }
}

void ConfigItemFilte::setBypass(bool bypass)
{
    _bypass = bypass;
    _smooth = 0;
    saveParamToDevice();
}

void ConfigItemFilte::setFilterType(quint16 type)
{
    _filterType = type;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemFilte::setEqPara(int freq, int gain, int q)
{
    _eqData->freq = freq;
    _eqData->gain = gain;
    _eqData->q    = q;
//    qDebug() << __FUNCTION__ << __LINE__
//             << freq << gain << q;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemFilte::setMute(bool enable)
{
    _body_AllPass.mute = enable;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemFilte::setInvert(bool enable)
{
    _body_AllPass.antiphase = enable;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}
