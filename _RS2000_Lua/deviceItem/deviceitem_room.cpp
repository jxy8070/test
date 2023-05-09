#include "deviceitem_room.h"
#include "Frame/mainwindow.h"

DeviceItem_Room::DeviceItem_Room(QString projFilePath, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(Dev_Type_Room, QString(), QImage(NULL), scenePos, 160, 10, parent)
{
    _textLength = 51;
    _fillColor = QColor(100,160,120,255);
    _isValid = true;

    _projFilePath = MainWindow::instance()->getSolutionPath(projFilePath+".pro");
    QFileInfo fileInfo(_projFilePath);
    _roomName = fileInfo.baseName();
}

void DeviceItem_Room::setInputOutPorts(QMap<int, QList<QString>> Inports, QMap<int, QList<QString>> Outports)
{
    memset(_inPortCountArray, 0, sizeof(_inPortCountArray));
    memset(_outPortCountArray, 0, sizeof(_outPortCountArray));

    clearInOutPorts();
//qDebug() << __FUNCTION__ << __LINE__ << Inports << Outports;
    int idx = 0;
    QMapIterator<int, QList<QString>> inIter(Inports);
    while(inIter.hasNext()) {
        inIter.next();
        for(QString name : inIter.value()) {
            if((DevPortType)inIter.key() == PortType_MONAURAL) _inPortCountArray[0]++;
            if((DevPortType)inIter.key() == PortType_AES67)    _inPortCountArray[1]++;
            if((DevPortType)inIter.key() == PortType_Dante)    _inPortCountArray[2]++;
            if((DevPortType)inIter.key() == PortType_USB)      _inPortCountArray[3]++;
            addInPort((DevPortType)inIter.key(), name, idx++);
        }
    }
    idx = 0;
    QMapIterator<int, QList<QString>> outIter(Outports);
    while(outIter.hasNext()) {
        outIter.next();
        for(QString name : outIter.value()) {
            if((DevPortType)outIter.key() == PortType_MONAURAL) _inPortCountArray[0]++;
            if((DevPortType)outIter.key() == PortType_AES67)    _inPortCountArray[1]++;
            if((DevPortType)outIter.key() == PortType_Dante)    _inPortCountArray[2]++;
            if((DevPortType)outIter.key() == PortType_USB)      _inPortCountArray[3]++;
            addOutPort((DevPortType)outIter.key(), name, idx++);
        }
    }
}

void DeviceItem_Room::loadFromSettings()
{
    QSettings *ConfigIni = new QSettings(MainWindow::instance()->getSolutionPath(), QSettings::IniFormat, 0);
    ConfigIni->beginGroup(PROJECT_GROUP);//添加到当前组
    ConfigIni->beginGroup(PROJECT_INFOMATION_GROUP);
    quint32 devCount = ConfigIni->value(PROJECT_DEVICE_COUNT).toUInt();
    ConfigIni->endGroup();
    for(int i = 0; i < devCount; i++) {
        ConfigIni->beginGroup(QString(PROJECT_DEVICE_GROUP).arg(i));

        QString deviceName = ConfigIni->value(QString(PROJECT_DEVICE_DEVNAME)).toString();
        if(deviceName != roomName()) {
            ConfigIni->endGroup();
            continue;
        }
        quint32 inputCount = ConfigIni->value(QString(SOLUTION_ROOM_INPUT_COUNT)).toUInt();
        quint32 outputCount= ConfigIni->value(QString(SOLUTION_ROOM_OUTPUT_COUNT)).toUInt();
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
        setInputOutPorts(roomInPorts, roomOutPorts);
        break;
    }
}

void DeviceItem_Room::setRoomName(QString name)
{
    _roomName = name;
    _projFilePath = MainWindow::instance()->getSolutionPath(_roomName+".pro");
}


void DeviceItem_Room::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    int maxPort = _inPortList.count() > _outPortList.count()? _inPortList.count() : _outPortList.count();
    if(maxPort)
        setHeight(25+14*maxPort+_interval);
    QRectF rcBoundOut = QRectF(0, 0, _rectWidth,  _rectHeight).adjusted(-0.5, -0.5, 0.5, 0.5);
    QRectF rcHead = QRectF(0, 0, _rectWidth,  18).adjusted(0.5, 0.5, -0.5, -0.5);
    QRectF rcBody = QRectF(0, 0, _rectWidth,  _rectHeight).adjusted(0, rcHead.height(), 0, 0);

    int h, s, l;
    _fillColor.getHsl(&h, &s,&l);
    QColor bordColor = QColor::fromHsl(h, s, l+ 30);
    QColor bodyColor = QColor(107, 107, 107);
    QColor portNameColor = QColor(180, 180, 180);
    painter->setPen(QPen(_fillColor, 1));
    painter->setBrush(_fillColor);
    painter->drawRoundedRect(rcBoundOut, 5, 5);

    QPainterPath bodyPath = getRoundRectPath(rcBody, 0, 0, 5, 5);
    painter->fillPath(bodyPath, QBrush(bodyColor));

    painter->setBrush(portNameColor);
    painter->setPen(QPen(bodyColor, 1));

    for(int i = 0; i< maxPort; i++)
    {
        painter->drawRoundedRect(QRectF(12, Port_Start_Y+14*i, _rectWidth -24,  12), 3, 3);
    }

    painter->setPen(QPen(Config::curSkinStyle()->DevItemTitilColor, 1));
    painter->drawText(rcHead, Qt::AlignHCenter, _roomName);

    //显示边框
    if (isSelected()) {
        if(hasFocus())  painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));
        else  painter->setPen(QPen(Qt::red, 1, Qt::DashLine));
    }
    else {
        painter->setPen(QPen(bordColor, 1));//Qt::white
    }
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(rcBoundOut, 5, 5);

    if(!_isValid) {
        painter->setBrush(QColor(246,96,64, 80));
        painter->drawRoundedRect(rcBoundOut, 5, 5);
        painter->setFont(QFont("Microsoft YaHei", 24));
        painter->drawText(rcBoundOut, Qt::AlignCenter , "?");
    }
}

void DeviceItem_Room::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)//鼠标双击事件
{
    Q_UNUSED(event)
    if(!_isValid) {
        MainWindow::instance()->showNotice(tr("This module is invalid!"));
        return ;
    }
    qDebug() << __FUNCTION__ << __LINE__ << _projFilePath;
    MainWindow::instance()->openSubWindow(_projFilePath);
}

//DesignItem* DeviceItem_Room::clone(DesignItem* toItem)
//{
//    qDebug() << __FUNCTION__ << __LINE__;
//    if(toItem == NULL) toItem = new DeviceItem_Room(_devType, _defaultDeviceItemName, scenePos(), NULL);
//    return toItem;
//}

