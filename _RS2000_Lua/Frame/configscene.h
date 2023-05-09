#ifndef CONFIGSCENE_H
#define CONFIGSCENE_H

#include "common.h"
#include "absgraphscene.h"
#include <QMap>
#include <QString>
#include <QColorDialog>
#include <QMenu>
#include <QFile>
#include <QSettings>

class ToolTip;
class LineItem;
class ItemPort;
class ConfigItem;
class ConfigView;
class Device_Trn;
class Trn_Interface;
class Trn_SceneWidget;
class ConfigItemSnapshot;
class QTreeWidgetItem;
class CtrlItemLua;
class SerpentineConnector;

struct SubPack //算法下载中的子包
{
    SubPack(){
        count = 0;
        dataBuff.clear();
    }
    int count;
    QByteArray dataBuff;
    void Clear() {count = 0; dataBuff.clear();}
};

class ConfigScene : public AbsGraphScene
{
    Q_OBJECT
    enum { SELECT_ZVALUE = 10};

public:
    ConfigScene(Device_Trn *trnDev,  AbsGraphView *pView  = 0);
    ~ConfigScene();
    QGraphicsItem * CopyGraphicsLineItem(QGraphicsItem *graphItem, QMap<QGraphicsItem *, QGraphicsItem *> itemMap) override;
    void bindTrnDevice(Device_Trn *trnDev);

    void open(QString filePath, bool isImport);
    void save();
    void saveAs(QString filePath);
    static void createSceneIni(QString filePath, Device_Trn *trnDev);
    void saveParamToDevice(ConfigItem *cfgItem);
    static bool getConfigFileMac(QString filePath, QString& mac, DevType& devType);

    void subTypeChanged(ConfigItem* item, int newTypeIdx, int oldTypeIdx);
    QString getUniqueName(QString customName);
    void setPinLineVisible(ItemPort* pinPort, bool visible);
    void setDebugMode(bool isDebug, bool needDownload);
    void clearDebugModeData();
    bool setOutputMute(bool isMute);
    CLuaPinParam_PC* getInCtrlPinDefTree(ItemPort* pinPort);
    QList<CLuaPinParam_PC*> getOutCtrlPinDefTree(ItemPort* pinPort);
    QList<ConfigItem*> getConfigItemList();

public://inline
    inline bool isDebuging() {return _isDebuging;}
    inline QList<ConfigItem*> getUciList() { return _UCIList; }
    inline QString filePath(){ return _filePath; }

    Device_Trn *getTrnDev() { return _trnDev; }
    Trn_Interface *getTrnInterface() { return _trnINF; }
    int getSpaceUsage() { return _spaceUsage; }
    int getTimeUsage() { return _timeUsage; }

public slots:
    void onCfgContextMenuClicked();
    void deviceOffline();
    void loadFromDevice();
    void onActionTriggeredSlots();//hrp
    void onTreeWidgetKey(Qt::Key key, QTreeWidgetItem *treeWidItem);//hrp
    void addSnapBankItems(QList<QGraphicsItem*> itemList, bool b); //hrp
    void delSnapBankItems(QList<QGraphicsItem*> itemList, bool b, bool sceneStatus = false); //hrp
    void addUserControlInterfaceItem();
    void deleteCfgItems(QList<QGraphicsItem *> cfgItemList);
    void saveSceneToDevice(quint8 scene_idx);
    void testSlot();

private slots:
    void fileFromDeviceSlot(unsigned char *data, int len);
    void fileFromDeviceEndSlot(void);
    void ParaChangedFromUserSlot(unsigned int *para);
    void ModuleRtnValueSlot(unsigned int *v);
    void InOutLevelSlot(unsigned int *level);

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *e) override;

private:
    bool onDownloadAlgorithm();
    void prepareAlgorithmPack(QList<SubPack> &modulenamePacks, QList<SubPack> &overviewPacks, QList<SubPack> &detailPacks);
    void saveHandler(QString filePath);
    void initConfigItems(bool clearLayer);
    QList<ConfigItem*> getFirstLayerItems();
    QList<ConfigItem*> getNextLayerItems(ConfigItem* cfgItem);
    void setCfgItemLayer(ConfigItem* cfgItem, QList<ConfigItem*> cfgPath);
    void setCpuGroup(QList<ConfigItem*> cfgPath);
    bool checkDesign(); //检查方案连线、CPU、空间等
    QList<CtrlItemLua*> prepareLuaAlgorithm();
    bool isPointInArea(QRectF area, QPointF point);
    ConfigItem* castToConfigItem(QGraphicsItem* item);
    bool setAlgorithmMatrix();
    ItemPort* getLinkedStartPort(ItemPort* port);       //获取连接的起点端口
    QList<ItemPort*> getLinkedEndPort(ItemPort* port);         //获取连接的终点端口
    QList<ConfigItem*> getLabelMatchedItems(LineLabel* label);
    ItemPort* getLabelMatchedStartPort(LineLabel* label);
    QList<ItemPort*> getLabelMatchedEndPort(LineLabel* label);
    int setAlgorithmPortMatrix(ConfigItem* item, int colIdx, int rowIdx);

    QSet<ConfigItem *> foundByNextSerConItem(ItemPort *portItem);
    ItemPort *getLinkedStartPortBySerCon(ItemPort *portItem, int portNum);
    bool readConfigItem(int nmuVersion, QSettings *ConfigIni, QString itemGroup);
    bool readLineItem(int nmuVersion, QSettings *ConfigIni, QString itemGroup);

protected: //右键菜单相关
    void initMenuActions();
    void updateMenuActions() override;
    QMenu *_relevance;
    QAction *_showSpaceTimePercent;
    QAction *_copyObject;
    QAction *_pasteObject;
    QAction *_copyParameterConfig;
    QAction *_pasteParameterConfig;
    QAction *_propertyAction;

private:
    Device_Trn *_trnDev = NULL;
    Trn_Interface *_trnINF = NULL;
    QString _filePath;
    QList<ConfigItem *> _UCIList;
    QList<ToolTip *> _tooltipList;

    int _maxConfigLayer = 0;
    float _spaceUsage;
    float _timeUsage;
    int _cpuGroupIdx = 0;
    QMap<int, QSet<ConfigItem*>> _cpuGroupMap;
    QList<ConfigItem*> _cfgItemList;
    QList<LineItem*> _lineList;

    ConfigItem *_copyItem;
    QByteArray _devSceneBuff;
    QMap<quint16,ConfigItem *>  _algorithmMap;
};
#endif // CONFIGSCENE_H
