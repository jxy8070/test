#ifndef CFGSETTINGDYN_H
#define CFGSETTINGDYN_H

#include <QPushButton>

namespace Ui {
class cfgsettingdyn;

class cfgsettingdyn_Compressor_FangTu;
class cfgsettingdyn_Leveler_FangTu;
class cfgsettingdyn_Expander_FangTu;
class cfgsettingdyn_Limiter_FangTu;
class cfgsettingdyn_NoiseGate_FangTu;
}

class ConfigItemDyn;
class CfgSettingDyn : public QWidget
{
    Q_OBJECT
public:
    explicit CfgSettingDyn(ConfigItemDyn *cfgItem, QWidget *parent = 0);
    void setMeterDisp(float i, float o);

    void updateUiData();
signals:

public slots:

private slots:
    void processSilderSlot(double value);
    void processSilderSlot_FangTu(double value);
    void processPushButtonSlot(bool flag);

private:
    void initUi();
    void initData();
    void initSignalConnect();

    void initUi_FangTu(void);
    void initData_FangTu(void);
    void initSignalConnect_FangTu(void);

private:
    ConfigItemDyn *_cfgItem;
    Ui::cfgsettingdyn* _uiDyn;
    Ui::cfgsettingdyn_Compressor_FangTu *_uiCompressor_FangTu;
    Ui::cfgsettingdyn_Leveler_FangTu *_uiLeveler_FangTu;
    Ui::cfgsettingdyn_Expander_FangTu *_uiExpander_FangTu;
    Ui::cfgsettingdyn_Limiter_FangTu *_uiLimiter_FangTu;
    Ui::cfgsettingdyn_NoiseGate_FangTu *_uiNoiseGate_FangTu;
};

#endif // CFGSETTINGDYN_H
