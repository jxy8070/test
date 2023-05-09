#ifndef CONFIGMIXERNEWPUSHBUTTON_H
#define CONFIGMIXERNEWPUSHBUTTON_H

#include <QPushButton>
#include <QSlider>
#include <QMouseEvent>
#include <QWheelEvent>

enum Status{
    NewBtn_NotSelect = 0,
    NewBtn_IsSelect,
    NewBtn_InsideItem,
    NewBtn_externalItem,
    NewBtn_InputType,
    NewBtn_OutputType,
    NewBtn_DefaultType,
};

class configMixerNewPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit configMixerNewPushButton(QWidget *parent = 0);
    void init(int value);

signals:
    void gainValueChanged(int);

protected:
    void contextMenuEvent(QContextMenuEvent *) override;

//    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;//鼠标进入事件
    void leaveEvent(QEvent *event) override;//鼠标离开事件

    void paintEvent(QPaintEvent *) override;
//    void wheelEvent(QWheelEvent *ev) override;

    int getAngleValue(int value);
    QColor getStatusColor();

public slots:
    void buttonDispChanged(int value);

private:
    QSlider *slider;
    QMenu *menu;

    int _value;
    Status _mouseStatus;
};

#endif // CONFIGMIXERNEWPUSHBUTTON_H
