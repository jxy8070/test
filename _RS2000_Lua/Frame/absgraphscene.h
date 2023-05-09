#ifndef ABSGRAPHSCENE_H
#define ABSGRAPHSCENE_H

#include <QGraphicsSceneDragDropEvent>
#include <QApplication>
#include <QUndoStack>
#include <QString>
#include <QColorDialog>
#include <QMenu>
#include <QLineF>
#include <QGraphicsScene>
#include "common.h"
#include "DesignItem.h"

//随设备位置移动场景滚动条
enum ScrollBarMoveOrderEnum {
    ScrollBarMoveOrder_None = 0,
    ScrollBarMoveOrder_Up,
    ScrollBarMoveOrder_Down,
    ScrollBarMoveOrder_Left,
    ScrollBarMoveOrder_Right,
};

enum AlignMode
{
    AlignLeft = 0,
    AlignRight,
    AlignTop,
    AlignBottom,
    AlignEqHSpace,
    AlignEqVSpace,
};



class DeviceItem;
class AbsGraphView;
class ControlTemplate;
class LineLabel;
class AbsGraphScene : public QGraphicsScene
{
    Q_OBJECT
    enum {SceneRectLimit_W = 14000, SceneRectLimit_H = 9000};

public:
    enum ToolkitType{
        ToolkitType_invalid = -1,
        ToolkitType_text,
        ToolkitType_GroupBox,
        ToolkitType_Polyline,
    };
    void setToolkitType(ToolkitType type);
    ToolkitType getToolkitType() { return _toolkitType; }
public:
    explicit AbsGraphScene( AbsGraphView *pView);
    ~AbsGraphScene();
    virtual QGraphicsItem * CopyGraphicsLineItem(QGraphicsItem *graphItem, QMap<QGraphicsItem *, QGraphicsItem *> itemMap);

    QList<QGraphicsItem*> prepareItems(QList<QGraphicsItem*> sourceItems);
    void addGraphicsItems(QList<QGraphicsItem*> itemList);
    void deleteGraphicsItems(QList<QGraphicsItem*> itemList);
    void itemPropChanged(DesignItem* dItem, const char *propName, const QVariant& newVal, const QVariant& oldVal);
    void setItemsAlign(AlignMode alignMode);
    void setItemsEqSpace(AlignMode alignMode);
    QPointF getCursorScenePos();
    QPointF drawAlignLine(QGraphicsItem* moveItem, bool excludeSelected);
    void addLineLabel(bool& skipSpace);
    void addSetLabelCommand(QList<LineLabel*> labelList, QString lableName);
    void matchLineLabels();     //检查匹配LineLabel
    QList<LineLabel*> getUnmatchedLabels();
    QList<LineLabel*> filterLabels(DevPortType portType, bool isInput, const QString& text);
    bool isLabelNameDuplicate(const QString& labelName);
    QString getNewLabelName(const QString& labelName);

    void addSnapBankController(QGraphicsItem *parentItem, QList<QGraphicsItem *> itemList);
    void delSnapBankController(QGraphicsItem *parentItem, QList<QGraphicsItem *> itemList);

    void addSerpentineConnector();

    void autoFitSceneSize();    
    void setFitPos(QGraphicsItem *item, QPointF& scenePos);
    void sortByPosition(QList<DesignItem *>& items, bool isHorizontal);
    QString getViewTitle();
    bool checkEditable();
    void focusSingleItemInRect(QRectF rect);

    //分块显示
    QStringList getPageList() { return _pageList; }
    QString getCurPage(){ return _curPageName; }
public slots:
    void addPage(QString pageName) { _pageList << pageName; }
    void deletePage(QString pageName);
    void renamePage(QString oldPageName, QString newPageName);
    void setCurPage(QString pageName);

signals:
    void designItemChanged(AbsGraphScene *pScene);
    void needSaveChanged(bool needSave);
    void updateSolutionRoomPorts(QString fileName, QMap<int, QList<QString>> inPorts, QMap<int, QList<QString>> outPorts);

private:
    QList<ItemPort*> getStartItemPortsForLine();
    QList<ItemPort*> getStartItemPortsForLabel();
    QList<ItemPort*> getEndItemPorts(QPointF pos, int portType, bool isInput);

public: //inline
    inline QUndoStack *getUndoStack() { return _undoStack; }
    inline bool isNeedSave() {return _needSave;}
    inline void setNeedSaveState(bool needSave) {if(_needSave != needSave){_needSave = needSave; emit needSaveChanged(needSave);} }
    inline QString sheetSize() { return QString::asprintf("%.0f x %.0f", sceneRect().width(), sceneRect().height()); }

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *e) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *) override;
    void dropEvent(QGraphicsSceneDragDropEvent *e) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

protected slots:
    void onBaseActionTriggered();
    virtual void onItemsDelete();
    virtual void onItemsCopy();
    virtual void onItemsPaste();
    void onEscapePress();
    void onSkinChanged();
    void onEscapeCreateGraphicsItem();
    void onFocusItemChanged(QGraphicsItem* newItem, QGraphicsItem* oldItem, Qt::FocusReason reason);

protected:
    static QLineF getShortLine(bool isHorizontal , qreal p, qreal p1, qreal p2, qreal p3, qreal p4);
    QList<QGraphicsItem*> getConnectedLinesOrLineLabels(DesignItem* dItem);
    QGraphicsItem * CopyGraphicsDesignItem(QGraphicsItem *graphItem);
    QGraphicsItem * CopyGraphicsPortItem(QGraphicsItem *graphItem);
    bool checkDeviceLink(ItemPort *startPort, ItemPort *endPort); //检查规则
    void addTempGroup();
    QList<QGraphicsItem*> getTempGroupChild();
    void resetTempGroup();
    QRectF getItemsBoundingRect(QPointF topLeft, QList<QGraphicsItem *> items);
protected:
    AbsGraphView* _view;

    QUndoStack *_undoStack;
    bool _needSave;
    bool _isDebuging = false;       //是否调试模式
    LineLabel* _copiedLabel = NULL;  //拷贝Label
    QList<QGraphicsItem*> _copyItems;
    QGraphicsItemGroup* _tempGroup;
    QGraphicsLineItem* _vAlignLine;
    QGraphicsLineItem* _hAlignLine;
    QList<LineLabel*> _labelList;
    QGraphicsProxyWidget* _labelEditProxy;
    QGraphicsItem* _oldFocusItem;
    int _extInOutPortNum[2] = {0};   //Number of input and output labels for the room
    QStringList _pageList;
    QString _curPageName;

protected: //右键菜单
    void initCommonMenuActions();
    virtual void updateMenuActions();
    QMenu _contextMenu;
    QAction *_delAction;
    QMenu *_selectAllMenu;
    QAction *_selectAllItem;
    QAction *_selectAllLink;
    QAction *_selectAllObject;

protected://自定义绘制工具
    void createGraphicsItem();
    ToolkitType _toolkitType;
    ControlTemplate *rectItem = NULL;
    QGraphicsPathItem *_grapPathItem = NULL;  // 多边形模块 多点绘制临时路径
    QPainterPath _pPath;
};

#endif // ABSGRAPHSCENE_H
