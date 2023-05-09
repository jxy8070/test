#include "cfgsettinginput.h"
#include "ui_cfgsettingport.h"
#include "configItem/configiteminput.h"
#include "ui_CfgSettingPort_FangTu.h"
#include "device/AES70device.h"


CfgSettingInput::CfgSettingInput(ConfigItemInput *inputItem, QWidget *parent) :
    QWidget(parent)
{
    _inputItem = inputItem;

    //根据设备平台初始化前置增益
    PlatformType platform = Utils::getPlatformType(inputItem->getDevType());
    if(platform == Platform_DSP){
        _preAmpGains << "0" << "+6" << "+12" << "+18" << "+24" << "+30" << "+36" << "+42";
    }
    else{
        _preAmpGains << "0" << "+3" << "+6" << "+9" << "+12" << "+15" << "+18" << "+21" << "+24"
                  << "+27" << "+30" << "+33" << "+36" << "+39" << "+42" << "+45" << "+48" << "+51";
    }
#if Logo_FangTu
    _ui_FangTu = new Ui::CfgSettingPort_FangTu;
    _ui_FangTu->setupUi(this);
    _levelFade = new QTimer(this);
    connect(_levelFade, &QTimer::timeout, this, &CfgSettingInput::onLevelFadeSlot);
    initUI_FangTu();
#else
    _ui = new Ui::CfgSettingPort;
    _ui->setupUi(this);
    initUI();
#endif


    for(int i = 0; i < _muteList.count(); i++) {
        _phaseList.at(i)->setProperty("form", "Invert");
        _muteList.at(i)->setProperty("form", "Mute");
    }
    updateUiData();
}

CfgSettingInput::~CfgSettingInput()
{
    if(_ui_FangTu)
        delete _ui_FangTu;
    if(_ui)
        delete _ui;
}

void CfgSettingInput::setVuDisp(unsigned char *d)
{
//    d[0] = 80; d[1] = 60; d[2] = 40; d[3] = 20;
    for (int i = 0; i < _inputItem->getMaxPort(); ++i) {
        _vuList.at(i)->setLCurValue(d[i] - 100);
        _vuLabelList.at(i)->setText(QString::number(d[i] - 100));
        if(_vuList.at(i)->getMaxValue() == 0 && d[i] - 100 > 0) {
            _vuLabelList.at(i)->setText(0);
            _vuLabelList.at(i)->setStyleSheet("background:#FF0000;");
        }
        else {
            _vuLabelList.at(i)->setText(QString::number(d[i] - 100));
            _vuLabelList.at(i)->setStyleSheet("");
        }
#if Logo_FangTu
        if(d[i] <= 0) {
            _vuList.at(i)->setRCurValue(d[i] - 100);
            _vuLabel_R_list.at(i)->setText(QString::number(d[i] - 100));
        }else {
            if(_muteList.at(i)->isChecked()) {
                _vuList.at(i)->setRCurValue(_fadeValue.at(i));
                _vuLabel_R_list.at(i)->setText(QString::number(_fadeValue.at(i)));
            }else {
                _vuList.at(i)->setRCurValue(d[i] - 100 + qRound(_gainList.at(i)->value()));
                _vuLabel_R_list.at(i)->setText(QString::number(d[i] - 100 + qRound(_gainList.at(i)->value())));
            }
        }
#endif
    }
}


void CfgSettingInput::onPhantomClicked(bool enable)
{
    for (int i = 0; i < _inputItem->getMaxPort(); ++i) {
        if (sender() == _phantomList.at(i)) {
            _inputItem->setPhantom(i, enable);
            return;
        }
    }
}

void CfgSettingInput::onPregainValueChanged(int value)
{
    for (int i = 0; i < _inputItem->getMaxPort(); ++i) {
        if (sender() == _pregainList.at(i)) {
            _inputItem->setPreampGain(i, value);
            return;
        }
    }
}

void CfgSettingInput::onGainValueChanged(double value)
{
    for (int i = 0; i < _inputItem->getMaxPort(); ++i) {
        if (sender() == _gainList.at(i)) {
            _inputItem->setGain(i, value);
            return;
        }
    }
}

void CfgSettingInput::onMuteClicked(bool enable)
{
    for (int i = 0; i < _inputItem->getMaxPort(); ++i) {
        if (sender() == _muteList.at(i)) {
            _inputItem->setMute(i, enable);
#if Logo_FangTu
            if(_muteList.at(i)->isChecked()) {
                _fadeValue[i] = _vuList.at(i)->getRCurValue();
                _fadeValue[i]--;
                _levelFade->start(6000/100);
            }else {
                _levelFade->stop();
            }
#endif
            return;
        }
    }
}

void CfgSettingInput::onPhaseClicked(bool enable)
{
    for (int i = 0; i < _inputItem->getMaxPort(); ++i) {
        if (sender() == _phaseList.at(i)) {
            _inputItem->setInvert(i, enable);
            return;
        }
    }
}

void CfgSettingInput::onUserLabelChanged()
{
    for (int i = 0; i < _textEditList.count(); ++i) {
        if (sender() == _textEditList.at(i)) {
            _inputItem->setUserLabel(i, _textEditList.at(i)->toPlainText());
            return;
        }
    }
}

void CfgSettingInput::onLevelFadeSlot()
{
    int flagA = 0, flagB = 0;

    for(int i = 0; i < _inputItem->getMaxPort(); i++) {
        if(_muteList.at(i)->isChecked()) {
            flagA++;
            if(_fadeValue.at(i) > -100) _fadeValue[i]--;
            if(_fadeValue.at(i) == -100) flagB++;
        }
    }
//    qDebug() << __FUNCTION__ << __LINE__ << flagA << flagB;
    if(flagA == flagB) {
        _levelFade->stop();
    }
}


void CfgSettingInput::updateUiData()
{
    for (int i = 0; i < _inputItem->getMaxPort(); ++i) {
        disconnect(_gainList.at(i), SIGNAL(valueChanged(double)), this, SLOT(onGainValueChanged(double)));
        disconnect(_muteList.at(i), SIGNAL(clicked(bool)), this, SLOT(onMuteClicked(bool)));
        disconnect(_phaseList.at(i), SIGNAL(clicked(bool)), this, SLOT(onPhaseClicked(bool)));
        disconnect(_textEditList.at(i), SIGNAL(textChanged()), this, SLOT(onUserLabelChanged()));

        QString chName = Utils::getCfgPortChannelName(_inputItem->getPortType(), _inputItem->getLogicChannel(i));
        _channelList.at(i)->setText(chName);
        _gainList.at(i)->setValue(_inputItem->getGain(i));
        _muteList.at(i)->setChecked(_inputItem->isMute(i));
        _phaseList.at(i)->setChecked(_inputItem->isInvert(i));
        _textEditList.at(i)->setText(_inputItem->getUserLabel(i));
        _textEditList.at(i)->setAlignment(Qt::AlignCenter);//每次setText、setPlainText之后需要重新设置居中

        connect(_gainList.at(i), SIGNAL(valueChanged(double)), this, SLOT(onGainValueChanged(double)));
        connect(_muteList.at(i), SIGNAL(clicked(bool)), this, SLOT(onMuteClicked(bool)));
        connect(_phaseList.at(i), SIGNAL(clicked(bool)), this, SLOT(onPhaseClicked(bool)));
        connect(_textEditList.at(i), SIGNAL(textChanged()), this, SLOT(onUserLabelChanged()));
        if(_inputItem->getPortType() == PortType_MONAURAL) {
            disconnect(_pregainList.at(i), SIGNAL(currentIndexChanged(int)), this, SLOT(onPregainValueChanged(int)));
            disconnect(_phantomList.at(i), SIGNAL(clicked(bool)), this, SLOT(onPhantomClicked(bool)));
            _pregainList.at(i)->setCurrentIndex(_inputItem->getPreampGain(i));
            _phantomList.at(i)->setChecked(_inputItem->getPhantom(i));

            connect(_pregainList.at(i), SIGNAL(currentIndexChanged(int)), this, SLOT(onPregainValueChanged(int)));
            connect(_phantomList.at(i), SIGNAL(clicked(bool)), this, SLOT(onPhantomClicked(bool)));
        }
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
            _fadeValue.append(_vuList.at(i)->getRCurValue());
    }
}

void CfgSettingInput::initUI()
{
    _channelWidgetList.append(_ui->widget);
    _channelWidgetList.append(_ui->widget_3);
    _channelWidgetList.append(_ui->widget_4);
    _channelWidgetList.append(_ui->widget_5);
    _channelWidgetList.append(_ui->widget_6);
    _channelWidgetList.append(_ui->widget_7);
    _channelWidgetList.append(_ui->widget_8);
    _channelWidgetList.append(_ui->widget_9);
    _channelWidgetList.append(_ui->widget_10);
    _channelWidgetList.append(_ui->widget_11);
    _channelWidgetList.append(_ui->widget_12);
    _channelWidgetList.append(_ui->widget_13);
    _channelWidgetList.append(_ui->widget_14);
    _channelWidgetList.append(_ui->widget_15);
    _channelWidgetList.append(_ui->widget_16);
    _channelWidgetList.append(_ui->widget_17);

    _vuList.append(_ui->Disp_Level_1);
    _vuList.append(_ui->Disp_Level_2);
    _vuList.append(_ui->Disp_Level_3);
    _vuList.append(_ui->Disp_Level_4);
    _vuList.append(_ui->Disp_Level_5);
    _vuList.append(_ui->Disp_Level_6);
    _vuList.append(_ui->Disp_Level_7);
    _vuList.append(_ui->Disp_Level_8);
    _vuList.append(_ui->Disp_Level_9);
    _vuList.append(_ui->Disp_Level_10);
    _vuList.append(_ui->Disp_Level_11);
    _vuList.append(_ui->Disp_Level_12);
    _vuList.append(_ui->Disp_Level_13);
    _vuList.append(_ui->Disp_Level_14);
    _vuList.append(_ui->Disp_Level_15);
    _vuList.append(_ui->Disp_Level_16);

    _vuLabelList.append(_ui->lbl_Level_1);
    _vuLabelList.append(_ui->lbl_Level_2);
    _vuLabelList.append(_ui->lbl_Level_3);
    _vuLabelList.append(_ui->lbl_Level_4);
    _vuLabelList.append(_ui->lbl_Level_5);
    _vuLabelList.append(_ui->lbl_Level_6);
    _vuLabelList.append(_ui->lbl_Level_7);
    _vuLabelList.append(_ui->lbl_Level_8);
    _vuLabelList.append(_ui->lbl_Level_9);
    _vuLabelList.append(_ui->lbl_Level_10);
    _vuLabelList.append(_ui->lbl_Level_11);
    _vuLabelList.append(_ui->lbl_Level_12);
    _vuLabelList.append(_ui->lbl_Level_13);
    _vuLabelList.append(_ui->lbl_Level_14);
    _vuLabelList.append(_ui->lbl_Level_15);
    _vuLabelList.append(_ui->lbl_Level_16);

    _channelList.append(_ui->lbl_ChName_1);
    _channelList.append(_ui->lbl_ChName_2);
    _channelList.append(_ui->lbl_ChName_3);
    _channelList.append(_ui->lbl_ChName_4);
    _channelList.append(_ui->lbl_ChName_5);
    _channelList.append(_ui->lbl_ChName_6);
    _channelList.append(_ui->lbl_ChName_7);
    _channelList.append(_ui->lbl_ChName_8);
    _channelList.append(_ui->lbl_ChName_9);
    _channelList.append(_ui->lbl_ChName_10);
    _channelList.append(_ui->lbl_ChName_11);
    _channelList.append(_ui->lbl_ChName_12);
    _channelList.append(_ui->lbl_ChName_13);
    _channelList.append(_ui->lbl_ChName_14);
    _channelList.append(_ui->lbl_ChName_15);
    _channelList.append(_ui->lbl_ChName_16);

    _pregainList.append(_ui->comboBox_PreampGain_1);
    _pregainList.append(_ui->comboBox_PreampGain_2);
    _pregainList.append(_ui->comboBox_PreampGain_3);
    _pregainList.append(_ui->comboBox_PreampGain_4);
    _pregainList.append(_ui->comboBox_PreampGain_5);
    _pregainList.append(_ui->comboBox_PreampGain_6);
    _pregainList.append(_ui->comboBox_PreampGain_7);
    _pregainList.append(_ui->comboBox_PreampGain_8);
    _pregainList.append(_ui->comboBox_PreampGain_9);
    _pregainList.append(_ui->comboBox_PreampGain_10);
    _pregainList.append(_ui->comboBox_PreampGain_11);
    _pregainList.append(_ui->comboBox_PreampGain_12);
    _pregainList.append(_ui->comboBox_PreampGain_13);
    _pregainList.append(_ui->comboBox_PreampGain_14);
    _pregainList.append(_ui->comboBox_PreampGain_15);
    _pregainList.append(_ui->comboBox_PreampGain_16);
    for(QComboBox * preAmp : _pregainList)
    {
        preAmp->addItems(_preAmpGains);
    }
    _phantomList.append(_ui->btn_Phantom_1);
    _phantomList.append(_ui->btn_Phantom_2);
    _phantomList.append(_ui->btn_Phantom_3);
    _phantomList.append(_ui->btn_Phantom_4);
    _phantomList.append(_ui->btn_Phantom_5);
    _phantomList.append(_ui->btn_Phantom_6);
    _phantomList.append(_ui->btn_Phantom_7);
    _phantomList.append(_ui->btn_Phantom_8);
    _phantomList.append(_ui->btn_Phantom_9);
    _phantomList.append(_ui->btn_Phantom_10);
    _phantomList.append(_ui->btn_Phantom_11);
    _phantomList.append(_ui->btn_Phantom_12);
    _phantomList.append(_ui->btn_Phantom_13);
    _phantomList.append(_ui->btn_Phantom_14);
    _phantomList.append(_ui->btn_Phantom_15);
    _phantomList.append(_ui->btn_Phantom_16);

    _gainList.append(_ui->spinBoxGain_1);
    _gainList.append(_ui->spinBoxGain_2);
    _gainList.append(_ui->spinBoxGain_3);
    _gainList.append(_ui->spinBoxGain_4);
    _gainList.append(_ui->spinBoxGain_5);
    _gainList.append(_ui->spinBoxGain_6);
    _gainList.append(_ui->spinBoxGain_7);
    _gainList.append(_ui->spinBoxGain_8);
    _gainList.append(_ui->spinBoxGain_9);
    _gainList.append(_ui->spinBoxGain_10);
    _gainList.append(_ui->spinBoxGain_11);
    _gainList.append(_ui->spinBoxGain_12);
    _gainList.append(_ui->spinBoxGain_13);
    _gainList.append(_ui->spinBoxGain_14);
    _gainList.append(_ui->spinBoxGain_15);
    _gainList.append(_ui->spinBoxGain_16);

    _muteList.append(_ui->btnMute_1);
    _muteList.append(_ui->btnMute_2);
    _muteList.append(_ui->btnMute_3);
    _muteList.append(_ui->btnMute_4);
    _muteList.append(_ui->btnMute_5);
    _muteList.append(_ui->btnMute_6);
    _muteList.append(_ui->btnMute_7);
    _muteList.append(_ui->btnMute_8);
    _muteList.append(_ui->btnMute_9);
    _muteList.append(_ui->btnMute_10);
    _muteList.append(_ui->btnMute_11);
    _muteList.append(_ui->btnMute_12);
    _muteList.append(_ui->btnMute_13);
    _muteList.append(_ui->btnMute_14);
    _muteList.append(_ui->btnMute_15);
    _muteList.append(_ui->btnMute_16);

    _phaseList.append(_ui->btnInvert_1);
    _phaseList.append(_ui->btnInvert_2);
    _phaseList.append(_ui->btnInvert_3);
    _phaseList.append(_ui->btnInvert_4);
    _phaseList.append(_ui->btnInvert_5);
    _phaseList.append(_ui->btnInvert_6);
    _phaseList.append(_ui->btnInvert_7);
    _phaseList.append(_ui->btnInvert_8);
    _phaseList.append(_ui->btnInvert_9);
    _phaseList.append(_ui->btnInvert_10);
    _phaseList.append(_ui->btnInvert_11);
    _phaseList.append(_ui->btnInvert_12);
    _phaseList.append(_ui->btnInvert_13);
    _phaseList.append(_ui->btnInvert_14);
    _phaseList.append(_ui->btnInvert_15);
    _phaseList.append(_ui->btnInvert_16);

    _textEditList.append(_ui->textEdit_1);
    _textEditList.append(_ui->textEdit_2);
    _textEditList.append(_ui->textEdit_3);
    _textEditList.append(_ui->textEdit_4);
    _textEditList.append(_ui->textEdit_5);
    _textEditList.append(_ui->textEdit_6);
    _textEditList.append(_ui->textEdit_7);
    _textEditList.append(_ui->textEdit_8);
    _textEditList.append(_ui->textEdit_9);
    _textEditList.append(_ui->textEdit_10);
    _textEditList.append(_ui->textEdit_11);
    _textEditList.append(_ui->textEdit_12);
    _textEditList.append(_ui->textEdit_13);
    _textEditList.append(_ui->textEdit_14);
    _textEditList.append(_ui->textEdit_15);
    _textEditList.append(_ui->textEdit_16);

    _ui->scrollArea->setStyleSheet(QLatin1String("QScrollArea {background-color:transparent;}"));
    _ui->scrollAreaWidgetContents->setStyleSheet(QLatin1String("QWidget#scrollAreaWidgetContents {background-color:transparent;}"));

    for (int i = 0; i < _inputItem->getMaxPort(); ++i) {
        _gainList.at(i)->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
        connect(_gainList.at(i), SIGNAL(valueChanged(double)), this, SLOT(onGainValueChanged(double)));
        connect(_muteList.at(i), SIGNAL(clicked(bool)), this, SLOT(onMuteClicked(bool)));
        connect(_phaseList.at(i), SIGNAL(clicked(bool)), this, SLOT(onPhaseClicked(bool)));
        connect(_textEditList.at(i), SIGNAL(textChanged()), this, SLOT(onUserLabelChanged()));
    }

    if (_inputItem->getPortType() != PortType_MONAURAL) {
        setFixedSize(4+99+89*_inputItem->getMaxPort(), 398);
        _ui->label_PreampGain->setVisible(false);
        _ui->label_Phantom->setVisible(false);
        for (int i = 0; i < _inputItem->getMaxPort(); ++i) {
            _pregainList.at(i)->setVisible(false);
            _phantomList.at(i)->setVisible(false);
        }
    }
    else {
        setFixedSize(13+99+89*_inputItem->getMaxPort(), 454);
        for (int i = 0; i < _inputItem->getMaxPort(); ++i) {
            _pregainList.at(i)->setCurrentIndex(_inputItem->getPreampGain(i));
            _phantomList.at(i)->setChecked(_inputItem->getPhantom(i));
            _vuList.at(i)->setMaxValue(0);
            connect(_pregainList.at(i), SIGNAL(currentIndexChanged(int)), this, SLOT(onPregainValueChanged(int)));
            connect(_phantomList.at(i), SIGNAL(clicked(bool)), this, SLOT(onPhantomClicked(bool)));
        }
    }

    for (int i = _inputItem->getMaxPort(); i < 16; ++i) {
        _channelWidgetList.at(i)->setVisible(false);
    }
}

void CfgSettingInput::initUI_FangTu()
{
    _channelWidgetList.append(_ui_FangTu->widget_1);
    _channelWidgetList.append(_ui_FangTu->widget_2);
    _channelWidgetList.append(_ui_FangTu->widget_3);
    _channelWidgetList.append(_ui_FangTu->widget_4);
    _channelWidgetList.append(_ui_FangTu->widget_5);
    _channelWidgetList.append(_ui_FangTu->widget_6);
    _channelWidgetList.append(_ui_FangTu->widget_7);
    _channelWidgetList.append(_ui_FangTu->widget_8);
    _channelWidgetList.append(_ui_FangTu->widget_9);
    _channelWidgetList.append(_ui_FangTu->widget_10);
    _channelWidgetList.append(_ui_FangTu->widget_11);
    _channelWidgetList.append(_ui_FangTu->widget_12);
    _channelWidgetList.append(_ui_FangTu->widget_13);
    _channelWidgetList.append(_ui_FangTu->widget_14);
    _channelWidgetList.append(_ui_FangTu->widget_15);
    _channelWidgetList.append(_ui_FangTu->widget_16);

    _channelWidgetSetList.append(_ui_FangTu->widget_set_1);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_2);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_3);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_4);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_5);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_6);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_7);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_8);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_9);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_10);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_11);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_12);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_13);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_14);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_15);
    _channelWidgetSetList.append(_ui_FangTu->widget_set_16);

    _vuList.append(_ui_FangTu->Disp_Level_1);
    _vuList.append(_ui_FangTu->Disp_Level_2);
    _vuList.append(_ui_FangTu->Disp_Level_3);
    _vuList.append(_ui_FangTu->Disp_Level_4);
    _vuList.append(_ui_FangTu->Disp_Level_5);
    _vuList.append(_ui_FangTu->Disp_Level_6);
    _vuList.append(_ui_FangTu->Disp_Level_7);
    _vuList.append(_ui_FangTu->Disp_Level_8);
    _vuList.append(_ui_FangTu->Disp_Level_9);
    _vuList.append(_ui_FangTu->Disp_Level_10);
    _vuList.append(_ui_FangTu->Disp_Level_11);
    _vuList.append(_ui_FangTu->Disp_Level_12);
    _vuList.append(_ui_FangTu->Disp_Level_13);
    _vuList.append(_ui_FangTu->Disp_Level_14);
    _vuList.append(_ui_FangTu->Disp_Level_15);
    _vuList.append(_ui_FangTu->Disp_Level_16);

    _vuLabelList.append(_ui_FangTu->lbl_L_Level_1);
    _vuLabelList.append(_ui_FangTu->lbl_L_Level_2);
    _vuLabelList.append(_ui_FangTu->lbl_L_Level_3);
    _vuLabelList.append(_ui_FangTu->lbl_L_Level_4);
    _vuLabelList.append(_ui_FangTu->lbl_Level_5);
    _vuLabelList.append(_ui_FangTu->lbl_Level_6);
    _vuLabelList.append(_ui_FangTu->lbl_Level_7);
    _vuLabelList.append(_ui_FangTu->lbl_Level_8);
    _vuLabelList.append(_ui_FangTu->lbl_Level_9);
    _vuLabelList.append(_ui_FangTu->lbl_Level_10);
    _vuLabelList.append(_ui_FangTu->lbl_Level_11);
    _vuLabelList.append(_ui_FangTu->lbl_Level_12);
    _vuLabelList.append(_ui_FangTu->lbl_Level_13);
    _vuLabelList.append(_ui_FangTu->lbl_Level_14);
    _vuLabelList.append(_ui_FangTu->lbl_Level_15);
    _vuLabelList.append(_ui_FangTu->lbl_Level_16);

    _vuLabel_R_list.append(_ui_FangTu->lbl_R_Level_1);
    _vuLabel_R_list.append(_ui_FangTu->lbl_R_Level_2);
    _vuLabel_R_list.append(_ui_FangTu->lbl_R_Level_3);
    _vuLabel_R_list.append(_ui_FangTu->lbl_R_Level_4);

    _channelList.append(_ui_FangTu->lbl_ChName_1);
    _channelList.append(_ui_FangTu->lbl_ChName_2);
    _channelList.append(_ui_FangTu->lbl_ChName_3);
    _channelList.append(_ui_FangTu->lbl_ChName_4);
    _channelList.append(_ui_FangTu->lbl_ChName_5);
    _channelList.append(_ui_FangTu->lbl_ChName_6);
    _channelList.append(_ui_FangTu->lbl_ChName_7);
    _channelList.append(_ui_FangTu->lbl_ChName_8);
    _channelList.append(_ui_FangTu->lbl_ChName_9);
    _channelList.append(_ui_FangTu->lbl_ChName_10);
    _channelList.append(_ui_FangTu->lbl_ChName_11);
    _channelList.append(_ui_FangTu->lbl_ChName_12);
    _channelList.append(_ui_FangTu->lbl_ChName_13);
    _channelList.append(_ui_FangTu->lbl_ChName_14);
    _channelList.append(_ui_FangTu->lbl_ChName_15);
    _channelList.append(_ui_FangTu->lbl_ChName_16);

    _pregainList.append(_ui_FangTu->comboBox_PreampGain_1);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_2);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_3);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_4);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_5);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_6);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_7);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_8);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_9);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_10);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_11);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_12);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_13);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_14);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_15);
    _pregainList.append(_ui_FangTu->comboBox_PreampGain_16);

    _phantomList.append(_ui_FangTu->btn_Phantom_1);
    _phantomList.append(_ui_FangTu->btn_Phantom_2);
    _phantomList.append(_ui_FangTu->btn_Phantom_3);
    _phantomList.append(_ui_FangTu->btn_Phantom_4);
    _phantomList.append(_ui_FangTu->btn_Phantom_5);
    _phantomList.append(_ui_FangTu->btn_Phantom_6);
    _phantomList.append(_ui_FangTu->btn_Phantom_7);
    _phantomList.append(_ui_FangTu->btn_Phantom_8);
    _phantomList.append(_ui_FangTu->btn_Phantom_9);
    _phantomList.append(_ui_FangTu->btn_Phantom_10);
    _phantomList.append(_ui_FangTu->btn_Phantom_11);
    _phantomList.append(_ui_FangTu->btn_Phantom_12);
    _phantomList.append(_ui_FangTu->btn_Phantom_13);
    _phantomList.append(_ui_FangTu->btn_Phantom_14);
    _phantomList.append(_ui_FangTu->btn_Phantom_15);
    _phantomList.append(_ui_FangTu->btn_Phantom_16);

    _gainList.append(_ui_FangTu->spinBoxGain_1);
    _gainList.append(_ui_FangTu->spinBoxGain_2);
    _gainList.append(_ui_FangTu->spinBoxGain_3);
    _gainList.append(_ui_FangTu->spinBoxGain_4);
    _gainList.append(_ui_FangTu->spinBoxGain_5);
    _gainList.append(_ui_FangTu->spinBoxGain_6);
    _gainList.append(_ui_FangTu->spinBoxGain_7);
    _gainList.append(_ui_FangTu->spinBoxGain_8);
    _gainList.append(_ui_FangTu->spinBoxGain_9);
    _gainList.append(_ui_FangTu->spinBoxGain_10);
    _gainList.append(_ui_FangTu->spinBoxGain_11);
    _gainList.append(_ui_FangTu->spinBoxGain_12);
    _gainList.append(_ui_FangTu->spinBoxGain_13);
    _gainList.append(_ui_FangTu->spinBoxGain_14);
    _gainList.append(_ui_FangTu->spinBoxGain_15);
    _gainList.append(_ui_FangTu->spinBoxGain_16);

    _muteList.append(_ui_FangTu->btnMute_1);
    _muteList.append(_ui_FangTu->btnMute_2);
    _muteList.append(_ui_FangTu->btnMute_3);
    _muteList.append(_ui_FangTu->btnMute_4);
    _muteList.append(_ui_FangTu->btnMute_5);
    _muteList.append(_ui_FangTu->btnMute_6);
    _muteList.append(_ui_FangTu->btnMute_7);
    _muteList.append(_ui_FangTu->btnMute_8);
    _muteList.append(_ui_FangTu->btnMute_9);
    _muteList.append(_ui_FangTu->btnMute_10);
    _muteList.append(_ui_FangTu->btnMute_11);
    _muteList.append(_ui_FangTu->btnMute_12);
    _muteList.append(_ui_FangTu->btnMute_13);
    _muteList.append(_ui_FangTu->btnMute_14);
    _muteList.append(_ui_FangTu->btnMute_15);
    _muteList.append(_ui_FangTu->btnMute_16);

    _phaseList.append(_ui_FangTu->btnInvert_1);
    _phaseList.append(_ui_FangTu->btnInvert_2);
    _phaseList.append(_ui_FangTu->btnInvert_3);
    _phaseList.append(_ui_FangTu->btnInvert_4);
    _phaseList.append(_ui_FangTu->btnInvert_5);
    _phaseList.append(_ui_FangTu->btnInvert_6);
    _phaseList.append(_ui_FangTu->btnInvert_7);
    _phaseList.append(_ui_FangTu->btnInvert_8);
    _phaseList.append(_ui_FangTu->btnInvert_9);
    _phaseList.append(_ui_FangTu->btnInvert_10);
    _phaseList.append(_ui_FangTu->btnInvert_11);
    _phaseList.append(_ui_FangTu->btnInvert_12);
    _phaseList.append(_ui_FangTu->btnInvert_13);
    _phaseList.append(_ui_FangTu->btnInvert_14);
    _phaseList.append(_ui_FangTu->btnInvert_15);
    _phaseList.append(_ui_FangTu->btnInvert_16);

    _textEditList.append(_ui_FangTu->textEdit_1);
    _textEditList.append(_ui_FangTu->textEdit_2);
    _textEditList.append(_ui_FangTu->textEdit_3);
    _textEditList.append(_ui_FangTu->textEdit_4);
    _textEditList.append(_ui_FangTu->textEdit_5);
    _textEditList.append(_ui_FangTu->textEdit_6);
    _textEditList.append(_ui_FangTu->textEdit_7);
    _textEditList.append(_ui_FangTu->textEdit_8);
    _textEditList.append(_ui_FangTu->textEdit_9);
    _textEditList.append(_ui_FangTu->textEdit_10);
    _textEditList.append(_ui_FangTu->textEdit_11);
    _textEditList.append(_ui_FangTu->textEdit_12);
    _textEditList.append(_ui_FangTu->textEdit_13);
    _textEditList.append(_ui_FangTu->textEdit_14);
    _textEditList.append(_ui_FangTu->textEdit_15);
    _textEditList.append(_ui_FangTu->textEdit_16);



    _ui_FangTu->scrollArea->setStyleSheet(QLatin1String("QScrollArea {border: 0px solid rgb(255, 255, 255); background-color:transparent;}"));
    _ui_FangTu->scrollAreaWidgetContents->setStyleSheet(QLatin1String("QWidget#scrollAreaWidgetContents {background-color:transparent; padding: 0px;}"));

    for (int i = 0; i < _inputItem->getMaxPort(); ++i) {
        _gainList.at(i)->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
        connect(_gainList.at(i), SIGNAL(valueChanged(double)), this, SLOT(onGainValueChanged(double)));
        connect(_muteList.at(i), SIGNAL(clicked(bool)), this, SLOT(onMuteClicked(bool)));
        connect(_phaseList.at(i), SIGNAL(clicked(bool)), this, SLOT(onPhaseClicked(bool)));
        connect(_textEditList.at(i), SIGNAL(textChanged()), this, SLOT(onUserLabelChanged()));
    }

    if (_inputItem->getPortType() != PortType_MONAURAL) {
//        setFixedSize(4+99+89*_inputItem->getMaxPort(), 398);
        setFixedSize(108+209*_inputItem->getMaxPort()/4, 417);
        _ui_FangTu->label_PreampGain->setVisible(false);
        _ui_FangTu->label_Phantom->setVisible(false);
        for (int i = 0; i < _inputItem->getMaxPort(); ++i) {
            _pregainList.at(i)->setVisible(false);
            _phantomList.at(i)->setVisible(false);
        }
    }
    else {
//        setFixedSize(13+99+89*_inputItem->getMaxPort(), 454);
        _ui_FangTu->label_PreampGain->setFixedWidth(96);
        setFixedSize(158+209*_inputItem->getMaxPort()/4, 466);
        for (int i = 0; i < _inputItem->getMaxPort(); ++i) {
            _pregainList.at(i)->setCurrentIndex(_inputItem->getPreampGain(i));
            _phantomList.at(i)->setChecked(_inputItem->getPhantom(i));

            connect(_pregainList.at(i), SIGNAL(currentIndexChanged(int)), this, SLOT(onPregainValueChanged(int)));
            connect(_phantomList.at(i), SIGNAL(clicked(bool)), this, SLOT(onPhantomClicked(bool)));
        }
    }

    for (int i = _inputItem->getMaxPort(); i < 16; ++i) {
        _channelWidgetList.at(i)->setVisible(false);
        _channelWidgetSetList.at(i)->setVisible(false);
    }

    _ui_FangTu->groupBox_2->setVisible(false);
    _ui_FangTu->groupBox_3->setVisible(false);
    _ui_FangTu->groupBox_4->setVisible(false);
    _ui_FangTu->groupBox_set_2->setVisible(false);
    _ui_FangTu->groupBox_set_3->setVisible(false);
    _ui_FangTu->groupBox_set_4->setVisible(false);
}


