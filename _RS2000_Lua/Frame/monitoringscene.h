#ifndef MONITORINGSCENE_H
#define MONITORINGSCENE_H

#include <QGraphicsScene>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QUndoStack>
#include <QApplication>
#include <QString>
#include "common.h"
#include "configview.h"
#include "configItem/configitem.h"
#include "monitoring_aids/monitorproperty.h"
#include "monitoring_aids/monitoitemfactory.h"
#include "monitoring_aids/monitoitemfactory.h"

#define MONITOR 1

class MonitoringView;
class MonitoringScene : public QGraphicsScene
{
    Q_OBJECT
    enum {NORMAL_ZVALUE = 0, SELECT_ZVALUE = 10};
    enum SceneState {STATE_START = 0, STATE_SELECTAREA, STATE_WAITFORPORT};
public:
    explicit MonitoringScene(QObject *parent = nullptr);

     inline QUndoStack *getUndoStack() { return _undoStack; }
     inline MonitoringView *getView() { return _view; }
protected:
    void dropEvent(QGraphicsSceneDragDropEvent *e);//鼠标放下事件
    void dragEnterEvent(QGraphicsSceneDragDropEvent *e);//拖拽进入当前窗口事件
    void dragMoveEvent(QGraphicsSceneDragDropEvent *e);//拖拽移动事件
    void mousePressEvent(QGraphicsSceneMouseEvent *event);//鼠标按下事件
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);//鼠标移动事件
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);//鼠标释放事件
    void keyPressEvent(QKeyEvent *event);//键盘按下事件
    void keyReleaseEvent(QKeyEvent *event);//键盘释放事件
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);//右键菜单事件
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);//鼠标双击事件

    void setConnection();
    void set_Size(int w, int h);
    void setInitialize();
public:
    void open(QString fileName,int index);
    void SavePro(QString fileName);
    void saveItem();
signals:
    void sendSaveclick(bool);
public slots:
    void on_deleteClicked();
    void on_SetProperty();
    void on_PushButtonClicked();
    void on_getItemXY();


private:
    int _configIndex;            //设备的自增序号
    SceneState _sceneState;
    qreal _scaleView;              //缩放比例
    QMenu _displayMenu;          //右键菜单

    QMenu *_selectAllMenu;       //select
    QAction *_selectAll;
    QAction *_selectAllMeter;
    QAction *_selectAllClipper;
    QAction *_selectAllButton;
    QAction *_selectAllLabel;

    QMenu *_scale;               //scale
    QAction *_scale_25;
    QAction *_scale_50;
    QAction *_scale_75;
    QAction *_scale_100;
    QAction *_scale_125;
    QAction *_scale_150;
    QAction *_scale_175;
    QAction *_scale_200;

    QAction *_delAction;         //del
    QAction *_propertyAction;    //property

    QMenu *_displayStyle;         //displayStyle
    QAction *_display1;
    QAction *_display2;
    QAction *_display3;
    QAction *_display4;
    QAction *_display5;

    MonitoringView *_view;
    QUndoStack *_undoStack;
#if MONITOR
    MonitorItem *_monitorItem;
#else
    ConfigItem *_cfgItem;
    ConfigView *_view;
#endif
    QGraphicsTextItem *_sheetMaxSize;
    bool _labelEditor;   //标签是否可被改变大小

    //////////////monitor_Pos///////////
    QPointF _pressPos;   //鼠标按下位置
    QPointF _releasePos; //鼠标释放位置
    QPointF _offsetPos;  //坐标偏移

};

#endif // MONITORINGSCENE_H
