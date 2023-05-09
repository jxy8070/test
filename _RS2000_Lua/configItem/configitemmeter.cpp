#include "configitemmeter.h"
#include "configSetting/cfgsettingmeter.h"

ConfigItemMeter::ConfigItemMeter(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    int inNum = 1, outNum = 0;
    memset(&_body_1, 0, sizeof(IG_PARACTL_METER1));
    memset(&_body_2, 0, sizeof(IG_PARACTL_METER2));
    memset(&_body_4, 0, sizeof(IG_PARACTL_METER4));
    memset(&_body_8, 0, sizeof(IG_PARACTL_METER8));
#if Logo_FangTu /* Function */
    addSubType(cfgType);
#else
    addSubType(CFG_TYPE_METER_1);
    addSubType(CFG_TYPE_METER_2);
    addSubType(CFG_TYPE_METER_4);
    addSubType(CFG_TYPE_METER_8);
#endif
    _meterNum = 1;
    switch(getCfgType())
    {
    case CFG_TYPE_METER_1:
        _meterNum = 1;
        break;
    case CFG_TYPE_METER_2:
        _meterNum = 2;
        break;
    case CFG_TYPE_METER_4:
        _meterNum = 4;
        break;
    case CFG_TYPE_METER_8:
        _meterNum = 8;
        break;
    default:
        Q_ASSERT(false);
    }

    for (int i = 0; i < 8; ++i)
    {
        _holdTime[i] = 100;
        _holdEnable[i] = true;
        _infiniteHold[i] = false;
    }

    _rowCol = 0xFFFFFFFF;
    inNum = _meterNum;
    initPort(inNum, outNum);

    _setting = NULL;
}

ConfigItemMeter::~ConfigItemMeter()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

DesignItem *ConfigItemMeter::clone(DesignItem *fromItem)
{
    DesignItem *item = ConfigItem::clone(fromItem);
    if (item != NULL) {
        ConfigItemMeter *toItem = qgraphicsitem_cast<ConfigItemMeter *>(item);
        for (int i = 0; i < _meterNum; ++i) {
            toItem->setHoldEnable(i, getHoldEnable(i));
            toItem->setInfiniteHole(i, getInfiniteHold(i));
            toItem->setUserName(i, getUserName(i));
        }
    }

    return item;
}

QWidget *ConfigItemMeter::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingMeter(this);
    }
    return _setting;
}


QByteArray ConfigItemMeter::getDetailData()
{
    QByteArray paraBuff;

    switch (_cfgType) {
    case CFG_TYPE_METER_1:
        for (int i = 0; i < 1; ++i)
        {
            if (_infiniteHold[i])
            {
                _body_1.meter[i].ppmHoldCtrlMode = DEV_METER_HOLDMODE_KEEPMAXVALUE;
            } else if (_holdEnable[i]) {
                _body_1.meter[i].ppmHoldCtrlMode = DEV_METER_HOLDMODE_KEEPSETTIME;
            } else {
                _body_1.meter[i].ppmHoldCtrlMode = DEV_METER_HOLDMODE_DEFAULT;
            }
            _body_1.meter[i].ppmHoldTime_ms = _holdTime[i];
        }
        paraBuff.append((const char*)&_body_1, sizeof(IG_PARACTL_METER1));
        break;
    case CFG_TYPE_METER_2:
        for (int i = 0; i < 2; ++i)
        {
            if (_infiniteHold[i])
            {
                _body_2.meter[i].ppmHoldCtrlMode = DEV_METER_HOLDMODE_KEEPMAXVALUE;
            } else if (_holdEnable[i]) {
                _body_2.meter[i].ppmHoldCtrlMode = DEV_METER_HOLDMODE_KEEPSETTIME;
            } else {
                _body_2.meter[i].ppmHoldCtrlMode = DEV_METER_HOLDMODE_DEFAULT;
            }
            _body_2.meter[i].ppmHoldTime_ms = _holdTime[i];
        }
        paraBuff.append((const char*)&_body_2, sizeof(IG_PARACTL_METER2));
        break;
    case CFG_TYPE_METER_4:
        for (int i = 0; i < 4; ++i)
        {
            if (_infiniteHold[i])
            {
                _body_4.meter[i].ppmHoldCtrlMode = DEV_METER_HOLDMODE_KEEPMAXVALUE;
            } else if (_holdEnable[i]) {
                _body_4.meter[i].ppmHoldCtrlMode = DEV_METER_HOLDMODE_KEEPSETTIME;
            } else {
                _body_4.meter[i].ppmHoldCtrlMode = DEV_METER_HOLDMODE_DEFAULT;
            }
            _body_4.meter[i].ppmHoldTime_ms = _holdTime[i];
        }
        paraBuff.append((const char*)&_body_4, sizeof(IG_PARACTL_METER4));
        break;
    case CFG_TYPE_METER_8:
        for (int i = 0; i < 8; ++i)
        {
            if (_infiniteHold[i])
            {
                _body_8.meter[i].ppmHoldCtrlMode = DEV_METER_HOLDMODE_KEEPMAXVALUE;
            } else if (_holdEnable[i]) {
                _body_8.meter[i].ppmHoldCtrlMode = DEV_METER_HOLDMODE_KEEPSETTIME;
            } else {
                _body_8.meter[i].ppmHoldCtrlMode = DEV_METER_HOLDMODE_DEFAULT;
            }
            _body_8.meter[i].ppmHoldTime_ms = _holdTime[i];
        }
        paraBuff.append((const char*)&_body_8, sizeof(IG_PARACTL_METER8));
        break;
    default:
        break;
    }

    return paraBuff;
}

void ConfigItemMeter::setDetailData(unsigned int *d)
{
    switch (_cfgType) {
    case CFG_TYPE_METER_1:
        memcpy(&_body_1, d, sizeof(IG_PARACTL_METER1));
        for(int i = 0; i < 1; i++)
        {
            _holdTime[i] = _body_1.meter[i].ppmHoldTime_ms;
        }
        break;
    case CFG_TYPE_METER_2:
        memcpy(&_body_2, d, sizeof(IG_PARACTL_METER2));
        for(int i = 0; i < 2; i++)
        {
            _holdTime[i] = _body_2.meter[i].ppmHoldTime_ms;
        }
        break;
    case CFG_TYPE_METER_4:
        memcpy(&_body_4, d, sizeof(IG_PARACTL_METER4));
        for(int i = 0; i < 4; i++)
        {
            _holdTime[i] = _body_4.meter[i].ppmHoldTime_ms;
        }
        break;
    case CFG_TYPE_METER_8:
        memcpy(&_body_8, d, sizeof(IG_PARACTL_METER8));
        for(int i = 0; i < 8; i++)
        {
            _holdTime[i] = _body_8.meter[i].ppmHoldTime_ms;
        }
        break;
    default:
        break;
    }

    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemMeter::setRtnData(unsigned int *d)
{

}

void ConfigItemMeter::updateCfgSettingLoadedData()
{
    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemMeter::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_METER_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    for (int j = 0; j < 8; ++j)
    {
        setUserName(j, ConfigIni->value(QString(CONFIG_DEVICE_DEV_METER_USERNAME).arg(j)).toString());
    }
    ConfigIni->endGroup();
}

void ConfigItemMeter::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_METER_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    for (int i = 0; i < 8; ++i)
    {
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_METER_USERNAME).arg(i), getUserName(i));
    }
    ConfigIni->endGroup();
}

QVariant ConfigItemMeter::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    //该模块暂时没有Lua控制针脚
    Q_UNUSED(param)
    assert(false);
    return val;
}

void ConfigItemMeter::clearVuData()
{
    if(_setting)
    {
        _setting->clearAllVuData();
    }
}

void ConfigItemMeter::setHoldTime(int idx, int vlu)
{
    _holdTime[idx] = vlu;
    saveParamToDevice();
}

void ConfigItemMeter::setHoldEnable(int idx, bool flag)
{
    _holdEnable[idx] = flag;
    saveParamToDevice();
}

void ConfigItemMeter::setInfiniteHole(int idx, bool flag)
{
    _infiniteHold[idx] = flag;
    saveParamToDevice();
}

void ConfigItemMeter::setUserName(int idx, QString name)
{
    _userName[idx] = name;
    saveParamToDevice();
}
