#include "device_monitoring_aids.h"
#include "monitoring_aids/monitorblackstage.h"
#include <QMessageBox>



Device_Monitoring_Aids::Device_Monitoring_Aids(QString mac, QString devName, DevType devType, QString devIp, QString creator):
    AES70Device(mac, devName, devType, devIp, creator)
{
}

void Device_Monitoring_Aids::serialize(QString fileName)
{
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    QDataStream ds(&f);
    ds << _configData;
    f.close();
}

void Device_Monitoring_Aids::deserialize(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QDataStream ds(&file);

    ConfigData_Amp_0_2_Line configData;
    ds >> configData;
    file.close();
}


void Device_Monitoring_Aids::processPacket(PacketBase *pack)
{
//    switch(pack->getMsgType())
//    {
//    case Msg_Type_HeartbeatAck:
//        _saveState = DevSave_None;
//        break;
//    case Msg_Type_GetInitAck:
//        _configData = pack->getBody_RT1052_Poe().Line_4_4_InitData.getDataPack();
//        if(isAddedToProject()) {
//            emit initConfigFromDevice();
//        }
//        switchStatus(Dev_Online);
//        break;
//    case Msg_Type_iMXRT1052_POE_Monitor_Msg:
//        connect(this, SIGNAL(disPlaySig(MsgData_RT1052_Poe)), MonitorBlackStage::instance(), SLOT(onDisplaySlot(MsgData_RT1052_Poe)));
//        emit disPlaySig(pack->getBody_RT1052_Poe());
//        break;
//    case Msg_Type_iMXRT1052_POE_Save_Ack:
//    {
//        QMessageBox *box = new QMessageBox(QMessageBox::NoIcon, "info", tr("Save Success!"), QMessageBox::Ok);
//        QThread thread;
//        box->moveToThread(&thread);
//        box->show();
//    }
//        break;
//    default:
//        qDebug()<<QString("Unknown packet type%1").arg(pack->getMsgType());
//        break;
//    }
}

void Device_Monitoring_Aids::sendStartMoniter()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Start_Monitor);
}

void Device_Monitoring_Aids::sendStopMoniter()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Stop_Monitor);
}
