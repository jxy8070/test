#include "configmixerpushbutton.h"
#include <QContextMenuEvent>
#include <QWidgetAction>
#include <QBoxLayout>
#include <QMenu>
#include <QDebug>

ConfigMixerPushButton::ConfigMixerPushButton(QWidget *parent) :
    QPushButton(parent)
{
    QWidget *popup = new QWidget(this);

    slider = new QSlider(/*Qt::Horizontal, */popup);
    slider->setRange(-100, 0);
    slider->setTickPosition(QSlider::TicksAbove);
    slider->setSliderPosition(0);
    slider->setSingleStep(1);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(buttonDispChanged(int)));

    QBoxLayout *popupLayout = new QHBoxLayout(popup);
    popupLayout->setMargin(1);
    popupLayout->addWidget(slider);

    QWidgetAction *action = new QWidgetAction(this);
    action->setDefaultWidget(popup);

    menu = new QMenu(this);
    menu->addAction(action);
}

void ConfigMixerPushButton::init(int value)
{
    slider->setValue(value);
    setText(QString("(%1)").arg(QString::number(value)));
}

void ConfigMixerPushButton::contextMenuEvent(QContextMenuEvent *)
{
    QPoint point(0, size().height());
    menu->exec(mapToGlobal(point));
}

void ConfigMixerPushButton::buttonDispChanged(int value)
{
    setText(QString("(%1)").arg(QString::number(value)));

    emit gainValueChanged(value);
}
