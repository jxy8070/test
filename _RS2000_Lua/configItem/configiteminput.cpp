#include "configiteminput.h"
#include "configSetting/cfgsettinginput.h"
#include "Frame/configview.h"
#include "deviceItem/deviceItem.h"
#include "device/device_trn.h"
#include "ItemPort.h"
//#include "utils.h"

ConfigItemInput::ConfigItemInput(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    memset(&_body1, 0, sizeof(IG_PARACTL_INPUT1));
    memset(&_bodyNew, 0, sizeof(IG_PARACTL_INPUT_NEW));
    addSubType(_cfgType);
    _max_port = 0x0001 << (_cfgType & 0x00FF);  //根据输入模块类型获取最大端口数1，2，4，8，16
    switch (_cfgType) {
    case CFG_TYPE_INPUT_1:
        _body1.mute = 0;
        _body1.antiPhase = 0;
        _body1.gain = (0 + 100) * 100;
        break;
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        _bodyNew.mute = 0;
        _bodyNew.antiPhase = 0;
        for (int i = 0; i < _max_port/2; ++i) {
            _bodyNew.gain[i].gainA = (0 + 100) * 100;
            _bodyNew.gain[i].gainB = (0 + 100) * 100;
        }
        break;
    default:
        break;
    }

    QStringList tmp;
    for (int i = 0; i < _max_port; ++i) {
        _userLabel.append(QString("Input %1").arg(i + 1));
    }

    initPort(0, _max_port, tmp, _userLabel);

    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) {
//        foreach(ItemPort *port, _inPortList)
//            port->setEnableMute(true);
        foreach(ItemPort *port, _outPortList)
            port->setEnableMute(true);
    }
    //输入输出模块由于chNo在initChannel中确定，Lua控制针脚的初始化也移动到initChannel()
}

ConfigItemInput::~ConfigItemInput()
{
    if (_setting != NULL) {
        delete _setting;
        _setting = NULL;
    }
}


void ConfigItemInput::initRealChannel(DevPortType portType, int realCh, int logicCh)
{
    _portType = portType;
    _realCh = realCh;
    _logicCh = logicCh;

    QString cftTypeName = Utils::getCfgPortModelName(true, _portType, logicCh, _max_port);
    setcfgTypeName(cftTypeName);
    for(int i = 0; i< _max_port; i++){
        QString portName = Utils::getCfgPortChannelName(_portType, _logicCh+i, tr("In"));
        _userLabel[i] = portName;
        _outPortList[i]->setPortName(portName);
    }
    QColor itemColor(242,155,118);
    if(_portType == PortType_AES67) itemColor = QColor(246,179,127);

    setFillColor(itemColor);

    QList<CLuaPinParam_PC*> pinList;
    for(int i = 0; i<_max_port; i++)
    {
        int chNoOffset = i;
        QString groupName = QString("In%1").arg(chNoOffset);
        pinList.append(new CLuaPinParam_PC(this, _customName, "AntiPhase", 0x00000001, 0x00000002, chNoOffset, 0x00, groupName));
        pinList.append(new CLuaPinParam_PC(this, _customName, "Mute", 0x00000003, 0x00000004, chNoOffset, 0x00, groupName));
        pinList.append(new CLuaPinParam_PC(this, _customName, "Gain", 0x00000005, 0x00000006, chNoOffset, 0x00, groupName));
    }
    initControlPins(pinList);
}

QWidget *ConfigItemInput::getSettingForm()
{
    if(_proxyItem == NULL) _setting = NULL;
    if (_setting == NULL)
    {
        _setting = new CfgSettingInput(this);
    }
    return _setting;
}

QByteArray ConfigItemInput::getOverviewData(bool is16Bit)
{
    QByteArray sigFlowBuff;
    if(is16Bit){
        //格式化16bit真实通道
        IG_SIG_REAL_CH16 realChDef;
        int chGroup = (_max_port + 1) / 2;   //根据分组端口数格式化数据
        for(int i = 0; i<chGroup; i++ ){
            memset(&realChDef, 0, sizeof(IG_SIG_REAL_CH16));
            realChDef.channelA = _realCh +i*2;
            if(i*2 + 1 < _max_port) realChDef.channelB = _realCh+i*2+1;
            sigFlowBuff.append((const char*)&realChDef, sizeof(IG_SIG_REAL_CH16));
        }
        //格式化16bit信号流layer，row
        IG_SIGFLOW_SINGLE16 flowDef;
        int oDefCnt = _outputDefList.count();
        for(int i = 0; i<oDefCnt; i++)
        {
            memset(&flowDef, 0, sizeof(IG_SIGFLOW_SINGLE16));
            flowDef.column  = _outputDefList[i]->layer;
            flowDef.line    = _outputDefList[i]->row;
            sigFlowBuff.append((const char*)&flowDef, sizeof(IG_SIGFLOW_SINGLE16));
        }
    }
    else{
        //格式化8bit真实通道
        IG_SIG_REAL_CHANNEL realChDef;
        int chGroup = (_max_port + 3) / 4;   //根据分组端口数格式化数据
        for(int i = 0; i<chGroup; i++ ){
            memset(&realChDef, 0, sizeof(IG_SIG_REAL_CHANNEL));
            realChDef.channelA = _realCh+i*4;
            if(i*4 + 1 < _max_port) realChDef.channelB = _realCh+i*4+ 1;
            if(i*4 + 2 < _max_port) realChDef.channelC = _realCh+i*4+ 2;
            if(i*4 + 3 < _max_port) realChDef.channelD = _realCh+i*4+ 3;
            sigFlowBuff.append((const char*)&realChDef, sizeof(IG_SIG_REAL_CHANNEL));
        }

        //格式化8bit信号流layer，row
        IG_SIGFLOW8_DOUBLE flowDef;
        int oDefCnt = _outputDefList.count();
        for(int i = 0; i<oDefCnt; i++)
        {
            if(i%2 == 0){
                memset(&flowDef, 0, sizeof(IG_SIGFLOW8_DOUBLE));
                flowDef.columnA  = _outputDefList[i]->layer;
                flowDef.lineA    = _outputDefList[i]->row;
                if(i == oDefCnt -1){
                    sigFlowBuff.append((const char*)&flowDef, sizeof(IG_SIGFLOW8_DOUBLE)); //奇数端口数最后一个端口
                }
            }
            else{
                flowDef.columnB  = _outputDefList[i]->layer;
                flowDef.lineB    = _outputDefList[i]->row;
                sigFlowBuff.append((const char*)&flowDef, sizeof(IG_SIGFLOW8_DOUBLE));
            }
        }
    }
    return sigFlowBuff;
}

QByteArray ConfigItemInput::getDetailData()
{
    QByteArray paraBuff;
    switch (_cfgType) {
    case CFG_TYPE_INPUT_1:
        paraBuff.append((const char *)&_body1, sizeof(IG_PARACTL_INPUT1));
        break;
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        //根据模块类型确Detail包长度: (2+_max_port/2)个32位整数
        paraBuff.append((const char *)&_bodyNew, sizeof(unsigned int) * (2 + _max_port/2));
        break;
    default:
        break;
    }

    return paraBuff;
}

void ConfigItemInput::setDetailData(unsigned int *d)
{
    if (_cfgType == CFG_TYPE_INPUT_1) {
        memcpy(&_body1, d, sizeof(IG_PARACTL_INPUT1));
    }
    else{
        memcpy(&_bodyNew, d, sizeof(IG_PARACTL_INPUT_NEW));
    }
    if (_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemInput::setRtnData(unsigned int *d)
{
    quint8* pStartVU = (quint8*)d;

    if(getDevType() == Dev_Type_NAP_STM32H750_8A8N){
        //8*8设备电平表和16*16一样的总路数64路，后部分空置
        //对输入电平表处理方式没影响
        pStartVU += getRealChannel(0);
    }
    else{
        pStartVU += getRealChannel(0);
    }

    if(_proxyItem == NULL) _setting = NULL;
    if (_setting!= NULL && _setting->isVisible()) {
        _setting->setVuDisp(pStartVU);
    }
}

void ConfigItemInput::updateCfgSettingLoadedData()
{
    if (_setting != NULL && _setting->isVisible())
    {
        _setting->updateUiData();
    }
}

void ConfigItemInput::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_INPUT_GROUP));
    for (int i = 0; i < _max_port; ++i) {
        setUserLabel(i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_UserLabel).arg(i)).toString());
    }

    switch (_cfgType) {
    case CFG_TYPE_INPUT_1:
        setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
        setGain(0,       ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_GAIN).arg(0)).toDouble());
        setInvert(0,     ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_INVERT).arg(0)).toBool());
        setMute(0,       ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_MUTE).arg(0)).toBool());
        if(_portType == PortType_MONAURAL){
            setPhantom(0,    ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_Phantom).arg(0)).toBool());
            setPreampGain(0, ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_PreGain).arg(0)).toInt());
        }
        break;
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
        for (int i = 0; i < _max_port; ++i) {
            setGain(i,       ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_GAIN).arg(i)).toDouble());
            setInvert(i,     ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_INVERT).arg(i)).toBool());
            setMute(i,       ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_MUTE).arg(i)).toBool());
            if(_portType == PortType_MONAURAL){
                setPhantom(i,    ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_Phantom).arg(i)).toBool());
                setPreampGain(i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_PreGain).arg(i)).toInt());
            }
        }
        break;
    default:
        break;
    }
    ConfigIni->endGroup();
}


void ConfigItemInput::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_INPUT_GROUP));
    for (int i = 0; i < _max_port; ++i) {
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_UserLabel).arg(i), getUserLabel(i));
    }
    switch (_cfgType) {
    case CFG_TYPE_INPUT_1:
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO),            getModuleNum());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_CHANNEL),        getRealChannel(0));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_GAIN).arg(0),    getGain(0));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_INVERT).arg(0),  isInvert(0));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_MUTE).arg(0),    isMute(0));
        if(_portType == PortType_MONAURAL){
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_Phantom).arg(0), getPhantom(0));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_PreGain).arg(0), getPreampGain(0));
        }
        break;
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO),     getModuleNum());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_CHANNEL), getRealChannel(0));
        for (int i = 0; i < _max_port; ++i) {
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_GAIN).arg(i),    getGain(i));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_INVERT).arg(i),  isInvert(i));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_MUTE).arg(i),    isMute(i));
            if(_portType == PortType_MONAURAL){
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_Phantom).arg(i), getPhantom(i));
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_INPUT_PreGain).arg(i), getPreampGain(i));
            }
        }
        break;
    default:
        break;
    }
    ConfigIni->endGroup();
}

QVariant ConfigItemInput::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    switch(param->SetCmdKey())
    {
    case 0x00000002:
        setInvert(param->GetExData1(), val.toBool());
        return QVariant(val.toBool());
    case 0x00000004:
        setMute(param->GetExData1(), val.toBool());
        return QVariant(val.toBool());
    case 0x00000006:
        setGain(param->GetExData1(), val.toDouble());
        return QVariant(val.toDouble());
    default:
        return QVariant();
    }
}

void ConfigItemInput::setUserLabel(int idx, QString text)
{
    _userLabel.replace(idx, text);
    if (idx < _outPortList.count()) {
        _outPortList.at(idx)->setPortName(text);
    }
}

QString ConfigItemInput::getUserLabel(int idx)
{
    return _userLabel.at(idx);
}

bool ConfigItemInput::getPhantom(int offset)
{
//    AES70Device *AES70Dev = qobject_cast<ConfigScene *>(scene())->getTrnDev();
//    Device_Trn *trnDev = qobject_cast<Device_Trn *>(AES70Dev);
    return _trnDev->getConfigData().InMic48V[getRealChannel(offset)];
}

int ConfigItemInput::getPreampGain(int offset)
{
//    AES70Device *AES70Dev = qobject_cast<ConfigScene *>(scene())->getTrnDev();
//    Device_Trn *trnDev = qobject_cast<Device_Trn *>(AES70Dev);
    return _trnDev->getConfigData().InMicGain[getRealChannel(offset)];
}

void ConfigItemInput::setPhantom(int offset, bool checked)
{
    if(_trnDev->isOnline()){
        _trnDev->sendPhantomPack(getRealChannel(offset), checked);
    }
}

void ConfigItemInput::setPreampGain(int offset, int value)
{
    if(_trnDev->isOnline()){
        _trnDev->sendMicGainPack(getRealChannel(offset), value);
    }
}

void ConfigItemInput::setGain(int offset, double gain)
{
//    qDebug() << __FUNCTION__ << __LINE__ << gain;
    switch (_cfgType) {
    case CFG_TYPE_INPUT_1:
        _body1.gain = (gain + 100) * 100;
        break;
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        if (offset%2)  _bodyNew.gain[offset/2].gainB = (gain + 100) * 100;
        else   _bodyNew.gain[offset/2].gainA = (gain + 100) * 100;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemInput::setInvert(int offset, bool isInvert)
{
    switch (_cfgType) {
    case CFG_TYPE_INPUT_1:
        _body1.antiPhase =  isInvert;
        break;
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        _bodyNew.antiPhase &= ~(0x01 << offset);        //清除指定位
        _bodyNew.antiPhase |= (isInvert << offset);     //设置指定位
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemInput::setMute(int offset, bool isMute)
{
    switch (_cfgType) {
    case CFG_TYPE_INPUT_1:
        _body1.mute =  isMute;
        break;
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        _bodyNew.mute &= ~(0x01 << offset);
        _bodyNew.mute |= (isMute << offset);
        break;
    default:
        break;
    }

    //刷新设备及算法静音通道状态
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) {
        _outPortList.at(offset)->setMute(isMute);
        int realCh = _realCh +offset;
    }

    saveParamToDevice();
}

double ConfigItemInput::getGain(int offset)
{
    switch (_cfgType) {
    case CFG_TYPE_INPUT_1:
        return _body1.gain * 0.01 - 100;
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        if (offset&0x01)  return _bodyNew.gain[offset/2].gainB * 0.01 - 100;
        else  return _bodyNew.gain[offset/2].gainA * 0.01 - 100;
        break;
    default:
        break;
    }

    return 0;
}

bool ConfigItemInput::isInvert(int offset)
{
    switch (_cfgType) {
    case CFG_TYPE_INPUT_1:
        return _body1.antiPhase;
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        return ((_bodyNew.antiPhase >> offset) & 0x01);
    default:
        break;
    }

    return 0;
}

bool ConfigItemInput::isMute(int offset)
{
    switch (_cfgType) {
    case CFG_TYPE_INPUT_1:
        return _body1.mute;
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        return ((_bodyNew.mute >> offset) & 0x01);
    default:
        break;
    }

    return 0;
}

