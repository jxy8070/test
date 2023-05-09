#include "devsetting_wmic_and_miccenterdn_fangtu.h"
#include "ui_devsetting_wmic_and_miccenterdn_fangtu.h"

#include "../trn/trn_dynwidget.h"
#include "../trn/eqview_allband.h"
#include "customWidget/overwriteitem.h"
#include "../../deviceItem/deviceitem_mic_quad.h"

DevSetting_WMIC_And_MicCenterDN_FangTu::DevSetting_WMIC_And_MicCenterDN_FangTu(DeviceItem_Mic_Quad *devItem, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSetting_WMIC_And_MicCenterDN_FangTu)
{
    ui->setupUi(this);
    _devItem = devItem;

    connectSlots();
    if(_devItem->getType() == Dev_Type_FionTu_WMIC_FangTu) {
        ui->label_26->setHidden(true);
        ui->Meter_Mic_In->setHidden(true);
        ui->Btn_Phase_Mic_In->setHidden(true);
        ui->Gain_Mic_In->setHidden(true);
        ui->Btn_Mute_Mic_In->setHidden(true);
        ui->Btn_Dyn_Mic_In->setHidden(true);
        ui->Btn_LowCut_Mic_In->setHidden(true);
        ui->Btn_Eq_Mic_In->setHidden(true);
    }
}

DevSetting_WMIC_And_MicCenterDN_FangTu::~DevSetting_WMIC_And_MicCenterDN_FangTu()
{
    delete ui;
}

void DevSetting_WMIC_And_MicCenterDN_FangTu::connectSlots()
{
    //phase
    connect(ui->Btn_Phase_AIn_1, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onPhaseBtnSlots);
    connect(ui->Btn_Phase_AIn_2, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onPhaseBtnSlots);
    connect(ui->Btn_Phase_AIn_3, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onPhaseBtnSlots);
    connect(ui->Btn_Phase_AIn_4, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onPhaseBtnSlots);

    connect(ui->Btn_Phase_NOut_1, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onPhaseBtnSlots);
    connect(ui->Btn_Phase_NOut_2, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onPhaseBtnSlots);
    connect(ui->Btn_Phase_NOut_3, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onPhaseBtnSlots);
    connect(ui->Btn_Phase_NOut_4, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onPhaseBtnSlots);

    //Gain
    connect(ui->Gain_AIn_1, QOverload<double>::of(&ConfigGainWidget::valueChanged), this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onGainBtnSlots);
    connect(ui->Gain_AIn_2, QOverload<double>::of(&ConfigGainWidget::valueChanged), this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onGainBtnSlots);
    connect(ui->Gain_AIn_3, QOverload<double>::of(&ConfigGainWidget::valueChanged), this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onGainBtnSlots);
    connect(ui->Gain_AIn_4, QOverload<double>::of(&ConfigGainWidget::valueChanged), this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onGainBtnSlots);

    connect(ui->Gain_NOut_1, QOverload<double>::of(&ConfigGainWidget::valueChanged), this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onGainBtnSlots);
    connect(ui->Gain_NOut_2, QOverload<double>::of(&ConfigGainWidget::valueChanged), this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onGainBtnSlots);
    connect(ui->Gain_NOut_3, QOverload<double>::of(&ConfigGainWidget::valueChanged), this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onGainBtnSlots);
    connect(ui->Gain_NOut_4, QOverload<double>::of(&ConfigGainWidget::valueChanged), this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onGainBtnSlots);

    //Mute
    connect(ui->Btn_Mute_AIn_1, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onMuteBtnSlots);
    connect(ui->Btn_Mute_AIn_2, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onMuteBtnSlots);
    connect(ui->Btn_Mute_AIn_3, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onMuteBtnSlots);
    connect(ui->Btn_Mute_AIn_4, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onMuteBtnSlots);

    connect(ui->Btn_Mute_NOut_1, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onMuteBtnSlots);
    connect(ui->Btn_Mute_NOut_2, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onMuteBtnSlots);
    connect(ui->Btn_Mute_NOut_3, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onMuteBtnSlots);
    connect(ui->Btn_Mute_NOut_4, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onMuteBtnSlots);

    //Dyn
    connect(ui->Btn_Dyn_AIn_1, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onDynBtnSlots);
    connect(ui->Btn_Dyn_AIn_2, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onDynBtnSlots);
    connect(ui->Btn_Dyn_AIn_3, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onDynBtnSlots);
    connect(ui->Btn_Dyn_AIn_4, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onDynBtnSlots);

    //Low Cut
    connect(ui->Btn_LowCut_AIn_1, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onLowCutBtnSlots);
    connect(ui->Btn_LowCut_AIn_2, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onLowCutBtnSlots);
    connect(ui->Btn_LowCut_AIn_3, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onLowCutBtnSlots);
    connect(ui->Btn_LowCut_AIn_4, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onLowCutBtnSlots);

    //Eq
    connect(ui->Btn_Eq_AIn_1, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onEqBtnSlots);
    connect(ui->Btn_Eq_AIn_2, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onEqBtnSlots);
    connect(ui->Btn_Eq_AIn_3, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onEqBtnSlots);
    connect(ui->Btn_Eq_AIn_4, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onEqBtnSlots);

    //Mixer
    connect(ui->Btn_Mixer, &QPushButton::clicked, this, &DevSetting_WMIC_And_MicCenterDN_FangTu::onMixerSlot);
}

void DevSetting_WMIC_And_MicCenterDN_FangTu::disconnectSlots()
{
    //Phase
    ui->Btn_Phase_AIn_1->disconnect(this);
    ui->Btn_Phase_AIn_2->disconnect(this);
    ui->Btn_Phase_AIn_3->disconnect(this);
    ui->Btn_Phase_AIn_4->disconnect(this);

    ui->Btn_Phase_NOut_1->disconnect(this);
    ui->Btn_Phase_NOut_2->disconnect(this);
    ui->Btn_Phase_NOut_3->disconnect(this);
    ui->Btn_Phase_NOut_4->disconnect(this);

    //Gain
    ui->Gain_AIn_1->disconnect(this);
    ui->Gain_AIn_2->disconnect(this);
    ui->Gain_AIn_3->disconnect(this);
    ui->Gain_AIn_4->disconnect(this);

    ui->Gain_NOut_1->disconnect(this);
    ui->Gain_NOut_2->disconnect(this);
    ui->Gain_NOut_3->disconnect(this);
    ui->Gain_NOut_4->disconnect(this);

    //Mute
    ui->Btn_Mute_AIn_1->disconnect(this);
    ui->Btn_Mute_AIn_2->disconnect(this);
    ui->Btn_Mute_AIn_3->disconnect(this);
    ui->Btn_Mute_AIn_4->disconnect(this);

    ui->Btn_Mute_NOut_1->disconnect(this);
    ui->Btn_Mute_NOut_2->disconnect(this);
    ui->Btn_Mute_NOut_3->disconnect(this);
    ui->Btn_Mute_NOut_4->disconnect(this);

    //dyn
    ui->Btn_Dyn_AIn_1->disconnect(this);
    ui->Btn_Dyn_AIn_2->disconnect(this);
    ui->Btn_Dyn_AIn_3->disconnect(this);
    ui->Btn_Dyn_AIn_4->disconnect(this);

    //Low Cut
    ui->Btn_LowCut_AIn_1->disconnect(this);
    ui->Btn_LowCut_AIn_2->disconnect(this);
    ui->Btn_LowCut_AIn_3->disconnect(this);
    ui->Btn_LowCut_AIn_4->disconnect(this);

    //Eq
    ui->Btn_Eq_AIn_1->disconnect(this);
    ui->Btn_Eq_AIn_2->disconnect(this);
    ui->Btn_Eq_AIn_3->disconnect(this);
    ui->Btn_Eq_AIn_4->disconnect(this);
}

void DevSetting_WMIC_And_MicCenterDN_FangTu::onPhaseBtnSlots(bool checked)
{

}

void DevSetting_WMIC_And_MicCenterDN_FangTu::onGainBtnSlots(double value)
{

}

void DevSetting_WMIC_And_MicCenterDN_FangTu::onMuteBtnSlots(bool checked)
{

}

void DevSetting_WMIC_And_MicCenterDN_FangTu::onDynBtnSlots()
{
    int channel;
    if(sender() == ui->Btn_Dyn_AIn_1) {
        channel = 0;
    }else if(sender() == ui->Btn_Dyn_AIn_2) {
        channel = 1;
    }else if(sender() == ui->Btn_Dyn_AIn_3) {
        channel = 2;
    }else if(sender() == ui->Btn_Dyn_AIn_4) {
        channel = 3;
    }

    DynData dynData;
    memset(&dynData, 0, sizeof(DynData));

    DevSettingDlg dlg;
    Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
//    dynWidget->setDisabled(_devAmp->isFactoryMode());//是否禁用小部件
    dynWidget->showWidget(false, channel, QString(), dynData);

    dlg.resize(dynWidget->size());
    connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
    dlg.exec();
}

void DevSetting_WMIC_And_MicCenterDN_FangTu::onLowCutBtnSlots()
{
    if(sender() == ui->Btn_LowCut_AIn_1) {

    }else if(sender() == ui->Btn_LowCut_AIn_2) {

    }else if(sender() == ui->Btn_LowCut_AIn_3) {

    }else if(sender() == ui->Btn_LowCut_AIn_4) {

    }
}

void DevSetting_WMIC_And_MicCenterDN_FangTu::onEqBtnSlots()
{
    int channel;
    if(sender() == ui->Btn_Eq_AIn_1) {
        channel = 0;
    }else if(sender() == ui->Btn_Eq_AIn_2) {
        channel = 1;
    }else if(sender() == ui->Btn_Eq_AIn_3) {
        channel = 2;
    }else if(sender() == ui->Btn_Eq_AIn_4) {
        channel = 3;
    }
    EqData15B eq15B;
    memset(&eq15B, 0, sizeof(EqData15B));



    DevSettingDlg dlg;
    EqView_AllBand *eqView = new EqView_AllBand(15, &dlg);
    eqView->showWidget(channel, QString(), eq15B.bypass, eq15B.eq);
    dlg.resize(eqView->size());
    dlg.exec();
}

void DevSetting_WMIC_And_MicCenterDN_FangTu::onMixerSlot()
{

}
