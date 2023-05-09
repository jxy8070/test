#ifndef DEVSETTING_AMP_0_2_LINE_FANGTU_H
#define DEVSETTING_AMP_0_2_LINE_FANGTU_H

namespace Ui {
class DevSetting_Amp_0_2_Line_FangTu;
}

#include "Protocal/packet.h"
#include "customWidget/overwriteitem.h"

// 功放控制界面
class AlarmConfig;
class DeviceItem_Amp_0_2_Line;
class Device_Amp_0_2_Line;
class DevSetting_Amp_0_2_Line_FangTu : public DevSettingDlg
{
    Q_OBJECT
public:
    DevSetting_Amp_0_2_Line_FangTu(DeviceItem_Amp_0_2_Line *devItem, QWidget *parent = 0);
    ~DevSetting_Amp_0_2_Line_FangTu();

protected:

private slots:
    void displayConfigValue();

    /* Gain\Mute\Phase\Delay */
    void setGain(int value);
    void setMute(bool enable);
    void setPhase(bool enable);
    void setDelay(int value);

    /* Eq */
    void setEq(void);
    void onEqChanged(quint8 ch, bool bypass, EqData *eqPara);

    /* DYN */
    void setDyn(void);
    void onDynChanged(quint8 ch, bool isIn ,DynData dynData);

    /* XOVER */
    void setXOver(void);

    /* AOut Mixer */
    void setAOutMixer(void);

    /* vu */
    void DisplaySlot(struct MsgData_Amp_0_2_Line b);

    void on_save_clicked();

private:
    void connectSigSlots();
    void onPfDataChanged(quint8 ch, bool hfb, bool lfb);
private:
#define MAX_DELAY_TIME      200*48
    Ui::DevSetting_Amp_0_2_Line_FangTu *ui;
    DeviceItem_Amp_0_2_Line *_devItem;
    Device_Amp_0_2_Line *_devAmp;
};

#endif // DEVSETTING_AMP_0_2_LINE_FANGTU_H
