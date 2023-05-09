#ifndef TOOLKITTREE_H
#define TOOLKITTREE_H

#include <QTreeWidget>
#include <QDragEnterEvent>
#include "configscene.h"
#include <QModelIndex>

enum DevType;
enum CfgType;
enum SubWnd_Type;

class ToolKitTreeItem;
class ConfigScene;
class ConfigItem;
class ConfigItemUCI;
class ToolKitTree : public QTreeWidget
{
    Q_OBJECT

public:
    enum TreeType{
        TreeStrType_default = 0,
        TreeStrType_Solution,
        TreeStrType_DevModule,
        TreeStrType_DevInterface,
        TreeStrType_component,
    };
    ToolKitTree(TreeType type, QWidget *parent = 0);
    ToolKitTree(QWidget *parent = 0);
    void SetTreeItems(QList<QTreeWidgetItem *> items, ConfigScene *pScene = NULL);
    void HideDockWidget();
    //设备列表(和客户相关)
    static QList<QTreeWidgetItem *> GetDeviceItems();
    //算法模块(和平台相关)
    static QList<QTreeWidgetItem *> GetModelItems_Dsp(int totalPortNum, ConfigScene *pScene = NULL);
    static QList<QTreeWidgetItem *> GetModelItems_X86(int totalPortNum, ConfigScene *pScene = NULL);
    static QList<QTreeWidgetItem *> GetInOutItems(ConfigScene *pScene, bool groupAnalogNet = true);
    static QList<QTreeWidgetItem *> GetGuiItems();

    static QList<QTreeWidgetItem *> GetSnapshotItems(ConfigScene *pScene);
    static QList<QTreeWidgetItem *> GetUserControlInterfacesItems(ConfigScene *pScene);
//    static QList<QTreeWidgetItem *> GetConfigLeftTree(ConfigScene *pScene);
    static QList<QTreeWidgetItem *> GetUciLoadProcItems(QHash<QString, ConfigItemUCI *> list);

    inline TreeType getTreeType() { return _treeType; }
signals:
    void keyEventSignals(Qt::Key, QTreeWidgetItem *);
private:
    void LoadingCollapsedNodes(QList<QTreeWidgetItem *> items);//hrp
    static QTreeWidgetItem *getTreeItemName(QTreeWidgetItem *treeWidItem, QString text);//hrp
public slots:
    void portChanged(AbsGraphScene *pScene);
    void onOpenNodesSlots(QTreeWidgetItem *item);
    void onCloseNodesSlots(QTreeWidgetItem *item);

    void addSnapshotBankBtn(QList<QTreeWidgetItem *> items, ConfigScene *pScene);
    void addUserControlInterface(QList<QTreeWidgetItem *> items, ConfigScene *pScene);
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void performDrag();

    virtual void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
//==============拖拽改变顺序==============//
//    void dragEnterEvent(QDragEnterEvent *e);
//    void dragMoveEvent(QDragMoveEvent *e);
//    void dropEvent(QDropEvent *e);

private:
    QMetaObject::Connection _portChangedCon;
    QMetaObject::Connection _createSnapBankCon;
    QMetaObject::Connection _createSnapBankFromSelCon;
    QMetaObject::Connection _key_DelCon;
    QPoint _startPos;
    static QTreeWidgetItem *AddTreeRoot(QString name);
    static ToolKitTreeItem *AddToolKitTreeItem(QString name, ConfigItem *cfgItem, int type);
    static QTreeWidgetItem *AddTreeNode(QTreeWidgetItem *parent, DevType type, QString typeName, int subType = 0);
    static QTreeWidgetItem *AddTreeNode(QTreeWidgetItem *parent, CfgType type, int subType = 0);
    static QTreeWidgetItem *AddTreeNode(QTreeWidgetItem *parent, QString modelName, CfgType type, ConfigItem* pItem);
    QUndoStack *_undoStack;

    QList<QString> _closeNodes;
    TreeType _treeType;
};

#endif // TOOLKITTREE_H
