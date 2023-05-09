#include "stable.h"
#include "trn_gainwidget.h"
#include "ui_trn_gainwidget.h"

Trn_GainWidget::Trn_GainWidget(QWidget *parent) :
    DevSettingWidget(parent),
    _ui(new Ui::Trn_GainWidget)
{
    _ui->setupUi(this);
    hide();

    QList<QPushButton *>  pushList = findChildren<QPushButton*>();
    foreach (QPushButton *pushBtn, pushList) {
        QString name = pushBtn->objectName();
        if(!name.startsWith(QLatin1String("In"))) continue;
        QList<QString> var = name.mid(2).split(QChar('_'));
        pushBtn->setProperty("channel", var.at(0).toInt());
        pushBtn->setProperty("gain", var.at(1).toInt());
        connect(pushBtn, SIGNAL(clicked()), this, SLOT(onGainSelected()));
    }
}

Trn_GainWidget::~Trn_GainWidget()
{
    delete _ui;
}

void Trn_GainWidget::showWidget(qint8 *inMicGain)
{
    for(int i = 0; i< TOTAL_INPUT; i++)
    {
        int gain = 60-3*inMicGain[i];
        QString objName =QString("In%1_%2").arg(i+1).arg(gain);

        QPushButton *pushBtn = findChild<QPushButton *>(objName);
        if(pushBtn != NULL) pushBtn->setChecked(true);
    }
    show();
}

void Trn_GainWidget::onGainSelected()
{
    quint8 channel = sender()->property("channel").toInt() -1;
    int gain = sender()->property("gain").toInt();
    int gainIdx = 20 - gain/3;
    emit gainChanged(channel, gainIdx);
}
