#ifndef DEVSETTING_RT1052_POE_60W_2_4_FANGTU_H
#define DEVSETTING_RT1052_POE_60W_2_4_FANGTU_H

#include "customWidget/overwriteitem.h"
#include "Protocal/packet.h"


// 功放控制界面
class AlarmConfig;
class DeviceItem_RT1052_Poe_60W_2_4;
class Device_RT1052_Poe_60W_2_4;
namespace Ui {
    class DevSetting_RT1052_Poe_60W_2_4_FangTu;
}
class DevSetting_RT1052_Poe_60W_2_4_FangTu : public DevSettingDlg
{
    Q_OBJECT
public:
    DevSetting_RT1052_Poe_60W_2_4_FangTu(DeviceItem_RT1052_Poe_60W_2_4 *devItem, QWidget *parent = 0);
    ~DevSetting_RT1052_Poe_60W_2_4_FangTu();

protected:

private slots:
    void displayConfigValue();

    /* Gain\Mute\Phase\Delay */
    void setGain(int value);
    void setMute(bool enable);
    void setPhase(bool enable);
    void setDelay(double value);

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
    void setAOutMixer(void);
    void MixerChangedSlot(int out, int in, bool enable);

    void DisplaySlot(struct MsgData_RT1052_Poe b);

    void on_save_clicked();
    void on_saveAsBtn_clicked();
    void on_loadBtn_clicked();

private:
    void connectSigSlots();
    void onPfDataChanged(quint8 ch, bool hfb, bool lfb);

private:
    #define MAX_DELAY_TIME      200*48
    Ui::DevSetting_RT1052_Poe_60W_2_4_FangTu *ui;

    DeviceItem_RT1052_Poe_60W_2_4 *_devItem;
    Device_RT1052_Poe_60W_2_4 *_devAmp;
};

#endif // DEVSETTING_RT1052_POE_60W_2_4_FANGTU_H
