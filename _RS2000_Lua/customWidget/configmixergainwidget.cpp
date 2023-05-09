#include "configmixergainwidget.h"
#include "ui_configmixergainwidget.h"


configMixerGainWidget::configMixerGainWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configMixerGainWidget)
{
    ui->setupUi(this);

    ui->widget_gain->setMaxValue(0);
    ui->widget_gain->setMinValue(-100);
    ui->pushButton_mixer->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(128, 0, 0); border: 0px none #606060;}"
                                                            "QPushButton:checked{ background-color: rgb(0, 255, 0); border: 0px none #606060;}"
                                                            "QPushButton:hover{border: 2px solid #00FF00; border-radius: 4px;}"));
    connect(ui->pushButton_mixer, SIGNAL(gainValueChanged(int)), ui->widget_gain, SLOT(onRouterGainChanged(int)));
    connect(ui->pushButton_mixer, SIGNAL(clicked(bool)), ui->widget_gain, SLOT(onRouterMuteChanged(bool)));
}

configMixerGainWidget::~configMixerGainWidget()
{
    delete ui;
}

void configMixerGainWidget::resizeEvent(QResizeEvent *event)
{
    QSize changedSize = event->size() - event->oldSize();
//    qDebug() << __FUNCTION__ << __LINE__ << changedSize << this->height() << this->width()
//             << ui->widget_gain->height() << ui->widget_gain->width()
//             << ui->pushButton_mixer->height();

    if(changedSize.width())
    {
        ui->pushButton_mixer->resize(ui->widget_gain->getPieRect().width(), ui->pushButton_mixer->height());
        this->setMaximumHeight(this->width() / 2 + ui->pushButton_mixer->height());
        this->setMinimumHeight(this->width() / 2 + ui->pushButton_mixer->height());
    }
}

pieGain::pieGain(QWidget *parent)
    : QWidget(parent)
{
    _rectPie.setRect(1, 1, this->width() - 2, this->width() - 2);
    _value = 0;
    _maxValue = 0;
    _minValue = -100;
    _status = false;
    _defaultColor.setRgb(128, 0, 0);
    _inside = false;
//    qDebug() << __FUNCTION__ << __LINE__ << _rectPie;
}

void pieGain::setDefoultColor(int type)
{
    switch (type) {
    case pieGain_Type_blue:
        _defaultColor = QColor(124, 155, 207);
        break;
    case pieGain_Type_green:
        _defaultColor = QColor(105, 192, 138);
        break;
    case pieGain_Type_orange:
        _defaultColor = QColor(248, 196, 115);
        break;
    case pieGain_Type_yellow:
        _defaultColor = QColor(254, 248, 134);
        break;
    default:
        break;
    }
}

void pieGain::setSize(QRect rect)
{
    this->resize(rect.width(), rect. right());
    _rectPie.setRect(1, 1, this->width() - 2, this->width() - 2);
    this->setMinimumHeight(this->width());
}

void pieGain::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);//渲染边缘
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setBackground(Qt::white);
    //设置画笔颜色
    if(_inside) {
        QPen pen(QColor(0, 255, 0), 2);
        painter.setPen(pen);
    }else {
        painter.setPen(Qt::black);
    }
    //设置填充颜色(背景)
    QBrush brush;
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    //painter.setPen(pen);
//    QRect rect(0, 0, 35, 35);
    int startAngle = 0 * 16;
    int stopAngle = 180 * 16;
    painter.drawPie(_rectPie, startAngle, stopAngle);
//    QRect r = QRect(0, 0, this->width(), this->height());
//    painter.drawRect(r);

    //    扇形dB
    QBrush sectorBrush;
    sectorBrush.setColor(getStatusColor(_status));
    sectorBrush.setStyle(Qt::SolidPattern);
    painter.setBrush(sectorBrush);
    int startsangle = -180 * 16;
    int stopangle = getAngleValue(_value) * 16;
    painter.drawPie(_rectPie, startsangle, stopangle);

//    painter.drawRect(this->rect());
//    qDebug() << __FUNCTION__ << __LINE__ << this->rect() << _rectPie;
}


int pieGain::getAngleValue(int value)
{
    double num = _maxValue - _minValue;
    value -= _minValue;
    return (-180 / num * value);
}

QColor pieGain::getStatusColor(bool status)
{
    if(status)
    {
        return QColor(0, 255, 0);
    }else {
        return _defaultColor;
    }
}

void pieGain::onRouterGainChanged(int value)
{
    _value = value;
//    qDebug() << __FUNCTION__<< __LINE__ << value;
    this->update();
}

void pieGain::onRouterGainChanged(double value)
{
    _value = value;
//    qDebug() << __FUNCTION__<< __LINE__ << value;
    this->update();
}

void pieGain::onRouterMuteChanged(bool status)
{
//        qDebug() << __FUNCTION__<< __LINE__ << status;
    _status = status;
    this->update();
}
