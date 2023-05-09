#include "uciportwidget.h"
#include "ui_uciportwidget.h"


UciPortWidget::UciPortWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UciPortWidget)
{
    ui->setupUi(this);
    setFixedSize(56, 591);
    ui->pushButton_Mute->setProperty("form", "Mute");
//    ui->textEdit_lab->verticalScrollBar()->hide();
    _opacity = new QGraphicsOpacityEffect;
    this->setGraphicsEffect(_opacity);
    _fadeValue = ui->widget_level->getRCurValue();
    _levelFade = new QTimer(this);
    connect(_levelFade, &QTimer::timeout, this, &UciPortWidget::onLevelFadeSlot);

    connect(ui->pushButton_Mute, &QPushButton::clicked, this, &UciPortWidget::onGainAndMuteChangedSlot);
    connect(ui->verticalSlider_gain, &QSlider::valueChanged, this, &UciPortWidget::onGainAndMuteChangedSlot);
    connect(ui->pushButton_gainAdd, &QPushButton::clicked, this, &UciPortWidget::onGainBtnClickedSlots);
    connect(ui->pushButton_gainMinus, &QPushButton::clicked, this, &UciPortWidget::onGainBtnClickedSlots);

    ui->pushButton_gainAdd->setStyleSheet(QLatin1String("QPushButton { border-radius: 8px; image:url(:/Image/Fangtu/gainAdd.png); }"
                                                        "QPushButton:pressed { image:url(:/Image/Fangtu/gainAdd_click.png); }"));
    ui->pushButton_gainMinus->setStyleSheet(QLatin1String("QPushButton { border-radius: 8px; image:url(:/Image/Fangtu/gainMinus.png); }"
                                                        "QPushButton:pressed { image:url(:/Image/Fangtu/gainMinus_click.png); }"));
    ui->pushButton_Mute->setStyleSheet(QLatin1String("QPushButton { "
                                                     "      border: 0px solid #8f8f91; "
                                                     "      border-radius: 5px; background-color: #C8E8F4; "
                                                     "      width: 50px; height: 18px; "
                                                     "}"
                                                     "QPushButton:checked[form = \"Mute\"] { background-color: #D11713; }"
                                                     "QPushButton:focus{ border:2px solid #ff0000; }"
                                                     "QPushButton:hover{ border:2px solid #00ff00; }"
                                                     ));
    ui->textEdit_lab->setStyleSheet(QLatin1String("QTextEdit{ "
                                                  "     border: 0px solid #8f8f91; "
                                                  "     border-radius: 5px; "
                                                  "     min-height: 35px;"
                                                  "     background-color: #C8E8F4; "
                                                  "     selection-background-color:#484848;"
                                                  "     selection-color:#DCDCDC;"
                                                  "}"
                                                  "QTextEdit:focus{ border:2px solid #ff0000; }"
                                                  "QTextEdit:hover{ border:2px solid #00ff00; }"
                                                  ));
    ui->lab_ch->setHidden(true);
    ui->lab_level_R->setHidden(true);
    ui->textEdit_lab->setEnabled(false);
}

UciPortWidget::~UciPortWidget()
{
    delete ui;
}

void UciPortWidget::setLevelValue(int value)
{
    ui->widget_level->setLCurValue(value);
    ui->lab_level_L->setText(QString::number(value));
    if((_cfgType&FCT_FAMILY_MASK) == CFG_TYPE_INPUT_1) {
        if(value <= -100) {
            ui->widget_level->setRCurValue(value);
            ui->lab_level_R->setText(QString::number(value));
        }else {
            if(getMute()) {
                ui->widget_level->setRCurValue(_fadeValue);
                ui->lab_level_R->setText(QString::number(_fadeValue));
            }else {
                ui->widget_level->setRCurValue(value + getGainValue());
                ui->lab_level_R->setText(QString::number(value + getGainValue()));
            }

        }
    }
}

void UciPortWidget::setGainRang(int minValue, int maxValue)
{
    ui->verticalSlider_gain->setRange(minValue, maxValue);

    int i = 1;
    int diffValue =  (maxValue - minValue) / 5;
    ui->label_1->setText(QString::number(maxValue));
    ui->label_2->setText(QString::number(maxValue-diffValue*i++));
    ui->label_3->setText(QString::number(maxValue-diffValue*i++));
    ui->label_4->setText(QString::number(maxValue-diffValue*i++));
    ui->label_5->setText(QString::number(maxValue-diffValue*i++));
    ui->label_6->setText(QString::number(minValue));
}

void UciPortWidget::setChannel(int ch)
{
    ui->lab_ch->setText(QString::number(ch+1));
}

void UciPortWidget::setGain(int value)
{
    ui->verticalSlider_gain->setValue(value);
}

void UciPortWidget::setMute(bool isMute)
{
    ui->pushButton_Mute->setChecked(isMute);
}

void UciPortWidget::setOpacity(qreal opacity)
{
    _opacity->setOpacity(opacity);
}

void UciPortWidget::setMark(QString mark)
{
    ui->lab_ch->setText(mark);
}

void UciPortWidget::setUserLab(QString lab)
{
    ui->textEdit_lab->setText(lab);
    ui->textEdit_lab->setAlignment(Qt::AlignCenter);
}

int UciPortWidget::getChannel()
{
    return ui->lab_ch->text().toInt() - 1;
}

int UciPortWidget::getGainValue()
{
    return ui->verticalSlider_gain->value();
}

bool UciPortWidget::getMute()
{
    return ui->pushButton_Mute->isChecked();
}

void UciPortWidget::refreshLevelDisplay()
{
    if((_cfgType&FCT_FAMILY_MASK) == CFG_TYPE_INPUT_1) {
        ui->widget_level->setProperty("SingleTrack", false);
        ui->widget_level->setProperty("ScaleLabel", false);
        ui->horizontalLayout_ch->setContentsMargins(0,0,0,0);
        ui->horizontalLayout_vu->setContentsMargins(0,0,0,0);
        ui->lab_level_R->setVisible(true);
    }else if((_cfgType&FCT_FAMILY_MASK) == CFG_TYPE_OUTPUT_1) {
        ui->widget_level->setProperty("SingleTrack", true);
        ui->widget_level->setProperty("ScaleLabel", true);
        ui->horizontalLayout_ch->setContentsMargins(15,0,0,0);
        ui->horizontalLayout_vu->setContentsMargins(15,0,0,0);
        ui->lab_level_R->setHidden(true);
    }
}

void UciPortWidget::onGainAndMuteChangedSlot()
{
    ui->label_gainValue->setText(QString::number(ui->verticalSlider_gain->value()) + "dB");
    emit gainAndMuteChanged(ui->verticalSlider_gain->value(),
                            ui->pushButton_Mute->isChecked());

    if(sender() != ui->pushButton_Mute) return;
    if(ui->pushButton_Mute->isChecked()) {
        _fadeValue = ui->widget_level->getRCurValue(); _fadeValue--;
//        qDebug() << __FUNCTION__ << __LINE__<< _fadeValue;
        _levelFade->start(6000/100);
    }else {
        _levelFade->stop();
    }
}

void UciPortWidget::onGainBtnClickedSlots()
{
    int gain = ui->verticalSlider_gain->value();
    if(sender() == ui->pushButton_gainAdd) {
        ui->verticalSlider_gain->setValue(++gain);
    }else if(sender() == ui->pushButton_gainMinus) {
        ui->verticalSlider_gain->setValue(--gain);
    }
}

void UciPortWidget::onLevelFadeSlot()
{
    if(_fadeValue <= -100) {
        _levelFade->stop();
    }
    _fadeValue--;

}
