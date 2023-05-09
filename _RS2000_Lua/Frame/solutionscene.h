#ifndef SOLUTIONSCENE_H
#define SOLUTIONSCENE_H

#include "common.h"
#include "absgraphscene.h"

class AbsGraphView;
class AES70Device;
class DeviceItem_Room;
class SolutionScene : public AbsGraphScene
{
    Q_OBJECT

public:
    explicit SolutionScene(AbsGraphView *pView = 0);
    virtual QGraphicsItem * CopyGraphicsLineItem(QGraphicsItem *graphItem, QMap<QGraphicsItem *, QGraphicsItem *> itemMap);

    inline int getDevItemIndex() { return _devItemIndex++;}
    inline QString fileName(){ return _fileName; }
    void open(QString fileName, QStringList projectList);
    void save(bool removeAll = true);
    void saveAs(QString fileName);
    void onMonitorChanged(DeviceItem* devItem);
    inline qint16 getGroup(){return _group;}

    DeviceItem_Room* castToRoomItem(QGraphicsItem* item);
    void deleteRoomItem(QString roomName);
    void renameRoomItem(QString oldName, QString newName);
    DeviceItem_Room *getRoomItem(QString roomName);

    void loadDeviceItem(QSettings *Configini, QString itemGroup, QMap<QString, QString> roomMap);
    void loadLineItem(QSettings *ConfigIni, QString itemGroup);

public slots:    
    void onProjActionTriggered();
    void viewDeviceProperty(void);
    void setMulticast();

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
signals:
    void deviceOnlineUpdate(QString Mc, QString Name);
    void monitorChanged();

protected: //右键菜单相关
    void initMenuActions();
    void updateMenuActions() override;
    QAction *_propertyAction;
    QAction *_networkStreamModeSelect;
    QAction *_multicastSet;

private:
    int _devItemIndex;  //自曾序号，确保名称唯一
    QString _fileName;
    qint16 _group;
};



#endif // SOLUTIONSCENE_H
