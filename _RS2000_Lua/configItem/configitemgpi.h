#ifndef CONFIGITEMGPI_H
#define CONFIGITEMGPI_H
#include "configitem.h"

typedef struct{
    __OPACK32_T3S0S1S2(
    OPACKU,chNo,8,            //0,1
    OPACKU,gpiNo,8,           //0~8,0无效GPI,既不受GPI控制，1~8GPI编号
    OPACKU,hw_gpiDecMode,8);  //0外设直接传输硬件GPI切换的通道，通道编号由chNo决定，gpiNo无效传0;
                              //1外设只通知模块路由控制引脚编号(gpiNo)，模块切换由引脚状态寄存器(FC_HARDWARE_GPIS)决定 ,chNo无效传0;
}IG_PARACTL_GPI;

class CfgSettingGPI;
class ConfigItemGPI : public ConfigItem
{
public:
    ConfigItemGPI(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemGPI();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    quint32 getGpiIdx(void) { return _body.gpiNo -1; }
    void setGpiIdx(quint32 idx);
private:
    IG_PARACTL_GPI _body;
    CfgSettingGPI *_setting;
};

#endif // CONFIGITEMGPI_H
