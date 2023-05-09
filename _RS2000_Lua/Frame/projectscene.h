#ifndef PROJECTSCENE_H
#define PROJECTSCENE_H


#include "common.h"
#include "absgraphscene.h"



class AbsGraphView;
class AES70Device;
class ProjectScene : public AbsGraphScene
{
    Q_OBJECT

public:
    explicit ProjectScene(AbsGraphView *pView = 0);
    virtual QGraphicsItem * CopyGraphicsLineItem(QGraphicsItem *graphItem, QMap<QGraphicsItem *, QGraphicsItem *> itemMap);

    inline int getDevItemIndex() { return _devItemIndex++;}
    inline QString filePath(){ return _filePath; }
    void fileLoad(QString &filePath, DevType loadType);
    void open(QString filePath);
    void save(QString filePath, bool removeAll = true);
    void saveAs(QString filePath);
    void onMonitorChanged(DeviceItem* devItem);
    QList<AES70Device*> getNotBindDevList(DevType type);
    QStringList getBindedMacList();
    inline qint16 getGroup(){return _group;}
    DeviceItem* castToDeviceItem(QGraphicsItem* item);

public slots:    
    void onProjActionTriggered();
    void viewDeviceProperty(void);
    void setMulticast();

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *e) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    bool isDeviceExistInScene(AES70Device* aes70Dev);
signals:
    void deviceOnlineUpdate(QString Mc, QString Name);
    void monitorChanged();

protected: //右键菜单相关
    void initMenuActions();
    void updateMenuActions() override;
    QAction *_propertyAction;
    QAction *_addToMonitor;
    QAction *_removeFromMonitor;
    QAction *_networkStreamModeSelect;
    QAction *_ip_set;
    QAction *_firmware_update;
    QAction *_algorithmAction;
    QAction *_multicastSet;

private:
    int _devItemIndex;  //自曾序号，确保名称唯一
    QString _filePath;
    qint16 _group;
};



#endif // PROJECTSCENE_H
