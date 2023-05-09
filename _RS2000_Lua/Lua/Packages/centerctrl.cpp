#include "centerctrl.h"
#include "config.h"

//////////////////////////////////////////////////////////////////
/// \brief UdpGram
//////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const UdpGram& gram)
{
    stream.writeRawData((const char*)&gram, sizeof(UdpGram));
    return stream;
}

QDataStream& operator>>(QDataStream& stream, UdpGram& gram)
{
    stream.readRawData((char*)&gram, sizeof(UdpGram));
    return stream;
}

UdpGram::UdpGram()
{
    memset(this, 0, sizeof(UdpGram));
}

UdpGram::UdpGram(const QString itemName, quint32 cmdKey)
{
    memset(this, 0, sizeof(UdpGram));
    QByteArray nameBa = itemName.toLatin1();
    int nameLen = nameBa.length();
    if(nameLen > 8)  nameLen = 8;
    memcpy(this->name, nameBa.data(), nameLen);
    this->cmdKey = qToBigEndian(cmdKey);
}

quint32 UdpGram::getCmdKey()
{
    return qFromBigEndian(cmdKey);
}

void UdpGram::setUint16(quint16 value, quint8 index)
{
    quint16* pGain = (quint16*)(data+index);
    *pGain = qToBigEndian(value);
}

quint16 UdpGram::getUint16(quint8 index)
{
    quint16* pGain = (quint16*)(data+index);
    quint16 iGain = qFromBigEndian(*pGain);
    return iGain;
}

void UdpGram::setGain(double gain, quint8 index)
{
    quint16 iGain = (quint16) ((gain + 100) * 100);
    quint16* pGain = (quint16*)(data+index);
    *pGain = qToBigEndian(iGain);
}

double UdpGram::getGain(quint8 index)
{
    quint16* pGain = (quint16*)(data+index);
    quint16 iGain = qFromBigEndian(*pGain);
    return iGain / 100.0  -100;
}



////////////////////////////////////////////////////////
/// \brief Singleton模式相关代码
QMutex CenterCtrl::Mutex;//实例互斥锁。
CenterCtrl* CenterCtrl::_Instance = NULL; //<使用原子指针,默认初始化为0。

CenterCtrl* CenterCtrl::instance()
{
    //使用双重检测。
    if(!_Instance)//第一次检测
    {
        QMutexLocker locker(&Mutex);//加互斥锁。

        if(!_Instance)//第二次检测。
            _Instance = new CenterCtrl();
    }

    return _Instance;
}


//////////////////////////////////////////////////////////////////
/// \brief ControlCommand
//////////////////////////////////////////////////////////////////
CenterCtrl::CenterCtrl()
{
    _udpDevice = new QUdpSocket();
    QString localIP = "";
    bool rs = _udpDevice->bind(QHostAddress(localIP), 0, QUdpSocket::ShareAddress);

}

void CenterCtrl::setDevAddress(QString devIp)
{
    QHostAddress newAddress(devIp);
    if(_devIpAddress != newAddress){
        _devIpAddress = newAddress;
    }
}

bool CenterCtrl::getCurSceneNo(int& sceneNo)
{
    UdpGram sendGram("SCENE", 0xFFFF0002);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        sceneNo = recvGram.getUint8(0);
        return true;
    }
    return false;
}

//================输入模块================//
bool CenterCtrl::getInputAntiPhase(const QString& itemName, quint8 chNo, bool& antiPhase)
{
    UdpGram sendGram(itemName, 0x00000001);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        antiPhase = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setInputAntiPhase(const QString& itemName, quint8 chNo, bool antiPhase)
{
    UdpGram sendGram(itemName, 0x00000002);
    sendGram.setUint8(chNo, 0);
    sendGram.setBool(antiPhase, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getInputMute(const QString& itemName, quint8 chNo, bool& isMute)
{
    UdpGram sendGram(itemName, 0x00000003);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        isMute = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setInputMute(const QString& itemName, quint8 chNo, bool isMute)
{
    UdpGram sendGram(itemName, 0x00000004);
    sendGram.setUint8(chNo, 0);
    sendGram.setBool(isMute, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getInputGain(const QString& itemName, quint8 chNo, double& gain)
{
    UdpGram sendGram(itemName, 0x00000005);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setInputGain(const QString& itemName, quint8 chNo, double gain)
{
    UdpGram sendGram(itemName, 0x00000006);
    sendGram.setUint8(chNo, 0);
    sendGram.setGain(gain, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

//================输入模块================//
bool CenterCtrl::getOutputAntiPhase(const QString& itemName, quint8 chNo, bool& antiPhase)
{
    UdpGram sendGram(itemName, 0x00010001);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        antiPhase = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setOutputAntiPhase(const QString& itemName, quint8 chNo, bool antiPhase)
{
    UdpGram sendGram(itemName, 0x00010002);
    sendGram.setUint8(chNo, 0);
    sendGram.setBool(antiPhase, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getOutputMute(const QString& itemName, quint8 chNo, bool& isMute)
{
    UdpGram sendGram(itemName, 0x00010003);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        isMute = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setOutputMute(const QString& itemName, quint8 chNo, bool isMute)
{
    UdpGram sendGram(itemName, 0x00010004);
    sendGram.setUint8(chNo, 0);
    sendGram.setBool(isMute, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getOutputGain(const QString& itemName, quint8 chNo, double& gain)
{
    UdpGram sendGram(itemName, 0x00010005);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setOutputGain(const QString& itemName, quint8 chNo, double gain)
{
    UdpGram sendGram(itemName, 0x00010006);
    sendGram.setUint8(chNo, 0);
    sendGram.setGain(gain, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

//================延时模块================//
bool CenterCtrl::getDelayBypass(const QString& itemName, bool& bypass)
{
    UdpGram sendGram(itemName, 0x00020001);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        bypass = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDelayBypass(const QString& itemName, bool bypass)
{
    UdpGram sendGram(itemName, 0x00020002);
    sendGram.setBool(bypass, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

//wj 高精度和普通精度时间需要处理
bool CenterCtrl::getDelayTime(const QString& itemName, double& ms, double& maxMs)
{
    UdpGram sendGram(itemName, 0x00020003);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ms = recvGram.getUint16(0);
        maxMs = recvGram.getUint16(2);
        return true;
    }
    return false;
}

bool CenterCtrl::setDelayTime(const QString& itemName, double ms)
{
    UdpGram sendGram(itemName, 0x00020004);
    sendGram.setUint16(ms, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}


//================路由模块================//
bool CenterCtrl::getRouterOutput(const QString& itemName, quint8 chNo, quint8& inCh)
{
    UdpGram sendGram(itemName, 0x00030001);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        inCh = recvGram.getUint8(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setRouterOutput(const QString& itemName, quint8 chNo, quint8 inCh)
{
    UdpGram sendGram(itemName, 0x00030002);
    sendGram.setUint8(chNo, 0);
    sendGram.setUint8(inCh, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

//================混音模块================//
bool CenterCtrl::getMixerInputState(const QString& itemName, quint8 chNo, bool& state)
{
    UdpGram sendGram(itemName, 0x00040001);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        state = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setMixerInputState(const QString& itemName, quint8 chNo, bool state)
{
    UdpGram sendGram(itemName, 0x00040002);
    sendGram.setUint8(chNo, 0);
    sendGram.setBool(state, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getMixerInputGain(const QString& itemName, quint8 chNo, double& gain)
{
    UdpGram sendGram(itemName, 0x00040003);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setMixerInputGain(const QString& itemName, quint8 chNo, double gain)
{
    UdpGram sendGram(itemName, 0x00040004);
    sendGram.setUint8(chNo, 0);
    sendGram.setGain(gain, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getMixerOutputState(const QString& itemName, quint8 chNo, bool& state)
{
    UdpGram sendGram(itemName, 0x00040005);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        state = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setMixerOutputState(const QString& itemName, quint8 chNo, bool state)
{
    UdpGram sendGram(itemName, 0x00040006);
    sendGram.setUint8(chNo, 0);
    sendGram.setBool(state, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getMixerOutputGain(const QString& itemName, quint8 chNo, double& gain)
{
    UdpGram sendGram(itemName, 0x00040007);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setMixerOutputGain(const QString& itemName, quint8 chNo, double gain)
{
    UdpGram sendGram(itemName, 0x00040008);
    sendGram.setUint8(chNo, 0);
    sendGram.setGain(gain, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getMixerNodeState(const QString& itemName, quint8 chIn, quint8 chOut, bool& state)
{
    UdpGram sendGram(itemName, 0x00040009);
    sendGram.setUint8(chOut, 0);
    sendGram.setUint8(chIn, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        state = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setMixerNodeState(const QString& itemName, quint8 chIn, quint8 chOut, bool state)
{
    UdpGram sendGram(itemName, 0x0004000A);
    sendGram.setUint8(chOut, 0);
    sendGram.setUint8(chIn, 1);
    sendGram.setBool(state, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getMixerNodeGain(const QString& itemName, quint8 chIn, quint8 chOut, double& gain)
{
    UdpGram sendGram(itemName, 0x0004000B);
    sendGram.setUint8(chOut, 0);
    sendGram.setUint8(chIn, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setMixerNodeGain(const QString& itemName, quint8 chIn, quint8 chOut, double gain)
{
    UdpGram sendGram(itemName, 0x0004000C);
    sendGram.setUint8(chOut, 0);
    sendGram.setUint8(chIn, 1);
    sendGram.setGain(gain, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}


//================混音模块================//
bool CenterCtrl::getDynThreshold(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x00050001);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDynThreshold(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x00050002);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDynRatio(const QString& itemName, double& ratio)
{
    UdpGram sendGram(itemName, 0x00050003);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ratio = recvGram.getUint16(0) / 100.0;
        return true;
    }
    return false;
}

bool CenterCtrl::setDynRatio(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x00050004);
    sendGram.setUint16(gain * 100, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDynAttackTime(const QString& itemName, quint16& ms, quint16& maxMs)
{
    UdpGram sendGram(itemName, 0x00050005);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ms = recvGram.getUint16(0);
        maxMs = recvGram.getUint16(4);
        return true;
    }
    return false;
}

bool CenterCtrl::setDynAttackTime(const QString& itemName, quint16 ms)
{
    UdpGram sendGram(itemName, 0x00050006);
    sendGram.setUint16(ms, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDynReleaseTime(const QString& itemName, quint16& ms, quint16& maxMs)
{
    UdpGram sendGram(itemName, 0x00050007);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ms = recvGram.getUint16(0);
        maxMs = recvGram.getUint16(4);
        return true;
    }
    return false;
}

bool CenterCtrl::setDynReleaseTime(const QString& itemName, quint16 ms)
{
    UdpGram sendGram(itemName, 0x00050008);
    sendGram.setUint16(ms, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDynGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x00050009);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDynGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x0005000A);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDynTargetLevel(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x0005000B);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDynTargetLevel(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x0005000C);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDynBypass(const QString& itemName, bool& bypass)
{
    UdpGram sendGram(itemName, 0x0005000D);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        bypass = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDynBypass(const QString& itemName, bool bypass)
{
    UdpGram sendGram(itemName, 0x0005000E);
    sendGram.setBool(bypass, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}


//================均衡模块================//
bool CenterCtrl::getEqFreq(const QString& itemName, quint8 ptIdx, quint16& freq)
{
    UdpGram sendGram(itemName, 0x00060001);
    sendGram.setUint8(ptIdx, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        freq = recvGram.getUint16(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setEqFreq(const QString& itemName, quint8 ptIdx, quint16 freq)
{
    UdpGram sendGram(itemName, 0x00060002);
    sendGram.setUint8(ptIdx, 0);
    sendGram.setUint16(freq, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getEqGain(const QString& itemName, quint8 ptIdx, double& gain, double& maxGain, double& minGain)
{
    UdpGram sendGram(itemName, 0x00060003);
    sendGram.setUint8(ptIdx, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        minGain = recvGram.getGain(2);
        maxGain = recvGram.getGain(4);
        return true;
    }
    return false;
}

bool CenterCtrl::setEqGain(const QString& itemName, quint8 ptIdx, double gain)
{
    UdpGram sendGram(itemName, 0x00060004);
    sendGram.setUint8(ptIdx, 0);
    sendGram.setGain(gain, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getEqQVal(const QString& itemName, quint8 ptIdx, double& qVal)
{
    UdpGram sendGram(itemName, 0x00060005);
    sendGram.setUint8(ptIdx, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        qVal = recvGram.getUint16(0) /100.0;
        return true;
    }
    return false;
}

bool CenterCtrl::setEqQVal(const QString& itemName, quint8 ptIdx, double qVal)
{
    UdpGram sendGram(itemName, 0x00060006);
    sendGram.setUint8(ptIdx, 0);
    sendGram.setUint16(qVal*100, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getEqBypass(const QString& itemName, bool& bypass)
{
    UdpGram sendGram(itemName, 0x00060007);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        bypass = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setEqBypass(const QString& itemName, bool bypass)
{
    UdpGram sendGram(itemName, 0x00060008);
    sendGram.setBool(bypass, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}


//================信号发生器模块================//
bool CenterCtrl::getSigFreq(const QString& itemName, quint16& freq)
{
    UdpGram sendGram(itemName, 0x00070001);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        freq = recvGram.getUint16(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setSigFreq(const QString& itemName, quint16 freq)
{
    UdpGram sendGram(itemName, 0x00070002);
    sendGram.setUint16(freq, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getSigGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x00070003);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setSigGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x00070004);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getSigMute(const QString& itemName, bool& mute)
{
    UdpGram sendGram(itemName, 0x00070005);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        mute = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setSigMute(const QString& itemName, bool mute)
{
    UdpGram sendGram(itemName, 0x00070006);
    sendGram.setBool(mute, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}


//================分频器模块================//
bool CenterCtrl::getSplitInMute(const QString& itemName, bool& mute)
{
    UdpGram sendGram(itemName, 0x00080001);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        mute = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setSplitInMute(const QString& itemName, bool mute)
{
    UdpGram sendGram(itemName, 0x00080002);
    sendGram.setBool(mute, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getSplitInGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x00080003);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setSplitInGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x00080004);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getSplitType(const QString& itemName, quint8 sNo, quint8& type)
{
    UdpGram sendGram(itemName, 0x00060001);
    sendGram.setUint8(sNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        type = recvGram.getUint8(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setSplitType(const QString& itemName, quint8 sNo, quint8 type)
{
    UdpGram sendGram(itemName, 0x00080006);
    sendGram.setUint8(sNo, 0);
    sendGram.setUint8(type, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getSplitFreq(const QString& itemName, quint8 sNo, quint16& freq)
{
    UdpGram sendGram(itemName, 0x00080007);
    sendGram.setUint8(sNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        freq = recvGram.getUint16(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setSplitFreq(const QString& itemName, quint8 sNo, quint16 freq)
{
    UdpGram sendGram(itemName, 0x00080008);
    sendGram.setUint8(sNo, 0);
    sendGram.setUint16(freq, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getSplitOutMute(const QString& itemName, bool& mute)
{
    UdpGram sendGram(itemName, 0x00080009);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        mute = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setSplitOutMute(const QString& itemName, bool mute)
{
    UdpGram sendGram(itemName, 0x0008000A);
    sendGram.setBool(mute, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getSplitOutGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x0008000B);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setSplitOutGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x0008000C);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getSplitOutAntiPhase(const QString& itemName, bool& antiPhase)
{
    UdpGram sendGram(itemName, 0x0008000D);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        antiPhase = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setSplitOutAntiPhase(const QString& itemName, bool antiPhase)
{
    UdpGram sendGram(itemName, 0x0008000E);
    sendGram.setBool(antiPhase, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}


//================滤波器模块================//
bool CenterCtrl::getFilterType(const QString& itemName, quint8& type)
{
    UdpGram sendGram(itemName, 0x00090001);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        type = recvGram.getUint8(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setFilterType(const QString& itemName, quint8 type)
{
    UdpGram sendGram(itemName, 0x00090002);
    sendGram.setUint8(type, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getFilterFreq(const QString& itemName, quint16& freq)
{
    UdpGram sendGram(itemName, 0x00090003);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        freq = recvGram.getUint16(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setFilterFreq(const QString& itemName, quint16 freq)
{
    UdpGram sendGram(itemName, 0x00090004);
    sendGram.setUint16(freq, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getFilterGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x00090005);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setFilterGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x00090006);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getFilterBypass(const QString& itemName, bool& bypass)
{
    UdpGram sendGram(itemName, 0x00090007);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        bypass = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setFilterBypass(const QString& itemName, bool bypass)
{
    UdpGram sendGram(itemName, 0x00090008);
    sendGram.setBool(bypass, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getFilterBandwidth(const QString& itemName, double& bandWidth)
{
    UdpGram sendGram(itemName, 0x00090009);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        bandWidth = recvGram.getUint16(0) / 1000.0;
        return true;
    }
    return false;
}

bool CenterCtrl::setFilterBandwidth(const QString& itemName, double bandWidth)
{
    UdpGram sendGram(itemName, 0x0009000A);
    sendGram.setUint16(bandWidth*1000, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}


//================啸叫抑制模块================//
bool CenterCtrl::getFreqshiftFreq(const QString& itemName, quint16& freq)
{
    UdpGram sendGram(itemName, 0x000A0001);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        freq = recvGram.getUint16(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setFreqshiftFreq(const QString& itemName, quint16 freq)
{
    UdpGram sendGram(itemName, 0x000A0002);
    sendGram.setUint16(freq, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getFreqshiftGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000A0003);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setFreqshiftGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000A0004);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}


//================自动混音模块================//
bool CenterCtrl::getAutoMixerThreshold(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000B0001);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerThreshold(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000B0002);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerAttackTime(const QString& itemName, quint16& ms, quint16& maxMs)
{
    UdpGram sendGram(itemName, 0x000B0003);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ms = recvGram.getUint16(0);
        maxMs = recvGram.getUint16(4);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerAttackTime(const QString& itemName, quint16 ms)
{
    UdpGram sendGram(itemName, 0x000B0004);
    sendGram.setUint16(ms, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerReleaseTime(const QString& itemName, quint16& ms, quint16& maxMs)
{
    UdpGram sendGram(itemName, 0x000B0005);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ms = recvGram.getUint16(0);
        maxMs = recvGram.getUint16(4);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerReleaseTime(const QString& itemName, quint16 ms)
{
    UdpGram sendGram(itemName, 0x000B0006);
    sendGram.setUint16(ms, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerHoldTime(const QString& itemName, quint16& ms, quint16& maxMs)
{
    UdpGram sendGram(itemName, 0x000B0007);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ms = recvGram.getUint16(0);
        maxMs = recvGram.getUint16(4);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerHoldTime(const QString& itemName, quint16 ms)
{
    UdpGram sendGram(itemName, 0x000B0008);
    sendGram.setUint16(ms, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerDepth(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000B000B);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerDepth(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000B000C);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerNoiseGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000B000F);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerNoiseGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000B0010);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerInputMute(const QString& itemName, quint8 chNo, bool& mute)
{
    UdpGram sendGram(itemName, 0x000B0011);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        mute = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerInputMute(const QString& itemName, quint8 chNo, bool mute)
{
    UdpGram sendGram(itemName, 0x000B0012);
    sendGram.setUint8(chNo, 0);
    sendGram.setBool(mute, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerInputGain(const QString& itemName, quint8 chNo, double& gain)
{
    UdpGram sendGram(itemName, 0x000B0013);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerInputGain(const QString& itemName, quint8 chNo, double gain)
{
    UdpGram sendGram(itemName, 0x000B0014);
    sendGram.setUint8(chNo, 0);
    sendGram.setGain(gain, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerOutputMute(const QString& itemName, quint8 chNo, bool& mute)
{
    UdpGram sendGram(itemName, 0x000B0015);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        mute = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerOutputMute(const QString& itemName, quint8 chNo, bool mute)
{
    UdpGram sendGram(itemName, 0x000B0016);
    sendGram.setUint8(chNo, 0);
    sendGram.setBool(mute, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerOutputGain(const QString& itemName, quint8 chNo, double& gain){
    UdpGram sendGram(itemName, 0x000B0017);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerOutputGain(const QString& itemName, quint8 chNo, double gain)
{
    UdpGram sendGram(itemName, 0x000B0018);
    sendGram.setUint8(chNo, 0);
    sendGram.setGain(gain, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerInputManual(const QString& itemName, quint8 chNo, bool& manual)
{
    UdpGram sendGram(itemName, 0x000B0019);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        manual = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerInputManual(const QString& itemName, quint8 chNo, bool manual)
{
    UdpGram sendGram(itemName, 0x000B001A);
    sendGram.setUint8(chNo, 0);
    sendGram.setBool(manual, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerInputMode(const QString& itemName, quint8 chNo, quint8& mode)
{
    UdpGram sendGram(itemName, 0x000B001B);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        mode = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerInputMode(const QString& itemName, quint8 chNo, quint8 mode)
{
    UdpGram sendGram(itemName, 0x000B001C);
    sendGram.setUint8(chNo, 0);
    sendGram.setUint8(mode, 1);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerInputPriority(const QString& itemName, quint8 chNo, quint16& prio)
{
    UdpGram sendGram(itemName, 0x000B001D);
    sendGram.setUint8(chNo, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        prio = recvGram.getUint16(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerInputPriority(const QString& itemName, quint8 chNo, quint16 prio)
{
    UdpGram sendGram(itemName, 0x000B001E);
    sendGram.setUint8(chNo, 0);
    sendGram.setUint16(prio, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerPercentage(const QString& itemName, double& percent)
{
    UdpGram sendGram(itemName, 0x000B001F);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        percent = recvGram.getUint16(0) / 100.0;
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerPercentage(const QString& itemName, double percent)
{
    UdpGram sendGram(itemName, 0x000B0020);
    quint16 perc = (quint16)percent * 100;
    sendGram.setUint16(perc, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerLastMicOn(const QString& itemName, quint8& status)
{
    UdpGram sendGram(itemName, 0x000B0023);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        status = recvGram.getUint8(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerLastMicOn(const QString& itemName, quint8 status)
{
    UdpGram sendGram(itemName, 0x000B0024);
    sendGram.setUint8(status, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerDefaultCh(const QString& itemName, quint8& defCn)
{
    UdpGram sendGram(itemName, 0x000B0025);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        defCn = recvGram.getUint8(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerDefaultCh(const QString& itemName, quint8 defCn)
{
    UdpGram sendGram(itemName, 0x000B0026);
    sendGram.setUint8(defCn, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerThresholdLevelAboveNoise(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000B0027);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerThresholdLevelAboveNoise(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000B0028);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}


bool CenterCtrl::getAutoMixerMaxNOM(const QString& itemName, bool& onOff)
{
    UdpGram sendGram(itemName, 0x000B0029);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        onOff = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerMaxNOM(const QString& itemName, bool onOff)
{
    UdpGram sendGram(itemName, 0x000B002A);
    sendGram.setBool(onOff, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerDirectOutsNOMAtten(const QString& itemName, bool& onOff)
{
    UdpGram sendGram(itemName, 0x000B002B);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        onOff = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerDirectOutsNOMAtten(const QString& itemName, bool onOff)
{
    UdpGram sendGram(itemName, 0x000B002C);
    sendGram.setBool(onOff, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerNOMAttenuationType(const QString& itemName, quint8& type)
{
    UdpGram sendGram(itemName, 0x000B002D);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        type = recvGram.getUint8(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerNOMAttenuationType(const QString& itemName, quint8 type)
{
    UdpGram sendGram(itemName, 0x000B002E);
    sendGram.setUint8(type, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerNOMAttenuationStep(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000B002F);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerNOMAttenuationStep(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000B0030);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerMaxNOMAttenuation(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000B0031);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerMaxNOMAttenuation(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000B0032);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAutoMixerResponseTime(const QString& itemName, quint16& ms)
{
    UdpGram sendGram(itemName, 0x000B0033);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ms = recvGram.getUint16(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAutoMixerResponseTime(const QString& itemName, quint16 ms)
{
    UdpGram sendGram(itemName, 0x000B0034);
    sendGram.setUint16(ms, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}


//================回声消除模块================//
bool CenterCtrl::getAECMicInGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000C0001);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAECMicInGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000C0002);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAECLineInGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000C0003);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAECLineInGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000C0004);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAECSpeakOutGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000C0005);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAECSpeakOutGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000C0006);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAECLineOutGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000C0007);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAECLineOutGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000C0008);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAECMicToSpeakGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000C0009);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAECMicToSpeakGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000C000A);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}


bool CenterCtrl::getAECMicToSpeakGainOn(const QString& itemName, bool& onOff)
{
    UdpGram sendGram(itemName, 0x000C000B);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        onOff = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAECMicToSpeakGainOn(const QString& itemName, bool onOff)
{
    UdpGram sendGram(itemName, 0x000C000C);
    sendGram.setBool(onOff, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

//Gain_6 = 0, Gain_9 = 1, Gain_12 = 2, Gain_15 = 3
bool CenterCtrl::getAECNRGain(const QString& itemName, quint8& gainType)
{
    UdpGram sendGram(itemName, 0x000C000D);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gainType = recvGram.getUint8(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAECNRGain(const QString& itemName, quint8 gainType)
{
    UdpGram sendGram(itemName, 0x000C000E);
    sendGram.setUint8(gainType, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getAECNRGainOn(const QString& itemName, bool& onOff)
{
    UdpGram sendGram(itemName, 0x000C000F);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        onOff = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setAECNRGainOn(const QString& itemName, bool onOff)
{
    UdpGram sendGram(itemName, 0x000C0010);
    sendGram.setBool(onOff, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

//================闪避模块================//
bool CenterCtrl::getDuckerAttackTime(const QString& itemName, quint16& ms)
{
    UdpGram sendGram(itemName, 0x000D0001);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ms = recvGram.getUint16(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDuckerAttackTime(const QString& itemName, quint16 ms)
{
    UdpGram sendGram(itemName, 0x000D0002);
    sendGram.setUint16(ms, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDuckerReleaseTime(const QString& itemName, quint16& ms)
{
    UdpGram sendGram(itemName, 0x000D0003);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ms = recvGram.getUint16(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDuckerReleaseTime(const QString& itemName, quint16 ms)
{
    UdpGram sendGram(itemName, 0x000D0004);
    sendGram.setUint16(ms, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDuckerHoldTime(const QString& itemName, quint16& ms)
{
    UdpGram sendGram(itemName, 0x000D0005);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ms = recvGram.getUint16(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDuckerHoldTime(const QString& itemName, quint16 ms)
{
    UdpGram sendGram(itemName, 0x000D0006);
    sendGram.setUint16(ms, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDuckerDetectTime(const QString& itemName, quint16& ms)
{
    UdpGram sendGram(itemName, 0x000D0007);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ms = recvGram.getUint16(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDuckerDetectTime(const QString& itemName, quint16 ms)
{
    UdpGram sendGram(itemName, 0x000D0008);
    sendGram.setUint16(ms, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDuckerThreshold(const QString& itemName, double& ms)
{
    UdpGram sendGram(itemName, 0x000D0009);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ms = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDuckerThreshold(const QString& itemName, double ms)
{
    UdpGram sendGram(itemName, 0x000D000A);
    sendGram.setGain(ms, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDuckerDepth(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000D000B);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDuckerDepth(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000D000C);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDuckerInGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000D000D);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDuckerInGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000D000E);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDuckerPriorityGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000D000F);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDuckerPriorityGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000D0010);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDuckerOutGain(const QString& itemName, double& gain)
{
    UdpGram sendGram(itemName, 0x000D0011);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDuckerOutGain(const QString& itemName, double gain)
{
    UdpGram sendGram(itemName, 0x000D0012);
    sendGram.setGain(gain, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getDuckerBypass(const QString& itemName, bool& bypass)
{
    UdpGram sendGram(itemName, 0x000D0013);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        bypass = recvGram.getBool(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setDuckerBypass(const QString& itemName, bool bypass)
{
    UdpGram sendGram(itemName, 0x000D0014);
    sendGram.setBool(bypass, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}


//================淡入淡出模块================//
bool CenterCtrl::getGainRampTime(const QString& itemName, quint8 type, quint16& ms)
{
    UdpGram sendGram(itemName, 0x000E0001);
    sendGram.setUint8(type, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        ms = recvGram.getUint16(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setGainRampTime(const QString& itemName, quint8 type, quint16 ms)
{
    UdpGram sendGram(itemName, 0x000E0002);
    sendGram.setUint8(type, 0);
    sendGram.setUint16(ms, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getGainRampGain(const QString& itemName, quint8 type, double& gain)
{
    UdpGram sendGram(itemName, 0x000B0011);
    sendGram.setUint8(type, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        gain = recvGram.getGain(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setGainRampGain(const QString& itemName, quint8 type, double gain)
{
    UdpGram sendGram(itemName, 0x000B0012);
    sendGram.setUint8(type, 0);
    sendGram.setBool(gain, 2);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getGainRampType(const QString& itemName, quint8& type)
{
    UdpGram sendGram(itemName, 0x000E0005);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        type = recvGram.getUint8(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setGainRampType(const QString& itemName, quint8 type)
{
    UdpGram sendGram(itemName, 0x000E0006);
    sendGram.setUint8(type, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}


//================播放器模块================//
bool CenterCtrl::getPlayerState(const QString& itemName, quint8& state)
{
    UdpGram sendGram(itemName, 0x000F0001);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs){
        state = recvGram.getUint8(0);
        return true;
    }
    return false;
}

bool CenterCtrl::setPlayerState(const QString& itemName, quint8 state)
{
    UdpGram sendGram(itemName, 0x000F0002);
    sendGram.setUint8(state, 0);

    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

bool CenterCtrl::getCurrentSceneNum(quint8 &sceneNum)
{
    UdpGram sendGram(QString("SCENE"), 0xFFFF0002);
    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    if(rs) {
        sceneNum = recvGram.getUint8(0);
    }
    return rs;
}

bool CenterCtrl::setCurrentSceneNum(quint8 sceneNum)
{
    UdpGram sendGram(QString("SCENE"), 0xFFFF0003);
    sendGram.setUint8(sceneNum, 0);
    UdpGram recvGram;
    bool rs = sendAndReceiveCommand(sendGram, recvGram);
    return rs;
}

//==================其它函数==================//
QByteArray CenterCtrl::formatDatagram(const UdpGram& udpGram)
{
    quint16 sum = 0;
    quint8* pData = (quint8*) &udpGram;
    for(int i = 0; i< sizeof(UdpGram); i++){
        sum += pData[i];
    }
    QByteArray udpGramBa;
    QDataStream stream(&udpGramBa, QIODevice::ReadWrite);
    stream.setByteOrder(QDataStream::BigEndian);

    stream << (quint16)0x7B20;      //帧头0x7B20
    stream << sum;                  //模块名称、功能ID、数据长度的字节累加和
    stream << udpGram;
    stream << (quint16)0x0000;      //4字节对齐预留位0x0000
    stream << (quint16)0x207B;      //帧尾0x207B
    return udpGramBa;
}

bool CenterCtrl::sendAndReceiveCommand(const UdpGram& sendGram, UdpGram& recvGram)
{
    QByteArray udpGram = formatDatagram(sendGram);
//qDebug() << __FUNCTION__ << __LINE__ <<udpGram.toHex();
    _udpDevice->writeDatagram(udpGram, _devIpAddress, 35030);
    bool ready = _udpDevice->waitForReadyRead(2000);
    if(!ready) return false; //接收超时

    QByteArray gramBa;
    while (_udpDevice->hasPendingDatagrams())//如果有数据等待接受
    {
        gramBa.resize(_udpDevice->pendingDatagramSize());//用UDP数据报的大小来设置字节数组的大小
        QHostAddress sender;
        quint16 senderPort;
        //接受不大于字节数组大小的数据报，并将其存储在字节数组的data中，发送方的主机地址和端口号分别存储在sender和senderport中
        _udpDevice->readDatagram(gramBa.data(), gramBa.size(), &sender, &senderPort);
    }

    if(gramBa.length() != 28) return false;
//qDebug() << __FUNCTION__ << __LINE__ << gramBa.toHex();
    QDataStream stream(&gramBa, QIODevice::ReadWrite);
    stream.setByteOrder(QDataStream::BigEndian);
    quint16 head, sum, temp, end;
    stream >> head >> sum;
    stream >> recvGram;
    stream >> temp >> end;
    int calcSum = 0;
    for(int i = 4; i< gramBa.length() -4; i++ ){
        calcSum += (quint8)gramBa[i];
    }
    if(head != 0x7B20 || end != 0x207B || recvGram.getCmdKey() != 0xFFFF0000 || sum != calcSum)
        return false;
    else
        return true;
}



