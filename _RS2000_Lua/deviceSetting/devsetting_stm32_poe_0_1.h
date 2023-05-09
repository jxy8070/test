#ifndef DEVSETTING_STM32_POE_0_1_H
#define DEVSETTING_STM32_POE_0_1_H

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
class DeviceItem_STM32_POE_0_1;
class Device_STM32_POE_0_1;
namespace Ui {
class DevSetting_STM32_POE_0_1;
}
class DevSetting_STM32_POE_0_1 : public DevSettingDlg
{
    Q_OBJECT
public:
    DevSetting_STM32_POE_0_1(DeviceItem_STM32_POE_0_1 *devItem, QWidget *parent = 0);
    ~DevSetting_STM32_POE_0_1();

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
    Ui::DevSetting_STM32_POE_0_1 *ui;
    DeviceItem_STM32_POE_0_1 *_devItem;
    Device_STM32_POE_0_1 *_devAmp;
};

#endif /* DEVSETTING_STM32_POE_0_1_H */
