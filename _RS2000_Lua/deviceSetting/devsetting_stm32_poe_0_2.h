#ifndef DEVSETTING_AMP_H
#define DEVSETTING_AMP_H

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
class DeviceItem_Amp;
class Device_Amp;
namespace Ui {
class DevSetting_Amp;
}
class DevSetting_Amp : public DevSettingDlg
{
    Q_OBJECT
public:
    DevSetting_Amp(DeviceItem_Amp *devItem, QWidget *parent = 0);
    ~DevSetting_Amp();

private slots:
    void displayConfigValue();

    void sliderValueChanged(int value);
    void pushbuttonClicked(bool enable);

    /* eq */
    void onEqChanged(quint8 ch, bool bypass, EqData *eqPara);

    /* dyn */
    void onDynChanged(quint8 ch, bool isIn ,DynData dynData);

private:
    void connectSigSlots();

private:
    Ui::DevSetting_Amp *_ui;
    DeviceItem_Amp *_devItem;
    Device_Amp *_devAmp;
};

#endif
