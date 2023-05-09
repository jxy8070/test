#ifndef MONITOR_SPK_H
#define MONITOR_SPK_H

#include <QGroupBox>


struct ADC_Data;
class AlarmConfig;
class Device_Spk;
class DeviceItem;
enum LimitState;
namespace Ui {
class Monitor_Spk;
}
class Monitor_Spk : public QGroupBox
{
    Q_OBJECT
public:
    explicit Monitor_Spk(Device_Spk *devSpk, DeviceItem *devItem, QWidget *parent = 0);
    ~Monitor_Spk();

protected:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);

private slots:
    void processPbClicked(bool flag);
    void onAdcDataChanged(AlarmConfig* alarmCfg);
    void setIntValueDisplay(int i);
    void setSliderValue();

    void syncVolumeChanged();
    void syncMuteChanged();
    void syncPhaseChanged();
    void syncRouterChanged();

    void unLivingDectect();
private:
    void initData();
    void pushButtonSetText(QPushButton *pb, bool flag);

private:
    Ui::Monitor_Spk *ui;
    Device_Spk *_devSpk;
    DeviceItem* _devItem;
    bool _errorState;
    bool _syncing;
    QTimer *_livingTimer;
    LimitState _stateBak[4];
    bool _alarmEnable;
};

#endif // MONITOR_SPK_H
