#include "configitemaudioplayer.h"
#include "configSetting/cfgsettingaudioplayer.h"
#include "device/device_trn.h"

int ConfigItemAudioPlayer::GlobalPlayerIdx = 0;

ConfigItemAudioPlayer::ConfigItemAudioPlayer(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    int inNum = 0, outNum = 2;
    memset(&_body, 0, sizeof(IG_PARACTL_PLAYER));
    addSubType(CFG_TYPE_AUDIO_PLAYER1);
    addSubType(CFG_TYPE_AUDIO_PLAYER2);
    addSubType(CFG_TYPE_AUDIO_PLAYER4);
    addSubType(CFG_TYPE_AUDIO_PLAYER8);
    addSubType(CFG_TYPE_AUDIO_PLAYER16);
    if(cfgType == CFG_TYPE_AUDIO_PLAYER1)  outNum = 1;
    else if(cfgType == CFG_TYPE_AUDIO_PLAYER2)  outNum = 2;
    else if(cfgType == CFG_TYPE_AUDIO_PLAYER4)  outNum = 4;
    else if(cfgType == CFG_TYPE_AUDIO_PLAYER8)  outNum = 8;
    else if(cfgType == CFG_TYPE_AUDIO_PLAYER16)  outNum = 16;

    initPort(inNum, outNum);
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "Play", 0x000F0001, 0x000F0002, 0x01, 0x00));
    pinList.append(new CLuaPinParam_PC(this, name, "Stop", 0x000F0001, 0x000F0002, 0x00, 0x00));
//    pinList.append(new CLuaPinParam_PC(this, name, "Pause", 0x000F0001, 0x000F0002, 0x02, 0x00));
    initControlPins(pinList);
    initData();
    _setting = NULL;
}

ConfigItemAudioPlayer::~ConfigItemAudioPlayer()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

void ConfigItemAudioPlayer::initData()
{
    _playerIdx = GlobalPlayerIdx++;
    _playCmd = 0;
    _startPersent = 0;
    _audioName = "";
}



QWidget *ConfigItemAudioPlayer::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingAudioPlayer(this);
    }
    return _setting;
}


QByteArray ConfigItemAudioPlayer::getDetailData()
{
    QByteArray paraBuff;
    _body.playerIdx = _playerIdx;
    _body.playCmd = _playCmd;
    _body.startPersent = (quint16)(_startPersent*65535/100);

    QByteArray ba = _audioName.toUtf8();
    int len = ba.length();
    memcpy(_body.audioFullPath, ba.data(), len);
    if(len < 256) memset(_body.audioFullPath + len, 0, 256 - len);

    paraBuff.append((const char*)&_body, sizeof(IG_PARACTL_PLAYER));

    return paraBuff;
}

void ConfigItemAudioPlayer::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(IG_PARACTL_PLAYER));

    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemAudioPlayer::setRtnData(unsigned int *d)
{
    if(_setting != NULL)
    {
        _setting->DispRtnData(_cfgType, d);
    }
}

void ConfigItemAudioPlayer::updateCfgSettingLoadedData()
{
    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

DesignItem* ConfigItemAudioPlayer::clone(DesignItem* toItem)
{
    ConfigItemAudioPlayer *item = NULL;
    if(toItem == NULL) item = new ConfigItemAudioPlayer(_trnDev, _cfgType, _customName, _cfgTypeName, scenePos(), _rectWidth);
    else item = qgraphicsitem_cast<ConfigItemAudioPlayer *>(toItem);
    if(item != NULL)
    {
        //复制参数
//        item->setMute(getMute());
    }
    return item;
}


void ConfigItemAudioPlayer::loadParaFromSetting(QSettings *ConfigIni)
{
//    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SIG_GROUP));
//    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
//    setMute(ConfigIni->value(QString(CONFIG_DEVICE_DEV_SIG_MUTE)).toBool());
//    setLevel(ConfigIni->value(QString(CONFIG_DEVICE_DEV_SIG_LEVEL)).toDouble());
//    setFreq(ConfigIni->value(QString(CONFIG_DEVICE_DEV_SIG_FREQ)).toDouble());
//    ConfigIni->endGroup();
}


void ConfigItemAudioPlayer::saveParaToSetting(QSettings *ConfigIni)
{
//    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SIG_GROUP));
//    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
//    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SIG_MUTE), getMute());
//    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SIG_LEVEL), getLevel());
//    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SIG_FREQ), getFreq());
//    ConfigIni->endGroup();
}

QVariant ConfigItemAudioPlayer::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    switch(param->SetCmdKey())
    {
    case 0x000F0002:
        _playCmd = param->GetExData1();
        saveParamToDevice();
        return QVariant();
    default:
        return QVariant();
    }
}

void ConfigItemAudioPlayer::play(QString audioName, double persent)
{
    _audioName = audioName;
    _playCmd = 0x01;
    _startPersent = persent;
    saveParamToDevice();
}

void ConfigItemAudioPlayer::stop()
{
     _playCmd = 0x00;
     saveParamToDevice();
}

void ConfigItemAudioPlayer::pause()
{
    _playCmd = 0x02;
    saveParamToDevice();
}

void ConfigItemAudioPlayer::resume()
{
    _playCmd = 0x03;
    saveParamToDevice();
}

void ConfigItemAudioPlayer::setPlay(bool state)
{
    Q_UNUSED(state)
    _playCmd = 0x01;
    saveParamToDevice();
}
