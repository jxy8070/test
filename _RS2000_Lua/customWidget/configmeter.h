#ifndef CONFIGMETER_H
#define CONFIGMETER_H

#include <QGroupBox>
namespace Ui {
class ConfigMeter;
}

class ConfigMeter : public QGroupBox
{
    Q_OBJECT

public:
    explicit ConfigMeter(QWidget *parent = 0);
    ~ConfigMeter();
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
    Ui::ConfigMeter *ui;
    int _channel;
};

#endif // CONFIGMETER_H
