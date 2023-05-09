#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QObject>
#include <QPainter>
#include <QGridLayout>
#include <QLabel>
#include "common.h"
#include <QGraphicsRectItem>
#include "device/device_trn.h"
#include "configItem/configitem.h"
#include "Frame/configview.h"
#include "Frame/configscene.h"

//enum CfgGroupType
//{
//    FCT_DEVFAMILY_TYPE_DELAY  = 0x01,     //延时器
//    FCT_DEVFAMILY_TYPE_ROUTER = 0x02,    //路由器
//    FCT_DEVFAMILY_TYPE_MIXER = 0x03,     //混音器
//    FCT_DEVFAMILY_TYPE_DYN = 0x04,       //动态处理
//    FCT_DEVFAMILY_TYPE_EQ = 0x05,        //均衡器
//    FCT_DEVFAMILY_TYPE_FILTER = 0x06,     //滤波器
//    FCT_DEVFAMILY_TYPE_SPLIT = 0x07,     //分频器
//    FCT_DEVFAMILY_TYPE_SIGGEN = 0x08,       //信号发生器
//    FCT_DEVFAMILY_TYPE_METER = 0x09,     //仪表显示
//    FCT_DEVFAMILY_TYPE_INPUT    =0x0A,      //输入
//    FCT_DEVFAMILY_TYPE_OUTPUT   = 0x0B,     //输出
//    FCT_DEVFAMILY_TYPE_GPI   = 0x0C,     //GPI模块
//    FCT_DEVFAMILY_TYPE_FREQSHIFT = 0x0D,   //移频
//    FCT_DEVFAMILY_TYPE_AUTOMIX = 0x0E, //自动混音
//    FCT_DEVFAMILY_TYPE_HARDAEC = 0x0F,       //硬件AEC
//    CFG_GROUP_MICROPHONECONTROL = 0xE0,   //特殊
//    //其它
//};


//QT_BEGIN_NAMESPACE
//namespace Ui { class toolTip; }
//QT_END_NAMESPACE

class ToolTip :  public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    enum{
        Type = UserType + 10,
    };
    ToolTip(QString configName, QString cfgTypeName, QString portName, bool isInput, bool isUsed, ConfigItem *configItem, QGraphicsItem *parent = nullptr);
    ~ToolTip();

    void initWindow();
    inline int getHeight() { return _high; }
    inline int getWidth() { return _with; }
    inline QString get_toolTipText() { return _toolTipText; }

    virtual int type() const { return Type; }
    void Show();
    void Hide();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString addLineBreak(QString text);

private slots:
    void setMeterValue(FC_RTU_METER *meter);
private:
    //Ui::toolTip *ui;

    QGridLayout *_layout;
    QLabel *_textLabel;

    int _with;
    int _high;
    int _maxValue;
    int _minValue;
    int _portNum;

    QString _toolTipText;
    QString _displayText;
    bool _isInput;
    QPoint _point;
    bool _isUsed;
    CfgType _cfgType;
    QPointF _pos;
    ConfigItem *_configItem;
private:
    FC_RTU_METER *_meter;
    QList<double> _meterValue;

};
#endif // TOOLTIP_H
