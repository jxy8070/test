#ifndef CTRLITEMADC_H
#define CTRLITEMADC_H

#include "configItem/configitem.h"

class CtrlSettingADC;
class CtrlItemADC : public ConfigItem
{
    Q_OBJECT
public:
    CtrlItemADC(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 100);

    QWidget *getSettingForm() override;
    QByteArray getDetailData() override {return QByteArray();}
    void setDetailData(unsigned int *d) override {return;}
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override {return;}
    DesignItem* clone(DesignItem* toItem) override {return NULL;}
    void loadParaFromSetting(QSettings *ConfigIni) override {return;}
    void saveParaToSetting(QSettings *ConfigIni) override {return;}

private:
    CtrlSettingADC* _setting = nullptr;
};

#endif // CTRLITEMADC_H
