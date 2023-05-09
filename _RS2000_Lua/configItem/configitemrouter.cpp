//#include "stable.h"
#include "configitemrouter.h"
#include "configSetting/cfgsettingrouter.h"

ConfigItemRouter::ConfigItemRouter(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    memset(&_body, 0, sizeof(IG_PARACTL_VROUTER));

    addSubType(cfgType);
//    addSubType(CFG_TYPE_ROUTER_4);
//    addSubType(CFG_TYPE_ROUTER_8);
//    addSubType(CFG_TYPE_ROUTER_16);
//    addSubType(CFG_TYPE_ROUTER_32);
//    addSubType(CFG_TYPE_ROUTER_48);
//    addSubType(CFG_TYPE_ROUTER_64);

    _cfgType = cfgType;
    _portNum = 0;
    switch(_cfgType) {
    case CFG_TYPE_ROUTER_4: _portNum = 4; break;
    case CFG_TYPE_ROUTER_8: _portNum = 8; break;
    case CFG_TYPE_ROUTER_16: _portNum = 16; break;
    case CFG_TYPE_ROUTER_32: _portNum = 32; break;
    case CFG_TYPE_ROUTER_48: _portNum = 48; break;
    case CFG_TYPE_ROUTER_64: _portNum = 64; break;
    case CFG_TYPE_ROUTER_96: _portNum = 96; break;
    case CFG_TYPE_ROUTER_128: _portNum = 128; break;
    case CFG_TYPE_ROUTER_192: _portNum = 192; break;
    case CFG_TYPE_ROUTER_256: _portNum = 256; break;
    default: break;
    }
    initPort(_portNum, _portNum);

    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    for(int i = 0; i< _outPortList.length(); i++)
    {
        pinList.append(new CLuaPinParam_PC(this, name, QString("Out%1").arg(i),
                                           0x00030001, 0x00030002, (quint8)i, 0x00));
    }
    initControlPins(pinList);

    _setting = NULL;
}

ConfigItemRouter::~ConfigItemRouter()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemRouter::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingRouter(this);
    }
    return _setting;
}


QByteArray ConfigItemRouter::getDetailData()
{
    QByteArray paraBuff;
    paraBuff.append((const char*)&_body, sizeof(uint32)*_portNum);
    return paraBuff;
}


void ConfigItemRouter::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(uint32)*_portNum);
    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemRouter::setRtnData(unsigned int *d)
{

}

void ConfigItemRouter::updateCfgSettingLoadedData()
{
    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemRouter::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_ROUTER_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    for (quint16 j = 0; j < getPortNum(); ++j)
    {
        setRouterInfo(j, ConfigIni->value(QString(CONFIG_DEVICE_DEV_ROUTER_ROUTERINFO).arg(j)).toUInt());
    }
    ConfigIni->endGroup();
}

void ConfigItemRouter::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_ROUTER_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    for (int i = 0; i < getPortNum(); ++i)
    {
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_ROUTER_ROUTERINFO).arg(i), getRouterInfo(i));
    }
    ConfigIni->endGroup();
}


QVariant ConfigItemRouter::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    switch(param->SetCmdKey())
    {
    case 0x00030002:
        setRouterInfo(param->GetExData1(), val.toInt());
        return QVariant(val.toInt());
    default:
        return QVariant();
    }
}

quint16 ConfigItemRouter::getRouterInfo(int idx)
{
    return _body.routerch[idx].itoo;
}

quint16 ConfigItemRouter::getPortNum()
{
    return _portNum;
}

void ConfigItemRouter::setRouterInfo(int idx, quint16 vlu)
{
    _body.routerch[idx].itoo = vlu;
    saveParamToDevice();
}
