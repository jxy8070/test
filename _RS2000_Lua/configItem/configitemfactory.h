#ifndef CONFIGITEMFACTORY_H
#define CONFIGITEMFACTORY_H

class ConfigItem;
class AbsGraphView;
class Device_Trn;
enum CfgType;
class ConfigItemFactory
{
public:
    static ConfigItem *createConfigItem(Device_Trn* trnDev, CfgType cfgType, QString customName, QPointF scenePos);
    static bool isStartCfgItem(CfgType cfgType);
    static bool isEndCfgItem(CfgType cfgType);
};

#endif // CONFIGITEMFACTORY_H
