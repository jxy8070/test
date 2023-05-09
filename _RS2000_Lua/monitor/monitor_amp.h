#ifndef MONITOR_AMP_H
#define MONITOR_AMP_H

#include <QGroupBox>

namespace Ui {
class Monitor_Amp;
}
class AlarmConfig;
class Device_Amp;
class DeviceItem;
struct ADC_Data;
enum LimitState;
class Monitor_Amp : public QGroupBox
{
    Q_OBJECT

public:
    explicit Monitor_Amp(Device_Amp* devAmp, DeviceItem* devItem, QWidget *parent = 0);
    ~Monitor_Amp();

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
    Ui::Monitor_Amp *ui;
    Device_Amp* _devAmp;
    DeviceItem* _devItem;
    bool _errorState;
    bool _syncing;
    QTimer *_livingTimer;
    LimitState _stateBak[8];
    bool _alarmEnable;
//    bool m_showFlag;
};

#endif // MONITOR_AMP_H
