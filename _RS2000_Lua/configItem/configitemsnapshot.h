#ifndef CONFIGITEMSNAPSHOT_H
#define CONFIGITEMSNAPSHOT_H

#include "configitem.h"
#include "Frame/configview.h"
//#include "configitemmixer.h"
#include "Protocal/eqpacket.h"
#include <QHash>
#include <QtDebug>
#include <QObject>

#define BtnType "btnType"
#define Serial "serial"

class SnapshotSaveObject;
class SnapshotSaveDelayData;
class SnapshotSaveRouterData;
class SnapshotSaveMixerData;
class SnapshotSaveDynData;
class SnapshotSavePriorityDuckerData;
class SnapshotSaveEqData;
class SnapshotSaveSplitData;
class SnapshotSaveFilteData;
class SnapshotSaveSignalData;
class SnapshotSaveMeterData;
class SnapshotSaveGpiData;
class SnapshotSaveFreqShiftData;
class SnapshotSaveAutoMixData;
class SnapshotSaveAecData;
class SnapshotSaveNfcData;
class SnapshotSaveGainRampData;
class SnapshotSaveGainData;
class SnapshotSaveCrossfaderData;
class SnapshotSaveInputData;
class SnapshotSaveOutputData;

class CfgSettingSnapshotBank;
class ConfigItemSnapshot : public ConfigItem
{
    Q_OBJECT
    Q_PROPERTY(int SnapCount MEMBER _snapNum NOTIFY snapCountChanged_Signal)
public:
    enum snapshotType{
        DataCount = 24,
        Create_snap_Global,
        Create_Snap_Bank,
        Create_snap_Bank_FromSel,
    };
    enum btnType{
        BtnType_Prev = 0,
        BtnType_Load,
        BtnType_Save,
        BtnType_Next,
        BtnType_label,

    };
    ConfigItemSnapshot(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);

    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    void uploadData(int type);
    void saveData(int type);

    inline bool getbuttonPattern(int idx) { return _buttonPattern[idx]; }
    inline void setButtonPattern(int idx, bool status) { _buttonPattern[idx] = status; }
    inline int getCurrentPattern() { return _currentPattern; }
    inline void setCurrentPattern(int type) { _currentPattern = type; }

    inline int getCreateType() { return _createType; }
    inline void setCreateType(int type) { _createType = type; }
    inline QSet<ConfigItem *> getFromSel_CtrlsList() { return _fromSel_CtrlsList; }
    inline void insertFromSel_CtrlsList(ConfigItem *cfgItem) {  _fromSel_CtrlsList.insert(cfgItem); }
    inline void removeFromSel_CtrlsList(ConfigItem *cfgItem) { _fromSel_CtrlsList.remove(cfgItem); }
    inline bool containsFromSel_CtrlsList(ConfigItem *cfgItem) { return _fromSel_CtrlsList.contains(cfgItem); }
    inline int getSnapCount() { return _snapNum; }
    inline void setSnapCount(int num) { _snapNum = num; }
    inline QString getLabel(int idx) { return _LabelList.at(idx); }
    inline void setLabel(int idx, QString text) { _LabelList[idx] = text; }

signals:
    void snapCountChanged_Signal(int);
private slots:
    void onSnapCountChanged_slots(int);

private:
    void upLoalModuleData(QSettings *,QString, SnapshotSaveObject *, int, CfgType);

public:
    bool _buttonPattern[DataCount]; //save loadBtn Enabel property
    int _currentPattern; // 1~24

    SnapshotSaveDelayData *_delayData = NULL;
    SnapshotSaveRouterData *_routerData = NULL;
    SnapshotSaveMixerData *_mixerData = NULL;
    SnapshotSaveDynData *_dynData = NULL;
    SnapshotSavePriorityDuckerData *_priorityDuckerData = NULL;
    SnapshotSaveEqData *_eqData = NULL;
    SnapshotSaveSplitData *_splitData = NULL;
    SnapshotSaveFilteData *_filteData = NULL;
    SnapshotSaveSignalData *_signalData = NULL;
    SnapshotSaveMeterData *_meterData = NULL;
    SnapshotSaveGpiData *_gpiData = NULL;
    SnapshotSaveFreqShiftData *_freqShiftData = NULL;
    SnapshotSaveAutoMixData *_autoMixData = NULL;
    SnapshotSaveAecData *_aecData = NULL;
    SnapshotSaveNfcData *_nfcData = NULL;
    SnapshotSaveGainRampData *_gainRampData = NULL;
    SnapshotSaveGainData *_gainData = NULL;
    SnapshotSaveCrossfaderData *_crossfaderData = NULL;
    SnapshotSaveInputData *_inputData = NULL;
    SnapshotSaveOutputData *_outputData = NULL;

    int _snapNum;  // 1 ~ 24
    int _createType = Create_snap_Global;
    QSet<ConfigItem *> _fromSel_CtrlsList;
    CfgSettingSnapshotBank *_cfgSettingSnapshot;
    QList<QString> _LabelList;

};

#endif // CONFIGITEMSNAPSHOT_H
