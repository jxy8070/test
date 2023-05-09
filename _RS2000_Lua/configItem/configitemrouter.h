#ifndef CONFIGITEMROUTER_H
#define CONFIGITEMROUTER_H

#include "configitem.h"

class CfgSettingRouter;
class ConfigItemRouter : public ConfigItem
{
public:
    ConfigItemRouter(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemRouter();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

public:
    quint16 getRouterInfo(int idx);
    quint16 getPortNum();

    void setRouterInfo(int idx, quint16 vlu);


private:
    int _portNum;
    typedef struct{
        struct{
            __OPACK32_T1S0(
                OPACKU,itoo,8
            );
        }routerch[256];
    }IG_PARACTL_VROUTER;

    IG_PARACTL_VROUTER _body;

    CfgSettingRouter *_setting;
    CfgType _cfgType;
};


#endif // CONFIGITEMROUTER_H
