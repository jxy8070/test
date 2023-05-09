#include "stable.h"
#include "devsetting_trn.h"
#include "deviceItem/deviceitem_trn.h"
#include "device/device_trn.h"
#include "trn/trn_gainwidget.h"
#include "trn/trn_copywidget.h"
#include "trn/trn_delaywidget.h"
#include "trn/trn_levelwidget.h"
#include "trn/trn_meterwidget.h"
#include "trn/trn_mixerwidget.h"
#include "trn/trn_dynwidget.h"
#include "trn/trn_scenewidget.h"
#include "customWidget/eqviewshg.h"
#include "trn/eqview_allband.h"
#include "customWidget/filterview.h"

const QString m_Gain_dBString[21] = {"+60", "+57", "+54", "+51", "+48", "+45", "+42", "+39", "+36", "+33", "+30", "+27", "+24", "+21", "+18", "+15", "+12", "+9", "+6", "+3", "0"};

QString DevSetting_TRN::getChannelName(int ch, bool isInput)
{
    QString inOut("In");
    if(!isInput) inOut= "Out";
    QString name;
    if(ch <8) name = QString("A-%1 %2 ").arg(inOut).arg(ch +1);
    else if(ch==8 || ch == 10) name = QString("D-%1 %2L ").arg(inOut).arg((ch -6)/2);
    else if(ch==9 || ch == 11) name = QString("D-%1 %2R ").arg(inOut).arg((ch -6)/2);
    else name = QString("NET-%1 %2 ").arg(inOut).arg((ch -11));
    return name;
}


DevSetting_TRN::DevSetting_TRN(DeviceItem_Trn *devItem, QWidget *parent) :
    QScrollArea(parent)
{
    _devItem = devItem;
    Q_ASSERT(devItem->getDanteDevice());
    _devBase =qobject_cast<Device_Trn*>(devItem->getDanteDevice());
    _gainWidget = NULL;
    _mixerWidget = NULL;
    _copyWidget = NULL;
    _inDelayWidget = NULL;
    _outDelayWidget = NULL;
    _inLevelWidget = NULL;
    _outLevelWidget = NULL;
    _dynWidget = NULL;
    _inEqWidget = NULL;
    _outEq15BWidget = NULL;
    _outSplitWidget = NULL;
    _inMeterWidget = NULL;
    _outMeterWidget = NULL;

    _fontBord.setBold(true);
    _fontBord.setPointSize(8);
    _fontNormal.setPointSize(8);
    for(int i = 0; i< 20; i++)
    {
        _buttonsList.append(new PushButtons());
    }

    initUi();

    _nameLabel->setText(devItem->customName());
    _ipLabel->setText(_devBase->getDeviceIp());
    _macLabel->setText(_devBase->getMac());

    _sceneSelectWidget = new Trn_SceneWidget;
    connect(_sceneSelectWidget, SIGNAL(sceneSelected(quint8)), this, SLOT(saveToScene(quint8)));

    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        initConfig(trn->getConfigData());
        connect(trn, SIGNAL(onConfigChanged(ConfigData_Trn)), this, SLOT(initConfig(ConfigData_Trn)));
        connect(trn, SIGNAL(dispMeter(quint16*,quint16*)), this, SLOT(dispMeter(quint16*,quint16*)));
    }
}

DevSetting_TRN::~DevSetting_TRN()
{
    qDeleteAll(_buttonsList);
}

void DevSetting_TRN::onPhantomPowerClicked(bool isOpen)
{
    if (isOpen == true)
    {
        int ret = QMessageBox::question(NULL,
                                        tr("Informations"),
                                        tr("+48V phantom power will open, are you sure continue?"),
                                        QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::No)
        {
            qobject_cast<QPushButton *>(sender())->setChecked(false);
            return;
        }
    }

    int channel = sender()->property("channel").toInt();
    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        trn->sendPhantomPack(channel, isOpen);
    }
}

void DevSetting_TRN::onGainWidgetChanged(quint8 ch, qint8 value)
{
    _buttonsList[ch]->micGain->setText(m_Gain_dBString[value]);

    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        trn->sendMicGainPack(ch, value);
    }
}

void DevSetting_TRN::clearCheckedButton()
{
    _btnHide->setChecked(true);
}

void DevSetting_TRN::childWidgetClose()
{
    if (_gainWidget != NULL && _gainWidget->isVisible()) _gainWidget->close();

    if (_inDelayWidget != NULL &&_inDelayWidget->isVisible()) _inDelayWidget->close();

    if (_outDelayWidget != NULL &&_outDelayWidget->isVisible()) _outDelayWidget->close();

    if (_inLevelWidget != NULL &&_inLevelWidget->isVisible()) _inLevelWidget->close();

    if (_outLevelWidget != NULL &&_outLevelWidget->isVisible()) _outLevelWidget->close();

    if (_mixerWidget != NULL &&_mixerWidget->isVisible()) _mixerWidget->close();

    if (_dynWidget != NULL &&_dynWidget->isVisible()) _dynWidget->close();

    if (_inEqWidget != NULL &&_inEqWidget->isVisible()) _inEqWidget->close();

    if (_outEq15BWidget != NULL &&_outEq15BWidget->isVisible()) _outEq15BWidget->close();

    if (_outSplitWidget != NULL &&_outSplitWidget->isVisible()) _outSplitWidget->close();

    if (_inMeterWidget != NULL &&_inMeterWidget->isVisible()) _inMeterWidget->close();

    if (_outMeterWidget != NULL &&_outMeterWidget->isVisible()) _outMeterWidget->close();

    if (_copyWidget != NULL && _copyWidget->isVisible()) _copyWidget->close();
}

void DevSetting_TRN::onInDelayClicked()
{
    clearCheckedButton();
    childWidgetClose();
    if(_inDelayWidget == NULL)
    {
        _inDelayWidget = new Trn_DelayWidget(true, _groupBoxSub);
        connect(_inDelayWidget, SIGNAL(delayChanged(quint8,quint16)), this, SLOT(onInDelayWidgetChanged(quint8,quint16)));
    }
    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        _inDelayWidget->showWidget(trn->getConfigData().InDelay);
    }
}

void DevSetting_TRN::onOutDelayClicked()
{
    clearCheckedButton();
    childWidgetClose();
    if(_outDelayWidget == NULL)
    {
        _outDelayWidget = new Trn_DelayWidget(false, _groupBoxSub);
        connect(_outDelayWidget, SIGNAL(delayChanged(quint8,quint16)), this, SLOT(onOutDelayWidgetChanged(quint8,quint16)));
    }
    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        _outDelayWidget->showWidget(trn->getConfigData().OutDelay);
    }
}

void DevSetting_TRN::onInLevelClick()
{
    clearCheckedButton();
    childWidgetClose();
    if(_inLevelWidget == NULL)
    {
        _inLevelWidget = new Trn_LevelWidget(true, _groupBoxSub);
        connect(_inLevelWidget, SIGNAL(levelChanged(quint8,float)), this, SLOT(onInLevelWidgetChanged(quint8,float)));
    }
    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        _inLevelWidget->showWidget(trn->getConfigData().InVolum);
    }
}

void DevSetting_TRN::onOutLevelClick()
{
    clearCheckedButton();
    childWidgetClose();
    if(_outLevelWidget == NULL)
    {
        _outLevelWidget = new Trn_LevelWidget(false, _groupBoxSub);
        connect(_outLevelWidget, SIGNAL(levelChanged(quint8,float)), this, SLOT(onOutLevelWidgetChanged(quint8,float)));
    }
    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        _outLevelWidget->showWidget(trn->getConfigData().OutVolum);
    }
}

void DevSetting_TRN::onInDynClicked(bool isChecked)
{
    Q_UNUSED(isChecked)
    childWidgetClose();
    if(_dynWidget == NULL)
    {
        _dynWidget = new Trn_DynWidget(_groupBoxSub);
        _dynWidget->move(440,0);
        connect(_dynWidget, SIGNAL(dynChanged(quint8, bool,DynData)), this, SLOT(onDynWidgetChanged(quint8, bool,DynData)));
    }
    int ch = sender()->property("channel").toInt();
    QString chName = getChannelName(ch, true);
    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        _dynWidget->showWidget(true, ch, chName, trn->getConfigData().InputComp[ch]);
    }
}

void DevSetting_TRN::onOutDynClicked(bool isChecked)
{
    Q_UNUSED(isChecked)
    childWidgetClose();
    if(_dynWidget == NULL)
    {
        _dynWidget = new Trn_DynWidget(_groupBoxSub);
        _dynWidget->move(440,0);
        connect(_dynWidget, SIGNAL(dynChanged(quint8, bool,DynData)), this, SLOT(onDynWidgetChanged(quint8, bool,DynData)));
    }
    int ch = sender()->property("channel").toInt();
    QString chName = getChannelName(ch, false);
    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        _dynWidget->showWidget(false, ch, chName, trn->getConfigData().OutputComp[ch]);
    }
}

void DevSetting_TRN::onInEqClicked(bool isChecked)
{
    Q_UNUSED(isChecked)
    childWidgetClose();
    if(_inEqWidget == NULL)
    {        
        _inEqWidget = new EqView_AllBand(3, _groupBoxSub);
        _inEqWidget->hide();
        _inEqWidget->move(250,0);
        _inEqWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        connect(_inEqWidget, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqWidgetChanged(quint8, bool, EqData*)));
    }
    int ch = sender()->property("channel").toInt();
    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        EqData3B eqPara = trn->getConfigData().InputEQ[ch];
        QString chName = getChannelName(ch, true) + "Eq";
        _inEqWidget->showWidget(ch, chName, eqPara.bypass, eqPara.eq);
    }

}


void DevSetting_TRN::onOutEq15BClicked(bool isChecked)
{
    Q_UNUSED(isChecked)
    childWidgetClose();
    if(_outEq15BWidget == NULL)
    {
        _outEq15BWidget = new EqViewSHG(EqViewSHG::SHG_15, _groupBoxSub);
        _outEq15BWidget->hide();
        _outEq15BWidget->move(400,0);
        _outEq15BWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        connect(_outEq15BWidget, SIGNAL(eqDataChanged(quint8,bool,qint16*)), this, SLOT(onOutEqWidgetChanged(quint8,bool,qint16*)));
    }
    int ch = sender()->property("channel").toInt();
    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        EqData15S eq15B = trn->getConfigData().OutputEQ[ch];

#ifdef OUTPUT_EQ_10SEGMENT
    	QString chName = getChannelName(ch, false) + tr("10Band Eq");
#else
    	QString chName = getChannelName(ch, false) + tr("15Band Eq");
#endif

        _outEq15BWidget->showWidget(ch, chName, eq15B.bypass, eq15B.gain);
    }

}

void DevSetting_TRN::onOutSplitClicked(bool isChecked)
{
    Q_UNUSED(isChecked)
    childWidgetClose();
    if(_outSplitWidget == NULL)
    {
        _outSplitWidget = new FilterView(_groupBoxSub);
        _outSplitWidget->hide();
        _outSplitWidget->move(250,0);
        _outSplitWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        connect(_outSplitWidget, SIGNAL(eqDataChanged(quint8,SplitData)), this, SLOT(onOutSplitWidgetClicked(quint8,SplitData)));
    }
    int ch = sender()->property("channel").toInt();
    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        SplitData splitData = trn->getConfigData().OutSplit[ch];
        QString chName = getChannelName(ch, false) + tr("Split");
        _outSplitWidget->showWidget(ch, chName, splitData);
    }
}

void DevSetting_TRN::onInMeterClicked()
{
    clearCheckedButton();
    childWidgetClose();
    if(_inMeterWidget == NULL)
    {
        _inMeterWidget = new Trn_MeterWidget(true, _groupBoxSub);
    }
    _inMeterWidget->show();
}

void DevSetting_TRN::onOutMeterClicked()
{
    clearCheckedButton();
    childWidgetClose();
    if(_outMeterWidget == NULL)
    {
        _outMeterWidget = new Trn_MeterWidget(false, _groupBoxSub);
    }
    _outMeterWidget->show();
}

void DevSetting_TRN::dispMeter(quint16 *meterI, quint16 *meterO)
{
    if (_inMeterWidget != NULL && _inMeterWidget->isVisible())
    {
        _inMeterWidget->dispMeter(meterI);
    }
    if (_outMeterWidget != NULL && _outMeterWidget->isVisible())
    {
        _outMeterWidget->dispMeter(meterO);
    }
}

void DevSetting_TRN::onMicGainClicked()
{
    clearCheckedButton();
    childWidgetClose();
    if(_gainWidget == NULL)
    {
        _gainWidget = new Trn_GainWidget(_groupBoxSub);
        connect(_gainWidget, SIGNAL(gainChanged(quint8,qint8)), this, SLOT(onGainWidgetChanged(quint8,qint8)));
    }
    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        _gainWidget->showWidget(trn->getConfigData().InMicGain);
    }
}


void DevSetting_TRN::onMixerClicked()
{
    clearCheckedButton();
    childWidgetClose();
    if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
    {
        Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
        if(_mixerWidget == NULL)
        {
            _mixerWidget = new Trn_MixerWidget(_groupBoxSub);
            connect(_mixerWidget, SIGNAL(mixerVolumeChanged(int,int,quint32)), this, SLOT(onMixerWidgetChanged(int,int,quint32)));
            connect(_mixerWidget, SIGNAL(editNameChanged(int,QString)), trn, SLOT(setUserName(int,QString)));
        }
        _mixerWidget->userNameInit(trn->getUserName());
    }
    _mixerWidget->showWidget(qobject_cast<Device_Trn *>(_devBase)->getConfigData().MixerVolum);
}

void DevSetting_TRN::saveToScene(quint8 num)
{
	if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
	{
		Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
	    trn->sendSaveScenePack(num);
	    Device_Trn *standbyTrn = qobject_cast<Device_Trn*>(_devItem->getStandbyDanteDevice());
	    if (standbyTrn != NULL)
	    {
	        standbyTrn->sendSaveScenePack(num);
	    }
	}
}

void DevSetting_TRN::initConfig(const ConfigData_Trn& cfgData)
{
    childWidgetClose();
    _sceneLabel->setText(tr("Scene %1").arg(QString::asprintf("%.2d", cfgData.CurrentSceneNum+1)));

    for(int i = 0; i< 8; i++)
    {
        _buttonsList[i]->micGain->setText(m_Gain_dBString[cfgData.InMicGain[i]]);
        _buttonsList[i]->phantom->setChecked(cfgData.InMic48V[i]);
    }

    for(int i = 0; i< 20; i++)
    {
        _buttonsList[i]->phase->setChecked(cfgData.InMicPhaseV[i]);
        _buttonsList[i]->outLevel->setText(QString::number(20.0 * log10(cfgData.OutVolum[i] / 131072.0), 'f', 1));
        _buttonsList[i]->outDelay->setText(QString::number(cfgData.OutDelay[i]));
    }

}

void DevSetting_TRN::onSave2DeviceClicked()
{
    _sceneSelectWidget->show();
}

void DevSetting_TRN::onSave2LocalClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("save as..."), QDir::currentPath(), tr("file (*.ctrl)"));
    if(!fileName.isEmpty()){
        if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
        {
            Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
            trn->serialize(fileName);
        }
    }
}


void DevSetting_TRN::onLoadCfgClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("file (*.ctrl)"));
    if(!fileName.isEmpty())
    {
        if (_devBase->getType() == DEV_TYPE_DNTE_TRN)
        {
            Device_Trn *trn = qobject_cast<Device_Trn *>(_devBase);
            trn->deserialize(fileName);
            trn->setSendConfigType(Device_Trn::SendAllConfigType_Load);
            initConfig(trn->getConfigData());
        }

//        QMessageBox::information(this, tr("Message"),
//                                 tr("Load Success"), QMessageBox::Ok);
    }
}

void DevSetting_TRN::initUi()
{    
    resize(1320, 680);
    QWidget *mainWidget = new QWidget();
    mainWidget->setFixedSize(1500, 1200);
    setWidget(mainWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    QGroupBox *groupBoxInfo = new QGroupBox(this);
    mainLayout->addWidget(groupBoxInfo);
    QGroupBox *groupBoxPara = new QGroupBox(this);
    mainLayout->addWidget(groupBoxPara);
    _groupBoxSub = new QGroupBox(this);
    _btnGroup = new QButtonGroup(this);
    _btnHide = new QPushButton(this);
    _btnHide->setCheckable(true);
    _btnHide->setAutoExclusive(true);
    _btnHide->hide();
    _btnGroup->addButton(_btnHide);
    mainLayout->setMargin(2);
    mainLayout->setSpacing(2);
    mainLayout->addWidget(_groupBoxSub);
    mainLayout->setStretch(0,0);
    mainLayout->setStretch(1,0);
    mainLayout->setStretch(2,1);
    initInfoUi(groupBoxInfo);
    initParaUi(groupBoxPara);

    groupBoxPara->setStyleSheet(QLatin1String(
        "QFrame:hover{ \n"
        "    background-color: rgb(128, 128, 0);}"
        "QPushButton[qssType=\"1\"]{"
        "    color: rgb(0, 0, 0);"
        "    background-color: rgb(255, 255, 255);"
        "    border-width: 1px;"
        "    border-style: solid;}"
        "QPushButton[qssType=\"2\"] {"
        "    background-color: rgb(128, 0, 0);"
        "    border-color: rgb(128,128,128);"
        "    border-width: 1px;"
        "    border-style: solid;"
        "    border-radius: 2px;}"
        "QPushButton:hover[qssType=\"2\"] {"
        "    background-color: rgb(128, 0, 0);"
        "    border-color: rgb(0,255,0);"
        "    border-width: 1px;"
        "    border-style: solid;"
        "    border-radius: 2px; }"
        "QPushButton:checked[qssType=\"2\"] {"
        "    background-color: rgb(0, 255, 0);"
        "    border-color: rgb(128,128,128);"
        "    border-width: 1px;"
        "    border-style: solid;"
        "    border-radius: 2px; }"
        "QPushButton:checked:hover[qssType=\"2\"] {"
        "    background-color: rgb(0, 255, 0);"
        "    border-color: rgb(0,0,255);"
        "    border-width: 1px;"
        "    border-style: solid;"
        "    border-radius: 2px; }"
        "QPushButton[qssType=\"3\"] {"
        "    color: rgb(0,128,0);"
        "    background-color: rgb(0, 0, 128);"
        "    border-width: 1px;"
        "    border-style: solid;"
        "    border-radius: 2px; }"
        "QPushButton:hover[qssType=\"3\"] {"
        "    color: rgb(0,255,0);"
        "    background-color: rgb(0, 0, 128);"
        "    border-width: 1px;"
        "    border-style: solid;"
        "    border-radius: 2px; }"
        "QPushButton:checked[qssType=\"3\"] {"
        "    background-color: rgb(255, 0, 0);"
        "    border-color: rgb(128,128,128);"
        "    border-width: 1px;"
        "    border-style: solid;"
        "    border-radius: 2px; }"
        ));
}

void DevSetting_TRN::initInfoUi(QGroupBox *groupBoxInfo)
{
    QGridLayout *gridLayout = new QGridLayout(groupBoxInfo);
    gridLayout->setContentsMargins(2, 2, 2, 2);
    gridLayout->setObjectName(QStringLiteral("gridLayoutInfo"));
    QLabel * lblInfoName = new QLabel(groupBoxInfo);;
    gridLayout->addWidget(lblInfoName, 0, 0, 1, 1);
    lblInfoName->setText(QApplication::translate("FCCtrl", "Name:", 0));

    _sceneLabel = new QLabel(groupBoxInfo);
    gridLayout->addWidget(_sceneLabel, 0, 3, 1, 1);
    _sceneLabel->setText(QApplication::translate("FCCtrl", "Scene 01", 0));

    _nameLabel = new QLabel(groupBoxInfo);
    gridLayout->addWidget(_nameLabel, 0, 1, 1, 1);
    _nameLabel->setText(QApplication::translate("FCCtrl", "TextLabel", 0));

    QLabel *lblInfoScene = new QLabel(groupBoxInfo);
    gridLayout->addWidget(lblInfoScene, 0, 2, 1, 1);
    lblInfoScene->setText(QApplication::translate("FCCtrl", "Current Scene:", 0));

    QPushButton *loadCfgBtn = new QPushButton(groupBoxInfo);
    gridLayout->addWidget(loadCfgBtn, 1, 4, 2, 1);
    loadCfgBtn->setText(QApplication::translate("FCCtrl", "Load", 0));
    connect(loadCfgBtn, SIGNAL(clicked()), this, SLOT(onLoadCfgClicked()));

    QPushButton *save2DeviceBtn = new QPushButton(groupBoxInfo);
    gridLayout->addWidget(save2DeviceBtn, 1, 2, 2, 1);
    save2DeviceBtn->setText(QApplication::translate("FCCtrl", "Save To Device", 0));
    connect(save2DeviceBtn, SIGNAL(clicked()), this, SLOT(onSave2DeviceClicked()));

    QLabel *lblInfoIp = new QLabel(groupBoxInfo);
    gridLayout->addWidget(lblInfoIp, 1, 0, 1, 1);
    lblInfoIp->setText(QApplication::translate("FCCtrl", "IP Address:", 0));

    QLabel *lblInfoMac = new QLabel(groupBoxInfo);
    gridLayout->addWidget(lblInfoMac, 2, 0, 1, 1);
    lblInfoMac->setText(QApplication::translate("FCCtrl", "MAC Address:", 0));

    _ipLabel = new QLabel(groupBoxInfo);
    gridLayout->addWidget(_ipLabel, 1, 1, 1, 1);
    _ipLabel->setText(QApplication::translate("FCCtrl", "TextLabel", 0));

    _macLabel = new QLabel(groupBoxInfo);
    gridLayout->addWidget(_macLabel, 2, 1, 1, 1);
    _macLabel->setText(QApplication::translate("FCCtrl", "TextLabel", 0));

    QPushButton *save2LocalBtn = new QPushButton(groupBoxInfo);
    gridLayout->addWidget(save2LocalBtn, 1, 3, 2, 1);
    save2LocalBtn->setText(QApplication::translate("FCCtrl", "Save To Local", 0));
    connect(save2LocalBtn, SIGNAL(clicked()), this, SLOT(onSave2LocalClicked()));

    QPushButton *copyBtn = new QPushButton(groupBoxInfo);
        gridLayout->addWidget(copyBtn, 1, 5, 2, 1);
        copyBtn->setText(QApplication::translate("FCCtrl", "Copy", 0));
        connect(copyBtn, SIGNAL(clicked()), this, SLOT(onCopyClicked()));

    QWidget *widget = new QWidget(groupBoxInfo);
    gridLayout->addWidget(widget, 1, 5, 1, 1);

    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 2);
    gridLayout->setColumnStretch(2, 1);
    gridLayout->setColumnStretch(3, 1);
    gridLayout->setColumnStretch(4, 1);
    gridLayout->setColumnStretch(5, 1);
    gridLayout->setColumnStretch(6, 2);
}

void DevSetting_TRN::initParaUi(QGroupBox *groupBoxPara)
{
    QGridLayout *gridLayoutPara = new QGridLayout(groupBoxPara);
    gridLayoutPara->setVerticalSpacing(0);
    gridLayoutPara->setHorizontalSpacing(2);
    gridLayoutPara->setMargin(0);
    initParaHeaderUi(gridLayoutPara);
    int rowIdx = 0;
    for(int i=0; i< 8; i++)
    {
        QString inputText = QString("A-In %1").arg(i+1).rightJustified(9, QLatin1Char(' '));
        QString outputText = QString("A-Out %1").arg(i+1).leftJustified(10, QLatin1Char(' '));
        initInputUi(gridLayoutPara, inputText, rowIdx);
        initOutputUi(gridLayoutPara, outputText, rowIdx);
        rowIdx++;
    }
    for(int i=0; i<2; i++)
    {
        QString inputText1 = QString("D-In %1L").arg(i+1).rightJustified(9, QLatin1Char(' '));
        QString outputText1 = QString("D-Out %1L").arg(i+1).leftJustified(10, QLatin1Char(' '));
        initInputUi(gridLayoutPara, inputText1, rowIdx);
        initOutputUi(gridLayoutPara, outputText1, rowIdx);
        rowIdx++;
        QString inputText2 = QString("D-In %1R").arg(i+1).rightJustified(9, QLatin1Char(' '));
        QString outputText2 = QString("D-Out %1R").arg(i+1).leftJustified(10, QLatin1Char(' '));
        initInputUi(gridLayoutPara, inputText2, rowIdx);
        initOutputUi(gridLayoutPara, outputText2, rowIdx);
        rowIdx++;
    }
    for(int i=0; i<8; i++)
    {
        QString inputText = QString("NET-In %1").arg(i+1).rightJustified(9, QLatin1Char(' '));
        QString outputText = QString("NET-Out %1").arg(i+1).leftJustified(10, QLatin1Char(' '));
        initInputUi(gridLayoutPara, inputText, rowIdx);
        initOutputUi(gridLayoutPara, outputText, rowIdx);
        rowIdx++;
    }

    QPushButton *mixerButton = new QPushButton(groupBoxPara);
    mixerButton->setFocusPolicy(Qt::NoFocus);
    mixerButton->setAutoFillBackground(true);
    mixerButton->setStyleSheet(QStringLiteral("background-image: url(:/Image/MatrixControlMixer.png);"));
    mixerButton->setFlat(true);
    mixerButton->setGeometry(0, 0, 111, 300);
    mixerButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayoutPara->addWidget(mixerButton, 1, 1, rowIdx, 1);
    connect(mixerButton, SIGNAL(clicked()), this, SLOT(onMixerClicked()));

    gridLayoutPara->setColumnMinimumWidth(1,112);
    gridLayoutPara->setColumnStretch(0, 17);
    gridLayoutPara->setColumnStretch(1, 0);
    gridLayoutPara->setColumnStretch(2, 17);
}

void DevSetting_TRN::setInputLayoutStretch(QHBoxLayout *horizontalLayout)
{
    horizontalLayout->setStretch(0,0);
    horizontalLayout->setStretch(1,1);
    horizontalLayout->setStretch(2,1);
    horizontalLayout->setStretch(3,1);
    horizontalLayout->setStretch(4,1);
    horizontalLayout->setStretch(5,1);
    horizontalLayout->setStretch(6,1);
    horizontalLayout->setStretch(7,1);
    horizontalLayout->setStretch(8,1);
    horizontalLayout->setStretch(9,1);
    horizontalLayout->setStretch(10,1);
    horizontalLayout->setStretch(11,1);
    horizontalLayout->setStretch(12,1);
    horizontalLayout->setStretch(13,1);
    horizontalLayout->setStretch(14,0);
}

void DevSetting_TRN::initInputUi(QGridLayout *gridLayoutPara, QString inputText, int rowIdx)
{
    QFrame *frame = new QFrame();
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    QHBoxLayout *horizontalLayout = new QHBoxLayout(frame);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setMargin(0);

    QLabel *lblInput = new QLabel(frame);
    lblInput->setText(inputText);
    horizontalLayout->addWidget(lblInput);

    AddSeparate(horizontalLayout);

    if(rowIdx < 8)
    {
        QPushButton *micGain = new QPushButton(frame);
        micGain->setProperty("qssType", "1");
        micGain->setProperty("channel", rowIdx);
        micGain->setFont(_fontNormal);
        micGain->setFlat(false);
        micGain->setText(QString("0"));
        horizontalLayout->addWidget(micGain);
        _buttonsList[rowIdx]->micGain =micGain;
        connect(micGain, SIGNAL(clicked()), this, SLOT(onMicGainClicked()));

        AddSeparate(horizontalLayout);

        QPushButton *phanton48v = new QPushButton(frame);
        phanton48v->setProperty("qssType", "2");
        phanton48v->setProperty("channel", rowIdx);
        phanton48v->setFont(_fontBord);
        phanton48v->setCheckable(true);
        phanton48v->setChecked(false);
        phanton48v->setFlat(false);
        horizontalLayout->addWidget(phanton48v);
        _buttonsList[rowIdx]->phantom =phanton48v;
        connect(phanton48v, SIGNAL(clicked(bool)), this, SLOT(onPhantomPowerClicked(bool)));
    }
    else
    {
        AddSeparate(horizontalLayout, true);
        AddSeparate(horizontalLayout);
        AddSeparate(horizontalLayout, true);
    }

    AddSeparate(horizontalLayout);

    QPushButton *phase = new QPushButton(frame);
    phase->setProperty("qssType", "2");
    phase->setProperty("channel", rowIdx);
    phase->setFont(_fontBord);
    phase->setCheckable(true);
    phase->setFlat(false);
    horizontalLayout->addWidget(phase);
    _buttonsList[rowIdx]->phase =phase;
    connect(phase, SIGNAL(clicked(bool)), this, SLOT(onPhaseClicked(bool)));

    AddSeparate(horizontalLayout);

    QPushButton *inMeter = new QPushButton(frame);
    inMeter->setProperty("qssType", "3");
    inMeter->setProperty("channel", rowIdx);
    inMeter->setFont(_fontBord);
    inMeter->setText(QString("M"));
    horizontalLayout->addWidget(inMeter);
    connect(inMeter, SIGNAL(clicked()), this, SLOT(onInMeterClicked()));

    AddSeparate(horizontalLayout);

    QPushButton *inEq = new QPushButton(frame);
    inEq->setProperty("qssType", "3");
    inEq->setProperty("channel", rowIdx);
    inEq->setFont(_fontBord);
    inEq->setText(QString("E"));
    inEq->setCheckable(true);
    inEq->setAutoExclusive(true);
    _btnGroup->addButton(inEq);
    horizontalLayout->addWidget(inEq);
    connect(inEq, SIGNAL(clicked(bool)), this, SLOT(onInEqClicked(bool)));

    AddSeparate(horizontalLayout);

    QPushButton *inDyn = new QPushButton(frame);
    inDyn->setProperty("qssType", "3");
    inDyn->setProperty("channel", rowIdx);
    inDyn->setFont(_fontBord);
    inDyn->setText(QString("C"));    
    inDyn->setCheckable(true);
    inDyn->setAutoExclusive(true);
    _btnGroup->addButton(inDyn);
    horizontalLayout->addWidget(inDyn);
    connect(inDyn, SIGNAL(clicked(bool)), this, SLOT(onInDynClicked(bool)));

    AddSeparate(horizontalLayout);

//    QPushButton *inDelay = new QPushButton(frame);
//    inDelay->setProperty("qssType", "1");
//    inDelay->setProperty("channel", rowIdx);
//    inDelay->setFont(_fontNormal);
//    inDelay->setText(QString("0"));
//    horizontalLayout->addWidget(inDelay);
//    _buttonsList[rowIdx]->inDelay =inDelay;
//    connect(inDelay, SIGNAL(clicked()), this, SLOT(onInDelayClicked()));

//    AddSeparate(horizontalLayout);

//    QPushButton *inLevel = new QPushButton(frame);
//    inLevel->setProperty("qssType", "1");
//    inLevel->setProperty("channel", rowIdx);
//    inLevel->setFont(_fontNormal);
//    inLevel->setText(QString("0"));
//    horizontalLayout->addWidget(inLevel);
//    _buttonsList[rowIdx]->inLevle =inLevel;
//    connect(inLevel, SIGNAL(clicked()), this, SLOT(onInLevelClick()));

//    AddSeparate(horizontalLayout);

    QLabel *lblMi = new QLabel(frame);
    lblMi->setGeometry(0,0,20,36);
    lblMi->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    QString moText = QString("MI %1").arg(rowIdx+1).rightJustified(5, QLatin1Char(' '));
    lblMi->setText(moText);
    horizontalLayout->addWidget(lblMi);
    setInputLayoutStretch(horizontalLayout);

    gridLayoutPara->addWidget(frame, rowIdx + 1, 0, 1, 1);
}

void DevSetting_TRN::initParaHeaderUi(QGridLayout *gridLayoutPara)
{
    //input
    QFrame *frameIn = new QFrame();
    frameIn->setFrameShape(QFrame::StyledPanel);
    frameIn->setFrameShadow(QFrame::Raised);
    frameIn->setStyleSheet(QLatin1String("QLabel{ qproperty-alignment: AlignHCenter;}"));
    QHBoxLayout *horizontalLayoutIn = new QHBoxLayout(frameIn);
    horizontalLayoutIn->setSpacing(0);
    horizontalLayoutIn->setMargin(0);

    QLabel *lblInput = new QLabel(frameIn);
    lblInput->setText(tr("    INPUT"));
    horizontalLayoutIn->addWidget(lblInput);

    horizontalLayoutIn->addWidget(new QLabel(QString("  "), frameIn));

    QLabel *gainIn = new QLabel(frameIn);
    gainIn->setText(tr("Gain"));
    horizontalLayoutIn->addWidget(gainIn);

    horizontalLayoutIn->addWidget(new QLabel(QString("  "), frameIn));

    QLabel *phan48vIn = new QLabel(frameIn);
    phan48vIn->setText(QString("48V"));
    horizontalLayoutIn->addWidget(phan48vIn);

    horizontalLayoutIn->addWidget(new QLabel(QString("  "), frameIn));

    QLabel *phaseIn = new QLabel(frameIn);
    phaseIn->setText(tr("PHAN"));
    horizontalLayoutIn->addWidget(phaseIn);

    horizontalLayoutIn->addWidget(new QLabel(QString("  "), frameIn));

    QLabel *meterIn = new QLabel(frameIn);
    meterIn->setText(tr("METER"));
    horizontalLayoutIn->addWidget(meterIn);

    horizontalLayoutIn->addWidget(new QLabel(QString("  "), frameIn));

    QLabel *eqIn = new QLabel(frameIn);
    eqIn->setText(tr("EQ"));
    horizontalLayoutIn->addWidget(eqIn);

    horizontalLayoutIn->addWidget(new QLabel(QString("  "), frameIn));

    QLabel *dynIn = new QLabel(frameIn);
    dynIn->setText(tr("DYN"));
    horizontalLayoutIn->addWidget(dynIn);

    horizontalLayoutIn->addWidget(new QLabel(QString("  "), frameIn));

//    QLabel *deleyIn = new QLabel(frameIn);
//    deleyIn->setText(QString("DELEY"));
//    horizontalLayoutIn->addWidget(deleyIn);

//    horizontalLayoutIn->addWidget(new QLabel(QString("  "), frameIn));

//    QLabel *levelIn = new QLabel(frameIn);
//    levelIn->setText(QString("LEVEL"));
//    horizontalLayoutIn->addWidget(levelIn);

//    horizontalLayoutIn->addWidget(new QLabel(QString("  "), frameIn));
    horizontalLayoutIn->addWidget(new QLabel(QString("     "), frameIn));
    setInputLayoutStretch(horizontalLayoutIn);
    gridLayoutPara->addWidget(frameIn, 0, 0, 1, 1);

    //output
    QFrame *frameOut = new QFrame();
    frameOut->setFrameShape(QFrame::StyledPanel);
    frameOut->setFrameShadow(QFrame::Raised);
    frameOut->setStyleSheet(QLatin1String("QLabel{ qproperty-alignment: AlignHCenter;}"));

    QHBoxLayout *horizontalLayoutOut = new QHBoxLayout(frameOut);
    horizontalLayoutOut->setSpacing(0);
    horizontalLayoutOut->setMargin(0);

    horizontalLayoutOut->addWidget(new QLabel(QString("     "), frameOut));
    horizontalLayoutOut->addWidget(new QLabel(QString("  "), frameOut));


    QLabel *levelOut = new QLabel(frameOut);
    levelOut->setText(tr("LEVEL"));
    horizontalLayoutOut->addWidget(levelOut);

    horizontalLayoutOut->addWidget(new QLabel(QString("  "), frameOut));

    QLabel *deleyOut = new QLabel(frameOut);
    deleyOut->setText(tr("DELEY"));
    horizontalLayoutOut->addWidget(deleyOut);

    horizontalLayoutOut->addWidget(new QLabel(QString("  "), frameOut));

    QLabel *eqOut = new QLabel(frameOut);
    eqOut->setText(tr("EQ"));
    horizontalLayoutOut->addWidget(eqOut);

    horizontalLayoutOut->addWidget(new QLabel(QString("  "), frameOut));

    QLabel *geq9Out = new QLabel(frameOut);
    geq9Out->setText(tr("SPLIT"));
    horizontalLayoutOut->addWidget(geq9Out);

    horizontalLayoutOut->addWidget(new QLabel(QString("  "), frameOut));

    QLabel *dynOut = new QLabel(frameOut);
    dynOut->setText(tr("DYN"));
    horizontalLayoutOut->addWidget(dynOut);

    horizontalLayoutOut->addWidget(new QLabel(QString("  "), frameOut));

    QLabel *meterOut = new QLabel(frameOut);
    meterOut->setText(tr("METER"));
    horizontalLayoutOut->addWidget(meterOut);

    horizontalLayoutOut->addWidget(new QLabel(QString("  "), frameOut));

    QLabel *lblOutput = new QLabel(frameOut);
    lblOutput->setText(tr("OUTPUT    "));
    horizontalLayoutOut->addWidget(lblOutput);

    setOutputLayoutStretch(horizontalLayoutOut);
    gridLayoutPara->addWidget(frameOut, 0, 2, 1, 1);
}

void DevSetting_TRN::setOutputLayoutStretch(QHBoxLayout *horizontalLayout)
{
    horizontalLayout->setStretch(0,0);
    horizontalLayout->setStretch(1,1);
    horizontalLayout->setStretch(2,1);
    horizontalLayout->setStretch(3,1);
    horizontalLayout->setStretch(4,1);
    horizontalLayout->setStretch(5,1);
    horizontalLayout->setStretch(6,1);
    horizontalLayout->setStretch(7,1);
    horizontalLayout->setStretch(8,1);
    horizontalLayout->setStretch(9,1);
    horizontalLayout->setStretch(10,1);
    horizontalLayout->setStretch(11,1);
    horizontalLayout->setStretch(10,1);
    horizontalLayout->setStretch(11,1);
    horizontalLayout->setStretch(12,1);
    horizontalLayout->setStretch(13,1);
    horizontalLayout->setStretch(14,0);
}

void DevSetting_TRN::initOutputUi(QGridLayout *gridLayoutPara, QString outputText, int rowIdx)
{
    QFrame *frame = new QFrame();
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);

    QHBoxLayout *horizontalLayout = new QHBoxLayout(frame);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setMargin(0);

    QLabel *lblMo = new QLabel(frame);
    lblMo->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    QString moText = QString("MO %1").arg(rowIdx+1).leftJustified(5, QLatin1Char(' '));
    lblMo->setText(moText);
    horizontalLayout->addWidget(lblMo);

    AddSeparate(horizontalLayout);

    QPushButton *outLevel = new QPushButton(frame);
    outLevel->setProperty("qssType", "1");
    outLevel->setProperty("channel", rowIdx);
    outLevel->setFont(_fontNormal);
    outLevel->setText(QString("0"));
    horizontalLayout->addWidget(outLevel);
    _buttonsList[rowIdx]->outLevel =outLevel;
    connect(outLevel, SIGNAL(clicked()), this, SLOT(onOutLevelClick()));

    AddSeparate(horizontalLayout);

    QPushButton *outDelay = new QPushButton(frame);
    outDelay->setProperty("qssType", "1");
    outDelay->setProperty("channel", rowIdx);
    outDelay->setFont(_fontNormal);
    outDelay->setText(QString("0"));
    horizontalLayout->addWidget(outDelay);
    _buttonsList[rowIdx]->outDelay =outDelay;
    connect(outDelay, SIGNAL(clicked()), this, SLOT(onOutDelayClicked()));

    AddSeparate(horizontalLayout);

    QPushButton *outEq = new QPushButton(frame);
    outEq->setProperty("qssType", "3");
    outEq->setProperty("channel", rowIdx);
    outEq->setFont(_fontBord);
    outEq->setText(QString("E"));
    outEq->setCheckable(true);
    outEq->setAutoExclusive(true);
    _btnGroup->addButton(outEq);
    horizontalLayout->addWidget(outEq);
    connect(outEq, SIGNAL(clicked(bool)), this, SLOT(onOutEq15BClicked(bool)));

    AddSeparate(horizontalLayout);

    QPushButton *split = new QPushButton(frame);
    split->setProperty("qssType", "3");
    split->setProperty("channel", rowIdx);
    split->setFont(_fontBord);
    split->setText(QString("S"));
    split->setCheckable(true);
    split->setAutoExclusive(true);
    _btnGroup->addButton(split);
    horizontalLayout->addWidget(split);
    connect(split, SIGNAL(clicked(bool)), this, SLOT(onOutSplitClicked(bool)));

    AddSeparate(horizontalLayout);

    QPushButton *outDyn = new QPushButton(frame);
    outDyn->setProperty("qssType", "3");
    outDyn->setProperty("channel", rowIdx);
    outDyn->setFont(_fontBord);
    outDyn->setText(QString("C"));
    outDyn->setCheckable(true);
    outDyn->setAutoExclusive(true);
    _btnGroup->addButton(outDyn);
    horizontalLayout->addWidget(outDyn);
    connect(outDyn, SIGNAL(clicked(bool)), this, SLOT(onOutDynClicked(bool)));

    AddSeparate(horizontalLayout);

    QPushButton *outMeter = new QPushButton(frame);
    outMeter->setProperty("qssType", "3");
    outMeter->setProperty("channel", rowIdx);
    outMeter->setFont(_fontBord);
    outMeter->setText(QString("M"));
    horizontalLayout->addWidget(outMeter);
    connect(outMeter, SIGNAL(clicked()), this, SLOT(onOutMeterClicked()));

    AddSeparate(horizontalLayout);

    QLabel *lblOutput = new QLabel(frame);
    lblOutput->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    lblOutput->setText(outputText);
    horizontalLayout->addWidget(lblOutput);

    setOutputLayoutStretch(horizontalLayout);
    gridLayoutPara->addWidget(frame, rowIdx + 1, 2, 1, 1);
}

void DevSetting_TRN::AddSeparate(QHBoxLayout *horizontalLayout, bool isRepeat)
{
    QLabel *separate = new QLabel();
    if(isRepeat) separate->setText(QStringLiteral("---"));
    else separate->setText(QStringLiteral("-->"));
    separate->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    horizontalLayout->addWidget(separate);
}

