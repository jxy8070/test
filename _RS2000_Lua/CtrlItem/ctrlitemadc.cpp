#include "ctrlitemadc.h"
#include "ctrlsettingadc.h"

CtrlItemADC::CtrlItemADC(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    _rectHeight = 36;
    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "ADC1", 0x00050002, 0xFFFFFFFF)); //wj1
    pinList.append(new CLuaPinParam_PC(this, name, "ADC2", 0x00050004, 0xFFFFFFFF));
    pinList.append(new CLuaPinParam_PC(this, name, "ADC3", 0x00050006, 0xFFFFFFFF));
    pinList.append(new CLuaPinParam_PC(this, name, "ADC4", 0x00050008, 0xFFFFFFFF));
    initControlPins(pinList, true);
}


QWidget *CtrlItemADC::getSettingForm()
{
    if (_setting == NULL) {
        _setting = new CtrlSettingADC(this);
    }

    return _setting;
}


void CtrlItemADC::setRtnData(unsigned int *d)
{
    quint16 * pStartVU = nullptr;
    DevType devType = getDevType();
    if(devType == Dev_Type_NAP_X86_16Analog_128AES67) {
        FC_RTU_InOutLevel128 *level = (FC_RTU_InOutLevel128 *)d;
        pStartVU = &level->adc[0];
    }
    else if(devType == Dev_Type_NAP_X86_32Analog_256AES67) {
//        FC_RTU_InOutLevel256 *level = (FC_RTU_InOutLevel256 *)d;
//        pStartVU = &level->adc[0];
    }

//    if (_setting) {
//        _setting->setVuDisp(pStartVU);
//    }
}
