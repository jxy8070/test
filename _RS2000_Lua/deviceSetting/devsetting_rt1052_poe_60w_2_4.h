#ifndef DEVSETTING_RT1052_POE_60W_2_4_H
#define DEVSETTING_RT1052_POE_60W_2_4_H

#include "customWidget/overwriteitem.h"
#include "Protocal/packet.h"


// 功放控制界面
class AlarmConfig;
class DeviceItem_RT1052_Poe_60W_2_4;
class Device_RT1052_Poe_60W_2_4;
namespace Ui {
#ifdef Logo_BeiJingHuiSheng
class DevSetting_RT1052_Poe_60W_2_4_Ceiadap;
#else
class DevSetting_RT1052_Poe_60W_2_4;
#endif
}
class DevSetting_RT1052_Poe_60W_2_4 : public DevSettingDlg
{
    Q_OBJECT
public:
    DevSetting_RT1052_Poe_60W_2_4(DeviceItem_RT1052_Poe_60W_2_4 *devItem, QWidget *parent = 0);
    ~DevSetting_RT1052_Poe_60W_2_4();

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
    void onChannelEqChanged(quint8 ch, bool bypass, EqData *eqPara);
    void onEqChanged(quint8 ch, bool bypass, EqData *eqPara);

    /* DYN */
    void setDyn(void);
    void onDynChanged(quint8 ch, bool isIn ,DynData dynData);

    /* XOVER */
    void setXOver(void);
    void onXoverChanged(uint8_t ch, ConfigData_Xover xData);

    /* AOut Mixer */
#ifdef Logo_BeiJingHuiSheng
    void mixerModeChangedSlot(int idx);
#endif
    void setAOutMixer(void);
    void MixerChangedSlot(int out, int in, bool enable);

    void DisplaySlot(struct MsgData_RT1052_Poe b);

    void on_save_clicked();

private:
    void connectSigSlots();
    void onPfDataChanged(quint8 ch, bool hfb, bool lfb);

private:
#define MAX_DELAY_TIME      200*48
#ifdef Logo_BeiJingHuiSheng
    Ui::DevSetting_RT1052_Poe_60W_2_4_Ceiadap *ui;
#else
    Ui::DevSetting_RT1052_Poe_60W_2_4 *ui;
#endif

    DeviceItem_RT1052_Poe_60W_2_4 *_devItem;
    Device_RT1052_Poe_60W_2_4 *_devAmp;
};

#endif
