#ifndef DEVICEITEM_H
#define DEVICEITEM_H



#include <QTimer>
#include "config.h"
#include "DesignItem.h"
class ItemPort;
class AES70Device;
enum DevPortType;
enum DevStatus;

//设备基类
class DeviceItem : public DesignItem
{
    Q_OBJECT
    Q_PROPERTY(QString DeviceName READ getDeviceName WRITE setDeviceName)
    Q_PROPERTY(int InAPortCount MEMBER (_inPortCountArray[0]) NOTIFY portDisplayCountChanged);
    Q_PROPERTY(int InNPortCount MEMBER (_inPortCountArray[1]) NOTIFY portDisplayCountChanged);
    Q_PROPERTY(int InDPortCount MEMBER (_inPortCountArray[2]) NOTIFY portDisplayCountChanged);
    Q_PROPERTY(int InUPortCount MEMBER (_inPortCountArray[3]) NOTIFY portDisplayCountChanged);
    Q_PROPERTY(int OutAPortCount MEMBER (_outPortCountArray[0]) NOTIFY portDisplayCountChanged);
    Q_PROPERTY(int OutNPortCount MEMBER (_outPortCountArray[1]) NOTIFY portDisplayCountChanged);
    Q_PROPERTY(int OutDPortCount MEMBER (_outPortCountArray[2]) NOTIFY portDisplayCountChanged);
    Q_PROPERTY(int OutUPortCount MEMBER (_outPortCountArray[3]) NOTIFY portDisplayCountChanged);
    Q_PROPERTY(QColor FillColor READ getFillColor WRITE setFillColor)

public:
    enum {
#if Logo_FangTu
        Port_Start_X = 5,
        Port_End_X = 15,
#else
        Port_Start_X = 1,
        Port_End_X = 11,
#endif
        Port_Start_Y = 22
    };
    enum PortNetworkBank {
        Port_Network_Input_Bank1 = 0,
        Port_Network_Input_Bank2,
        Port_Network_Input_Bank3,
        Port_Network_Input_Bank4,
        Port_Network_Output_Bank1,
        Port_Network_Output_Bank2,
        Port_Network_Output_Bank3,
        Port_Network_Output_Bank4,
        Port_Network_Max_Bank,
    };


public:
    DesignItem* clone(DesignItem* toItem) override;
    int checkZValue(void) override;
    virtual bool isNetworkDevice(); //是否网络设备
    virtual bool canMonitor();
    virtual bool canCustomAlgorithm(); //是否能自定义DSP算法
    virtual void showSettingDialog();
    virtual void adjustPortPosition();  //根据分组等调整位置
    virtual QWidget *getSettingDialog() { return NULL; }

    void initAfterAdd() override;
    void clearBeforeDelete() override;
    bool isOnline();    
    bool isMonitor() ;
    void setMonitor(bool isMonitor);
    QString getTypeName();
    QString getAES70Name() const;
    void bindAES70Device(AES70Device* dev);
    inline void getGroup(qint16 group) {_group = group;}
    void showIPSet(void);
    void showFirmwareUpdate(void);

    void replacePort(PortNetworkBank bank, int newType);
    void addInPort(DevPortType type, QString portName,  int portNum);
    void addOutPort(DevPortType type, QString portName,  int portNum);
    void modifyDistanceByGroup8(int idx, int portNum, int &itvNum);

    //inline
    inline void setUserZValue(int Z) {_UserZValue = Z;}
    inline int getUserZValue(void) { return _UserZValue; }
    inline int getInPortCount() const {return _inPortList.count();}
    inline int getOutPortCount() const { return _outPortList.count(); }
    inline bool isBind() const { return (_device != NULL);}
    inline AES70Device* getAES70Device() { return _device; }
    inline DevType getType(){return _devType;}

    void setHeight(int height);
    QList<ItemPort *> &getPortList(bool isInput)
    {
        if(isInput) return _inPortList;
        else return _outPortList;
    }
    QList<quint32> getMulticastPortList()
    {
        return _id;
    }
    void MulticastPortAppend(quint32 id)
    {
        _id.append(id);
    }
    void MulticastPortClear(void)
    {
        _id.clear();
    }

    void clearInOutPorts();
signals:
    void portDisplayCountChanged();
public slots:
    void portDisplayCountChangedSlots();
public: //属性
    QString getMfrsName();
    QString getDeviceName();
    void setDeviceName(QString deviceName);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    DeviceItem(DevType devType, QString deviceName, QImage image,
               QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent = NULL);
    void initDisplayInOutPortCount();
    virtual void initInOutPorts();

    void paint_Default(QPainter *painter);
    void paint_Kemu(QPainter *painter);
    void paint_FangTu(QPainter *painter);
private slots:
    void onDeviceStatusChangedSlots(DevStatus devStatus);
    void onTimerChangedSlot();
private:
    void replacePort(DevPortType type, QString portName, int portNum, bool bInput, int idx);
    int getPortCount(bool isInput, DevPortType type);

protected:
    int _UserZValue;
    AES70Device* _device = NULL; //对应设备类
    QList<ItemPort *> _inPortList;
    QList<ItemPort *> _outPortList;

    QList<ItemSeparator *> _separatorList;
    qreal _rectWidth;
    qreal _rectHeight;
    qreal _textLength;    
    DevType _devType;

    uint _inPortCountArray[4];   //0: Analog, 1: AES67, 2: Dante, 3: USB
    uint _outPortCountArray[4];  //0: Analog, 1: AES67, 2: Dante, 3: USB
    int _interval;
    bool _openGroup;
    QGraphicsProxyWidget *_proxyItem;
private:
    bool _isMonitor;
    QImage _image;
    QList<quint32> _id;
    qint16 _group;

    DevStatus _devStatus;
    QPainterPath _isOnlinePath;
    QTimer *_timer;
    QColor _curFillColor; //当前填充色，离线时可能与_fillColor不同
};




#endif // DEVICEITEM_H
