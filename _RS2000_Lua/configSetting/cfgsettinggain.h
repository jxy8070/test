#ifndef CFGSETTINGGAIN_H
#define CFGSETTINGGAIN_H

#include "Protocal/packet.h"

namespace Ui {
class CfgSettingGain;
}

class ConfigItemGain;
class CfgSettingGain : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingGain(ConfigItemGain *cfgItem, QWidget *parent = 0);
    ~CfgSettingGain();
    void updateUiData();
    void DispRtnData(unsigned int *d);

private slots:
    void pbClickedSlot(bool checked);
    void processDoubleSpinBoxSlot(double gain);

private:
    void initData(void);

    Ui::CfgSettingGain *ui;
    ConfigItemGain *_cfgItem;
};

#endif // CFGSETTINGGAIN_H
