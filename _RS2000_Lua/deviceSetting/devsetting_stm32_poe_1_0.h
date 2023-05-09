#ifndef DEVSETTING_MIC_H
#define DEVSETTING_MIC_H

#include "customWidget/overwriteitem.h"
#include "Protocal/packet.h"


// 功放控制界面
class AlarmConfig;
class DeviceItem_Mic;
class Device_Mic;
namespace Ui {
class DevSetting_Mic;
}
class DevSetting_Mic : public DevSettingDlg
{
    Q_OBJECT
public:
    DevSetting_Mic(DeviceItem_Mic *devItem, QWidget *parent = 0);
    ~DevSetting_Mic();

private slots:
    void displayConfigValue();
    void pbClicked(void);
    void phantomClicked(bool enable);
//    void sliderValueChanged(int value);
    void comboxValueChanged(int value);
    /* eq */
    void onEqChanged(quint8 ch, bool bypass, EqData *eqPara);
    /* dyn */
    void onDynChanged(quint8 ch, bool isIn, DynData dynData);
    /* xover */
    void onXoverChanged(uint8_t ch, struct ConfigData_Xover xData);

    void DisplaySlot(struct MsgData_STM32_POE b);
private:
    void connectSigSlots();

private:
    Ui::DevSetting_Mic *ui;
    DeviceItem_Mic *_devItem;
    Device_Mic *_devAmp;
};

#endif
