#ifndef DEVSETTING_AMP_2_0_MIC_LINE_FANGTU_H
#define DEVSETTING_AMP_2_0_MIC_LINE_FANGTU_H

#include "customWidget/overwriteitem.h"
#include "Protocal/packet.h"

namespace Ui {
class DevSetting_Amp_2_0_Mic_Line_FangTu;
}

// 功放控制界面
//class InterRouter;
//class SpkRouter;
//class EqViewNormal;
//class Trn_DynWidget;
//class AmpPf;
//class AlarmRecord;
class AlarmConfig;
class DeviceItem_Amp_2_0_Mic_Line;
class Device_Amp_2_0_Mic_Line;
class DevSetting_Amp_2_0_Mic_Line_FangTu : public DevSettingDlg
{
    Q_OBJECT
public:
    DevSetting_Amp_2_0_Mic_Line_FangTu(DeviceItem_Amp_2_0_Mic_Line *devItem, QWidget *parent = 0);
    ~DevSetting_Amp_2_0_Mic_Line_FangTu();

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
    void OutGainChanged(int value);

    void DisplaySlot(MsgData_Amp_2_0_Mic_Line b);

    void micOpenClicked(bool);

    void on_save_clicked();

private:
    void connectSigSlots();
private:
    Ui::DevSetting_Amp_2_0_Mic_Line_FangTu *ui;
    DeviceItem_Amp_2_0_Mic_Line *_devItem;
    Device_Amp_2_0_Mic_Line *_dev;

    bool _syncing;
    bool _hasData;
    QTimer *_timer;
};

#endif // DEVSETTING_AMP_2_0_MIC_LINE_FANGTU_H
