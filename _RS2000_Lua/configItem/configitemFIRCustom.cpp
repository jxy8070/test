#include "configItem/configitemFIRCustom.h"
#include "configSetting/cfgsettingFIRCustom.h"

ConfigItemFIRCustom::ConfigItemFIRCustom(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    memset(&_body, 0, sizeof(IG_PARACTL_SOFTFIR));
    addSubType(CFG_TYPE_SOFTFIR_MONO256);
    addSubType(CFG_TYPE_SOFTFIR_MONO512);
    addSubType(CFG_TYPE_SOFTFIR_MONO1024);
    addSubType(CFG_TYPE_SOFTFIR_STEREO256);
    addSubType(CFG_TYPE_SOFTFIR_STEREO512);
    addSubType(CFG_TYPE_SOFTFIR_STEREO1024);
    addSubType(CFG_TYPE_SOFTFIR_4R256);
    addSubType(CFG_TYPE_SOFTFIR_4R512);
    addSubType(CFG_TYPE_SOFTFIR_4R1024);
    switch(getCfgType())
    {
    case CFG_TYPE_SOFTFIR_MONO256:
        _coeff_size = 256;
        initPort(1, 1);
        break;
    case CFG_TYPE_SOFTFIR_MONO512:
        _coeff_size = 512;
        initPort(1, 1);
        break;
    case CFG_TYPE_SOFTFIR_MONO1024:
        _coeff_size = 1024;
        initPort(1, 1);
        break;
    case CFG_TYPE_SOFTFIR_STEREO256:
        _coeff_size = 256;
        initPort(2, 2);
        break;
    case CFG_TYPE_SOFTFIR_STEREO512:
        _coeff_size = 512;
        initPort(2, 2);
        break;
    case CFG_TYPE_SOFTFIR_STEREO1024:
        _coeff_size = 1024;
        initPort(2, 2);
        break;
    case CFG_TYPE_SOFTFIR_4R256:
        _coeff_size = 256;
        initPort(4, 4);
        break;
    case CFG_TYPE_SOFTFIR_4R512:
        _coeff_size = 512;
        initPort(4, 4);
        break;
    case CFG_TYPE_SOFTFIR_4R1024:
        _coeff_size = 1024;
        initPort(4, 4);
        break;
    default:
        Q_ASSERT(false);
    }

    for (quint32 i = 0; i < _coeff_size; ++i)
    {
        _coeff[i] = 0;
    }
//    _bypass = false;
    _bypass = true;
    _setting = NULL;
}

ConfigItemFIRCustom::~ConfigItemFIRCustom()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemFIRCustom::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingFIRCustom(this);
    }

    return _setting;
}


QByteArray ConfigItemFIRCustom::getDetailData()
{
    QByteArray paraBuff;
#define Coeff_Bank_Size 256

    int bank = _coeff_size / Coeff_Bank_Size;
    for (int i = 0; i < bank; ++i)
    {
        for (int j = 0; j < Coeff_Bank_Size; ++j)
        {
            _body.coeff[j] = _bypass ? 0 : _coeff[Coeff_Bank_Size*i + j];
        }
        _body.shift  = i * Coeff_Bank_Size;
        _body.len    = Coeff_Bank_Size;
        _body.effect = (i == (bank - 1) ? true : false);
        _body.bypass = _bypass;
//        qDebug() << __FUNCTION__ << __LINE__ << _body.effect;
        paraBuff.append((const char *)&_body, sizeof(IG_PARACTL_SOFTFIR));
    }

    return paraBuff;
}



void ConfigItemFIRCustom::setDetailData(unsigned int *d)
{

}

void ConfigItemFIRCustom::setRtnData(unsigned int *d)
{

}

void ConfigItemFIRCustom::updateCfgSettingLoadedData()
{

}

void ConfigItemFIRCustom::loadParaFromSetting(QSettings *ConfigIni)
{
    assert("no loadParaFromSetting(QSettings *ConfigIni)" );
}

void ConfigItemFIRCustom::saveParaToSetting(QSettings *ConfigIni)
{
    assert("no saveParaToSetting(QSettings *ConfigIni)" );
}


void ConfigItemFIRCustom::setCoeff(QStringList s)
{
    int count = s.count() < _coeff_size ? s.count() : _coeff_size;

    for (quint32 i = 0; i < _coeff_size; ++i)
    {
        _coeff[i] = 0;
    }

    for (quint32 i = 0; i < count; ++i)
    {
        _coeff[i] = s.value(i).toFloat();
    }

    saveParamToDevice();
}

void ConfigItemFIRCustom::setBypass(bool e)
{
    _bypass = e;
    saveParamToDevice();
}
