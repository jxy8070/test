#ifndef DEVICE_AMP_MODULE_H
#define DEVICE_AMP_MODULE_H

#include "AES70device.h"
#include "protocal/packet.h"

class AlarmConfig;
class Device_Amp_Module : public AES70Device
{
    Q_OBJECT
public:
    enum{
        AMP_CH1 = 0,
        AMP_CH2,
        AMP_CH3,
        AMP_CH4,
    };
    Device_Amp_Module(QString mac, QString devName, DevType devType, QString devIp, QString creator);
    virtual void serialize(QString fileName);
    virtual void deserialize(QString fileName);
    //on line
    ConfigData_Amp4_4& getConfigData() { return _configData; }

public:
    virtual void processPacket(PacketBase *pack);

    void sendVolume(quint8 type, quint8 ch, qint8 vol);
    void sendMute(quint8 type, quint8 ch, qint8 vol);
    void sendPhase(quint8 ch, qint8 vol);
    void sendDelay(quint8 ch, quint16 delay);
    void sendEq(quint8 type, quint8 ch, bool bypass, EqData *eqPara);
    void sendDyn(quint8 ch, DynData& dynPara);
    void sendMixer(quint8 outch, quint8 inch, bool enable);
    void sendRoute(quint8  ch, quint8 mode);
    void sendXover(quint8 ch, bool bypass, int low_freq,int high_freq, int low_stages, int high_stages);
    void sendSave(void);
    void sendStartMoniter(void);
    void sendStopMoniter(void);

signals:
    void disPlaySig(struct MsgData_RT1052_Poe);
    void adcDataChanged(AlarmConfig *);
    void initConfigFromDevice();

private:
    ConfigData_Amp4_4 _configData;
    ADC_Data _adcData;
    QTimer *_dispTimer;
};

#endif // DEVICE_AMP_MODULE_H
