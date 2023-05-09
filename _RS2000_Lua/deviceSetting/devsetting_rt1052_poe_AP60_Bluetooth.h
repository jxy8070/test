#ifndef DEVSETTING_RT1052_POE_AP60_BLUETOOTH_H
#define DEVSETTING_RT1052_POE_AP60_BLUETOOTH_H

#include "customWidget/overwriteitem.h"
#include "protocal/packet.h"


// 功放控制界面
class AlarmConfig;
class DeviceItem_RT1052_Poe_AP60_Bluetooth;
class Device_RT1052_Poe_AP60_Bluetooth;
namespace Ui {
class DevSetting_RT1052_Poe_AP60_Bluetooth;
}
class DevSetting_RT1052_Poe_AP60_Bluetooth : public DevSettingDlg
{
    Q_OBJECT
public:
    DevSetting_RT1052_Poe_AP60_Bluetooth(DeviceItem_RT1052_Poe_AP60_Bluetooth *devItem, QWidget *parent = 0);
    ~DevSetting_RT1052_Poe_AP60_Bluetooth();

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
    void setAOutMixer(void);
    void MixerChangedSlot(int out, int in, bool enable);

    void bt_function_clicked(bool e);
    void DisplaySlot(struct MsgData_RT1052_Poe b);

    void on_save_clicked();

private:
    void connectSigSlots();
    void onPfDataChanged(quint8 ch, bool hfb, bool lfb);

private:
#define MAX_DELAY_TIME      200*48
    Ui::DevSetting_RT1052_Poe_AP60_Bluetooth *ui;
    DeviceItem_RT1052_Poe_AP60_Bluetooth *_devItem;
    Device_RT1052_Poe_AP60_Bluetooth *_devAmp;
};

#endif
