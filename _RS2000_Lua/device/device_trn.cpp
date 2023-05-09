#include "stable.h"
#include "device_trn.h"
#include "Protocal/packetbase.h"
#include "device/devicemanager.h"
#include "deviceSetting/trn/trn_copywidget.h"
#include "DanteSdk/dantebrowser.h"
#include "Frame/mainwindow.h"

#define Debug_VUInfo 0 //显示输入输出信息
//////////////////////////////////////////////////////////////////
/// Trn_Interface
//////////////////////////////////////////////////////////////////
#include "configItem/configitem.h"
#include "configItem/configiteminput.h"
#include "configItem/configitemoutput.h"
struct PortGroupDef
{
    DevPortType portType;
    int realCh;
    int groupNum;
    int portIdx;
};

CfgType  getCfgTypeByGourpNum(bool isInput, int portGroupNum)
{
    if(isInput && portGroupNum == 1) return CFG_TYPE_INPUT_1;
    else if(isInput && portGroupNum == 2) return CFG_TYPE_INPUT_2;
    else if(isInput && portGroupNum == 4) return CFG_TYPE_INPUT_4;
    else if(isInput && portGroupNum == 8) return CFG_TYPE_INPUT_8;
    else if(isInput && portGroupNum == 16) return CFG_TYPE_INPUT_16;
    else if(!isInput && portGroupNum == 1) return CFG_TYPE_OUTPUT_1;
    else if(!isInput && portGroupNum == 2) return CFG_TYPE_OUTPUT_2;
    else if(!isInput && portGroupNum == 4) return CFG_TYPE_OUTPUT_4;
    else if(!isInput && portGroupNum == 8) return CFG_TYPE_OUTPUT_8;
    else if(!isInput && portGroupNum == 16) return CFG_TYPE_OUTPUT_16;
}

Trn_Interface::Trn_Interface(Device_Trn* trnDev)
{
    _trnDev = trnDev;
    DevType devType = _trnDev->getDeviceType();
    int portGroupNum = Config::instance()->getPortGroupNum();
    QList<PortGroupDef> groupDefList;

    Utils::devItemPorts_t  devPort = Utils::getDeviceItemPortNum(devType);

    //初始化输入输出Item
    int realCh = 0;
    if(devType == Dev_Type_NAP_STM32H750_8A8N) //8*8特殊处理，9~16路悬空
    {
        for(int i = 0; i < devPort.s_In.s_ANum; i+= portGroupNum, realCh += portGroupNum) {
            groupDefList.append(PortGroupDef{PortType_MONAURAL, realCh, portGroupNum, i});
        }
        realCh = 16; //网络通道号跳过悬空的9~16路
        for(int i = 0; i < devPort.s_In.s_NNum; i+= portGroupNum, realCh += portGroupNum) {
            groupDefList.append(PortGroupDef{PortType_AES67, realCh, portGroupNum, i});
        }
    }
    else if(Config::curMfrsCfg().CurMfrs == Mfrs_FTYX
            && devType == Dev_Type_NAP_Zynq7020_16Analog_48AES67
            && _trnDev->getDeviceName().endsWith("64N"))
    {
        //富通亚讯:假64路网络的特殊处理
        for(int i = 0; i < devPort.s_In.s_NNum; i+= portGroupNum, realCh += portGroupNum) {
            groupDefList.append(PortGroupDef{PortType_AES67, realCh, portGroupNum, i});
        }
        for(int i = 0; i < devPort.s_In.s_ANum; i+= portGroupNum, realCh += portGroupNum) {
            groupDefList.append(PortGroupDef{PortType_MONAURAL, realCh, portGroupNum, i});
        }
    }
    else{
        for(int i = 0; i < devPort.s_In.s_ANum; i+= portGroupNum, realCh += portGroupNum) {
            groupDefList.append(PortGroupDef{PortType_MONAURAL, realCh, portGroupNum, i});
        }
        for(int i = 0; i < devPort.s_In.s_NNum; i+= portGroupNum, realCh += portGroupNum) {
            groupDefList.append(PortGroupDef{PortType_AES67, realCh, portGroupNum, i});
        }
        for(int i = 0; i < devPort.s_In.s_UNum; i+= portGroupNum, realCh += portGroupNum) {
            int groupNum = portGroupNum;
            if(i+ portGroupNum > devPort.s_In.s_UNum)
            {
                groupNum = devPort.s_In.s_UNum -i;
            }
            groupDefList.append(PortGroupDef{PortType_USB, realCh, groupNum, i});
        }
    }

    //分开初始化输入、输出， 确保模块序号连续
    for(PortGroupDef groupDef : groupDefList)
    {
        QString customNameIn =  QString("Inp_%1").arg(getCfgItemIndex());
        CfgType inputType = getCfgTypeByGourpNum(true, groupDef.groupNum);
        ConfigItem *input = ConfigItemFactory::createConfigItem(_trnDev, inputType, customNameIn, QPointF(0, 0));
        qgraphicsitem_cast<ConfigItemInput *>(input)->initRealChannel(groupDef.portType, groupDef.realCh, groupDef.portIdx);
        _inputModelList.append(input);
    }
    for(PortGroupDef groupDef : groupDefList)
    {
        QString customNameOut =  QString("Out_%1").arg(getCfgItemIndex());
        CfgType outputType = getCfgTypeByGourpNum(false, groupDef.groupNum);
        ConfigItem *output = ConfigItemFactory::createConfigItem(_trnDev, outputType, customNameOut, QPointF(0, 0));
        qgraphicsitem_cast<ConfigItemOutput *>(output)->initRealChannel(groupDef.portType, groupDef.realCh, groupDef.portIdx);
        _outputModelList.append(output);
    }

    //snapshot    hrp
    QPointF pos(20, 20*(90 + 20));
    QString customNameSnapshot =  QString("%1_%2").arg(Utils::getCfgShortName(CFG_TYPE_SNAPSHOT)).arg(getCfgItemIndex());
    ConfigItem *snapshot = ConfigItemFactory::createConfigItem(_trnDev, CFG_TYPE_SNAPSHOT, customNameSnapshot, pos);
    _snapshotList.append(snapshot);

    if(devType ==  Dev_Type_NAP_X86_16Analog_128AES67)
    {
        QList<CfgType> ctrlList{CFG_TYPE_CTRL_GPI, CFG_TYPE_CTRL_GPO, CFG_TYPE_CTRL_AD,CFG_TYPE_CTRL_RS232, CFG_TYPE_CTRL_RS485};
        for(CfgType type : ctrlList)
        {
            QString customName =  QString("%1_%2").arg(Utils::getCfgShortName(type)).arg(getCfgItemIndex());
            _controlModelList.append(ConfigItemFactory::createConfigItem(_trnDev, type, customName, QPointF(0, 0)));
        }
    }
}

ConfigItem * Trn_Interface::getInOutItemByChannel(int nmuVersion, CfgType cfgType, int channel)
{
    bool isInput = (cfgType& FCT_FAMILY_MASK) == CFG_TYPE_INPUT_1;
    if(nmuVersion == 0)
    {
        if(isInput) {
            if(channel >= _inputModelList.length())
                return nullptr;
            return _inputModelList.at(channel);
        }
        else {
            if(channel >= _outputModelList.length())
                return nullptr;
            return _outputModelList.at(channel);
        }
    }
    else if(nmuVersion == 1)
    {
        if(isInput) {
            for(ConfigItem *item : _inputModelList)
            {
                ConfigItemInput* inItem = qobject_cast<ConfigItemInput*>(item);
                if(inItem != NULL && inItem->getRealChannel(0) == channel){
                    return item;
                }
            }
        }
        else {
            for(ConfigItem *item : _outputModelList)
            {
                ConfigItemOutput* outItem = qobject_cast<ConfigItemOutput*>(item);
                if(outItem != NULL && outItem->getRealChannel(0) == channel){
                    return item;
                }
            }
        }
        return nullptr;
    }
    else {
        return nullptr;
    }
}



//////////////////////////////////////////////////////////////////
/// Device_TRN
//////////////////////////////////////////////////////////////////

Device_Trn::Device_Trn(QString mac, QString devName, DevType devType, QString devIp, QString creator)
    :AES70Device(mac, devName, devType, devIp, creator)
{
    _configData.initConfigData();
    mixerUserNameLoad();
    _sendConfigType = SendAllConfigType_None;
}


#include <QDir>
#include "deviceSetting/devsetting_firmwareupdate.h"
bool Device_Trn::checkFirmwareVersion()
{
    if(!isOnline()) {
        MainWindow::instance()->showNotice(tr("Device is not Online!"));
        return false;
    }
    PlatformType platform = Utils::getPlatformType(_deviceType);
    //判定主控版本号   待完善 : 主控固件版本号 需升级至 3.0.1之后方可使用 软件AEC
    if(platform == Platform_DSP){
        QString curVer = QString("%1.%2.%3.%4").arg(transctrl_version[0]).arg(transctrl_version[1]).arg(transctrl_version[2]).arg(transctrl_version[3]);
        QString reqVer = QString("%1.%2.%3.%4").arg(Utils::maxMasterVersion[0]).arg(Utils::maxMasterVersion[1]).arg(Utils::maxMasterVersion[2]).arg(Utils::maxMasterVersion[3]);
        if(curVer < reqVer) {
            Utils::MessageBoxWarning(tr("Warning"),
            tr("Current main control version number: %1,\n Software requires the minimum master version number: %2!")
                                  .arg(curVer).arg(reqVer));
        }
    }

    //get upgrade file
    QList<Utils::FirmwareUpdate_t> verList;
    QString currDir = QString("%1/update/").arg(QApplication::applicationDirPath());
    if(!getUpgradeFile(verList, currDir) && isOnline()) {
        return true;
    }

    QString errorStr;
    bool res = true;

    if(_deviceType == Dev_Type_NAP_Zynq7020_16Analog_48AES67
            && transctrl_version[0] >= 3) //待完善
    {
        Utils::FirmwareUpdate_t currVer;
        memset(&currVer, 0, sizeof(Utils::FirmwareUpdate_t));
        bool isFind = false;
        for(Utils::FirmwareUpdate_t ver : verList) {
            PlatformType pType = getPlatformType();
            if(pType == ver.s_PFType && devVer[4] == ver.s_HW) {//查找相应升级包
                memcpy(&currVer, &ver, sizeof(ver));
                isFind = true;
            }
        }
        if(!isFind) {
            errorStr = tr("The software does not match the device!");
            res = false;
        }
        else if(currVer.s_FW[0] >  devVer[0] ||
               (currVer.s_FW[0] == devVer[0] && currVer.s_FW[1] >  devVer[1]) ||
               (currVer.s_FW[0] == devVer[0] && currVer.s_FW[1] == devVer[1] && currVer.s_FW[2] > devVer[2]))
        {
            errorStr = tr("Equipment needs to be upgraded.");
            res = false;
        }
        else if(currVer.s_FW[0] == devVer[0] && currVer.s_FW[1] == devVer[1] && currVer.s_FW[2] == devVer[2]){
            if(currVer.s_FW[3] > devVer[3]) {
                errorStr = tr("Updates are available for the device.");
                res = false;
            }
            else
                res = true;
        }
        else {
            errorStr = tr("It is recommended to use the latest software, or click \"OK\" to downgrade the device firmware.");
            res = false;
        }

        errorStr += "\n";
        errorStr += tr("Current firmware: %1.%2.%3.%4, Update firmware: %5.%6.%7.%8")
                        .arg(devVer[0]).arg(devVer[1]).arg(devVer[2]).arg(devVer[3])
                        .arg(currVer.s_FW[0]).arg(currVer.s_FW[1]).arg(currVer.s_FW[2]).arg(currVer.s_FW[3]);

        QString path = currDir + QString(currVer.s_fileName);
        if(!res) {
            if(Utils::MessageBoxQuestion(tr("Information"), errorStr, QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {
                if(!isFind) return res;
                DevSetting_FirmwareUpdate firUpdate(this);
                firUpdate.AutoUpdate(path, currVer.s_FW, devVer);
                firUpdate.exec();
                res = true;
            } else {
                return true;//点击Canncel
            }
        }
    }
    return res;//暂时可下载
}

bool Device_Trn::getUpgradeFile(QList<Utils::FirmwareUpdate_t> &verList, QString currDir)
{
    QDir dir(currDir);
    if(!dir.exists()) {
        dir.mkdir(currDir);
        qDebug() << __FUNCTION__ << __LINE__ << "No upgrade folder!";
        return false;//没有升级文件不做版本校验
    }
    QStringList filters; filters << "*.ftcn";
    QStringList files = dir.entryList(filters);
    if(files.isEmpty()) {
        qDebug() << __FUNCTION__ << __LINE__ << "The upgrade folder is empty!";
        return false;
    }

    for(QString file : files) {
        Utils::FirmwareUpdate_t ver;
        QString fileName = file;
        QString strType = file.remove(file.right(5)).right(3);
        uint type = 0;
        if(!QString::compare(strType, "dsp", Qt::CaseInsensitive))
            type = Platform_DSP;
        if(!QString::compare(strType, "x86", Qt::CaseInsensitive))
            type = Platform_X86;
        if(!QString::compare(strType, "arm", Qt::CaseInsensitive))
            type = Platform_ARM;
        QStringList strVer = file.remove(file.right(4)).split('_');
        QString FW = strVer.at(2);   FW.remove(0, 2);
        QString HW = strVer.at(3);   HW.remove(0, 2);

        int idx = 0;
        foreach(QString fw, FW.split('.')) {
            ver.s_FW[idx] = fw.toUInt();
            idx++;
        }
        QByteArray ary = fileName.toLatin1();
        ver.s_fileName = ary.data();
        ver.s_HW       = HW.toUInt();
        ver.s_PFType   = type;

        verList.append(ver);
    }
    return true;
}

int Device_Trn::getCpuCount()
{
    if(_deviceType == Dev_Type_NAP_X86_16Analog_128AES67){
        return 2;
    }
    else{
        return 1;
    }

}


void Device_Trn::checkAliveTimerOut()
{
//    sendHeartbeatPack();//wj1 心跳机制
}




void Device_Trn::setOffline()
{
    AES70Device::setOffline();
    emit deviceOffline();
}

void Device_Trn::processPacket(PacketBase* pack)
{
    MsgType mType = pack->getMsgType();
    switch(mType) {
    case Msg_Type_GetInitAck:
        switchStatus(Dev_Online);
        _configData = pack->getBodyTrn().config.getDataPack();//获得一个ConfigData_Trn类型
        break;
    case Msg_Type_HeartbeatAck:{
        transctrl_version[0] = pack->getBodyTrn()._heartbeat.transctrl_Major;
        transctrl_version[1] = pack->getBodyTrn()._heartbeat.transctrl_Minor;
        transctrl_version[2] = pack->getBodyTrn()._heartbeat.transctrl_Build;
        transctrl_version[3] = pack->getBodyTrn()._heartbeat.transctrl_HardwareV;
        for (int i = 0; i < 8; ++i) {
            devVer[i] = pack->getBodyTrn()._heartbeat.dsp_Version[i];
        }
        QString strVer = QString("%1.%2.%3.%4/%5.%6.%7.%8 [%9][%10]")
                    .arg(transctrl_version[0]).arg(transctrl_version[1]).arg(transctrl_version[2]).arg(transctrl_version[3])
                    .arg(devVer[0]).arg(devVer[1]).arg(devVer[2]).arg(devVer[3]).arg(devVer[4]).arg(devVer[5]);
        emit sendDSPVersion(strVer);
        break;
    }
    case Msg_Type_NAP_MonitorMeter://电平表
        if (_isDebugMode)
        {
            if(is16BitProtocal())
            {
                emit dispMeterA(pack->getBodyTrn().meter16Bit);
            }
            else
            {
                FC_RTU_METER meter[METER_MAXCHANNEL];
                memset(&meter, 0, sizeof(meter));
                FC_RTU_METER8Bit* meter8B =  pack->getBodyTrn().meter8Bit;
                for(int i = 0; i< METER_MAXCHANNEL; i++)
                {
                    if(!meter8B[i].effect) { break; }
                    meter[i].modeUnitNum = meter8B[i].modeUnitNumU *256 + meter8B[i].modeUnitNum;
                    meter[i].effect = 1;
                    meter[i].channelNum = meter8B[i].channelNum;
                    meter[i].vu = meter8B[i].vu;
                    meter[i].ppm = meter8B[i].ppm;
                }
                emit dispMeterA(meter);
            }
        }
        break;
    case Msg_Type_NAP_AutomaticMixerMeter: //新协议将AMC的显示数据移入Msg_Type_NAP_InOutLevel
        break;
//    case Msg_Type_NAP_AlgorithmSave:
    case Msg_Type_NAP_FileToDeviceEnd:
        MainWindow::instance()->showNotice(tr("Save Success!"));
        break;
    case Msg_Type_NAP_AlgorithmOverview:
    case Msg_Type_NAP_AlgorithmDetail:
    case Msg_Type_NAP_AlgorithmStart:
//        qDebug() << "MSG_TYPE_ALGORITHM";
        break;
    case Msg_Type_NAP_FileFromDeviceStart:
        break;
    case Msg_Type_NAP_FileFromDevice:
        emit fileFromDeviceSig(pack->getBodyTrn().file_data, pack->getBodySize());
        break;
    case Msg_Type_NAP_FileFromDeviceEnd:
        emit fileFromDeviceEndSig();
        break;
    case Msg_Type_NAP_ParaChangedFromUser:
    case Msg_Type_NAP_CenterCtrl_Ack:
        emit ParaChangedFromUserSig(pack->getBodyTrn().para_data);
        break;
    case Msg_Type_NAP_SceneChangedFromUser:
//        qDebug() << __FUNCTION__ << __LINE__;
        emit SceneChangedFromUserSig();
        break;
    case Msg_Type_NAP_ModuleRtnValue:
        if (_isDebugMode)
        {
            emit ModuleRtnValueSig(pack->getBodyTrn().para_data);
        }
        break;
    case Msg_Type_NAP_InOutLevel:
        if (_isDebugMode)
        {
            MsgData_Trn msgData = pack->getBodyTrn();
            emit InOutLevelSig((unsigned int *)&msgData.inOutLevel);

#if Debug_VUInfo
            int bodyLen = pack->getBodySize(); //调试:输入输出电平表
            quint8* vuData = (quint8*)&msgData;
            static QDateTime LastTime = QDateTime::currentDateTime();
            if(LastTime.msecsTo(QDateTime::currentDateTime()) > 10000)
            {
                FC_RTU_InOutLevel128 uvData = msgData.inOutLevel128;
                QString inVU,outVU;
                for(int i = 0; i< bodyLen/2; i++)
                {
                    inVU +=  QString().sprintf("%02X ", vuData[i]);
                    outVU +=  QString().sprintf("%02X ", vuData[bodyLen/2 +i]);
                    if(i%32 == 31) {
                        inVU += "\r\n";
                        outVU += "\r\n";
                    }
                }
                qDebug()<<"VU IN:" << inVU;
                qDebug()<<"VU Out:" << outVU;
                LastTime = QDateTime::currentDateTime();
            }
#endif
         }
        break;
    case Msg_Type_NAP_FileList:
        if (isAddedToProject())
        {
            FC_RTU_PLAYFILES playFilesData = pack->getBodyTrn().playFiles;
            QString fileListStr = QString::fromUtf8((const char*)playFilesData.fileNames);
            QStringList files = fileListStr.split('|', Qt::SkipEmptyParts);
            _fileList.append(files);
            if(playFilesData.totalCnt == playFilesData.stopIdx)
            {
                emit fileListChanged(_fileList);
            }
        }
        break;
    default:
        if(mType == Msg_Type_NAP_Phantom ||
            mType == Msg_Type_NAP_MicGain ||
            mType == Msg_Type_NAP_FileToDeviceStart ||
            mType == Msg_Type_NAP_FileToDevice ||
            mType == Msg_Type_NAP_FileToDeviceEnd ||
            mType == Msg_Type_NAP_ModuleNameOverview )
        {
            ; //可忽略的数据包（不需处理的确认包等）
        }
        else{
            qDebug()<<QString("!! Device_Trn(%1) resv MsgType: %2")
                      .arg(getDeviceIp()).arg(pack->getMsgType(), 0, 16);
        }
        break;
    }
}

void Device_Trn::serialize(QString fileName)
{
    QFile f(fileName);
    f.open(QFile::WriteOnly);
    QDataStream ds(&f);
    ds << _configData;

    f.close();
}

void Device_Trn::deserialize(QString fileName)
{
    QFile f(fileName);
    f.open(QFile::ReadOnly);
    QDataStream ds(&f);
    ds >> _configData;
    f.close();
}

void Device_Trn::initData(const ConfigData_Trn& configData)
{
    memcpy(&_configData, &configData, sizeof(ConfigData_Trn));
}

void Device_Trn::sendGroup()
{
    QByteArray packBuff;
    unsigned short group_no = _group;
    packBuff.append((const char *)&group_no, sizeof(unsigned short));
//    packBuff.resize(2);//packBuff在初始化的时候长度为0，需要设置字节数组的长度2
//    memcpy(packBuff.data(), &group_no, sizeof(unsigned short));
    sendPacket(Msg_Type_NAP_MicGroupSet,packBuff);
}

void Device_Trn::sendFileToDeviceStartPack(quint32 num)
{
    QByteArray array;

    array.append((const char *)&num, sizeof(quint32));

    sendPacketSync(Msg_Type_NAP_FileToDeviceStart, array, PacketBase::UDP_WAIT_MS);
}

void Device_Trn::sendFileToDevicePack(QByteArray buff)
{
    sendPacketSync(Msg_Type_NAP_FileToDevice, buff, PacketBase::UDP_WAIT_MS);
}

void Device_Trn::sendFileToDeviceEndPack(void)
{
    sendPacketSync_Nobody(Msg_Type_NAP_FileToDeviceEnd, PacketBase::UDP_WAIT_MS);
}

bool Device_Trn::sendFileFromDevicePack(void)
{
    int msgIndex = getMsgIdx();
    PacketBase *pack = PacketBase::createPacket_Nobody(Msg_Type_NAP_FileFromDeviceStart, msgIndex);
    quint16 msgTypeResp = Msg_Type_NAP_FileFromDeviceEnd;
    quint16 msgIndexResp = 0xFFFF;
    return DeviceManager::instance()->sendPacketSync(this, pack, 10*1000, msgTypeResp, msgIndexResp);
}

void Device_Trn::sendHeartbeatPack()
{
    std::string ctr = _deviceName.left(20).toStdString();
    const char *cname = ctr.c_str();
    QByteArray packBuff(cname, ctr.length());
    sendPacket(Msg_Type_Heartbeat, packBuff);
}

bool Device_Trn::sendClearScenePack()
{
    return sendPacketSync_Nobody(Msg_Type_NAP_ClearScene, PacketBase::UDP_WAIT_MS);
}

bool Device_Trn::sendPhantomPack(int channel, bool isOpen)
{
    struct {
        unsigned char ch;
        unsigned char value;
        unsigned short reserve;
    } Analog;

    _configData.InMic48V[channel] = isOpen;

    Analog.ch = channel;
    Analog.value = isOpen;

    QByteArray packBuff((const char *)&Analog, sizeof(Analog));
    return sendPacketSync(Msg_Type_NAP_Phantom, packBuff, PacketBase::UDP_WAIT_MS);
}

bool Device_Trn::sendMicGainPack(quint8 ch, qint8 value)
{
    struct {
        unsigned char ch;
        unsigned char value;
        unsigned short reserve;
    } Analog;

    _configData.InMicGain[ch] = value;

    Analog.ch = ch;
    Analog.value = value;

    QByteArray packBuff((const char *)&Analog, sizeof(Analog));
    return sendPacketSync(Msg_Type_NAP_MicGain, packBuff, PacketBase::UDP_WAIT_MS);
}

QByteArray Device_Trn::getGrobalHeadBuff(quint32 moduleCount)
{
    QByteArray headBuff;
    bool is16Bit = is16BitProtocal();
    if(is16Bit){
        IG_Global_HEAD16B global_head;
        memset(&global_head, 0, sizeof(IG_Global_HEAD16B));
        global_head.flag = IG_GLOBAL_HEAD_FLAG;
        global_head.moduleAmount = moduleCount;
        headBuff.append((const char *)&global_head, sizeof(IG_Global_HEAD16B));
    }
    else{
        IG_Global_HEAD8B global_head;
        memset(&global_head, 0, sizeof(IG_Global_HEAD8B));
        global_head.moduleAmountL = moduleCount%256;
        global_head.moduleAmountH = moduleCount/256;
        headBuff.append((const char *)&global_head, sizeof(IG_Global_HEAD8B));
    }
    return headBuff;
}

/*
 *            bit7 bit6 bit5 bit4 bit3             bit2              bit1           bit0
 * b_start     x    x    x    x   modulename data  modulename start  overview data   first download
 * */
bool Device_Trn::sendAlgorithmSavePack(bool bStart, bool bOverview, quint8 scene_idx, quint32 moduleCount, QByteArray buff)
{
    QByteArray headBuff = getGrobalHeadBuff(moduleCount);

    IG_Algorithm_Head algorithm_head;
    algorithm_head.current_idx = scene_idx;
    algorithm_head.b_start = (bOverview << 1) | bStart;
    algorithm_head.data_len = (buff.count() + headBuff.count()) / 4;

    QByteArray packBuff;
    packBuff.append((const char *)&algorithm_head, sizeof(IG_Algorithm_Head));
    packBuff.append(headBuff);
    packBuff.append(buff);

    saveFile(DEFAULT_DEBUG_DIR"Algorithm save.txt", true, packBuff);

//    sendPacketSync(Msg_Type_NAP_AlgorithmSave, packBuff, PacketBase::UDP_WAIT_MS);
    //Msg_Type_NAP_AlgorithmSave包只有最后一个包有回复包，只能改为异步发送并sleep 50ms(经验值)
    sendPacket(Msg_Type_NAP_AlgorithmSave, packBuff);
    QThread::msleep(50);
    return true;
}

bool Device_Trn::sendAlgorithmOverviewPack(bool bStart, quint32 moduleCount, QByteArray buff)
{
    QByteArray headBuff = getGrobalHeadBuff(moduleCount);
    IG_Algorithm_Head algorithm_head;
    algorithm_head.current_idx = 0;
    algorithm_head.b_start = bStart;
    algorithm_head.data_len = (buff.count() + headBuff.count()) / 4;

    QByteArray packBuff;
    packBuff.append((const char *)&algorithm_head, sizeof(IG_Algorithm_Head));
    packBuff.append(headBuff);
    packBuff.append(buff);

    saveFile(DEFAULT_DEBUG_DIR"Algorithm Overview.txt", false, packBuff);
    return sendPacketSync(Msg_Type_NAP_AlgorithmOverview, packBuff, PacketBase::UDP_WAIT_MS);
}

bool Device_Trn::sendModuleNamePack(bool bStart, quint32 moduleCount, QByteArray buff)
{    
    QByteArray headBuff = getGrobalHeadBuff(moduleCount);
    IG_Algorithm_Head algorithm_head;
    algorithm_head.current_idx = 0;
    algorithm_head.b_start = bStart;
    algorithm_head.data_len = (buff.count() + headBuff.count()) / 4;

    QByteArray packBuff;
    packBuff.append((const char *)&algorithm_head, sizeof(IG_Algorithm_Head));
    packBuff.append(getGrobalHeadBuff(moduleCount));
    packBuff.append(buff);

    saveFile(DEFAULT_DEBUG_DIR"ModuleName.txt", false, packBuff);

    return sendPacketSync(Msg_Type_NAP_ModuleNameOverview, packBuff, PacketBase::UDP_WAIT_MS);
}

/*
 *            bit7 bit6 bit5 bit4 bit3             bit2              bit1           bit0
 * b_start     x    x    x    x   modulename data  modulename start  overview data   first download
 * */
bool Device_Trn::sendModuleNameSavePack(bool bStart, quint8 scene_idx, quint32 moduleCount, QByteArray buff)
{
    QByteArray headBuff = getGrobalHeadBuff(moduleCount);
    IG_Algorithm_Head algorithm_head;
    algorithm_head.current_idx = scene_idx;
    algorithm_head.b_start = (0x01 << 3) | (bStart << 2);
    algorithm_head.data_len = (buff.count() + headBuff.count()) / 4;

    QByteArray packBuff;
    packBuff.append((const char *)&algorithm_head, sizeof(IG_Algorithm_Head));
    packBuff.append(getGrobalHeadBuff(moduleCount));
    packBuff.append(buff);

    saveFile(DEFAULT_DEBUG_DIR"ModuleNameSave.txt", false, packBuff);

    return sendPacketSync(Msg_Type_NAP_ModuleNameOverviewSave, packBuff, PacketBase::UDP_WAIT_MS);
}

bool Device_Trn::sendAlgorithmDetailPack(quint32 moduleCount, QByteArray buff)
{
    QByteArray headBuff = getGrobalHeadBuff(moduleCount);
    IG_Algorithm_Head algorithm_head;
    algorithm_head.current_idx = 0;
    algorithm_head.b_start = 0;
    algorithm_head.data_len = (buff.count() + headBuff.count()) / 4;

    QByteArray packBuff;
    packBuff.append((const char *)&algorithm_head, sizeof(IG_Algorithm_Head));
    packBuff.append(getGrobalHeadBuff(moduleCount));
    packBuff.append(buff);

    saveFile(DEFAULT_DEBUG_DIR"Algorithm Detail.txt", true, packBuff);

    return sendPacketSync(Msg_Type_NAP_AlgorithmDetail, packBuff, PacketBase::UDP_WAIT_MS);
}

bool Device_Trn::sendAlgorithmStartPack(void)
{
    QByteArray array;

    return sendPacketSync(Msg_Type_NAP_AlgorithmStart, array, PacketBase::UDP_WAIT_MS);
}

bool Device_Trn::sendAlgorithmRealtimePack(quint32 moduleCount, QByteArray buff)
{
    QByteArray headBuff = getGrobalHeadBuff(moduleCount);
    IG_Algorithm_Head algorithm_head;
    algorithm_head.data_len = (buff.count() + headBuff.count()) / 4;

    QByteArray packBuff;
    packBuff.append((const char *)&algorithm_head, sizeof(IG_Algorithm_Head));
    packBuff.append(headBuff);
    packBuff.append(buff);

    saveFile(DEFAULT_DEBUG_DIR"Algorithm RealTime.txt", false, packBuff);

    return sendPacketSync(Msg_Type_NAP_OnlineUpdate, packBuff, PacketBase::UDP_WAIT_MS);
}

void Device_Trn::sendUpdatePackage(quint32 totalCount, quint32 offset, quint32 paraLen, QByteArray buff)
{
    QByteArray packBuff;
    packBuff.append((const char *)&totalCount, sizeof(quint32));
    packBuff.append((const char *)&offset, sizeof(quint32));
    packBuff.append((const char *)&paraLen, sizeof(quint32));
    packBuff.append(buff);
    sendPacket(MSG_TYPE_UPDATE_PACKAGE, packBuff);
}

void Device_Trn::sendIP(quint32 ip)
{
    QByteArray packBuff((const char *)&ip, sizeof(quint32));
    sendPacket(Msg_Type_NAP_SetIP, packBuff);
}

void Device_Trn::sendNetworkStreamModeSelect(int bank, quint32 mode)
{
    struct {
        unsigned char bank;
        unsigned char mode;
        unsigned short reserve;
    } Network;

    _configData.NetworkMode[bank] = mode;
    Network.bank = bank;
    Network.mode = mode;

    QByteArray packBuff((const char *)&Network, sizeof(Network));
    sendPacket(Msg_Type_NAP_NetworkStreamModeSelect, packBuff);
}

void Device_Trn::setUserName(int ch, QString newName)
{
    _userName.replace(ch, newName);
    mixerUserNameSave();
}

void Device_Trn::mixerUserNameLoad()
{
    QFile f(QString("config/%1.conf").arg(getAES70Mac()));
    if (f.open(QIODevice::ReadOnly) == false)
    {
        for (int i = 0 ; i < 40; ++i)
        {
            _userName << " ";
        }
        return;
    }

    QDataStream ds(&f);//实现C++基本数据类型的序列化
    while(!ds.atEnd())//没有到达结束位置
    {
        ds >> _userName;//读取数据
    }
    f.close();
}

void Device_Trn::saveFile(QString filename, bool bAppend, QByteArray array)
{
    QFile file(filename);
    if (bAppend)
    {                                       //读取时，行结束符转换为‘\n’   设备以附加的模式打开
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
            return;
        }
    } else {
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return;
        }
    }

    QTextStream ds(&file);//为读写文本提供一个方便的接口,纯文本类操作
    int len = array.count() / 4;

    ds << "====================================================" << endl;
    for (int i = 0; i < len; ++i)
    {
        quint32 temp;
        memcpy(&temp, array.constData()+ i * 4, sizeof(quint32));//.constData()函数返回一个指向存储在字节数组中的数据的职指针，可用于访问数组中的数据
        ds << QString("send %1 = 0x%2;").arg(i).arg(temp, 8, 16,QChar('0')) << endl;//写入数据
    }
    file.close();
}

void Device_Trn::mixerUserNameSave()
{
    QFile f(QString("config/%1.conf").arg(getAES70Mac()));
    f.open(QIODevice::WriteOnly);
    QDataStream ds(&f);

    ds << _userName;

    f.close();
}

struct LuaPack
{
    char* luaItemName[8];
    quint32 luaPinCount;
    quint32 luaScriptLen;
    char* dataBuff; //luaPinCount * sizeof(IG_PARACTL_LUAPIN) + luaScriptLen;
};

#define LuaPackLen  1024
bool Device_Trn::sendLuaPack(int downIdx, int luaItemIdx, QByteArray& luaPackBa)
{
    int len = luaPackBa.length();
    int subPackCnt = (len + LuaPackLen - 1) / LuaPackLen;
    luaPackBa.resize(subPackCnt*LuaPackLen); //不足LuaPackLen部分，用0补全
    for(int i = 1; i <= subPackCnt; i++)
    {
        int startPos = (i -1)* LuaPackLen;
        int dataLen = len - startPos;
        if(dataLen > LuaPackLen) dataLen= LuaPackLen;
        bool rs = sendLuaSubPack(downIdx, luaItemIdx, subPackCnt, i, dataLen, luaPackBa.data() + startPos);
        if(!rs)
        {
            return false;
        }
    }

    return false;
}


bool Device_Trn::sendLuaSubPack(quint32 downIdx, quint32 luaItemIdx, quint32 totalSubCnt,
                     quint32 subCntIdx, quint32 dataLen, const char* dataPtr)
{
    quint32 checkSum = 0;
    for(int i = 0; i < dataLen; i++){
        checkSum += (quint8)dataPtr[i];
    }
    QByteArray luaGram;
    QDataStream stream(&luaGram, QIODevice::ReadWrite);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << downIdx;
    stream << luaItemIdx;
    stream << totalSubCnt;
    stream << subCntIdx;
    stream << checkSum;
    stream << dataLen;
    stream.writeRawData(dataPtr, LuaPackLen); //dataPtr已经用0补齐LuaPackLen
    qDebug()<< QString("downIdx:%1 luaItemIdx:%2 totalSubCnt:%3 subCntIdx:%4 checkSum:%5 dataLen:%6")
               .arg(downIdx).arg(luaItemIdx).arg(totalSubCnt).arg(subCntIdx).arg(checkSum).arg(dataLen);
    QUdpSocket* _udpLuaItem = new QUdpSocket();
    bool rs = _udpLuaItem->bind(QHostAddress(""), 0, QUdpSocket::ShareAddress);
    if(!rs) return false;
    _udpLuaItem->writeDatagram(luaGram.data(), luaGram.length(), QHostAddress(getDeviceIp()), //QHostAddress("192.168.1.31"),
                                    PacketBase::LUAITEM_UDP_PORT);

    bool ready = _udpLuaItem->waitForReadyRead(2000);
    if(!ready) return false; //接收超时
    QByteArray gramBa;
    while (_udpLuaItem->hasPendingDatagrams())//如果有数据等待接受
    {
        gramBa.resize(_udpLuaItem->pendingDatagramSize());//用UDP数据报的大小来设置字节数组的大小
        QHostAddress sender;
        quint16 senderPort;
        //接受不大于字节数组大小的数据报，并将其存储在字节数组的data中，发送方的主机地址和端口号分别存储在sender和senderport中
        _udpLuaItem->readDatagram(gramBa.data(), gramBa.size(), &sender, &senderPort);
    }

    QDataStream retStream(&gramBa, QIODevice::ReadWrite);
    stream.setByteOrder(QDataStream::LittleEndian);
    quint16 retPackId, subPackId, isOk;
    retStream >> retPackId >> subPackId >> isOk;
    return isOk > 0;
}

void Device_Trn::reflashFiles()
{
    _fileList.clear();
    sendPacket_Nobody(Msg_Type_NAP_FileList);
}
