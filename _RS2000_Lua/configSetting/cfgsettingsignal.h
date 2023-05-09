#ifndef CFGSETTINGSIGNAL_H
#define CFGSETTINGSIGNAL_H

#include <QDoubleSpinBox>
#include <QPushButton>

namespace Ui {
class cfgsettingsignal_sine;
class cfgsettingsignal_noise;
}

class ConfigItemSignal;
class CfgSettingSignal : public QWidget
{
    Q_OBJECT
public:
    explicit CfgSettingSignal(ConfigItemSignal *cfgItem, QWidget *parent = 0);
    ~CfgSettingSignal();

    void updateUiData();
signals:

public slots:

private slots:
    void processPushbuttonSlot(bool flag);
    void processDoubleSpinBoxSlot(double value);

private:
    void initUi();
    void initData();
    void initSignalConnect();
    void pushButtonSetText(QPushButton *pb, bool flag);

private:
    Ui::cfgsettingsignal_sine *_uiSine = NULL;
    Ui::cfgsettingsignal_noise *_uiNoise = NULL;
    ConfigItemSignal *_cfgItem;
};

#endif // CFGSETTINGSIGNAL_H
