#ifndef CONFIGITEMDYN_H
#define CONFIGITEMDYN_H

#include "configitem.h"

class CfgSettingDyn;
class ConfigItemDyn : public ConfigItem
{
    Q_OBJECT

public:
    ConfigItemDyn(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemDyn();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    bool   getBypass()    {return _body.bypass;}
    double getThreshold() {return _body.threshhold * 0.01 - 100;}
    double getRatio()     {return _body.ratio * 0.01;}
    double getGain();
    double getAttack()    {return _body.responseT_ms;}
    double getRelease()   {return _body.releaseT_ms;}

    void setBypass(bool flag);
    void setThreshold(double value);
    void setRatio(double value);
    void setGain(double value);
    void setAttack(double value);
    void setRelease(double value);

private:
    void initDyn(double ratio);

private:
    typedef struct{
        __OPACK32_T2(
            OPACKU,threshhold,16,
            OPACKU,ratio,16					//expand:oiratio = ratio*0.01f;comp,auto:oiratio = 1.0f/ratio
        );
        __OPACK32_T2(
            OPACKU,responseT_ms,16,
            OPACKU,releaseT_ms,16
        );
        __OPACK32_T2(
            OPACKU,eGain,16,
            OPACKU,oTargetGain,16
        );
        __OPACK32_T1S0(
            OPACKU,bypass,8
        );
    }IG_PARACTL_DYN;

    typedef struct{
        __OPACK32_T2(
            OPACKU,iLevel,16,
            OPACKU,oLevel,16
        );
    }IG_PARAGET_DYN;

    IG_PARACTL_DYN _body;

    CfgSettingDyn *_setting;
};
#endif // CONFIGITEMDYN_H
