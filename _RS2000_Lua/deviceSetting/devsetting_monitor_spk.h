#ifndef DEVSETTING_Monitor_SPK_H
#define DEVSETTING_Monitor_SPK_H

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
class DeviceItem_Monitor_SPK;
class Device_Monitor_SPK;
namespace Ui {
class DevSetting_Monitor_SPK;
}
class DevSetting_Monitor_SPK : public DevSettingDlg
{
    Q_OBJECT
public:
    DevSetting_Monitor_SPK(DeviceItem_Monitor_SPK *devItem, QWidget *parent = 0);
    ~DevSetting_Monitor_SPK();

protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    void displayConfigValue();

    /* 路由 */
    void on_router_clicked();

    /* eq */
    void on_eq1_clicked();
    void on_eq2_clicked();
    void on_eq3_clicked();
    void on_eq4_clicked();
    void onEqChanged(quint8 ch, bool bypass, EqData *eqPara);

    /* filter */
    void on_filter1HPFSet_clicked();
    void on_filter1LPFSet_clicked();
    void on_filter1HPF_clicked();
    void on_filter1Bypass_clicked();
    void on_filter1LPF_clicked();
    void on_filter2HPFSet_clicked();
    void on_filter2LPFSet_clicked();
    void on_filter2HPF_clicked();
    void on_filter2Bypass_clicked();
    void on_filter2LPF_clicked();
    void on_filter3HPFSet_clicked();
    void on_filter3LPFSet_clicked();
    void on_filter3HPF_clicked();
    void on_filter3Bypass_clicked();
    void on_filter3LPF_clicked();
    void on_filter4HPFSet_clicked();
    void on_filter4LPFSet_clicked();
    void on_filter4HPF_clicked();
    void on_filter4Bypass_clicked();
    void on_filter4LPF_clicked();
    void onPfDataChanged(quint8 channel);

    /* dyn */
    void on_dyn1_clicked();
    void on_dyn2_clicked();
    void on_dyn3_clicked();
    void on_dyn4_clicked();
    void onDynChanged(quint8 ch, bool isIn ,DynData dynData);

    /* 报警 */
    void setAlarmMode(bool b);
    void setEnableWidgets(int i);
    void setLimiterValue();
    void onShowAlarmDlg();

    /* slider和linetext联动 */
    void setIntValueDisplay(int i);
    void setSliderValue();


    void on_saveAsBtn_clicked();
    void on_loadBtn_clicked();
    void on_saveBtn_clicked();
    void on_pushButton60v_clicked();
    void on_pushButton100v_clicked();

    void on_gain1P_clicked(bool checked);
    void on_gain1M_clicked(bool checked);
    void on_gain2P_clicked(bool checked);
    void on_gain2M_clicked(bool checked);
    void on_gain3P_clicked(bool checked);
    void on_gain3M_clicked(bool checked);
    void on_gain4P_clicked(bool checked);
    void on_gain4M_clicked(bool checked);

    void onAdcDataChanged(AlarmConfig *alarmCfg);

    /* 和monitor界面同步 */
//    void syncVolumeChanged();
//    void syncMuteChanged();
//    void syncPhaseChanged();
    void closeSelf();
private:
    void resetVoltCurrLimit(quint8 volt);
    void connectSigSlots();
    void factoryModeReInit();
    void onPfDataChanged(quint8 ch, bool hfb, bool lfb);
    void saveAlarmSetting();
    void loadAlarmSetting();

private:
    Ui::DevSetting_Monitor_SPK *_ui;
    DeviceItem_Monitor_SPK *_devItem;
    Device_Monitor_SPK *_devAmp;

    bool _syncing;
    bool _hasData;
    QTimer *_timer;
};

#endif
