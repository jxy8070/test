#include "stable.h"
#include "projectscene.h"
#include "projectview.h"
#include "config.h"
#include "deviceItem/deviceItem.h"
#include "deviceItem/deviceitemfactory.h"
#include "deviceItem/deviceitem_trn.h"
#include "device/device_trn.h"
#include "device/devicemanager.h"
#include "deviceSetting/deviceproperty.h"
#include "deviceSetting/setmulticast.h"
#include "deviceItem/deviceitem_spk.h"
#include "device/device_spk.h"
#include "linelabel.h"
#include "mainwindow.h"

ProjectScene::ProjectScene(AbsGraphView *pView) :
    AbsGraphScene(pView)
{
    _group = 5;
    _devItemIndex = 0;
    initMenuActions();

    connect(this, SIGNAL(updateSolutionRoomPorts(QString, QMap<int, QList<QString>>, QMap<int, QList<QString>>)),
            MainWindow::instance(), SLOT(updateSolutionRoomPorts(QString, QMap<int, QList<QString>>, QMap<int, QList<QString>>)));
}


QGraphicsItem *ProjectScene::CopyGraphicsLineItem(QGraphicsItem *graphItem, QMap<QGraphicsItem *, QGraphicsItem *> itemMap)
{
    if(graphItem->type() == LineItem::Type)
    {
        LineItem* line = qgraphicsitem_cast<LineItem *>(graphItem);
        DeviceItem* DevItem0 = qgraphicsitem_cast<DeviceItem *>(itemMap.value(line->getStartDevice()));
        DeviceItem* DevItem1 = qgraphicsitem_cast<DeviceItem *>(itemMap.value(line->getEndDevice()));
        if(DevItem0 == NULL || DevItem1 == NULL) {
            return NULL;
        }
        for (ItemPort *p0 : DevItem0->getPortList(false)) {
            if (p0->getPortType() == line->getStartPort()->getPortType() &&
                p0->portNum() == line->getStartPort()->portNum()) {
                for (ItemPort *p1 : DevItem1->getPortList(true)) {
                    if (p1->getPortType() == line->getEndPort()->getPortType() &&
                        p1->portNum() == line->getEndPort()->portNum()) {

                        return line->clone(p0, p1);
                    }
                }
            }
        }
    }

    return NULL;
}

//初始化环境菜单和动作
void ProjectScene::initMenuActions()
{
    _propertyAction = new QAction(tr("Property"), this);
    _multicastSet = new QAction(tr("Multicast Config"), this);
    _addToMonitor = new QAction(tr("Add To Monitor"), this);
    _removeFromMonitor = new QAction(tr("Remove From Monitor"), this);
    _algorithmAction = new QAction(tr("DSP Algorithms"), this);
    _networkStreamModeSelect = new QAction(tr("Network Stream Mode"), this);
    _ip_set = new QAction(tr("IP Set"), this);
    _firmware_update = new QAction(tr("Firmware Update"), this);
    connect(_propertyAction, SIGNAL(triggered()), this, SLOT(viewDeviceProperty()));
    connect(_multicastSet, SIGNAL(triggered()), this, SLOT(setMulticast()));
    connect(_addToMonitor, SIGNAL(triggered()), this, SLOT(onProjActionTriggered()));
    connect(_removeFromMonitor, SIGNAL(triggered()), this, SLOT(onProjActionTriggered()));
    connect(_algorithmAction, SIGNAL(triggered()), this, SLOT(onProjActionTriggered()));
    connect(_networkStreamModeSelect, SIGNAL(triggered()), this, SLOT(onProjActionTriggered()));
    connect(_ip_set, SIGNAL(triggered()), this, SLOT(onProjActionTriggered()));
    connect(_firmware_update, SIGNAL(triggered()), this, SLOT(onProjActionTriggered()));

    _contextMenu.addAction(_delAction);
    _contextMenu.addAction(_contextMenu.addSeparator());

    _contextMenu.addAction(_propertyAction);
    _contextMenu.addAction(_multicastSet);
    _contextMenu.addAction(_algorithmAction);
    _contextMenu.addAction(_ip_set);
    _contextMenu.addAction(_firmware_update);
    _contextMenu.addAction(_networkStreamModeSelect);
    _contextMenu.addAction(_addToMonitor);
    _contextMenu.addAction(_removeFromMonitor);
    initCommonMenuActions();
}

void ProjectScene::updateMenuActions()
{
    _propertyAction->setVisible(false);
    _multicastSet->setVisible(false);
    _removeFromMonitor->setVisible(false);
    _addToMonitor->setVisible(false);
    _algorithmAction->setVisible(false);
    _ip_set->setVisible(false);
    _firmware_update->setVisible(false);
    _networkStreamModeSelect->setVisible(false);

    DeviceItem * devItem = castToDeviceItem(focusItem());
    if (devItem != NULL)
    {
        /* 网络设备才需要显示属性选项 */
        if (devItem->isNetworkDevice())
        {
            _propertyAction->setVisible(true);
            if (devItem->isBind())
            {
                _multicastSet->setVisible(true);
                _ip_set->setVisible(true);
                _firmware_update->setVisible(true);
            }
            else
            {
                _multicastSet->setVisible(false);
            }
        }
        else
        {
            _propertyAction->setVisible(false);
            _multicastSet->setVisible(false);
        }

        if (devItem->canMonitor())
        {
             _removeFromMonitor->setVisible(false);
             _addToMonitor->setVisible(false);
            if (devItem->isMonitor()) _removeFromMonitor->setVisible(true);
            else  _addToMonitor->setVisible(true);
        }
        if(devItem->canCustomAlgorithm())
        {
            _algorithmAction->setVisible(true);;
            if (devItem->getType() == Dev_Type_NAP_Zynq7020_16Analog_16AES67DanteMixer)
            {
                _networkStreamModeSelect->setVisible(true);
            }
        }
    }

    AbsGraphScene::updateMenuActions();
}


void ProjectScene::onProjActionTriggered()
{
    QGraphicsItem *curItem = focusItem();
    if (sender() == _addToMonitor)
    {
        DeviceItem *device = qgraphicsitem_cast<DeviceItem *>(curItem);
        if (!device->isOnline())
        {
            MainWindow::instance()->showNotice(tr("This Device is offline!") +
                                               "\r\n" + device->getDeviceName());
            return;
        }
        device->setMonitor(true);
    }
    else if (sender() == _removeFromMonitor)
    {
        DeviceItem *device = qgraphicsitem_cast<DeviceItem *>(curItem);
        device->setMonitor(false);
    }
    else if (sender() == _algorithmAction)
    {
        DeviceItem_Trn *trnItem = qgraphicsitem_cast<DeviceItem_Trn *>(curItem);
        if(trnItem){
            trnItem->showSettingDialog();
        }
    }
    else if (sender() == _ip_set)//设置中控面板IP地址
    {
        DeviceItem *d = qgraphicsitem_cast<DeviceItem *>(curItem);
        d->showIPSet();
    }
    else if (sender() == _firmware_update)//固件更新
    {
        DeviceItem *d = qgraphicsitem_cast<DeviceItem *>(curItem);
        d->showFirmwareUpdate();
    }
    else if (sender() == _networkStreamModeSelect)//AES67和Dante混合模式时设置网络流格式
    {
        DeviceItem_Trn *trnItem = qgraphicsitem_cast<DeviceItem_Trn *>(curItem);
        trnItem->showNetworkStreamModeSelect();
    }
}


void ProjectScene::dragEnterEvent(QGraphicsSceneDragDropEvent *e)
{
//    resetTempGroup();
//    QList<DeviceItem*> addedItems;
//    QString mimeDataType = "mimeDataList";
//    if(e->mimeData()->hasFormat(mimeDataType)) {
//        QByteArray array = e->mimeData()->data(mimeDataType);
//        QDataStream stream(&array, QIODevice::ReadOnly);
//        QList<DeviceItem *> dragItems;
//        QPointF sPos = e->scenePos();
//        int row, col;
//        QMap<QString, QByteArray> dataMap;
//        while(!stream.atEnd()) {
//            stream >> row >> col >> dataMap;
//            DevType dType = DevType(dataMap.value("type").toInt());
//            qulonglong devPtr = dataMap.value("itemPtr").toULongLong();
//            AES70Device* aes70Dev = (AES70Device *) (devPtr);

//            if(!Config::curMfrsCfg().isValidDevice(dType))
//            {
//                e->ignore();
//                Utils::MessageBoxInfoAsync(tr("Information"), tr("can't drag unknown device!"));
//                return;
//            }
//            if(aes70Dev != 0 && !aes70Dev->getCurRoomName().isEmpty())
//            {
//                e->ignore();
//                Utils::MessageBoxInfoAsync(tr("Information"), tr("device is exist!"));
//                return;
//            }

//            QString devName = tr("not bind");
//            if(aes70Dev != NULL) {
//                devName = aes70Dev->getDeviceName();
//                aes70Dev->addedToProject(true);
//            }

//            DeviceItem* devItem = DeviceItemFactory::CreateInstance(DevType(dType),
//                                        devName, sPos);//显示位置待完善
//            if(devItem != NULL)
//            {
//                sPos = devItem->mapToScene(devItem->rect().topRight());
//                devItem->bindAES70Device(aes70Dev);
//                addedItems.append(devItem);
//            }
//        }
//    }

    resetTempGroup();
    QString typeStr = QString::fromUtf8(e->mimeData()->data("type"));
    QString ptrStr = QString::fromUtf8(e->mimeData()->data("itemPtr"));
    QStringList typeList = typeStr.split(",");
    QStringList ptrList = ptrStr.split(",");

    QList<DeviceItem*> addedItems;
    QPointF sPos = e->scenePos();
    for(int i= 0; i<typeList.length(); i++)
    {
        DevType dType = DevType(typeList[i].toInt());
        if(!Config::curMfrsCfg().isValidDevice(dType))
        {
            e->ignore();
            MainWindow::instance()->showNotice(tr("can't drag unknown device!"));
            return;
        }
        qulonglong devPtr = ptrList[i].toULongLong();
        AES70Device* aes70Dev = (AES70Device *) (devPtr);
//        if(aes70Dev != NULL && isDeviceExistInScene(aes70Dev))
        if(aes70Dev != 0 && !aes70Dev->getCurRoomName().isEmpty())
        {
            e->ignore();
            MainWindow::instance()->showNotice(tr("device is exist!"));
            return;
        }

        QString devName = tr("not bind");
        if(aes70Dev != NULL) {
            devName = aes70Dev->getDeviceName();
            aes70Dev->addedToProject(true);
        }
        QString typeName = Config::curMfrsCfg().getDevTypeName(dType);
        DeviceItem* devItem = DeviceItemFactory::CreateInstance(DevType(typeList[i].toInt()),
                                    devName, sPos);
        if(devItem != NULL)
        {
            sPos = devItem->mapToScene(devItem->rect().topRight());
            devItem->bindAES70Device(aes70Dev);
            addedItems.append(devItem);
        }

    }


    if(addedItems.count() > 0)
    {
        for(DeviceItem* child : addedItems)
        {
            child->setOpacity(0.5);
            _tempGroup->addToGroup(child);
        }

        addTempGroup();
    }
    e->acceptProposedAction();
}

void ProjectScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    AbsGraphScene::dragLeaveEvent(event);
}


bool ProjectScene::isDeviceExistInScene(AES70Device* aes70Dev)
{
    for(QGraphicsItem* item : items())
    {
        DeviceItem *devItem = castToDeviceItem(item);
        if(devItem != NULL && devItem->getAES70Device() == aes70Dev)
            return true;
    }
    return false;
}

void ProjectScene::viewDeviceProperty(void)
{
    DeviceItem *deviceItem = qgraphicsitem_cast<DeviceItem *>(focusItem());
    if(deviceItem !=NULL)
    {
        DeviceProperty* deviceProp = new DeviceProperty(deviceItem);
        deviceProp->exec();

        if (deviceItem->getType() == Dev_Type_NAP_Zynq7020_16Analog_16AES67DanteMixer)
        {
            DeviceItem_Trn *trnItem = qgraphicsitem_cast<DeviceItem_Trn *>(deviceItem);
            Device_Trn *trn = qobject_cast<Device_Trn*>(trnItem->getAES70Device());
            if(trn)
            {
                trnItem->replacePort(DeviceItem_Trn::Port_Network_Input_Bank1, DevPortType(trn->getConfigData().NetworkMode[DeviceItem_Trn::Port_Network_Input_Bank1]));
                trnItem->replacePort(DeviceItem_Trn::Port_Network_Input_Bank2, DevPortType(trn->getConfigData().NetworkMode[DeviceItem_Trn::Port_Network_Input_Bank2]));
                trnItem->replacePort(DeviceItem_Trn::Port_Network_Output_Bank1, DevPortType(trn->getConfigData().NetworkMode[DeviceItem_Trn::Port_Network_Output_Bank1]));
                trnItem->replacePort(DeviceItem_Trn::Port_Network_Output_Bank2, DevPortType(trn->getConfigData().NetworkMode[DeviceItem_Trn::Port_Network_Output_Bank2]));
            }
        }
    }
}


void ProjectScene::setMulticast()
{
    DeviceItem *device = qgraphicsitem_cast<DeviceItem *>(selectedItems().at(0));
    quint16 num_slots = 0;
    quint32 id[20] = {0};
    quint8 net_port_num = 0;
    /* 获取网络输出端口个数 */
    for (ItemPort *port : device->getPortList(false))
    {
        if (port->getPortType() == PortType_Dante)
        {
            net_port_num++;
        }
    }

    if (net_port_num)
    {
        /* 显示界面，获取需要设置组播的通道号 */
        SetMulticast *set_multicast = new SetMulticast(&num_slots, id, net_port_num, device);
        set_multicast->exec();

        if (num_slots){//下载组播路由
            DeviceManager::instance()->downloadMulticast(device, num_slots, id);
        }
    }
    else
    {
        MainWindow::instance()->showNotice(tr("This Device don't has any network port!"));
    }
}


void ProjectScene::open(QString filePath)
{
    disconnect(this, SIGNAL(updateSolutionRoomPorts(QString, QMap<int, QList<QString>>, QMap<int, QList<QString>>)), nullptr, nullptr);
    _filePath = filePath;
    QStringList strlist = _filePath.split("/");//在“/”位置分割字符串
    _view->setWindowTitle(strlist.last());

    QSettings *ConfigIni = new QSettings(_filePath, QSettings::IniFormat,0);

    ConfigIni->beginGroup(PROJECT_GROUP);//添加到当前组

    ConfigIni->beginGroup(PROJECT_INFOMATION_GROUP);
//    QSizeF sz = ConfigIni->value(PROJECT_SCENESIZE).toSizeF(); //废弃SceneSize，改为自适应
    quint32 devCount = ConfigIni->value(PROJECT_DEVICE_COUNT).toUInt();
    quint32 lineCount = ConfigIni->value(PROJECT_LINE_COUNT).toUInt();
    quint32 lineLabCount = ConfigIni->value(PROJECT_LINELAB_COUNT).toUInt();
    ConfigIni->endGroup();//将组重置为beginGrouponp()调用之前的值
    if(devCount+lineCount > 0)
    {
        QProgressDialog *progressDlg= NULL;
        if(Config::ShowProgressDialog)
        {
            progressDlg = new QProgressDialog;//关于缓慢操作的一个进度反馈
            progressDlg->setWindowModality(Qt::ApplicationModal);//模态窗口
            progressDlg->setMinimumDuration(1);//以1秒的延时进入进度条
            progressDlg->setWindowTitle(tr("please wait"));
            progressDlg->setLabelText(tr("Loading file......"));
            progressDlg->setCancelButton(0);//取消按钮不会被显示
            progressDlg->setRange(0,devCount+lineCount+lineLabCount);//设置进度对话框的最小值和最大值
        }
        int proCount = 0;
        for (quint32 i = 0; i < devCount; ++i)
        {
            if(Config::ShowProgressDialog) progressDlg->setValue(proCount++);
            ConfigIni->beginGroup(QString(PROJECT_DEVICE_GROUP).arg(i));
            int devType        = ConfigIni->value(QString(PROJECT_DEVICE_DEVTYPE)).toInt();
            int ZValue         = ConfigIni->value(QString(PROJECT_DEVICE_ZValue)).toInt();
            QPointF scenePos   = ConfigIni->value(QString(PROJECT_DEVICE_DEVPOS)).toPointF();
            QString deviceName = ConfigIni->value(QString(PROJECT_DEVICE_DEVNAME)).toString();
            QString mac = ConfigIni->value(QString(PROJECT_DEVICE_DEVMAC)).toString();
            bool isMonitor = ConfigIni->value(QString(PROJECT_DEVICE_ISMONITOR)).toBool();
            ConfigIni->endGroup();

            DeviceItem *devItem = DeviceItemFactory::CreateInstance(DevType(devType),
                                    deviceName, scenePos);
            if(devItem != NULL)
            {
                devItem->setUserZValue(ZValue);
                QList<QGraphicsItem*> itemList{devItem};
                addGraphicsItems(itemList);
                AES70Device* dev = DeviceManager::instance()->getDeviceByMac(mac);
                if(dev != NULL)
                {
                    devItem->bindAES70Device(dev);
                    dev->setDeviceName(deviceName, false);
                    //监控暂时不保存
                    devItem->setMonitor(isMonitor);
                }
            }
        }


        for (quint32 i = 0; i < lineCount; ++i)
        {
            if(Config::ShowProgressDialog) progressDlg->setValue(proCount++);
            ConfigIni->beginGroup(QString(PROJECT_LINE_GROUP).arg(i));
            QPointF startPoint = ConfigIni->value(QString(PROJECT_LINE_STARTPOS)).toPointF();
            QPointF endPoint   = ConfigIni->value(QString(PROJECT_LINE_ENDPOS)).toPointF();
            ConfigIni->endGroup();

            ItemPort *startPort = NULL;
            ItemPort *endPort = NULL;

            for (QGraphicsItem *item : items(startPoint))
            {
                if (item->type() == ItemPort::Type)
                {
                    startPort = qgraphicsitem_cast<ItemPort *>(item);
                    break;
                }
            }
            for (QGraphicsItem *item : items(endPoint))
            {
                if (item->type() == ItemPort::Type)
                {
                    endPort = qgraphicsitem_cast<ItemPort *>(item);
                    break;
                }
            }

            LineItem *line = new LineItem(this, startPort, endPort);
            addItem(line);
        }

        for(quint32 i = 0; i < lineLabCount; i++) {
            if(Config::ShowProgressDialog) progressDlg->setValue(proCount++);
            ConfigIni->beginGroup(QString(PROJECT_LINELABEL_GROUP).arg(i));
            QPointF portPoint = ConfigIni->value(QString(PROJECT_LINELABEL_POS)).toPointF();
            QString name      = ConfigIni->value(QString(PROJECT_LINELABEL_NAME)).toString();
            int ZValue        = ConfigIni->value(QString(PROJECT_LINELABEL_ZVALUE)).toInt();
            ConfigIni->endGroup();

            ItemPort *portItem = NULL;
            for(QGraphicsItem *item : items(portPoint)) {
                if(item->type() == ItemPort::Type) {
                    bool same = qgraphicsitem_cast<DeviceItem *>(item->parentItem())->checkZValue() == ZValue;
                    if(same) {
                        portItem = qgraphicsitem_cast<ItemPort *>(item);
                    }
                }
            }
            if(portItem) {
                LineLabel *lineLab = new LineLabel(portItem);
                lineLab->setName(name);
                lineLab->setUserZValue(qgraphicsitem_cast<DeviceItem *>(portItem->parentItem())->checkZValue());
                portItem->setLineLabel(lineLab);
                _labelList.append(lineLab);
            }
        }
        if(Config::ShowProgressDialog) progressDlg->setValue(progressDlg->maximum());
    }
    ConfigIni->endGroup();
    delete ConfigIni;

    matchLineLabels();
    autoFitSceneSize();
    setNeedSaveState(false);
    connect(this, SIGNAL(updateSolutionRoomPorts(QString, QMap<int, QList<QString>>, QMap<int, QList<QString>>)),
            MainWindow::instance(), SLOT(updateSolutionRoomPorts(QString, QMap<int, QList<QString>>, QMap<int, QList<QString>>)));
}


void ProjectScene::saveAs(QString filePath)
{
    save(filePath, true);//参数默认true
}

void ProjectScene::save(QString filePath, bool removeAll)
{
    QStringList strlist = filePath.split("/");
    _view->setWindowTitle(strlist.last());//标题

    QSettings *ConfigIni = new QSettings(filePath, QSettings::IniFormat, 0);
    quint32 devCount = 0;
    quint32 lineCount = 0;
    quint32 linelabelCount = 0;

    QMap<QString, LineLabel *> roomInLab;
    QMap<QString, LineLabel *> roomOutLab;

    if (removeAll)
    {
        ConfigIni->clear();
    }
    else
    {
        ConfigIni->remove(PROJECT_GROUP);
    }
    ConfigIni->beginGroup(PROJECT_GROUP);

    QList<QGraphicsItem *> itemListTemp;
    for (QGraphicsItem *item : items())
    {
        if (item->type() == DeviceItem::Type)
        {
            itemListTemp.append(item);
        }
    }

    /* 按照创建的先后顺序排列item列表 */
    for (int i = 0; i < itemListTemp.count(); ++i) {
        for (int j = i; j < itemListTemp.count(); ++j) {
            DesignItem* srcDesign = qgraphicsitem_cast<DesignItem *>(itemListTemp.at(i));
            DesignItem* dstDesign = qgraphicsitem_cast<DesignItem *>(itemListTemp.at(j));
            if (srcDesign->checkZValue() > dstDesign->checkZValue()) {
                itemListTemp.swapItemsAt(i, j);
            }
        }
    }

    /* 按照创建顺序，先创建的模块先保存 */
    for(QGraphicsItem *item : itemListTemp) {
        DeviceItem *devItem = qgraphicsitem_cast<DeviceItem *>(item);

        ConfigIni->beginGroup(QString(PROJECT_DEVICE_GROUP).arg(devCount++));

        ConfigIni->setValue(QString(PROJECT_DEVICE_DEVTYPE), devItem->getType());
        ConfigIni->setValue(QString(PROJECT_DEVICE_DEVPOS), devItem->scenePos());
        ConfigIni->setValue(QString(PROJECT_DEVICE_ZValue),  devItem->getUserZValue());
        ConfigIni->setValue(QString(PROJECT_DEVICE_ISMONITOR), devItem->isMonitor());

        AES70Device* danteDev = devItem->getAES70Device();
        if (danteDev != NULL)
        {
            ConfigIni->setValue(QString(PROJECT_DEVICE_DEVNAME), danteDev->getDeviceName());
            ConfigIni->setValue(QString(PROJECT_DEVICE_DEVMAC),  danteDev->getAES70Mac());
        }
        ConfigIni->endGroup();
    }
    for (QGraphicsItem *item : items())
    {
        if (item->type() == LineItem::Type)
        {
            LineItem *connectLine = qgraphicsitem_cast<LineItem *>(item);
            ItemPort *startPort = connectLine->getStartPort();
            ItemPort *endPort   = connectLine->getEndPort();
            int startZValue     = connectLine->getUserStartZValue();
            int endZValue       = connectLine->getUserEndZValue();

            /* 取端口点的中间值，防止点位置超出三角形端口范围 */
            QPointF startPortPoint = startPort->centerPos();//获得端口的中间位置
            QPointF endPortPoint = endPort->centerPos();

            ConfigIni->beginGroup(QString(PROJECT_LINE_GROUP).arg(lineCount++));
            ConfigIni->setValue(QString(PROJECT_LINE_STARTPOS), startPortPoint);
            ConfigIni->setValue(QString(PROJECT_LINE_ENDPOS), endPortPoint);
            ConfigIni->setValue(QString(PROJECT_LINE_STARTZVALUE), startZValue);
            ConfigIni->setValue(QString(PROJECT_LINE_ENDZVALUE), endZValue);
            ConfigIni->endGroup();
        }
        if(item->type() == LineLabel::Type) {
            LineLabel *lineLabItem = qgraphicsitem_cast<LineLabel *>(item);
            ItemPort *port = lineLabItem->getLabelPort();
            QPointF PortPoint = port->scenePos();

            ConfigIni->beginGroup(QString(PROJECT_LINELABEL_GROUP).arg(linelabelCount++));
            ConfigIni->setValue(QString(PROJECT_LINELABEL_POS), PortPoint);
            ConfigIni->setValue(QString(PROJECT_LINELABEL_NAME), lineLabItem->name());
            ConfigIni->setValue(QString(PROJECT_LINELABEL_ZVALUE), lineLabItem->getUserZValue());
            ConfigIni->endGroup();
            if(lineLabItem->name().startsWith(Appoint)) {
                if(lineLabItem->isInput())
                    roomInLab.insert(lineLabItem->name(), lineLabItem);
                else
                    roomOutLab.insert(lineLabItem->name(), lineLabItem);
            }
        }
    }
    Utils::saveRoomInputOutputForSolutionFile(QFileInfo(filePath).baseName(), roomInLab, roomOutLab);

    ConfigIni->beginGroup(PROJECT_INFOMATION_GROUP);
    ConfigIni->setValue(PROJECT_SCENESIZE, sceneRect().size());
    ConfigIni->setValue(PROJECT_DEVICE_COUNT, devCount);
    ConfigIni->setValue(PROJECT_LINE_COUNT, lineCount);
    ConfigIni->setValue(PROJECT_LINELAB_COUNT, linelabelCount);
    ConfigIni->endGroup();

    ConfigIni->endGroup();
    delete ConfigIni;

    for (QGraphicsItem *item : items())
    {
        if (item->type() == DeviceItem::Type)
        {
            DeviceItem *devItem = qgraphicsitem_cast<DeviceItem *>(item);

            AES70Device* danteDev = devItem->getAES70Device();
            if (danteDev != NULL && (devItem->getType() == Dev_Type_AES67_SPK
                                     || devItem->getType() == Dev_Type_STM32_POE_AES67_SPK_0_2 || devItem->getType() == Dev_Type_STM32_POE_AES67_SPK_0_1
                                     || devItem->getType() == Dev_Type_RT1052_POE_AES67_LINE_4_4
                                     || devItem->getType() == Dev_Type_RT1052_POE_AES67_MIC_4_0 ||
                                     devItem->getType() == Dev_Type_RT1052_POE_AES67_MIC_2_0 ||
                                     devItem->getType() == Dev_Type_RT1052_POE_AES67_MIC_2_0_Type1 ||
                                     devItem->getType() == Dev_Type_RT1052_POE_AES67_MIC_2_0_Type2 ||
                                     devItem->getType() == Dev_Type_RT1052_POE_AES67_MIC_2_0_Type3 ||
                                     devItem->getType() == Dev_Type_RT1052_POE_AES67_LINE_0_2 ||
                                     devItem->getType() == Dev_Type_RT1052_AP60 ||
                                     devItem->getType() == Dev_Type_RT1052_AP60_Type1 ||
                                     devItem->getType() == Dev_Type_RT1052_AP60_Type2 ||
                                     devItem->getType() == Dev_Type_RT1052_AP400 ||
                                     devItem->getType() == Dev_Type_RT1052_AP60_Bluetooth))
            {
                danteDev->saveAlarmSetting();
            }
        }
    }

    setNeedSaveState(false);
}


void ProjectScene::fileLoad(QString &filePath,  DevType loadType)
{
    QString offlineStr;
    QFile dsFile(filePath);
    dsFile.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QDataStream ds(&dsFile);
    switch (loadType) {
    case Dev_Type_AES67_SPK:
    {
        ConfigData_Spk configData;
        ds >> configData;
        for (QGraphicsItem *itemT : items())
        {
            if (itemT->type() == DeviceItem::Type)
            {
                DeviceItem *itm = qgraphicsitem_cast<DeviceItem *>(itemT);
                if (itm->getType() == Dev_Type_AES67_SPK)
                {
                    DeviceItem_Spk *spk = qgraphicsitem_cast<DeviceItem_Spk *>(itemT);
                    if (spk->isBind())
                    {
                        if (spk->isOnline())
                        {
                            Device_Spk *devSpk = qobject_cast<Device_Spk *>(spk->getAES70Device());
                            devSpk->initData(configData);
                            devSpk->sendAllConfigPack(false);
                        } else {
                            offlineStr += tr("%1 is offline").arg(spk->getAES70Device()->getDeviceName()) + "\n";
                        }
                    }
                }
            }
        }
        break;
    }
    case Dev_Type_STM32_POE_AES67_SPK_0_2:
//    {
//        ConfigData_STM32_POE_0_2 configData;
//        ds >> configData;
//        for (QGraphicsItem *itemT : items())
//        {
//            if (itemT->type() == DeviceItem::Type)
//            {
//                DeviceItem *itm = qgraphicsitem_cast<DeviceItem *>(itemT);
//                if (itm->getType() == Dev_Type_STM32_POE_AES67_SPK_0_2)
//                {
//                    DeviceItem_Amp *amp = qgraphicsitem_cast<DeviceItem_Amp *>(itemT);
//                    if (amp->isBind())
//                    {
//                        if (amp->isOnline())
//                        {
//                            Device_Amp *devAmp = qobject_cast<Device_Amp *>(amp->getAES70Device());
//                            devAmp->initData(configData);
//                            devAmp->sendAllConfigPack(false);
//                        } else {
//                            offlineStr += tr("%1 is offline").arg(amp->getAES70Device()->getDeviceName()) + "\n";
//                        }
//                    }
//                }
//            }
//        }
//        break;
//    }
    case Dev_Type_STM32_POE_AES67_SPK_0_1:
    {
//        ConfigData_Amp configData;
        break;
    }
    case Dev_Type_RT1052_POE_AES67_LINE_4_4:
    {
        ConfigData_Amp4_4 configdata;
        break;
    }
    case Dev_Type_RT1052_POE_AES67_MIC_4_0:
    {
        ConfigData_Mic_Quad cfgData;
        break;
    }
    case Dev_Type_AES67_MONITOR_SPK:
    {
        ConfigData_Monitor_SPK configdata;
        break;
    }
    case Dev_Type_DNTE_TRN:
    {
        ConfigData_Trn configData;
        ds >> configData;
        for (QGraphicsItem *itemT : items())
        {
            if (itemT->type() == DeviceItem::Type)
            {
                DeviceItem *itm = qgraphicsitem_cast<DeviceItem *>(itemT);
                if (itm->getType() == Dev_Type_DNTE_TRN)
                {
                    DeviceItem_Trn *trn = qgraphicsitem_cast<DeviceItem_Trn *>(itemT);
                    if (trn->isBind())
                    {
                        if (trn->isOnline())
                        {
                            Device_Trn *devTrn = qobject_cast<Device_Trn *>(trn->getAES70Device());
                            devTrn->initData(configData);
                        } else {
                            offlineStr += tr("%1 is offline").arg(trn->getAES70Device()->getDeviceName()) + "\n";
                        }
                    }
                }
            }
        }
        break;
    }
    default:
        break;
    }

    if (offlineStr.isEmpty())
    {
        offlineStr = tr("Load Success");
    } else {
        offlineStr += tr("Please Retry!");
    }
    MainWindow::instance()->showNotice(offlineStr);
}


QList<AES70Device*> ProjectScene::getNotBindDevList(DevType type)
{
    QList<AES70Device*> devList = DeviceManager::instance()->getDevListByType(type);

    for (QGraphicsItem *item : items())
    {
        DeviceItem* devItem = qgraphicsitem_cast<DeviceItem *>(item);
        if(devItem != NULL && devItem->getType() == type)
        {
            devList.removeOne(devItem->getAES70Device());
        }
    }

    return devList;
}

QStringList ProjectScene::getBindedMacList()
{
    QStringList macList;
    for (QGraphicsItem *item : items())
    {
        DeviceItem* devItem = castToDeviceItem(item);
        if(devItem != NULL)
        {
            AES70Device* dev = devItem->getAES70Device();
            if(dev!=NULL)  macList.append(dev->getAES70Mac());
        }
    }

    return macList;
}

void ProjectScene::onMonitorChanged(DeviceItem* devItem)
{
    Q_UNUSED(devItem)
    emit monitorChanged();
}


DeviceItem* ProjectScene::castToDeviceItem(QGraphicsItem* item)
{
    DesignItem* dItem = qgraphicsitem_cast<DesignItem*>(item);
    DeviceItem* devItem = qobject_cast<DeviceItem*>(dItem);
    return devItem;
}

