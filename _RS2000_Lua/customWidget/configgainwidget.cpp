#include "configgainwidget.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QToolTip>
#include <QScreen>
#include "config.h"
#include "customWidget/overwriteitem.h"

#define SCREENHEIGH_DIV    10.0f
ConfigGainWidget* ConfigGainWidget::LastFocusedWidget = NULL;

ConfigGainWidget::ConfigGainWidget(double value, int maxVal, int minVal, CfgParamType type, QWidget *parent)
    : QLineEdit(parent),_paramType(type),
      _value(value), _maxValue(maxVal), _minValue(minVal), _decimalsNum(0),
      _KEMUColor1(QColor(164,163,163)), _KEMUColor2(QColor(192,192,192)), _isEndablePTR(false)
{
    _inside = false;
    _fixedWidth = getFixedWidth();
    _enableClicked = false;
    this->setMouseTracking(false);
    this->setFocusPolicy(Qt::ClickFocus);

    connect(this, QOverload<const QString&>::of(&ConfigGainWidget::textChanged), this, &ConfigGainWidget::onTextChanged);
    connect(this, &ConfigGainWidget::editingFinished, this, &ConfigGainWidget::onTextEditingFinished);
    _isMuteColor.setRgb(0, 255, 0);
    _defaultColor.setRgb(128, 0, 0);
    updateUiColor();
    this->setToolTipDuration(50000);
}

ConfigGainWidget::ConfigGainWidget(QWidget *parent)
    : QLineEdit(parent)
{
    _value = 0;
    _maxValue = 0;
    _minValue = -100;
    _decimalsNum = 0;
    _paramType = ParamType_Gain;
    _inside = false;
    _enableClicked = false;
    _isMuteColor.setRgb(0, 255, 0);
    _defaultColor.setRgb(128, 0, 0);

    _KEMUColor1 = QColor(164,163,163);
    _KEMUColor2 = QColor(192,192,192);

    _fixedWidth = getFixedWidth();
    updateUiColor();
    this->setMouseTracking(false);
    this->setFocusPolicy(Qt::ClickFocus);

    connect(this, QOverload<const QString&>::of(&ConfigGainWidget::textChanged), this, &ConfigGainWidget::onTextChanged);
    connect(this, &ConfigGainWidget::editingFinished, this, &ConfigGainWidget::onTextEditingFinished);
    this->setToolTipDuration(50000);
}

void ConfigGainWidget::SendValueChangedSignal()
{
    if(_paramType == ParamType_MixerGain && _isMute){
        emit valueChanged(-100);
        emit valueChanged(-100.0);
    }
    else{
        emit valueChanged(qRound(_value));
        emit valueChanged(_value);
    }
}


void ConfigGainWidget::updateUiColor()
{
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu && _fangTuIgnoreType){
        _defaultColor = QColor(69, 156, 99);
        _isMuteColor = QColor(69, 156, 99);
        return;
    }

    switch (_paramType)
    {
    case ParamType_MixerGain:
    case ParamType_Gain:
        _defaultColor = QColor(14, 224, 255);
        _isMuteColor = QColor(14, 224, 255);
        break;
    case ParamType_Freq:
        _defaultColor = QColor(105, 192, 138);
        _isMuteColor = QColor(105, 192, 138);
        break;
    case ParamType_Q:
        _defaultColor = QColor(124, 155, 207);
        _isMuteColor = QColor(124, 155, 207);
        break;
    case ParamType_Time:
        _defaultColor = QColor(254, 248, 134);
        _isMuteColor = QColor(254, 248, 134);
        break;
    case ParamType_Phase:
        _defaultColor = QColor(254, 148, 134);
        _isMuteColor = QColor(254, 148, 134);
        break;
    default:
        _defaultColor = QColor(69, 156, 99);
        _isMuteColor = QColor(69, 156, 99);
        break;
    }
}

void ConfigGainWidget::setRangeParam(double minValue, double maxValue, StepMode stepMode, CfgParamType paramType)
{
    _minValue = minValue;
    _maxValue = maxValue;
    _stepMode = stepMode;
    _paramType = paramType;
    updateUiColor();
}

void ConfigGainWidget::setValue(double value, bool emitSignal)
{
    if(_value != value){
        limitValue(value);
        _value = value;
        if(emitSignal){
            SendValueChangedSignal();
        }
    }
}

void ConfigGainWidget::setMute(bool isMute)
{
    if(_isMute != isMute) {
        _isMute = isMute;
        update();
    }
}

void ConfigGainWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if(getFixedWidth() != _fixedWidth)
    {
        _fixedWidth = getFixedWidth();
        this->setFixedSize(_fixedWidth, _fixedWidth);
    }
    _rectPie.setRect(_margeX, _margeY, this->width() - 2*_margeX, this->width() - 2*_margeX);

    if(Config::curSkinType() == Skin_FangTu) {
        paint_FangTu();
    }
    else if(Config::curSkinType() == Skin_KemuGrey ||
            Config::curSkinType() == Skin_KemuBlack  ||
            Config::curSkinType() == Skin_KemuWhite) {
        paint_KEMU();
    }
    else{
        paint_Defualt();
    }
}


void ConfigGainWidget::paint_Defualt()
{
    _backgroundColor.setRgb(200, 232, 244);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setBackground(Qt::white);
    painter.setPen(QPen(Qt::black, 0.1));

    //设置填充颜色(背景)
    QBrush brush(_backgroundColor, Qt::SolidPattern);
    painter.setBrush(brush);
    int angleStart = 0 * 16;
    int angleSpan = 180 * 16;
    painter.drawPie(_rectPie, angleStart, angleSpan);

    _rectVU = QRectF(1, this->height() / 2, _rectPie.width(), _rectPie.height() / 2 * (5/6.0f));
    painter.drawRect(_rectVU);

    //扇形dB值
    QBrush sectorBrush;
    sectorBrush.setColor(getStatusColor(_isMute));
    sectorBrush.setStyle(Qt::SolidPattern);
    painter.setBrush(sectorBrush);
    angleStart = (180 - getAngleValue(_value)) * 16;
    angleSpan = getAngleValue(_value) * 16;
    painter.drawPie(_rectPie, angleStart, angleSpan);

    if(_isMute){
        painter.setPen(Qt::red);
        painter.setFont(SkinStyle::getFont(8, true));
        painter.drawText(_rectVU, Qt::AlignCenter, "Mute");
    }
    else{
        int decimalsNum = getDecimalsNum();
        if(qAbs(_value) >= 10 && decimalsNum > 1) decimalsNum = 1;
        double dVal = qRound(_value*1000)/1000.0f; //系统内精度都只有0.01,取0.001精度足够了。
        QString dispVal = QString::number(dVal, 'f', decimalsNum);
        painter.setPen(Qt::black);
        painter.setFont(SkinStyle::getFont(8));
        painter.drawText(_rectVU, Qt::AlignCenter, dispVal);
    }

    QPainterPath _bordPath;
    _bordPath.moveTo(_rectPie.left(),_rectPie.height()/2 + _rectPie.top()+1);
    _bordPath.lineTo(_rectPie.left(), _rectVU.bottom());
    _bordPath.lineTo(_rectPie.right(), _rectVU.bottom());
    _bordPath.lineTo(_rectPie.right(), _rectPie.height()/2 + _rectPie.top()+1);
    _bordPath.arcTo(_rectPie, 0, 180);
    painter.setBrush(Qt::NoBrush);
    if(this->hasFocus()){
        painter.setPen(QPen(Qt::red, 1.5));
    }
    else if(_inside){
        painter.setPen(QPen(Qt::yellow, 1.5));
    }
    else{
        painter.setPen(Qt::NoPen);
    }
    painter.drawPath(_bordPath);
}

void ConfigGainWidget::paint_KEMU()
{
    _backgroundColor.setRgb(38, 42, 60);
    _isEndablePTR = true;
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QColor ptrColor(255, 0, 0); //指针颜色
    QPainterPath path;
    path.addRect(_rectPie);
    path = CustomRect::ScalingPerformedForCenterPointUnchanged(path, 0);
    //底层扇形
    painter.setPen(Qt::NoPen);
    QBrush brush(_KEMUColor1, Qt::SolidPattern);
    painter.setBrush(brush);
    int angleStart = 0 * 16;
    int angleSpan = 180 * 16;
    painter.drawPie(path.controlPointRect(), angleStart, angleSpan);

    //间隔扇形
    path = CustomRect::ScalingPerformedForCenterPointUnchanged(path, -2);
    painter.setBrush(_backgroundColor);
    angleStart = 0 * 16;
    angleSpan  = 180 * 16;
    painter.drawPie(path.controlPointRect(), angleStart, angleSpan);
    //扇形dB值
    path = CustomRect::ScalingPerformedForCenterPointUnchanged(path, -2);
    QColor dBColor = getStatusColor(_isMute);
    int s, h,v;
    dBColor.getHsv(&s, &h, &v);
    v = v*0.5;
    QColor dBColorBk = QColor::fromHsv(s, h, v);
    angleStart = 0 * 16;
    angleSpan = (180 - getAngleValue(_value)) * 16;;
    painter.setBrush(dBColorBk);
    painter.drawPie(path.controlPointRect(), angleStart, angleSpan);
    painter.setBrush(dBColor);
    painter.drawPie(path.controlPointRect(), angleSpan, 180 * 16 - angleSpan);

    //间隔扇形
    path = CustomRect::ScalingPerformedForCenterPointUnchanged(path, -12);
    painter.setBrush(_backgroundColor);
    angleStart = 0 * 16;
    angleSpan  = 180 * 16;
    painter.drawPie(path.controlPointRect(), angleStart, angleSpan);

    //指针部分
    path = CustomRect::ScalingPerformedForCenterPointUnchanged(path, -2);
    painter.setBrush(ptrColor);
    painter.drawEllipse(path.controlPointRect());
    if(_isEndablePTR) {
        painter.resetTransform();
        painter.translate(_rectPie.center());
        painter.setPen(QPen(ptrColor, 2));
        QLineF line(QPointF(0, 0), QPointF(3 -_rectPie.height()/2, 0));
        painter.rotate(getAngleValue(_value));
        painter.drawLine(line);
        painter.resetTransform();
    }
    //DB值矩形
    _rectVU = QRectF(_rectPie.left(), _rectPie.height()/2 + _rectPie.top()+1, _rectPie.width(), _rectPie.height() / 2 - 8);
    painter.setPen(QPen(_KEMUColor2, 1));
    painter.fillRect(_rectVU, QBrush(_KEMUColor2));
    painter.fillRect(_rectVU.left(), _rectVU.top()-1, _rectVU.width(), 1, QBrush(_backgroundColor));//分割线

    //dB值
    if(_isMute){
        painter.setPen(Qt::red);
        painter.setFont(SkinStyle::getFont(8, true));
        painter.drawText(_rectVU, Qt::AlignCenter, "Mute");
    }
    else{
        int decimalsNum = getDecimalsNum();
        if(qAbs(_value) >= 10 && decimalsNum > 1) decimalsNum = 1;
        double dVal = qRound(_value*1000)/1000.0f; //系统内精度都只有0.01,取0.001精度足够了。
        QString dispVal = QString::number(dVal, 'f', decimalsNum);
        painter.setPen(Qt::black);
        painter.setFont(SkinStyle::getFont(8));
        painter.drawText(_rectVU, Qt::AlignCenter, dispVal);
    }

    QPainterPath _bordPath;
    _bordPath.moveTo(_rectPie.left(),_rectPie.height()/2 + _rectPie.top()+1);
    _bordPath.lineTo(_rectPie.left(), _rectVU.bottom());
    _bordPath.lineTo(_rectPie.right(), _rectVU.bottom());
    _bordPath.lineTo(_rectPie.right(), _rectPie.height()/2 + _rectPie.top()+1);
    _bordPath.arcTo(_rectPie, 0, 180);
    painter.setBrush(Qt::NoBrush);
    if(this->hasFocus()){
        painter.setPen(QPen(QColor(0,255,0), 1.5));
    }
    else if(_inside){
        painter.setPen(QPen(QColor(0,255,0,160), 1.5));
    }
    else{
        painter.setPen(Qt::NoPen);
    }
    painter.drawPath(_bordPath);
}

void ConfigGainWidget::paint_FangTu()
{
    _backgroundColor.setRgb(200, 232, 244);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setBackground(Qt::white);
    painter.setPen(QPen(Qt::black, 0.1));


    //设置填充颜色(背景)
    QBrush brush(_backgroundColor, Qt::SolidPattern);
    painter.setBrush(brush);
    int angleStart = 0 * 16;
    int angleSpan = 180 * 16;
    painter.drawPie(_rectPie, angleStart, angleSpan);

    _rectVU =QRect(1, this->height() / 2, _rectPie.width(), _rectPie.height() / 2/* - 3*/);
    painter.drawRect(_rectVU);

    //扇形dB值
    QBrush sectorBrush;
    sectorBrush.setColor(getStatusColor(_isMute));
    sectorBrush.setStyle(Qt::SolidPattern);
    painter.setBrush(sectorBrush);
    angleStart = (180 - getAngleValue(_value)) * 16;
    angleSpan = getAngleValue(_value) * 16;
    painter.drawPie(_rectPie, angleStart, angleSpan);

    painter.setPen(Qt::black);

    int decimalsNum = getDecimalsNum();
    if(qAbs(_value) >= 10 && decimalsNum > 1) decimalsNum = 1;
    double dVal = qRound(_value*1000)/1000.0f; //系统内精度都只有0.01,取0.001精度足够了。
    QString dispVal = QString::number(dVal, 'f', decimalsNum);
    painter.drawText(_rectVU, Qt::AlignCenter, dispVal);

    //画边框
    QPainterPath _bordPath;
    _bordPath.moveTo(_rectPie.left(),_rectPie.height()/2 + _rectPie.top()+1);
    _bordPath.lineTo(_rectPie.left(), _rectVU.bottom());
    _bordPath.lineTo(_rectPie.right(), _rectVU.bottom());
    _bordPath.lineTo(_rectPie.right(), _rectPie.height()/2 + _rectPie.top()+1);
    _bordPath.arcTo(_rectPie, 0, 180);
    painter.setBrush(Qt::NoBrush);
    if(_inside) {
        if(this->hasFocus())
            painter.setPen(QPen(QColor(104, 180, 104), 1.5));
        else
            painter.setPen(QPen(QColor(0, 255, 0), 1.5));
    }else {
        if(this->hasFocus())
            painter.setPen(QPen(Qt::red, 1.5));
        else
            painter.setPen(QPen(Qt::black, 0.1));
    }
    painter.drawPath(_bordPath);
}


void ConfigGainWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton) {
        _startPoint = e->pos();
        _pressedValue = _value;
    }
    QLineEdit::mousePressEvent(e);
    LastFocusedWidget = this;
}

void ConfigGainWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton) {
        if (!_enableClicked)
        {
            int diff = e->pos().x() - _startPoint.x() + _startPoint.y() - e->pos().y();
            if(diff != 0){
                calcValueFromCoord(diff);
            }
        }
     }

    QLineEdit::mouseMoveEvent(e);
}

void ConfigGainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton && e->pos() == _startPoint ) {
        if(_paramType == ParamType_MixerGain) {
            if (_enableClicked) {
                _isMute = !_isMute;
                if(_isMute) { _value = 0; }
                else { _value = -100; }
                SendValueChangedSignal();
                emit muteChanged(_isMute);
            }
            update();
        }
    }
}

void ConfigGainWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    //只有ParamType_MixerGain类型才支持双击改变Mute，且需双击在文本区域
    if(_paramType == ParamType_MixerGain && _rectVU.contains(e->pos())
            && Config::curSkinType() != Skin_FangTu)
    {
        _isMute = !_isMute;
        SendValueChangedSignal();
        emit muteChanged(_isMute);
        update();
    }
}


void ConfigGainWidget::enterEvent(QEvent *ev)
{
    setInside(true);
    if(_paramType == ParamType_MixerGain) {
        int input(0);
        int output(0);
        QString tooltipText;
        if(this->property("col").isValid()) {
            int col = this->property("col").toUInt();
            input = ++col;
            tooltipText += QString("Input:%1 ").arg(input);
        }

        if(this->property("row").isValid()) {
           int row = this->property("row").toUInt();
           output = ++row;
           tooltipText += QString("Output:%1 ").arg(output);
        }

        int dispValue = _value;
        if (getDecimalsNum()) {
            tooltipText += QString("\nGain:%1 Mute:%2").arg(QString::number(_value, 'f', getDecimalsNum())).arg(isCheckedToText());
        }
        else {
            tooltipText += QString("\nGain:%1 Mute:%2").arg(dispValue).arg(isCheckedToText());
        }
        this->setToolTip(tooltipText);//为了延长显示时间，showText中设置时间不起作用，而setToolTip又不能设置显示位置
        QToolTip::showText(this->mapToGlobal(QPoint(width(), -40)), tooltipText, this, this->rect()/*, 50000*/);
    }else {
        QString tooltipText;
        int dispValue = _value;
        if (getDecimalsNum()) {
            tooltipText = QString("Value: %1 \nMax: %2 Min: %3").arg(QString::number(_value, 'f', getDecimalsNum()))
                    .arg(QString::number(_maxValue, 'f', getDecimalsNum())).arg(QString::number(_minValue, 'f', getDecimalsNum()));
        }
        else {
            tooltipText = QString("Value: %1 \nMax: %2 Min: %3").arg(dispValue)
                    .arg(QString::number(_maxValue, 'f', getDecimalsNum())).arg(QString::number(_minValue, 'f', getDecimalsNum()));
        }
        this->setToolTip(tooltipText);
        QToolTip::showText(this->mapToGlobal(QPoint(width(), -40)), tooltipText, this, this->rect()/*, 50000*/);
    }
//    QLineEdit::enterEvent(ev);
}

void ConfigGainWidget::leaveEvent(QEvent *ev)
{
    setInside(false);
    QToolTip::hideText();
    //    QLineEdit::leaveEvent(ev);
}

bool ConfigGainWidget::event(QEvent *ev)
{
    //根据厂家版本调整控件大小
    if(ev->type() == QEvent::Resize) {
        int width = getFixedWidth();
        this->setFixedSize(width, width);
        parentWidget()->layout()->setAlignment(this, Qt::AlignCenter);
    }
    return QLineEdit::event(ev);
}

void ConfigGainWidget::limitValue(double &value)
{
    if(value > _maxValue)
        value = _maxValue;
    else if(value < _minValue)
        value = _minValue;
}

int ConfigGainWidget::getAngleValue(double value)
{
    int range = 180;
    int angleVal = 0;
    if(_stepMode == 0 || _stepMode == 2){//线性
        angleVal = range * (value - _minValue)/(_maxValue - _minValue) ;
    }
    else if(_stepMode == 1){//log10指数
        angleVal = range *log10(value/_minValue) / log10(_maxValue/_minValue);
    }
    return angleVal;
}

QColor ConfigGainWidget::getStatusColor(bool status)
{
    if(status)
    {
        return _isMuteColor;
    }else {
        return _defaultColor;
    }
}

void ConfigGainWidget::adjustOnePercent(bool isAdd)
{
    _pressedValue = _value;
    int diff = isAdd ? 1 : -1;
    calcValueFromCoord(diff);
}

int ConfigGainWidget::getFixedWidth()
{
    if(Config::curSkinType() == Skin_FangTu) {
        return 50;
    }else {
        return 46;
    }
}

void ConfigGainWidget::calcValueFromCoord(int diff)
{
    if(_isMute) { //静音状态不准编辑
        return;
    }
    double value = _pressedValue;
    if(_stepMode == StepMode_Line){
        value = _pressedValue + (_maxValue - _minValue) * diff / 100.0;
    }
    else if(_stepMode == StepMode_Log10){
        double logY = log10(_maxValue / _minValue)*diff /100.0;
        value = _pressedValue*pow(10, logY);
    }
    else if(_stepMode == StepMode_LineStep) {
        int dp2 = diff *(_maxValue - _minValue) /(100 * _stepSize);
        value = _pressedValue + _stepSize * dp2;
    }

    limitValue(value);
    _value = value;
    SendValueChangedSignal();
    update();
}

QString ConfigGainWidget::isCheckedToText()
{
    if(isMute()) return "True";
    else return "False";
}


void ConfigGainWidget::onTextChanged(QString text)
{
    QToolTip::showText(this->mapToGlobal(QPoint(width(), -40)), this->text(), this);
}

void ConfigGainWidget::onTextEditingFinished()
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
