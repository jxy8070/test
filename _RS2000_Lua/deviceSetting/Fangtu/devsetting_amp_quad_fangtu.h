#ifndef DEVSETTING_AMP_QUAD_FANGTU_H
#define DEVSETTING_AMP_QUAD_FANGTU_H


#include "customWidget/overwriteitem.h"
#include "Protocal/packet.h"


// 功放控制界面
class AlarmConfig;
class DeviceItem_Amp_Quad;
class Device_Amp_Quad;
namespace Ui {
class DevSetting_Amp_Quad_FangTu;
}
class DevSetting_Amp_Quad_FangTu : public DevSettingDlg
{
    Q_OBJECT
public:
    DevSetting_Amp_Quad_FangTu(DeviceItem_Amp_Quad *devItem, QWidget *parent = 0);
    ~DevSetting_Amp_Quad_FangTu();

protected:

private slots:
    void displayConfigValue();

    /* Gain\Mute\Phase\Delay */
    void setGain(int value);
    void setMute(bool enable);
    void setPhase(bool enable);
//    void setDelay(int value);
    void setDelayMs(double value);

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
    void setNOutRoute(void);

    void DisplaySlot(struct MsgData_RT1052_Poe b);
    /* 路由 */
//    void on_router_clicked();

    /* eq */
//    void on_eq1_clicked();


    /* filter */
//    void on_filter1HPFSet_clicked();
//    void on_filter1LPFSet_clicked();
//    void on_filter1HPF_clicked();
//    void on_filter1Bypass_clicked();
//    void on_filter1LPF_clicked();
    void onPfDataChanged(quint8 channel);

    /* dyn */
//    void on_dyn1_clicked();


//    void on_saveAsBtn_clicked();
//    void on_loadBtn_clicked();
//    void on_saveBtn_clicked();

//    void on_gain1P_clicked(bool checked);
//    void on_gain1M_clicked(bool checked);

//    void onAdcDataChanged(AlarmConfig *alarmCfg);
    void on_save_clicked();

private:
    void connectSigSlots();
    void onPfDataChanged(quint8 ch, bool hfb, bool lfb);
private:
#define MAX_DELAY_TIME      200*48
    Ui::DevSetting_Amp_Quad_FangTu *ui;
    DeviceItem_Amp_Quad *_devItem;
    Device_Amp_Quad *_devAmp;
};

#endif // DEVSETTING_AMP_QUAD_FANGTU_H
