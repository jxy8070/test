#include "toolkittreeitem.h"
#include "configItem/configitem.h"
#include "configItem/configitemsnapshot.h"
#include "configItem/configitemuci.h"
#include "device/device_trn.h"

ToolKitTreeItem::ToolKitTreeItem(const QStringList &strings, ConfigItem *cfgItem, int type)
    : QTreeWidgetItem(strings, type),
      _cfgItem(cfgItem)
{
    switch(this->type()) {
    case SnapType_Bank:
        addChild_SnapBank();
       if(_cfgItem->getCfgType() == CFG_TYPE_SNAPSHOT || _cfgItem->getCfgType() == CFG_TYPE_SNAPSHOT_BANK)
       {
           ConfigItemSnapshot *snapshot = qgraphicsitem_cast<ConfigItemSnapshot *>(_cfgItem);
           if(snapshot->getCreateType() == ConfigItemSnapshot::Create_snap_Bank_FromSel)
               addChild_Controllers();
       }
        break;
    case UciType_CfgUCI:
        if(_cfgItem->getCfgType() == CFG_TYPE_UCI) {
//            addChild_uciCtrlItems();
        }
        break;
    case UciType_Proc:
        if(_cfgItem->getCfgType() == CFG_TYPE_UCI) {
            ConfigItemUCI *uciItem = qgraphicsitem_cast<ConfigItemUCI *>(_cfgItem);
            QString AES70DevStatus = tr("Offline");
            if(uciItem->getAES70Dev()) {
                DevStatus devState = uciItem->getAES70Dev()->getStatus();
                AES70DevStatus = AES70Device::getStatusName(devState);
            }
            qDebug() << __FUNCTION__ << __LINE__ << AES70DevStatus;
            QString title;
//            title += uciItem->getLoadUciFileProcData().s_type + "\n";
            title += uciItem->getLoadUciFileProcData().s_ip + "\n";
            title += uciItem->getLoadUciFileProcData().s_customName + " ";
            title += "(" + AES70DevStatus + ")";
            setText(0, title);
            break;
        }
    default:
        break;
    }
}

void ToolKitTreeItem::addController(QTreeWidgetItem *treeWdgItem)
{
    if(_cfgItem->getCfgType() == CFG_TYPE_SNAPSHOT || _cfgItem->getCfgType() == CFG_TYPE_SNAPSHOT_BANK)
    {
        ConfigItemSnapshot *snapshot = qgraphicsitem_cast<ConfigItemSnapshot *>(_cfgItem);
        ConfigItem *cfgItem = (ConfigItem *)treeWdgItem->data(0, SnapType_ItemPtr).toULongLong();
        if(!cfgItem) return ;
//        qDebug() << __FUNCTION__ << __LINE__ << cfgItem->customName();
        snapshot->insertFromSel_CtrlsList(cfgItem);
//        qDebug() << __FUNCTION__ << __LINE__ << cfgItem->customName();
#if Logo_FangTu
    treeWdgItem->setForeground(0, QColor(255, 255, 255));
#endif
        this->addChild(treeWdgItem);
    }
}

void ToolKitTreeItem::delController(QTreeWidgetItem *treeWdgItem)
{
    if(_cfgItem->getCfgType() == CFG_TYPE_SNAPSHOT || _cfgItem->getCfgType() == CFG_TYPE_SNAPSHOT_BANK)
    {
        ConfigItemSnapshot *snapshot = qgraphicsitem_cast<ConfigItemSnapshot *>(_cfgItem);
        ConfigItem *cfgItem = (ConfigItem *)treeWdgItem->data(0, SnapType_ItemPtr).toULongLong();
        if(!cfgItem) return ;
//        qDebug() << __FUNCTION__ << __LINE__ << cfgItem->customName();
        snapshot->removeFromSel_CtrlsList(cfgItem);
        this->removeChild(treeWdgItem);
    }
}

void ToolKitTreeItem::onUCiDevStatusChangedSlot()
{
    QString AES70DevStatus;
    ConfigItemUCI *uciItem = qgraphicsitem_cast<ConfigItemUCI *>(_cfgItem);
    if(uciItem->getAES70Dev() != NULL)
    {
        DevStatus devState = uciItem->getAES70Dev()->getStatus();
        AES70DevStatus = AES70Device::getStatusName(devState);
    }

    qDebug() << __FUNCTION__ << __LINE__ << AES70DevStatus;
    QString title;
//            title += uciItem->getLoadUciFileProcData().s_type + "\n";
    title += uciItem->getLoadUciFileProcData().s_ip + "\n";
    title += uciItem->getLoadUciFileProcData().s_customName + "  ";
    title += AES70DevStatus;
    setText(0, title);
}

void ToolKitTreeItem::addChild_SnapBank()
{
    QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << _cfgItem->configTypeName());
    item->setData(0, DataKey_Type, QVariant(_cfgItem->getCfgType()));
    item->setData(0, DataKey_SubType, QVariant(0));
    qulonglong longPtr = (qulonglong)_cfgItem;
    item->setData(0, DataKey_ItemPtr, QVariant(longPtr));

    QString iconDir = Config::curSkinStyle()->getSkinIconDir();
    if(_cfgItem->getCfgType() == CFG_TYPE_SNAPSHOT || _cfgItem->getCfgType() == CFG_TYPE_SNAPSHOT_BANK) {
        if(_cfgItem->scene() == NULL)  item->setIcon(0, QIcon(QString(":/Image/%1/snapshotEnable.png").arg(iconDir)));
        else item->setIcon(0, QIcon(QString(":/Image/%1/snapshotDisable.png").arg(iconDir)));
    }

    this->addChild(item);
}

void ToolKitTreeItem::addChild_Controllers()
{
    ConfigItemSnapshot *snapshot = qgraphicsitem_cast<ConfigItemSnapshot *>(_cfgItem);
    QSetIterator<ConfigItem *> i(snapshot->getFromSel_CtrlsList());
    while(i.hasNext()){
        ConfigItem *ctrlItem = i.next();
        if(!ctrlItem) continue;
//        qDebug() << __FUNCTION__ << __LINE__ << snapItem->configTypeName() << snapItem->customName();
        QString name = QString("%1( %2 )All Controls").arg(ctrlItem->configTypeName()).arg(ctrlItem->customName());
        name.remove("\n");
        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << name, SnapType_controller);
        qulonglong longPtr = (qulonglong)ctrlItem;
        item->setData(0, SnapType_ItemPtr, QVariant(longPtr));

        item->setFont(0, SkinStyle::getFont(10));

        QString iconDir = Config::curSkinStyle()->getSkinIconDir();
        if(ctrlItem->scene() != NULL) {
#if Logo_FangTu
            item->setForeground(0, QBrush(Qt::white));
#else
            item->setForeground(0, QBrush(Qt::black));
#endif
            item->setIcon(0, QIcon(QString(":/Image/%1/ctrlsEnable.png").arg(iconDir)));
        } else {
            item->setForeground(0, QBrush(Qt::red));
            item->setIcon(0, QIcon(QString(":/Image/%1/ctrlsDisable.png").arg(iconDir)));
        }

        this->addChild(item);
    }

}

void ToolKitTreeItem::addChild_uciCtrlItems()
{
//    ConfigItemUCI *uci = qgraphicsitem_cast<ConfigItemUCI *>(_cfgItem);
//    QSetIterator<ConfigItem *> i(uci->getUciCtrlItemList());
//    while(i.hasNext()) {
//        ConfigItem *uciCtrlItem = i.next();
//        if(!uciCtrlItem) continue;
//        QString name = QString("%1( %2 )").arg(uciCtrlItem->configTypeName()).arg(uciCtrlItem->customName());
//        name.remove("\n");
//        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << name, UciType_CfgUciCtrl);
//        item->setForeground(0, QBrush(Qt::white));
//        this->addChild(item);
//    }
}

