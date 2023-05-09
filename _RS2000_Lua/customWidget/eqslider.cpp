#include "stable.h"
#include "eqslider.h"
#include "eqgraph.h"
#include "config.h"


EqSlider::EqSlider(int idx, int fx, int gain, bool bypass, bool isShowTick, QWidget *parent) :
    QWidget(parent)
{
    _idx = idx;
    if(fx<1000) _fxString = QString::number(fx);
    else
    {
        _fxString = QString::number(fx/1000.0, 'f', 1) + "k";
    }
    _isShowTick = isShowTick;
    setupUi();
    setObjectName("eqSlider");
    connect(_gainSlider, SIGNAL(sliderMoved(int)), this, SLOT(onGainChanged()));
    connect(_gainEdit, SIGNAL(editingFinished()), this, SLOT(onGainChanged()));
    connect(_bypass, SIGNAL(clicked(bool)), this, SLOT(onBypassClicked(bool)));

    setGainValue(gain);
    setBypass(bypass);
    setStyleSheet("*{font:9px;}"); //确保能显示完成文"-20.0"
}

void EqSlider::setupUi()
{
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);

    _layout = new QVBoxLayout();
    setLayout(_layout);
    _layout->setSpacing(4);
    _layout->setContentsMargins(2, 2, 2, 2);
    _upLabel = new QLabel(this);
    _upLabel->setSizePolicy(sizePolicy);
    _upLabel->setAlignment(Qt::AlignCenter);
    _upLabel->setText(_fxString);
    _layout->addWidget(_upLabel);

    _gainSlider = new QSlider(this);
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(1);
    _gainSlider->setSizePolicy(sizePolicy1);
    _gainSlider->setOrientation(Qt::Vertical);
    _gainSlider->setMaximum(1500);
    _gainSlider->setSingleStep(1.0);
    _gainSlider->setMinimum(-3000);

    if(_isShowTick) _gainSlider->setTickPosition(QSlider::TicksBothSides);
    _layout->addWidget(_gainSlider);

    _gainEdit = new QLineEdit(this);
    _gainEdit->setFixedHeight(18);
    _gainEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _gainEdit->setText("0.0");
    _gainEdit->setAlignment(Qt::AlignHCenter);
    _layout->addWidget(_gainEdit);

    _bypass = new QPushButton(this);
    _bypass->setCheckable(true);
    _bypass->setChecked(false);
    _bypass->setProperty("form", "Bypass");
    _layout->addWidget(_bypass);
    setFixedWidth(35);

}

//重写paintEven使自定义类支持QSS
void EqSlider::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, & opt, & painter, this);
}

int EqSlider::gainValue()
{
    return _gainSlider->value();
}

bool EqSlider::getBypass()
{
    return _bypass->isChecked();
}

void EqSlider::setGainValue(int value)
{
    if(_lastGainInt != value){
        _lastGainInt = value;
        _gainEdit->setText(QString::number(value * 0.01, 'f', 1));
        _gainSlider->setValue(value);
    }
}

void EqSlider::setCurrent(bool isCurrent)
{
    _isSelected = isCurrent;
    if(isCurrent) setStyleSheet(QLatin1String("background-color:#FFFF00"));
    else setStyleSheet(QLatin1String("background-color:#F0F0F0"));
}

void EqSlider::setBypass(bool enable)
{
    disconnect(_bypass, SIGNAL(clicked(bool)), this, SLOT(onBypassClicked(bool)));
    _bypass->setChecked(enable);
    connect(_bypass, SIGNAL(clicked(bool)), this, SLOT(onBypassClicked(bool)));
}

void EqSlider::onBypassClicked(bool enable)
{
    emit bypassClicked(_idx, enable);
}

#include "Frame/mainwindow.h"
void EqSlider::onGainChanged()
{
    int gainInt = 0;
    if(sender() == _gainEdit){
        bool valid;
        float gain = _gainEdit->text().toFloat(&valid);
        if(valid) {
            gainInt = gain * 100;
            gainInt = gainInt > _gainSlider->maximum()?  _gainSlider->maximum() : gainInt;
            gainInt = gainInt < _gainSlider->minimum()?  _gainSlider->minimum() : gainInt;
        }
        else{
            MainWindow::instance()->showNotice(tr("invalid gain:") +_gainEdit->text(), 1000);
            _gainEdit->setText(QString::number(_gainSlider->value() * 0.01, 'f', 1));
            return;
        }
    }
    else if(sender() == _gainSlider){
        gainInt = _gainSlider->value();
    }

    if(_lastGainInt != gainInt){
        setGainValue(gainInt);
        emit gainChanged(_idx, gainInt);
    }
}
