#ifndef CFGSETTINGDELAY_H
#define CFGSETTINGDELAY_H

#include <QPushButton>
#include <QDoubleSpinBox>


class ConfigItemDelay;
class ConfigGainWidget;

class CfgSettingDelay : public QWidget
{
    Q_OBJECT
public:
    CfgSettingDelay(ConfigItemDelay *cfgItem, QWidget *parent = 0);

    void updateUiData();
signals:

public slots:
    void onDelayChanged(int delay);
    void onBypassChanged(bool bypass);
    void onDistanceChanged(double m);
private:
    void setupUi();

private:
    QPushButton *_btnBypass;
    ConfigGainWidget *_spinDelay;
    ConfigItemDelay *_cfgItemDelay;
    ConfigGainWidget *_spinDistance;
    QString _units;
};

#endif // CFGSETTINGDELAY_H
