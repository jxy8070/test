#ifndef CONFIGMIXERROTARYKNOB_H
#define CONFIGMIXERROTARYKNOB_H

#include <QDial>
#include <QPainter>
#include <QMouseEvent>
#include <QRect>
//#include "configSetting/cfgsettingmixer.h"

enum{
    NotSelect = 0,
    IsSelect,
    InsideItem,
    InputType,
    OutputType,
    DefaultType,
};

class CfgSettingMixer;
class configMixerRotaryKnob : public QDial
{
    Q_OBJECT
public:

    configMixerRotaryKnob(int type = DefaultType, QWidget* parent = 0);

    inline void setIsSelect(int status) { _isSelect = status; }
    inline bool getIsSelect() { return _isSelect; }
    inline void setNodeSwitch(bool status) { _nodeSwitch = status; }
    inline bool getNodeSwitch() { return _nodeSwitch; }
    inline void setPrevValue(int value) { _prevValue = value; }

    void paintEvent(QPaintEvent* e);

    //void mouseMoveEvent(QMouseEvent* e);//鼠标移动
    void mousePressEvent(QMouseEvent* e);//鼠标按下
    void mouseReleaseEvent(QMouseEvent* e);//鼠标释放
    void enterEvent(QEvent *event);//鼠标进入事件
    void leaveEvent(QEvent *event);//鼠标离开事件

    void setGainValue(int value);
private slots:

private:
    int _isSelect;
    int _mouseStatus;
    //int _value;
    int _prevValue;
    QString _textDB;
    int _angle;
    int _type;
    bool _nodeSwitch;

    CfgSettingMixer *_cfgSettingMixer;
};

#endif // CONFIGMIXERROTARYKNOB_H
