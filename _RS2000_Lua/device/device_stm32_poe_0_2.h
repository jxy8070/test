﻿#ifndef DEVICE_AMP_H
#define DEVICE_AMP_H

#include "AES70device.h"
#include "protocal/packet.h"
class AlarmConfig;


class Device_Amp: public AES70Device
{
    Q_OBJECT
public:
    enum {
        AMP_CH1 = 1,
        AMP_CH2 = 2,
    };

    Device_Amp(QString mac, QString devName, DevType devType, QString devIp, QString creator);
    virtual void serialize(QString fileName);
    virtual void deserialize(QString fileName);
    void initData(const ConfigData_STM32_POE_0_2 &configData);
    QString getRouterInfo(quint8 ch);

public://处理数据包
    virtual void processPacket(PacketBase* pack);

    void sendAllConfigPack(bool isNeedDisSuccess);
    void sendVoltTypePack(quint8 voltType);
    void sendSwitch(bool flag);
    void sendVolume(quint8 ch, qint8 vol);
    void sendMute(quint8 ch, bool flag);
    void sendPhase(quint8 ch, bool flag);
    void sendRouter(quint8 ch, quint8 routerCh);
    void sendCommonDataPack(quint8 ch, ControlValue vlu);
    void sendEqPack(quint8 ch, bool bypass, EqData* eqPara);
    void sendDynPack(quint8 ch, DynData& dynPara);
    void sendDividerPack(quint8 ch, DividerData &hf, DividerData &lf);
    void sendSave2DevicePack();
    void sendUpdatePackage(quint32 totalCount, quint32 offset, quint32 paraLen, QByteArray buff);
public://inline
    int isPowerOn() { return 1;}//_configData.Switch;}
    ConfigData_STM32_POE_0_2& getConfigData() {return _configData;}

signals:
    void adcDataChanged(AlarmConfig *);
    void initConfigFromDevice();
    void volumeChanged();
    void muteChanged();
    void phaseChanged();
    void routerChanged();

private:
    ConfigData_STM32_POE_0_2 _configData;
    ADC_Data _adcData;
};

#endif // DEVICE_AMP_H
