#ifndef CONFIGITEMAUDIOPLAYER_H
#define CONFIGITEMAUDIOPLAYER_H
#include "configitem.h"

class CfgSettingAudioPlayer;
class ConfigItemAudioPlayer : public ConfigItem
{
    Q_OBJECT
public:
    ConfigItemAudioPlayer(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 106);
    ~ConfigItemAudioPlayer();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    DesignItem* clone(DesignItem* toItem) override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    void play(QString audioName, double persent = 0);
    void stop();
    void pause();
    void resume();

    void setPlay(bool state);
    inline void setStop(bool state){stop();}

    static int GlobalPlayerIdx;    

private:
    void initData();

private:
    int _playerIdx;
    quint8 _playCmd;
    QString _audioName;
    double _startPersent;

    typedef struct{
        __OPACK32_T3(
            OPACKU,playerIdx,8,				//序号
            OPACKU,playCmd,8,				//0:stop, 1:start, 2:pause, 3:resume
            OPACKU,startPersent,16
        );
        char audioFullPath[256];
    }IG_PARACTL_PLAYER;

    IG_PARACTL_PLAYER _body;
    CfgSettingAudioPlayer *_setting;
};

#endif // CONFIGITEMAUDIOPLAYER_H
