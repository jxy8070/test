#include "stable.h"

#include "devicemanager.h"
#include "AES70device.h"
#include "device_trn.h"
#include "device_mic_quad.h"
#include "devicemodel.h"
#include "Protocal/packetbase.h"
#include "Frame/projectscene.h"
#include "Frame/solutionscene.h"
#include "deviceItem/deviceItem.h"
#include "AES70Lib/SDK_INCLUDE/AES70ManagerExport.h"
#include "AES70Lib/SDK_INCLUDE/AES70Connectors.h"
#include <Windows.h>
#include "linelabel.h"
#include "Frame/mainwindow.h"

#ifdef DanteSupported
#include "DanteSdk/dantebrowser.h"
#include "DanteSdk/danterouting.h"
#endif
DeviceManager::DeviceManager(QObject *parent) : QObject(parent)
{
    //初始化主播Udp
    _udpBroadcast = new QUdpSocket;
    _udpBroadcast->bind(QHostAddress(Config::instance()->getCurIp()), 9001, QUdpSocket::ShareAddress);//允许其他服务绑定到相同的地址和端口
    if(!_udpBroadcast->joinMulticastGroup(QHostAddress(PacketBase::GROUPCAST_IP), Config::instance()->getCurItr()))//加入指定的多播组
    {
        Utils::MessageBoxWarning(tr("Warning"), tr("Error, Check the host ip address(%1)!")
                              .arg(Config::instance()->getCurItr().humanReadableName()));
    }
    connect(_udpBroadcast, SIGNAL(readyRead()), this, SLOT(broadcastPackHandler()));//数据从协议栈到达Qt应用程序，则触发一次readyRead()信号

    //初始化设备Udp
    _udpDevice = new UdpSocketServer(Config::instance()->getCurIp(), 9527);
    connect(_udpDevice, SIGNAL(packetReceived(PacketBase*)), this, SLOT(devicePackHandler(PacketBase*)), Qt::QueuedConnection);


    //初始化dante Browser，如果启用dante
    _danteBrowser = NULL;
#ifdef DanteSupported
    _danteBrowser = new DanteBrowser;
    connect(_danteBrowser, SIGNAL(deviceRemoved(QString)), this, SLOT(setDeviceOffline(QString)));
    connect(_danteBrowser, SIGNAL(deviceAdded(QString, QString, QString, QString)),
            this, SLOT(onDeviceAdded(QString, QString, QString, QString)));
    _danteBrowser->open();
#endif

    //初始化aes70 Browser
    _pAES70Browser = AES70_OpenDeviceManager();
    if (_pAES70Browser != NULL)
    {
        AES70_SettingAppNetIf(_pAES70Browser, Config::instance()->getCurIpIndex(), 0);
        AES70_StartManager(_pAES70Browser, Config::instance()->getCurIpIndex(), 0,
                           0, 0, 0, 0, NULL, 0, NULL);

        _AES70DevFindTimer = new QTimer(this);
        connect(_AES70DevFindTimer, SIGNAL(timeout()), this, SLOT(flashAES70DeviceList()));
        _AES70DevFindTimer->start(AES70FINDER_PERIOD * 1000);
    }

    //设备确认定时任务
    _confirmTimer  = new QTimer(this);
    connect(_confirmTimer, SIGNAL(timeout()),this, SLOT(deviceConfirmHandler()));
    _confirmTimer->start(CONFIRM_PERIOD * 1000);

    _pluggingOutCable = false;
    _deviceModel = new DeviceModel(this);//发现在线设备的列表
}

DeviceManager::~DeviceManager()
{
    _udpBroadcast->close();
    delete _udpBroadcast;
    delete _udpDevice;
    if(_danteBrowser != NULL)
        delete _danteBrowser;
}

void DeviceManager::setDeviceOffline(QString mac)
{
    AES70Device* device = getDeviceByMac(mac);
    if(device != NULL)
    {        
        device->setOffline();
        _deviceModel->updateDevice(device);
        emit deviceStateChanged(device);
    }
}


void DeviceManager::onDeviceAdded(QString mac, QString ip, QString devName, QString devDesp)
{
    bool isSoundcard = false;
    bool isAES70Found = true;
    QString creator = "AES70";
    if(devDesp != "AES70") {
        isAES70Found = false;
        creator = "Dante";
    }
    DevType devType = Dev_Type_Unknow;
    if(isAES70Found)
    {
        if(devName.endsWith("_VS") || devName.endsWith("_ASIO")) {//兼容WDM、VSIO虚拟声卡
            isSoundcard = true;
            devType = Dev_Type_VirtualSoundCard_AES67;
        }
    }
    else {
        if(devDesp.startsWith("Dante Virtual Soundcard")) {
            isSoundcard = true;
            devType = Dev_Type_VirtualSoundCard_Dante;
        }
        else{
            _DanteIpNameMap.insert(ip, devName); //Dante设备只认虚拟网卡，其他设备只记录Map
        }
    }

    AES70Device* device = getDeviceByMac(mac);  //通过mac地址获得设备
    if(device == NULL) {//新建设备
        device = new AES70Device(mac, devName, devType, ip, creator);
        _deviceModel->insertDevice(0, device);
    }
    else{
        device->updateAES70Info(devName, ip, creator);
    }
    device->setUpdateTime();
    //将虚拟声卡的初始状态置为online
    if(isSoundcard) {
        device->switchStatus(Dev_Online);
    }
    else{
       device->switchStatus(Dev_AES70Found);
    }
    _deviceModel->updateDevice(device);//刷新mode
    emit deviceStateChanged(device);
}

void DeviceManager::cleanup()
{
#ifdef DanteSupported
    if(_danteBrowser != NULL) {
        delete _danteBrowser;
    }
#endif
    delete _udpBroadcast;
    delete _udpDevice;
    delete _deviceModel;
}


//发送广播指令MSG_TYPE_SEARCH_DEVS 搜索设备
void DeviceManager::sendBroadcastPack()
{
    PacketBase* broadcast = PacketBase::createBroadcastPacket(Msg_Type_Probe);
    qint64 ret = _udpBroadcast->writeDatagram(broadcast->getBuff(), QHostAddress(PacketBase::GROUPCAST_IP), PacketBase::GROUPCAST_PORT);

    //网线拔出后需要重新加入组播才能接收到组播消息
    if (_udpBroadcast->error() == QAbstractSocket::NetworkError && ret != -1 && _pluggingOutCable)
    {
#ifdef QT_DEBUG
        qDebug() << __FUNCTION__ << __LINE__
                 << broadcast->getBuff().toHex() << _udpBroadcast->state()
                 << _udpBroadcast->error() << _udpBroadcast->errorString();
#endif
        //重新指定的多播组留在操作系统选择的默认接口上
        _udpBroadcast->leaveMulticastGroup(QHostAddress(PacketBase::GROUPCAST_IP), Config::instance()->getCurItr());
        disconnect(_udpBroadcast, SIGNAL(readyRead()), this, SLOT(broadcastPackHandler()));
        _udpBroadcast->joinMulticastGroup(QHostAddress(PacketBase::GROUPCAST_IP), Config::instance()->getCurItr());
        connect(_udpBroadcast, SIGNAL(readyRead()), this, SLOT(broadcastPackHandler()));
    }

    if (ret == -1) {
        _pluggingOutCable = true;
    }
    else {
        _pluggingOutCable = false;
    }
}

//确认设备是否在线
void DeviceManager::deviceConfirmHandler()
{
    QList<AES70Device*> devList = _deviceModel->getDeviceList();//设备列表
    foreach(AES70Device* dev, devList)
    {
        if (dev->getDeviceType() == Dev_Type_VirtualSoundCard_AES67 ||
            dev->getDeviceType() == Dev_Type_VirtualSoundCard_Dante ||
            dev->getDeviceType() == Dev_Type_Unknow ||
            dev->isOnline() == false) {
            continue; //虚拟声卡及离线设备不需要确认
        }

        dev->confirmOnline(CONFIRM_PERIOD);
    }
}

QString getMacString(CAES70Device& aes70Dev)
{
    QString mac = QString("%1%2%3%4%5%6")
            .arg(aes70Dev.m_Info.m_FirmwareInfo.m_MAC_mDNS[0][0], 2, 16, QLatin1Char('0'))
            .arg(aes70Dev.m_Info.m_FirmwareInfo.m_MAC_mDNS[0][1], 2, 16, QLatin1Char('0'))
            .arg(aes70Dev.m_Info.m_FirmwareInfo.m_MAC_mDNS[0][2], 2, 16, QLatin1Char('0'))
            .arg(aes70Dev.m_Info.m_FirmwareInfo.m_MAC_mDNS[0][3], 2, 16, QLatin1Char('0'))
            .arg(aes70Dev.m_Info.m_FirmwareInfo.m_MAC_mDNS[0][4], 2, 16, QLatin1Char('0'))
            .arg(aes70Dev.m_Info.m_FirmwareInfo.m_MAC_mDNS[0][5], 2, 16, QLatin1Char('0'));
    return mac;
}

QString getIpString(CAES70Device& aes70Dev)
{
    QString ip = QString("%1.%2.%3.%4")
            .arg((aes70Dev.m_Info.m_FirmwareInfo.m_nIpService[0] >> 24) & 0xFF)
            .arg((aes70Dev.m_Info.m_FirmwareInfo.m_nIpService[0] >> 16) & 0xFF)
            .arg((aes70Dev.m_Info.m_FirmwareInfo.m_nIpService[0] >>  8) & 0xFF)
            .arg((aes70Dev.m_Info.m_FirmwareInfo.m_nIpService[0] >>  0) & 0xFF);
    return ip;
}



void DeviceManager::flashAES70DeviceList()
{
    std::vector<CAES70Device> DeviceList;
    AES70_GetDeviceList(_pAES70Browser, DeviceList);

    QSet<QString> curMacSet;
    std::vector<CAES70Device>::iterator it;
    for(it = DeviceList.begin(); it != DeviceList.end(); ++it)
    {
        QString DevMac = getMacString(*it);
        QString DevIP = getIpString((*it));
        QString DevName = QString::fromStdWString((*it).m_Info.m_FirmwareInfo.m_strDeviceNameU);
        curMacSet.insert(DevMac);
//        qDebug() << __FUNCTION__ << __LINE__ << DevName << DevMac << DevIP;

        if (!_AES70DeviceNameMap.contains(DevMac))//新设备添加
        {
            QMutexLocker locker(&_MapMutex);
            _AES70DeviceNameMap.insert(DevMac, DevName);
            onDeviceAdded(DevMac, DevIP, DevName, "AES70");
        }else if(_AES70DeviceNameMap[DevMac] != DevName) { //覆盖旧设备名称
            QMutexLocker locker(&_MapMutex);
            _AES70DeviceNameMap[DevMac] = DevName;
            onDeviceAdded(DevMac, DevIP, DevName, "AES70");
        }
    }

    //记录这次未发现的在线设备，并设置为离线
    QStringList macList;
    QMap<QString, QString>::const_iterator iter;
    for (iter = _AES70DeviceNameMap.constBegin(); iter != _AES70DeviceNameMap.constEnd(); ++iter)
    {
        if (!curMacSet.contains(iter.key())) {
            macList.append(iter.key());
        }
    }
    for(QString mac : macList) {
        setDeviceOffline(mac);
    }

    getDevSocketInfo(QStringList()<<"x86-14"); //wj3
    sendBroadcastPack();
}

//获取用于路由下载的设备信息
QList<DevSocketInfo> DeviceManager::getDevSocketInfo(QStringList devNameList)
{
    std::vector<CAES70Device> DeviceList;
    AES70_GetDeviceList(_pAES70Browser, DeviceList);
    QList<DevSocketInfo> socketInfoList;
    std::vector<CAES70Device>::iterator it;
    for(it = DeviceList.begin(); it != DeviceList.end(); ++it)
    {
        QString devName = QString::fromStdWString(it->GetDeviceNameU());
        if(devNameList.contains(devName))
        {
            DevSocketInfo sockInfo;
//            sockInfo.DevName = devName;
//            sockInfo.TxConnectorNum = it->GetRxConnectors();
//            sockInfo.RxConnectorNum =
            sockInfo.MaxPinNum = it->GetMaxPortsPerPin();
            sockInfo.TxChNum = it->GetTxChNum();
            sockInfo.RxChNum = it->GetRxChNum();
            socketInfoList.append(sockInfo);

            qDebug() << devName << sockInfo.MaxPinNum << sockInfo.TxChNum << sockInfo.RxChNum; //wj3
        }
    }
    return socketInfoList;
}


bool DeviceManager::sendPacket(AES70Device *device, PacketBase* pack)
{
    QMutexLocker locker(&_Mutex);//加互斥锁。
    return _udpDevice->sendPacket(device, pack);
}


bool DeviceManager::sendPacketSync(AES70Device *device, PacketBase* pack,
                                    int waitMs, quint16 msgType, quint16 msgIndex)
{
    QMutexLocker locker(&_Mutex);//加互斥锁。
    return _udpDevice->sendPacketSync(device, pack, waitMs, msgType, msgIndex);
}

//处理组播包
void DeviceManager::broadcastPackHandler()
{
    while (_udpBroadcast->hasPendingDatagrams())
    {
        //接收并解析数据包
        QByteArray packBuff;
        packBuff.resize(_udpBroadcast->pendingDatagramSize());//用UDP数据报的大小来设置字节数组的大小
        QHostAddress sender;
        quint16 senderPort;
        _udpBroadcast->readDatagram(packBuff.data(), packBuff.size(), &sender, &senderPort);
        PacketBase* pack = PacketBase::createPacketByBuff(packBuff);
        if(pack == NULL ) continue;
        QScopedPointer<PacketBase> packPtr(pack);

//        qDebug() << __FUNCTION__ << __LINE__ << sender.toString() << pack->getDeviceIP() << pack->getDeviceMac()
//                 << pack->getMfrsType() << pack->getDeviceType() << pack->getMsgType();
        QString devMac = packPtr->getDeviceMac();
        AES70Device* device = getDeviceByMac(devMac.toLower());//根据mac地址获得设备类型
        if(device == NULL || device->getStatus() == Dev_Offline) {
            continue;
        }

        if(device->getDeviceType() != pack->getDeviceType())
        {   //根据数据报中的设备类型创建一个新的设备类型
            AES70Device* newDev = AES70Device::createDevice(devMac, device->getDeviceName(),
                                                            pack->getDeviceType(), pack->getDeviceIP(), device->getDevCreator());
            if (newDev == NULL){
                continue;
            }
            _deviceModel->replaceDevice(device, newDev);//新设备替换旧设备
            delete device;
            qDebug() << "$$" << __FUNCTION__ << "replaceDevice(device, newDev)";
            device = newDev;
        }
        else if(device->getDeviceIp() != pack->getDeviceIP())
        {
            device->setDeviceIp(pack->getDeviceIP());  //如果设备的IP与数据包中不同，则设置为数据包中的IP
            _deviceModel->updateDevice(device);//刷新mode
            emit deviceStateChanged(device);
        }
        device->setUpdateTime();
        device->setMfrsType(pack->getMfrsType());

        //混合传输器需要添加Dante的设备IP和名称
        if (pack->getDeviceType() == Dev_Type_NAP_Zynq7020_16Analog_16AES67DanteMixer)
        {
            device->setDanteIP(pack->getBodyTrn().NetInfo.dante.getIp());
            QString danteName = _DanteIpNameMap.value(pack->getBodyTrn().NetInfo.dante.getIp());
            if (!danteName.isEmpty())
            {
                device->setDanteName(danteName);
            }
        }

        DevStatus dev_status = Dev_Initialized;
        if (pack->getDeviceType() == Dev_Type_Zynq7010_AES3ToAES67_DongFangJiaLian ||
                pack->getDeviceType() == Dev_Type_RT1052_DR3020 ) {
            dev_status = Dev_Online;
        }

        //如果设备状态改变，则刷新
        if(device->switchStatus(dev_status)) {
            emit deviceStateChanged(device);
        }

        //对设备发送初始化包
        if(device->getStatus() == Dev_Initialized) {
            device->sendInitPack();
        }
    }
}

//处理设备Udp包
void DeviceManager::devicePackHandler(PacketBase* pack)
{
//   qDebug() << __FUNCTION__ << __LINE__ << pack << "start!";
    QScopedPointer<PacketBase> packPtr(pack);

    //改为通过Mac地址分发数据包
    AES70Device* device = getDeviceByMac(pack->getDeviceMac());

    if(device != NULL)
    {
        device->processPacket(pack);//根据接受数据报中包含数据类型进行反馈
        device->setUpdateTime();//返回当前日期时间
    }
}

//获得设备列表，type为Dev_Type_Unknow时获取所有类别
QList<AES70Device*> DeviceManager::getDevListByType(DevType type, bool onlineOnly)
{
    return _deviceModel->getDeviceListByType(type, onlineOnly);
}

bool DeviceManager::setDeviceName(AES70Device *device, QString NewName)
{
    CM3_ERROR cm3_err = 0;
//    DWORD devIp = ipAddress_StringToBinary(device->getDeviceIp());
    DWORD msgID = AES70_GetMsgID(_pAES70Browser);
    CAES70Device CAES70Dev;
    bool isSucceed = AES70_GetDeviceByIp(_pAES70Browser, /*devIp*/ (LPCTSTR)device->getDeviceIp().utf16(), CAES70Dev);
    if(!isSucceed) return isSucceed;
    QThread::msleep(50);
    isSucceed = AES70_SetDeviceName(_pAES70Browser, msgID+1, CAES70Dev.GetDeviceIp(),
                                    CAES70Dev.GetServicePortNoCfg(), (LPCTSTR)NewName.utf16(), cm3_err, true);
    return isSucceed;
}

unsigned long DeviceManager::ipAddress_StringToBinary(QString ip)
{
    unsigned long devIp = 0;
    int idx = 0, i = 0;
    QListIterator<QString> iter(ip.split("."));
    iter.toBack();
    while(iter.hasPrevious()){
        QString ipStr = iter.previous();
        bool ok = false;
        uint num = ipStr.toUInt(&ok, 10);
        i = 0;
        while(i < 8) {
            devIp |= (num % 2) << idx;
            num /= 2;
            idx++; i++;
        }
    }
    return devIp;
}

AES70Device* DeviceManager::getDeviceByMac(QString mac)//通过mac地址获得设备
{
    return _deviceModel->getDeviceByMac(mac.toLower());
}


void DeviceManager::downloadSolutionRoute(SolutionScene* scene)
{
    //wj 待实现
}

void DeviceManager::downloadProjectRoute(ProjectScene* scene)
{
    qint16 group;
    QString Warning;
    QList<DeviceItem*> devItemList;
    QList<LineItem*> linkItemList;
    QList<LineLabel *> inputLabList;
    QList<LineLabel *> outputLabList;
    //提供一个关于缓慢操作的进度反馈
    QProgressDialog progressDlg(Q_NULLPTR,
        Qt::Widget | Qt::CustomizeWindowHint | Qt::WindowTitleHint);//widget窗口的默认类型，
                                                                    //CustomizeWindowHint关闭默认窗口的标题提示
                                                                    //WindowTitleHint为窗口提供标题栏
    progressDlg.setWindowModality(Qt::ApplicationModal);//设置模态窗口
    progressDlg.setWindowTitle(tr("Download......"));//设置窗口标题
    progressDlg.setCancelButton(0);//取消按钮不被显示
    progressDlg.setMinimumDuration(1);//保存对话框出现之前必须经过的时间
    progressDlg.setRange(0,100);//设置进度对话框的最小值为0，最大值为100
    progressDlg.show();
    int process = 1;
    progressDlg.setValue(process++);//保存当前进度量
    progressDlg.setValue(process);

    group = scene->getGroup();//获得分组序号
    //获取需要处理的DeviceItem及，已经绑定dante
    foreach (QGraphicsItem *item, scene->items())
    {
        if (item->type() == DeviceItem::Type)//type()返回类型值（所有标准的item与唯一的Type值相关联）
        {
            DeviceItem *devItem = qgraphicsitem_cast<DeviceItem *>(item);
            if (devItem->isNetworkDevice() && devItem->isBind())
            {
#if Logo_BeiJingHuiSheng
                if(devItem->getAES70Device()->getDeviceType() == Dev_Type_NAP_Zynq7020_16Analog_16AES67 ||
                   devItem->getAES70Device()->getDeviceType() == Dev_Type_NAP_Zynq7020_16Analog_48AES67)
                {
                    Device_Trn *trn = qobject_cast<Device_Trn *>(devItem->getAES70Device());
                    trn->setGroup(group);
                    qDebug()<<"group:"<<trn->getGroup();
                    group++;
                }
#endif
                devItemList.append(devItem);//添加到设备列表中
            }
        }

        if (item->type() == LineItem::Type)
        {
            LineItem *linkItem = qgraphicsitem_cast<LineItem *>(item);
            DeviceItem* DevItem0 = qgraphicsitem_cast<DeviceItem *>(linkItem->getStartDevice());
            DeviceItem* DevItem1 = qgraphicsitem_cast<DeviceItem *>(linkItem->getEndDevice());
            if (DevItem0->isNetworkDevice() && DevItem1->isNetworkDevice())
            {
                linkItemList.append(linkItem);
            }
        }

        if(item->type() == LineLabel::Type) {
            LineLabel *lineLabItem = qgraphicsitem_cast<LineLabel *>(item);
            if(!lineLabItem->isMatched()) continue;
            if(lineLabItem->isInput())
                inputLabList.append(lineLabItem);
            else
                outputLabList.append(lineLabItem);
        }
    }

    if(devItemList.count() == 0)//如果设备列表为空
    {
        MainWindow::instance()->showNotice(tr("no device need to process"));
        return;
    }

    //清除路由设置
    float sep = 48. / devItemList.count();
    foreach (DeviceItem *devItem, devItemList)
    {
        if (devItem->isOnline())//清除路由设置
        {
            //AES70发现的设备
            if (devItem->getAES70Device()->getDevCreator() == "AES70" &&
                    devItem->getAES70Device()->getDeviceType() != Dev_Type_NAP_Zynq7020_16Analog_16AES67DanteMixer) /* AES70设备 */
            {
                CM3_ERROR cm3_err = 0;
                AES70_Delete_AES67_ALL_SinkConnectors(_pAES70Browser, (LPCTSTR)devItem->getAES70Name().utf16(), cm3_err, false, E_COM_INTERFACE::eAutoSelectInterface);
                AES70_Delete_AES67_ALL_SourceConnectors(_pAES70Browser, (LPCTSTR)devItem->getAES70Name().utf16(), cm3_err, false, E_COM_INTERFACE::eAutoSelectInterface);

                QThread::msleep(100);
            }
#ifdef DanteSupported
            else {
                DanteRouting route.setup(devItem->getAES70Device()->getDanteName());
                int j = 0;
                foreach (ItemPort *portItem, devItem->getPortList(true)) {
                    if (portItem->getPortType() == PortType_Dante){ //如果是输入网络端口
                        route.process_router(++j, NULL, 0);
                        QThread::msleep(100);
                    }
                }
                route.cleanup();

                QThread::msleep(100);
                /* 清除组播状态 */
                route.setup(devItem->getAES70Device()->getDanteName());
                route.process_delete_txMultipleFlow();
                route.cleanup();
                QThread::msleep(100);
            }
#endif
        }
        else {
            Warning += tr("%1 is offline.").arg(devItem->getDeviceName()) + "\r\n";
        }
        process += sep;
        progressDlg.setValue(process);
    }
    process = 50;
    progressDlg.setValue(process);

    //重新设置路由
    sep = 50. / (linkItemList.count() + devItemList.count() + inputLabList.count());
    foreach (LineItem *linkItem, linkItemList)
    {
        DeviceItem* DevItem0 = qgraphicsitem_cast<DeviceItem *>(linkItem->getStartDevice());
        DeviceItem* DevItem1 = qgraphicsitem_cast<DeviceItem *>(linkItem->getEndDevice());
        if (DevItem0->isOnline() && DevItem1->isOnline())
        {
            if (linkItem->getStartPort()->getPortType() == PortType_AES67)//如果是网络端口
            {
#if Logo_BeiJingHuiSheng
                if (DevItem1->getType() == Dev_Type_NAP_Zynq7020_16Analog_16AES67 ||
                    DevItem1->getType() == Dev_Type_NAP_Zynq7020_16Analog_48AES67)
                {
                    Device_Trn *trn = qobject_cast<Device_Trn *>(DevItem1->getAES70Device());
                    trn->sendGroup();
//                        qDebug() << __FUNCTION__ << __LINE__
//                                 << DevItem0->getType() << Dev_Type_RT1052_POE_AES67_MIC_4_0;
                    if (DevItem0->getType() == Dev_Type_RT1052_POE_AES67_MIC_4_0)
                    {
                        Device_Mic_Quad *mic = qobject_cast<Device_Mic_Quad *>(DevItem0->getAES70Device());
                        mic->setGroup(trn->getGroup());
                        mic->sendGroup(linkItem->getStartPort()->portNum(), linkItem->getEndPort()->portNum());
//                            qDebug()<<"group:"<<group;
//                            group++;
                    }

                }
#endif
                CM3_ERROR cm3_err = 0;
                AES70_ConnectChannel(_pAES70Browser,
                                        (LPCTSTR)DevItem0->getAES70Name().utf16(), linkItem->getStartPort()->portNum(),
                                        (LPCTSTR)DevItem1->getAES70Name().utf16(), linkItem->getEndPort()->portNum(),
                                        cm3_err, E_COM_INTERFACE::eAutoSelectInterface);
                QThread::msleep(100);
            }
#ifdef DanteSupported
            if (linkItem->getStartPort()->getPortType() == PortType_Dante) {
                if (DevItem0->getType() != Dev_Type_NAP_Zynq7020_16Analog_16Dante &&
                    DevItem1->getType() != Dev_Type_NAP_Zynq7020_16Analog_16Dante &&
                    DevItem0->getType() != Dev_Type_NAP_Zynq7020_16Analog_16Dante_Type1 &&
                    DevItem1->getType() != Dev_Type_NAP_Zynq7020_16Analog_16Dante_Type1)
                {
                    QString outdeviceName = DevItem0->getAES70Device()->getDanteName();
                    quint16 outdevicePort = linkItem->getStartPort()->portNum() + 1;
                    QString indeviceName = DevItem1->getAES70Device()->getDanteName();
                    quint16 indevicePort = linkItem->getEndPort()->portNum() + 1;

                    DanteRouting route.setup(indeviceName);
                    route.process_router(indevicePort, outdeviceName, outdevicePort);
                    route.cleanup();
                    QThread::msleep(100);
                }
            }
#endif
        }
        process += sep;
        progressDlg.setValue(process);
    }

    foreach (LineLabel *inLab, inputLabList) {
        if(inLab->getPortType() == PortType_AES67) {
            DeviceItem *devItem0 = 0, *devItem1 = 0;
            int portNum0 = 0, portNum1 = 0;
            foreach(LineLabel *outLab, outputLabList) {
                if(inLab->name() == outLab->name()) {
                    devItem0 = qgraphicsitem_cast<DeviceItem *>(outLab->getLabelPort()->parentItem());
                    devItem1 = qgraphicsitem_cast<DeviceItem *>(inLab ->getLabelPort()->parentItem());
                    portNum0 = outLab->getLabelPort()->portNum();
                    portNum1 = inLab ->getLabelPort()->portNum();
                    break;
                }
            }
            if(!devItem0 || !devItem1) continue;
            CM3_ERROR cm3_err = 0;
            AES70_ConnectChannel(_pAES70Browser,
                                    (LPCTSTR)devItem0->getAES70Name().utf16(), portNum0,
                                    (LPCTSTR)devItem1->getAES70Name().utf16(), portNum1,
                                    cm3_err, E_COM_INTERFACE::eAutoSelectInterface);
            QThread::msleep(100);
        }
        process += sep;
        progressDlg.setValue(process);
    }
    //重新设置路由完成后，进度条的值，为100
    progressDlg.setValue(100);

    if (Warning != NULL) {
        Utils::MessageBoxWarning(tr("Warning"), Warning);
    }
    else{
        MainWindow::instance()->showNotice(tr("Download Over!"));
    }
}

void DeviceManager::downloadMulticast(DeviceItem *devItem, quint16 num_slots, quint32 *id)
{
    QString warning;
    if (devItem->isOnline())//如果设备在线
    {
        if (devItem->getAES70Device()->getDevCreator() != "AES70")  //Dante发现设备
        {
#ifdef DanteSupported
            DanteRouting route.setup(devItem->getAES70Name()); /* 清除组播状态 */
            route.process_delete_txMultipleFlow();
            QThread::msleep(100);

            /* 设置组播 */
            if (num_slots)
            {
                if (num_slots > 8){
                    int tmp1 = num_slots / 8;
                    int tmp2 = num_slots % 8;
                    int offset = 0;
                    for (int i = 0; i < tmp1; ++i){
                        route.process_add_txMultipleFlow(8, &id[offset]);
                        QThread::msleep(100);
                        offset += 8;
                    }

                    route.process_add_txMultipleFlow(tmp2, &id[offset]);
                }
                else{
                    route.process_add_txMultipleFlow(num_slots, id);
                }

                QThread::msleep(100);
                route.cleanup();
            }
#endif
        }
    } else {
        warning += tr("%1 is offline.").arg(devItem->getDeviceName()) + "\r\n";
    }

    if (warning != NULL)
    {
        QMessageBox::information(NULL, tr("Warning"), warning);
    }
    else
    {
        QMessageBox::information(NULL, tr("Information"), tr("Download Over!"));
        if (devItem->getType() != Dev_Type_VirtualSoundCard_Dante)//如果设备类型不是虚拟声卡
        {
            QMessageBox::warning(NULL, tr("Multicast Config"), tr("Please power off the %1 for 1 minute!").arg(devItem->getAES70Device()->getDeviceName()));
        }
    }
}

void DeviceManager::removeofflineAES70Device(QString mac)
{
    QMutexLocker locker(&_MapMutex);
    if (_AES70DeviceNameMap.contains(mac)) {
        _AES70DeviceNameMap.remove(mac);
    }
}

////////////////////////////////////////////////////////
/// \brief Singleton模式相关代码
QMutex DeviceManager::_Mutex;//实例互斥锁。
DeviceManager* DeviceManager::_Instance; //<使用原子指针,默认初始化为0。

DeviceManager* DeviceManager::instance()
{
    //使用双重检测。
    if(!_Instance)//第一次检测
    {
        QMutexLocker locker(&_Mutex);//加互斥锁。

        if(!_Instance)//第二次检测。
            _Instance = new DeviceManager();
    }

    return _Instance;
}

void DeviceManager::release()
{
    if(_Instance) {
        QMutexLocker locker(&_Mutex);
        delete _Instance;
        _Instance = NULL;
    }
}
