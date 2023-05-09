#include "stable.h"
#include "AES70device.h"
#include "devicemanager.h"
#include "devicemodel.h"
#include "protocal/packetbase.h"
#include "device_trn.h"
#include "device_stm32_poe_0_2.h"
#include "device_stm32_poe_0_1.h"
#include "device_amp_quad.h"
#include "device_amp_dn8000.h"
#include "device_stm32_poe_1_0.h"
#include "device_spk.h"
#include "device_monitor_spk.h"
#include "device_mic_quad.h"
#include "device_amp_0_2_line.h"
#include "device_amp_2_0_mic_line.h"
#include "danteSdk/dantebrowser.h"
#include "device_rt1052_poe_60w_2_4.h"
#include "device_rt1052_poe_AP60_Bluetooth.h"
#include "device_amp_module.h"
#include "device_monitoring_aids.h"
#include "device_rt1052_AP400.h"

#include "Protocal/firmwareudpserver.h"

MfrsType AES70Device::_mfrsType = Mfrs_UNKNOWN;

AES70Device* AES70Device::createDevice(QString mac, QString devName, DevType devType, QString devIp, QString creator)
{
    AES70Device* device = NULL;
    switch(devType)
    {
    case Dev_Type_NAP_Zynq7020_16Analog_16AES67:
    case Dev_Type_NAP_Zynq7020_16Analog_48AES67:
    case Dev_Type_NAP_Zynq7020_64AES67:
    case Dev_Type_NAP_Zynq7020_16Analog_16AES67DanteMixer:
    case Dev_Type_NAP_Zynq7020_16Analog_16Dante:
    case Dev_Type_NAP_Zynq7020_16Analog_16Dante_Type1:
    case Dev_Type_NAP_Zynq7020_16Analog_32Dante:
    case Dev_Type_NAP_X86_16Analog_128AES67:
    case Dev_Type_NAP_X86_32Analog_256AES67:
    case Dev_Type_NAP_FT2000_256AES67:
    case Dev_Type_NAP_STM32H750_8A8N:
        device= new Device_Trn(mac, devName, devType, devIp, creator);
        break;
    case Dev_Type_AES67_SPK:
    case Dev_Type_DNTE_SPK:
        device= new Device_Spk(mac, devName, devType, devIp, creator);
        break;
    case Dev_Type_STM32_POE_AES67_SPK_0_2:
        device = new Device_Amp(mac, devName, devType, devIp, creator);
        break;
    case Dev_Type_STM32_POE_AES67_SPK_0_1:
        device = new Device_STM32_POE_0_1(mac, devName, devType, devIp, creator);
        break;
    case Dev_Type_RT1052_POE_AES67_LINE_4_4:
        device = new Device_Amp_Quad(mac, devName, devType, devIp, creator);
        break;        
    case Dev_Type_RT1052_AMPLIFIER_AA4500:
    case Dev_Type_RT1052_AMPLIFIER_AA4500_Type1:
    case Dev_Type_RT1052_AMPLIFIER_AA2500:
        device = new Device_Amp_Module(mac, devName, devType, devIp, creator);
        break;        
    case Dev_Type_RT1052_Amplifier_DongFangJiaLian:
        device = new Device_Amp_DN8000(mac, devName, devType, devIp, creator);
        break;
    case Dev_Type_RT1052_POE_AES67_MIC_2_0:
    case Dev_Type_RT1052_POE_AES67_MIC_2_0_Type1:
    case Dev_Type_RT1052_POE_AES67_MIC_2_0_Type2:
    case Dev_Type_RT1052_POE_AES67_MIC_2_0_Type3:
        device = new Device_Amp_2_0_Mic_Line(mac, devName, devType, devIp, creator);
        break;
    case Dev_Type_RT1052_POE_AES67_LINE_0_2:
        device = new Device_Amp_0_2_Line(mac, devName, devType, devIp, creator);
        break;
    case Dev_Type_RT1052_AP60:
    case Dev_Type_RT1052_AP60_Type1:
    case Dev_Type_RT1052_AP60_Type2:
        device = new Device_RT1052_Poe_60W_2_4(mac, devName, devType, devIp, creator);
        break;
    case Dev_Type_RT1052_AP60_Bluetooth:
        device = new Device_RT1052_Poe_AP60_Bluetooth(mac, devName, devType, devIp, creator);
        break;
    case Dev_Type_RT1052_AP400:
        device = new Device_RT1052_AP400(mac, devName, devType, devIp, creator);
        break;
    case Dev_Type_STM32_POE_AES67_MIC_2_0:
        device = new Device_Mic(mac, devName, devType, devIp, creator);
        break;
    case Dev_Type_RT1052_POE_AES67_MIC_4_0:
        //FT暂时
    case Dev_Type_FionTu_WMIC_FangTu:
    case Dev_Type_I_Mic_Center_DN_FangTu:
        device = new Device_Mic_Quad(mac, devName, devType, devIp, creator);
        break;
    case Dev_Type_VirtualSoundCard_Dante:
    case Dev_Type_VirtualSoundCard_AES67:
    case Dev_Type_Zynq7010_AES3ToAES67_DongFangJiaLian:
    case Dev_Type_RT1052_DR3020:
        device =  new AES70Device(mac, devName, devType, devIp, creator);
        break;
    ///////////////////monitoring_aids////////////////////
    case Dev_Type_RT1052_MONITORING_AIDS:
        device = new Device_Monitoring_Aids(mac, devName, devType, devIp, creator);
        break;
    case Dev_Type_AES67_MONITOR_SPK:
        device = new Device_Monitor_SPK(mac, devName, devType, devIp, creator);
        break;
    default:
        break;
    }
//        qDebug() << __FUNCTION__ << __LINE__ << mac << devType << customName << devDisp;
    return device;
}

void AES70Device::saveAlarmSetting()
{
    if(_alarmCfg != NULL){
        _alarmCfg->saveAlarmSetting();
    }
}

void AES70Device::loadAlarmSetting()
{
    if(_alarmCfg != NULL){
        _alarmCfg->loadAlarmSetting();
    }
}

void AES70Device::clearAlarmSetting()
{
    if(_alarmCfg != NULL){
        _alarmCfg->clearAlarmSetting();
    }
}


QString AES70Device::getStatusName(DevStatus type)
{
    switch(type)
    {
    case Dev_Offline: return tr("Offline");
    case Dev_DanteFound: return tr("Dante Found");
    case Dev_AES70Found: return tr("AES70 Found");
    case Dev_Initialized: return tr("Initialized");
    case Dev_Online: return tr("Online");
    default: return tr("Unknow");
    }
}


AES70Device::AES70Device(QString mac, QString devName, DevType devType, QString devIp, QString creator)
{
    _AES70Mac = mac.toLower();
    _deviceType = devType;
    _AES70IP = devIp;
    if(!devName.isEmpty()) _deviceName = devName;
    else _deviceName = mac;
    _devCreator = creator;
    if(creator =="Dante"){
        _danteIP = devIp;
        _danteName = devName;
    }
    _msgIdx = 0;
    _status = Dev_Offline;
}

void  AES70Device::updateAES70Info(QString devName, QString devIp, QString creator)
{
    if(_AES70IP != devIp) _AES70IP = devIp;
    if(_deviceName != devName) _deviceName = devName;;
    if(_devCreator != creator) _devCreator = creator;
    if(creator =="Dante"){
        _danteIP = devIp;
        _danteName = devName;
    }
}

void AES70Device::setUpdateTime()
{
//    qDebug()<<"DanteDevice::setUpdateTime";
    _lastUpdateTime = QDateTime::currentDateTime();
}

#include "Frame/mainwindow.h"
#include <QtConcurrent/QtConcurrent>
void AES70Device::setDeviceName(QString newName, bool setToDev)
{
    QRegExp regExp("^[0-9A-Za-z-]+$");//限制输入：允许大小写字母，数字，'-'
    if(!regExp.exactMatch(newName)) {
        MainWindow::instance()->showNotice(tr("The new name has illegal characters"));
        return ;
    }
    if(_deviceName == newName) return;

    if(!setToDev){
        _deviceName = newName;
    }
    else if(isOnline()) {
        bool isSucceed = DeviceManager::instance()->setDeviceName(this, newName);//采用AES70协议
        if(isSucceed) {
            MainWindow::instance()->showNotice(tr("The device name is set successfully!"));
            _deviceName = newName;
            DeviceManager::instance()->getDeviceModel()->updateDevice(this);
            emit deviceNameChanged(newName);
        }else {
            MainWindow::instance()->showNotice(tr("Failed to synchronize the device name!"));
        }
    }
    else {
        MainWindow::instance()->showNotice(tr("This Device is offline!") +
                                           "\r\n" + _deviceName);
    }
}

void AES70Device::setCurRoomName(QString roomName)
{
    _curRoomName = roomName;
    DeviceManager::instance()->getDeviceModel()->updateDevice(this);
}

void AES70Device::setOffline()
{
#ifdef DanteSupported
    DeviceManager::instance()->getDanteBrowser()->removeDevice(_deviceName);
#endif
    _lastUpdateTime = QDateTime::currentDateTime();
    switchStatus(Dev_Offline);
    DeviceManager::instance()->removeofflineAES70Device(this->getAES70Mac());
}

PlatformType AES70Device::getPlatformType()
{
    return Utils::getPlatformType(_deviceType);
}

bool AES70Device::is16BitProtocal()
{
    if(getPlatformType() == Platform_X86){
        return true;
    }
    else if(getPlatformType() == Platform_ARM){  //wj2 ARM暂时用8bit协议
        return false;
    }
    else{
        return false;
    }
}

int AES70Device::maxConfigLayer()
{
    if(getPlatformType() == Platform_X86){ //支持ARM
        return 128;
    }
    else{
        return 16;
    }
}

void AES70Device::confirmOnline(int confirmSecond)
{
    int second = -1 * 4 * confirmSecond;
    QDateTime offlineTime = QDateTime::currentDateTime().addSecs(second);
//    qDebug() << __FUNCTION__ << __LINE__ << _status << _AES70IP
//             << offlineTime << _lastUpdateTime;
    if(_deviceType != Dev_Type_VirtualSoundCard_Dante &&
            _deviceType != Dev_Type_VirtualSoundCard_AES67 &&
            _lastUpdateTime < offlineTime)
    {
        //处理离线，并更新状态
        setOffline();
    } else {
        //发送初始化包或发送心跳包
        if(_status != Dev_Online)
        {
            sendInitPack();
        } else {
            /* 取消心跳包，通过每个设备实时接收的数据来检测设备是否在线，10s以内的心跳包会导致AMP的SAVE命令无法获得save success响应 */
            sendHeartbeatPack();
        }
    }
}

QString AES70Device::getMfrsName(MfrsType mfrsType)
{
    switch(mfrsType)
    {
    case Mfrs_Fitcan:
        return tr("Fitcan");
    case Mfrs_FangTu:
        return "FionTu";
    case Mfrs_KEMU:
        return "KEMU";
    case Mfrs_ControlMax:
        return "ControlMax";
    case Mfrs_GuangZhouShengTuo:
        return "GuangZhouShengTuo";
    case Mfrs_Hushan:
        return "Hushan";
    case Mfrs_BeiJingHuiSheng:
        return "BeiJingHuiSheng";
    case Mfrs_GuRui:
        return "GuRui";
    case Mfrs_YiChuang:
        return "YiChuang";
    default:
        return "unknown";
    }
}

void AES70Device::processPacket(PacketBase* pack)
{
    Q_UNUSED(pack)
//    Q_ASSERT(false); //调用错误， 只有确定类型才能处理数据包
}

void AES70Device::sendInitPack()
{
    sendPacket_Nobody(Msg_Type_GetInit);
}

void AES70Device::sendHeartbeatPack()
{
    sendPacket_Nobody(Msg_Type_Heartbeat);
}

void AES70Device::serialize(QString fileName)
{
    Q_UNUSED(fileName)
    Q_ASSERT(false); //调用错误， 只有确定类型才能序列化
}

void AES70Device::deserialize(QString fileName)
{
    Q_UNUSED(fileName)
    Q_ASSERT(false); //调用错误， 只有确定类型才能序列化
}


QString AES70Device::getDeviceIp() const
{
    return _AES70IP;
}

void AES70Device::setDeviceIp(QString ipAddress)
{
    if(_AES70IP != ipAddress) {
        _AES70IP = ipAddress;
    }
}

bool AES70Device::sendPacket_Nobody(MsgType msgType)
{
    PacketBase *pack = PacketBase::createPacket_Nobody(msgType , getMsgIdx());
    DeviceManager::instance()->sendPacket(this, pack);
    return true;
}


bool AES70Device::sendPacket(MsgType msgType, QByteArray& msgData)
{
    PacketBase *pack = PacketBase::createPacket_Withbody(msgType , getMsgIdx(), msgData);
    DeviceManager::instance()->sendPacket(this, pack);
    return true;
}

bool AES70Device::sendPacketSync_Nobody(MsgType msgType, int waitMs)
{
    int msgIndex = getMsgIdx();
    PacketBase *pack = PacketBase::createPacket_Nobody(msgType, msgIndex);
    quint16 msgTypeResp = msgType;
    quint16 msgIndexResp = msgIndex;
    return DeviceManager::instance()->sendPacketSync(this, pack, waitMs, msgTypeResp, msgIndexResp);
}

bool AES70Device::sendPacketSync(MsgType msgType, QByteArray& msgData, int waitMs)
{
    int msgIndex = getMsgIdx();
    PacketBase *pack = PacketBase::createPacket_Withbody(msgType , msgIndex, msgData);
    quint16 msgTypeResp = msgType;
    quint16 msgIndexResp = msgIndex;
    return DeviceManager::instance()->sendPacketSync(this, pack, waitMs, msgTypeResp, msgIndexResp);
}


bool AES70Device::switchStatus(DevStatus newStatus)
{
    bool isChanged = false;
//    qDebug() << "DanteDevice::switchStatus" << _AES70IP
//             << _status << newStatus;
    if(_status == newStatus) {
        return isChanged;
    }

    switch(newStatus)
    {
    case Dev_Offline:
    case Dev_Online:
        _status = newStatus;
        isChanged = true;
        break;
    case Dev_DanteFound:
    case Dev_AES70Found:
        if(_status == Dev_Offline){
            _status = newStatus;
            isChanged = true;
        }
        break;
    case Dev_Initialized:
        if(_status != Dev_Online) {
            _status = newStatus;
            isChanged = true;
        }
        break;
     default:
        Q_ASSERT(false);
    }

    if(isChanged){
        DeviceManager::instance()->getDeviceModel()->updateDevice(this);
        emit deviceStatusChanged(_status);
    }
    return isChanged;
}





