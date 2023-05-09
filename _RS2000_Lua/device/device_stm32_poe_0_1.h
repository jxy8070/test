#ifndef DEVICE_STM32_POE_0_1_H
#define DEVICE_STM32_POE_0_1_H

#include "AES70device.h"
#include "protocal/packet.h"
class AlarmConfig;

class Device_STM32_POE_0_1: public AES70Device
{
    Q_OBJECT
public:
    Device_STM32_POE_0_1(QString mac, QString devName, DevType devType, QString devIp, QString creator);
    virtual void serialize(QString fileName);
    virtual void deserialize(QString fileName);
    void initData(const ConfigData_STM32_POE_0_1 &configData);
    QString getRouterInfo(quint8 ch);

public://处理数据包
    virtual void processPacket(PacketBase* pack);

    void sendVol(int vol);
    void sendMute(bool mute);
    void sendEq(bool bypass, EqData* eqPara);
    void sendDyn(DynData& dynPara);
    void sendSave();
public://inline
    int isPowerOn() { return 0;}//_configData.Switch;}
    ConfigData_STM32_POE_0_1& getConfigData() {return _configData;}

signals:
    void adcDataChanged(AlarmConfig *);
    void initConfigFromDevice();
    void volumeChanged();
    void muteChanged();
    void phaseChanged();
    void routerChanged();

private:
    ConfigData_STM32_POE_0_1 _configData;
    ADC_Data _adcData;
};

#endif // DEVICE_STM32_POE_0_1_H
