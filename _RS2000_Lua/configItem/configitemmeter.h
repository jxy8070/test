#ifndef CONFIGITEMMETER_H
#define CONFIGITEMMETER_H

#include "configitem.h"

class CfgSettingMeter;
class ConfigItemMeter : public ConfigItem
{
    Q_OBJECT
public:
    ConfigItemMeter(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemMeter();
    DesignItem *clone(DesignItem *fromItem) override;
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;
    void clearVuData();

    int getMeterNum() { return _meterNum; }
    int getHoldTime(int idx) { return _holdTime[idx]; }
    bool getHoldEnable(int idx) { return _holdEnable[idx]; }
    bool getInfiniteHold(int idx) { return _infiniteHold[idx]; }
    QString getUserName(int idx) { return _userName[idx]; }

    void setHoldTime(int idx, int vlu);
    void setHoldEnable(int idx, bool flag);
    void setInfiniteHole(int idx, bool flag);
    void setUserName(int idx, QString name);

    quint32 getRowCol() { return _rowCol; }

private:
    int _holdTime[8];
    bool _holdEnable[8];
    bool _infiniteHold[8];
    int _meterNum;
    QString _userName[8];
    quint32 _rowCol;

    enum{
        DEV_METER_HOLDMODE_DEFAULT 		= 0X00,
        DEV_METER_HOLDMODE_KEEPSETTIME 	= 0X10,
        DEV_METER_HOLDMODE_KEEPMAXVALUE = 0X11,
    };

    typedef struct{
        struct{
            __OPACK32_T2S0S1(
                OPACKU,ppmHoldCtrlMode,8,
                OPACKU,ppmHoldTime_ms,16
            );
        }meter[1];
    }IG_PARACTL_METER1;

    typedef struct{
        struct{
            __OPACK32_T2S0S1(
                OPACKU,ppmHoldCtrlMode,8,
                OPACKU,ppmHoldTime_ms,16
            );
        }meter[2];
    }IG_PARACTL_METER2;

    typedef struct{
        struct{
            __OPACK32_T2S0S1(
                OPACKU,ppmHoldCtrlMode,8,
                OPACKU,ppmHoldTime_ms,16
            );
        }meter[4];
    }IG_PARACTL_METER4;

    typedef struct{
        struct{
            __OPACK32_T2S0S1(
                OPACKU,ppmHoldCtrlMode,8,
                OPACKU,ppmHoldTime_ms,16
            );
        }meter[8];
    }IG_PARACTL_METER8;

    IG_PARACTL_METER1 _body_1;
    IG_PARACTL_METER2 _body_2;
    IG_PARACTL_METER4 _body_4;
    IG_PARACTL_METER8 _body_8;

    CfgSettingMeter *_setting;
};
#endif // CONFIGITEMMETER_H
