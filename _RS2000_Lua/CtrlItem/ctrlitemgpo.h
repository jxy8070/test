#ifndef CTRLITEMGPO_H
#define CTRLITEMGPO_H

#include "configItem/configitem.h"

class CtrlSettingGpo;
class CtrlItemGpo : public ConfigItem
{
    Q_OBJECT
public:
    CtrlItemGpo(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 100);

    QWidget *getSettingForm() override;
    QByteArray getDetailData() override {return QByteArray();}
    void setDetailData(unsigned int *d) override {return;}
    void setRtnData(unsigned int *d) override {return;}
    void updateCfgSettingLoadedData() override {return;}
    DesignItem* clone(DesignItem* toItem) override {return NULL;}
    void loadParaFromSetting(QSettings *ConfigIni) override {return;}
    void saveParaToSetting(QSettings *ConfigIni) override {return;}

private:
    CtrlSettingGpo* _setting = nullptr;
    friend class ConfigScene;
};

#endif // CTRLITEMGPO_H
