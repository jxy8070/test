#ifndef DEVICE_MIC_QUAD_H
#define DEVICE_MIC_QUAD_H

#include "AES70device.h"
#include "protocal/packet.h"
class AlarmConfig;

class Device_Mic_Quad: public AES70Device
{
    Q_OBJECT
public:
    Device_Mic_Quad(QString mac, QString devName, DevType devType, QString devIp, QString creator);
    virtual void serialize(QString fileName);
    virtual void deserialize(QString fileName);
    inline void setGroup(qint16 group) { _group =group; }

public://处理数据包
    virtual void processPacket(PacketBase* pack);

    void sendMicGain(int ch, int value);
    void sendMicPhantom(int ch, bool enable);
    void sendGain(int type, int ch, int vol);
    void sendMute(quint8 type, quint8 ch, qint8 vol);
    void sendPhase(int ch, bool enable);
    void sendXover(quint8 ch, bool bypass, int high_freq, int high_stages);
    void sendMixer(quint8 outch, quint8 inch, bool enable);
    void sendEq(quint8 ch, bool bypass, EqData* eqPara);
    void sendDyn(quint8 ch, DynData& dynPara);
    void sendSave(void);
    void sendStartMonitor(void);
    void sendStopMonitor(void);
    void sendGroup(int startPort, int endPort);
    void sendAFCClear(void);

public://inline
//    int isPowerOn() { return _configData.Switch;}
    ConfigData_Mic_Quad& getConfigData() {return _configData;}

signals:
    void displaySig(MsgData_Mic_Quad);
    void adcDataChanged(AlarmConfig *);
    void initConfigFromDevice();
    void volumeChanged();
    void muteChanged();
    void phaseChanged();
    void routerChanged();

private:
    ConfigData_Mic_Quad _configData;
    ADC_Data _adcData;
    qint16 _group;
};

#endif // DEVICE_AMP_QUAD_H
