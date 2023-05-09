#ifndef CFGSETTINGEQ_H
#define CFGSETTINGEQ_H

#include "customWidget/eqviewshg.h"
#include "customWidget/eqviewnormal.h"

class ConfigItemEq;
struct EqData;
enum SGH_Type;
class CfgSettingEq : public QWidget
{
    Q_OBJECT
public:
    CfgSettingEq(ConfigItemEq *cfgItem, QWidget *parent = 0);
    void updateUiData();

private slots:
    void onEqDataChanged(quint8 idx, bool seg_bypass, int freq, int gain, int q, int type);
    void onEqDataChanged(quint8 idx, int gain);
    void onEqDataSegBypass(int idx, bool enable);
    void onEqDataBypassClicked(bool enabled);
    void onEqDataResetClicked(void);

private:
    void setupUiNormal(bool bypass, QList<EqData*> eqList);
    void setupUiShg(EqViewSHG::SHG_Type type, bool bypass, QList<EqData*> eqList);

    ConfigItemEq *_cfgItemEq;
    EqViewNormal * _normalView;
    EqViewSHG * _shgView;
};

#endif // CFGSETTINGEQ_H
