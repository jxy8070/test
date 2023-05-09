#ifndef DEVSETTING_AMP_MODULE_H
#define DEVSETTING_AMP_MODULE_H

#include "customWidget/overwriteitem.h"
#include "protocal/packet.h"

// 功放控制界面
class AlarmConfig;
class DeviceItem_RT1052_AMP_MODULE_4_4;
class Device_Amp_Module;
namespace Ui {
class DevSetting_Amp_Module;
}

class DevSetting_Amp_Module : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit DevSetting_Amp_Module(DeviceItem_RT1052_AMP_MODULE_4_4 *devItem, QWidget *parent = 0);
    ~DevSetting_Amp_Module();


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

    /* AOut Mixer */
    void setAOutMixer(void);

    /* NOut Route */
//    void setNOutRoute(void);

    void DisplaySlot(struct MsgData_RT1052_Poe b);

//    void onPfDataChanged(quint8 channel);

    void on_save_clicked();


private:
    void connectSigSlots();
//    void onPfDataChanged(quint8 ch, bool hfb, bool lfb);
private:
#define MAX_DELAY_TIME      200*48
    Ui::DevSetting_Amp_Module *ui;
    DeviceItem_RT1052_AMP_MODULE_4_4 *_devItem;
    Device_Amp_Module *_devAmp;
};

#endif // DEVSETTING_AMP_MODULE_H
