#ifndef CONFIGITEMUCI_H
#define CONFIGITEMUCI_H

#include "configitem.h"
#include <QHash>

class Device_Trn;
typedef struct ProcessorData{
    QString s_mac;
    int s_type;
    QString s_ip;
    QString s_customName;
}ProcessorData_t;

class ConfigItemUCI : public ConfigItem
{
public:
    typedef struct Uci_Data {
        uint s_channel;
        uint s_visible;
        uint s_page;
        uint s_pos;
        uint s_cfgType;
        char *s_modelType;
        char *s_name;
        char *s_userLab;
        union{
            struct gainRange{           //input、output gainRange
                int maxValue;
                int minValue;
            }s_gainRange;
            uint s_mixerNodesSwitch;    //Mixer nodes switch status
        };
        Uci_Data(int cfgType, QByteArray type, QByteArray name, QByteArray userLab,
                 int ch, int page, int pos, int visible = 1) {
            s_modelType = (char *)malloc(type.length()+1);
            s_name      = (char *)malloc(name.length()+1);
            s_userLab   = (char *)malloc(userLab.length() +1);
            memcpy(s_modelType, type.data(), type.length());
            memcpy(s_name,      name.data(), name.length());
            memcpy(s_userLab, userLab.data(), userLab.length());
            s_modelType[type.length()] = '\0';
            s_name[name.length()] = '\0';
            s_userLab[userLab.length()] = '\0';

            s_channel = ch;
            s_visible = visible;
            s_page = page;
            s_pos = pos;
            s_cfgType = cfgType;
            s_gainRange.maxValue = 0;
            s_gainRange.minValue = 0;
            s_mixerNodesSwitch = 0;
        }
        void setVisible(int v) {
            s_visible = v;
        }
        void setPage(int page) {
            s_page = page;
        }
        void setPos(int pos) {
            s_pos = pos;
        }
        void setGainRange(int minValue, int maxValue) {
            s_gainRange.minValue = minValue;
            s_gainRange.maxValue = maxValue;
        }
        void setLab(QString lab) {
            QByteArray userLab = lab.toUtf8();
            memcpy(s_userLab, userLab.data(), userLab.length());
            s_userLab[userLab.length()] = '\0';
            printf("%s", s_userLab);
        }
        void setMixerNodesSwitch(int status) {
            s_mixerNodesSwitch = status;
        }
    }Uci_data_t;
public:
    ConfigItemUCI(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);

    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    QByteArray getOnlineData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    void addTabWidgetData(Uci_data_t *data) { _uciTabWidgetItemList.append(data); }
    void setAES70Device(Device_Trn *dev) { _trnDev = dev; }
//    void delUciCtrlItem(ConfigItem *item) { _uciCtrlItemList.removeAll(item); }
    void saveDataToUciTable(QString fileName);
    QList<QString> openDatafromUciFile(QString fileName);

    QString getAES70DevMac();
    Device_Trn *getAES70Dev() { return _trnDev; };

    QList<Uci_data_t *> getTabWidgetItemList() { return _uciTabWidgetItemList; }
    QMap<int, Uci_data_t *> getTabWidgetItem(CfgType cfgType);
    QMap<int, Uci_data_t *> getTabWidgetItem(int page);
    //////////////////////uci界面///////////////////////////////
    void setLoadUciFileProcData(ProcessorData_t data) { _loadfromUcifileProcData = data; }
    ProcessorData_t getLoadUciFileProcData() { return _loadfromUcifileProcData; }

    void setSceneTitle(QString title) { _sceneTitle = title; }
    QString getSceneTitle() const { return _sceneTitle; }
public slots:

private:

    QList<Uci_data_t *> _uciTabWidgetItemList;
    /////////////////uci 界面使用//////////////////////
    ProcessorData_t _loadfromUcifileProcData;
    QString _sceneTitle = tr("Title");
};

#endif // CONFIGITEMUCI_H
