#ifndef DEVSETTING_RT1052_AP400_FANGTU_H
#define DEVSETTING_RT1052_AP400_FANGTU_H

#include "customWidget/overwriteitem.h"
#include "Protocal/packet.h"

namespace Ui {
class DevSetting_RT1052_AP400_FangTu;
}

// 功放控制界面
class AlarmConfig;
class DeviceItem_RT1052_AP400;
class Device_RT1052_AP400;
class DevSetting_RT1052_AP400_FangTu : public DevSettingDlg
{
    Q_OBJECT
public:
    DevSetting_RT1052_AP400_FangTu(DeviceItem_RT1052_AP400 *devItem, QWidget *parent = 0);
    ~DevSetting_RT1052_AP400_FangTu();

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
    void setAOutRoute(void);
#ifdef Logo_BeiJingHuiSheng
    void setRouteMode(int v);
    void setPriSelect(int v);
    void setThreshold(int v);
    void setAutoRecovery(bool v);
#endif
    /* Directivity Angle */
    void setDirectivityAngle(int cur);

    void DisplaySlot(struct MsgData_RT1052_Poe b);

    void on_save_clicked();

private:
    void connectSigSlots();
    void onPfDataChanged(quint8 ch, bool hfb, bool lfb);
    void HiddenControls(void);
private:
#define MAX_DELAY_TIME      200*48

    Ui::DevSetting_RT1052_AP400_FangTu *ui;
    DeviceItem_RT1052_AP400 *_devItem;
    Device_RT1052_AP400 *_devAmp;
};

#endif // DEVSETTING_RT1052_AP400_FANGTU_H
