#include "stable.h"

#include "deviceItem.h"
#include "concretedeviceitem.h"
#include "device/AES70device.h"
#include "device/devicemanager.h"
#include "deviceItem/deviceitemfactory.h"
#include "Frame/projectscene.h"
#include "deviceSetting/devsetting_setip.h"
#include "deviceSetting/devsetting_firmwareupdate.h"
#include <QMetaEnum>
#include "customWidget/basewindow.h"
#include "Frame/mainwindow.h"
#include "linelabel.h"

DeviceItem::DeviceItem(DevType devType, QString deviceName, QImage image,
                       QPointF scenePos, qreal rectWidth, qreal rectHeight,
                       QGraphicsItem *parent) :  DesignItem()
{
    Q_UNUSED(parent)
    setFlags(QGraphicsRectItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable |
             QGraphicsRectItem::ItemIsMovable    | QGraphicsItem::ItemSendsGeometryChanges);//通过单机项目或者使用框选来选择项目

    setAcceptHoverEvents(true);//接受悬停事件
    _textLength = 41;
    _rectWidth = rectWidth;
    _rectHeight = rectHeight;

    setRect(0, 0,_rectWidth, _rectHeight);
    setPos(scenePos);//设置项的位置为scenePos

    _interval = 0;
    _proxyItem = NULL;

    _devType = devType;
    _image = image;
    _isMonitor = false;

    _devStatus = Dev_Offline;
    _timer = new QTimer();
    connect(_timer, &QTimer::timeout, this, &DeviceItem::onTimerChangedSlot);

    _fillColor = Config::curMfrsCfg().getDeviceFillColor(devType);
    _curFillColor = _fillColor;

//    qDebug() << __FUNCTION__ << __LINE__ << this->setProperty("InAPortCount", _inPortCountArray[0]);
    connect(this, SIGNAL(portDisplayCountChanged()), this, SLOT(portDisplayCountChangedSlots()));

    initDisplayInOutPortCount();

    //kemu风格的在线状态插头图形
    _isOnlinePath.moveTo(12, 9);
    _isOnlinePath.arcTo(2,2,20,14, 90, 180);
    _isOnlinePath.addRect(11, 5, 5, 2);
    _isOnlinePath.addRect(11, 11, 5, 2);
}

void DeviceItem::initDisplayInOutPortCount()
{
    memset(_inPortCountArray, 0, sizeof(uint)*4);
    memset(_outPortCountArray, 0, sizeof(uint)*4);
    _openGroup = true;
    uint8_t isUsing = 0xFF;
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) isUsing = 0xEE;// FT: 无USB端口
    Utils::devItemPorts_t ports = Utils::getDeviceItemPortNum(_devType, isUsing);
//    qDebug() << __FUNCTION__ << __LINE__ << ports.s_In.s_ANum << ports.s_In.s_NNum << ports.s_In.s_DNum << ports.s_In.s_UNum
//             << ports.s_Out.s_ANum << ports.s_Out.s_NNum << ports.s_Out.s_DNum << ports.s_Out.s_UNum;

    _inPortCountArray[0] = ports.s_In.s_ANum;
    _inPortCountArray[1] = ports.s_In.s_NNum;
    _inPortCountArray[2] = ports.s_In.s_DNum;
    _inPortCountArray[3] = ports.s_In.s_UNum;
    _outPortCountArray[0] = ports.s_Out.s_ANum;
    _outPortCountArray[1] = ports.s_Out.s_NNum;
    _outPortCountArray[2] = ports.s_Out.s_DNum;
    _outPortCountArray[3] = ports.s_Out.s_UNum;

    for(int i = 0; i < 8; i++) {
        int a = i / 4;
        int b = i % 4;
        if(a == 0) this->setData(i, _inPortCountArray[b]);
        if(a == 1) this->setData(i, _inPortCountArray[b]);
    }
    if(_devType == Dev_Type_DNTE_AMP ||
       _devType == Dev_Type_RT1052_POE_AES67_MIC_2_0 ||
       _devType == Dev_Type_RT1052_POE_AES67_MIC_2_0_Type1 ||
       _devType == Dev_Type_RT1052_POE_AES67_MIC_2_0_Type2 ||
       _devType == Dev_Type_RT1052_POE_AES67_MIC_2_0_Type3 ||
       _devType == Dev_Type_RT1052_POE_AES67_MIC_8_0 ||
       _devType == Dev_Type_RT1052_POE_AES67_LINE_0_2 ||
       _devType == Dev_Type_RT1052_POE_AES67_LINE_0_8 ||
       _devType == Dev_Type_RT1052_POE_AES67_MIC_4_0 ||
       _devType == Dev_Type_Room) _openGroup = false;//关闭分组功能

    initInOutPorts();
}

void DeviceItem::initInOutPorts()
{
    _interval = 0;
    int count = 0;
    /// Analog In And Out
    uint Acount = _inPortCountArray[0] < _outPortCountArray[0] ? _outPortCountArray[0] : _inPortCountArray[0];
    for(uint i = 0; i < Acount; i++) {
        if(i < _inPortCountArray[0]) addInPort(PortType_MONAURAL, QString(), i);
        if(i < _outPortCountArray[0]) addOutPort(PortType_MONAURAL, QString(), i);
        modifyDistanceByGroup8(i, Acount, _interval);
    }
#if Logo_FangTu
    if(Acount && _openGroup) {
        QPointF pos = QPointF(Port_End_X,Port_Start_Y+14*Acount+_interval);
        ItemSeparator* sep = new ItemSeparator(true, pos, _rectWidth-Port_End_X, this);
        _separatorList.append(sep);
        _interval += 10;
    }
#endif
    /// Network In And Out
    uint Ncount = _inPortCountArray[1] < _outPortCountArray[1] ? _outPortCountArray[1] : _inPortCountArray[1];
    for(uint i = 0; i < Ncount; i++) {
        if(i < _inPortCountArray[1]) addInPort(PortType_AES67, QString(), i);
        if(i < _outPortCountArray[1]) addOutPort(PortType_AES67, QString(), i);
        modifyDistanceByGroup8(i, Ncount, _interval);
    }
    /// Dante In And Out
    uint Dcount = _inPortCountArray[2] < _outPortCountArray[2] ? _outPortCountArray[2] : _inPortCountArray[2];
    for(uint i = 0; i < Dcount; i++) {
        if(i < _inPortCountArray[2]) addInPort(PortType_Dante, QString(), i);
        if(i < _outPortCountArray[2]) addOutPort(PortType_Dante, QString(), i);
        modifyDistanceByGroup8(i, Dcount, _interval);
    }
    ///USB In And Out不绘制在DeviceItem中


    if(_openGroup){
        for(int idx = 0; idx < 3; idx++) {    //4=>3：不计算USB端口数量
            count += _inPortCountArray[idx] > _outPortCountArray[idx] ? _inPortCountArray[idx] : _outPortCountArray[idx];
        }
    }
    else{
        count  = getInPortCount() > getOutPortCount() ? getInPortCount() : getOutPortCount();
    }

    if(count == 0) count = 8;//无端口，用作显示
    setHeight(25+14*count+_interval);

    update();
}

DesignItem* DeviceItem::clone(DesignItem* from)
{
    ProjectScene *projScene = qobject_cast<ProjectScene *>(scene());
    QString devName = QString("Device_%1").arg(projScene->getDevItemIndex());
    DeviceItem* item = DeviceItemFactory::CreateInstance(_devType,
                                                         devName,
                                                         scenePos());
    return item;
}

int DeviceItem::checkZValue(void)
{
    return _UserZValue;
}

void DeviceItem::onDeviceStatusChangedSlots(DevStatus devStatus)
{
    if(_devStatus ==_device->getStatus()){
        return;
    }
     _devStatus = _device->getStatus();
    _curFillColor = _fillColor;
    if(_devStatus == Dev_Offline)
    {
        _timer->start(500);
    }
    else if(_devStatus == Dev_Online && _timer->isActive())
    {
        _timer->stop();
    }
    MainWindow::instance()->initTreeProperty(this);
    update();
}

void DeviceItem::onTimerChangedSlot()
{
    if(_curFillColor == _fillColor)
    {
        _curFillColor = Qt::red;
    }else {
        _curFillColor = _fillColor;
    }
    this->update();
}


void DeviceItem::setHeight(int height)
{
    _rectHeight = height;
    setRect(0, 0,_rectWidth, _rectHeight);
}

void DeviceItem::clearInOutPorts()
{
    foreach(ItemPort *item, _inPortList) {
        for(QGraphicsItem *gItem : this->scene()->items(Qt::AscendingOrder)) {
            if(gItem->type() == LineItem::Type) {
                LineItem *LItem = qgraphicsitem_cast<LineItem *>(gItem);
                if(LItem->getEndPort() == item) {
                    LItem->clearBeforeDelete();
                    this->scene()->removeItem(LItem);
                    delete LItem;
                }
            }else if(gItem->type() == LineLabel::Type) {
                LineLabel *labItem = qgraphicsitem_cast<LineLabel *>(gItem);
                if(labItem->getLabelPort() == item) {
                    this->scene()->removeItem(labItem);
                    delete labItem;
                }
            }
        }
        _inPortList.removeAll(item);
        delete item;
    }
    foreach(ItemPort *item, _outPortList) {
        for(QGraphicsItem *gItem : this->scene()->items(Qt::AscendingOrder)) {
            if(gItem->type() == LineItem::Type) {
                LineItem *LItem = qgraphicsitem_cast<LineItem *>(gItem);
                if(LItem->getStartPort() == item) {
                    this->scene()->removeItem(LItem);
                    delete LItem;
                }
            }else if(gItem->type() == LineLabel::Type) {
                LineLabel *labItem = qgraphicsitem_cast<LineLabel *>(gItem);
                if(labItem->getLabelPort() == item) {
                    this->scene()->removeItem(labItem);
                    delete labItem;
                }
            }
        }
        _outPortList.removeAll(item);
        delete item;
    }

    foreach(ItemSeparator *item, _separatorList) {
        _separatorList.removeAll(item);
        delete item;
    }
}

void DeviceItem::portDisplayCountChangedSlots()
{
//    qDebug() << __FUNCTION__ << __LINE__ << _inPortCountArray[0] << _inPortCountArray[1] << _inPortCountArray[2]
//             << _outPortCountArray[0] << _outPortCountArray[1] << _outPortCountArray[2];

    clearInOutPorts();

    initInOutPorts();
}

void DeviceItem::addInPort(DevPortType type, QString portName,
                         int portNum)
{
    int idx = portNum;
    if((type == PortType_AES67 || type == PortType_Dante) && _openGroup)
        idx += _inPortCountArray[0] > _outPortCountArray[0] ? _inPortCountArray[0] : _outPortCountArray[0];


    QPointF pos = QPointF(Port_Start_X, Port_Start_Y+14*idx+_interval);
    ItemPort* port = new ItemPort(type, portName, portNum, true, pos, _textLength, this);
    _inPortList.append(port);
}

void DeviceItem::addOutPort(DevPortType type, QString portName, int portNum)
{
    int idx = portNum;
    if((type == PortType_AES67 || type == PortType_Dante) && _openGroup)
        idx += _inPortCountArray[0] > _outPortCountArray[0] ? _inPortCountArray[0] : _outPortCountArray[0];

    QPointF pos = QPointF(_rectWidth-Port_End_X,Port_Start_Y+14*idx+_interval);
    ItemPort* port = new ItemPort(type, portName, portNum, false, pos, _textLength, this);
    _outPortList.append(port);
}

//根据分组等调整位置
void DeviceItem::adjustPortPosition()
{
    ;
}

void DeviceItem::modifyDistanceByGroup8(int idx, int portNum, int &itvNum)
{
    int curIdx = idx +1;
    if(curIdx % 8 == 0 && curIdx != portNum){
        itvNum += Config::curMfrsCfg().DeviceGroupDis;
    }
}



void DeviceItem::replacePort(DevPortType type, QString portName, int portNum, bool bInput, int idx)
{
    ItemPort *portItem = NULL;
    QPointF pos;
    if (bInput)
    {
        portItem = _inPortList.at(idx);
        pos = portItem->pos();
        _inPortList.removeAt(idx);
        delete portItem;
    } else {
        portItem = _outPortList.at(idx);
        pos = portItem->pos();
        _outPortList.removeAt(idx);
        delete portItem;
    }
    ItemPort* port = new ItemPort(type, portName, portNum, bInput, pos, _textLength, this);
    if(bInput) _inPortList.insert(idx, port);
    else _outPortList.insert(idx, port);
}


int DeviceItem::getPortCount(bool isInput, DevPortType type)
{
    int count = 0;
    if(isInput) {
        foreach(ItemPort *item, _inPortList) {
            if(item->getPortType() == type) count++;
        }
    }else {
        foreach(ItemPort *item, _outPortList) {
            if(item->getPortType() == type) count++;
        }
    }
    return count;
}


void DeviceItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)//鼠标双击事件
{
    Q_UNUSED(event)
    QString info;
    if (!isNetworkDevice())
    {
        info = tr("This device didn't have any userInterface!");
    }
    else if (!isBind())//设备未绑定mac
    {
        info = tr("Please set device property  first!");
    }

    if(!info.isNull()){
        MainWindow::instance()->showNotice(info);
    }
    else{
        showSettingDialog();//虚函数，激活子窗口（打开设置窗口）
    }
}

void DeviceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setFont(SkinStyle::getFont(8));
    if(Config::curSkinType() == Skin_FangTu) {
        paint_FangTu(painter);
    }
    else if(Config::curSkinType() == Skin_KemuGrey ||
            Config::curSkinType() == Skin_KemuBlack ||
            Config::curSkinType() == Skin_KemuWhite) {
        paint_Kemu(painter);
    }
    else{
        paint_Default(painter);
    }
}

void DeviceItem::paint_Kemu(QPainter *painter)
{
    QRectF rcBoundOut = QRectF(0, 0, _rectWidth,  _rectHeight).adjusted(-0.5, -0.5, 0.5, 0.5);
    QRectF rcBody = QRectF(0, 18, _rectWidth,  _rectHeight-18).adjusted(0, 0, 0, 0);

    int h, s, l;
    _fillColor.getHsl(&h, &s,&l);
    QColor bordColor = QColor::fromHsl(h, s, l+ 30);
    QColor bodyColor = QColor(107, 107, 107);
    QColor portAreaColor = QColor(180, 180, 180);
    painter->setPen(QPen(_fillColor, 1));
    painter->setBrush(_fillColor);
    painter->drawRoundedRect(rcBoundOut, 5, 5);

    QPainterPath bodyPath = getBottomRoundRectPath(rcBody, 5);
    painter->fillPath(bodyPath, QBrush(bodyColor));

    painter->setBrush(portAreaColor);
    painter->setPen(QPen(bodyColor, 1));
    int maxPort = _inPortList.count() > _outPortList.count()? _inPortList.count() : _outPortList.count();
    for(int i = 0; i< maxPort; i++)
    {
        painter->drawRoundedRect(QRectF(12, Port_Start_Y+14*i, _rectWidth -24,  12), 3, 3);
    }

    QString newName = getDeviceName();     //设备名称
    QRectF rcHead = QRectF(0, 0, _rectWidth,  18).adjusted(0.5, 0.5, -0.5, -0.5);
    painter->setPen(Qt::NoPen);
    if(isOnline()){
        painter->setBrush(QColor(Config::curSkinStyle()->OnlineTextColor));
    }
    else{
        painter->setBrush(QColor(125,125,125));
    }
    painter->drawPath(_isOnlinePath);
    painter->setPen(QPen(Config::curSkinStyle()->DevItemTitilColor, 1));
    rcHead = rcHead.adjusted(10,0,0,0);
    painter->drawText(rcHead, Qt::AlignHCenter, newName);

    //显示边框
    if (isSelected()) {
        if(hasFocus())  painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));
        else  painter->setPen(QPen(Qt::red, 1, Qt::DashLine));
    }
    else {
        painter->setPen(QPen(bordColor, 1));
    }
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(rcBoundOut, 5, 5);
}

void DeviceItem::paint_Default(QPainter *painter)
{
    QRectF rcBoundIn = QRectF(0, 17, _rectWidth,  _rectHeight -17).adjusted(0.5, 0.5, -0.5, -0.5);
    QRectF rcBound = QRectF(0, 17, _rectWidth,  _rectHeight -17).adjusted(-0.5, -0.5, 0.5, 0.5);

    //设备圆角矩形框
    painter->setPen(QPen(_curFillColor, 1));
    painter->setBrush(_curFillColor);
    painter->drawRoundedRect(rcBoundIn, 5, 5);

    QString newName = getTypeName() + " <" + getDeviceName() + ">"; //设备名称
    painter->setPen(QPen(Config::curSkinStyle()->DevItemTitilColor, 1));
    painter->drawText(QRectF(5, 0, _rectWidth-5, 17), Qt::AlignLeft, newName);

    if (!_image.isNull())//外部设备会用到图片
    {
        QPixmap pixmapToShow = QPixmap::fromImage(_image.scaled(QSize(48, 48), Qt::KeepAspectRatioByExpanding) );
        painter->drawPixmap(1,18, pixmapToShow);
    }
    if (_isMonitor)
    {
        painter->drawImage(_rectWidth-45, _rectHeight-34, QImage("://Image/Monitor.png"));
    }

    //绘制Focus、选中的状态
    if (hasFocus())
    {
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rcBound);
    }
    else if (isSelected())
    {
        painter->setPen(QPen(Qt::red, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rcBound);
    }
}

void DeviceItem::paint_FangTu(QPainter *painter)
{
    QRectF rcBoundIn = QRectF(0, 17, _rectWidth,  _rectHeight -17).adjusted(0.5, 0.5, -0.5, -0.5);
    QRectF rcBound = QRectF(0, 17, _rectWidth,  _rectHeight -17).adjusted(-0.5, -0.5, 0.5, 0.5);
    //设备名称
    QString newName = getTypeName() + " <" + getDeviceName() + ">";

    painter->setPen(QPen(Config::curSkinStyle()->DevItemTitilColor, 1));
    painter->setFont(SkinStyle::getFont(8));
    painter->drawText(QRectF(5, 0, _rectWidth-5, 17), Qt::AlignLeft, newName);

    //设备圆角矩形框
    if (isSelected()) {
        painter->setPen(QPen(Qt::red, 3));
    }
    else {
        painter->setPen(QPen(QColor(34, 177, 61), 2));
    }
    painter->setBrush(_curFillColor);
    painter->drawRoundedRect(rcBoundIn, 5, 5);

    if (!_image.isNull())//外部设备会用到图片
    {
        QPixmap pixmapToShow = QPixmap::fromImage(_image.scaled(QSize(48, 48), Qt::KeepAspectRatioByExpanding) );
        painter->drawPixmap(1,18, pixmapToShow);
    }
    if (_isMonitor)
    {
        painter->drawImage(_rectWidth-45, _rectHeight-34, QImage("://Image/Monitor.png"));
    }
}


void DeviceItem::bindAES70Device(AES70Device* dev)
{
    if(dev)
    {
        _device = dev;
        connect(_device, &AES70Device::deviceStatusChanged, this, &DeviceItem::onDeviceStatusChangedSlots);
        onDeviceStatusChangedSlots(_device->getStatus());

        ProjectScene *projScene = qobject_cast<ProjectScene *>(this->scene());
        if(projScene) {
            projScene->setNeedSaveState(true);
            QString roomName = projScene->getViewTitle();
            if(roomName.endsWith('*')) roomName.chop(1);
            _device->setCurRoomName(roomName);
        }

    }


    if(_devType == Dev_Type_AES67_SPK)
    {
        setMonitor(true);//监控平台
    }
}

void DeviceItem::replacePort(PortNetworkBank bank, int newType)
{
    bool bInput = true;
    int offset = 0;
    DevPortType tmp[2] = {PortType_AES67, PortType_Dante};
    switch (bank) {
    case Port_Network_Input_Bank1:
        bInput = true;
        offset = 16;
        break;
    case Port_Network_Input_Bank2:
        bInput = true;
        offset = 24;
        break;
    case Port_Network_Output_Bank1:
        bInput = false;
        offset = 16;
        break;
    case Port_Network_Output_Bank2:
        bInput = false;
        offset = 24;
        break;
    default:
        return;
    }

    for (int i = 0; i < 8; ++i)
    {
         replacePort(tmp[newType], QString(), i+offset-16, bInput, i+offset);
    }
}

QString DeviceItem::getAES70Name() const
{
    if(_device == NULL)  return QString();
    else return _device->getDeviceName();
}

bool DeviceItem::isMonitor()
{
    return _isMonitor;
}

void DeviceItem::setMonitor(bool isMonitor)
{
    if (_devType == Dev_Type_AES67_SPK
            || _devType == Dev_Type_STM32_POE_AES67_SPK_0_1/* || _devType == DEV_TYPE_DNTE_AMP4_4 || _devType == DEV_TYPE_DNTE_AMP_ONEOUT */)
    {
        _isMonitor = isMonitor;
        update();
    }

    ProjectScene *projScene = qobject_cast<ProjectScene *>(scene());//返回项的当前场景
    Q_ASSERT(projScene != NULL);
    projScene->onMonitorChanged(this);
}

bool DeviceItem::isOnline()
{
    if(_device != NULL) return _device->isOnline();
    return false;
}

bool DeviceItem::isNetworkDevice()
{
    return true;
}

bool DeviceItem::canMonitor()
{
    return false;
}

bool DeviceItem::canCustomAlgorithm()
{
    return false;
}

#include <QGraphicsProxyWidget>
void DeviceItem::showSettingDialog()
{
    ProjectScene *pScene = qobject_cast<ProjectScene *>(scene());
    QPointF scenePos = QPointF(this->scenePos().x()+_rectWidth/2, this->scenePos().y()+_rectHeight/2);
    if(!_proxyItem) {
        QWidget *wnd = getSettingDialog();
        if(!wnd) return;
        wnd->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
        BaseWindow *settingDialog = new BaseWindow(wnd);
        QString title = getTypeName() + " <" + getDeviceName() + ">";
        settingDialog->setTitle(title);
        _proxyItem = pScene->addWidget(settingDialog);
        pScene->setFitPos(_proxyItem, scenePos);
        _proxyItem->show();
        _proxyItem->setFocus();
    }else {
        pScene->setFitPos(_proxyItem, scenePos);
        _proxyItem->show();
        _proxyItem->setFocus();
    }

    return;
}


#include <Frame/absgraphscene.h>
void DeviceItem::initAfterAdd()
{
    if (_device != NULL)
    {
        _device->loadAlarmSetting();
        AbsGraphScene *absScene = qobject_cast<AbsGraphScene *>(this->scene());
        if(absScene) {
            QString roomName = absScene->getViewTitle();
            if(roomName.endsWith('*')) roomName.chop(1);
            _device->setCurRoomName(roomName);
        }
    }
}

void DeviceItem::clearBeforeDelete()
{
    if(_device != 0) {
        _device->setCurRoomName(QString());
    }
}

QString DeviceItem::getTypeName()
{
    return Config::curMfrsCfg().getDevTypeName(_devType);
}

void DeviceItem::showIPSet()
{
    if(_device)
    {
        if(_device->isOnline()){
            DevSetting_SetIP frm(_device);
            frm.exec();
        }
        else{
            MainWindow::instance()->showNotice(tr("This Device is offline!") +
                                               "\r\n" + _device->getDeviceName());
        }
    }
}

void DeviceItem::showFirmwareUpdate()
{
    if(_device)
    {
        if(_device->isOnline()){
            DevSetting_FirmwareUpdate frm(_device);
            frm.exec();
        }
        else{
            MainWindow::instance()->showNotice(tr("This Device is offline!") +
                                               "\r\n" + _device->getDeviceName());
        }
    }
}

QString DeviceItem::getMfrsName()
{
    if (_device != NULL)
    {
#ifdef QT_NO_DEBUG
        switch(Config::curMfrsCfg().CurMfrs) {
        case Mfrs_Fitcan:
            return _device->getMfrsName();
        default:
            return _device->getMfrsName(Config::curMfrsCfg().CurMfrs);
        }
#else
        return _device->getMfrsName();
#endif
    }
    else {
        return tr("unknown");
    }
}
//======================属性======================//
QString DeviceItem::getDeviceName()
{
    if (_device != NULL)
    {
        return _device->getDeviceName();
    }
    else {
        return tr("not bind");
    }
}




void DeviceItem::setDeviceName(QString deviceName)
{
    if (_device != NULL && deviceName != tr("not bind"))
    {
        _device->setDeviceName(deviceName, true);
    }
}
