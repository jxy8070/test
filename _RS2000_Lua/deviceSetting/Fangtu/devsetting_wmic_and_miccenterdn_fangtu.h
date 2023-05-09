#ifndef DEVSETTING_WMIC_AND_MICCENTERDN_FANGTU_H
#define DEVSETTING_WMIC_AND_MICCENTERDN_FANGTU_H

#include "customWidget/overwriteitem.h"

namespace Ui {
class DevSetting_WMIC_And_MicCenterDN_FangTu;
}

class DeviceItem_Mic_Quad;
class DevSetting_WMIC_And_MicCenterDN_FangTu : public DevSettingDlg
{
    Q_OBJECT
//hrp  添加界面，无功能
public:
    explicit DevSetting_WMIC_And_MicCenterDN_FangTu(DeviceItem_Mic_Quad *devItem, QWidget *parent = nullptr);
    ~DevSetting_WMIC_And_MicCenterDN_FangTu();

    void connectSlots();
    void disconnectSlots();
public slots:
    void onPhaseBtnSlots(bool checked);
    void onGainBtnSlots(double value);
    void onMuteBtnSlots(bool checked);
    void onDynBtnSlots();
    void onLowCutBtnSlots();
    void onEqBtnSlots();
    void onMixerSlot();

private:
    Ui::DevSetting_WMIC_And_MicCenterDN_FangTu *ui;

    DeviceItem_Mic_Quad *_devItem;
};

#endif // DEVSETTING_WMIC_AND_MICCENTERDN_FANGTU_H
