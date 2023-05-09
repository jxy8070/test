#include "configdelaynewdly.h"
#include "ui_configdelaynewdly.h"
#include <QToolTip>

configDelayNewDly::configDelayNewDly(QWidget *parent, int type) :
    QDialog(NULL),
    ui(new Ui::configDelayNewDly)
{
    ui->setupUi(this);

    _value = 0;
    _isCheckable = false;
    _NewDlyType = type;
    ui->doubleSpinBox->setMinimumHeight(19);
    ui->verticalLayout->setContentsMargins(0, 0, 0, 6);
    switch(_NewDlyType) {
    case NewDlyType_Defaut:
    {
        ui->doubleSpinBox->setStyleSheet(QLatin1String("QDoubleSpinBox{ border:1px solid #242424; selection-background-color:#484848;"
                                                       "selection-color:#DCDCDC; }"
                                                       "QDoubleSpinBox::up-button"
                                                       "{ image:url(:/psblack/add_top.png); width:9px; padding:2px 2px 0px 0px; }"
                                                       "QDoubleSpinBox::down-button"
                                                       "{ image:url(:/psblack/add_bottom.png); width:9px; padding:0px 2px 2px 0px; }"
                                                       "QDoubleSpinBox::up-button:pressed"
                                                       "{ top: -2px; }"
                                                       "QDoubleSpinBox::down-button:pressed"
                                                       "{ bottom:-2px; }"
                                                       "QDoubleSpinBox::hover"
                                                       "{ border: 2px solid #00FF00; }"));
        break;
    }
    case NewDlyType_Mixer:
    {
        ui->doubleSpinBox->setStyleSheet(QLatin1String("QDoubleSpinBox{ border:1px solid #242424; selection-background-color:#484848;"
                                                       "selection-color:#DCDCDC;}"
                                                       "QDoubleSpinBox::hover"
                                                       "{ border: 2px solid #00FF00; }"
                                                       "QDoubleSpinBox::up-button,QDoubleSpinBox::down-button"
                                                       "{ width:0px; }"));
        ui->doubleSpinBox->setAlignment(Qt::AlignHCenter);
        break;
    }
    default:
        break;
    }
//    connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), ui->widget_pie, SLOT(onRouterGainChanged(double)));
    connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onRouterGainChanged(double)));
    connect(this, SIGNAL(sendMouseClickedSignal(bool)), ui->widget_pie, SLOT(onRouterMuteChanged(bool)));
}

configDelayNewDly::~configDelayNewDly()
{
    delete ui;
}


QDoubleSpinBox *configDelayNewDly::getDoubleSpinBox()
{
    return ui->doubleSpinBox;
}

pieGain *configDelayNewDly::getPieGain()
{
    return ui->widget_pie;
}

void configDelayNewDly::setMaximum(double max)
{
    ui->widget_pie->setMaxValue(max);
    ui->doubleSpinBox->setMaximum(max);
}

void configDelayNewDly::setMinimum(double min)
{
    ui->widget_pie->setMinValue(min);
    ui->doubleSpinBox->setMinimum(min);
}

void configDelayNewDly::setChecked(bool status)
{
    emit sendMouseClickedSignal(status);
}

bool configDelayNewDly::isChecked()
{
    return ui->widget_pie->getStatus();
}

void configDelayNewDly::resizeEvent(QResizeEvent *event)
{
    QSize changedSize = event->size() - event->oldSize();
//    qDebug() << __FUNCTION__ << __LINE__ << changedSize <<"this:" << this->height() << this->width()
//             << "widget_pie:" << ui->widget_pie->height() << ui->widget_pie->width()
//             << "doubleSpinBox:" << ui->doubleSpinBox->height() << ui->doubleSpinBox->width();

    if(changedSize.width())
    {
        ui->widget_pie->setSize(this->rect());
        ui->doubleSpinBox->setMinimumWidth(ui->widget_pie->getPieRect().width());
        ui->doubleSpinBox->setMinimumHeight(ui->widget_pie->getPieRect().height() / 2);
        ui->doubleSpinBox->setMaximumWidth(ui->widget_pie->getPieRect().width());
        ui->doubleSpinBox->setMaximumHeight(ui->widget_pie->getPieRect().height() / 2);
//        qDebug() << __FUNCTION__ << __LINE__ << this->rect() << ui->doubleSpinBox->size() << ui->widget_pie->getPieRect();
//        this->setMaximumHeight(this->width() / 2 + ui->doubleSpinBox->height() + 3);
//        this->setMinimumHeight(this->width() / 2 + ui->doubleSpinBox->height());
    }
}

bool configDelayNewDly::event(QEvent *event)
{
    if(event->type() == QEvent::ToolTip)
    {
        QHelpEvent *tooltip = static_cast<QHelpEvent *>(event);
        QToolTip::showText(tooltip->globalPos(), QString("value: %1").arg(ui->doubleSpinBox->value()));
    }
    return QWidget::event(event);
}

void configDelayNewDly::mousePressEvent(QMouseEvent *ev)
{
    if(!_isCheckable) return ;
    emit sendMouseClickedSignal(!ui->widget_pie->getStatus());
    emit clicked();
}

void configDelayNewDly::enterEvent(QEvent *ev)
{
    if(_isCheckable)
        ui->widget_pie->setInside(true);
}

void configDelayNewDly::leaveEvent(QEvent *ev)
{
    ui->widget_pie->setInside(false);
}

void configDelayNewDly::onRouterGainChanged(double value)
{
    ui->widget_pie->onRouterGainChanged(value);
    emit gainValueChanged(value);
}
