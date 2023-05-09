#include "configitemNFC.h"
#include "configSetting/cfgsettingNFC.h"

ConfigItemNFC::ConfigItemNFC(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    memset(&_body, 0, sizeof(IG_PARACTL_NFC));

    _body.outGain = (0 + 100) * 100;
    _body.outMode = 0;
    _body.clearfilter = 0;
    _body.bandwidth = 5;
    _body.fixfilters = 0;
    _body.dynfilters = 0;
    _body.maxNotchDepth = (0 + 100) * 100;
    addSubType(CFG_TYPE_NFC1_8NF);
    addSubType(CFG_TYPE_NFC1_16NF);
    addSubType(CFG_TYPE_NFC1_24NF);
    addSubType(CFG_TYPE_NFC1_32NF);
    initPort(1, 1);

    _setting = NULL;
}

ConfigItemNFC::~ConfigItemNFC()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemNFC::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingNFC(this);
    }

    return _setting;
}


QByteArray ConfigItemNFC::getDetailData()
{
    QByteArray paraBuff;

    paraBuff.append((const char *)&_body, sizeof(IG_PARACTL_NFC));

    return paraBuff;
}


void ConfigItemNFC::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(IG_PARACTL_NFC));
    if(_setting != NULL&& _setting->isVisible())
    {
        _setting->updateUiData();
    }
}

void ConfigItemNFC::setRtnData(unsigned int *d)
{
    if(_setting != NULL && _setting->isVisible())
    {
        switch (_cfgType) {
        case CFG_TYPE_NFC1_8NF:
        {
            IG_PARAGET_NFC_8NF * p = (IG_PARAGET_NFC_8NF *)d;
            for (int i = 0; i < 8; ++i)
            {
                _setting->setDisp(i, p->howl[i].howlFreqs, p->howl[i].rtNotchDepth * 0.01 - 100);
            }
            break;
        }
        case CFG_TYPE_NFC1_16NF:
        {
            IG_PARAGET_NFC_16NF * p = (IG_PARAGET_NFC_16NF *)d;
            for (int i = 0; i < 16; ++i)
            {
                _setting->setDisp(i, p->howl[i].howlFreqs, p->howl[i].rtNotchDepth * 0.01 - 100);
            }
            break;
        }
        case CFG_TYPE_NFC1_24NF:
        {
            IG_PARAGET_NFC_24NF * p = (IG_PARAGET_NFC_24NF *)d;
            for (int i = 0; i < 24; ++i)
            {
                _setting->setDisp(i, p->howl[i].howlFreqs, p->howl[i].rtNotchDepth * 0.01 - 100);
            }
            break;
        }
        case CFG_TYPE_NFC1_32NF:
        {
            IG_PARAGET_NFC_32NF * p = (IG_PARAGET_NFC_32NF *)d;
            for (int i = 0; i < 32; ++i)
            {
                _setting->setDisp(i, p->howl[i].howlFreqs, p->howl[i].rtNotchDepth * 0.01 - 100);
            }
            break;
        }
        }
    }
}

void ConfigItemNFC::updateCfgSettingLoadedData()
{
    if(_setting != NULL && _setting->isVisible())
    {
        _setting->updateUiData();
    }
}

void ConfigItemNFC::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_NFC_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    setOutGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_NFC_OUTGAIN)).toInt());
    setOutMode(ConfigIni->value(QString(CONFIG_DEVICE_DEV_NFC_OUTMODEL)).toInt());
    setBandwidth(ConfigIni->value(QString(CONFIG_DEVICE_DEV_NFC_BANDWIDTH)).toInt());
    setFixfilters(ConfigIni->value(QString(CONFIG_DEVICE_DEV_NFC_FIXFILTERS)).toInt());
    setDynfilters(ConfigIni->value(QString(CONFIG_DEVICE_DEV_NFC_DYNFILTERS)).toInt());
    setMaxNotchDepth(ConfigIni->value(QString(CONFIG_DEVICE_DEV_NFC_MAXNOTCHDEPTH)).toInt());
    ConfigIni->endGroup();
}

void ConfigItemNFC::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_NFC_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_NFC_OUTGAIN), getOutGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_NFC_OUTMODEL), getOutMode());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_NFC_BANDWIDTH), getBandwidth());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_NFC_FIXFILTERS), getFixfilters());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_NFC_DYNFILTERS), getDynfilters());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_NFC_MAXNOTCHDEPTH), getMaxNotchDepth());
    ConfigIni->endGroup();
}



void ConfigItemNFC::setOutGain(int d)
{
    _body.outGain = (d + 100) * 100;
    saveParamToDevice();
}

void ConfigItemNFC::setOutMode(unsigned char d)
{
    _body.outMode = d;
    saveParamToDevice();
}

void ConfigItemNFC::setClearFilter(unsigned char d)
{
    _body.clearfilter = d;
    saveParamToDevice();
    _body.clearfilter = 0;
}

void ConfigItemNFC::setBandwidth(unsigned char d)
{
    _body.bandwidth = d;
    saveParamToDevice();
}

void ConfigItemNFC::setFixfilters(unsigned char d)
{
    _body.fixfilters = d;
    saveParamToDevice();
}

void ConfigItemNFC::setDynfilters(unsigned char d)
{
    _body.dynfilters = d;
    saveParamToDevice();
}

void ConfigItemNFC::setMaxNotchDepth(int d)
{
    _body.maxNotchDepth = (d + 100) * 100;
    saveParamToDevice();
}
