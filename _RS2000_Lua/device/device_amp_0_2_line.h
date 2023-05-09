#ifndef DEVICE_AMP_0_2_LINE_H
#define DEVICE_AMP_0_2_LINE_H

#include "AES70device.h"
#include "protocal/packet.h"

class Device_Amp_0_2_Line: public AES70Device
{
    Q_OBJECT
public:
    Device_Amp_0_2_Line(QString mac, QString devName, DevType devType, QString devIp, QString creator);
    virtual void serialize(QString fileName);
    virtual void deserialize(QString fileName);

public://处理数据包
    virtual void processPacket(PacketBase* pack);
    void sendVolume(quint8 type, quint8 ch, qint8 vol);
    void sendMute(quint8 type, quint8 ch, qint8 vol);
    void sendPhase(quint8 ch, qint8 vol);
    void sendDelay(quint8 ch, quint16 delay);
    void sendEq(quint8 ch, bool bypass, EqData* eqPara);
    void sendDyn(quint8 ch, DynData& dynPara);
    void sendMixer(quint8 outch, quint8 inch, bool enable);
    void sendXover(quint8 ch, bool bypass, int low_freq, int high_freq, int low_stages, int high_stages);
    void sendSave(void);
    void sendStartMonitor(void);
    void sendStopMonitor(void);

public://inline
    ConfigData_Amp_0_2_Line& getConfigData() {return _configData;}


signals:
    void displaySig(struct MsgData_Amp_0_2_Line);
    void adcDataChanged(AlarmConfig *);
    void initConfigFromDevice();
    void volumeChanged();
    void muteChanged();
    void phaseChanged();
    void routerChanged();

private:
    ConfigData_Amp_0_2_Line _configData;
    ADC_Data _adcData;

};

#endif // DEVICE_AMP_0_2_LINE_H
