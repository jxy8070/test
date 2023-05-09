#ifndef DEVICE_MIC_H
#define DEVICE_MIC_H

#include "AES70device.h"
#include "protocal/packet.h"
class AlarmConfig;


class Device_Mic: public AES70Device
{
    Q_OBJECT
public:
    Device_Mic(QString mac, QString devName, DevType devType, QString devIp, QString creator);
    virtual void serialize(QString fileName);
    virtual void deserialize(QString fileName);
    void initData(const ConfigData_STM32_POE_1_0 &configData);

public://处理数据包
    virtual void processPacket(PacketBase* pack);

    void sendPhantom(uint8_t enable);
    void sendMicGain(int8_t value);
    void sendEq(bool bypass, EqData* eqPara);
    void sendDyn(DynData& dynPara);
    void sendXover(struct ConfigData_Xover xData);
    void sendStartMonitor(void);
    void sendStopMonitor(void);

public://inline
    ConfigData_STM32_POE_1_0& getConfigData() {return _configData;}

signals:
    void displaySig(struct MsgData_STM32_POE);
    void adcDataChanged(AlarmConfig *);
    void initConfigFromDevice();
    void volumeChanged();
    void muteChanged();
    void phaseChanged();
    void routerChanged();

private:
    ConfigData_STM32_POE_1_0 _configData;
    ADC_Data _adcData;
};

#endif // DEVICE_MIC_H
