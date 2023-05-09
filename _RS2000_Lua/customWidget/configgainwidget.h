#ifndef CONFIGGAINWIDGET_H
#define CONFIGGAINWIDGET_H

///////////////////////2022-3-17 hrp///////////////////////

#include <QWidget>
#include <QMouseEvent>
#include <QLineEdit>
#include <QDebug>

enum StepMode{
    StepMode_Line = 0,  //线性
    StepMode_Log10,     //指数
    StepMode_LineStep,  //指定步长
};

enum CfgParamType{
    ParamType_MixerGain = 1,    //混音增益
    ParamType_Gain,             //增益
    ParamType_Q,                //Q值
    ParamType_Freq,             //频率
    ParamType_Time,             //时间
    ParamType_Phase,            //相位
    ParamType_Other,            //其它
};

class ConfigGainWidget : public QLineEdit
{
    Q_OBJECT

    Q_PROPERTY(double Value READ value WRITE setValue USER true)
    Q_PROPERTY(double MaxValue READ getMaxValue WRITE setMaxValue USER true)
    Q_PROPERTY(double MinValue READ getMinValue WRITE setMinValue USER true)
    Q_PROPERTY(int Decimals READ getDecimalsNum WRITE setDecimalsNum USER true)
    Q_PROPERTY(int StepMode READ getStepMode WRITE setStepMode)
    Q_PROPERTY(int ParamType READ getParamType WRITE setParamType)
public:
    ConfigGainWidget(double value = 0, int maxVal = 0, int minVal = -100,
                     CfgParamType type = ParamType_Gain, QWidget *parent = nullptr);
    ConfigGainWidget(QWidget *parent = nullptr);

    void adjustOnePercent(bool isAdd);
    static int getFixedWidth();

    void setRangeParam(double minValue, double maxValue, StepMode stepMode, CfgParamType paramType);
    void setValue(double value, bool emitSignal = false);
    void setMute(bool isMute);
    inline void setInside(bool inside) { _inside = inside; update(); }
    inline QRectF getPieRect() { return _rectPie; }
    inline bool isMute() { return _isMute; }
    inline double value() { return _isMute ? -100 : _value; }
    inline int getMaxValue() { return _maxValue; }
    inline int getMinValue() { return _minValue; }
    inline int getDecimalsNum() { return _decimalsNum; }
    inline int getStepMode() { return _stepMode; }
    inline int getParamType() { return _paramType; }
    inline void setMaxValue(double value) { _maxValue = value; }
    inline void setMinValue(double value) { _minValue = value; }
    inline void setDecimalsNum(int num) { _decimalsNum = num; this->update(); }
    inline void SetEnbleClicked(bool flag) {_enableClicked = flag;}
    inline void setSingleStep(double step) { _stepSize = step; _stepMode = StepMode_LineStep; }
    inline void setStepMode(int stepMode) {_stepMode = (StepMode)stepMode;} //调整模式，0:线性， 1:指数 2:指定步长
    inline void setRange(double minValue, double maxValue){_minValue = minValue; _maxValue = maxValue; limitValue(_value); }
    inline void setFangTuIgnoreType(bool ignore) {_fangTuIgnoreType = ignore; updateUiColor(); }
    inline void setParamType(int type) { _paramType = (CfgParamType)type; updateUiColor(); }

    static ConfigGainWidget* LastFocusedWidget;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void enterEvent(QEvent *ev) override;
    void leaveEvent(QEvent *ev) override;
    bool event(QEvent *ev) override;
    void updateUiColor();
    void limitValue(double &value);
    int getAngleValue(double value);
    QColor getStatusColor(bool status);
    void calcValueFromCoord(int diff);

private:
    void SendValueChangedSignal();
    QString isCheckedToText();
    void paint_Defualt();
    void paint_KEMU();
    void paint_FangTu();

signals:
    void valueChanged(int);
    void valueChanged(double);
    void muteChanged(bool isMute);

public slots:
    void onTextChanged(QString);
    void onTextEditingFinished();

private:
    CfgParamType _paramType;
    bool _fangTuIgnoreType = true;
    StepMode _stepMode = StepMode_Line;
    double _stepSize;           //调整步长
    bool _enableClicked;        //允许单击
    double _value;              //Gain值
    double _maxValue;           //Gain最大值
    double _minValue;           //Gain最小值
    int _decimalsNum;           //小数点后几位
    bool _isMute = false;       //mixer节点静音
    bool _inside;               //鼠标是否进入
    QRectF _rectPie;            //半圆rect
    QRectF _rectVU;             //半圆rect
    QPainterPath _bordPath;     //边框path
    QColor _defaultColor;       //默认颜色（半圆）
    QColor _isMuteColor;     //按下后颜色
    QColor _backgroundColor;    //背景颜色
    QPoint _startPoint;         //鼠标左键点击坐标
    double _pressedValue;       //鼠标按下的值
    int _fixedWidth = 50;
    float _margeX = 1;
    float _margeY = 1;

    //KEMU
    QColor _KEMUColor1;
    QColor _KEMUColor2;
    bool _isEndablePTR;
};


#endif // CONFIGGAINWIDGET_H
