#ifndef TOOLKITTREEITEM_H
#define TOOLKITTREEITEM_H

#include <QWidget>
#include <QTreeWidgetItem>

class ConfigItem;
class ToolKitTreeItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    enum ToolKitTreeItemType {
       SnapType_Bank = 1340,
       SnapType_controller,
       SnapType_ItemPtr,
       UciType_CfgUCI,
       UciType_CfgUciCtrl,
       UciType_Proc,
    };
    ToolKitTreeItem(const QStringList &strings, ConfigItem *cfgItem, int type = SnapType_Bank);

    inline ConfigItem *getcfgItem() { return _cfgItem; }
//    void setEditable(bool b);

    void addController(QTreeWidgetItem *treeWdgItem);
    void delController(QTreeWidgetItem *treeWdgItem);
signals:

public slots:
    void onUCiDevStatusChangedSlot();

private:
    void addChild_SnapBank();
    void addChild_Controllers();
    void addChild_uciCtrlItems();
private:

//    QWidget *_widget;
    ConfigItem *_cfgItem;
};




#endif // TOOLKITTREEITEM_H
