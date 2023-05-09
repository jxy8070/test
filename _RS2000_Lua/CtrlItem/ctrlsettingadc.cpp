#include "ctrlitemadc.h"
#include "ctrlsettingadc.h"
#include "ui_ctrlsettingadc.h"
#include "device/device_trn.h"

CtrlSettingADC::CtrlSettingADC(CtrlItemADC* ctrlItem, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CtrlSettingADC)
{
    _ctrlItem = ctrlItem;
    ui->setupUi(this);
    ConfigScene* pScene = qobject_cast<ConfigScene*> (ctrlItem->scene());
    if(pScene != nullptr){
        Device_Trn* trnDev = pScene->getTrnDev();
        if(trnDev) {
            connect(trnDev, SIGNAL(InOutLevelSig(unsigned int *)), this, SLOT(setCtrlInfo(unsigned int *)));
        }
    }
}

CtrlSettingADC::~CtrlSettingADC()
{
    delete ui;
}


void CtrlSettingADC::setCtrlInfo(unsigned int * pData)
{
    quint16 * pAdc = nullptr;
    DevType devType = _ctrlItem->getDevType();
    if(devType == Dev_Type_NAP_X86_16Analog_128AES67) {
        FC_RTU_InOutLevel128 *uvData = (FC_RTU_InOutLevel128 *)pData;
        pAdc = &uvData->adc[0];
    }
    else if(devType == Dev_Type_NAP_X86_32Analog_256AES67) {
//        FC_RTU_InOutLevel256 *level = (FC_RTU_InOutLevel256 *)d;
//        pStartVU = &level->adc[0];
    }

    ui->txtADC1->setText(QString("%1").arg(pAdc[0]/10/100.0));
    ui->txtADC2->setText(QString("%1").arg(pAdc[1]/10/100.0));
    ui->txtADC3->setText(QString("%1").arg(pAdc[2]/10/100.0));
    ui->txtADC4->setText(QString("%1").arg(pAdc[3]/10/100.0));
}
