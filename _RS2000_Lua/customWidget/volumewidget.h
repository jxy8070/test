#ifndef VOLUME_H
#define VOLUME_H

#include <QWidget>

class VolumeWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool IsApplidGain READ isApplidGain WRITE setIsApplidGain)
    Q_PROPERTY(int MaxValue READ getMaxValue WRITE setMaxValue)
    Q_PROPERTY(int MinValue READ getMinValue WRITE setMinValue)
    Q_PROPERTY(double Color1Pos READ getColor1Pos WRITE setColor1Pos)
    Q_PROPERTY(double Color2Pos READ getColor2Pos WRITE setColor2Pos)
    Q_PROPERTY(double Color3Pos READ getColor3Pos WRITE setColor3Pos)
    Q_PROPERTY(QColor Color1 READ getColor1 WRITE setColor1)
    Q_PROPERTY(QColor Color2 READ getColor2 WRITE setColor2)
    Q_PROPERTY(QColor Color3 READ getColor3 WRITE setColor3)
    Q_PROPERTY(bool SingleTrack READ isSingleTrack WRITE setSingleTrack)
    Q_PROPERTY(bool ScaleLabel READ isScaleLabel WRITE setScaleLabel)

public:
    VolumeWidget(QWidget *parent = 0);
    ~VolumeWidget();
    void setMaxValue(double value);
    void setMinValue(double value);
    double getMaxValue()const;
    double getMinValue()const;
    double getLCurValue()const;
    double getRCurValue()const;
    double getValidValue(double val);
    void setPPM(double l, double d);
    void setIsApplidGain(bool isApplid);

public:
    double getColor1Pos(){return _pos1;} const
    void setColor1Pos(double pos){_pos1 = pos;}
    double getColor2Pos(){return _pos2;} const
    void setColor2Pos(double pos){_pos2 = pos;}
    double getColor3Pos() {return _pos3;} const
    void setColor3Pos(double pos){_pos3 = pos;}
    QColor getColor1(){return _color1;} const
    void setColor1(QColor color){_color1 = color;}
    QColor getColor2(){ return _color2; } const
    void setColor2(QColor color) {_color2 = color;}
    QColor getColor3() { return _color3; } const
    void setColor3(QColor color) { _color3 = color;}
    void setSingleTrack(bool track) { _bSingle = track;}
    bool isSingleTrack() { return _bSingle; }
    void setScaleLabel(bool e) { _bScaleLabel = e; }
    bool isScaleLabel(void) { return _bScaleLabel; }   
    bool isApplidGain(void) { return _isApplidGain; }
signals:
    void LCurValue(double d);
    void RCurValue(double d);
public slots:
    void setLCurValue(double value);
    void setRCurValue(double value);
protected:
    void paintEvent(QPaintEvent *);
    void paintOld(QPainter& painter);
    void paintNew(QPainter& painter);

private:
    int _maxValue;
    int _minValue;
    double _lCurValue;
    double _rCurValue;
    double _pos1;
    double _pos2;
    double _pos3;
    QColor _color1;
    QColor _color2;
    QColor _color3;
    bool _bSingle;
    double _ppm;
    bool _bScaleLabel;
    bool _isApplidGain;
};

#endif //VOLUME_H
