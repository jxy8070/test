#include "tooltip.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QTimerEvent>

ToolTip::ToolTip(QString configName, QString cfgTypeName, QString portName,
                 bool isInput, bool isUsed, ConfigItem *configItem, QGraphicsItem *parent)
    : QGraphicsRectItem(parent)

{
    _isInput = isInput;
    _isUsed = isUsed;
    _cfgType = configItem->getCfgType();
    _meter = NULL;
    _configItem = configItem;

    if(portName.contains(" "))
    {
//        _portNum = QString(portName.split(" ").at(1)[1]).toInt() - 1;
        QString str = QString(portName.split(" ").at(1));
        str.remove(QString(")"));
        str.remove(QString("("));
        _portNum = str.toInt() - 1;

//        qDebug()<<__FUNCTION__<<__LINE__<<portName<<str<<_portNum;
    }else {
        if(configItem->getCfgType() == CFG_TYPE_SPLIT_2)
        {
            if(portName == "L") { _portNum = 0; }
            if(portName == "H") { _portNum = 1; }
        }
        if(configItem->getCfgType() == CFG_TYPE_SPLIT_3)
        {
            if(portName == "L") { _portNum = 0; }
            if(portName == "M") { _portNum = 1; }
            if(portName == "H") { _portNum = 2; }
        }
        if(configItem->getCfgType() == CFG_TYPE_SPLIT_4)
        {
            if(portName == "L")  { _portNum = 0; }
            if(portName == "LM") { _portNum = 1; }
            if(portName == "HM") { _portNum = 2; }
            if(portName == "H")  { _portNum = 3; }
        }
    }

    _maxValue = 20;
    _minValue = -100;

    _with = 70;
    _high = 120;

    _meterValue.clear();
    for(int i = 0; i < 64; i++)
    {
        _meterValue.append(-100);
    }

//    _layout = new QGridLayout();
//    _textLabel = new QLabel();
//    _textLabel->setWordWrap(true);//换行策略
//    _textLabel->setAlignment(Qt::AlignTop);
//    _layout->addWidget(_textLabel);
//    _textLabel->setText(_toolTipText);

    switch(_cfgType) {
    case CFG_TYPE_INPUT_1:
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        _toolTipText = configName + " - " + cfgTypeName + "\n" + portName;
//        _with = 160;
//        _high = 50;
        break;
    case CFG_TYPE_OUTPUT_1:
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        _toolTipText = configName + " - " + cfgTypeName + "\n" + portName;
//        _with = 160;
//        _high = 50;
        break;
    case CFG_TYPE_DELAY_5:
    case CFG_TYPE_DELAY_10:
    case CFG_TYPE_DELAY_20:
    case CFG_TYPE_DELAY_50:
    case CFG_TYPE_DELAY_100:
    case CFG_TYPE_DELAY_500:
    case CFG_TYPE_DELAY_1000:
        _toolTipText = configName + " - " + cfgTypeName + "\n" + portName;
//        _with = 160;
//        _high = 50;
        break;
    case CFG_TYPE_ROUTER_4:
    case CFG_TYPE_ROUTER_8:
    case CFG_TYPE_ROUTER_16:
    case CFG_TYPE_ROUTER_32:
    case CFG_TYPE_ROUTER_64:
        _toolTipText = configName + " - " + cfgTypeName + "\n" + portName;
//        _with = 160;
//        _high = 50;
        break;
    case CFG_TYPE_MIXER_4X1:
    case CFG_TYPE_MIXER_4X2:
    case CFG_TYPE_MIXER_4X4:
    case CFG_TYPE_MIXER_8X1:
    case CFG_TYPE_MIXER_8X2:
    case CFG_TYPE_MIXER_8X4:
    case CFG_TYPE_MIXER_8X8:
    case CFG_TYPE_MIXER_16X16:
    case CFG_TYPE_MIXER_32X32:
    case CFG_TYPE_MIXER_64X64:
        _toolTipText = configName + " - " + cfgTypeName + "\n" + portName;
//        _with = 160;
//        _high = 50;
        break;
    case CFG_TYPE_DYN_AUTO:
    case CFG_TYPE_DYN_COMP:
    case CFG_TYPE_DYN_LIMIT:
    case CFG_TYPE_DYN_NOISE:
    case CFG_TYPE_DYN_EXPAND:
    case CFG_TYPE_DYN_PEAK:
        _toolTipText = configName + " - " + cfgTypeName + "\n" + portName;
//        _with = 160;
//        _high = 50;
        break;
    case CFG_TYPE_EQ_1:
    case CFG_TYPE_EQ_3:
    case CFG_TYPE_EQ_5:
    case CFG_TYPE_EQ_7:
    case CFG_TYPE_EQ_10:
    case CFG_TYPE_EQ_16:
    case CFG_TYPE_EQ_1X3:
    case CFG_TYPE_EQ_2X3:
    case CFG_TYPE_EQ_3X3:
        _toolTipText = configName + " - " + cfgTypeName + "\n" + portName;
//        _with = 160;
//        _high = 50;
        break;
    case CFG_TYPE_FILTE_LP:
    case CFG_TYPE_FILTE_HP:
    case CFG_TYPE_FILTE_LS:
    case CFG_TYPE_FILTE_HS:
        _toolTipText = configName + " - " + cfgTypeName + "\n" + portName;
//        _with = 160;
//        _high = 30;
        break;
    case CFG_TYPE_SPLIT_2:
    case CFG_TYPE_SPLIT_3:
    case CFG_TYPE_SPLIT_4:
        _toolTipText = configName + " - " + cfgTypeName + "\n" + portName;
//        _with = 160;
//        _high = 50;
        break;
    case CFG_TYPE_SIG_SINE:
    case CFG_TYPE_SIG_NOISE:
    case CFG_TYPE_SIG_PINKNOISE:
        _toolTipText = configName + " - " + cfgTypeName + "\n" + portName;
//        _with = 160;
//        _high = 30;
        break;
    case CFG_TYPE_METER_1:
    case CFG_TYPE_METER_2:
    case CFG_TYPE_METER_4:
    case CFG_TYPE_METER_8:
        _toolTipText = cfgTypeName + "\n" + configName + "\n" + portName;
        _with = 70;
        _high = 120;
        break;
    case CFG_TYPE_GPI:

//        _with = 160;
//        _high = 30;
        break;
    case CFG_TYPE_FREQSHIFT:
    case CFG_TYPE_FREQSHIFT_PLUS:

//        _with = 160;
//        _high = 50;
        break;
    case CFG_TYPE_AUTOMIX_GS_8:

//        _with = 160;
//        _high = 50;
        break;
    case CFG_TYPE_AEC_2_2:

//        _with = 160;
//        _high = 50;
        break;
    case CFG_TYPE_VOICETUBE_1:
    case CFG_TYPE_VOICETUBE_2:
//        _with = 160;
//        _high = 50;
        break;
    default:
//        _toolTipText = QString("No CfgGroup Device");//加 \n 实现换行
//        _with = 160;
//        _high = 30;
        break;
    }
    _displayText = _toolTipText;
    //_displayText = addLineBreak(_toolTipText);
    initWindow();
}

ToolTip::~ToolTip()
{
    //delete ui;
}

void ToolTip::initWindow()
{
    //窗口不可移动
    //this->setWindowFlags(Qt::FramelessWindowHint);//窗口无边框
    //this->setAttribute(Qt::WA_TranslucentBackground);//半透明背景
    //this->resize(_with, _high);

    this->setRect(0, 0, _with, _high);
//    qDebug()<<__FUNCTION__<<this->pos();

    //this->move(_point.toPoint());
}

void ToolTip::Show()
{
    ConfigScene* pScene = qobject_cast<ConfigScene*>(scene());
    if(pScene != NULL)
    {
        if( _cfgType == CFG_TYPE_METER_1 || _cfgType == CFG_TYPE_METER_2 || _cfgType == CFG_TYPE_METER_4 || _cfgType == CFG_TYPE_METER_8)
        {
            connect(pScene->getTrnDev(), SIGNAL(dispMeterA(FC_RTU_METER*)), this, SLOT(setMeterValue(FC_RTU_METER*)));
        }
    }

    qDebug()<<__FUNCTION__<<__LINE__<<_cfgType<<_portNum;
    this->show();
}

void ToolTip::Hide()
{
    ConfigScene* pScene = qobject_cast<ConfigScene*>(scene());
    if(pScene != NULL)
    {
        if(_cfgType == CFG_TYPE_METER_1 || _cfgType == CFG_TYPE_METER_2 || _cfgType == CFG_TYPE_METER_4 || _cfgType == CFG_TYPE_METER_8)
        {
            disconnect(pScene->getTrnDev(), SIGNAL(dispMeterA(FC_RTU_METER*)), this, SLOT(setMeterValue(FC_RTU_METER*)));
        }
    }
    this->hide();
}

void ToolTip::setMeterValue(FC_RTU_METER *meter)
{
    if(!_meterValue.isEmpty())
    {
        _meterValue.clear();
    }
    //qDebug()<<__FUNCTION__<<__LINE__<<"setMeterValue";
    for (int i = 0; i < METER_MAXCHANNEL; ++i)
    {
        if (!meter[i].effect) return;
        if (meter[i].modeUnitNum == _configItem->getModuleNum())
        {
            double ppm = meter[i].ppm * 0.01 - 100;
            double vu = meter[i].vu * 0.01 -100;
//            qDebug() << "ppm" << ppm << "vu" << vu<< meter[i].modeUnitNum<<"i:"<<i<<_portNum<<meter[i].channelNum;
            //_configMeterList.at(meter[i].channelNum)->meterUpdate(vu, ppm);

            _meterValue.append(vu);
            update();
        }
    }
}

void ToolTip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(QColor(250, 250, 210));

    QPointF points[5] = {QPointF(0, 0), QPointF(_with, 0), QPointF(_with, _high-10), QPointF(10, _high-10), QPointF(0, _high)};
    painter->drawPolygon(points, 5);

//    switch (_cfgType) {
//    case CFG_TYPE_METER_1:
//    case CFG_TYPE_METER_2:
//    case CFG_TYPE_METER_4:
//    case CFG_TYPE_METER_8:
//    {
        int x = 40, y = 5;
        int h = 96, w = 15;

//        painter->drawText(QRect(x+w+w, y, 105, 105), Qt::AlignHCenter, _displayText);

        int m = y;
        for(int i = _maxValue; i >= _minValue; i -= 20)
        {
            painter->drawText(QRect(15, m-2, 25, 10), Qt::AlignHCenter, QString("%1").arg(i));
            m += 15;
        }

        QLinearGradient lg = QLinearGradient(x, y, x, y+h);//垂直渐变
        lg.setColorAt(0, Qt::red);
        lg.setColorAt(0.0917, Qt::yellow);
        lg.setColorAt(1, Qt::green);
        painter->setBrush(lg);
        painter->setPen(Qt::darkGray);
        painter->drawRect(QRectF(x, y, w, h));

        for(int i = 0; i < h/3; i++)
        {
            painter->drawLine(x, y+i*3, x+w, y+i*3);
        }
        int val = 0;
        if(_meterValue.isEmpty())
        {
            val = _minValue;
        }else {
            val = _meterValue[_portNum];
        }
//        qDebug()<<__FUNCTION__<<__LINE__<<"Val:"<<val<<_minValue<<_meterValue[_portNum];
        val = val - 20;
        val = (h * -1.0)/(20 + 100) * val;
//        qDebug()<<__FUNCTION__<<__LINE__<<"Val:"<<val;
        painter->setPen(Qt::darkGray);
        painter->setBrush(Qt::darkGray);
        painter->drawRect(QRectF(x, y-1, w, val));

//        painter->setPen(Qt::darkRed);
//        painter->setBrush(Qt::darkRed);
//        painter->drawRect(QRect(10, y+h+5, 65, 15));
//        painter->setPen(Qt::yellow);
//        painter->drawText(QRect(10, y+h+5, 65, 15), Qt::AlignCenter, QString("%1 dBu").arg(_meterValue));
//        break;
//    }
//    default:
//        painter->drawText(QRect(2, 2, _with-4, _high-4), Qt::AlignHCenter, _displayText);
//        break;
//    }

}


QString ToolTip::addLineBreak(QString text)
{
    int length = 0;
    QString displayText;
    displayText.clear();
    qDebug()<<__FUNCTION__<<__LINE__<<text;
    if(text.contains(" "))
    {
        QStringList textList = text.split(" ");
        qDebug()<<__FUNCTION__<<__LINE__<<textList;
        for(int i = 0; i < textList.count(); i++)
        {
            length += textList[i].length() + 1;
            if(length <= 230/10 || length <= 240/10)
            {
                displayText.append(textList[i] + " ");
            }else {
                displayText.append("\n");
                length = 0;
            }
        }
    }else {
        int i = 0;
        while(i != text.length()-1)
        {
           displayText += text.mid(i, 200/10) + "\n";
           i++;
        }
    }
    qDebug()<<__FUNCTION__<<__LINE__<<displayText;
    return displayText;
}

