#include "configitemmixer.h"
#include "configSetting/cfgsettingmixer.h"
#include "configSetting/cfgsettingmixer_kemu.h"

ConfigItemMixer::ConfigItemMixer(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    memset(&_body_4x1, 0, sizeof(IG_PARACTL_VMIXER4X1));
    memset(&_body_4x2, 0, sizeof(IG_PARACTL_VMIXER4X2));
    memset(&_body_4x4, 0, sizeof(IG_PARACTL_VMIXER4X4));
    memset(&_body_8x1, 0, sizeof(IG_PARACTL_VMIXER8X1));
    memset(&_body_8x2, 0, sizeof(IG_PARACTL_VMIXER8X2));
    memset(&_body_8x4, 0, sizeof(IG_PARACTL_VMIXER8X4));
    memset(&_body_8x8, 0, sizeof(IG_PARACTL_VMIXER8X8));
    memset(&_body_16x16, 0, sizeof(IG_PARACTL_VMIXER16X16));
    memset(&_body_32x32, 0, sizeof(IG_PARACTL_VMIXER32X32));
    memset(&_body_64x64, 0, sizeof(IG_PARACTL_VMIXER64X64));


    memset(_nodeSw, 0, 2*64*sizeof(uint32));
    memset(chSwIn,  0, 2*sizeof(uint32));
    memset(chSwOut, 0, 2*sizeof(uint32));

    switch (cfgType) {
    case CFG_TYPE_MIXER_4X1:
    case CFG_TYPE_MIXER_4X2:
    case CFG_TYPE_MIXER_4X4:
        addSubType(CFG_TYPE_MIXER_4X1);
        addSubType(CFG_TYPE_MIXER_4X2);
        addSubType(CFG_TYPE_MIXER_4X4);
        break;
    case CFG_TYPE_MIXER_8X1:
    case CFG_TYPE_MIXER_8X2:
    case CFG_TYPE_MIXER_8X4:
    case CFG_TYPE_MIXER_8X8:
        addSubType(CFG_TYPE_MIXER_8X1);
        addSubType(CFG_TYPE_MIXER_8X2);
        addSubType(CFG_TYPE_MIXER_8X4);
        addSubType(CFG_TYPE_MIXER_8X8);
        break;
    case CFG_TYPE_MIXER_16X16:
        addSubType(CFG_TYPE_MIXER_16X16);
        break;
    case CFG_TYPE_MIXER_32X32:
        addSubType(CFG_TYPE_MIXER_32X32);
        break;
    case CFG_TYPE_MIXER_64X64:
        addSubType(CFG_TYPE_MIXER_64X64);
        break;
    }

    createLabelTextList();

    switch(cfgType) {
    case CFG_TYPE_MIXER_4X1:
        _body_4x1.inSwitch = 0;
        _body_4x1.outSwitch = 0;
        for (int i = 0; i < 2; ++i)
        {
            _body_4x1.ingain[i].gainA = (0 + 100) * 100;
            _body_4x1.ingain[i].gainB = (0 + 100) * 100;
        }
        for (int i = 0; i < 1; ++i)
        {
            _body_4x1.outgain[i].gain = (0 + 100) * 100;

            _body_4x1.iToOSwitch[i].sw = 0xFFFF;

            for (int j = 0; j < 2; ++j)
            {
                _body_4x1.iToOGain[i][j].gainA = (-100 + 100) * 100;
                _body_4x1.iToOGain[i][j].gainB = (-100 + 100) * 100;
            }
        }
        initPort(4, 1, _inLabel, _outLabel);
        break;
    case CFG_TYPE_MIXER_4X2:
        _body_4x2.inSwitch = 0;
        _body_4x2.outSwitch = 0;
        for (int i = 0; i < 2; ++i)
        {
            _body_4x2.ingain[i].gainA = (0 + 100) * 100;
            _body_4x2.ingain[i].gainB = (0 + 100) * 100;

            for (int j = 0; j < 2; ++j)
            {
                _body_4x2.iToOGain[i][j].gainA = (-100 + 100) * 100;
                _body_4x2.iToOGain[i][j].gainB = (-100 + 100) * 100;
            }
        }
        for (int i = 0; i < 1; ++i)
        {
            _body_4x2.outgain[i].gainA = (0 + 100) * 100;
            _body_4x2.outgain[i].gainB = (0 + 100) * 100;

            _body_4x2.iToOSwitch[i].swA = 0xFFFF;
            _body_4x2.iToOSwitch[i].swB = 0xFFFF;
        }
        initPort(4, 2, _inLabel, _outLabel);
        break;
    case CFG_TYPE_MIXER_4X4:
        _body_4x4.inSwitch = 0;
        _body_4x4.outSwitch = 0;
        for (int i = 0; i < 2; ++i)
        {
            _body_4x4.ingain[i].gainA = (0 + 100) * 100;
            _body_4x4.ingain[i].gainB = (0 + 100) * 100;

            _body_4x4.outgain[i].gainA = (0 + 100) * 100;
            _body_4x4.outgain[i].gainB = (0 + 100) * 100;

            _body_4x4.iToOSwitch[i].swA = 0xFFFF;
            _body_4x4.iToOSwitch[i].swB = 0xFFFF;

            for (int j = 0; j < 4; ++j)
            {
                _body_4x4.iToOGain[j][i].gainA = (-100 + 100) * 100;
                _body_4x4.iToOGain[j][i].gainB = (-100 + 100) * 100;
            }
        }
        initPort(4, 4, _inLabel, _outLabel);
        break;
    case CFG_TYPE_MIXER_8X1:
        _body_8x1.inSwitch = 0;
        _body_8x1.outSwitch = 0;
        for (int i = 0; i < 4; ++i)
        {
            _body_8x1.ingain[i].gainA = (0 + 100) * 100;
            _body_8x1.ingain[i].gainB = (0 + 100) * 100;
        }
        for (int i = 0; i < 1; ++i)
        {
            _body_8x1.outgain[i].gain = (0 + 100) * 100;

            _body_8x1.iToOSwitch[i].sw = 0xFFFF;

            for (int j = 0; j < 4; ++j)
            {
                _body_8x1.iToOGain[i][j].gainA = (-100 + 100) * 100;
                _body_8x1.iToOGain[i][j].gainB = (-100 + 100) * 100;
            }
        }
        initPort(8, 1, _inLabel, _outLabel);
        break;
    case CFG_TYPE_MIXER_8X2:
        _body_8x2.inSwitch = 0;
        _body_8x2.outSwitch = 0;
        for (int i = 0; i < 4; ++i)
        {
            _body_8x2.ingain[i].gainA = (0 + 100) * 100;
            _body_8x2.ingain[i].gainB = (0 + 100) * 100;

            for (int j = 0; j < 4; ++j)
            {
                _body_8x2.iToOGain[i][j].gainA = (-100 + 100) * 100;
                _body_8x2.iToOGain[i][j].gainB = (-100 + 100) * 100;
            }
        }
        for (int i = 0; i < 1; ++i)
        {
            _body_8x2.outgain[i].gainA = (0 + 100) * 100;
            _body_8x2.outgain[i].gainB = (0 + 100) * 100;

            _body_8x2.iToOSwitch[i].swA = 0xFFFF;
            _body_8x2.iToOSwitch[i].swB = 0xFFFF;
        }
        initPort(8, 2, _inLabel, _outLabel);
        break;
    case CFG_TYPE_MIXER_8X4:
        _body_8x4.inSwitch = 0;
        _body_8x4.outSwitch = 0;
        for (int i = 0; i < 4; ++i)
        {
            _body_8x4.ingain[i].gainA = (0 + 100) * 100;
            _body_8x4.ingain[i].gainB = (0 + 100) * 100;

            for (int j = 0; j < 4; ++j)
            {
                _body_8x4.iToOGain[i][j].gainA = (-100 + 100) * 100;
                _body_8x4.iToOGain[i][j].gainB = (-100 + 100) * 100;
            }
        }
        for (int i = 0; i < 2; ++i)
        {
            _body_8x4.outgain[i].gainA = (0 + 100) * 100;
            _body_8x4.outgain[i].gainB = (0 + 100) * 100;

            _body_8x4.iToOSwitch[i].swA = 0xFFFF;
            _body_8x4.iToOSwitch[i].swB = 0xFFFF;
        }
        initPort(8, 4, _inLabel, _outLabel);
        break;
    case CFG_TYPE_MIXER_8X8:
        _body_8x8.inSwitch = 0;
        _body_8x8.outSwitch = 0;
        for (int i = 0; i < 4; ++i)
        {
            _body_8x8.ingain[i].gainA = (0 + 100) * 100;
            _body_8x8.ingain[i].gainB = (0 + 100) * 100;

            _body_8x8.outgain[i].gainA = (0 + 100) * 100;
            _body_8x8.outgain[i].gainB = (0 + 100) * 100;

            _body_8x8.iToOSwitch[i].swA = 0xFFFF;
            _body_8x8.iToOSwitch[i].swB = 0xFFFF;

            for (int j = 0; j < 8; ++j)
            {
                _body_8x8.iToOGain[j][i].gainA = (-100 + 100) * 100;
                _body_8x8.iToOGain[j][i].gainB = (-100 + 100) * 100;
            }
        }
        initPort(8, 8, _inLabel, _outLabel);
        break;
    case CFG_TYPE_MIXER_16X16:
        _body_16x16.inSwitch = 0;
        _body_16x16.outSwitch = 0;
        for (int i = 0; i < 8; ++i)
        {
            _body_16x16.ingain[i].gainA = (0 + 100) * 100;
            _body_16x16.ingain[i].gainB = (0 + 100) * 100;

            _body_16x16.outgain[i].gainA = (0 + 100) * 100;
            _body_16x16.outgain[i].gainB = (0 + 100) * 100;

            _body_16x16.iToOSwitch[i].swA = 0xFFFF;
            _body_16x16.iToOSwitch[i].swB = 0xFFFF;

            for (int j = 0; j < 16; ++j)
            {
                _body_16x16.iToOGain[j][i].gainA = (-100 + 100) * 100;
                _body_16x16.iToOGain[j][i].gainB = (-100 + 100) * 100;
            }
        }
        initPort(16, 16, _inLabel, _outLabel);
        break;
    case CFG_TYPE_MIXER_32X32:
        for(int i = 0; i < 32; i++)
        {
            _nodeSw[i][0]   = 0xFFFFFFFF;
            _nodeSw[i][1]   = 0xFFFFFFFF;
            for (int j = 0; j < 32; ++j)
            {
                _nodeGain[i][j] = (-100 + 100) * 100;
            }

            _inAndOutGain[i][IN_SELECT] = (0 + 100) * 100;
            _inAndOutGain[i][OUT_SELECT] = (0 + 100) * 100;
        }
        initPort(32, 32, _inLabel, _outLabel);
        break;
    case CFG_TYPE_MIXER_64X64:
        for(int i = 0; i < 64; i++)
        {
            _nodeSw[i][0]   = 0xFFFFFFFF;
            _nodeSw[i][1]   = 0xFFFFFFFF;
            for (int j = 0; j < 64; ++j)
            {
                _nodeGain[i][j] = (-100 + 100) * 100;
            }

            _inAndOutGain[i][IN_SELECT] = (0 + 100) * 100;
            _inAndOutGain[i][OUT_SELECT] = (0 + 100) * 100;
        }
        initPort(64, 64, _inLabel, _outLabel);
        break;
//    case CFG_TYPE_MIXER_20X20:
//        inNum = 20; outNum = 20;
//        break;
    default:
        Q_ASSERT(false);
        break;
    }

    memset(_displayRange, 0, sizeof(_displayRange));

    //控制针脚初始化
    if(_inPortList.length()<= 8){ //lua针脚过多导致刷新效率低
        QList<CLuaPinParam_PC*> pinList;
        for(int i = 0; i< _inPortList.length(); i++)
        {
            QString groupName = QString("In%1").arg(i+1);
            pinList.append(new CLuaPinParam_PC(this, name, "State", 0x00040001, 0x00040002, (quint8)i, 0x00, groupName));
            pinList.append(new CLuaPinParam_PC(this, name, "Gain", 0x00040003, 0x00040004, (quint8)i, 0x00, groupName));
            for(int j = 0; j<  _outPortList.length(); j++)
            {
                QString groupName2 = QString("Out%1").arg(j+1);
                pinList.append(new CLuaPinParam_PC(this, name, "State", 0x00040009, 0x0004000A, (quint8)i, (quint8)j, groupName, groupName2));
                pinList.append(new CLuaPinParam_PC(this, name, "Gain", 0x0004000B, 0x0004000C, (quint8)i, (quint8)j, groupName, groupName2));
            }
        }
        for(int i = 0; i< _outPortList.length(); i++)
        {
            QString groupName = QString("Out%1").arg(i+1);
            pinList.append(new CLuaPinParam_PC(this, name, "State", 0x00040005, 0x00040006, (quint8)i, 0x00, groupName));
            pinList.append(new CLuaPinParam_PC(this, name, "Gain", 0x00040007, 0x00040008, (quint8)i, 0x00, groupName));
        }
        initControlPins(pinList);
    }

    _NodesSwitch = false;
    connect(this, &ConfigItemMixer::nodesSwitchChanged_signal, this, &ConfigItemMixer::nodesSwitchChanged_slot);
}

ConfigItemMixer::~ConfigItemMixer()
{
    if(_setting != NULL) {
        delete  _setting;
    }
    if(_setting_Kemu != NULL) {
        delete  _setting_Kemu;
    }
}

QWidget *ConfigItemMixer::getSettingForm()
{
    if(Config::curMfrsCfg().CurMfrs == Mfrs_KEMU){
        if(_setting_Kemu == NULL) {
            _setting_Kemu = new CfgSettingMixer_Kemu(this);
        }
        return _setting_Kemu;
    }
    else{
        if(_setting == NULL) {
            _setting = new CfgSettingMixer(this);
        }
        return _setting;
    }
}


QByteArray ConfigItemMixer::getDetailData()
{
    QByteArray paraBuff;

    switch(_cfgType) {
    case CFG_TYPE_MIXER_4X1:
        paraBuff.append((const char*)&_body_4x1, sizeof(IG_PARACTL_VMIXER4X1));
        break;
    case CFG_TYPE_MIXER_4X2:
        paraBuff.append((const char*)&_body_4x2, sizeof(IG_PARACTL_VMIXER4X2));
        break;
    case CFG_TYPE_MIXER_4X4:
        paraBuff.append((const char*)&_body_4x4, sizeof(IG_PARACTL_VMIXER4X4));
        break;
    case CFG_TYPE_MIXER_8X1:
        paraBuff.append((const char*)&_body_8x1, sizeof(IG_PARACTL_VMIXER8X1));
        break;
    case CFG_TYPE_MIXER_8X2:
        paraBuff.append((const char*)&_body_8x2, sizeof(IG_PARACTL_VMIXER8X2));
        break;
    case CFG_TYPE_MIXER_8X4:
        paraBuff.append((const char*)&_body_8x4, sizeof(IG_PARACTL_VMIXER8X4));
        break;
    case CFG_TYPE_MIXER_8X8:
        paraBuff.append((const char*)&_body_8x8, sizeof(IG_PARACTL_VMIXER8X8));
        break;
    case CFG_TYPE_MIXER_16X16:
        paraBuff.append((const char*)&_body_16x16, sizeof(IG_PARACTL_VMIXER16X16));
        break;
    case CFG_TYPE_MIXER_32X32:
        for (int i = 0; i < 32; i++)
        {
            prepare32BodyData(i, ROW_SELECT);
            paraBuff.append((const char*)&_body_32x32, sizeof(IG_PARACTL_VMIXER32X32));
        }
        for(int i = 0; i < 32; i++)
        {
            prepare32BodyData(i, COL_SELECT);
            paraBuff.append((const char*)&_body_32x32, sizeof(IG_PARACTL_VMIXER32X32));
        }
        break;
    case CFG_TYPE_MIXER_64X64:
        for (int i = 0; i < 64; i++)
        {
            prepare64BodyData(i, ROW_SELECT);
            paraBuff.append((const char*)&_body_64x64, sizeof(IG_PARACTL_VMIXER64X64));
        }
        for(int i = 0; i < 64; i++)
        {
            prepare64BodyData(i, COL_SELECT);
            paraBuff.append((const char*)&_body_64x64, sizeof(IG_PARACTL_VMIXER64X64));
        }
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    return paraBuff;
}

QByteArray ConfigItemMixer::getOnlineData()
{
    QByteArray paraBuff;
    switch(_cfgType)
    {
    case CFG_TYPE_MIXER_32X32:
        paraBuff.append((const char*)&_body_32x32, sizeof(IG_PARACTL_VMIXER32X32));
        break;
    case CFG_TYPE_MIXER_64X64:
        paraBuff.append((const char*)&_body_64x64, sizeof(IG_PARACTL_VMIXER64X64));
        break;
    default:
        break;
    }
    return paraBuff;
}

void ConfigItemMixer::setDetailData(unsigned int *d)
{
    switch (_cfgType) {
    case CFG_TYPE_MIXER_4X1:
        memcpy(&_body_4x1, d, sizeof(IG_PARACTL_VMIXER4X1));
        break;
    case CFG_TYPE_MIXER_4X2:
        memcpy(&_body_4x2, d, sizeof(IG_PARACTL_VMIXER4X2));
        break;
    case CFG_TYPE_MIXER_4X4:
        memcpy(&_body_4x4, d, sizeof(IG_PARACTL_VMIXER4X4));
        break;
    case CFG_TYPE_MIXER_8X1:
        memcpy(&_body_8x1, d, sizeof(IG_PARACTL_VMIXER8X1));
        break;
    case CFG_TYPE_MIXER_8X2:
        memcpy(&_body_8x2, d, sizeof(IG_PARACTL_VMIXER8X2));
        break;
    case CFG_TYPE_MIXER_8X4:
        memcpy(&_body_8x4, d, sizeof(IG_PARACTL_VMIXER8X4));
        break;
    case CFG_TYPE_MIXER_8X8:
        memcpy(&_body_8x8, d, sizeof(IG_PARACTL_VMIXER8X8));
        break;
    case CFG_TYPE_MIXER_16X16:
        memcpy(&_body_16x16, d, sizeof(IG_PARACTL_VMIXER16X16));
        break;
    case CFG_TYPE_MIXER_32X32:
        memcpy(&_body_32x32, d, sizeof(IG_PARACTL_VMIXER32X32));
        setDev_32_Data();
        break;
    case CFG_TYPE_MIXER_64X64:
        memcpy(&_body_64x64, d, sizeof(IG_PARACTL_VMIXER64X64));
        setDev_64_Data();
        break;
    default:
        break;
    }
    updateCfgSettingLoadedData();
}

void ConfigItemMixer::setRtnData(unsigned int *d)
{

}

void ConfigItemMixer::updateCfgSettingLoadedData()
{
    if(_setting != NULL) {
        _setting->updateUiData();
    }
    if(_setting_Kemu != NULL) {
        _setting_Kemu->updateUiData();
    }
}

void ConfigItemMixer::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_MIXER_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    for (int j = 0; j < getInNum(); ++j)
    {
        setInGainMute(j,
                           ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_INGAIN).arg(j)).toDouble(),
                           ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_INMUTE).arg(j)).toBool());
    }
    for (int i = 0; i < getOutNum(); ++i)
    {
        for (int j = 0; j < getInNum(); ++j)
        {
            setRouterGain(i, j,
                               ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_ROUTERGAIN).arg(i).arg(j)).toInt());

            if(j < 32)
            {
                setRouterInfo((i|0x00), ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_OUTROUTER_L).arg(i)).toUInt());
            }else {
                setRouterInfo((i|0x80), ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_OUTROUTER_H).arg(i)).toUInt());
            }
        }
    }
    for (int j = 0; j < getOutNum(); ++j)
    {
        setOutGainMute(j,
                            ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_OUTGAIN).arg(j)).toDouble(),
                            ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_OUTMUTE).arg(j)).toBool());
    }

    for (int i = 0; i < 64; ++i) {
        setLabText(false, i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_InUserLabel).arg(i)).toString());
        setLabText(true, i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_OutUserLabel).arg(i)).toString());
//        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_InUserLabel).arg(i), _inAndOutLabel[0][i]);
//        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_OutUserLabel).arg(i), _inAndOutLabel[1][i]);
    }
    ConfigIni->endGroup();
}

void ConfigItemMixer::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_MIXER_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    for (int i = 0; i < getInNum(); ++i)
    {
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_INGAIN).arg(i), getInGainMute(i).gain);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_INMUTE).arg(i), getInGainMute(i).mute);
    }
    for (int i = 0; i < getOutNum(); ++i)
    {
        for (int j = 0; j < getInNum(); ++j)
        {
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_ROUTERGAIN).arg(i).arg(j), getRouterGain(i, j));

            if(j < 32)
            {
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_OUTROUTER_L).arg(i), getRouterInfo(i|0x00));
            }else {
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_OUTROUTER_H).arg(i), getRouterInfo(i|0x80));
            }
        }
    }
    for (int i = 0; i < getOutNum(); ++i)
    {
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_OUTGAIN).arg(i), getOutGainMute(i).gain);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_OUTMUTE).arg(i), getOutGainMute(i).mute);
    }
    for (int i = 0; i < 64; ++i) {
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_InUserLabel).arg(i), getLabText(false, i));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_OutUserLabel).arg(i), getLabText(true, i));
    }
    ConfigIni->endGroup();
}

quint32 ConfigItemMixer::getRouterInfo(int idx)
{
//    qDebug()<<__FUNCTION__<<__LINE__<<idx;//行
    int z = idx / 2;
    int x = idx % 2;

    switch(_cfgType) {
    case CFG_TYPE_MIXER_4X1:
        return _body_4x1.iToOSwitch[idx].sw;
    case CFG_TYPE_MIXER_4X2:
        if (x)
        {
            return _body_4x2.iToOSwitch[z].swB;
        } else {
            return _body_4x2.iToOSwitch[z].swA;
        }
    case CFG_TYPE_MIXER_4X4:
        if (x)
        {
            return _body_4x4.iToOSwitch[z].swB;
        } else {
            return _body_4x4.iToOSwitch[z].swA;
        }
    case CFG_TYPE_MIXER_8X1:
        return _body_8x1.iToOSwitch[idx].sw;
    case CFG_TYPE_MIXER_8X2:
        if (x)
        {
            return _body_8x2.iToOSwitch[z].swB;
        } else {
            return _body_8x2.iToOSwitch[z].swA;
        }
    case CFG_TYPE_MIXER_8X4:
        if (x)
        {
            return _body_8x4.iToOSwitch[z].swB;
        } else {
            return _body_8x4.iToOSwitch[z].swA;
        }
    case CFG_TYPE_MIXER_8X8:
        if (x)
        {
            return _body_8x8.iToOSwitch[z].swB;
        } else {
            return _body_8x8.iToOSwitch[z].swA;
        }
    case CFG_TYPE_MIXER_16X16:
        if (x)
        {
            return _body_16x16.iToOSwitch[z].swB;
        } else {
            return _body_16x16.iToOSwitch[z].swA;
        }
    case CFG_TYPE_MIXER_32X32:
        return _nodeSw[idx][NODE_SW_L32];
    case CFG_TYPE_MIXER_64X64:
//        qDebug() << __FUNCTION__ << __LINE__ << idx << (idx & 0x80) << (idx & 0x7F);
        if (idx & 0x80)
        {
            return _nodeSw[(idx&0x7F)][NODE_SW_H32];
        } else {
            return _nodeSw[(idx&0x7F)][NODE_SW_L32];
        }
    default:
        Q_ASSERT(false);
        break;
    }

    return 0;
}

bool ConfigItemMixer::getRouterMute(int row, int col)
{
    int z = row / 2;
    int x = row % 2;

    switch (_cfgType) {
    case CFG_TYPE_MIXER_4X1:
        return _body_4x1.iToOSwitch[row].sw & (0x1 << col);
    case CFG_TYPE_MIXER_4X2:
        if(x) {
            return _body_4x2.iToOSwitch[z].swB & (0x1 << col);
        }else {
            return _body_4x2.iToOSwitch[z].swA & (0x1 << col);
        }
     case CFG_TYPE_MIXER_4X4:
        if(x) {
            return _body_4x4.iToOSwitch[z].swB & (0x1 << col);
        }else {
            return _body_4x4.iToOSwitch[z].swA & (0x1 << col);
        }
    case CFG_TYPE_MIXER_8X1:
        return _body_8x1.iToOSwitch[row].sw & (0x1 << col);
    case CFG_TYPE_MIXER_8X2:
        if(x) {
            return _body_8x2.iToOSwitch[z].swB & (0x1 << col);
        }else {
            return _body_8x2.iToOSwitch[z].swA & (0x1 << col);
        }
    case CFG_TYPE_MIXER_8X4:
        if(x) {
            return _body_8x4.iToOSwitch[z].swB & (0x1 << col);
        }else {
            return _body_8x4.iToOSwitch[z].swA & (0x1 << col);
        }
    case CFG_TYPE_MIXER_8X8:
        if(x) {
            return _body_8x8.iToOSwitch[z].swB & (0x1 << col);
        }else {
            return _body_8x8.iToOSwitch[z].swA & (0x1 << col);
        }
    case CFG_TYPE_MIXER_16X16:
        if(x) {
            return _body_16x16.iToOSwitch[z].swB & (0x1 << col);
        }else {
            return _body_16x16.iToOSwitch[z].swA & (0x1 << col);
        }
    case CFG_TYPE_MIXER_32X32:
        return _nodeSw[row][NODE_SW_L32] & (0x1 << col);
    case CFG_TYPE_MIXER_64X64:
        if (col < 32) {
            return _nodeSw[row][NODE_SW_L32] & (0x1 << col);
        } else {
            return _nodeSw[row][NODE_SW_H32] & (0x1 << (col - 32));
        }
    default:
        Q_ASSERT(false);
        break;
    }
}

QVariant ConfigItemMixer::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    switch(param->SetCmdKey())
    {
    case 0x00040002:
    {
        GainMute data = getInGainMute(param->GetExData1());
        setInGainMute(param->GetExData1(), data.gain, val.toBool());
        return QVariant(val.toBool());
    }
    case 0x00040004:
    {
        GainMute data = getInGainMute(param->GetExData1());
        setInGainMute(param->GetExData1(), val.toDouble(), data.mute);
        return QVariant(val.toDouble());
    }
    case 0x00040006:
    {
        GainMute data = getOutGainMute(param->GetExData1());
        setOutGainMute(param->GetExData1(), data.gain, val.toBool());
        return QVariant(val.toBool());
    }
    case 0x00040008:
    {
        GainMute data = getOutGainMute(param->GetExData1());
        setOutGainMute(param->GetExData1(), val.toDouble(), data.mute);
        return QVariant(val.toDouble());
    }
    case 0x0004000A:
        setRouterInfo(param->GetExData1(),param->GetExData2(), val.toBool());
        return QVariant(val.toBool());
    case 0x0004000C:
        setRouterGain(param->GetExData1(),param->GetExData2(), val.toDouble());
        return QVariant(val.toDouble());
    default:
        return QVariant();
    }
}

qint8 ConfigItemMixer::getRouterGain(int row, int col)
{
//    qDebug()<<__FUNCTION__<<__LINE__<<"row:"<<row<<" "<<"col:"<<col;//行列
    int z = col / 2;
    int x = col % 2;
    switch(_cfgType) {
    case CFG_TYPE_MIXER_4X1:
        if (x)
        {
            return _body_4x1.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _body_4x1.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_4X2:
        if (x)
        {
            return _body_4x2.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _body_4x2.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_4X4:
        if (x)
        {
            return _body_4x4.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _body_4x4.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_8X1:
        if (x)
        {
            return _body_8x1.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _body_8x1.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_8X2:
        if (x)
        {
            return _body_8x2.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _body_8x2.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_8X4:
        if (x)
        {
            return _body_8x4.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _body_8x4.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_8X8:
        if (x)
        {
            return _body_8x8.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _body_8x8.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_16X16:
        if (x)
        {
            return _body_16x16.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _body_16x16.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_32X32:
        return (_nodeGain[row][col] * 0.01 - 100);
    case CFG_TYPE_MIXER_64X64:
        return (_nodeGain[row][col] * 0.01 - 100);
    default:
        Q_ASSERT(false);
        break;
    }

    return 0;
}

GainMute ConfigItemMixer::getInGainMute(int idx)
{
    GainMute val;

    int z = idx / 2;
    int x = idx % 2;

    switch(_cfgType) {
    case CFG_TYPE_MIXER_4X1:
        if (x)
        {
            val.gain = _body_4x1.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_4x1.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_4x1.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_4X2:
        if (x)
        {
            val.gain = _body_4x2.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_4x2.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_4x2.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_4X4:
        if (x)
        {
            val.gain = _body_4x4.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_4x4.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_4x4.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X1:
        if (x)
        {
            val.gain = _body_8x1.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_8x1.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_8x1.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X2:
        if (x)
        {
            val.gain = _body_8x2.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_8x2.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_8x2.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X4:
        if (x)
        {
            val.gain = _body_8x4.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_8x4.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_8x4.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X8:
        if (x)
        {
            val.gain = _body_8x8.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_8x8.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_8x8.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_16X16:
        if (x)
        {
            val.gain = _body_16x16.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_16x16.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_16x16.inSwitch >> idx) & 0x01;
        //qDebug()<<__FUNCTION__<<__LINE__<<((_body_16x16.inSwitch >> idx) & 0x01);
        break;
    case CFG_TYPE_MIXER_32X32:
        val.gain = _inAndOutGain[idx][IN_SELECT] * 0.01 - 100;
        val.mute = (chSwIn[NODE_SW_L32] >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_64X64:
        val.gain = _inAndOutGain[idx][IN_SELECT] * 0.01 - 100;
        if (idx < 32)
        {
            val.mute = (chSwIn[NODE_SW_L32] >> idx) & 0x01;
        } else {
            val.mute = (chSwIn[NODE_SW_H32] >> (idx - 32)) & 0x01;
        }
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    return val;
}

GainMute ConfigItemMixer::getOutGainMute(int idx)
{
    GainMute val;

    int z = idx / 2;
    int x = idx % 2;

    switch(_cfgType) {
    case CFG_TYPE_MIXER_4X1:
        val.gain = _body_4x1.outgain[idx].gain * 0.01 - 100;
        val.mute = (_body_4x1.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_4X2:
        if (x)
        {
            val.gain = _body_4x2.outgain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_4x2.outgain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_4x2.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_4X4:
        if (x)
        {
            val.gain = _body_4x4.outgain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_4x4.outgain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_4x4.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X1:
        val.gain = _body_8x1.outgain[idx].gain * 0.01 - 100;
        val.mute = (_body_8x1.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X2:
        if (x)
        {
            val.gain = _body_8x2.outgain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_8x2.outgain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_8x2.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X4:
        if (x)
        {
            val.gain = _body_8x4.outgain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_8x4.outgain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_8x4.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X8:
        if (x)
        {
            val.gain = _body_8x8.outgain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_8x8.outgain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_8x8.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_16X16:
        if (x)
        {
            val.gain = _body_16x16.outgain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _body_16x16.outgain[z].gainA * 0.01 - 100;
        }
        val.mute = (_body_16x16.outSwitch >> idx) & 0x01;
//        qDebug()<<__FUNCTION__<<__LINE__<<_body_16x16.outSwitch<<idx<<((_body_16x16.outSwitch >> idx) & 0x01);
//        qDebug() << __FUNCTION__ << __LINE__ << idx << val.mute;
        break;
    case CFG_TYPE_MIXER_32X32:
        val.gain = _inAndOutGain[idx][OUT_SELECT] * 0.01 - 100;
        val.mute = (chSwOut[NODE_SW_L32] >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_64X64:
        val.gain = _inAndOutGain[idx][OUT_SELECT] * 0.01 - 100;
        if (idx < 32)
        {
            val.mute = (chSwOut[NODE_SW_L32] >> idx) & 0x01;
        } else {
            val.mute = (chSwOut[NODE_SW_H32] >> (idx - 32)) & 0x01;
        }
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    return val;
}

void ConfigItemMixer::setRouterInfo(int idx, quint32 vlu, quint32 vlu_h)
{
    int z = idx / 2;
    int x = idx % 2;
//    qDebug() << __FUNCTION__ << __LINE__ << idx << vlu << vlu_h;
    switch(_cfgType) {
    case CFG_TYPE_MIXER_4X1:
        _body_4x1.iToOSwitch[idx].sw =  vlu;
        break;
    case CFG_TYPE_MIXER_4X2:
        if (x)
        {
            _body_4x2.iToOSwitch[z].swB = vlu;
        } else {
            _body_4x2.iToOSwitch[z].swA = vlu;
        }
        break;
    case CFG_TYPE_MIXER_4X4:
        if (x)
        {
            _body_4x4.iToOSwitch[z].swB = vlu;
        } else {
            _body_4x4.iToOSwitch[z].swA = vlu;
        }
        break;
    case CFG_TYPE_MIXER_8X1:
        _body_8x1.iToOSwitch[idx].sw = vlu;
        break;
    case CFG_TYPE_MIXER_8X2:
        if (x)
        {
            _body_8x2.iToOSwitch[z].swB = vlu;
        } else {
            _body_8x2.iToOSwitch[z].swA = vlu;
        }
        break;
    case CFG_TYPE_MIXER_8X4:
        if (x)
        {
            _body_8x4.iToOSwitch[z].swB = vlu;
        } else {
            _body_8x4.iToOSwitch[z].swA = vlu;
        }
        break;
    case CFG_TYPE_MIXER_8X8:
        if (x)
        {
            _body_8x8.iToOSwitch[z].swB = vlu;
        } else {
            _body_8x8.iToOSwitch[z].swA = vlu;
        }
        break;
    case CFG_TYPE_MIXER_16X16:
        if (x)
        {
            _body_16x16.iToOSwitch[z].swB = vlu;
        } else {
            _body_16x16.iToOSwitch[z].swA = vlu;
        }
        break;
    case CFG_TYPE_MIXER_32X32:
        _nodeSw[idx][NODE_SW_L32] = vlu;
        break;
    case CFG_TYPE_MIXER_64X64:
        _nodeSw[(idx)][NODE_SW_L32] = vlu;
        _nodeSw[(idx)][NODE_SW_H32] = vlu_h;
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    saveParamToDevice();
}

void ConfigItemMixer::setRouterInfo(int row, int col, bool isChacked)
{
    int z = row / 2;
    int x = row % 2;

//    qDebug() << __FUNCTION__ << __LINE__ << row << col << isChacked;

    switch (_cfgType) {
    case CFG_TYPE_MIXER_4X1:
        if (isChacked)
        {
            _body_4x1.iToOSwitch[row].sw |=  (0x1 << col);
        } else {
            _body_4x1.iToOSwitch[row].sw &=  ~(0x1 << col);
        }
        break;
    case CFG_TYPE_MIXER_4X2:
        if(x)
        {
            if(isChacked)
            {
                _body_4x2.iToOSwitch[z].swB |= (0x1 << col);
            }else{
                _body_4x2.iToOSwitch[z].swB &= ~(0x1 << col);
            }
        }else {
            if(isChacked)
            {
                _body_4x2.iToOSwitch[z].swA |= (0x1 << col);
            }else{
                _body_4x2.iToOSwitch[z].swA &= ~(0x1 << col);
            }
        }
        break;
     case CFG_TYPE_MIXER_4X4:
        if(x)
        {
            if(isChacked)
            {
                _body_4x4.iToOSwitch[z].swB |= (0x1 << col);
            }else{
                _body_4x4.iToOSwitch[z].swB &= ~(0x1 << col);
            }
        }else {
            if(isChacked)
            {
                _body_4x4.iToOSwitch[z].swA |= (0x1 << col);
            }else{
                _body_4x4.iToOSwitch[z].swA &= ~(0x1 << col);
            }
        }
        break;
    case CFG_TYPE_MIXER_8X1:
        if(isChacked)
        {
            _body_8x1.iToOSwitch[row].sw |= (0x1 << col);
        }else {
            _body_8x1.iToOSwitch[row].sw &= ~(0x1 << col);
        }
        break;
    case CFG_TYPE_MIXER_8X2:
        if(x)
        {
            if(isChacked)
            {
                _body_8x2.iToOSwitch[z].swB |= (0x1 << col);
            }else {
                _body_8x2.iToOSwitch[z].swB &= ~(0x1 << col);
            }
        }else {
            if(isChacked)
            {
                _body_8x2.iToOSwitch[z].swA |= (0x1 << col);
            }else {
                _body_8x2.iToOSwitch[z].swA &= ~(0x1 << col);
            }
        }
        break;
    case CFG_TYPE_MIXER_8X4:
        if(x)
        {
            if(isChacked)
            {
                _body_8x4.iToOSwitch[z].swB |= (0x1 << col);
            }else {
                _body_8x4.iToOSwitch[z].swB &= ~(0x1 << col);
            }
        }else {
            if(isChacked)
            {
                _body_8x4.iToOSwitch[z].swA |= (0x1 << col);
            }else {
                _body_8x4.iToOSwitch[z].swA &= ~(0x1 << col);
            }
        }
        break;
    case CFG_TYPE_MIXER_8X8:
        if(x)
        {
            if(isChacked)
            {
                _body_8x8.iToOSwitch[z].swB |= (0x1 << col);
            }else {
                _body_8x8.iToOSwitch[z].swB &= ~(0x1 << col);
            }
        }else {
            if(isChacked)
            {
                _body_8x8.iToOSwitch[z].swA |= (0x1 << col);
            }else {
                _body_8x8.iToOSwitch[z].swA &= ~(0x1 << col);
            }
        }
        break;
    case CFG_TYPE_MIXER_16X16:
        if(x)
        {
            if(isChacked)
            {
                _body_16x16.iToOSwitch[z].swB |= (0x1 << col);
            }else {
                _body_16x16.iToOSwitch[z].swB &= ~(0x1 << col);
            }
        }else {
            if(isChacked)
            {
                _body_16x16.iToOSwitch[z].swA |= (0x1 << col);
            }else {
                _body_16x16.iToOSwitch[z].swA &= ~(0x1 << col);
            }
        }
        break;
    case CFG_TYPE_MIXER_32X32:
        if(isChacked)
        {
            _nodeSw[row][NODE_SW_L32] |= (0x1 << col);
        }else {
            _nodeSw[row][NODE_SW_L32] &= ~(0x1 << col);
        }

        prepare32BodyData(row, ROW_SELECT);
        break;
    case CFG_TYPE_MIXER_64X64:
//        qDebug() << __FUNCTION__ << __LINE__
//                 << col;
        if (col < 32)
        {
            if(isChacked)
            {
                _nodeSw[row][NODE_SW_L32] |= (0x1 << col);
            }else {
                _nodeSw[row][NODE_SW_L32] &= ~(0x1 << col);
            }
        } else {
            if(isChacked)
            {
                _nodeSw[row][NODE_SW_H32] |= (0x1 << (col - 32));
            }else {
                _nodeSw[row][NODE_SW_H32] &= ~(0x1 << (col - 32));
            }
        }

        prepare64BodyData(row, ROW_SELECT);
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    saveParamToDevice();
}

void ConfigItemMixer::setRouterGain(int row, int col, qint8 value)
{
    int z = col / 2;
    int x = col % 2;

    switch(_cfgType) {
    case CFG_TYPE_MIXER_4X1:
        if (x)
        {
            _body_4x1.iToOGain[row][z].gainB = (value + 100) * 100;
        } else {
            _body_4x1.iToOGain[row][z].gainA = (value + 100) * 100;
        }
        break;
    case CFG_TYPE_MIXER_4X2:
        if (x)
        {
            _body_4x2.iToOGain[row][z].gainB = (value + 100) * 100;
        } else {
            _body_4x2.iToOGain[row][z].gainA = (value + 100) * 100;
        }
        break;
    case CFG_TYPE_MIXER_4X4:
        if (x)
        {
            _body_4x4.iToOGain[row][z].gainB = (value + 100) * 100;
        } else {
            _body_4x4.iToOGain[row][z].gainA = (value + 100) * 100;
        }
        break;
    case CFG_TYPE_MIXER_8X1:
        if (x)
        {
            _body_8x1.iToOGain[row][z].gainB = (value + 100) * 100;
        } else {
            _body_8x1.iToOGain[row][z].gainA = (value + 100) * 100;
        }
        break;
    case CFG_TYPE_MIXER_8X2:
        if (x)
        {
            _body_8x2.iToOGain[row][z].gainB = (value + 100) * 100;
        } else {
            _body_8x2.iToOGain[row][z].gainA = (value + 100) * 100;
        }
        break;
    case CFG_TYPE_MIXER_8X4:
        if (x)
        {
            _body_8x4.iToOGain[row][z].gainB = (value + 100) * 100;
        } else {
            _body_8x4.iToOGain[row][z].gainA = (value + 100) * 100;
        }
        break;
    case CFG_TYPE_MIXER_8X8:
        if (x)
        {
            _body_8x8.iToOGain[row][z].gainB = (value + 100) * 100;
        } else {
            _body_8x8.iToOGain[row][z].gainA = (value + 100) * 100;
        }
        break;
    case CFG_TYPE_MIXER_16X16:
        if (x)
        {
            _body_16x16.iToOGain[row][z].gainB = (value + 100) * 100;
        } else {
            _body_16x16.iToOGain[row][z].gainA = (value + 100) * 100;
        }
        break;
    case CFG_TYPE_MIXER_32X32:
        _nodeGain[row][col] = (value + 100) * 100;
        prepare32BodyData(row, ROW_SELECT);
        break;
    case CFG_TYPE_MIXER_64X64:
        _nodeGain[row][col] = (value + 100) * 100;
//        qDebug() << __FUNCTION__ << __LINE__ << _nodeGain[row][col];
        prepare64BodyData(row, ROW_SELECT);
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    saveParamToDevice();
}

void ConfigItemMixer::setInGainMute(int idx, double gain, bool mute)
{
    int z = idx / 2;
    int x = idx % 2;

    switch(_cfgType) {
    case CFG_TYPE_MIXER_4X1:
        if (x)
        {
            _body_4x1.ingain[z].gainB = (gain + 100) * 100;
        } else {
            _body_4x1.ingain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_4x1.inSwitch |= (1 << idx);
        } else {
            _body_4x1.inSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_4X2:
        if (x)
        {
            _body_4x2.ingain[z].gainB = (gain + 100) * 100;
        } else {
            _body_4x2.ingain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_4x2.inSwitch |= (1 << idx);
        } else {
            _body_4x2.inSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_4X4:
        if (x)
        {
            _body_4x4.ingain[z].gainB = (gain + 100) * 100;
        } else {
            _body_4x4.ingain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_4x4.inSwitch |= (1 << idx);
        } else {
            _body_4x4.inSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_8X1:
        if (x)
        {
            _body_8x1.ingain[z].gainB = (gain + 100) * 100;
        } else {
            _body_8x1.ingain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_8x1.inSwitch |= (1 << idx);
        } else {
            _body_8x1.inSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_8X2:
        if (x)
        {
            _body_8x2.ingain[z].gainB = (gain + 100) * 100;
        } else {
            _body_8x2.ingain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_8x2.inSwitch |= (1 << idx);
        } else {
            _body_8x2.inSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_8X4:
        if (x)
        {
            _body_8x4.ingain[z].gainB = (gain + 100) * 100;
        } else {
            _body_8x4.ingain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_8x4.inSwitch |= (1 << idx);
        } else {
            _body_8x4.inSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_8X8:
        if (x)
        {
            _body_8x8.ingain[z].gainB = (gain + 100) * 100;
        } else {
            _body_8x8.ingain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_8x8.inSwitch |= (1 << idx);
        } else {
            _body_8x8.inSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_16X16:
        if (x)
        {
            _body_16x16.ingain[z].gainB = (gain + 100) * 100;
        } else {
            _body_16x16.ingain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_16x16.inSwitch |= (1 << idx);
        } else {
            _body_16x16.inSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_32X32:
        _inAndOutGain[idx][IN_SELECT] = (gain + 100) * 100;
        if (mute)
        {
            chSwIn[NODE_SW_L32] |= (1 << idx);
        } else {
            chSwIn[NODE_SW_L32] &= ~(1 << idx);
        }
        prepare32BodyData(idx, COL_SELECT);
        break;
    case CFG_TYPE_MIXER_64X64:
        _inAndOutGain[idx][IN_SELECT] = (gain + 100) * 100;
        if (idx < 32)
        {
            if (mute)
            {
                chSwIn[NODE_SW_L32] |= (1 << idx);
            } else {
                chSwIn[NODE_SW_L32] &= ~(1 << idx);
            }
        } else {
            if (mute)
            {
                chSwIn[NODE_SW_H32] |= (1 << (idx-32));
            } else {
                chSwIn[NODE_SW_H32] &= ~(1 << (idx-32));
            }
        }
        prepare64BodyData(idx, COL_SELECT);
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    saveParamToDevice();
}

void ConfigItemMixer::setOutGainMute(int idx, double gain, bool mute)
{
    int z = idx / 2;
    int x = idx % 2;

    switch(_cfgType) {
    case CFG_TYPE_MIXER_4X1:
        _body_4x1.outgain[idx].gain = (gain + 100) * 100;
        if (mute)
        {
            _body_4x1.outSwitch |= (1 << idx);
        } else {
            _body_4x1.outSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_4X2:
        if (x)
        {
            _body_4x2.outgain[z].gainB = (gain + 100) * 100;
        } else {
            _body_4x2.outgain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_4x2.outSwitch |= (1 << idx);
        } else {
            _body_4x2.outSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_4X4:
        if (x)
        {
            _body_4x4.outgain[z].gainB = (gain + 100) * 100;
        } else {
            _body_4x4.outgain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_4x4.outSwitch |= (1 << idx);
        } else {
            _body_4x4.outSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_8X1:
        _body_8x1.outgain[idx].gain = (gain + 100) * 100;
        if (mute)
        {
            _body_8x1.outSwitch |= (1 << idx);
        } else {
            _body_8x1.outSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_8X2:
        if (x)
        {
            _body_8x2.outgain[z].gainB = (gain + 100) * 100;
        } else {
            _body_8x2.outgain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_8x2.outSwitch |= (1 << idx);
        } else {
            _body_8x2.outSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_8X4:
        if (x)
        {
            _body_8x4.outgain[z].gainB = (gain + 100) * 100;
        } else {
            _body_8x4.outgain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_8x4.outSwitch |= (1 << idx);
        } else {
            _body_8x4.outSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_8X8:
        if (x)
        {
            _body_8x8.outgain[z].gainB = (gain + 100) * 100;
        } else {
            _body_8x8.outgain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_8x8.outSwitch |= (1 << idx);
        } else {
            _body_8x8.outSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_16X16:
        if (x)
        {
            _body_16x16.outgain[z].gainB = (gain + 100) * 100;
        } else {
            _body_16x16.outgain[z].gainA = (gain + 100) * 100;
        }
        if (mute)
        {
            _body_16x16.outSwitch |= (1 << idx);
        } else {
            _body_16x16.outSwitch &= ~(1 << idx);
        }
        break;
    case CFG_TYPE_MIXER_32X32:
       _inAndOutGain[idx][OUT_SELECT] = (gain + 100) * 100;
       if (mute)
       {
           chSwOut[NODE_SW_L32] |= (1 << idx);
       } else {
           chSwOut[NODE_SW_L32] &= ~(1 << idx);
       }
       prepare32BodyData(idx, ROW_SELECT);
       break;
    case CFG_TYPE_MIXER_64X64:
       _inAndOutGain[idx][OUT_SELECT] = (gain + 100) * 100;
       if (idx < 32)
       {
           if (mute)
           {
               chSwOut[NODE_SW_L32] |= (1 << idx);
           } else {
               chSwOut[NODE_SW_L32] &= ~(1 << idx);
           }
       } else {
           if (mute)
           {
               chSwOut[NODE_SW_H32] |= (1 << (idx-32));
           } else {
               chSwOut[NODE_SW_H32] &= ~(1 << (idx-32));
           }
       }
       prepare64BodyData(idx, ROW_SELECT);
       break;
    default:
        Q_ASSERT(false);
        break;
    }

    saveParamToDevice();
}

void ConfigItemMixer::createLabelTextList()
{
    for (int i = 0; i < 64; ++i) {
        _inLabel.append(QString("In %1").arg(i+1));
        _outLabel.append(QString("Out %1").arg(i+1));
    }
}

QString ConfigItemMixer::getLabText(int bOutput, int idx)
{
    return bOutput ? _outLabel.at(idx) : _inLabel.at(idx);
}

void ConfigItemMixer::setLabText(int bOutput, int idx, QString text)
{
    if (bOutput) {
        _outLabel.replace(idx, text);
        if (idx < _outPortList.count()) {
            _outPortList.at(idx)->setPortName(text);
        }
    }
    else {
        _inLabel.replace(idx, text);
        if (idx < _inPortList.count()) {
            _inPortList.at(idx)->setPortName(text);
        }
    }
}

void ConfigItemMixer::nodesSwitchChanged_slot()
{
//    qDebug() << __FUNCTION__ << __LINE__ << _NodesSwitch;
    if(_setting) {
        _setting->setNodesSwitch();
    }
    //Kemu没用nodesSwitch功能
}

void ConfigItemMixer::prepare32BodyData(int idx, int order)
{
    memset(&_body_32x32, 0, sizeof(IG_PARACTL_VMIXER32X32));

    switch (order) {
    case ROW_SELECT:
        _body_32x32.cfgChInfo.chNo = idx;
        _body_32x32.cfgChInfo.chType = order;
        _body_32x32.cfgChInfo.chSw = (chSwOut[NODE_SW_L32] >> idx) & 0x01;
        _body_32x32.cfgChInfo.chGain = _inAndOutGain[idx][OUT_SELECT];
        _body_32x32.miso.misoSwitch[0] = _nodeSw[idx][NODE_SW_L32];
        for (int i = 0; i < 16; ++i)
        {
            _body_32x32.miso.misoGain[i].gainA = _nodeGain[idx][2*i];
            _body_32x32.miso.misoGain[i].gainB = _nodeGain[idx][2*i+1];
        }
        break;
    case COL_SELECT:
        _body_32x32.cfgChInfo.chNo = idx;
        _body_32x32.cfgChInfo.chType = order;
        _body_32x32.cfgChInfo.chSw = (chSwIn[NODE_SW_L32] >> idx) & 0x01;
        _body_32x32.cfgChInfo.chGain = _inAndOutGain[idx][IN_SELECT];
        _body_32x32.simo.simoSwitch[0] = 0;
        for(int i = 0; i < 32; i++)
        {
            _body_32x32.simo.simoSwitch[0] |= (((_nodeSw[i][NODE_SW_L32] >> idx) & 0x01) << i);
        }
        for (int i = 0; i < 16; ++i)
        {
            _body_32x32.simo.simoGain[i].gainA = _nodeGain[2*i][idx];
            _body_32x32.simo.simoGain[i].gainB = _nodeGain[2*i+1][idx];
        }
        break;
    default:
        break;
    }
}

void ConfigItemMixer::prepare64BodyData(int idx, int order)
{
    memset(&_body_64x64, 0, sizeof(IG_PARACTL_VMIXER64X64));

    switch (order) {
    case ROW_SELECT:
        _body_64x64.cfgChInfo.chNo = idx;
        _body_64x64.cfgChInfo.chType = order;
        if (idx < 32)
        {
            _body_64x64.cfgChInfo.chSw = (chSwOut[NODE_SW_L32] >> idx) & 0x01;
        } else {
            _body_64x64.cfgChInfo.chSw = (chSwOut[NODE_SW_H32] >> (idx-32)) & 0x01;
        }
        _body_64x64.cfgChInfo.chGain = _inAndOutGain[idx][OUT_SELECT];
        _body_64x64.miso.misoSwitch[0] = _nodeSw[idx][NODE_SW_L32];
        _body_64x64.miso.misoSwitch[1] = _nodeSw[idx][NODE_SW_H32];
        for (int i = 0; i < 32; ++i)
        {
            _body_64x64.miso.misoGain[i].gainA = _nodeGain[idx][2*i];
            _body_64x64.miso.misoGain[i].gainB = _nodeGain[idx][2*i+1];
        }
//        qDebug() << __FUNCTION__ << __LINE__
//                 << idx << _body_64x64.miso.misoSwitch[0] << _body_64x64.miso.misoSwitch[1]
//                 << _body_64x64.miso.misoGain[0].gainA
//                 << _body_64x64.miso.misoGain[0].gainB;
        break;
    case COL_SELECT:
        _body_64x64.cfgChInfo.chNo = idx;
        _body_64x64.cfgChInfo.chType = order;

        _body_64x64.cfgChInfo.chGain = _inAndOutGain[idx][IN_SELECT];
        _body_64x64.simo.simoSwitch[0] = 0;
        _body_64x64.simo.simoSwitch[1] = 0;

        if (idx < 32)
        {
            _body_64x64.cfgChInfo.chSw = (chSwIn[NODE_SW_L32] >> idx) & 0x01;
            for(int i = 0; i < 32; i++)
            {
                _body_64x64.simo.simoSwitch[0] |= (((_nodeSw[i][NODE_SW_L32] >> idx) & 0x01) << i);
                _body_64x64.simo.simoSwitch[1] |= (((_nodeSw[32+i][NODE_SW_L32] >> idx) & 0x01) << i);
            }
        } else {
            _body_64x64.cfgChInfo.chSw = (chSwIn[NODE_SW_H32] >> (idx-32)) & 0x01;
            for(int i = 0; i < 32; i++)
            {
                _body_64x64.simo.simoSwitch[0] |= (((_nodeSw[i][NODE_SW_H32] >> (idx-32)) & 0x01) << i);
                _body_64x64.simo.simoSwitch[1] |= (((_nodeSw[32+i][NODE_SW_H32] >> (idx-32)) & 0x01) << i);
            }
        }

        for (int i = 0; i < 32; ++i)
        {
            _body_64x64.simo.simoGain[i].gainA = _nodeGain[2*i][idx];
            _body_64x64.simo.simoGain[i].gainB = _nodeGain[2*i+1][idx];
        }
//        qDebug() << __FUNCTION__ << __LINE__
//                 << idx << _body_64x64.simo.simoSwitch[0] << _body_64x64.simo.simoSwitch[1]
//                 << _body_64x64.simo.simoGain[0].gainA
//                 << _body_64x64.simo.simoGain[0].gainB;
        break;
    default:
        break;
    }
}

void ConfigItemMixer::setDev_32_Data()
{
    int idx = _body_32x32.cfgChInfo.chNo;
    int order = _body_32x32.cfgChInfo.chType;
    switch (order) {
    case ROW_SELECT:
        if (_body_32x32.cfgChInfo.chSw)
        {
            chSwOut[NODE_SW_L32] |= (1 << idx);
        } else {
            chSwOut[NODE_SW_L32] &= ~(1 << idx);
        }
        _inAndOutGain[idx][OUT_SELECT] = _body_32x32.cfgChInfo.chGain;
        _nodeSw[idx][NODE_SW_L32] = _body_32x32.miso.misoSwitch[0];
        for(int i = 0; i < 16; ++i)
        {
            _nodeGain[idx][2*i]   = _body_32x32.miso.misoGain[i].gainA;
            _nodeGain[idx][2*i+1] = _body_32x32.miso.misoGain[i].gainB;
        }
        break;
    case COL_SELECT:
        if(_body_32x32.cfgChInfo.chSw)
        {
            chSwIn[NODE_SW_L32] |= (1 << idx);
        }else {
            chSwIn[NODE_SW_L32] &= ~(1 << idx);
        }
        _inAndOutGain[idx][IN_SELECT] = _body_32x32.cfgChInfo.chGain;
        for(int i = 0; i < 32; i++)
        {
            _nodeSw[i][NODE_SW_L32] |= ((_body_32x32.simo.simoSwitch[0] << idx) & 0x01);
        }
        for(int i = 0; i < 16; ++i)
        {
            _nodeGain[2*i][idx]   = _body_32x32.simo.simoGain[i].gainA;
            _nodeGain[2*i+1][idx] = _body_32x32.simo.simoGain[i].gainB;
        }
        break;
    default:
        break;
    }


}

void ConfigItemMixer::setDev_64_Data()
{
    int idx = _body_64x64.cfgChInfo.chNo;
    int order = _body_64x64.cfgChInfo.chType;
    switch (order) {
    case ROW_SELECT:
        if(idx < 32)
        {
            if(_body_64x64.cfgChInfo.chSw)
            {
                chSwOut[NODE_SW_L32] |= (1 << idx);
            }else {
                chSwOut[NODE_SW_L32] &= ~(1 << idx);
            }
        }else {
            if(_body_64x64.cfgChInfo.chSw)
            {
                chSwOut[NODE_SW_H32] |= (1 << (idx - 32));
            }else {
                chSwOut[NODE_SW_H32] &= ~(1 << (idx - 32));
            }
        }
        _inAndOutGain[idx][OUT_SELECT] = _body_64x64.cfgChInfo.chGain;
        _nodeSw[idx][NODE_SW_L32] = _body_64x64.miso.misoSwitch[0];
        _nodeSw[idx][NODE_SW_H32] = _body_64x64.miso.misoSwitch[1];
        for(int i = 0; i < 32; ++i)
        {
            _nodeGain[idx][2*i]   = _body_64x64.miso.misoGain[i].gainA;
            _nodeGain[idx][2*i+1] = _body_64x64.miso.misoGain[i].gainB;
        }
        break;
    case COL_SELECT:
        if(idx < 32)
        {
            if(_body_64x64.cfgChInfo.chSw)
            {
                chSwIn[NODE_SW_L32] |= (1 << idx);
            }else {
                chSwIn[NODE_SW_L32] &= ~(1 << idx);
            }
            for(int i = 0; i < 32; i++)
            {
                _nodeSw[i][NODE_SW_L32]    |= (_body_64x64.simo.simoSwitch[0] << idx);
                _nodeSw[32+i][NODE_SW_L32] |= (_body_64x64.simo.simoSwitch[1] << idx);
            }
        }else {
            if(_body_64x64.cfgChInfo.chSw)
            {
                chSwIn[NODE_SW_H32] |= (1 << (idx - 32));
            }else {
                chSwIn[NODE_SW_H32] &= ~(1 << (idx - 32));
            }
            for(int i = 0; i < 32; i++)
            {
                _nodeSw[i][NODE_SW_H32]    |= (_body_64x64.simo.simoSwitch[0] << (idx - 32));
                _nodeSw[32+i][NODE_SW_H32] |= (_body_64x64.simo.simoSwitch[1] << (idx - 32));
            }
        }
        _inAndOutGain[idx][IN_SELECT] = _body_64x64.cfgChInfo.chGain;
        for(int i = 0; i < 32; i++)
        {
            _nodeGain[2*i][idx]   = _body_64x64.simo.simoGain[i].gainA;
            _nodeGain[2*i+1][idx] = _body_64x64.simo.simoGain[i].gainB;
        }
        break;
    default:
        break;
    }
}

quint16 ConfigItemMixer::getInNum()
{
    switch(_cfgType) {
    case CFG_TYPE_MIXER_4X1:
    case CFG_TYPE_MIXER_4X2:
    case CFG_TYPE_MIXER_4X4:
        return 4;
    case CFG_TYPE_MIXER_8X1:
    case CFG_TYPE_MIXER_8X2:
    case CFG_TYPE_MIXER_8X4:
    case CFG_TYPE_MIXER_8X8:
        return 8;
    case CFG_TYPE_MIXER_16X16:
        return 16;
    case CFG_TYPE_MIXER_32X32:
        return 32;
    case CFG_TYPE_MIXER_64X64:
        return 64;
    default:
        Q_ASSERT(false);
        break;
    }

    return 0;
}

quint16 ConfigItemMixer::getOutNum()
{
    switch(_cfgType) {
    case CFG_TYPE_MIXER_4X1:
    case CFG_TYPE_MIXER_8X1:
        return 1;
    case CFG_TYPE_MIXER_4X2:
    case CFG_TYPE_MIXER_8X2:
        return 2;
    case CFG_TYPE_MIXER_4X4:
    case CFG_TYPE_MIXER_8X4:
        return 4;
    case CFG_TYPE_MIXER_8X8:
        return 8;
    case CFG_TYPE_MIXER_16X16:
        return 16;
    case CFG_TYPE_MIXER_32X32:
        return 32;
    case CFG_TYPE_MIXER_64X64:
        return 64;
    default:
        Q_ASSERT(false);
        break;
    }

    return 0;
}
