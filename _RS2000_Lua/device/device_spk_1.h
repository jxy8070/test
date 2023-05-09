#ifndef DEVICE_SPK_1_H
#define DEVICE_SPK_1_H

#include "dantedevice.h"
#include "protocal/packet.h"

class AlarmConfig;
class Device_Spk_1 : public DanteDevice
{
    Q_OBJECT
public:
    Device_Spk_1(const DanteDevice* danteDev);
    Device_Spk_1(QString mac, DevType devType, QString customName, QString serial);
    virtual void serialize(QString fileName);
    virtual void deserialize(QString fileName);
    void initData(const ConfigData_Spk& configData);
    QString getRouterInfo();
    AlarmConfig *getAlarmCfg() {return _alarmCfg;}
    bool isFactoryMode() { return _factoryMode; }
    void changeFactoryMode() { _factoryMode = !_factoryMode; }

public://处理数据包
    virtual void processPacket(PacketBase* pack);
    virtual void sendInitPack();
    virtual void sendHeartbeatPack();
    virtual void saveAlarmSetting();
    virtual void loadAlarmSetting();
    virtual void clearAlarmSetting();
    void sendAllConfigPack(bool isNeedDisSuccess);
    void sendVoltTypePack(quint8 voltType);
    void sendSwitch(bool flag);
    void sendVolume(qint8 vol);
    void sendMute(bool flag);
    void sendPhase(bool flag);
    void sendRouter(quint8 routerCh);
    void sendCommonDataPack();
    void sendEqPack(quint8 ch, bool bypass, EqData *eqPara);
    void sendDynPack(quint8 ch, DynData &data);
    void sendDividerPack(quint8 ch, DividerData &hf, DividerData &lf);
    void sendSave2DevicePack();
    void sendUpdatePackage(quint32 totalCount, quint32 offset, quint32 paraLen, QByteArray buff);
signals:
    void adcDataChanged(AlarmConfig *);
    void initConfigFromDevice();
    void volumeChanged();
    void muteChanged();
    void phaseChanged();
    void routerChanged();

public://inline
    int isPowerOn() { return _configData.Switch;}
    ConfigData_Spk& getConfigData() {return _configData;}

private:
    ConfigData_Spk _configData;
    ADC_Data _adcData;
    AlarmConfig *_alarmCfg;

    bool _factoryMode;
    DevSaveState _saveState;
};


#endif // DEVICE_SPK_1_H
