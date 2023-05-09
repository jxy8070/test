#ifndef CFGSETTINGFILTE_H
#define CFGSETTINGFILTE_H

namespace Ui {
  class  CfgSettingFilte;
}
class ConfigItemFilte;
struct EqPara;
class CfgSettingFilte : public QWidget
{
    Q_OBJECT
public:
    CfgSettingFilte(ConfigItemFilte *cfgItem,  QWidget *parent = 0);    

    void initUi();
    void updateUiData();
public slots:
    void onSetBypass(bool isBypass);
    void onSetMute(bool is);
    void onSetInvert(bool is);
//    void onTypeChanged(int idx);
    void onFrequencyChanged();
    void onGainChanged();
    void onFilterTypeChanged(int cur);
    void onValueChanged(int idx, int freq, int gain, int q);
    void Bandwidth_ValueChanged(double value);

private:
    Ui::CfgSettingFilte *_ui;
    ConfigItemFilte *_cfgItemFilte;
    EqPara* _eqPara;
};

#endif // CFGSETTINGFILTE_H
