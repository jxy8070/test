#include "configitemgpi.h"
#include "configSetting/cfgsettinggpi.h"
#include <QByteArray>

ConfigItemGPI::ConfigItemGPI(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    initPort(2, 1);

    addSubType(CFG_TYPE_GPI);
    memset(&_body, 0, sizeof(IG_PARACTL_GPI));
    _body.gpiNo = 1;
    _body.hw_gpiDecMode = 1;
    _setting = NULL;
}

ConfigItemGPI::~ConfigItemGPI()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemGPI::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingGPI(this);
    }
    return _setting;
}


QByteArray ConfigItemGPI::getDetailData()
{
    QByteArray paraBuff;
    paraBuff.append((const char *)&_body, sizeof(IG_PARACTL_GPI));

    return paraBuff;
}


void ConfigItemGPI::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(IG_PARACTL_GPI));

    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemGPI::setRtnData(unsigned int *d)
{

}

void ConfigItemGPI::updateCfgSettingLoadedData()
{
    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemGPI::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_GPI_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    setGpiIdx(ConfigIni->value(QString(CONFIG_DEVICE_DEV_GPI_GPINUM)).toUInt());
    ConfigIni->endGroup();
}

void ConfigItemGPI::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_GPI_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_GPI_GPINUM), getGpiIdx());
    ConfigIni->endGroup();
}

QVariant ConfigItemGPI::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    //该模块暂时没有Lua控制针脚
    Q_UNUSED(param)
    assert(false);
    return val;
}

void ConfigItemGPI::setGpiIdx(quint32 idx)
{
    _body.gpiNo = idx +1;
    saveParamToDevice();
}
