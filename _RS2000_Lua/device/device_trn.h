#ifndef DEVICE_TRN_H
#define DEVICE_TRN_H

#include <QString>
#include "AES70device.h"
#include "protocal/packetbase.h"

class ConfigItem;
class Device_Trn;
class Trn_Interface
{
public:
    Trn_Interface(Device_Trn* trnDev);
    ConfigItem * getInOutItemByChannel(int nmuVersion, CfgType cfgType, int channel);
    inline void appendSnapShotItem(ConfigItem *cfgItem) {_snapshotList.append(cfgItem);}
    inline void removeOneSnapShotItem(ConfigItem *cfgItem) {_snapshotList.removeAll(cfgItem);}
    inline void removeAllSnapShotItem(ConfigItem *cfgItem) {_snapshotList.removeAll(cfgItem);}
    inline bool containsSnapShotItem(ConfigItem *cfgItem) {return _snapshotList.contains(cfgItem);}
    inline QList<ConfigItem*> getInputModelList() {return _inputModelList;}
    inline QList<ConfigItem*> getOutputModelList() {return _outputModelList;}
    inline QList<ConfigItem*> getSnapshotList() { return _snapshotList; }
    inline QList<ConfigItem*> getControlModelList() {return _controlModelList;}
    inline int getCfgItemIndex() { return _cfgItemIndex++;}
private:
    int _cfgItemIndex = 0;  //自曾序号，确保名称唯一
    Device_Trn* _trnDev;
    QList<ConfigItem*> _inputModelList;
    QList<ConfigItem*> _outputModelList;
    QList<ConfigItem *> _snapshotList;
    QList<ConfigItem*> _controlModelList;
};


class Device_Trn : public AES70Device
{
    Q_OBJECT
public:
	enum DeviceTrnSendAllConfigTypeEnum {
	    SendAllConfigType_None = 0,
	    SendAllConfigType_Load,
	    SendAllConfigType_Copy,
	};

    Device_Trn(QString mac, QString devName, DevType devType, QString devIp, QString creator);

    virtual void setOffline();
    virtual void serialize(QString fileName);
    virtual void deserialize(QString fileName);

    void initData(const ConfigData_Trn& configData);
    bool checkFirmwareVersion();
    bool getUpgradeFile(QList<Utils::FirmwareUpdate_t> &verList, QString currDir);
    int getCpuCount();

public:
    QStringList getUserName() { return _userName; }
    void setSendConfigType(DeviceTrnSendAllConfigTypeEnum type) { _sendConfigType = type; }

    inline  void setGroup(qint16 group) { _group = group; }
    inline qint16 getGroup() { return _group; }
    void setDebugMode(bool isDebugMode) { _isDebugMode = isDebugMode; }
public slots:
    void setUserName(int ch, QString newName);
    void checkAliveTimerOut();

private:
    QByteArray getGrobalHeadBuff(quint32 moduleCount);
    void mixerUserNameSave();
    void mixerUserNameLoad();
    void saveFile(QString filename, bool bAppend, QByteArray array);
signals:
    void dispMeterA(FC_RTU_METER *meter);
    void fileListChanged(QStringList playList);
    void deviceOffline();
    void fileFromDeviceSig(unsigned char *data, int len);
    void fileFromDeviceEndSig(void);
    void ParaChangedFromUserSig(unsigned int *data);
    void SceneChangedFromUserSig(void);
    void ModuleRtnValueSig(unsigned int *data);
    void InOutLevelSig(unsigned int *level);

public://处理数据包
    virtual void processPacket(PacketBase* pack);
    virtual void sendHeartbeatPack();
    bool sendClearScenePack();
    bool sendPhantomPack(int channel, bool isOpen);
    bool sendMicGainPack(quint8 ch, qint8 value);
    bool sendAlgorithmSavePack(bool bStart, bool bOverview, quint8 scene_idx, quint32 moduleCount, QByteArray buff);
    bool sendAlgorithmOverviewPack(bool bStart, quint32 moduleCount, QByteArray buff);
    bool sendModuleNamePack(bool bStart, quint32 moduleCount, QByteArray buff);
    bool sendModuleNameSavePack(bool bStart, quint8 scene_idx, quint32 moduleCount, QByteArray buff);
    bool sendAlgorithmDetailPack(quint32 moduleCount, QByteArray buff);
    bool sendAlgorithmStartPack(void);
    bool sendAlgorithmRealtimePack(quint32 moduleCount, QByteArray buff);
    void sendUpdatePackage(quint32 totalCount, quint32 offset, quint32 paraLen, QByteArray buff);
    void sendIP(quint32 ip);
    void sendNetworkStreamModeSelect(int bank, quint32 mode);
    void sendGroup();
    void sendFileToDeviceStartPack(quint32 num);
    void sendFileToDevicePack(QByteArray buff);
    void sendFileToDeviceEndPack(void);
    bool sendFileFromDevicePack(void);

    //lua控件的数据包发送
    bool sendLuaPack(int downIdx, int luaItemIdx, QByteArray& luaPack);
    bool sendLuaSubPack(quint32 downIdx, quint32 luaItemIdx, quint32 totalSubCnt, quint32 subCntIdx, quint32 dataLen, const char* dataPtr);

    void reflashFiles(); // reflashPlayList
public:
    ConfigData_Trn& getConfigData() { return _configData;}
    int getItemUserZValueNum(bool increases) { if(increases) return _userZValueNum++; else return _userZValueNum; }
    void setItemUserZValueNum(int zValue) { _userZValueNum = zValue; }


private:
    int _userZValueNum = 0;
    bool _isDebugMode = false;
    QMap<int, QWaitCondition*> _waitConditionMap; 
    ConfigData_Trn _configData;
    QStringList _userName;
    QDomDocument* _domDoc;
    DeviceTrnSendAllConfigTypeEnum _sendConfigType;
    qint16 _group;
    QStringList _fileList;
    quint8 transctrl_version[4] = {0};
    quint8 devVer[8] = {0};   //Version of DSP、X86、ARM
};

#endif // DEVICE_TRN_H
