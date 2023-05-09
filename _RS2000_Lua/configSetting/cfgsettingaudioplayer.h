#ifndef CFGSETTINGAUDIOPLAYER_H
#define CFGSETTINGAUDIOPLAYER_H

#include "Protocal/packet.h"

class ConfigItemAudioPlayer;
class AES70Device;
class Device_Trn;
namespace Ui {
class CfgSettingAudioPlayer;
}

class CfgSettingAudioPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingAudioPlayer(ConfigItemAudioPlayer *cfgItem, QWidget *parent = nullptr);
    ~CfgSettingAudioPlayer();
    void updateUiData();
    void DispRtnData(CfgType type, unsigned int *d);

private:
    void initUI(void);
    void initConnectSlots(void);
    Device_Trn* getTrnDevice();

private slots:
    void onPlay();
    void onStop();
    void onPause();
    void onResume();
    void onPauseResume(bool isPause);
    void onReflashFiles();
    void onFileListChanged(QStringList playFiles);

private:
    Ui::CfgSettingAudioPlayer *ui;
    ConfigItemAudioPlayer *_cfgItem;
};

#endif // CFGSETTINGAUDIOPLAYER_H
