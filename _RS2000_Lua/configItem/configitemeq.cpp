#include "configitemeq.h"
#include "configSetting/cfgsettingeq.h"
#include "protocal/packet.h"
#include "customWidget/eqgraph.h"

ConfigItemEq::ConfigItemEq(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    int inNum = 1, outNum = 1;
    _smooth = 0;
    _qValue = -1;
    switch (cfgType) {
    case CFG_TYPE_EQ_1:
    case CFG_TYPE_EQ_3:
    case CFG_TYPE_EQ_5:
    case CFG_TYPE_EQ_7:
    case CFG_TYPE_EQ_10:
    case CFG_TYPE_EQ_16:
        addSubType(CFG_TYPE_EQ_3);
        addSubType(CFG_TYPE_EQ_5);
        addSubType(CFG_TYPE_EQ_10);
        addSubType(CFG_TYPE_EQ_16);
        memset(&_body_peq, 0, sizeof(_body_peq));
        break;
    case CFG_TYPE_EQ_1X3:
    case CFG_TYPE_EQ_2X3:
    case CFG_TYPE_EQ_3X3:
        addSubType(CFG_TYPE_EQ_1X3);
        addSubType(CFG_TYPE_EQ_2X3);
        addSubType(CFG_TYPE_EQ_3X3);
        memset(&_body_geq, 0, sizeof(_body_geq));
        break;
    default: break;
    }

    switch(cfgType) {
    case CFG_TYPE_EQ_1:
        _eqCount = 1;
        break;
    case CFG_TYPE_EQ_3:
         _eqCount = 3;
        break;
    case CFG_TYPE_EQ_5:
         _eqCount = 5;
        break;
    case CFG_TYPE_EQ_7:
         _eqCount = 7;
        break;
    case CFG_TYPE_EQ_10:
         _eqCount = 10;
        break;
    case CFG_TYPE_EQ_16:
         _eqCount = 16;
        break;
    case CFG_TYPE_EQ_1X3:
         _eqCount = 31;
         _freqList<<20<<25<<32<<40<<50<<63<<80<<100<<125<<160<<200<<250<<320
                     <<400<<500<<640<<800<<1000<<1300<<1600<<2000<<2600<<3200
                    <<4000<<5100<<6500<<8100<<10000<<13000<<16000<<20000;
         _qValue = 4.3;
        break;
    case CFG_TYPE_EQ_2X3:
         _eqCount = 15;
         _freqList<<25<<40<<63<<100<<160<<250<<400<<635<<1000<<1600<<2500
                     <<4000<<6400<<10000<<16000;
         _qValue = 2.1;
        break;
    case CFG_TYPE_EQ_3X3:
         _eqCount = 10;
         _freqList<<32<<63<<125<<250<<500<<1000<<2000<<4000<<8000<<16000;
         _qValue = 1.4;
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    if (_qValue == -1) //如果是peq
    {
        int step = EqGraph::SCALE_CNT / _eqCount;
        _qValue = 1.0;
        for(int i = 0; i< _eqCount; i++)
        {
            int fs = 20 * (pow(10.0, 3.0 * (i*step + step/2) / (EqGraph::SCALE_CNT - 1)));
            _freqList.append(fs);
        }
    }

    initPort(inNum, outNum);
    initEq();

    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "Bypass", 0x00060007, 0x00060008));
    for(int i = 0; i< _eqCount; i++)
    {
        QString groupName = QString("Point%1").arg(i+1);
        pinList.append(new CLuaPinParam_PC(this, name, "Freq", 0x00060001, 0x00060002, (quint8)i, 0x00, groupName));
        pinList.append(new CLuaPinParam_PC(this, name, "Gain", 0x00060003, 0x00060004, (quint8)i, 0x00, groupName));
        pinList.append(new CLuaPinParam_PC(this, name, "QVal", 0x00060005, 0x00060006, (quint8)i, 0x00, groupName));
    }
    initControlPins(pinList);
    _setting = NULL;
}

ConfigItemEq::~ConfigItemEq()
{
    qDeleteAll(_eqList);
    if (_setting != NULL)
    {
        delete _setting;
    }
}

void ConfigItemEq::initEq()
{
    _segBypass = 0x80000000;
    _eqList.clear();
    for(int i = 0; i < _eqCount; i++)
    {
        EqData *eqData = new EqData;
        eqData->freq = _freqList.at(i);
        eqData->gain = 0;
        eqData->q = _qValue * 100;
        eqData->eqType = FCT_PEQ_PEAK;
        _eqList.append(eqData);
    }
}

QWidget *ConfigItemEq::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingEq(this);
    }
    return _setting;
}

QByteArray ConfigItemEq::getDetailData()
{
    QByteArray paraBuff;
    quint32 bypass = _segBypass;
    if(_segBypass & 0x80000000) bypass = 0xFFFFFFFF;
    switch (_cfgType) {
    case CFG_TYPE_EQ_1:       
    case CFG_TYPE_EQ_3:
    case CFG_TYPE_EQ_5:
    case CFG_TYPE_EQ_7:
    case CFG_TYPE_EQ_10:
    case CFG_TYPE_EQ_16:
        for (int i = 0; i < _eqCount; ++i)
        {
            _body_peq[i].freq_hz = _eqList.at(i)->freq;
            _body_peq[i].gain = ((_eqList.at(i)->gain / 100.0) + 100) * 100;
            _body_peq[i].q = (_eqList[i]->q / 100.0) / 0.01;
            _body_peq[i].eq_type = _eqList.at(i)->eqType;
        }
        paraBuff.append((const char*)&_body_peq, sizeof(IG_PARACTL_PEQ)*_eqList.count());
        paraBuff.append((const char*)&bypass, sizeof(quint32));
        paraBuff.append((const char*)&_smooth, sizeof(quint32));
        break;
    case CFG_TYPE_EQ_1X3:
    case CFG_TYPE_EQ_2X3:
    case CFG_TYPE_EQ_3X3:
        for (int i = 0; i < _eqCount; ++i)
        {
            _body_geq[i].gain = (_eqList.at(i)->gain*0.01 + 100) * 100;
        }
        paraBuff.append((const char*)&_body_geq, sizeof(IG_PARACTL_GEQ)*_eqList.count());
        paraBuff.append((const char*)&bypass, sizeof(quint32));
        paraBuff.append((const char*)&_smooth, sizeof(quint32));
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    _smooth = 0;
    return paraBuff;
}


void ConfigItemEq::setDetailData(unsigned int *d)
{
    int quint32Cnt = 0;
    switch(_cfgType) {
    case CFG_TYPE_EQ_1:
    case CFG_TYPE_EQ_3:
    case CFG_TYPE_EQ_5:
    case CFG_TYPE_EQ_7:
    case CFG_TYPE_EQ_10:
    case CFG_TYPE_EQ_16:
        memcpy(&_body_peq, d, sizeof(IG_PARACTL_PEQ)*_eqCount);
        for (int i = 0; i < _eqList.count(); ++i)
        {
            _eqList.at(i)->freq = _body_peq[i].freq_hz;
            _eqList.at(i)->gain = (_body_peq[i].gain*0.01 - 100) * 100;
            _eqList.at(i)->q    = _body_peq[i].q;
            _eqList.at(i)->eqType = _body_peq[i].eq_type;
        }
        quint32Cnt = sizeof(IG_PARACTL_PEQ)*_eqCount /4;
        memcpy(&_segBypass, d + quint32Cnt, sizeof(quint32));
        memcpy(&_smooth, d + quint32Cnt+1, sizeof(quint32));
        break;
    case CFG_TYPE_EQ_1X3:    //1/3倍频
    case CFG_TYPE_EQ_2X3:    //2/3倍频
    case CFG_TYPE_EQ_3X3:    //1倍频
        memcpy(&_body_geq, d, sizeof(IG_PARACTL_GEQ)*_eqCount);
        for (int i = 0; i < _eqList.count(); ++i) {
            _eqList.at(i)->gain = ((_body_geq[i].gain / 100) - 100) * 100;
        }
        quint32Cnt = sizeof(IG_PARACTL_PEQ)*_eqCount /4;
        memcpy(&_segBypass, d + quint32Cnt, sizeof(quint32));
        memcpy(&_smooth, d + quint32Cnt+1, sizeof(quint32));
        break;
    default:
        Q_ASSERT(false);
    }

    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemEq::setRtnData(unsigned int *d)
{

}

void ConfigItemEq::updateCfgSettingLoadedData()
{
    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemEq::setEqData(quint8 idx, bool seg_bypass, int freq, int gain, int q, int type)
{
    _eqList.at(idx)->freq = freq;
    _eqList.at(idx)->gain = gain;
    _eqList.at(idx)->q = q;
    _eqList.at(idx)->eqType = type;
    _segBypass &= ~(0x1 << idx);
    _segBypass |= (seg_bypass << idx);
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemEq::setEqData(quint8 idx, int gain)
{
    _eqList.at(idx)->gain = gain;

    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemEq::setAllBypass(bool bypass)
{
    if(bypass){
        _smooth = 0;
        _segBypass |= 0x80000000;
    }
    else{
        _smooth = 0xFFFFFFFF;
        _segBypass &= 0x7FFFFFFF;
    }
    saveParamToDevice();
}

void ConfigItemEq::setSegBypass(quint32 idx, bool seg_bypass)
{
    _segBypass &= ~(0x1 << idx);
    _segBypass |= (seg_bypass << idx);
//    qDebug() << __FUNCTION__ << __LINE__ << idx << seg_bypass << _segBypass;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemEq::reinit()
{
    initEq();
    saveParamToDevice();
}

void ConfigItemEq::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_EQ_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    setAllBypass(ConfigIni->value(QString(CONFIG_DEVICE_DEV_EQ_BYPASS)).toBool());
    for (int j = 0; j < getEqDataList().count(); ++j)
    {
        setEqData(j, ConfigIni->value(QString(CONFIG_DEVICE_DEV_EQ_SEGBYPASS).arg(j)).toBool(),
                     ConfigIni->value(QString(CONFIG_DEVICE_DEV_EQ_FREQ).arg(j)).toUInt(),
                     ConfigIni->value(QString(CONFIG_DEVICE_DEV_EQ_GAIN).arg(j)).toInt(),
                     ConfigIni->value(QString(CONFIG_DEVICE_DEV_EQ_Q).arg(j)).toUInt(), 0);
    }
    ConfigIni->endGroup();
}

void ConfigItemEq::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_EQ_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_EQ_BYPASS), getAllBypass());
    for (int i = 0; i < getEqDataList().count(); ++i)
    {
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_EQ_SEGBYPASS).arg(i), getSegBypass(i));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_EQ_FREQ).arg(i), getEqDataList().at(i)->freq);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_EQ_GAIN).arg(i), getEqDataList().at(i)->gain);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_EQ_Q).arg(i), getEqDataList().at(i)->q);
    }
    ConfigIni->endGroup();
}

QVariant ConfigItemEq::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    EqData *eData = NULL;
    int idx = param->GetExData1();
    if(idx >= 0 && idx < _eqList.count())
        eData = _eqList.at(idx);
    switch(param->SetCmdKey())
    {
    case 0x00060002:
        eData->freq = val.toInt();
        saveParamToDevice();
        return QVariant(val.toInt());
    case 0x00060004:
        eData->gain = val.toDouble() * 100;
        saveParamToDevice();
        return QVariant(val.toDouble());
    case 0x00060006:
        eData->q = val.toDouble() * 100;
        saveParamToDevice();
        return QVariant(val.toInt());
    case 0x00060008:
        setAllBypass(val.toBool());
        saveParamToDevice();
        return QVariant(val.toBool());
    default:
        return QVariant();
    }
}
