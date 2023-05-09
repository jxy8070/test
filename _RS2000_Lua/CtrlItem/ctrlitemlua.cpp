#include "ctrlitemLua.h"
#include "LuaIde/luawindow.h"
#include "Lua/Packages/luapinparam.h"

CtrlItemLua::CtrlItemLua(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    _portInCount = 4;
    _portOutCount = 4;
    _rsPortInCount = 0;
    _rsPortOutCount = 0;
    QPointF pos0;
    for(int i = 0; i< MaxPinCount + MaxRs232Count; i++)
    {
        DevPortType portType = PortType_CtrlPin;
        if(i >=MaxPinCount) portType = PortType_RS232;
        ItemPort* portIn = new ItemPort(portType, QString(), i, true, pos0, PORT_TEXT_LENGTH, this);
        _pinInList.append(portIn);
        ItemPort* portOut = new ItemPort(portType, QString(), i, false, pos0, PORT_TEXT_LENGTH, this);
        _pinOutList.append(portOut);
    }
    updateControlPins();
    memset(_pinDefIn, 0, sizeof(_pinDefIn));
    memset(_pinDefOut, 0, sizeof(_pinDefOut));

    //默认初始测试的脚本
    QFile file("E:/test.lua");
    if (file.open(QFile::ReadOnly)) {
        _luaScript = QLatin1String(file.readAll());
    }
//    _luaScript = "Controls.Outputs[1].Number = -12.12";
}


CtrlItemLua::~CtrlItemLua()
{
    for(int i = 0; i< MaxPinCount + MaxRs232Count; i++)
    {
        delete _pinInList[i];
        delete _pinOutList[i];
    }
}

QByteArray CtrlItemLua::getDetailData()
{
    char name[8];
    QString itemName = _customName.left(8);
    memcpy(name, itemName.toUtf8().data(), itemName.length());
    memset(name + itemName.length(), 0,  8- itemName.length());
    //0~7输出预留 8~15输入预留
    CLuaPinParam_PC::_LuaPinStartIndex = 20;
    QList<IG_PARACTL_LUAPIN*> allPinList;
    for(int i = 0; i < 8; i++)
    {
        if(_pinDefOut[i] == NULL) continue;
        _pinDefOut[i]->getLuaPinList(allPinList, 0xFFFFFFFF, i);
    }
    for(int i = 0; i < 8; i++)
    {
        if(_pinDefIn[i] == NULL) continue;
        _pinDefIn[i]->getLuaPinList(allPinList, 0xFFFFFFFF, i+8);
    }
    quint32 pinCnt = allPinList.length();

    QByteArray luaScriptBa = _luaScript.toUtf8();
    quint32 scriptLen = luaScriptBa.length();
    QByteArray luaGram;
    QDataStream stream(&luaGram, QIODevice::ReadWrite);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.writeRawData(name, 8);
    stream << pinCnt << scriptLen;
    for(IG_PARACTL_LUAPIN* luaPin : allPinList)
    {
        stream.writeRawData((const char *)luaPin, sizeof(IG_PARACTL_LUAPIN));
    }

    stream.writeRawData(luaScriptBa.data(), scriptLen);
    return luaGram;
}

void CtrlItemLua::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    bool ShowBorder = true;
    int BorderWidth = 1;
    int roundRadius = 4;
    qreal df = BorderWidth / 2.0;

    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    if (isSelected()) {
        if(hasFocus())  painter->setPen(QPen(Qt::red, BorderWidth, Qt::SolidLine));
        else  painter->setPen(QPen(Qt::red, BorderWidth, Qt::DashLine));
    }
    else if(ShowBorder) {
        painter->setPen(QPen(Config::curSkinStyle()->DevItemTitilColor, BorderWidth));
    }
    else {
        painter->setPen(QPen(_fillColor, BorderWidth));
    }
    painter->setBrush(_fillColor);
    painter->drawRoundedRect(rect().adjusted(df, df, -df, -df), roundRadius, roundRadius);
    QRectF rc = rect().adjusted(PortWidth, 0, -PortWidth, 0);
    QString info = _cfgTypeName + "\r\n" + _customName;
    painter->setPen(QPen(Config::curSkinStyle()->DevItemTitilColor, 1));
    painter->setFont(SkinStyle::getFont(9));
    painter->drawText(rc, Qt::AlignCenter, info);

    //绘制Focus、选中的状态
    if (isSelected())
    {
        if(hasFocus())  painter->setPen(QPen(Qt::red, BorderWidth, Qt::SolidLine));
        else  painter->setPen(QPen(Qt::red, BorderWidth, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect().adjusted(df, df, -df, -df));
    }
}

void CtrlItemLua::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)//鼠标双击事件
{
    //打开lua脚本界面
    LuaWindow* luaEditForm =  LuaWindow::instance();
    luaEditForm->show(this);

}

void CtrlItemLua::setPinInCount(int portInNum)
{
    _portInCount = portInNum;
    updateControlPins();
}

void CtrlItemLua::setPinOutCount(int portOutNum)
{
    if(portOutNum < 0 || portOutNum > 8) return;
    _portOutCount = portOutNum;
    updateControlPins();
}

void CtrlItemLua::setRsPortInCount(int rsPortInCount)
{
    _rsPortInCount= rsPortInCount;
    updateControlPins();
}

void CtrlItemLua::setRsPortOutCount(int rsPortOutCount)
{
    if(rsPortOutCount < 0 || rsPortOutCount > 2) return;
    _rsPortOutCount = rsPortOutCount;
    updateControlPins();
}

void CtrlItemLua::updateControlPins()
{
    _rectHeight = 0;
    int inCnt = _portInCount + _rsPortInCount ;
    int outCnt = _portOutCount +_rsPortOutCount;
    int maxCnt = inCnt > outCnt ? inCnt : outCnt;
    _pinsHeight = 4 + maxCnt * PortDist;
    if(_pinsHeight < 36) _pinsHeight = 36;
    setRect(0, 0, _rectWidth, _rectHeight + _pinsHeight);

    int pinYI = _rectHeight +2;
    int pinYO = _rectHeight +2;
    for(int i = 0; i< MaxPinCount + MaxRs232Count; i++)
    {
        bool isShowIn = false;
        bool isShowOut = false;
        if(i< MaxPinCount){
            if(i< _portInCount) {
                _pinInList[i]->setPos(QPointF(2, pinYI));
                isShowIn = true;
            }
            _pinInList[i]->setVisible(isShowIn);
            setPinLineVisible(_pinInList[i], isShowIn);

            if(i< _portOutCount) {
                _pinOutList[i]->setPos(QPointF(_rectWidth -PortWidth, pinYO));
                isShowOut = true;
            }
            _pinOutList[i]->setVisible(isShowOut);
            setPinLineVisible(_pinOutList[i], isShowOut);
        }
        else{
            int idx = i - MaxPinCount;
            if(idx< _rsPortInCount) {
                _pinInList[i]->setPos(QPointF(2, pinYI));
                isShowIn = true;
            }
            _pinInList[i]->setVisible(isShowIn);
            setPinLineVisible(_pinInList[i], isShowIn);

            if(idx< _rsPortOutCount) {
                _pinOutList[i]->setPos(QPointF(_rectWidth -PortWidth, pinYO));
                isShowOut = true;
            }
            _pinOutList[i]->setVisible(isShowOut);
            setPinLineVisible(_pinOutList[i], isShowOut);
        }

        if(isShowIn) pinYI += PortDist;
        if(isShowOut) pinYO += PortDist;
    }
    update();
}

