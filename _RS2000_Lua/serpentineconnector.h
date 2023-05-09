#ifndef SERPENTINECONNECTOR_H
#define SERPENTINECONNECTOR_H
#include <QObject>
#include "DesignItem.h"
#include "Protocal/packet.h"

enum SerConType {
    SerConType_Invalid = 0,
    SerConType_In,
    SerConType_Out,
};
class SerpentineConnector : public DesignItem
{
    Q_OBJECT
    Q_PROPERTY(QString Name READ getModuleName)
    Q_PROPERTY(int ChCount MEMBER _channelCount NOTIFY channelCountSignal)
    Q_PROPERTY(QColor FillColor MEMBER _backgroundColor /*NOTIFY backgroundColorSignal*/)
public:
    enum {Type = UserType + 7,};
    SerpentineConnector(SerConType conType, int channelCount, DevPortType portType);

    int type() const override;
    QPainterPath shape() const override;
    DesignItem* clone(DesignItem* toItem) override;
    int checkZValue(void) override { return _userZValue; }
    void setUserZValue(int zValue) { _userZValue = zValue; }
    void initPort(int inNum, int outNum);
    void refreshBorderPath();
    void conectingLine(QList<ItemPort *> &startPortList);
    void saveDataToSettings(QSettings *ConfigIni);
    void loadDataFromSettings(QSettings *ConfigIni);

    inline SerConType serConType() { return _ConType; }
    inline QList<ItemPort *> getInPortList() { return _inPortList; }
    inline QList<ItemPort *> getOutPortList() { return _outPortList; }
    inline const QString getModuleName() { return _moduleName; }
    inline int getChannelCount() { return _channelCount; }
    inline DevPortType getChannelPortType() { return _channelPortType; }
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
signals:
    void channelCountSignal(int);
    void backgroundColorSignal(QColor);
private slots:
    void channelCountSlot(int count);
//    void backgroundColorSlot(QColor bgColor);
private:
    enum{
        maxPortNameWidth = 4,
        portDist = 12,
        portWidthAndHeight = 12,
    };
    const int _rectWidth;       //连接器宽度
    int _rectHeight;            //连接器高度
    QPainterPath _borderPath;   //边框轨迹
    qreal _borderWidth;         //边框宽度
    QColor _borderColor;        //边框颜色
    QColor _backgroundColor;    //背景色
private:
    const QString _moduleName;          //模块名
    SerConType _ConType;                //连接器类型
    int _channelCount;                  //输入or输出 通道数量
    DevPortType _channelPortType;       //通道类型
    QList<ItemPort *> _inPortList;      //输入通道链表
    QList<ItemPort *> _outPortList;     //输出通道链表

    int _userZValue;

};

#endif // SERPENTINECONNECTOR_H
