#include "stable.h"
#include "cfgsettingfilte.h"
#include "ui_cfgsettingfilte.h"
#include "configItem/configitemfilte.h"
#include "customWidget/eqgraph.h"

CfgSettingFilte::CfgSettingFilte(ConfigItemFilte *cfgItem,  QWidget *parent)
    : QWidget(parent),
      _ui(new Ui::CfgSettingFilte)
{
    _cfgItemFilte = cfgItem;
    _ui->setupUi(this);
    initUi();

//    connect(_ui->spinBoxFrequency, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged()));
    connect(_ui->spinBoxFrequency, SIGNAL(valueChanged(int)), this, SLOT(onFrequencyChanged()));
    connect(_ui->eqGraph, SIGNAL(valueChanged(int,int,int,int)), this, SLOT(onValueChanged(int,int,int,int)));
    connect(_ui->btnBypass, SIGNAL(clicked(bool)), this, SLOT(onSetBypass(bool)));
    connect(_ui->btn_mute, SIGNAL(clicked(bool)), this, SLOT(onSetMute(bool)));
    connect(_ui->btn_invert, SIGNAL(clicked(bool)), this, SLOT(onSetInvert(bool)));
//    connect(_ui->spinBoxGain, SIGNAL(valueChanged(double)), this, SLOT(onGainChanged()));
    connect(_ui->spinBoxGain, SIGNAL(valueChanged(double)), this, SLOT(onGainChanged()));
    connect(_ui->comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterTypeChanged(int)));
    
    connect(_ui->doubleSpinBox_bandwidth, SIGNAL(valueChanged(double)), this, SLOT(Bandwidth_ValueChanged(double)));
}


void CfgSettingFilte::initUi()
{
    switch(_cfgItemFilte->getCfgType()) {
    case CFG_TYPE_FILTE_LP:   //低通
    case CFG_TYPE_FILTE_HP:   //高通
        _ui->labelGain->hide();
        _ui->spinBoxGain->hide();
        _ui->label_Bandwidth->hide();
        _ui->doubleSpinBox_bandwidth->hide();
        _ui->btn_mute->hide();
        _ui->btn_invert->hide();
        _ui->label_2->hide();
        _ui->label_3->hide();
        break;
    case CFG_TYPE_FILTE_LS:   //低通斜坡
    case CFG_TYPE_FILTE_HS:   //高通斜坡
        _ui->labelType->hide();
        _ui->comboBoxType->hide();
        _ui->label_Bandwidth->hide();
        _ui->doubleSpinBox_bandwidth->hide();
        _ui->btn_mute->hide();
        _ui->btn_invert->hide();
        _ui->label_2->hide();
        _ui->label_3->hide();
        break;
    case CFG_TYPE_FILTER_BP:
        _ui->labelType->hide();
        _ui->comboBoxType->hide();
        _ui->labelGain->hide();
        _ui->spinBoxGain->hide();
        _ui->btn_mute->hide();
        _ui->btn_invert->hide();
        _ui->label_2->hide();
        _ui->label_3->hide();
        break;
    case CFG_TYPE_FILTER_AP:
        _ui->labelType->hide();
        _ui->comboBoxType->hide();
        _ui->labelGain->hide();
        _ui->spinBoxGain->hide();
        _ui->label_Bandwidth->setText(tr("Q-Factor"));
        break;
    case FCT_FAMILY_MASK:
    case CFG_TYPE_2_GROUP_TYPE:
    case CFG_TYPE_DELAY_5:
    case CFG_TYPE_DELAY_10:
    case CFG_TYPE_DELAY_20:
    case CFG_TYPE_DELAY_50:
    case CFG_TYPE_DELAY_100:
    case CFG_TYPE_DELAY_500:
    case CFG_TYPE_DELAY_1000:
    case CFG_TYPE_HPDELY_1:
    case CFG_TYPE_HPDELY_5:
    case CFG_TYPE_HPDELY_10:
    case CFG_TYPE_HPDELY_20:
    case CFG_TYPE_HPDELY_50:
    case CFG_TYPE_ROUTER_4:
    case CFG_TYPE_ROUTER_8:
    case CFG_TYPE_ROUTER_16:
    case CFG_TYPE_ROUTER_32:
    case CFG_TYPE_ROUTER_48:
    case CFG_TYPE_ROUTER_64:
    case CFG_TYPE_ROUTER_96:
    case CFG_TYPE_ROUTER_128:
    case CFG_TYPE_ROUTER_192:
    case CFG_TYPE_ROUTER_256:
    case CFG_TYPE_MIXER_4X1:
    case CFG_TYPE_MIXER_4X2:
    case CFG_TYPE_MIXER_4X4:
    case CFG_TYPE_MIXER_8X1:
    case CFG_TYPE_MIXER_8X2:
    case CFG_TYPE_MIXER_8X4:
    case CFG_TYPE_MIXER_8X8:
    case CFG_TYPE_MIXER_16X16:
    case CFG_TYPE_MIXER_32X32:
    case CFG_TYPE_MIXER_64X64:
    case CFG_TYPE_DYN_AUTO:
    case CFG_TYPE_DYN_COMP:
    case CFG_TYPE_DYN_LIMIT:
    case CFG_TYPE_DYN_NOISE:
    case CFG_TYPE_DYN_EXPAND:
    case CFG_TYPE_DYN_PEAK:
    case CFG_TYPE_EQ_1:
    case CFG_TYPE_EQ_3:
    case CFG_TYPE_EQ_5:
    case CFG_TYPE_EQ_7:
    case CFG_TYPE_EQ_10:
    case CFG_TYPE_EQ_16:
    case CFG_TYPE_EQ_1X3:
    case CFG_TYPE_EQ_2X3:
    case CFG_TYPE_EQ_3X3:
    case CFG_TYPE_SPLIT_2:
    case CFG_TYPE_SPLIT_3:
    case CFG_TYPE_SPLIT_4:
    case CFG_TYPE_SIG_SINE:
    case CFG_TYPE_SIG_NOISE:
    case CFG_TYPE_SIG_PINKNOISE:
    case CFG_TYPE_METER_1:
    case CFG_TYPE_METER_2:
    case CFG_TYPE_METER_4:
    case CFG_TYPE_METER_8:
    case CFG_TYPE_INPUT_1:
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
    case CFG_TYPE_OUTPUT_1:
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
    case CFG_TYPE_GPI:
    case CFG_TYPE_FREQSHIFT:
    case CFG_TYPE_FREQSHIFT_PLUS:
    case CFG_TYPE_AUTOMIX_GS_4:
    case CFG_TYPE_AUTOMIX_GS_8:
    case CFG_TYPE_AUTOMIX_GS_16:
    case CFG_TYPE_AUTOMIX_GS_32:
    case CFG_TYPE_AUTOMIXNGAT_4:
    case CFG_TYPE_AUTOMIXNGAT_8:
    case CFG_TYPE_AUTOMIXNGAT_16:
    case CFG_TYPE_AUTOMIXNGAT_32:
    case CFG_TYPE_AUTOMIXNGRT_4:
    case CFG_TYPE_AUTOMIXNGRT_8:
    case CFG_TYPE_AUTOMIXNGRT_16:
    case CFG_TYPE_AUTOMIXNGRT_32:
    case CFG_TYPE_AEC_2_2:
    case CFG_TYPE_SOFTFIR_MONO256:
    case CFG_TYPE_SOFTFIR_MONO512:
    case CFG_TYPE_SOFTFIR_MONO1024:
    case CFG_TYPE_SOFTFIR_STEREO256:
    case CFG_TYPE_SOFTFIR_STEREO512:
    case CFG_TYPE_SOFTFIR_STEREO1024:
    case CFG_TYPE_SOFTFIR_4R256:
    case CFG_TYPE_SOFTFIR_4R512:
    case CFG_TYPE_SOFTFIR_4R1024:
    case CFG_TYPE_NFC1_8NF:
    case CFG_TYPE_NFC1_16NF:
    case CFG_TYPE_NFC1_24NF:
    case CFG_TYPE_NFC1_32NF:
    case CFG_TYPE_PDUCKER_4:
    case CFG_TYPE_PDUCKER_8:
    case CFG_TYPE_PDUCKER_16:
    case CFG_TYPE_PDUCKER_32:
    case CFG_TYPE_GAINRAMP_1:
    case CFG_TYPE_GAINRAMP_2:
    case CFG_TYPE_GAINRAMP_4:
    case CFG_TYPE_GAINRAMP_8:
    case CFG_TYPE_GAINRAMP_16:
    case CFG_TYPE_GAINRAMP_32:
    case CFG_TYPE_GAIN_1:
    case CFG_TYPE_GAIN_2:
    case CFG_TYPE_GAIN_4:
    case CFG_TYPE_GAIN_8:
    case CFG_TYPE_GAIN_16:
    case CFG_TYPE_GAIN_32:
    case CFG_TYPE_CROSSFADER_2X1:
    case CFG_TYPE_CROSSFADER_4X2:
    case CFG_TYPE_CROSSFADER_8X4:
    case CFG_TYPE_CROSSFADER_16X8:
    case CFG_TYPE_CROSSFADER_32X16:
    case CFG_TYPE_SOFTAEC_8K64:
    case CFG_TYPE_SOFTAEC_8K96:
    case CFG_TYPE_SOFTAEC_8K128:
    case CFG_TYPE_SOFTAEC_8K160:
    case CFG_TYPE_SOFTAEC_8K192:
    case CFG_TYPE_SOFTAEC_8K256:
    case CFG_TYPE_SOFTAEC_16K64:
    case CFG_TYPE_SOFTAEC_16K96:
    case CFG_TYPE_SOFTAEC_16K128:
    case CFG_TYPE_SOFTAEC_16K160:
    case CFG_TYPE_SOFTAEC_16K192:
    case CFG_TYPE_SOFTAEC_16K256:
    case CFG_TYPE_SOFTAEC_IK_8K64:
    case CFG_TYPE_SOFTAEC_IK_8K96:
    case CFG_TYPE_SOFTAEC_IK_8K128:
    case CFG_TYPE_SOFTAEC_IK_8K160:
    case CFG_TYPE_SOFTAEC_IK_8K192:
    case CFG_TYPE_SOFTAEC_IK_8K256:
    case CFG_TYPE_SOFTAEC_IK_16K64:
    case CFG_TYPE_SOFTAEC_IK_16K96:
    case CFG_TYPE_SOFTAEC_IK_16K128:
    case CFG_TYPE_SOFTAEC_IK_16K160:
    case CFG_TYPE_SOFTAEC_IK_16K192:
    case CFG_TYPE_SOFTAEC_IK_16K256:
    case CFG_TYPE_UCI:
    case CFG_TYPE_CTRL_LUA:
    case CFG_TYPE_CTRL_GPI:
    case CFG_TYPE_CTRL_GPO:
    case CFG_TYPE_CTRL_AD:
    case CFG_TYPE_CTRL_RS232:
    case CFG_TYPE_CTRL_RS485:
    case CFG_TYPE_AUDIO_PLAYER1:
    case CFG_TYPE_AUDIO_PLAYER2:
    case CFG_TYPE_AUDIO_PLAYER4:
    case CFG_TYPE_AUDIO_PLAYER8:
    case CFG_TYPE_AUDIO_PLAYER16:
    case CFG_TYPE_AUDIO_RECORD1:
    case CFG_TYPE_AUDIO_RECORD2:
    case CFG_TYPE_AUDIO_RECORD4:
    case CFG_TYPE_VOICETUBE_1:
    case CFG_TYPE_VOICETUBE_2:
    case CFG_TYPE_SNAPSHOT:
    case CFG_TYPE_SNAPSHOT_BANK:
    case CFG_TYPE_2:
    case CFG_TYPE_3:
    case CFG_TYPE_4:
    case CFG_TYPE_5:
    case CFG_TYPE_SCENE:
    case CFG_TYPE_NETWORKDEVICE:
        break;
    }

    _ui->comboBoxType->addItem(QLatin1String("L-R 12"));
    _ui->comboBoxType->addItem(QLatin1String("L-R 24"));
    _ui->comboBoxType->addItem(QLatin1String("L-R 36"));
    _ui->comboBoxType->addItem(QLatin1String("L-R 48"));
    _ui->comboBoxType->addItem(QLatin1String("BW 6"));
    _ui->comboBoxType->addItem(QLatin1String("BW 12"));
    _ui->comboBoxType->addItem(QLatin1String("BW 18"));
    _ui->comboBoxType->addItem(QLatin1String("BW 24"));
    _ui->comboBoxType->addItem(QLatin1String("BW 30"));
    _ui->comboBoxType->addItem(QLatin1String("BW 36"));
    _ui->comboBoxType->addItem(QLatin1String("BW 42"));
    _ui->comboBoxType->addItem(QLatin1String("BW 48"));
    _ui->comboBoxType->setCurrentIndex(_cfgItemFilte->getFilterType());

    /* 主要为初始化类型 */
    _ui->eqGraph->initParaList(_cfgItemFilte->getEqData()->freq, _cfgItemFilte->getEqData()->gain, _cfgItemFilte->getEqData()->q, _cfgItemFilte->getEqType());
    _ui->eqGraph->initParaListEnd();

    _ui->eqGraph->setBypass(_cfgItemFilte->getBypass());
    _ui->eqGraph->setPassType(_cfgItemFilte->getFilterType());
    _ui->btnBypass->setChecked(_cfgItemFilte->getBypass());
    _ui->btn_mute->setChecked(_cfgItemFilte->getMute());
    _ui->btn_invert->setChecked(_cfgItemFilte->getInvert());

    _ui->spinBoxFrequency->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _ui->spinBoxFrequency->setValue(_cfgItemFilte->getEqData()->freq);

    _ui->spinBoxGain->setRangeParam(-30, 9, StepMode_Line, ParamType_Gain);
    _ui->spinBoxGain->setValue(_cfgItemFilte->getEqData()->gain / 100.);
    _ui->btnBypass->setProperty("form", "Bypass");
    _ui->btn_mute->setProperty("form", "Mute");
    _ui->btn_invert->setProperty("form", "Invert");
    _ui->doubleSpinBox_bandwidth->setRangeParam(0.01, 3, StepMode_Log10, ParamType_Q);
    _ui->doubleSpinBox_bandwidth->setDecimalsNum(2);
    _ui->doubleSpinBox_bandwidth->setValue(_cfgItemFilte->getEqData()->q / 100.0);
}

void CfgSettingFilte::updateUiData()
{
    _ui->comboBoxType->setCurrentIndex(_cfgItemFilte->getFilterType());

    _ui->eqGraph->setBypass(_cfgItemFilte->getBypass());
    _ui->eqGraph->setPassType(_cfgItemFilte->getFilterType());
    _ui->btnBypass->setChecked(_cfgItemFilte->getBypass());
    _ui->btn_mute->setChecked(_cfgItemFilte->getMute());
    _ui->btn_invert->setChecked(_cfgItemFilte->getInvert());

    _ui->spinBoxFrequency/*->getDoubleSpinBox()*/->setValue(_cfgItemFilte->getEqData()->freq);
    _ui->spinBoxGain->setValue(_cfgItemFilte->getEqData()->gain / 100.);
    _ui->doubleSpinBox_bandwidth->setValue(_cfgItemFilte->getEqData()->q / 100.0);

    _ui->eqGraph->setValue(0, _cfgItemFilte->getEqData()->freq, _cfgItemFilte->getEqData()->gain, _cfgItemFilte->getEqData()->q);
}

void CfgSettingFilte::onValueChanged(int idx, int freq, int gain, int q)
{
    Q_UNUSED(idx);
    disconnect(_ui->spinBoxFrequency, SIGNAL(valueChanged(int)), this, SLOT(onFrequencyChanged()));
    disconnect(_ui->spinBoxGain, SIGNAL(valueChanged(double)), this, SLOT(onGainChanged()));

//    qDebug() << __FUNCTION__ << __LINE__ << idx << freq << gain << q;
    _ui->spinBoxFrequency->setValue(freq);
    _ui->spinBoxGain->setValue(gain/100.);

    connect(_ui->spinBoxFrequency, SIGNAL(valueChanged(int)), this, SLOT(onFrequencyChanged()));
    connect(_ui->spinBoxGain, SIGNAL(valueChanged(double)), this, SLOT(onGainChanged()));

    _cfgItemFilte->setEqPara(freq, gain, q);
}


void CfgSettingFilte::Bandwidth_ValueChanged(double value)
{
    _cfgItemFilte->setEqPara(_cfgItemFilte->getEqData()->freq, _cfgItemFilte->getEqData()->gain, value*100);
    _ui->eqGraph->setValue(0, _cfgItemFilte->getEqData()->freq, _cfgItemFilte->getEqData()->gain, _cfgItemFilte->getEqData()->q);
}

void CfgSettingFilte::onFrequencyChanged()
{
//    _ui->spinBoxFrequency->setSingleStep(_ui->spinBoxFrequency->getValue()/20.0);

    _cfgItemFilte->setEqPara(_ui->spinBoxFrequency->value(), _cfgItemFilte->getEqData()->gain, _cfgItemFilte->getEqData()->q);
    _ui->eqGraph->setValue(0, _cfgItemFilte->getEqData()->freq, _cfgItemFilte->getEqData()->gain, _cfgItemFilte->getEqData()->q);
}

void CfgSettingFilte::onGainChanged()
{
    _cfgItemFilte->setEqPara(_cfgItemFilte->getEqData()->freq, _ui->spinBoxGain->value()*100, _cfgItemFilte->getEqData()->q);
    _ui->eqGraph->setValue(0, _cfgItemFilte->getEqData()->freq, _cfgItemFilte->getEqData()->gain, _cfgItemFilte->getEqData()->q);
}

void CfgSettingFilte::onFilterTypeChanged(int cur)
{
    _ui->eqGraph->setPassType(cur);
    _cfgItemFilte->setFilterType(cur);
}

void CfgSettingFilte::onSetBypass(bool isBypass)
{
    _ui->eqGraph->setBypass(isBypass);
    _cfgItemFilte->setBypass(isBypass);
}

void CfgSettingFilte::onSetMute(bool is)
{
    _cfgItemFilte->setMute(is);
}

void CfgSettingFilte::onSetInvert(bool is)
{
    _cfgItemFilte->setInvert(is);
    _ui->eqGraph->setSegInvert(0, is);
}
