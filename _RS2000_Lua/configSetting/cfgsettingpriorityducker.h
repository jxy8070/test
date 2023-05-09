#ifndef CFGSETTINGPRIORITYDUCKER_H
#define CFGSETTINGPRIORITYDUCKER_H

#include "protocal/packet.h"

namespace Ui {
class CfgSettingPriorityDucker;
class CfgSettingPriorityDucker_FangTu;
}

class ConfigItemPriorityDucker;
class CfgSettingPriorityDucker : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingPriorityDucker(ConfigItemPriorityDucker *cfgItem, QWidget *parent = 0);
    ~CfgSettingPriorityDucker();
    void updateUiData();
    void DispRtnData(unsigned int *d);
private:
    void initUi(void);
    void initUi_FangTu(void);

private slots:
    void on_verticalSlider_InGain_valueChanged(int value);
    void on_verticalSlider_OutGain_valueChanged(int value);
    void on_pushButton_Bypass_clicked(bool checked);
    void on_dial_ThresholdLevel_valueChanged(int value);
    void on_dial_Depth_valueChanged(int value);
    void on_dial_PriorityGain_valueChanged(int value);
    void on_dial_AttackTime_valueChanged(int value);
    void on_dial_HoldTime_valueChanged(int value);
    void on_dial_ReleaseTime_valueChanged(int value);
    void on_dial_DetectorTime_valueChanged(int value);

private:
    typedef struct{
        __OPACK32_T2(
            OPACKU,mainChDuckerAppliedGain,16,			//detectorPriorChInLevel_dbfs = detectorPriorChInLevel * 0.01 - 100;
            OPACKU,detectorPriorChInLevel,16			//detectorPriorChInLevel_dbfs = detectorPriorChInLevel * 0.01 - 100;
        );
    }IG_PARAGET_PDUCKER;

    Ui::CfgSettingPriorityDucker *_uiDuck = NULL;
    Ui::CfgSettingPriorityDucker_FangTu *_ui_FangTu = NULL;
    ConfigItemPriorityDucker *_cfgItem;
};

#endif // CFGSETTINGPRIORITYDUCKER_H
