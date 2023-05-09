#include "cfgsettingmeter.h"
#include "configItem/configitemmeter.h"
#include "stable.h"
#include "customWidget/configmeter.h"
#include "customWidget/configmeter_fangtu.h"
#include "device/device_trn.h"
#include "Frame/configview.h"
#include "Frame/configscene.h"

CfgSettingMeter::CfgSettingMeter(ConfigItemMeter *cfgItem, QWidget *parent)
    : QWidget(parent)
{
    Q_ASSERT(cfgItem != NULL);

//    setWindowFlags(Qt::WindowStaysOnTopHint);
    _cfgItem = cfgItem;
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_METER_1:
#if Logo_FangTu
        setFixedSize(165, 350);
#else
        setFixedSize(210, 210);
#endif
        break;
    case CFG_TYPE_METER_2:
#if Logo_FangTu
        setFixedSize(225, 350);
#else
        setFixedSize(420, 210);
#endif
        break;
    case CFG_TYPE_METER_4:
#if Logo_FangTu
        setFixedSize(345, 350);
#else
        setFixedSize(840, 210);
#endif
        break;
    case CFG_TYPE_METER_8:
#if Logo_FangTu
        setFixedSize(585, 350);
#else
        setFixedSize(840, 420);
#endif
        break;
    default:
        Q_ASSERT(false);
    }
#if Logo_FangTu
    setupUi_FangTu();
#else
    setupUi();
#endif
}

void CfgSettingMeter::updateUiData()
{
#if Logo_FangTu
    for(int i = 0; i < _configMeterListFangTu.count(); i++)
    {
        _configMeterListFangTu.at(i)->init(i, _cfgItem->getHoldTime(i), _cfgItem->getHoldEnable(i), _cfgItem->getInfiniteHold(i), _cfgItem->getUserName(i));
    }
#else
    for(int i = 0; i < _configMeterList.count(); i++)
    {
        _configMeterList.at(i)->init(i, _cfgItem->getHoldTime(i), _cfgItem->getHoldEnable(i), _cfgItem->getInfiniteHold(i), _cfgItem->getUserName(i));
    }
#endif
}

void CfgSettingMeter::clearAllVuData()
{
    for (int i = 0; i < _configMeterList.count(); ++i)
    {
        _configMeterList.at(i)->meterUpdate(-100, 100);
    }
}


void CfgSettingMeter::setupUi()
{
    QGridLayout *mainLayout = new QGridLayout(this);

    for (int i = 0; i < _cfgItem->getMeterNum(); ++i)
    {
        ConfigMeter *meter = new ConfigMeter(this);
        meter->init(i, _cfgItem->getHoldTime(i), _cfgItem->getHoldEnable(i), _cfgItem->getInfiniteHold(i), _cfgItem->getUserName(i));
        connect(meter, SIGNAL(holdTimeChanged(int,int)), this, SLOT(holdTimeChanged(int,int)));
        connect(meter, SIGNAL(holdEnableChanged(int,bool)), this, SLOT(holdEnableChanged(int,bool)));
        connect(meter, SIGNAL(infiniteHoldChanged(int,bool)), this, SLOT(infiniteHoldChanged(int,bool)));
        connect(meter, SIGNAL(userNameChanged(int,QString)), this, SLOT(userNameChanged(int,QString)));
        _configMeterList.append(meter);
        mainLayout->addWidget(meter, i < 4 ? 0 : 1, i < 4 ? i : (i - 4), 1, 1);
    }

    ConfigScene* pCfgScene = qobject_cast<ConfigScene*>(_cfgItem->scene());
    if (pCfgScene != NULL)
    {
        connect(pCfgScene->getTrnDev(), SIGNAL(dispMeterA(FC_RTU_METER*)), this, SLOT(dispMeter(FC_RTU_METER *)));
    }

    setStyleSheet(QLatin1String("QPushButton{ background-color: rgb(128, 0, 0);  border: 1px none #000000;	border-radius: 4px;}"
        "QPushButton:hover{background-color: rgb(128, 0, 0); border: 2px solid #00FF00; border-radius: 4px;}"
                                "QPushButton::checked{background-color: rgb(0, 255, 0);	border: 1px none #000000; border-radius: 4px;}"));
}

void CfgSettingMeter::setupUi_FangTu()
{
    QGridLayout *mainLayout = new QGridLayout(this);
    QGroupBox *mtBox = new QGroupBox;
    QHBoxLayout *mtLayout = new QHBoxLayout;
    QVBoxLayout *labLayout = new QVBoxLayout;
    labLayout->setContentsMargins(0, 90, 0, 0);

    QLabel *lab_level        = new QLabel(tr("Input Level\n(dBFS)")); labLayout->addWidget(lab_level/*, Qt::AlignRight*/);
    QLabel *lab_HoldTime     = new QLabel(tr("Hold Time"));           labLayout->addWidget(lab_HoldTime/*, Qt::AlignRight*/);
    QLabel *lab_HoldEnable   = new QLabel(tr("Hold Enable"));         labLayout->addWidget(lab_HoldEnable/*, Qt::AlignRight*/);
    QLabel *lab_InfiniteHold = new QLabel(tr("Infinite Hold"));       labLayout->addWidget(lab_InfiniteHold/*, Qt::AlignRight*/);
    QLabel *lab_UserLab      = new QLabel(tr("User Lab"));            labLayout->addWidget(lab_UserLab/*, Qt::AlignRight*/);
    QLabel *lab_ph           = new QLabel(tr(""));                    labLayout->addWidget(lab_ph/*, Qt::AlignRight*/);
                                                lab_level->setAlignment(Qt::AlignRight); /*lab_level->setStyleSheet(QLatin1String("border: 1px solid white;"));*/
    lab_HoldTime->setFixedHeight(25);           lab_HoldTime->setAlignment(Qt::AlignRight);
    lab_HoldEnable->setFixedHeight(17);         lab_HoldEnable->setAlignment(Qt::AlignRight);
    lab_InfiniteHold->setFixedHeight(17);       lab_InfiniteHold->setAlignment(Qt::AlignRight);
    lab_UserLab->setFixedHeight(17);            lab_UserLab->setAlignment(Qt::AlignRight);
    lab_ph->setFixedHeight(12);                 lab_ph->setAlignment(Qt::AlignRight);

    mtLayout->setContentsMargins(9, 7, 7, 7);
    mtLayout->setSpacing(6);
    for (int i = 0; i < _cfgItem->getMeterNum(); ++i)
    {
        ConfigMeter_FangTu *meter = new ConfigMeter_FangTu(this);
        meter->init(i, _cfgItem->getHoldTime(i), _cfgItem->getHoldEnable(i), _cfgItem->getInfiniteHold(i), _cfgItem->getUserName(i));
        connect(meter, SIGNAL(holdTimeChanged(int,int)), this, SLOT(holdTimeChanged(int,int)));
        connect(meter, SIGNAL(holdEnableChanged(int,bool)), this, SLOT(holdEnableChanged(int,bool)));
        connect(meter, SIGNAL(infiniteHoldChanged(int,bool)), this, SLOT(infiniteHoldChanged(int,bool)));
        connect(meter, SIGNAL(userNameChanged(int,QString)), this, SLOT(userNameChanged(int,QString)));
        _configMeterListFangTu.append(meter);
        mtLayout->addWidget(meter, Qt::AlignCenter);
    }
    mtBox->setLayout(mtLayout);

    mainLayout->addLayout(labLayout, 0, 0);
    mainLayout->addWidget(mtBox, 0, 1);
    ConfigScene* pCfgScene = qobject_cast<ConfigScene*>(_cfgItem->scene());
    if (pCfgScene != NULL)
    {
        connect(pCfgScene->getTrnDev(), SIGNAL(dispMeterA(FC_RTU_METER*)), this, SLOT(dispMeter(FC_RTU_METER *)));
    }
}

void CfgSettingMeter::holdTimeChanged(int ch, int value)
{
    _cfgItem->setHoldTime(ch, value);
}

void CfgSettingMeter::holdEnableChanged(int ch, bool flag)
{
    _cfgItem->setHoldEnable(ch, flag);
}

void CfgSettingMeter::infiniteHoldChanged(int ch, bool flag)
{
    _cfgItem->setInfiniteHole(ch, flag);
}

void CfgSettingMeter::userNameChanged(int ch, QString name)
{
    _cfgItem->setUserName(ch, name);
}

void CfgSettingMeter::dispMeter(FC_RTU_METER *meter)
{
    if(!isVisible()) return;
    QSet<int> flashedChannels;
    for (int i = 0; i < METER_MAXCHANNEL; ++i)
    {
        if (!meter[i].effect) break;
        if (meter[i].modeUnitNum == _cfgItem->getModuleNum())
        {
            int channel = (int)meter[i].channelNum;
            double ppm = meter[i].ppm * 0.01 - 100;
            double vu = meter[i].vu * 0.01 - 100;

            if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
            {
                if(channel < _configMeterListFangTu.count() && !flashedChannels.contains(channel))
                {
                    _configMeterListFangTu.at(meter[i].channelNum)->meterUpdate(vu, ppm);
                    flashedChannels.insert(channel);
                }
            }
            else {
                if(channel < _configMeterList.count() && !flashedChannels.contains(channel))
                {
                    _configMeterList.at(meter[i].channelNum)->meterUpdate(vu, ppm);
                    flashedChannels.insert(channel);
                }
            }
        }
    }
}

