#ifndef DEVICE_AMP_2_0_MIC_LINE_H
#define DEVICE_AMP_2_0_MIC_LINE_H

#include "AES70device.h"
#include "Protocal/packet.h"
class AlarmConfig;

class Device_Amp_2_0_Mic_Line: public AES70Device
{
    Q_OBJECT
public:
    Device_Amp_2_0_Mic_Line(QString mac, QString devName, DevType devType, QString devIp, QString creator);
    virtual void serialize(QString fileName);
    virtual void deserialize(QString fileName);

public://处理数据包
    virtual void processPacket(PacketBase* pack);
    void sendMicGain(int value);
    void sendMicPhantom(bool enable);
    void sendGain(int type, int ch, int vol);
    void sendMute(quint8 type, quint8 ch, qint8 vol);
    void sendPhase(int ch, bool enable);
    void sendXover(quint8 ch, bool bypass, int low_freq, int high_freq, int low_stages, int high_stages);
    void sendMixer(quint8 outch, quint8 inch, bool enable);
    void sendEq(quint8 ch, bool bypass, EqData* eqPara);
    void sendDyn(quint8 ch, DynData& dynPara);
    void sendSave(void);
    void sendStartMonitor(void);
    void sendStopMonitor(void);

    void sendMic_open(quint8);

public://inline
    ConfigData_Amp_2_0_Mic_Line& getConfigData() {return _configData;}

signals:
    void displaySig(MsgData_Amp_2_0_Mic_Line);
    void adcDataChanged(AlarmConfig *);
    void initConfigFromDevice();
    void volumeChanged();
    void muteChanged();
    void phaseChanged();
    void routerChanged();

private:
    ConfigData_Amp_2_0_Mic_Line _configData;
    ADC_Data _adcData;
};

#endif // DEVICE_AMP_2_0_MIC_LINE_H
