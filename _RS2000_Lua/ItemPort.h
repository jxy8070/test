#ifndef DEVICEPORTITEM_H
#define DEVICEPORTITEM_H


#include <QGraphicsPathItem>


class LineLabel;
class CLuaPinParam_PC;
class ItemPort : public QGraphicsPathItem
{
    enum{ RADIUS = 4, TEXT_DISTANCE =9};

public:
    ItemPort(DevPortType type, QString portName, int portNum,bool isInput,
                             QPointF &pos, int maxTextPixel, QGraphicsItem *parent);
    ~ItemPort();
	
    QPointF centerPos();
    enum { Type = UserType + 2 };
    int type() const override;
    ItemPort* clone();
    QRectF boundingRect() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    QPainterPath shape() const override;
    void paintText(QPainter *painter);
    void initPort(DevPortType &type, QString portName, int portNum,bool isInput);
    void initPort_FangTu(DevPortType &type, QString portName, int portNum,bool isInput);

public:
    inline QString portName() { return _portName; }
    inline void setPortName(QString newName) { _portName = newName; setToolTip(_portName); }
    inline int portNum() { return _portNum; }
    inline DevPortType getPortType() { return _portType; }
    inline bool isPhysicsPort() { return _portType ==PortType_MONAURAL; }
    inline bool enable() {return !_isInput || _enable;} //输出端口或可用输入端口
    inline bool isInput() { return _isInput; }
    inline void setEnable(bool enable) {_enable = enable;}
    inline LineLabel* getLineLabel() {return _linelabel;}
    inline void setLineLabel(LineLabel* label) {_linelabel = label;}

    inline CLuaPinParam_PC* getControlPinDef() { return _ctrlPinDef;}
    inline void setControlPinDef(CLuaPinParam_PC* pinDef){ _ctrlPinDef = pinDef;}

    inline QColor getPainterColor() { return _painterColor; }
public slots:
    inline bool isMute() { return _isMute; }
    inline void setMute(bool mute) { _isMute = mute; update();}
    inline void setEnableMute(bool en) { _EnableMute = en; }

private:
    int _painterWidth;      //画笔宽度
    QColor _painterColor;   //画笔颜色
    QPainterPath _painterPath;
    QPainterPath _sharpPath;
    DevPortType _portType;   //端口类型
    QString _portName;       //端口名
    int _portNum;            //端口编号
    bool _isInput;           //是否输入端口
    bool _enable =true;      //是否可用,只对输入端口有效
    QPointF _pos;            //端口显示位置
    qreal _maxTextPixel;     //端口名称最大宽度
    LineLabel* _linelabel;
    CLuaPinParam_PC* _ctrlPinDef;

    bool _isMute;           //Mute
    bool _EnableMute;       //启用Mute
private:
    static const qreal TextHeightPixel; //端口文本高度
    //只读默认颜色
    static const QColor ActiveColor;
    /*static const*/ QColor DefaultColor;
    static const QColor DisableColor;
    static const QColor MuteFillColor;      // 静音颜色
};


class ItemSeparator : public QGraphicsPathItem
{
public:
    ItemSeparator(bool isInput, QPointF &pos, int width, QGraphicsItem *parent)
        : QGraphicsPathItem(parent),
        _isInput(isInput), _pos(pos),_width(width)
    {
        Q_UNUSED(_isInput);
        _defaultColor.setRgb(64,124,66);
        setPos(pos);
    }
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override{
        Q_UNUSED(option);Q_UNUSED(widget);
        QPen pen(_defaultColor, 2);
        painter->setPen(pen);
        painter->setBrush(_defaultColor);
        painter->drawLine(QLine(5, 5, _width - 20, 5));
    }
private:
    bool _isInput;
    QPointF _pos;
    QColor _defaultColor;
    int _width;
};

#endif // DEVICEPORTITEM_H
