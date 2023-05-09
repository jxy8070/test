#ifndef CONFIGMIXERGAINWIDGET_H
#define CONFIGMIXERGAINWIDGET_H

#include <QDialog>
#include "configmixerpushbutton.h"

class pieGain : public QWidget
{
    Q_OBJECT
public:
    enum{
        pieGain_Type_blue = 0,
        pieGain_Type_yellow,
        pieGain_Type_orange,
        pieGain_Type_green,
    };

public:
    pieGain(QWidget *parent = nullptr);

    void setDefoultColor(int type);
    void setSize(QRect rect);
    void setInside(bool inside) { _inside = inside; this->update(); }
    inline QRect getPieRect() { return _rectPie; }
    inline bool getStatus() { return _status; }
    inline void setMaxValue(int value) { _maxValue = value; }
    inline void setMinValue(int value) { _minValue = value; }
    inline void setDefoultColor(QColor color) { _defaultColor = color; }

protected:
    void paintEvent(QPaintEvent *event) override;
//    void resizeEvent(QResizeEvent *event) override;

    int getAngleValue(int value);
    QColor getStatusColor(bool status);
public slots:
    void onRouterGainChanged(int);
    void onRouterGainChanged(double);
    void onRouterMuteChanged(bool);
private:
    QRect _rectPie;
    int _value;
    int _maxValue;
    int _minValue;
    bool _status;
    bool _inside;
    QColor _defaultColor;
};

namespace Ui {
class configMixerGainWidget;
}

class configMixerGainWidget : public QDialog
{
    Q_OBJECT

public:
    explicit configMixerGainWidget(QWidget *parent = nullptr);
    ~configMixerGainWidget();

protected:
    void resizeEvent(QResizeEvent *) override;
private:
    Ui::configMixerGainWidget *ui;
};

#endif // CONFIGMIXERGAINWIDGET_H
