#ifndef CONFIGITEMAUDIORECORD_H
#define CONFIGITEMAUDIORECORD_H
#include "configitem.h"

class CfgSettingAudioRecord;
class ConfigItemAudioRecord : public ConfigItem
{
public:
    ConfigItemAudioRecord(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 106);
    ~ConfigItemAudioRecord();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    DesignItem* clone(DesignItem* toItem) override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    void start();
    void stop();

    static int GlobalRecordIdx;
private:
    void initData();

private:
    int _recordIdx;
    quint8 _recordCmd;
    quint16 _sampleRate;    //采样率
    char _recordPath[200];
    char _recordPerfix[56];
    CfgSettingAudioRecord *_setting;

    typedef struct{
        __OPACK32_T3(
            OPACKU,recordIdx,8,				//序号
            OPACKU,recordCmd,8,					//0:stop, 1:start, 2:pause, 3:resume
            OPACKU,sampleRate,16
        );
        char recordPath[200];
        char recordPerfix[56];
    }IG_PARACTL_RECORD;

    IG_PARACTL_RECORD _body;
};

#endif // CONFIGITEMAUDIORECORD_H
