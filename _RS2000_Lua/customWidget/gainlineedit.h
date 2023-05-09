#ifndef GAINLINEEDIT_H
#define GAINLINEEDIT_H

#include <QWidget>
#include <QMouseEvent>
#include <QLineEdit>



class GainLineEdit : public QLineEdit
{
    Q_OBJECT

    Q_PROPERTY(double Value READ value WRITE setValue USER true)
    Q_PROPERTY(double MaxValue READ getMaxValue WRITE setMaxValue USER true)
    Q_PROPERTY(double MinValue READ getMinValue WRITE setMinValue USER true)

public:
    GainLineEdit(double value = 0, int maxVal = 0, int minVal = -100, QWidget *parent = nullptr);

    void adjustOnePercent(bool isAdd);
    static int getFixedWidth();

    void setValue(double value, bool emitSignal = false);
    inline void setInside(bool inside) { _inside = inside; update(); }
    inline double value() { return _value; }
    inline int getMaxValue() { return _maxValue; }
    inline int getMinValue() { return _minValue; }
    inline int getDecimalsNum() { return _decimalsNum; }
    inline void setMaxValue(double value) { _maxValue = value; }
    inline void setMinValue(double value) { _minValue = value; }
    inline void setDecimalsNum(int num) { _decimalsNum = num; this->update(); }
    inline void setRange(double minValue, double maxValue){_minValue = minValue; _maxValue = maxValue; limitValue(_value); }

    static GainLineEdit* LastFocusedWidget;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void enterEvent(QEvent *ev) override;
    void leaveEvent(QEvent *ev) override;
    bool event(QEvent *ev) override;
    void limitValue(double &value);
    int getAngleValue(double value);
    QColor getStatusColor(bool status);
    void calcValueFromCoord(int diff);

private:
    void SendValueChangedSignal();
    void paint_Defualt();

signals:
    void valueChanged(double);

public slots:
    void onTextChanged(QString);
    void onTextEditingFinished();

private:
    double _stepSize;           //调整步长
    double _value;              //Gain值
    double _maxValue;           //Gain最大值
    double _minValue;           //Gain最小值
    int _decimalsNum;           //小数点后几位
    bool _inside;               //鼠标是否进入
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


#endif // GAINLINEEDIT_H
