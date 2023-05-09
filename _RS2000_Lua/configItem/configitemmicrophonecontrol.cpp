#include "configitemmicrophonecontrol.h"
#include "protocal/packet.h"
#include "configSetting/cfgsettingmicrophonecontrol.h"


ConfigItemMicrophoneControl::ConfigItemMicrophoneControl(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
  :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    memset(&_header32, 0, sizeof(IG_SIGFLOW_MICROPHONECTRL));
    memset(&_body32, 0, sizeof(IG_PARACTL_MICROPHONECTRL));
    memset(&_header48, 0, sizeof(IG_SIGFLOW_SIGSWITCH48));
    memset(&_body48, 0, sizeof(IG_PARACTL_SIGSWITCH48));
//    qDebug()<<"max"<<_body.max_mic_num;
//    _vipMicrophoneNum = 0;
//    _allowMicrophoneNum = 0;
//    addSubType(CFG_TYPE_VOICETUBE_1);
    addSubType(CFG_TYPE_VOICETUBE_2);
	
	int portNum = 32;
    switch (cfgType) {
	case CFG_TYPE_VOICETUBE_1:
		portNum = 32;
		break;
	case CFG_TYPE_VOICETUBE_2:
		portNum = 48;
		break;
	default:
		break;
	}

    initPort(portNum, portNum);
	
//    qDebug()<<"name:"<<name<<"_"<<"typeName:"<<typeName;
    _setting = NULL;
}

ConfigItemMicrophoneControl::~ConfigItemMicrophoneControl()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemMicrophoneControl::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingMicrophoneControl(this);
    }
    return _setting;
}

QByteArray ConfigItemMicrophoneControl::getOverviewData(bool is16Bit)
{
    //麦克风控制协议，三所定制功能,目前只支持8bit协议
    QByteArray paraBuff;

    if (_cfgType == CFG_TYPE_VOICETUBE_1) {
        for(int i = 0; i <16; i++) {
            _header32.flow.inFlow[i].columnA = _inputDefList[2*i]->layer;
            _header32.flow.inFlow[i].lineA   = _inputDefList[2*i]->row;
            _header32.flow.inFlow[i].columnB = _inputDefList[2*i+1]->layer;
            _header32.flow.inFlow[i].lineB   = _inputDefList[2*i+1]->row;

            _header32.flow.outFlow[i].columnA = _outputDefList[2*i]->layer;
            _header32.flow.outFlow[i].lineA = _outputDefList[2*i]->row;
            _header32.flow.outFlow[i].columnB = _outputDefList[2*i+1]->layer;
            _header32.flow.outFlow[i].lineB = _outputDefList[2*i+1]->row;
        }
        _header32.vip_no      = _body32.vip_no;
        _header32.max_mic_num = _body32.max_mic_num;
        _header32.module_num  = this->getModuleNum();
//      _header32.exdata = (vip_no << 24) | (max_mic_num << 16) | (_moduleNum);
        paraBuff.append((const char *)&_header32,sizeof(IG_SIGFLOW_MICROPHONECTRL));
    }
    else if (_cfgType == CFG_TYPE_VOICETUBE_2) {
        for(int i = 0; i < 24; i++) {
            _header48.flow.inFlow[i].columnA = _inputDefList[2*i]->layer;
            _header48.flow.inFlow[i].lineA   = _inputDefList[2*i]->row;
            _header48.flow.inFlow[i].columnB = _inputDefList[2*i+1]->layer;
            _header48.flow.inFlow[i].lineB   = _inputDefList[2*i+1]->row;

            _header48.flow.outFlow[i].columnA = _outputDefList[2*i]->layer;
            _header48.flow.outFlow[i].lineA = _outputDefList[2*i]->row;
            _header48.flow.outFlow[i].columnB = _outputDefList[2*i+1]->layer;
            _header48.flow.outFlow[i].lineB = _outputDefList[2*i+1]->row;
        }
        _header48.vip_no      = _body48.vip_no;
        _header48.max_mic_num = _body48.max_mic_num;
        _header48.module_num  = this->getModuleNum();
//      _header48.exdata = (vip_no << 24) | (max_mic_num << 16) | (_moduleNum);
        paraBuff.append((const char *)&_header48,sizeof(IG_SIGFLOW_SIGSWITCH48));
    }
    return paraBuff;
}

QByteArray ConfigItemMicrophoneControl::getDetailData()
{
    QByteArray paraBuff;

    if (_cfgType == CFG_TYPE_VOICETUBE_1) {
        _body32.module_num = _header32.module_num;
    //    _body32.exdata = (vip_no << 24) | (max_mic_num << 16) | (_moduleNum);
        _body32.sigswitch = 0x00000000;
        paraBuff.append((const char *)&_body32,sizeof(IG_PARACTL_MICROPHONECTRL));
    }
    else if (_cfgType == CFG_TYPE_VOICETUBE_2) {
        _body48.module_num = _header48.module_num;
    //    _body48.exdata = (vip_no << 24) | (max_mic_num << 16) | (_moduleNum);
        _body48.sigswitch[0] = 0x00000000;
        _body48.sigswitch[1] = 0x00000000;
        paraBuff.append((const char *)&_body48,sizeof(IG_PARACTL_SIGSWITCH48));
    }
    return paraBuff;
}


void ConfigItemMicrophoneControl::setDetailData(unsigned int *d)
{

}

void ConfigItemMicrophoneControl::setRtnData(unsigned int *d)
{

}

void ConfigItemMicrophoneControl::updateCfgSettingLoadedData()
{

}

void ConfigItemMicrophoneControl::loadParaFromSetting(QSettings *ConfigIni)
{
    assert("no loadParaFromSetting(QSettings *ConfigIni)" );
}

void ConfigItemMicrophoneControl::saveParaToSetting(QSettings *ConfigIni)
{
    assert("no saveParaToSetting(QSettings *ConfigIni)" );
}

QVariant ConfigItemMicrophoneControl::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    //该模块暂时没有Lua控制针脚
    Q_UNUSED(param)
    assert(false);
    return val;
}

quint16 ConfigItemMicrophoneControl::getMicrophoneNo()
{
    unsigned char vip_no = 0;
    if (_cfgType == CFG_TYPE_VOICETUBE_1) {
        vip_no = _body32.vip_no;
    }
    else if (_cfgType == CFG_TYPE_VOICETUBE_2) {
        vip_no = _body48.vip_no;
    }
    return vip_no;
}

quint16 ConfigItemMicrophoneControl::getMaxConnectNum()
{
    unsigned char max_mic_num = 0;
    if (_cfgType == CFG_TYPE_VOICETUBE_1) {
        max_mic_num = _body32.max_mic_num;
    }
    else if (_cfgType == CFG_TYPE_VOICETUBE_2) {
        max_mic_num = _body48.max_mic_num;
    }
    return max_mic_num;
}

void ConfigItemMicrophoneControl::setMicrophoneControlInfo(int idx,int type)
{   
    if(type == 0) {
        if (_cfgType == CFG_TYPE_VOICETUBE_1) {
            _body32.vip_no = idx;
        }
        else if (_cfgType == CFG_TYPE_VOICETUBE_2) {
            _body48.vip_no = idx;
        }
    }
    else if(type == 1) {
        if (_cfgType == CFG_TYPE_VOICETUBE_1) {
            _body32.max_mic_num = idx;
        }
        else if (_cfgType == CFG_TYPE_VOICETUBE_2) {
            _body48.max_mic_num = idx;
        }
    }

    saveParamToDevice();
}
