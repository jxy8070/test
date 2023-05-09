#ifndef DEVSETTING_SPK_1_H
#define DEVSETTING_SPK_1_H

#include <QDialog>
#include "protocal/packet.h"


class DeviceItem_Spk;
class Device_Spk_1;
class AlarmConfig;
namespace Ui {
class DevSetting_Spk;
}
/////////////////////////////////////////////
/// \brief DevSetting_Spk_1 音箱控制界面
/////////////////////////////////////////////
class DevSetting_Spk_1 : public QDialog
{
    Q_OBJECT
public:
    DevSetting_Spk_1(DeviceItem_Spk *devItem, QWidget *parent = 0);
    ~DevSetting_Spk_1();

protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    void displayConfigData();

    /* 路由 */
    void on_router_clicked();

    /* eq */
    void on_eq_clicked();
    void onEqChanged(quint8 ch, bool bypass, EqData *eqPara);

    /* filter */
    void on_dividerHF_clicked();
    void on_dividerLF_clicked();
    void onPfDataChanged(quint8 channel);

    /* dyn */
    void on_dyn_clicked();
    void onDynChanged(quint8 ch, bool isIn ,DynData dynData);

    /* 报警 */
    void setAlarmMode(bool b);
    void setEnableWidgets(int i);
    void setLimiterValue();
    void on_alarmButton_clicked();

    /* slider和linetext联动 */
    void setIntValueDisplay(int i);
    void setSliderValue();

    void on_loadButton_clicked();
    void on_saveAsButton_clicked();
    void save2Device();
    void on_pushButton60v_clicked();
    void on_pushButton100v_clicked();

    void on_gain1P_clicked(bool checked);
    void on_gain1M_clicked(bool checked);

    void onAdcDataChanged(AlarmConfig *data);

    void closeSelf();
signals:

private:
    void resetVoltCurrLimit(quint8 voltType);
    void connectSigSlots();
    void factoryModeReInit();
    void onPfDataChanged(quint8 ch, bool hfb, bool lfb);
    void saveAlarmSetting();
    void loadAlarmSetting();

private:
    Ui::DevSetting_Spk *_ui;
    DeviceItem_Spk *_devItem;
    Device_Spk_1 *_devSpk;

    bool _syncing;

    bool _hasData;
    QTimer *_timer;
};
#endif // DEVSETTING_SPK_1_H

