#ifndef DEVSETTING_MIC_QUAD_H
#define DEVSETTING_MIC_QUAD_H

#include "customWidget/overwriteitem.h"
#include "protocal/packet.h"


// 功放控制界面
//class InterRouter;
//class SpkRouter;
//class EqViewNormal;
//class Trn_DynWidget;
//class AmpPf;
//class AlarmRecord;
class AlarmConfig;
class DeviceItem_Mic_Quad;
class Device_Mic_Quad;
namespace Ui {
class DevSetting_Mic_Quad;
}
class DevSetting_Mic_Quad : public DevSettingDlg
{
    Q_OBJECT
public:
    DevSetting_Mic_Quad(DeviceItem_Mic_Quad *devItem, QWidget *parent = 0);
    ~DevSetting_Mic_Quad();

private slots:
    void displayConfigValue();

    void MicGainChanged(int value);
    void MicPhantomChanged(bool enable);
    void GainChanged(int value);
    void setMute(bool enable);

    void xoverClicked(void);

    void DynClicked(void);
    void onDynChanged(quint8 ch, bool isIn, DynData dynData);

    void EqClicked(void);
    void onEqChanged(quint8 ch, bool bypass, EqData *eqPara);

    void OutPhaseClicked(bool enable);
    void MixerClicked(void);
    void MixerChangedSlot(int out, int in, bool enable);
    void OutGainChanged(int value);

    void DisplaySlot(MsgData_Mic_Quad b);

    void on_save_clicked();

private:
    void connectSigSlots();

private:
    Ui::DevSetting_Mic_Quad *ui;
    DeviceItem_Mic_Quad *_devItem;
    Device_Mic_Quad *_dev;

    bool _syncing;
    bool _hasData;
    QTimer *_timer;
};

#endif
