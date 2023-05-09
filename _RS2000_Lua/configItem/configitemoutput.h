#ifndef CONFIGITEMOUTPUT_H
#define CONFIGITEMOUTPUT_H
#include "configitem.h"

class CfgSettingOutput;
class ConfigItemOutput : public ConfigItem
{
    Q_OBJECT
public:
    ConfigItemOutput(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemOutput();
    void initRealChannel(DevPortType portType, int realCh, int logicCh);
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
    inline DevPortType getPortType() {return _portType;}
    inline int getLogicChannel(int offset){return _logicCh + offset;}
    inline int getRealChannel(int offset){return _realCh + offset;}
    inline quint16 getMaxPort(void) { return _max_port; }

    void setUserLabel(int idx, QString text);
    QString getUserLabel(int idx);
    double getGain(int offset);
    bool isInvert(int offset);
    bool isMute(int offset);

    void setGain(int offset, double gain);
    void setInvert(int offset, bool isInvert);
    void setMute(int offset, bool isMute);
    void setAllMute(bool isMute);

private:
    DevPortType _portType;
    typedef struct{
        __OPACK32_T3S0S1S2(
            OPACKU,mute,4,				//1静音，0正常输出
            OPACKU,antiPhase,4,			//1反相，0正向
            OPACKU,gain,16              //1开，0关,gain_db = gain * 0.01 - 100;
        );
    }IG_PARACTL_OUTPUT1;
    typedef struct{
        unsigned int mute;              //1静音，0正常输出
        unsigned int antiPhase;         //1反相，0正向
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16         //gain_db = gain * 0.01 - 100;
            );
        }gain[8];
    }IG_PARACTL_OUTPUT_NEW;

    quint16 _realCh = 0;            //真实通道号
    quint16 _logicCh = 0;           //逻辑通道号
    IG_PARACTL_OUTPUT1 _body1;
    IG_PARACTL_OUTPUT_NEW _bodyNew;

    CfgSettingOutput *_setting = NULL;
    quint16 _max_port;
    QStringList _userLabel;
};

//================真实协议参考interfaceprotocol.h================//

#endif // CONFIGITEMOUTPUT_H
