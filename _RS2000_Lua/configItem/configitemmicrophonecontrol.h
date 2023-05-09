#ifndef CONFIGITEMMICROPHONECONTROL_H
#define CONFIGITEMMICROPHONECONTROL_H

#include "configitem.h"

class CfgSettingMicrophoneControl;
class ConfigItemMicrophoneControl : public ConfigItem
{
public:
    ConfigItemMicrophoneControl(Device_Trn* trnDev, CfgType cfgType,QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemMicrophoneControl();
    QWidget *getSettingForm() override;
    QByteArray getOverviewData(bool is16Bit) override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

public:
    quint16 getMicrophoneNo();
    quint16 getMaxConnectNum();
    void setMicrophoneControlInfo(int idx,int type);

private:

    typedef struct{
     IG_SIGFLOW8_32X32 flow;
     union {
         unsigned int exdata;       //留给上位机使用的额外数据，本地不用
         struct {
             unsigned char vip_no;
             unsigned char max_mic_num;
             unsigned char module_num;
         };
     };
    }IG_SIGFLOW_MICROPHONECTRL;

    typedef struct{
     unsigned int sigswitch;       //信号通断开关
     union {
         unsigned int exdata;       //留给上位机使用的额外数据，本地不用
         struct {
             unsigned char vip_no;
             unsigned char max_mic_num;
             unsigned char module_num;
         };
     };
    }IG_PARACTL_MICROPHONECTRL;

    typedef struct{
        IG_SIGFLOW8_48X48 flow;
        union {
            unsigned int exdata;       //留给上位机使用的额外数据，本地不用
            struct {
                unsigned char vip_no;
                unsigned char max_mic_num;
                unsigned char module_num;
            };
        };
    }IG_SIGFLOW_SIGSWITCH48;

    typedef struct{
        unsigned int sigswitch[2];						//信号通断开关
        union {
            unsigned int exdata;       //留给上位机使用的额外数据，本地不用
            struct {
                unsigned char vip_no;
                unsigned char max_mic_num;
                unsigned char module_num;
            };
        };
    }IG_PARACTL_SIGSWITCH48;

    IG_SIGFLOW_MICROPHONECTRL _header32;
    IG_PARACTL_MICROPHONECTRL _body32;
    IG_SIGFLOW_SIGSWITCH48 _header48;
    IG_PARACTL_SIGSWITCH48 _body48;
    CfgSettingMicrophoneControl *_setting;
};
#endif // CONFIGITEMMICROPHONECONTROL_H
