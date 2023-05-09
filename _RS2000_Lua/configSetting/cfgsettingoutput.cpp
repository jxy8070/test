#include "cfgsettingoutput.h"
#include "ui_cfgsettingport.h"
#include "configItem/configitemoutput.h"
#include "ui_CfgSettingPort_FangTu.h"

CfgSettingOutput::CfgSettingOutput(ConfigItemOutput *outputItem, QWidget *parent) :
    QWidget(parent)
{
    _outputItem = outputItem;
#if Logo_FangTu
    _ui_FangTu = new Ui::CfgSettingPort_FangTu;
    _ui_FangTu->setupUi(this);
    initUI_FangTu();
#else
    _ui = new Ui::CfgSettingPort;
    _ui->setupUi(this);
    initUI();
#endif
    updateUiData();

    for(int i = 0; i < _muteList.count(); i++) {
        _phaseList.at(i)->setProperty("form", "Invert");
        _muteList.at(i)->setProperty("form", "Mute");
    }
}

CfgSettingOutput::~CfgSettingOutput()
{
    if(_ui)
        delete _ui;
    if(_ui_FangTu)
        delete _ui_FangTu;
}

void CfgSettingOutput::setVuDisp(unsigned char *d)
{
    for (int i = 0; i < _outputItem->getMaxPort(); ++i) {
        int vu = d[i];
        _vuList.at(i)->setLCurValue(d[i] - 100);
        if(_vuList.at(i)->getMaxValue() == 0 && d[i] - 100 > 0) {
            _vuLabelList.at(i)->setText(0);
            _vuLabelList.at(i)->setStyleSheet("background:#FF0000;");
        }
        else {
            _vuLabelList.at(i)->setText(QString::number(d[i] - 100));
            _vuLabelList.at(i)->setStyleSheet("");
        }
    }
}

void CfgSettingOutput::onGainValueChanged(double value)
{
    for (int i = 0; i < _outputItem->getMaxPort(); ++i) {
        if (sender() == _gainList.at(i)) {
            _outputItem->setGain(i, value);
            return;
        }
    }
}

void CfgSettingOutput::onMuteClicked(bool enable)
{
    for (int i = 0; i < _outputItem->getMaxPort(); ++i) {
        if (sender() == _muteList.at(i)) {
            _outputItem->setMute(i, enable);
            return;
        }
    }
}

void CfgSettingOutput::onPhaseClicked(bool enable)
{
    for (int i = 0; i < _outputItem->getMaxPort(); ++i) {
        if (sender() == _phaseList.at(i)) {
            _outputItem->setInvert(i, enable);
            return;
        }
    }
}

void CfgSettingOutput::onUserLabelChanged()
{
    for (int i = 0; i < _textEditList.count(); ++i) {
        if (sender() == _textEditList.at(i)) {
            _outputItem->setUserLabel(i, _textEditList.at(i)->toPlainText());
            return;
        }
    }
}

void CfgSettingOutput::initUI()
{
    _ui->label_PreampGain->setVisible(false);
    _ui->label_Phantom->setVisible(false);

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

    _ui->comboBox_PreampGain_1->setVisible(false);
    _ui->comboBox_PreampGain_2->setVisible(false);
    _ui->comboBox_PreampGain_3->setVisible(false);
    _ui->comboBox_PreampGain_4->setVisible(false);
    _ui->comboBox_PreampGain_5->setVisible(false);
    _ui->comboBox_PreampGain_6->setVisible(false);
    _ui->comboBox_PreampGain_7->setVisible(false);
    _ui->comboBox_PreampGain_8->setVisible(false);
    _ui->comboBox_PreampGain_9->setVisible(false);
    _ui->comboBox_PreampGain_10->setVisible(false);
    _ui->comboBox_PreampGain_11->setVisible(false);
    _ui->comboBox_PreampGain_12->setVisible(false);
    _ui->comboBox_PreampGain_13->setVisible(false);
    _ui->comboBox_PreampGain_14->setVisible(false);
    _ui->comboBox_PreampGain_15->setVisible(false);
    _ui->comboBox_PreampGain_16->setVisible(false);

    _ui->btn_Phantom_1->setVisible(false);
    _ui->btn_Phantom_2->setVisible(false);
    _ui->btn_Phantom_3->setVisible(false);
    _ui->btn_Phantom_4->setVisible(false);
    _ui->btn_Phantom_5->setVisible(false);
    _ui->btn_Phantom_6->setVisible(false);
    _ui->btn_Phantom_7->setVisible(false);
    _ui->btn_Phantom_8->setVisible(false);
    _ui->btn_Phantom_9->setVisible(false);
    _ui->btn_Phantom_10->setVisible(false);
    _ui->btn_Phantom_11->setVisible(false);
    _ui->btn_Phantom_12->setVisible(false);
    _ui->btn_Phantom_13->setVisible(false);
    _ui->btn_Phantom_14->setVisible(false);
    _ui->btn_Phantom_15->setVisible(false);
    _ui->btn_Phantom_16->setVisible(false);

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

    for(int i = 0; i < _ui->scrollAreaWidgetContents->layout()->count(); i++) {
        _gainList.at(i)->setFixedSize(40, 40);
        _gainList.at(i)->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    }

    setFixedSize(4+99+89*_outputItem->getMaxPort(), 398);
    for (int i = 0; i < _outputItem->getMaxPort(); ++i) {
        connect(_gainList.at(i), SIGNAL(valueChanged(double)), this, SLOT(onGainValueChanged(double)));
        connect(_muteList.at(i), SIGNAL(clicked(bool)), this, SLOT(onMuteClicked(bool)));
        connect(_phaseList.at(i), SIGNAL(clicked(bool)), this, SLOT(onPhaseClicked(bool)));
        connect(_textEditList.at(i), SIGNAL(textChanged()), this, SLOT(onUserLabelChanged()));
        if (_outputItem->getPortType() == PortType_MONAURAL) {
            _vuList.at(i)->setMaxValue(0);
        }
    }

    for (int i = _outputItem->getMaxPort(); i < 16; ++i) {
        _channelWidgetList.at(i)->setVisible(false);
    }
}

void CfgSettingOutput::initUI_FangTu()
{
    _ui_FangTu->label_PreampGain->setVisible(false);
    _ui_FangTu->label_Phantom->setVisible(false);

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

    //电平
    _ui_FangTu->widget_1->layout()->setAlignment(Qt::AlignLeft);
    _ui_FangTu->horizontalLayout_ch1->setContentsMargins(18, 0, 0, 0);
    _ui_FangTu->horizontalLayout_leveLab_1->setContentsMargins(18, 0, 0, 0);
    _vuList.at(0)->setProperty("SingleTrack", true);
    _vuList.at(0)->setProperty("ScaleLabel", true);
    _ui_FangTu->lbl_R_Level_1->setVisible(false);

    _ui_FangTu->widget_2->layout()->setAlignment(Qt::AlignLeft);
    _ui_FangTu->horizontalLayout_ch2->setContentsMargins(18, 0, 0, 0);
    _ui_FangTu->horizontalLayout_leveLab_2->setContentsMargins(18, 0, 0, 0);
    _vuList.at(1)->setProperty("SingleTrack", true);
    _vuList.at(1)->setProperty("ScaleLabel", true);
    _ui_FangTu->lbl_R_Level_2->setVisible(false);

    _ui_FangTu->widget_3->layout()->setAlignment(Qt::AlignLeft);
    _ui_FangTu->horizontalLayout_ch3->setContentsMargins(18, 0, 0, 0);
    _ui_FangTu->horizontalLayout_leveLab_3->setContentsMargins(18, 0, 0, 0);
    _vuList.at(2)->setProperty("SingleTrack", true);
    _vuList.at(2)->setProperty("ScaleLabel", true);
    _ui_FangTu->lbl_R_Level_3->setVisible(false);

    _ui_FangTu->widget_4->layout()->setAlignment(Qt::AlignLeft);
    _ui_FangTu->horizontalLayout_ch4->setContentsMargins(18, 0, 0, 0);
    _ui_FangTu->horizontalLayout_leveLab_4->setContentsMargins(18, 0, 0, 0);
    _vuList.at(3)->setProperty("SingleTrack", true);
    _vuList.at(3)->setProperty("ScaleLabel", true);
    _ui_FangTu->lbl_R_Level_4->setVisible(false);

    _ui_FangTu->widget_levelScale->setVisible(false);

    _ui_FangTu->comboBox_PreampGain_1->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_2->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_3->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_4->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_5->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_6->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_7->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_8->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_9->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_10->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_11->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_12->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_13->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_14->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_15->setVisible(false);
    _ui_FangTu->comboBox_PreampGain_16->setVisible(false);

    _ui_FangTu->btn_Phantom_1->setVisible(false);
    _ui_FangTu->btn_Phantom_2->setVisible(false);
    _ui_FangTu->btn_Phantom_3->setVisible(false);
    _ui_FangTu->btn_Phantom_4->setVisible(false);
    _ui_FangTu->btn_Phantom_5->setVisible(false);
    _ui_FangTu->btn_Phantom_6->setVisible(false);
    _ui_FangTu->btn_Phantom_7->setVisible(false);
    _ui_FangTu->btn_Phantom_8->setVisible(false);
    _ui_FangTu->btn_Phantom_9->setVisible(false);
    _ui_FangTu->btn_Phantom_10->setVisible(false);
    _ui_FangTu->btn_Phantom_11->setVisible(false);
    _ui_FangTu->btn_Phantom_12->setVisible(false);
    _ui_FangTu->btn_Phantom_13->setVisible(false);
    _ui_FangTu->btn_Phantom_14->setVisible(false);
    _ui_FangTu->btn_Phantom_15->setVisible(false);
    _ui_FangTu->btn_Phantom_16->setVisible(false);

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

    _ui_FangTu->scrollArea->setStyleSheet(QLatin1String("QScrollArea {background-color:transparent;}"));
    _ui_FangTu->scrollAreaWidgetContents->setStyleSheet(QLatin1String("QWidget#scrollAreaWidgetContents {background-color:transparent;}"));

    for(int i = 0; i < _ui_FangTu->scrollAreaWidgetContents->layout()->count(); i++) {
        _gainList.at(i)->setFixedSize(50, 50);
        _gainList.at(i)->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    }

//    setFixedSize(4+99+89*_outputItem->getMaxPort(), 398);
    setFixedSize(107+209*_outputItem->getMaxPort()/4, 416);
    for (int i = 0; i < _outputItem->getMaxPort(); ++i) {
        connect(_gainList.at(i), SIGNAL(valueChanged(double)), this, SLOT(onGainValueChanged(double)));
        connect(_muteList.at(i), SIGNAL(clicked(bool)), this, SLOT(onMuteClicked(bool)));
        connect(_phaseList.at(i), SIGNAL(clicked(bool)), this, SLOT(onPhaseClicked(bool)));
        connect(_textEditList.at(i), SIGNAL(textChanged()), this, SLOT(onUserLabelChanged()));
    }

    for (int i = _outputItem->getMaxPort(); i < 16; ++i) {
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


void CfgSettingOutput::updateUiData()
{
    for (int i = 0; i < _outputItem->getMaxPort(); ++i) {
        disconnect(_gainList.at(i), SIGNAL(valueChanged(double)), this, SLOT(onGainValueChanged(double)));
        disconnect(_muteList.at(i), SIGNAL(clicked(bool)), this, SLOT(onMuteClicked(bool)));
        disconnect(_phaseList.at(i), SIGNAL(clicked(bool)), this, SLOT(onPhaseClicked(bool)));
        disconnect(_textEditList.at(i), SIGNAL(textChanged()), this, SLOT(onUserLabelChanged()));

        QString chName = Utils::getCfgPortChannelName(_outputItem->getPortType(), _outputItem->getLogicChannel(i));
        _channelList.at(i)->setText(chName);
        _gainList.at(i)->setValue(_outputItem->getGain(i));
        _muteList.at(i)->setChecked(_outputItem->isMute(i));
        _phaseList.at(i)->setChecked(_outputItem->isInvert(i));
        _textEditList.at(i)->setText(_outputItem->getUserLabel(i));
        _textEditList.at(i)->setAlignment(Qt::AlignCenter);

        connect(_gainList.at(i), SIGNAL(valueChanged(double)), this, SLOT(onGainValueChanged(double)));
        connect(_muteList.at(i), SIGNAL(clicked(bool)), this, SLOT(onMuteClicked(bool)));
        connect(_phaseList.at(i), SIGNAL(clicked(bool)), this, SLOT(onPhaseClicked(bool)));
        connect(_textEditList.at(i), SIGNAL(textChanged()), this, SLOT(onUserLabelChanged()));
    }
}
