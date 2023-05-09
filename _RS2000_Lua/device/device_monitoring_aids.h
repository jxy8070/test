#ifndef DEVICE_MONITORING_AIDS_H
#define DEVICE_MONITORING_AIDS_H

#include <QObject>
#include "AES70device.h"
#include "protocal/packet.h"
#include "protocal/packetbase.h"

class Device_Monitoring_Aids : public AES70Device
{
    Q_OBJECT
public:
    enum {
        AMP_CH1 = 0,
        AMP_CH2,
        AMP_CH3,
        AMP_CH4,
    };
    Device_Monitoring_Aids(QString mac, QString devName, DevType devType, QString devIp, QString creator);
    virtual void serialize(QString fileName);
    virtual void deserialize(QString fileName);
    virtual void processPacket(PacketBase *pack);

    void sendStartMoniter(void);
    void sendStopMoniter(void);
signals:
    void disPlaySig(MsgData_RT1052_Poe);
    void initConfigFromDevice();
public slots:

private:
    ConfigData_Amp4_4 _configData;
};

#endif // DEVICE_MONITORING_AIDS_H
