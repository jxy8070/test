#ifndef CTRLITEMRS485_H
#define CTRLITEMRS485_H

#include "configItem/configitem.h"
class CtrlSettingSerial;
class CtrlItemRs485 : public ConfigItem
{
    Q_OBJECT
public:
    CtrlItemRs485(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 100);

    QWidget *getSettingForm() override;
    QByteArray getDetailData() override {return QByteArray();}
    void setDetailData(unsigned int *d) override {return;}
    void setRtnData(unsigned int *d) override {return;}
    void updateCfgSettingLoadedData() override {return;}
    DesignItem* clone(DesignItem* toItem) override {return NULL;}
    void loadParaFromSetting(QSettings *ConfigIni) override {return;}
    void saveParaToSetting(QSettings *ConfigIni) override {return;}

private:
    CtrlSettingSerial *_setting = nullptr;
};

#endif // CTRLITEMRS485_H
