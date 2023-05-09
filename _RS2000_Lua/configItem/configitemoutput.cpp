#include "configitemoutput.h"
#include "configsetting/cfgsettingoutput.h"
#include "deviceItem/deviceItem.h"

ConfigItemOutput::ConfigItemOutput(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    memset(&_body1, 0, sizeof(IG_PARACTL_OUTPUT1));
    memset(&_bodyNew, 0, sizeof(IG_PARACTL_OUTPUT_NEW));

    addSubType(cfgType);
    _max_port = 0x0001 << (_cfgType & 0x00FF);  //根据输入模块类型获取最大端口数1，2，4，8，16
    switch (_cfgType) {
    case CFG_TYPE_OUTPUT_1:
        _body1.mute = 0;
        _body1.antiPhase = 0;
        _body1.gain = (0 + 100) * 100;
        break;
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        _bodyNew.mute = 0;
        _bodyNew.antiPhase = 0;
        for (int i = 0; i < _max_port/2; ++i) {
            _bodyNew.gain[i].gainA = (0 + 100) * 100;
            _bodyNew.gain[i].gainB = (0 + 100) * 100;
        }
        break;
    default:
        _max_port = 0;
        break;
    }

    QStringList tmp;
    for (int i = 0; i < 16; ++i) {
        _userLabel.append(QString("Output %1").arg(i + 1));
    }
    initPort(_max_port, 0, _userLabel, tmp);

    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) {
        foreach(ItemPort *port, _inPortList)
            port->setEnableMute(true);
//        foreach(ItemPort *port, _outPortList)
//            port->setEnableMute(true);
    }
}

ConfigItemOutput::~ConfigItemOutput()
{
    if (_setting != NULL) {
        delete _setting;
        _setting = NULL;
    }
}


void ConfigItemOutput::initRealChannel(DevPortType portType,int realCh, int logicCh)
{
    _portType = portType;
    _realCh = realCh;
    _logicCh = logicCh;

    QString cftTypeName = Utils::getCfgPortModelName(false, _portType, logicCh, _max_port);
    setcfgTypeName(cftTypeName);

    for(int i = 0; i< _max_port; i++){
        QString portName = Utils::getCfgPortChannelName( _portType, _logicCh+i, tr("Out"));
        _userLabel[i] = portName;
        _inPortList[i]->setPortName(portName);
    }

    QColor itemColor(250,205,137);
    if(_portType == PortType_AES67) itemColor = QColor(255,247,153);

    setFillColor(itemColor);

    QList<CLuaPinParam_PC*> pinList;
    for(int i = 0; i<_max_port; i++)
    {
        int chNoOffset = i;
        QString groupName = QString("Out%1").arg(chNoOffset);
        pinList.append(new CLuaPinParam_PC(this, _customName, "AntiPhase", 0x00010001, 0x00010002, chNoOffset, 0x00, groupName));
        pinList.append(new CLuaPinParam_PC(this, _customName, "Mute", 0x00010003, 0x00010004, chNoOffset, 0x00, groupName));
        pinList.append(new CLuaPinParam_PC(this, _customName, "Gain", 0x00010005, 0x00010006, chNoOffset, 0x00, groupName));
    }
    initControlPins(pinList);
}

QWidget *ConfigItemOutput::getSettingForm()
{
    if(_proxyItem == NULL) _setting = NULL;
    if (_setting == NULL)
    {
        _setting = new CfgSettingOutput(this);
    }
    return _setting;
}

QByteArray ConfigItemOutput::getOverviewData(bool is16Bit)
{
    QByteArray sigFlowBuff;

    if(is16Bit){
        //格式化16bit真实通道
        IG_SIG_REAL_CH16 realChDef;
        int chGroup = (_max_port + 1) / 2;   //根据分组端口数格式化数据
        for(int i = 0; i<chGroup; i++ ){
            memset(&realChDef, 0, sizeof(IG_SIG_REAL_CH16));
            realChDef.channelA = _realCh+i*2;
            if(i*2 + 1 < _max_port) realChDef.channelB = _realCh+i*2+ 1;
            sigFlowBuff.append((const char*)&realChDef, sizeof(IG_SIG_REAL_CH16));
        }

        //格式化16bit信号流layer，row
        IG_SIGFLOW_SINGLE16 flowDef;
        int oDefCnt = _inputDefList.count();
        for(int i = 0; i<oDefCnt; i++) {
            memset(&flowDef, 0, sizeof(IG_SIGFLOW_SINGLE16));
            flowDef.column  = _inputDefList[i]->layer;
            flowDef.line    = _inputDefList[i]->row;
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
        int oDefCnt = _inputDefList.count();
        for(int i = 0; i<oDefCnt; i++) {
            if(i%2 == 0){
                memset(&flowDef, 0, sizeof(IG_SIGFLOW8_DOUBLE));
                flowDef.columnA  = _inputDefList[i]->layer;
                flowDef.lineA    = _inputDefList[i]->row;
                if(i == oDefCnt -1){
                    sigFlowBuff.append((const char*)&flowDef, sizeof(IG_SIGFLOW8_DOUBLE)); //奇数端口数最后一个端口
                }
            }
            else{
                flowDef.columnB  = _inputDefList[i]->layer;
                flowDef.lineB    = _inputDefList[i]->row;
                sigFlowBuff.append((const char*)&flowDef, sizeof(IG_SIGFLOW8_DOUBLE));
            }
        }
    }

    return sigFlowBuff;
}

QByteArray ConfigItemOutput::getDetailData()
{
    QByteArray paraBuff;
    switch (_cfgType) {
    case CFG_TYPE_OUTPUT_1:
        paraBuff.append((const char *)&_body1, sizeof(IG_PARACTL_OUTPUT1));
        break;
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        //根据模块类型确Detail包长度: (2+_max_port/2)个32位整数
        paraBuff.append((const char *)&_bodyNew, sizeof(unsigned int) * (2 + _max_port/2));
        break;
    default:
        break;
    }

    return paraBuff;
}


void ConfigItemOutput::setDetailData(unsigned int *d)
{
    if (_cfgType == CFG_TYPE_OUTPUT_1) {
        memcpy(&_body1, d, sizeof(IG_PARACTL_OUTPUT1));
    }
    else{
        memcpy(&_bodyNew, d, sizeof(IG_PARACTL_OUTPUT_NEW));
    }
    if (_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemOutput::setRtnData(unsigned int *d)
{
    quint8* pStartVU = (quint8*)d;
    DevType devType = getDevType();
    if(devType == Dev_Type_NAP_STM32H750_8A8N){
        //8*8设备电平表和16*16一样的总路数64路，后部分空置
        int totalNum = Utils::getDevTatalPortNum(Dev_Type_NAP_Zynq7020_16Analog_48AES67);
        pStartVU += totalNum + getRealChannel(0);
    }
    else{
        int totalNum = Utils::getDevTatalPortNum(devType);
        pStartVU += totalNum + getRealChannel(0);
    }

//    FC_RTU_InOutLevel* level1= (FC_RTU_InOutLevel*)d;
//    FC_RTU_InOutLevel128* level2= (FC_RTU_InOutLevel128*)d;
    if(_proxyItem == NULL) _setting = NULL;
    if (_setting) {
        _setting->setVuDisp(pStartVU);
    }
}

void ConfigItemOutput::updateCfgSettingLoadedData()
{
    if (_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemOutput::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_OUTPUT_GROUP));
    for (int i = 0; i < _max_port; ++i) {
        setUserLabel(i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_UserLabel).arg(i)).toString());
    }
    switch (_cfgType) {
    case CFG_TYPE_OUTPUT_1:
        setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
        setGain(0,   ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_GAIN).arg(0)).toDouble());
        setInvert(0, ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_INVERT).arg(0)).toBool());
        setMute(0,   ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_MUTE).arg(0)).toBool());
        break;
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:        
        setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
        for (int i = 0; i < _max_port; ++i) {
            setGain(i,   ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_GAIN).arg(i)).toDouble());
            setInvert(i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_INVERT).arg(i)).toBool());
            setMute(i,   ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_MUTE).arg(i)).toBool());
        }
        break;
    default:
        break;
    }
    ConfigIni->endGroup();
}


void ConfigItemOutput::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_OUTPUT_GROUP));
    for (int i = 0; i < 16; ++i) {
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_UserLabel).arg(i), getUserLabel(i));
    }
    switch (_cfgType) {
    case CFG_TYPE_OUTPUT_1:
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_CHANNEL), getRealChannel(0));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_GAIN).arg(0),   getGain(0));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_INVERT).arg(0), isInvert(0));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_MUTE).arg(0),   isMute(0));
        break;
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO),      getModuleNum());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_CHANNEL), getRealChannel(0));
        for (int i = 0; i < _max_port; ++i) {
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_GAIN).arg(i),   getGain(i));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_INVERT).arg(i), isInvert(i));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_OUTPUT_MUTE).arg(i),   isMute(i));
        }
        break;
    default:
        break;
    }
    ConfigIni->endGroup();
}

QVariant ConfigItemOutput::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    switch(param->SetCmdKey())
    {
    case 0x00010002:
        setInvert(param->GetExData1(), val.toBool());
        return QVariant(val.toBool());
    case 0x00010004:
        setMute(param->GetExData1(), val.toBool());
        return QVariant(val.toBool());
    case 0x00010006:
        setGain(param->GetExData1(), val.toDouble());
        return QVariant(val.toDouble());
    default:
        return QVariant();
    }
}

void ConfigItemOutput::setUserLabel(int idx, QString text)
{
    _userLabel.replace(idx, text);
    if (idx < _inPortList.count()) {
        _inPortList.at(idx)->setPortName(text);
    }
}

QString ConfigItemOutput::getUserLabel(int idx)
{
    return _userLabel.at(idx);
}

void ConfigItemOutput::setGain(int offset, double gain)
{
    switch (_cfgType) {
    case CFG_TYPE_OUTPUT_1:
        _body1.gain = (gain + 100) * 100;
        break;
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        if (offset%2)  _bodyNew.gain[offset/2].gainB = (gain + 100) * 100;
        else   _bodyNew.gain[offset/2].gainA = (gain + 100) * 100;
        break;
    default:
        break;
    }
    saveParamToDevice();
}

void ConfigItemOutput::setInvert(int offset, bool isInvert)
{
    switch (_cfgType) {
    case CFG_TYPE_OUTPUT_1:
        _body1.antiPhase =  isInvert;
        break;
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        _bodyNew.antiPhase &= ~(0x01 << offset);        //清除指定位
        _bodyNew.antiPhase |= (isInvert << offset);     //设置指定位
        break;
    default:
        break;
    }
    saveParamToDevice();
}

void ConfigItemOutput::setMute(int offset, bool isMute)
{
    switch (_cfgType) {
    case CFG_TYPE_OUTPUT_1:
        _body1.mute =  isMute;
        break;
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        _bodyNew.mute &= ~(0x01 << offset);
        _bodyNew.mute |= (isMute << offset);
        break;
    default:
        break;
    }

    //刷新设备及算法静音通道状态
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) {
        _inPortList.at(offset)->setMute(isMute);
    }


    saveParamToDevice();
}

void ConfigItemOutput::setAllMute(bool isMute)
{
    switch (_cfgType) {
    case CFG_TYPE_OUTPUT_1:
        _body1.mute =  isMute;
        break;
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        for(int offset = 0; offset< _max_port; offset++)
        {
            _bodyNew.mute &= ~(0x01 << offset);
            _bodyNew.mute |= (isMute << offset);
        }
        break;
    default:
        break;
    }

    //刷新设备及算法静音通道状态
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) {
        for(int offset = 0; offset< _max_port; offset++)
        {
            _inPortList.at(offset)->setMute(isMute);
        }
    }
    if(_setting){
        _setting->updateUiData();
    }

    saveParamToDevice();
}

double ConfigItemOutput::getGain(int offset)
{
    switch (_cfgType) {
    case CFG_TYPE_OUTPUT_1:
        return _body1.gain * 0.01 - 100;
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        if (offset&0x01)  return _bodyNew.gain[offset/2].gainB * 0.01 - 100;
        else  return _bodyNew.gain[offset/2].gainA * 0.01 - 100;
        break;
    default:
        break;
    }

    return 0;
}

bool ConfigItemOutput::isInvert(int offset)
{
    switch (_cfgType) {
    case CFG_TYPE_OUTPUT_1:
        return _body1.antiPhase;
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        return ((_bodyNew.antiPhase >> offset) & 0x01);
    default:
        break;
    }

    return 0;
}

bool ConfigItemOutput::isMute(int offset)
{
    switch (_cfgType) {
    case CFG_TYPE_OUTPUT_1:
        return _body1.mute;
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        return ((_bodyNew.mute >> offset) & 0x01);
    default:
        break;
    }

    return 0;
}


