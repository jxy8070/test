#ifndef CONFIGMETER_FANGTU_H
#define CONFIGMETER_FANGTU_H

#include <QWidget>

namespace Ui {
class ConfigMeter_FangTu;
}

class ConfigMeter_FangTu : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigMeter_FangTu(QWidget *parent = nullptr);
    ~ConfigMeter_FangTu();
    void init(int channel, int holdTime, bool holdEnable, bool infiniteHold, QString name);
    void meterUpdate(double vol, double ppm);

signals:
    void holdTimeChanged(int, int);
    void holdEnableChanged(int, bool);
    void infiniteHoldChanged(int, bool);
    void userNameChanged(int, QString);

private slots:
    void onSpinboxProcess(int value);
    void onPushbuttonProcess(bool checked);
    void onLineEditProcess();

private:
    Ui::ConfigMeter_FangTu *ui;
    int _channel;
};

#endif // CONFIGMETER_FANGTU_H
