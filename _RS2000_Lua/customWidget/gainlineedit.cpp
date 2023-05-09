#include "gainlineedit.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QToolTip>
#include <QScreen>
#include "config.h"
#include "customWidget/overwriteitem.h"

#define SCREENHEIGH_DIV    10.0f
GainLineEdit* GainLineEdit::LastFocusedWidget = NULL; //wj2待确认



GainLineEdit::GainLineEdit(double value, int maxVal, int minVal, QWidget *parent)
    : QLineEdit(parent)
{
    _value = value;
    _maxValue = maxVal;
    _minValue = minVal;
    _decimalsNum = 0;
    _inside = false;
    _isMuteColor.setRgb(0, 255, 0);
    _defaultColor.setRgb(128, 0, 0);

    _KEMUColor1 = QColor(164,163,163);
    _KEMUColor2 = QColor(192,192,192);

    _fixedWidth = getFixedWidth();
    this->setMouseTracking(false);
    this->setFocusPolicy(Qt::ClickFocus);

    connect(this, QOverload<const QString&>::of(&GainLineEdit::textChanged), this, &GainLineEdit::onTextChanged);
    connect(this, &GainLineEdit::editingFinished, this, &GainLineEdit::onTextEditingFinished);
    this->setToolTipDuration(50000);
}

void GainLineEdit::SendValueChangedSignal()
{
    emit valueChanged(qRound(_value));
}


void GainLineEdit::setValue(double value, bool emitSignal)
{
    if(_value != value){
        limitValue(value);
        _value = value;
        if(emitSignal){
            SendValueChangedSignal();
        }
    }
}


void GainLineEdit::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    paint_Defualt();
}


void GainLineEdit::paint_Defualt()
{
    _backgroundColor.setRgb(200, 232, 244);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(QPen(Qt::black, 0.1));

    painter.setBrush(QBrush(_backgroundColor));
    if(this->hasFocus()){
        painter.setPen(QPen(Qt::red, 1.5));
    }
    else if(_inside){
        painter.setPen(QPen(Qt::yellow, 1.5));
    }
    else{
        painter.setPen(Qt::NoPen);
    }
    painter.drawRoundedRect(rect(), 5, 5);

    int decimalsNum = 1;
    if(qAbs(_value) >= 100) decimalsNum = 0;
    double dVal = qRound(_value*1000)/1000.0f; //系统内精度都只有0.01,取0.001精度足够了。
    QString dispVal = QString::number(dVal, 'f', decimalsNum);
    painter.setPen(Qt::black);
    painter.setFont(SkinStyle::getFont(8));
    painter.drawText(rect(), Qt::AlignCenter, dispVal);
}

void GainLineEdit::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton) {
        _startPoint = e->pos();
        _pressedValue = _value;
    }
    QLineEdit::mousePressEvent(e);
    LastFocusedWidget = this;
}

void GainLineEdit::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton) {
        int diff = e->pos().x() - _startPoint.x() + _startPoint.y() - e->pos().y();
        if(diff != 0){
            calcValueFromCoord(diff);
        }
     }

    QLineEdit::mouseMoveEvent(e);
}

void GainLineEdit::mouseReleaseEvent(QMouseEvent *e)
{
    QLineEdit::mouseReleaseEvent(e);
}

void GainLineEdit::enterEvent(QEvent *ev)
{
    setInside(true);
    int row = 0;
    int col = 0;
    if(this->property("col").isValid()) {
        col = this->property("col").toUInt() +1;
    }

    if(this->property("row").isValid()) {
       row = this->property("row").toUInt()+1;
    }

    QString tipText = QString("In:%1, Out%2 \nMax:%3 Min:%4").arg(col).arg(row)
            .QString::number(_maxValue, 'f', getDecimalsNum())
            .QString::number(_minValue, 'f', getDecimalsNum());

    this->setToolTip(tipText);//为了延长显示时间，showText中设置时间不起作用，而setToolTip又不能设置显示位置
    QToolTip::showText(this->mapToGlobal(QPoint(width(), -40)), tipText, this, this->rect());
}

void GainLineEdit::leaveEvent(QEvent *ev)
{
    setInside(false);
    QToolTip::hideText();
    //    QLineEdit::leaveEvent(ev);
}

bool GainLineEdit::event(QEvent *ev)
{
    return QLineEdit::event(ev);
}

void GainLineEdit::limitValue(double &value)
{
    if(value > _maxValue)
        value = _maxValue;
    else if(value < _minValue)
        value = _minValue;
}



QColor GainLineEdit::getStatusColor(bool status)
{
    if(status)
    {
        return _isMuteColor;
    }else {
        return _defaultColor;
    }
}

void GainLineEdit::adjustOnePercent(bool isAdd)
{
    _pressedValue = _value;
    int diff = isAdd ? 1 : -1;
    calcValueFromCoord(diff);
}

int GainLineEdit::getFixedWidth()
{
    if(Config::curSkinType() == Skin_FangTu) {
        return 50;
    }else {
        return 46;
    }
}

void GainLineEdit::calcValueFromCoord(int diff)
{
    double value = _pressedValue + (_maxValue - _minValue) * diff / 100.0;
    limitValue(value);
    _value = value;
    SendValueChangedSignal();
    update();
}


void GainLineEdit::onTextChanged(QString text)
{
    QToolTip::showText(this->mapToGlobal(QPoint(width(), -40)), this->text(), this);
}

void GainLineEdit::onTextEditingFinished()
{
    QToolTip::hideText();
    bool ok = false;
    double value = this->text().toDouble(&ok);
//    qDebug() << __FUNCTION__ << __LINE__ << "text: " << this->text()<< "value: " << value << "ok: " << ok;
    if(ok) {
        limitValue(value);
        _value = value;

        SendValueChangedSignal();
        this->update();
    }
    this->clear();
}
