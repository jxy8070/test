#include "stable.h"
#include "configitem.h"
#include "ItemPort.h"
#include "utils.h"
#include "Protocal/packet.h"
#include "Frame/absgraphview.h"
#include "Frame/configview.h"
#include "Frame/configscene.h"
#include "device/device_trn.h"
#include "deviceItem/deviceitem_trn.h"
#include "ItemPort.h"
#include <QGraphicsProxyWidget>
#include "customWidget/basewindow.h"
#include <QTextBrowser>
#include "configItem/configiteminput.h"
#include "configItem/configitemoutput.h"

bool ConfigItem::ShowSpaceTimePercent = false;

ConfigItem::ConfigItem(Device_Trn* devTrn, CfgType cfgType, QString name, QString typeName, QPointF scenePos,
                        int width) :DesignItem()
{
//    qDebug() << __FUNCTION__ << __LINE__
//             << name << typeName << QString::number(cfgType, 16);
    _trnDev = devTrn;
    _cfgType = cfgType;
    _fillColor = Config::curSkinStyle()->getConfigItemColor(cfgType);
    _layer = LAYER_IVALID;    
    _customName = name;
    _cfgTypeName = typeName;
    _rectWidth = width;
    _rectHeight = -1;
    _pinsHeight = 0;

    _proxyItem = NULL;
    _point = scenePos;
    _ptrOffset = 0;
    setFlags(QGraphicsRectItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable |
             QGraphicsRectItem::ItemIsMovable    | QGraphicsItem::ItemSendsGeometryChanges);//通过单机项目或者使用框选来选择项目
    setAcceptHoverEvents(true);//接受悬停事件

    if(_fillColor.valueF()<0.5) _textColor = Qt::white;
    else _textColor = Qt::black;
    setPos(scenePos);//将项的位置设置为scenePos
}


CfgType ConfigItem::getCfgType()
{
    return _cfgType;
}

ConfigItem::~ConfigItem()
{
//    foreach (QList<OutputDef*> OutputDefList, _multipleOutputDefList) {
//        qDeleteAll(OutputDefList);
//    }
//    _multipleOutputDefList.clear();
//    qDebug()<<__FUNCTION__<<__LINE__;
    //使用c++deletes操作符删除范围是[开始，结束]中的所有项，项目的类型必须是指针类型
    qDeleteAll(_inPortList);
    qDeleteAll(_inputDefList);
    qDeleteAll(_pinInList);
    qDeleteAll(_outPortList);
    qDeleteAll(_outputDefList);
    qDeleteAll(_pinOutList);
}

DesignItem *ConfigItem::clone(DesignItem *from)
{
    ConfigItem *item = NULL;

    if (from == NULL) {//copy object
        CfgType groupFirst =(CfgType)(_cfgType& 0xFF00);  //模块存在混合的情况:2通道和4通道都存在
        if (groupFirst == CFG_TYPE_INPUT_1 ||
                groupFirst == CFG_TYPE_OUTPUT_1 ||
                _cfgType == CFG_TYPE_SNAPSHOT ||
                _cfgType == CFG_TYPE_SNAPSHOT_BANK) {
            return NULL;
        }

        int cfgItemIndex = 9999;
        ConfigScene* cfgScene = qobject_cast<ConfigScene*>(scene());
        if(cfgScene != NULL) {
            cfgItemIndex = cfgScene->getTrnInterface()->getCfgItemIndex();
        }
        QString customName =  QString("%1_%2").arg(Utils::getCfgShortName(CfgType(_cfgType))).arg(cfgItemIndex);
        item = ConfigItemFactory::createConfigItem(_trnDev, _cfgType, customName, scenePos());
        item->setDetailData((unsigned int *)(getDetailData().data()));
    }
    else {//copy parameter
        ConfigItem *fromItem = qobject_cast<ConfigItem*>(from);
        this->setDetailData((unsigned int *)fromItem->getDetailData().data());
    }

    return item;
}

int ConfigItem::checkZValue(void)
{
//    qDebug() << __FUNCTION__ << __LINE__ << customName() << _UserZValue;
    return _UserZValue;
}


void ConfigItem::saveParamToDevice()
{
    ConfigScene * pCfgScene = qobject_cast<ConfigScene *>(scene());
    if (pCfgScene != NULL)
    {
        pCfgScene->saveParamToDevice(this);
    }
}

int ConfigItem::byte2int(QByteArray byte, int offset)
{
    int addr = byte[offset] & 0x000000FF;
    addr |= ((byte[offset+1] << 8) & 0x0000FF00);
    addr |= ((byte[offset+2] << 16) & 0x00FF0000);
    addr |= ((byte[offset+3] << 24) & 0xFF000000);
    return addr;
}

quint16 ConfigItem::byte2Uint16(QByteArray byte, int offset)
{
    quint16 addr = byte[offset] & 0x00FF;
    addr |= ((byte[offset+1] << 8) & 0xFF00);
    return addr;
}

quint32 ConfigItem::byte2Uint32(QByteArray byte, int offset)
{
    quint32 addr = byte[offset] & 0x000000FF;
    addr |= ((byte[offset+1] << 8) & 0x0000FF00);
    addr |= ((byte[offset+2] << 16) & 0x00FF0000);
    addr |= ((byte[offset+3] << 24) & 0xFF000000);
    return addr;
}

bool ConfigItem::byte2Bool(QByteArray byte, int offset)
{
    bool flag = byte[offset];
    return flag;
}

void ConfigItem::initPort(int inNum, int outNum, QStringList iPortNames, QStringList oPortNames)
{
    Q_ASSERT(_rectHeight == -1); //重复初始化
    int maxPortNum = inNum > outNum ? inNum : outNum;
    _rectHeight = 2 + maxPortNum*PortDist + 1;
    if(_rectHeight < 42) _rectHeight  = 42;
#if Logo_FangTu
    if(((getCfgType() & FCT_FAMILY_MASK) == 0x1000 && getCfgType() < CFG_TYPE_SOFTFIR_4R256)
           || (getCfgType() & FCT_FAMILY_MASK) == 0x1100 || getCfgType() == CFG_TYPE_FREQSHIFT_PLUS
           || getCfgType() == CFG_TYPE_DELAY_100) {
        _rectHeight += PortDist + 3;
    }
#endif

    setRect(0, 0, _rectWidth, _rectHeight + _pinsHeight);
    for(int i =0; i<inNum; i++)//输入端口
    {
        QPointF pos(2, 2 + PortDist*i);
        QString portName;
        if(iPortNames.count() > i) portName = iPortNames[i];
        else portName = QString("Input %1").arg(i+1);

        ItemPort* port = new ItemPort(PortType_Config, portName, i, true, pos, PORT_TEXT_LENGTH, this);
        _inPortList.append(port);

        SigFlowDef16 *inDef = new SigFlowDef16();
        memset(inDef, 0xFFFFFFFF, sizeof(SigFlowDef16));
        _inputDefList.append(inDef);
    }

    for(int i =0; i<outNum; i++)
    {
        QPointF pos(_rectWidth -PortWidth, 2 + PortDist*i);
        QString portName;
        if(oPortNames.count() > i) portName = oPortNames[i];
        else portName = QString("Output %1").arg(i+1);

        ItemPort* port = new ItemPort(PortType_Config, portName, i, false, pos, PORT_TEXT_LENGTH, this);
        _outPortList.append(port);
        SigFlowDef16 *outDef = new SigFlowDef16();
        memset(outDef, 0xFFFFFFFF, sizeof(SigFlowDef16));
        _outputDefList.append(outDef);
    }
}

void ConfigItem::initControlPins(QList<CLuaPinParam_PC*> pinList, bool visible)
{
    bool isLuaDev =  Config::curMfrsCfg().isLuaDevice(getDevType());
    if(!isLuaDev){
        setRect(0, 0, _rectWidth, _rectHeight + _pinsHeight);
        return;
    }

    //先清除
    if(_pinDefList.count() > 0)
    {
        _pinDefList.clear();
        for(ItemPort* pinPort : _pinInList){
            this->childItems().removeOne(pinPort);
        }
        _pinInList.clear();
        for(ItemPort* pinPort : _pinOutList){
            this->childItems().removeOne(pinPort);
        }
        _pinOutList.clear();
    }
    _pinDefList = pinList;

    QPointF pos1(2, 2);
    for(int i = 0; i< _pinDefList.count(); i++ )
    {
        CLuaPinParam_PC* pinDef = _pinDefList[i];
        pinDef->_bVisible = visible;
        if(pinDef->_setCmdKey != 0xFFFFFFFF) {
            pinDef->pinPortIn = new ItemPort(PortType_CtrlPin, pinDef->_pinName, i, true, pos1, PORT_TEXT_LENGTH, this);
            pinDef->pinPortIn->setControlPinDef(pinDef);
        }

        if(pinDef-> _getCmdKey!= 0xFFFFFFFF) {
            pinDef->pinPortOut = new ItemPort(PortType_CtrlPin, pinDef->_pinName, i, false, pos1, PORT_TEXT_LENGTH, this);
            pinDef->pinPortOut->setControlPinDef(pinDef);
        }
        _pinInList.append(pinDef->pinPortIn);
        _pinOutList.append(pinDef->pinPortOut);
    }
    updateControlPins();
}

void ConfigItem::updateControlPins()
{
    int pinY = _rectHeight + 2.5;
    int pinCnt = 0;
    for(int i = 0; i< _pinDefList.count(); i++)
    {
        CLuaPinParam_PC* pin = _pinDefList[i];
        bool isInShow = false;
        bool isOutShow = false;
        if(pin->_bVisible == true)
        {
            if(pin->_setCmdKey != 0xFFFFFFFF)
            {
                _pinInList[i]->setPos(roundRadius +2.5, pinY);
                isInShow = true;
            }
            if(pin->_getCmdKey!= 0xFFFFFFFF)
            {
                _pinOutList[i]->setPos(_rectWidth -roundRadius-PortWidth-0.5, pinY);
                isOutShow = true;
            }
            pinY += PortDist;
            pinCnt++;
        }
        if(_pinInList[i] != NULL) {
            _pinInList[i]->setVisible(isInShow);
            setPinLineVisible(_pinInList[i], isInShow);
        }
        if(_pinOutList[i] != NULL) {
            _pinOutList[i]->setVisible(isOutShow);
            setPinLineVisible(_pinOutList[i], isOutShow);
        }
    }
    if(pinCnt> 0) _pinsHeight = 4+ pinCnt*PortDist;
    else _pinsHeight = 0;
    setRect(0, 0, _rectWidth, _rectHeight + _pinsHeight);
    update();
}


void ConfigItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(Config::curSkinType() == Skin_FangTu) {
        paint_FangTu(painter);
    }
    else if(Config::curSkinType() == Skin_KemuGrey ||
            Config::curSkinType() == Skin_KemuBlack  ||
            Config::curSkinType() == Skin_KemuWhite) {
        paint_Kemu(painter);
    }
    else{
        paint_Default(painter);
    }

    if(ShowSpaceTimePercent)
    {
        QRectF bordRc(rect().left()+12, rect().top()-2, rect().width() -24, 16);
        painter->setPen(QPen(QColor(0, 0, 0), 1));
        painter->setFont(SkinStyle::getFont(8));

        DevType devType = _trnDev->getDeviceType();
        QString spacePercent = QString::number(Utils::getSpaceUsage(devType, _cfgType), 'f', 2);
        QString timePercent = QString::number(Utils::getTimeUsage(devType, _cfgType), 'f', 2);
#ifdef QT_DEBUG
        QString percentStr = QString("L:%1, G:%2").arg(_layer) .arg(_cpuGroupIdx);
//        QString percentStr = QString("C:%1, M:%2,\nZValue: %3").arg(timePercent)
//                .arg(spacePercent).arg(_UserZValue);
#else
        QString percentStr = QString("C:%1, M:%2").arg(timePercent).arg(spacePercent);
#endif
        painter->fillRect(bordRc, QBrush(QColor(255,255,255,180)));
        painter->drawText(bordRc, Qt::AlignTop|Qt::AlignHCenter, percentStr);
    }
}

QPainterPath ConfigItem::getPaintPath(int borderWidth, int dx, int dy)
{
    QPainterPath paintPath;
    qreal df = borderWidth / 2.0;
    int diameter = roundRadius * 2;
    QRectF cfgRc = QRectF(dx, dy, _rectWidth - 2*dx, _rectHeight- 2*dy).adjusted(df, df, -df, -df);
    QRectF tlRc(cfgRc.left(), cfgRc.top(), diameter, diameter);
    QRectF trRC(cfgRc.right()-diameter, cfgRc.top(), diameter, diameter);
    QRectF mlRC(cfgRc.left(), cfgRc.bottom()-diameter, diameter, diameter);
    QRectF mrRC(cfgRc.right()-diameter, cfgRc.bottom()-diameter, diameter, diameter);
    paintPath.moveTo(mrRC.left() + roundRadius, mrRC.bottom());
    paintPath.arcTo(mrRC, 270, 90);
    paintPath.lineTo(trRC.right(), trRC.bottom()-roundRadius);
    paintPath.arcTo(trRC, 0, 90);
    paintPath.lineTo(tlRc.right()-roundRadius, tlRc.top());
    paintPath.arcTo(tlRc, 90, 90);
    paintPath.lineTo(mlRC.left(), mlRC.top() + roundRadius);
    paintPath.arcTo(mlRC, 180, 90);
    if(_pinsHeight > 0 &&  dx == 0)
    {
        QRectF pinsRc = QRectF(roundRadius +dx, _rectHeight-dy, _rectWidth-2*roundRadius -2*dx, _pinsHeight).adjusted(df, df, -df, -df);
        QRectF blRC(pinsRc.left(), pinsRc.bottom() - diameter, diameter, diameter);
        QRectF brRC(pinsRc.right() - diameter, pinsRc.bottom() - diameter, diameter, diameter);
        paintPath.lineTo(blRC.left(), blRC.top() + roundRadius);
        paintPath.arcTo(blRC, 180, 90);
        paintPath.lineTo(brRC.left() + roundRadius, brRC.bottom());
        paintPath.arcTo(brRC, 270, 90);
    }
    paintPath.lineTo(mrRC.left() + roundRadius, mrRC.bottom());
    return paintPath;
}

QLinearGradient ConfigItem::getLinearGradient(QRectF rect, QColor fillColor)
{
    qreal h, s, v;
    float radio = 1;

    fillColor.getHsvF(&h, &s, &v);
    QColor color1, color2, color3;
    qreal s2 = (s-(0.2*radio) < 0.0) ? 0.0 : s-(0.2*radio);
    qreal v1 = (v-(0.1*radio) < 0.0) ? 0.0 : v-(0.1*radio);
    qreal v2 = (v+(0.2*radio) > 1.0) ? 1.0 : v+(0.2*radio);
    qreal v3 = (v-(0.2*radio) < 0.0) ? 0.0 : v-(0.2*radio);
    color1.setHsvF(h, s, v1);
    color2.setHsvF(h, s2, v2);
    color3.setHsvF(h, s, v3);
    QLinearGradient linearGradient(rect.left(), rect.top(), rect.left(), rect.bottom());//初始化，设置开始和结束点
    linearGradient.setColorAt(0.0, color1);
    linearGradient.setColorAt(0.2, color2);
    linearGradient.setColorAt(1.0, color3);
    return linearGradient;
}

void ConfigItem::paint_Default(QPainter *painter)
{
    bool showBorder = true;
    int borderWidth = 1;
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QPainterPath paintPath = getPaintPath(borderWidth);
    QLinearGradient gradient = getLinearGradient(paintPath.boundingRect(), _fillColor);
    painter->setBrush(QBrush(gradient));
    painter->setPen(QPen(Qt::black, 1));
    painter->drawPath(paintPath);

    QRectF rc = QRectF(PortWidth, 0, _rectWidth - 2*PortWidth, _rectHeight);
    QString info = _cfgTypeName + "\r\n" + _customName;
    painter->setPen(QPen(_textColor, 1));

    painter->setFont(SkinStyle::getFont(9));
    painter->drawText(rc, Qt::AlignCenter, info);

    if(_pinsHeight > 0) //画ControlPins
    {
        painter->setPen(Qt::DotLine);
        painter->drawLine(QLineF(roundRadius, _rectHeight-0.5, _rectWidth - roundRadius, _rectHeight-0.5));
        painter->setPen(QPen(_textColor));
        painter->setFont(SkinStyle::getFont(7));
        int pinY0 = _rectHeight +2;
        for(CLuaPinParam_PC* pin : _pinDefList)
        {
           if(pin->_bVisible == true)
           {
               QRectF rc(PortWidth, pinY0, _rectWidth -24, PortDist);
               painter->drawText(rc, Qt::AlignCenter, pin->_pinNameWithGroup);
               pinY0 += PortDist;
           }
        }
    }

    //边框
    if (isSelected()) {
        if(hasFocus())
            painter->setPen(QPen(Qt::red, borderWidth, Qt::SolidLine));
        else
            painter->setPen(QPen(Qt::red, borderWidth, Qt::DashLine));
    }
    else {
        if(showBorder)
            painter->setPen(QPen(_textColor, borderWidth));
        else
            painter->setPen(QPen(_fillColor, borderWidth));
    }
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(paintPath);
}

void ConfigItem::paint_Kemu(QPainter *painter)
{
//    bool showBorder = true;
    int borderWidth = 1;
    QColor bordColor(150, 140, 200);
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    qreal df = borderWidth / 2.0;
    QPainterPath paintOut = getPaintPath(borderWidth);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(QColor(120,100,160,200)));
    painter->drawPath(paintOut);
    QPainterPath pathIn = getPaintPath(borderWidth, 12, 3);
    QLinearGradient gradient = getLinearGradient(paintOut.boundingRect(), _fillColor);
    painter->fillPath(pathIn, QBrush(gradient));
    QRectF rc = QRectF(PortWidth, 0, _rectWidth - 2*PortWidth, _rectHeight);
    QString info = _cfgTypeName + "\r\n" + _customName;
    painter->setPen(QPen(_textColor, 1));
    painter->setFont(SkinStyle::getFont(8));
    painter->drawText(rc, Qt::AlignCenter, info);

    if(_pinsHeight > 0) //画ControlPins
    {
//        painter->drawLine(QLineF(roundRadius + 12, _rectHeight-df -2, _rectWidth - roundRadius - 12, _rectHeight-df));
        painter->setPen(QPen(_textColor, 1));
        painter->setFont(SkinStyle::getFont(7));
        int pinY0 = _rectHeight +1;

        QColor portNameColor = QColor(180, 180, 180);
        painter->setBrush(portNameColor);
        for(CLuaPinParam_PC* pin : _pinDefList)
        {
           if(pin->_bVisible == true)
           {
               QRectF rc(18, pinY0, _rectWidth -36, PortDist);
               painter->drawRoundedRect(rc, 3, 3);
               painter->drawText(rc, Qt::AlignCenter, pin->_pinNameWithGroup);
               pinY0 += PortDist;
           }
        }
    }
    //边框
    if (isSelected()) {
        if(hasFocus())
            painter->setPen(QPen(Qt::red, borderWidth, Qt::SolidLine));
        else
            painter->setPen(QPen(Qt::red, borderWidth, Qt::DashLine));
    }
    else {
            painter->setPen(QPen(Qt::transparent, borderWidth));
    }
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(paintOut);
}


void ConfigItem::paint_FangTu(QPainter *painter)
{
    bool showBorder = true;
    int borderWidth = 2;
    QColor BorderColor = QColor(34, 177, 61);
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    qreal df = borderWidth / 2.0;
    if (isSelected())
    {
        if(hasFocus())  painter->setPen(QPen(Qt::red, borderWidth, Qt::SolidLine));
        else  painter->setPen(QPen(Qt::red, borderWidth, Qt::DashLine));
    }
    else if(showBorder)
        painter->setPen(QPen(BorderColor, borderWidth));
    else
        painter->setPen(QPen(_fillColor, borderWidth));

    painter->setBrush(_fillColor);
    painter->drawRoundedRect(rect().adjusted(df, df, -df, -df), roundRadius, roundRadius);
    QRectF rc = QRectF(PortWidth, 0, _rectWidth - 2*PortWidth, _rectHeight);
    QString info = _cfgTypeName + "\r\n" + _customName;
    painter->setPen(QPen(_textColor, 1));
    painter->setFont(SkinStyle::getFont(9));
    painter->drawText(rc, Qt::AlignCenter, info);

    if(_pinsHeight > 0) //画ControlPins
    {
        painter->setPen(QPen(BorderColor, borderWidth));
        painter->drawLine(QLineF(0, _rectHeight-df, _rectWidth, _rectHeight-df));
        painter->setPen(QPen(_textColor, 1));
        painter->setFont(SkinStyle::getFont(8));
        int pinY0 = _rectHeight +2;
        for(CLuaPinParam_PC* pin : _pinDefList)
        {
           if(pin->_bVisible == true)
           {
               QRectF rc(PortWidth, pinY0, _rectWidth -24, PortDist);
               painter->drawText(rc, Qt::AlignCenter, pin->_pinNameWithGroup);
               pinY0 += PortDist;
           }
        }
    }
}

void ConfigItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)//鼠标双击事件
{
    showSettingForm(event->pos());//打开设置界面
}

bool ConfigItem::isValid()
{
    return (_layer != LAYER_IVALID);
}

void ConfigItem::setCustomName(QString name)
{
    _customName = name;

    for(CLuaPinParam_PC* pinDef : _pinDefList)
    {
        pinDef->setName(name);
    }
}

int ConfigItem::subTypeIndex()
{
    int idx = 0;
    QString subTypeName = Utils::getCfgTypeName(_cfgType);
    subTypeName.remove("\n");
    for(; idx< _subTypeNameList.count(); idx ++)
    {
        if(_subTypeNameList[idx] == subTypeName) break;
    }
    return idx;
}

void ConfigItem::addSubType(CfgType cfgType)
{
    QString subTypeName = Utils::getCfgTypeName(cfgType);
    subTypeName.remove("\n");
    _subTypeNameList.append(subTypeName);
    _subTypeList.append(cfgType);
}

CfgType ConfigItem::subType(int subTypeIdx)
{
    return _subTypeList[subTypeIdx];
}

QString ConfigItem::getCtrlPins()
{
    if(_pinDefList.count() == 0){
        return QString();
    }
    QStringList pinDefs;
    for(CLuaPinParam_PC* pin : _pinDefList)
    {
        QString visible = pin->_bVisible ? "1" : "0";
        pinDefs << QString("%0:%1").arg(pin->_pinNameWithGroup).arg(visible);
    }
    return ";" + pinDefs.join(";");
}

void ConfigItem::setCtrlPins(QString pinSetting)
{
    QStringList pinDefs = pinSetting.split(";", QString::SkipEmptyParts);
    for(int i = 0; i< _pinDefList.length(); i++ )
    {
        bool visible = true;
        if(pinDefs[i].endsWith(":0")) {
            visible = false;
        }
        _pinDefList[i]->_bVisible = visible;
    }
    updateControlPins();
}

void ConfigItem::setPinLineVisible(ItemPort* pin, bool isVisible)
{
    ConfigScene* pScene = qobject_cast<ConfigScene*>(scene());

    if(pScene != NULL) {
        pScene->setPinLineVisible(pin, isVisible);
    }
}

QString ConfigItem::getControlsInfo()
{
    QStringList pinDefs;
    for(CLuaPinParam_PC* pin : _pinDefList)
    {
        pinDefs << pin->_pinNameWithGroup;
    }
    return pinDefs.join("\n");
}

QWidget* ConfigItem::getControlsInfoWidget(ConfigItem* cfgItem)
{
    QWidget* ctrlInfoWidget = new QWidget();
    QVBoxLayout* vLayout = new QVBoxLayout(ctrlInfoWidget);
    vLayout->setObjectName("vLayout");
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* lblName = new QLabel();
    lblName->setObjectName("lblName");
    vLayout->addWidget(lblName);

    QTextBrowser* ctrlInfo = new QTextBrowser();
    ctrlInfo->setObjectName("ctrlInfo");
    vLayout->addWidget(ctrlInfo);
    if(cfgItem != NULL)
    {
        QString strName = QString("(%1) Controls").arg(cfgItem->customName());
        lblName->setText(strName);
        ctrlInfo->setText(cfgItem->getControlsInfo());
    }
    ctrlInfoWidget->setMinimumSize(300, 500);
    return ctrlInfoWidget;
}

void ConfigItem::setLayer(int layer)
{
    if(layer > _layer)
    {
        _layer = layer;
        update();
    }
}

void ConfigItem::clearLayer()
{
    _layer = LAYER_IVALID;
    _cpuIndex = -1;
    _cpuGroupIdx = -1;
    update();
}

void ConfigItem::setAllPortEnable()
{
    for(ItemPort* port : _inPortList){
        port->setEnable(true);
    }
    for(ItemPort* port : _outPortList){
        port->setEnable(true);
    }
}


void ConfigItem::showSettingForm(QPointF mousePos)
{
    QPointF scenePos = this->mapToScene(mousePos);
    ConfigScene *pScene = qobject_cast<ConfigScene *>(scene());
    if (_proxyItem == NULL) {
        //gjj 加载提醒
        QWidget *cfgForm = getSettingForm();
        if(cfgForm != NULL){
            BaseWindow *baseWidget = new BaseWindow(cfgForm);
            QString titleName = QString("%1(%2)").arg(
                        Config::instance()->curMfrsCfg().CurMfrs == Mfrs_FangTu ? configTypeName() : tr("Setting")).arg(customName());
            titleName.remove("\n");
            baseWidget->setTitle(titleName);
            //scene中QGraphicsProxyWidget显示在上层
            _proxyItem = pScene->addWidget(baseWidget);
            pScene->setFitPos(_proxyItem, scenePos);
            _proxyItem->setFocus();
            _proxyItem->show();
        }
    }
    else {
        pScene->setFitPos(_proxyItem, scenePos);
        _proxyItem->show();
        _proxyItem->setFocus();
    }
}

void ConfigItem::clearPortPara()
{
    for(int i = 0; i < _outputDefList.count(); ++i)
    {
        memset(_outputDefList.at(i), 0xFFFFFFFF, sizeof(SigFlowDef16));
    }

    for (int i = 0; i < _inputDefList.count(); ++i)
    {
        memset(_inputDefList.at(i), 0xFFFFFFFF, sizeof(SigFlowDef16));
    }
}

void ConfigItem::setInputDef(int portNum, int col, int row)
{
    _inputDefList.at(portNum)->layer = (quint16)col;
    _inputDefList.at(portNum)->row   = (quint16)row;
}

void ConfigItem::getInputDef(int portNum, int *col, int *row)
{
    *col = _inputDefList.at(portNum)->layer;
    *row = _inputDefList.at(portNum)->row;
}

void ConfigItem::setOutputDef(int portNum, int col, int row)
{
    SigFlowDef16 *outputDef = _outputDefList[portNum];
    outputDef->layer = (quint16)col;
    outputDef->row = (quint16)row;
}

void ConfigItem::getOutputDef(int portNum, int *col, int *row)
{
    SigFlowDef16 *outputDef = _outputDefList[portNum];
    *col = outputDef->layer;
    *row = outputDef->row;
}

QByteArray ConfigItem::getSigFlowHeadBuff(int bufSize, bool is16Bit)
{
    QByteArray headBuff;
    if(is16Bit) {
        IG_SIGFLOW_MODULEHEAD16B preHead16;
        memset(&preHead16, 0, sizeof(IG_SIGFLOW_MODULEHEAD16B));
        preHead16.family = _cfgType >> CFG_TYPE_2_GROUP_TYPE;
        preHead16.genus = _cfgType & 0xFF;
        preHead16.flag = IG_SIGFLOW_MODULEHEAD_FLAG;
        preHead16.blockLen32b = bufSize>>2;
        preHead16.protocal = 0;
        //根据下载算法设置的CpuIndex改变主核独立核算法
        if(preHead16.family == 0X16 || preHead16.family == 0X17){
            preHead16.auxiliaryInfoA = _cpuIndex;
            if(_cpuIndex == 0) { preHead16.family = 0X16;}
            else {preHead16.family = 0X17;}
        }
        headBuff.append((const char*)&preHead16, sizeof(IG_SIGFLOW_MODULEHEAD16B));
    }
    else{
        IG_SIGFLOW_MODULEHEAD8B preHead8;
        memset(&preHead8, 0, sizeof(IG_SIGFLOW_MODULEHEAD8B));
        preHead8.family = _cfgType >> CFG_TYPE_2_GROUP_TYPE;
        preHead8.genus = _cfgType & 0xFF;
        headBuff.append((const char*)&preHead8, sizeof(IG_SIGFLOW_MODULEHEAD8B));
    }
    return headBuff;
}

QByteArray ConfigItem::getParaSetHeadBuff(int bufSize, bool is16Bit)
{
    QByteArray headBuff;
    if(is16Bit) {
        IG_PARASET_MODULEHEAD16B detailHeader;
        memset(&detailHeader, 0, sizeof(IG_PARASET_MODULEHEAD16B));
        detailHeader.moduleNo = _moduleNum;
        detailHeader.flag = IG_PARASET_MODULEHEAD_FLAG;
        detailHeader.blockLen32b = bufSize>>2;
        headBuff.append((const char *)&detailHeader, sizeof(IG_PARASET_MODULEHEAD16B));
    }
    else{
        IG_PARASET_MODULEHEAD8B detailHeader;
        memset(&detailHeader, 0, sizeof(IG_PARASET_MODULEHEAD8B));
        detailHeader.moduleNo = _moduleNum%256;
        detailHeader.moduleNoU = _moduleNum/256;
        headBuff.append((const char *)&detailHeader, sizeof(IG_PARASET_MODULEHEAD8B));
    }
    return headBuff;
}

QByteArray ConfigItem::getOverviewPackBuff(bool is16Bit)
{
    QByteArray paraBuff;
    QByteArray overviewBuff = getOverviewData(is16Bit);
    paraBuff.append(getSigFlowHeadBuff(overviewBuff.size(), is16Bit));
    paraBuff.append(overviewBuff);

    return paraBuff;
}

QByteArray ConfigItem::getModuleNamePackBuff(bool is16Bit)
{
    QByteArray paraBuff;    
    paraBuff.append(getSigFlowHeadBuff(8,is16Bit));

    QByteArray nameBuff = _customName.toLocal8Bit();
    if (nameBuff.size() < 8) { //用0补齐8字节
        nameBuff.append(8-nameBuff.size(), 0);
    }

    paraBuff.append(nameBuff.left(8));
    return paraBuff;
}

QByteArray ConfigItem::getDetailPackBuff(bool is16Bit)
{
    QByteArray paraBuff;
    QByteArray detailBuff = getDetailData();
    paraBuff.append(getParaSetHeadBuff(detailBuff.size(), is16Bit));
    paraBuff.append(detailBuff);

    return paraBuff;
}

QByteArray ConfigItem::getOverviewData(bool is16Bit)
{
    QByteArray sigFlowBuff;

    if(is16Bit){
        //格式化16bit信号流layer，row
        IG_SIGFLOW_SINGLE16 flowDef;
        int iDefCnt = _inputDefList.count();
        for(int i = 0; i<iDefCnt; i++) {
            memset(&flowDef, 0, sizeof(IG_SIGFLOW_SINGLE16));
            flowDef.column  = _inputDefList[i]->layer;
            flowDef.line    = _inputDefList[i]->row;
            sigFlowBuff.append((const char*)&flowDef, sizeof(IG_SIGFLOW_SINGLE16));
        }

        int oDefCnt = _outputDefList.count();
        for(int i = 0; i<oDefCnt; i++)
        {
            memset(&flowDef, 0, sizeof(IG_SIGFLOW_SINGLE16));
            flowDef.column  = _outputDefList[i]->layer;
            flowDef.line    = _outputDefList[i]->row;
            sigFlowBuff.append((const char*)&flowDef, sizeof(IG_SIGFLOW_SINGLE16));
        }
    }
    else {
        //格式化8bit信号流layer，row
        IG_SIGFLOW8_DOUBLE flowDef;
        int iDefCnt = _inputDefList.count();
        for(int i = 0; i<iDefCnt; i++)
        {
            if(i%2 == 0){
                memset(&flowDef, 0, sizeof(IG_SIGFLOW8_DOUBLE));
                flowDef.columnA  = _inputDefList[i]->layer;
                flowDef.lineA    = _inputDefList[i]->row;
                if(i == iDefCnt -1){
                    sigFlowBuff.append((const char*)&flowDef, sizeof(IG_SIGFLOW8_DOUBLE)); //奇数端口数最后一个端口
                }
            }
            else{
                flowDef.columnB  = _inputDefList[i]->layer;
                flowDef.lineB    = _inputDefList[i]->row;
                sigFlowBuff.append((const char*)&flowDef, sizeof(IG_SIGFLOW8_DOUBLE));
            }
        }

        int oDefCnt = _outputDefList.count();
        for(int i = 0; i<oDefCnt; i++)
        {
            if(i%2 == 0){
                memset(&flowDef, 0, sizeof(IG_SIGFLOW8_DOUBLE));
                flowDef.columnA  = _outputDefList[i]->layer;
                flowDef.lineA    = _outputDefList[i]->row;
                if(i == oDefCnt -1){
                    sigFlowBuff.append((const char*)&flowDef, sizeof(IG_SIGFLOW8_DOUBLE)); //奇数端口数最后一个端口
                }
            }
            else{
                flowDef.columnB  = _outputDefList[i]->layer;
                flowDef.lineB    = _outputDefList[i]->row;
                sigFlowBuff.append((const char*)&flowDef, sizeof(IG_SIGFLOW8_DOUBLE));
            }
        }
        //为了兼容协议定义的问题，添加额外的数据(8bit信号流)
        if(iDefCnt == 16 && oDefCnt == 16)
        {
            sigFlowBuff.append(16*4/2, 0x00);
        }
    }

    return sigFlowBuff;
}

QByteArray ConfigItem::getOnlineDetailPara(bool is16Bit)
{
    QByteArray paraBuff;
    QByteArray detailBuff = getOnlineData();
    paraBuff.append(getParaSetHeadBuff(detailBuff.size(), is16Bit));
    paraBuff.append(detailBuff);//追加保存在字节数组的数据

    return paraBuff;
}

quint32 ConfigItem::getPacketModuleNum(quint32* buff, bool is16Bit, int& cnt32B)
{
    if(is16Bit){
        IG_PARASET_MODULEHEAD16B *head = (IG_PARASET_MODULEHEAD16B *)buff;
        cnt32B = sizeof(IG_PARASET_MODULEHEAD16B) / sizeof(quint32);
        return head->moduleNo;
    }
    else {
        IG_PARAGET_MODULEHEAD *head = (IG_PARAGET_MODULEHEAD *)buff;        
        cnt32B = sizeof(IG_PARAGET_MODULEHEAD) / sizeof(quint32);
        return head->moduleNo;
    }
}

DevType ConfigItem::getDevType()
{
    if(_trnDev != NULL)
        return _trnDev->getDeviceType();
    else
        return Dev_Type_Unknow;
}

void ConfigItem::clearBeforeDelete()
{
    if (_proxyItem != NULL)
    {
        _proxyItem->hide();
        delete _proxyItem;
        _proxyItem = 0;
    }
}


