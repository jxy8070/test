#include "stable.h"
#include "solutionscene.h"
#include "solutionview.h"
#include "config.h"
#include "deviceItem/deviceItem.h"
#include "deviceItem/deviceitem_room.h"
#include "deviceItem/deviceitemfactory.h"
#include "deviceItem/deviceitem_trn.h"
#include "device/device_trn.h"
#include "device/devicemanager.h"
#include "deviceSetting/deviceproperty.h"
#include "deviceSetting/setmulticast.h"
#include "deviceItem/deviceitem_spk.h"
#include "device/device_spk.h"
#include "mainwindow.h"

SolutionScene::SolutionScene(AbsGraphView *pView) :
    AbsGraphScene(pView)
{
    _group = 5;
    _devItemIndex = 0;
    initMenuActions();
}


QGraphicsItem *SolutionScene::CopyGraphicsLineItem(QGraphicsItem *graphItem, QMap<QGraphicsItem *, QGraphicsItem *> itemMap)
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
                    p0->portNum() == line->getStartPort()->portNum())
             {
                for (ItemPort *p1 : DevItem1->getPortList(true)) {
                    if (p1->getPortType() == line->getEndPort()->getPortType() &&
                            p1->portNum() == line->getEndPort()->portNum())
                    {
                        return line->clone(p0, p1);
                    }
                }
            }
        }
    }

    return NULL;
}

//初始化环境菜单和动作
void SolutionScene::initMenuActions()
{
    _propertyAction = new QAction(tr("Property"), this);
    _multicastSet = new QAction(tr("Multicast Config"), this);
    _networkStreamModeSelect = new QAction(tr("Network Stream Mode"), this);
    connect(_propertyAction, SIGNAL(triggered()), this, SLOT(viewDeviceProperty()));
    connect(_multicastSet, SIGNAL(triggered()), this, SLOT(setMulticast()));
    connect(_networkStreamModeSelect, SIGNAL(triggered()), this, SLOT(onProjActionTriggered()));

    _contextMenu.addAction(_delAction);
    _contextMenu.addAction(_contextMenu.addSeparator());

    _contextMenu.addAction(_propertyAction);
    _contextMenu.addAction(_multicastSet);
    _contextMenu.addAction(_networkStreamModeSelect);
    initCommonMenuActions();
}

void SolutionScene::updateMenuActions()
{
    _propertyAction->setVisible(false);
    _multicastSet->setVisible(false);
    _networkStreamModeSelect->setVisible(false);

    AbsGraphScene::updateMenuActions();
}


void SolutionScene::onProjActionTriggered()
{
    QGraphicsItem *curItem = focusItem();
    if (sender() == _networkStreamModeSelect)//AES67和Dante混合模式时设置网络流格式
    {
        DeviceItem_Trn *trnItem = qgraphicsitem_cast<DeviceItem_Trn *>(curItem);
        trnItem->showNetworkStreamModeSelect();
    }
}

void SolutionScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    QString defMimDataType = QString("application/x-qabstractitemmodeldatalist");
    if(event->mimeData()->hasFormat(defMimDataType)) {
        QByteArray array = event->mimeData()->data(defMimDataType);
        QDataStream stream(&array, QIODevice::ReadOnly);
        QList<DeviceItem *> dragItems;
        int row, col;
        QMap<int, QVariant> dataMap;
        while(!stream.atEnd()) {
            stream >> row >> col >> dataMap;
            QString fileName = dataMap.value(0).toString();
//            QString filePath = dataMap.value(Qt::UserRole+1).toString();
            if(getRoomItem(fileName) != NULL) {
                event->ignore();
                MainWindow::instance()->showNotice(tr("%1 already existed!").arg(fileName));
                return;
            }else {
                DeviceItem *devItem = DeviceItemFactory::CreateInstance(Dev_Type_Room, QFileInfo(fileName).baseName(), event->scenePos());
                if(!devItem) continue;
                dragItems.append(devItem);
                qobject_cast<DeviceItem_Room *>(devItem)->loadFromSettings();
            }
        }
        if(dragItems.length() > 0) {
            QPointF pos(0, 0);
            foreach(DeviceItem *item, dragItems) {
                item->setOpacity(0.5);
                item->setPos(pos);
                pos += item->rect().bottomLeft();
                _tempGroup->addToGroup(item);
            }
            addTempGroup();
        }
    }
    event->acceptProposedAction();
}

void SolutionScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    AbsGraphScene::dragLeaveEvent(event);
}


void SolutionScene::viewDeviceProperty(void)
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


void SolutionScene::setMulticast()
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

        if (num_slots) {//下载组播路由
            DeviceManager::instance()->downloadMulticast(device, num_slots, id);
        }
    }
    else
    {
        MainWindow::instance()->showNotice(tr("This Device don't has any network port!"));
    }
}



void SolutionScene::open(QString fileName, QStringList projectList)
{
    _fileName = fileName;
    _view->setWindowTitle(QFileInfo(fileName).fileName());

    QMap<QString, QString> roomMap;
    for(QString projPath : projectList)
    {
        QFileInfo fileInfo(projPath);
        roomMap.insert(fileInfo.baseName(), projPath);
    }

    QSettings *ConfigIni = new QSettings(_fileName, QSettings::IniFormat,0);
    ConfigIni->beginGroup(PROJECT_GROUP);//添加到当前组
    ConfigIni->beginGroup(PROJECT_INFOMATION_GROUP);
    //    QSizeF sz = ConfigIni->value(PROJECT_SCENESIZE).toSizeF(); //废弃SceneSize，改为自适应
    quint32 devCount = ConfigIni->value(PROJECT_DEVICE_COUNT).toUInt();
    quint32 lineCount = ConfigIni->value(PROJECT_LINE_COUNT).toUInt();
    ConfigIni->endGroup();//将组重置为beginGrouponp()调用之前的值

    QProgressDialog *progressDlg = NULL;
    if(Config::ShowProgressDialog){
        progressDlg = new QProgressDialog;//关于缓慢操作的一个进度反馈
        progressDlg->setWindowModality(Qt::ApplicationModal);//模态窗口
        progressDlg->setMinimumDuration(1);//以1秒的延时进入进度条
        progressDlg->setWindowTitle(tr("please wait"));
        progressDlg->setLabelText(tr("Loading file......"));
        progressDlg->setCancelButton(0);//取消按钮不会被显示
        progressDlg->setRange(0,devCount+lineCount+1);//设置进度对话框的最小值和最大值
    }
    int proCount = 0;
    for (quint32 i = 0; i < devCount; ++i)// load device
    {
        if(Config::ShowProgressDialog) progressDlg->setValue(proCount++);
        loadDeviceItem(ConfigIni, QString(PROJECT_DEVICE_GROUP).arg(i), roomMap);
    }


    for (quint32 i = 0; i < lineCount; ++i)// load line
    {
        if(Config::ShowProgressDialog) progressDlg->setValue(proCount++);
        loadLineItem(ConfigIni, QString(PROJECT_LINE_GROUP).arg(i));
    }

    if(Config::ShowProgressDialog) progressDlg->setValue(devCount+lineCount+1);
    ConfigIni->endGroup();
    delete ConfigIni;

    autoFitSceneSize();
    setNeedSaveState(false);
}


void SolutionScene::saveAs(QString fileName)
{
    _fileName = fileName;
    save();//参数默认true
}

void SolutionScene::save(bool removeAll)
{
    QStringList strlist = _fileName.split("/");
    _view->setWindowTitle(strlist.last());//标题

    QSettings *ConfigIni = new QSettings(_fileName, QSettings::IniFormat, 0);
    quint32 devCount = 0;
    quint32 lineCount = 0;

    if (removeAll)
    {
        ConfigIni->clear();
    }
//    else
//    {
//        ConfigIni->remove(PROJECT_GROUP);
//    }
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
        DeviceItem_Room *roomItem = castToRoomItem(item);
        if(roomItem != NULL)
        {
            ConfigIni->beginGroup(QString(PROJECT_DEVICE_GROUP).arg(devCount++));
            ConfigIni->setValue(QString(PROJECT_DEVICE_DEVTYPE), roomItem->getType());
            ConfigIni->setValue(QString(PROJECT_DEVICE_DEVPOS), roomItem->scenePos());
            ConfigIni->setValue(QString(PROJECT_DEVICE_ZValue),  roomItem->getUserZValue());
            ConfigIni->setValue(QString(PROJECT_DEVICE_ISMONITOR), roomItem->isMonitor());
            ConfigIni->setValue(QString(PROJECT_DEVICE_ROOMPROJ), roomItem->roomProject());
            ConfigIni->setValue(QString(PROJECT_DEVICE_DEVNAME), roomItem->roomName());
            ConfigIni->setValue(QString(PROJECT_DEVICE_JOINSCENE), true);
            ConfigIni->endGroup();
        }
    }

    for (QGraphicsItem *item : items())
    {
        if (item->type() == LineItem::Type)
        {
            LineItem *connectLine = qgraphicsitem_cast<LineItem *>(item);
            ItemPort *startPort = connectLine->getStartPort();
            ItemPort *endPort   = connectLine->getEndPort();

            /* 取端口点的中间值，防止点位置超出三角形端口范围 */
            QPointF startPortPoint = startPort->centerPos();//获得端口的中间位置
            QPointF endPortPoint = endPort->centerPos();

            ConfigIni->beginGroup(QString(PROJECT_LINE_GROUP).arg(lineCount++));
            ConfigIni->setValue(QString(PROJECT_LINE_STARTPOS), startPortPoint);
            ConfigIni->setValue(QString(PROJECT_LINE_ENDPOS), endPortPoint);
            ConfigIni->endGroup();
        }
    }

    ConfigIni->beginGroup(PROJECT_INFOMATION_GROUP);
    ConfigIni->setValue(PROJECT_SCENESIZE, sceneRect().size());
    ConfigIni->setValue(PROJECT_DEVICE_COUNT, devCount);
    ConfigIni->setValue(PROJECT_LINE_COUNT, lineCount);
    ConfigIni->endGroup();

    ConfigIni->endGroup();
    delete ConfigIni;
    setNeedSaveState(false);
}


DeviceItem_Room* SolutionScene::castToRoomItem(QGraphicsItem* item)
{
    DesignItem* dItem = qgraphicsitem_cast<DesignItem*>(item);
    DeviceItem_Room* roomItem = qobject_cast<DeviceItem_Room*>(dItem);
    return roomItem;
}

void SolutionScene::deleteRoomItem(QString roomName)
{
    if(roomName.endsWith(".pro")) roomName.chop(4);
    foreach(QGraphicsItem *item, items()) {
        DeviceItem_Room *roomItem = castToRoomItem(item);
        if(!roomItem) continue;
        if(roomItem->roomName() == roomName) {
            removeItem(item);
            break;
        }
    }
}

void SolutionScene::renameRoomItem(QString oldName, QString newName)
{
    if(oldName.endsWith(".pro")) oldName.chop(4);
    if(newName.endsWith(".pro")) newName.chop(4);
    foreach(QGraphicsItem *item, items()) {
        DeviceItem_Room *roomItem = castToRoomItem(item);
        if(!roomItem) continue;
        if(roomItem->roomName() == oldName) {
            roomItem->setRoomName(newName);
            break;
        }
    }
}

DeviceItem_Room *SolutionScene::getRoomItem(QString roomName)
{
    if(roomName.endsWith(".pro")) roomName.chop(4);
    foreach(QGraphicsItem *item, items()) {
        DeviceItem_Room *roomItem = castToRoomItem(item);
        if(!roomItem) continue;
        if(roomItem->roomName() == roomName) {
            return roomItem;
        }
    }
    return nullptr;
}

void SolutionScene::loadDeviceItem(QSettings *ConfigIni, QString itemGroup, QMap<QString, QString> roomMap)
{
    ConfigIni->beginGroup(itemGroup);
    bool isJoinScene   = ConfigIni->value(QString(PROJECT_DEVICE_JOINSCENE)).toBool();
    if(!isJoinScene) {
        ConfigIni->endGroup();
        return ;
    }
    int devType        = ConfigIni->value(QString(PROJECT_DEVICE_DEVTYPE)).toInt();
    int ZValue         = ConfigIni->value(QString(PROJECT_DEVICE_ZValue)).toInt();
    QPointF scenePos   = ConfigIni->value(QString(PROJECT_DEVICE_DEVPOS)).toPointF();
    QString deviceName = ConfigIni->value(QString(PROJECT_DEVICE_DEVNAME)).toString();
//        QString mac = ConfigIni->value(QString(PROJECT_DEVICE_DEVMAC)).toString();
//        QString roomProj = ConfigIni->value(QString(PROJECT_DEVICE_ROOMPROJ)).toString();

    quint32 inputCount = ConfigIni->value(QString(SOLUTION_ROOM_INPUT_COUNT)).toUInt();
    quint32 outputCount= ConfigIni->value(QString(SOLUTION_ROOM_OUTPUT_COUNT)).toUInt();

//        bool isMonitor = ConfigIni->value(QString(PROJECT_DEVICE_ISMONITOR)).toBool();
    QMap<int, QList<QString>> roomInPorts;
    QMap<int, QList<QString>> roomOutPorts;
    for(quint32 i = 0; i < inputCount; i++) {
        ConfigIni->beginGroup(QString(SOLUTION_ROOM_INPUT_GROUP).arg(i));
        quint32 portType = ConfigIni->value(QString(SOLUTION_ROOM_PORT_TYPE)).toUInt();
        QString portName = ConfigIni->value(QString(SOLUTION_ROOM_PORT_NAME)).toString();
        ConfigIni->endGroup();

        QList<QString> values = roomInPorts.value(portType);
        values << (portName);
        roomInPorts.insert(portType, values);
    }
    for(quint32 i = 0; i < outputCount; i++) {
        ConfigIni->beginGroup(QString(SOLUTION_ROOM_OUTPUT_GROUP).arg(i));
        quint32 portType = ConfigIni->value(QString(SOLUTION_ROOM_PORT_TYPE)).toUInt();
        QString portName = ConfigIni->value(QString(SOLUTION_ROOM_PORT_NAME)).toString();
        ConfigIni->endGroup();

        QList<QString> values = roomOutPorts.value(portType);
        values << (portName);
        roomOutPorts.insert(portType, values);
    }

    ConfigIni->endGroup();
    DeviceItem *devItem = DeviceItemFactory::CreateInstance(DevType(devType), deviceName, scenePos);
    if(devItem != NULL)
    {
        DeviceItem_Room* room = qobject_cast<DeviceItem_Room*>(devItem);
        if(room != NULL ) {
//                room->InitProject(roomProj);
            if(roomMap.keys().contains(room->roomName())){
                room->setUserZValue(ZValue);

                QList<QGraphicsItem*> itemList{devItem};
                addGraphicsItems(itemList);
                roomMap.remove(room->roomName());
                room->setInputOutPorts(roomInPorts, roomOutPorts);
            }
            else{
                delete room;
            }
        }
    }
}

void SolutionScene::loadLineItem(QSettings *ConfigIni, QString itemGroup)
{
    ConfigIni->beginGroup(itemGroup);
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
//    qDebug() << __FUNCTION__ << __LINE__ << (startPort == NULL) << (endPort == NULL);
    if(!startPort || !endPort) return;
    LineItem *line = new LineItem(this, startPort, endPort);
    addGraphicsItems(QList<QGraphicsItem *>{line});
}
