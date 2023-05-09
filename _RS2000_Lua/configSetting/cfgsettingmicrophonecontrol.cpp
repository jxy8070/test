#include "cfgsettingmicrophonecontrol.h"
#include "configItem/configitemmicrophonecontrol.h"
#include "stable.h"
#include <QComboBox>

CfgSettingMicrophoneControl::CfgSettingMicrophoneControl(ConfigItemMicrophoneControl *cfgItem, QWidget *parent)
    :QWidget(parent)
{
    _cfgItem = cfgItem;
    CfgType cfgType = cfgItem->getCfgType();
    switch (cfgType) {
    case CFG_TYPE_VOICETUBE_1:
        //       _rowNum = 2;
        //       _colNum = 2;
        _microphoneNum = 32;
        _maxNum = 32;
//        setFixedSize(300,200);
        break;
    case CFG_TYPE_VOICETUBE_2:
        _microphoneNum = 48;
        _maxNum = 48;
        break;
    default:
        Q_ASSERT(false);
    }
    setFixedSize(300,200);
    setupUi();
    setMicrophoneControl();

    connect(_box_microphoneNo, SIGNAL(currentIndexChanged(int)), this, SLOT(onMicrophoneNoClicked(int)));
    connect(_box_MaxNum, SIGNAL(currentIndexChanged(int)), this, SLOT(onMicrophoneNoClicked(int)));
}

void CfgSettingMicrophoneControl::setupUi()
{
    QGridLayout *mainLayout = new QGridLayout(this);
    addHeader(mainLayout);
}

void CfgSettingMicrophoneControl::addHeader(QGridLayout *mainLayout)
{
    QLabel *label_MicroponeNum = new QLabel(this);
    QLabel *label_MaxNum = new QLabel(this);
    label_MicroponeNum->setText("Microphone Control: ");
    label_MaxNum->setText("Max number of connections allowed:");
    mainLayout->addWidget(label_MicroponeNum,0,0,1,1,Qt::AlignCenter);
    mainLayout->addWidget(label_MaxNum,1,0,1,1,Qt::AlignCenter);

    _box_microphoneNo = new QComboBox(this);
    _box_MaxNum = new QComboBox(this);
    for(int i = 0; i <= _microphoneNum; i++)
    {
        if(i == 0)
            _box_microphoneNo->addItem(QString("None"));
        else
            _box_microphoneNo->addItem(QString("No: %1").arg(i));
    }
    for(int j = 1; j <= _maxNum; j++)
    {
        _box_MaxNum->addItem(QString("    %1 ").arg(j));
    }
    mainLayout->addWidget(_box_microphoneNo,0,1,1,1,Qt::AlignCenter);
    mainLayout->addWidget(_box_MaxNum,1,1,1,1,Qt::AlignCenter);
    mainLayout->addWidget(new QWidget);
}

void CfgSettingMicrophoneControl::setMicrophoneControl()
{
    _box_microphoneNo->setCurrentIndex(_cfgItem->getMicrophoneNo());
    _box_MaxNum->setCurrentIndex(_cfgItem->getMaxConnectNum());
}

void CfgSettingMicrophoneControl::onMicrophoneNoClicked(int index)
{
    int type = 0;
    if(sender() == _box_microphoneNo)
    {
//        qDebug()<<"index:"<<index;
        type = 0;
        _cfgItem->setMicrophoneControlInfo(index,type);
    }else if(sender() == _box_MaxNum)
    {
        type = 1;
        _cfgItem->setMicrophoneControlInfo(index,type);
//        qDebug()<<"index:"<<index;
    }
}



