#ifndef CFGSETTINGMETER_H
#define CFGSETTINGMETER_H

#include "protocal/packet.h"

class ConfigItemMeter;
class ConfigMeter;
class ConfigMeter_FangTu;
class CfgSettingMeter : public QWidget
{
    Q_OBJECT
public:
    explicit CfgSettingMeter(ConfigItemMeter *cfgItem, QWidget *parent = 0);

    void updateUiData();
    void clearAllVuData();
signals:

public slots:
private slots:
    void holdTimeChanged(int ch, int value);
    void holdEnableChanged(int ch, bool flag);
    void infiniteHoldChanged(int ch, bool flag);
    void userNameChanged(int ch, QString name);
    void dispMeter(FC_RTU_METER *meter);

private:
    void setupUi();
    void setupUi_FangTu(void);

private:
    ConfigItemMeter *_cfgItem;
    QList<ConfigMeter *> _configMeterList;
    QList<ConfigMeter_FangTu *> _configMeterListFangTu;
};

#endif // CFGSETTINGMETER_H
