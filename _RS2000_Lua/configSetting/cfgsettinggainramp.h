#ifndef CFGSETTINGGAINRAMP_H
#define CFGSETTINGGAINRAMP_H

#include "Protocal/packet.h"

namespace Ui {
class CfgSettingGainRamp;
class CfgSettingGainRamp_FangTu;
}

class ConfigItemGainRamp;
class CfgSettingGainRamp : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingGainRamp(ConfigItemGainRamp *cfgItem, QWidget *parent = 0);
    ~CfgSettingGainRamp();
    void updateUiData();
    void DispRtnData(unsigned int *d);

private slots:
    void on_pushButton_A_clicked(bool checked);
    void on_pushButton_B_clicked(bool checked);
    void on_pushButton_Mute_clicked(bool checked);
    void on_dial_TimeA_valueChanged(int value);
    void on_dial_TimeMute_valueChanged(int value);
    void on_dial_TimeB_valueChanged(int value);
    void on_dial_GainA_valueChanged(int value);
    void on_dial_GainB_valueChanged(int value);

private:
    void initUI();
    void initData();
    void initUI_FangTu();
    void initData_FangTu();
    typedef struct{
        __OPACK32_T1S0(
            OPACKU,rtRampGain,16						//rtRampGain_dbfs = rtRampGain * 0.01 - 100;
        );
    } IG_PARAGET_GAINRAMP;
    Ui::CfgSettingGainRamp *_uiGain = NULL;
    Ui::CfgSettingGainRamp_FangTu *_ui_FangTu = NULL;
    ConfigItemGainRamp *_cfgItem;
};

#endif // CFGSETTINGGAINRAMP_H
