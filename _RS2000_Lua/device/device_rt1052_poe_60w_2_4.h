#ifndef DEVICE_RT1052_POE_60W_2_4_H
#define DEVICE_RT1052_POE_60W_2_4_H

#include "AES70device.h"
#include "protocal/packet.h"
class AlarmConfig;

class Device_RT1052_Poe_60W_2_4: public AES70Device
{
    Q_OBJECT
public:
    enum {
        AMP_CH1 = 0,
        AMP_CH2,
        AMP_CH3,
        AMP_CH4,
    };

    Device_RT1052_Poe_60W_2_4(QString mac, QString devName, DevType devType, QString devIp, QString creator);
    virtual void serialize(QString fileName);
    virtual void deserialize(QString fileName);
    void sendAllConfigPack(bool b);
public://处理数据包
    virtual void processPacket(PacketBase* pack);

    void sendVolume(quint8 type, quint8 ch, qint8 vol);
    void sendMute(quint8 type, quint8 ch, qint8 vol);
    void sendPhase(quint8 ch, qint8 vol);
    void sendDelay(quint8 ch, quint16 delay);
    void sendEq(quint8 type, quint8 ch, bool bypass, EqData* eqPara);
    void sendDyn(quint8 ch, DynData& dynPara);
    void sendMixer(quint8 outch, quint8 inch, bool enable);
    void sendRoute(quint8 ch, quint8 mode);
    void sendXover(quint8 ch, bool bypass, int low_freq, int high_freq, int low_stages, int high_stages);
    void sendSave(void);
    void sendStartMonitor(void);
    void sendStopMonitor(void);
	
#ifdef Logo_BeiJingHuiSheng
    void sendMixerMode(quint8 mode);
#endif

public://inline
    ConfigData_RT1052_Poe_60W_2_4& getConfigData() {return _configData;}

signals:
    void displaySig(struct MsgData_RT1052_Poe);
    void adcDataChanged(AlarmConfig *);
    void initConfigFromDevice();
    void volumeChanged();
    void muteChanged();
    void phaseChanged();
    void routerChanged();

private:
    ConfigData_RT1052_Poe_60W_2_4 _configData;
    ADC_Data _adcData;
    QTimer *_dispTimer;
};

#endif // DEVICE_RT1052_POE_60W_2_4_H
