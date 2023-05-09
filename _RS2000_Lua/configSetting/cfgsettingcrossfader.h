#ifndef CFGSETTINGCROSSFADER_H
#define CFGSETTINGCROSSFADER_H

#include "Protocal/packet.h"

namespace Ui {
class CfgSettingCrossfader;
}

class ConfigItemCrossfader;
class CfgSettingCrossfader : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingCrossfader(ConfigItemCrossfader *cfgItem, QWidget *parent = 0);
    ~CfgSettingCrossfader();
    void updateUiData();
    void DispRtnData(unsigned int *d);

private slots:
    void on_pushButton_A_clicked(bool checked);
    void on_pushButton_B_clicked(bool checked);
    void on_dial_Time_valueChanged(int value);
    void on_comboBox_currentIndexChanged(int index);

private:
    void initData(void);
    typedef struct{
        __OPACK32_T2(
            OPACKU,chBandAGain,16,						//chBandAGain_dbfs = chBandAGain * 0.01 - 100;
            OPACKU,chBandBGain,16						//chBandBGain_dbfs = chBandBGain * 0.01 - 100;
        );
        __OPACK32_T1S0(
            OPACKU,chBandRtPos,16						//chBandRtPos_percent = chBandRtPos * 0.01;
        );
    }IG_PARAGET_CROSSFADER;
    Ui::CfgSettingCrossfader *ui;
    ConfigItemCrossfader *_cfgItem;
};

#endif // CFGSETTINGCROSSFADER_H
