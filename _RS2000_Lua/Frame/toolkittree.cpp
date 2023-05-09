#include "stable.h"
#include "toolkittree.h"
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QDebug>
#include "config.h"
#include "configItem/configitem.h"
#include "customWidget/toolkittreeitem.h"
#include <QMap>
#include "mainwindow.h"
#include "config.h"
#include "device/device_trn.h"

//#include <QGraphicsItem>
ToolKitTree::ToolKitTree(TreeType type, QWidget *parent) :
    QTreeWidget(parent), _undoStack(new QUndoStack(this)), _treeType(type)
{
    setDragDropMode(QAbstractItemView::DragOnly);
    setHeaderHidden(true);
}

ToolKitTree::ToolKitTree(QWidget *parent) :
    QTreeWidget(parent), _undoStack(new QUndoStack(this)), _treeType(TreeStrType_default)
{
    setDragDropMode(QAbstractItemView::DragOnly);
    setHeaderHidden(true);
}

void ToolKitTree::HideDockWidget()
{
    parentWidget()->hide();
}

void ToolKitTree::SetTreeItems(QList<QTreeWidgetItem *> items, ConfigScene *pScene)
{
    clear();
    if(items.count() == 0)
    {
       parentWidget()->hide();
    }
    else
    {
        addTopLevelItems(items);
        LoadingCollapsedNodes(items);//hrp
        addSnapshotBankBtn(items, pScene);//hrp
        addUserControlInterface(items, pScene);//hrp
    }

    if(pScene != NULL) //注册属性改变事件
    {
        disconnect(_portChangedCon);
        _portChangedCon = connect(pScene, SIGNAL(designItemChanged(AbsGraphScene*)),
                    this, SLOT(portChanged(AbsGraphScene*)));
        disconnect(_key_DelCon);
        _key_DelCon = connect(this, QOverload<Qt::Key, QTreeWidgetItem *>::of(&ToolKitTree::keyEventSignals),
                              pScene, &ConfigScene::onTreeWidgetKey);
    }
}

void ToolKitTree::portChanged(AbsGraphScene *pScene)
{
    ConfigScene* pCfgScene = qobject_cast<ConfigScene*>(pScene);
    int totalPortNum = Utils::getDevTatalPortNum(pCfgScene->getTrnDev()->getDeviceType());
    PlatformType pType = pCfgScene->getTrnDev()->getPlatformType();
//    SetTreeItems(GetConfigLeftTree(pCfgScene), pCfgScene);
    switch(_treeType) {
    case TreeStrType_DevInterface:
        SetTreeItems(GetInOutItems(pCfgScene), pCfgScene);
        break;
    case TreeStrType_component:
    {
        if(pType == Platform_DSP)
            SetTreeItems(GetModelItems_Dsp(totalPortNum, pCfgScene), pCfgScene);
        else if(pType == Platform_X86 || pType == Platform_ARM)
            SetTreeItems(GetModelItems_X86(totalPortNum, pCfgScene), pCfgScene);
        break;
    }
    default:
        break;
    }

}

void ToolKitTree::onCloseNodesSlots(QTreeWidgetItem *item)
{
    _closeNodes.append(item->text(0));
}

void ToolKitTree::onOpenNodesSlots(QTreeWidgetItem *item)
{
    _closeNodes.removeAll(item->text(0));
}

void ToolKitTree::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        _startPos = e->pos();
    }
    QTreeWidget::mousePressEvent(e);
}

void ToolKitTree::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        int distance = (e->pos() - _startPos).manhattanLength();
        if(distance >= QApplication::startDragDistance())   //当拖动距离大于一个推荐抖动距离时，表示同意已经拖动操作了
        {
            performDrag();
        }
    }
//    QTreeWidget::mouseMoveEvent(e);
}


void ToolKitTree::performDrag()
{
    QList<QString> typeList;
    QList<QString> ptrList;
    QList<QTreeWidgetItem*> items = selectedItems();
    for(QTreeWidgetItem *item : items)
    {
        if(item && item->childCount() == 0)     //必须是非顶层item才可以移动数据
        {
            typeList.append( item->data(0, DataKey_Type).toString());
            ptrList.append(item->data(0, DataKey_ItemPtr).toString());
        }
    }
    if(typeList.count() > 0)
    {
        QMimeData *mineData = new QMimeData;
        mineData->setText(QString("Count:%1").arg(typeList.count()));
        QString typeListStr =typeList.join(",");
        QString ptrListStr = ptrList.join(",");
        mineData->setData("type", typeListStr.toUtf8());
        mineData->setData("itemPtr", ptrListStr.toUtf8());
        QDrag *drag = new QDrag(this);      //QDrag类支基于mime的拖放数据传输
        drag->setMimeData(mineData);
        drag->exec(Qt::MoveAction);
    }
}

void ToolKitTree::addSnapshotBankBtn(QList<QTreeWidgetItem *> items, ConfigScene *pScene)//hrp   --add new snapshot bank
{
    if(!pScene) return ;
    this->setColumnCount(2);
    this->setColumnWidth(0, this->width() - 30);
    this->setColumnWidth(1, 5);
    foreach(QTreeWidgetItem *item, items) {
        if(item->text(0) == tr("Snapshots Models") || item->text(0) == "快照保存") {
            QPushButton *btn = new QPushButton();
            btn->setObjectName("addBtn");
            btn->setAutoFillBackground(true);
            setItemWidget(item, 1, btn);
            connect(btn, &QPushButton::clicked, this, [=](){
                QMenu snapMenu;
                QAction *createSnapBank = new QAction(tr("Create new Snapshot Bank"));
                QAction *createSnapBankFromSelect = new QAction(tr("Create new Snapshot Bank form selection"));
                snapMenu.addAction(createSnapBank);
                snapMenu.addAction(createSnapBankFromSelect);

                createSnapBank->setVisible(false); //hrp  功能暂不支持
                if(pScene->selectedItems().isEmpty())
                    createSnapBankFromSelect->setDisabled(true);
                else
                    createSnapBankFromSelect->setDisabled(false);

                disconnect(_createSnapBankCon);
                disconnect(_createSnapBankFromSelCon);
//                disconnect(_key_DelCon);
                _createSnapBankCon = connect(createSnapBank, &QAction::triggered, pScene, &ConfigScene::onActionTriggeredSlots);
                _createSnapBankFromSelCon = connect(createSnapBankFromSelect, &QAction::triggered, pScene, &ConfigScene::onActionTriggeredSlots);
//                _key_DelCon = connect(this, QOverload<Qt::Key, QTreeWidgetItem *>::of(&ToolKitTree::keyEventSignals), pScene, &ConfigScene::onTreeWidgetKey);
                snapMenu.exec(btn->mapToGlobal(QPoint(0, 16+1)));
            });
        }
    }
}

void ToolKitTree::addUserControlInterface(QList<QTreeWidgetItem *> items, ConfigScene *pScene)
{
    QList<QTreeWidgetItem *> uciItems = findItems(tr("User Control Interfaces"), Qt::MatchCaseSensitive, 0);
    if(uciItems.isEmpty()) return ;
    QPushButton *btn = new QPushButton();
    btn->setObjectName("addBtn");
    btn->setAutoFillBackground(true);
    setItemWidget(uciItems.at(0), 1, btn);
    connect(btn, &QPushButton::clicked, this, [=](){
        pScene->addUserControlInterfaceItem();
    });
}

void ToolKitTree::resizeEvent(QResizeEvent *event) //hrp
{
    this->setColumnWidth(0, event->size().width() - 30);
    this->setColumnWidth(1, 16);
}

void ToolKitTree::keyPressEvent(QKeyEvent *event)
{
    QTreeWidgetItem *curItem = this->currentItem();
    if(curItem == NULL) return;
    switch (event->key()) {
    case Qt::Key_Delete:
    {
        if(curItem->type() == ToolKitTreeItem::SnapType_Bank)
        {
            ToolKitTreeItem *snapBank = static_cast<ToolKitTreeItem *>(curItem);
            if(!snapBank) break;
            emit keyEventSignals(Qt::Key_Delete, snapBank);
        }else if(curItem->type() == ToolKitTreeItem::SnapType_controller) {
//            qDebug() << __FUNCTION__ << __LINE__ << curItem->text(0) << curItem->parent()->text(0);
            ToolKitTreeItem *snapBank = static_cast<ToolKitTreeItem *>(curItem->parent());
            if(snapBank)
                emit keyEventSignals(Qt::Key_Delete, curItem);
        }else if(curItem->type() == ToolKitTreeItem::UciType_CfgUCI) {
            emit keyEventSignals(Qt::Key_Delete, curItem);
        }
        break;
    }
    default:
        break;
    }
    QTreeWidget::keyPressEvent(event);
}

void ToolKitTree::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(this->currentItem() && this->currentItem()->type() == ToolKitTreeItem::UciType_CfgUCI) {
        ToolKitTreeItem *uciTreeItem = static_cast<ToolKitTreeItem *>(this->currentItem());
        MainWindow::instance()->setCurrentSubWindow_UCI(uciTreeItem->getcfgItem(), uciTreeItem->getcfgItem()->customName());
    }else {
        QTreeWidget::mouseDoubleClickEvent(e);
    }
}

QTreeWidgetItem *ToolKitTree::AddTreeRoot(QString name)
{
    QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << name);
    return item;
}

ToolKitTreeItem *ToolKitTree::AddToolKitTreeItem(QString name, ConfigItem *cfgItem, int type)
{
    ToolKitTreeItem * item = new ToolKitTreeItem(QStringList() << name, cfgItem, type);
    return item;
}

QTreeWidgetItem *ToolKitTree::AddTreeNode(QTreeWidgetItem *parent, DevType type, QString typeName, int subType)
{
    QTreeWidgetItem * item=new QTreeWidgetItem(QStringList() << typeName);
    item->setData(0, DataKey_Type, QVariant(type));
    item->setData(0, DataKey_SubType, QVariant(subType));
    item->setData(0, DataKey_ItemPtr, QVariant(QMetaType::ULongLong, 0));
    parent->addChild(item);
    return item;
}

QTreeWidgetItem *ToolKitTree::AddTreeNode(QTreeWidgetItem *parent, CfgType type, int subType)
{
    Utils::setMaxMasterVersion(type);
    QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << Config::curMfrsCfg().getCfgModelName(type));
    item->setData(0, DataKey_Type, QVariant(type));
    item->setData(0, DataKey_SubType, QVariant(subType));
    item->setData(0, DataKey_ItemPtr, QVariant(QMetaType::ULongLong, 0));
    parent->addChild(item);
    return item;
}

QTreeWidgetItem *ToolKitTree::AddTreeNode(QTreeWidgetItem *parent, QString modelName, CfgType type, ConfigItem* pItem)
{
    QIcon iconEnable, iconDisable;
    QTreeWidgetItem * item=new QTreeWidgetItem(QStringList() << modelName);
    item->setData(0, DataKey_Type, QVariant(type));
    item->setData(0, DataKey_SubType, QVariant(0));
    qulonglong longPtr = (qulonglong)pItem;
    item->setData(0, DataKey_ItemPtr, QVariant(longPtr));


    QString iconDir = Config::curSkinStyle()->getSkinIconDir();
    CfgType groupFirst =(CfgType)(type& 0xFF00);
    if(groupFirst == CFG_TYPE_INPUT_1 || groupFirst == CFG_TYPE_OUTPUT_1 ||
       groupFirst == CFG_TYPE_CTRL_LUA)
    {
        iconEnable.addPixmap(QPixmap(QString(":/Image/%1/portEnable.png").arg(iconDir)));
        iconDisable.addPixmap(QPixmap(QString(":/Image/%1/portDisable.png").arg(iconDir)));
        if(pItem->scene() == NULL)  item->setIcon(0, iconEnable);
        else item->setIcon(0, iconDisable);
    }
    else if(type == CFG_TYPE_SNAPSHOT || type == CFG_TYPE_SNAPSHOT_BANK) {
        iconEnable.addPixmap(QPixmap(QString(":/Image/%1/snapshotEnable.png").arg(iconDir)));
        iconDisable.addPixmap(QPixmap(QString(":/Image/%1/snapshotDisable.png").arg(iconDir)));
        if(pItem->scene() == NULL)  item->setIcon(0, iconEnable);
        else item->setIcon(0, iconDisable);
    }

    parent->addChild(item);
    return item;
}


QList<QTreeWidgetItem *> ToolKitTree::GetDeviceItems( )
{
    QList<MfrsDev> devList =  Config::curMfrsCfg().MfrsDevList;
    QList<QTreeWidgetItem *> items;
    QMap<QString, QTreeWidgetItem*> groupNodeMap;
    QSet<QString> groupSet;

    QTreeWidgetItem* extDevice = AddTreeRoot(tr("External Device"));
    for(MfrsDev dev : devList){
        QString devGroupName = dev.GroupNameEn;
        QString devTypeName = dev.TypeNameEn;
        if(Config::instance()->getLanguage() == LANGUAGE_SIMPLIED_CHINESE)
        {
            devGroupName = dev.GroupNameChs;
            devTypeName = dev.TypeNameChs;
        }
        if(dev.IsNet){
            QList<QString> groupNames = devGroupName.split("\\");
//            qDebug() << __FUNCTION__ << __LINE__ << groupNames;
            QTreeWidgetItem *child;
            if(!groupNodeMap.contains(groupNames.first())) {
                child = AddTreeRoot(groupNames.first());
                items.append(child);
                groupNodeMap[groupNames.first()] = child;
            }else {
                child = groupNodeMap[groupNames.first()];
            }
            QTreeWidgetItem *item;
            for(int i = 0; i < groupNames.length(); i++) {
                if(groupNames.length() > 1 && i > 0)
                {
                    item = getTreeItemName(child, groupNames[i]);
                    if(item == NULL) {
                        item = AddTreeRoot(groupNames[i]);
                        child->addChild(item);
                        child = item;
                    }
                    if(groupNames[i] == groupNames.last())
                        AddTreeNode(item, dev.Type, devTypeName);
                }
                if(groupNames.length() == 1) {
                    AddTreeNode(child, dev.Type, devTypeName);
                }
            }

//            if(!groupNodeMap.contains(devGroupName)){
//                QTreeWidgetItem *child = AddTreeRoot(devGroupName);
//                items.append(child);

//                groupNodeMap[devGroupName] = child;
//            }
//            AddTreeNode(groupNodeMap[devGroupName], dev.Type, devTypeName);
        }
        else{
            AddTreeNode(extDevice, dev.Type, devTypeName);
        }
    }
    if(extDevice->childCount() > 0){
        items.append(extDevice);
    }

    return items;
}


QList<QTreeWidgetItem *> ToolKitTree::GetModelItems_Dsp(int totalPortNum, ConfigScene *pScene)
{
    QList<QTreeWidgetItem *> itemList;
    QTreeWidgetItem* audioCompGroup = AddTreeRoot(tr("Audio Components"));
    itemList.append(audioCompGroup);
    QTreeWidgetItem* dynamicsGroup = AddTreeRoot(tr("Dynamics"));
    itemList.last()->addChild(dynamicsGroup);
    AddTreeNode(dynamicsGroup, CFG_TYPE_DYN_AUTO);
    AddTreeNode(dynamicsGroup, CFG_TYPE_DYN_COMP);
    AddTreeNode(dynamicsGroup, CFG_TYPE_DYN_EXPAND);
    AddTreeNode(dynamicsGroup, CFG_TYPE_DYN_NOISE);
    AddTreeNode(dynamicsGroup, CFG_TYPE_DYN_LIMIT);
    AddTreeNode(dynamicsGroup, CFG_TYPE_PDUCKER_4);

    QTreeWidgetItem *eqFilterGroup = AddTreeRoot(tr("Equalizers and Filters"));
    itemList.last()->addChild(eqFilterGroup);
    AddTreeNode(eqFilterGroup, CFG_TYPE_EQ_3);
    AddTreeNode(eqFilterGroup, CFG_TYPE_EQ_1X3);
    AddTreeNode(eqFilterGroup, CFG_TYPE_SPLIT_2);
    AddTreeNode(eqFilterGroup, CFG_TYPE_FILTE_HP);
    AddTreeNode(eqFilterGroup, CFG_TYPE_FILTE_LP);
    AddTreeNode(eqFilterGroup, CFG_TYPE_FILTER_BP);
    AddTreeNode(eqFilterGroup, CFG_TYPE_FILTER_AP);
    AddTreeNode(eqFilterGroup, CFG_TYPE_SOFTFIR_MONO256);

    QTreeWidgetItem *AFCGroup = AddTreeRoot(tr("Automatic Feedback Controllers"));
    itemList.last()->addChild(AFCGroup);
    AddTreeNode(AFCGroup, CFG_TYPE_FREQSHIFT);
    AddTreeNode(AFCGroup, CFG_TYPE_NFC1_8NF);

    QTreeWidgetItem *mixersGroup = AddTreeRoot(tr("Mixers"));
    itemList.last()->addChild(mixersGroup);
    AddTreeNode(mixersGroup, CFG_TYPE_MIXER_4X1);
    AddTreeNode(mixersGroup, CFG_TYPE_MIXER_8X1);
    AddTreeNode(mixersGroup, CFG_TYPE_MIXER_16X16);
    if(totalPortNum >= 32) AddTreeNode(mixersGroup, CFG_TYPE_MIXER_32X32);
    if(totalPortNum >= 64) AddTreeNode(mixersGroup, CFG_TYPE_MIXER_64X64);
    AddTreeNode(mixersGroup, CFG_TYPE_AUTOMIXNGAT_4);

    QTreeWidgetItem *routersGroup = AddTreeRoot(tr("Routers"));
    itemList.last()->addChild(routersGroup);
    AddTreeNode(routersGroup, CFG_TYPE_ROUTER_4);
    AddTreeNode(routersGroup, CFG_TYPE_ROUTER_8);
    AddTreeNode(routersGroup, CFG_TYPE_ROUTER_16);
//    AddTreeNode(RoutersItem, CFG_TYPE_ROUTER_32);
//    AddTreeNode(RoutersItem, CFG_TYPE_ROUTER_64);

    AddTreeNode(audioCompGroup, CFG_TYPE_GAIN_1);

    AddTreeNode(audioCompGroup, CFG_TYPE_GAINRAMP_1);

    QTreeWidgetItem *delaysGroup = AddTreeRoot(tr("Delays"));
    itemList.last()->addChild(delaysGroup);
    AddTreeNode(delaysGroup, CFG_TYPE_HPDELY_1);
    AddTreeNode(delaysGroup, CFG_TYPE_DELAY_100);
    AddTreeNode(delaysGroup, CFG_TYPE_DELAY_1000);

    QTreeWidgetItem *aecGroup = AddTreeRoot(tr("AEC"));
    audioCompGroup->addChild(aecGroup);
    AddTreeNode(aecGroup, CFG_TYPE_AEC_2_2);
    if(Config::curMfrsCfg().CurMfrs != Mfrs_FangTu)
        AddTreeNode(aecGroup, CFG_TYPE_SOFTAEC_8K128);

    QTreeWidgetItem *generatorsGroup = AddTreeRoot(tr("Generators"));
    itemList.last()->addChild(generatorsGroup);
    AddTreeNode(generatorsGroup, CFG_TYPE_SIG_SINE);
    AddTreeNode(generatorsGroup, CFG_TYPE_SIG_PINKNOISE);
    AddTreeNode(generatorsGroup, CFG_TYPE_SIG_NOISE);

    QTreeWidgetItem *metersGroup = AddTreeRoot(tr("Meters"));
    itemList.last()->addChild(metersGroup);
    AddTreeNode(metersGroup, CFG_TYPE_METER_1);
    AddTreeNode(metersGroup, CFG_TYPE_METER_2);
    AddTreeNode(metersGroup, CFG_TYPE_METER_4);
    AddTreeNode(metersGroup, CFG_TYPE_METER_8);
    if(Config::curMfrsCfg().CurMfrs == Mfrs_BeiJingHuiSheng) {
        AddTreeNode(audioCompGroup, CFG_TYPE_VOICETUBE_1);
    }
    //消防强切
    AddTreeNode(audioCompGroup, CFG_TYPE_GPI);

    if(!pScene) return itemList;
    itemList.append(ToolKitTree::GetSnapshotItems(pScene));
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) {
        itemList.append(ToolKitTree::GetUserControlInterfacesItems(pScene));
    }

    return itemList;
}

QList<QTreeWidgetItem *> ToolKitTree::GetModelItems_X86(int totalPortNum, ConfigScene *pScene)
{
    QList<QTreeWidgetItem *> itemList;
    QTreeWidgetItem* audioCompGroup = AddTreeRoot(tr("Audio Components"));
    itemList.append(audioCompGroup);

    AddTreeNode(audioCompGroup, CFG_TYPE_AUDIO_PLAYER2);
//    AddTreeNode(audioCompItem, CFG_TYPE_AUDIO_RECORD2);
    QTreeWidgetItem* dynamicsGroup = AddTreeRoot(tr("Dynamics"));
    audioCompGroup->addChild(dynamicsGroup);
    AddTreeNode(dynamicsGroup, CFG_TYPE_DYN_AUTO);
    AddTreeNode(dynamicsGroup, CFG_TYPE_DYN_COMP);
    AddTreeNode(dynamicsGroup, CFG_TYPE_DYN_EXPAND);
    AddTreeNode(dynamicsGroup, CFG_TYPE_DYN_NOISE);
    AddTreeNode(dynamicsGroup, CFG_TYPE_DYN_LIMIT);
    AddTreeNode(dynamicsGroup, CFG_TYPE_PDUCKER_4);

    QTreeWidgetItem *eqFilterGroup = AddTreeRoot(tr("Equalizers and Filters"));
    audioCompGroup->addChild(eqFilterGroup);
    AddTreeNode(eqFilterGroup, CFG_TYPE_EQ_5);
    AddTreeNode(eqFilterGroup, CFG_TYPE_EQ_1X3);
    AddTreeNode(eqFilterGroup, CFG_TYPE_SPLIT_2);
    AddTreeNode(eqFilterGroup, CFG_TYPE_FILTE_HP);
    AddTreeNode(eqFilterGroup, CFG_TYPE_FILTE_LP);
    AddTreeNode(eqFilterGroup, CFG_TYPE_FILTER_BP);
    AddTreeNode(eqFilterGroup, CFG_TYPE_FILTER_AP);
    AddTreeNode(eqFilterGroup, CFG_TYPE_SOFTFIR_MONO256);

    QTreeWidgetItem *AFCGroup = AddTreeRoot(tr("Automatic Feedback Controllers"));
    audioCompGroup->addChild(AFCGroup);
    AddTreeNode(AFCGroup, CFG_TYPE_FREQSHIFT);
    AddTreeNode(AFCGroup, CFG_TYPE_NFC1_8NF);

    QTreeWidgetItem *mixersGroup = AddTreeRoot(tr("Mixers"));
    audioCompGroup->addChild(mixersGroup);
    AddTreeNode(mixersGroup, CFG_TYPE_MIXER_4X1);
    AddTreeNode(mixersGroup, CFG_TYPE_MIXER_8X1);
    AddTreeNode(mixersGroup, CFG_TYPE_MIXER_16X16);
    AddTreeNode(mixersGroup, CFG_TYPE_MIXER_32X32);
    AddTreeNode(mixersGroup, CFG_TYPE_MIXER_64X64);
    AddTreeNode(mixersGroup, CFG_TYPE_AUTOMIXNGAT_4);

    QTreeWidgetItem *routersGroup = AddTreeRoot(tr("Routers"));
    audioCompGroup->addChild(routersGroup);
    AddTreeNode(routersGroup, CFG_TYPE_ROUTER_4);
    AddTreeNode(routersGroup, CFG_TYPE_ROUTER_8);
    AddTreeNode(routersGroup, CFG_TYPE_ROUTER_16);
    AddTreeNode(routersGroup, CFG_TYPE_ROUTER_32);
    AddTreeNode(routersGroup, CFG_TYPE_ROUTER_64);
    if(totalPortNum >= 96) AddTreeNode(routersGroup, CFG_TYPE_ROUTER_96);
    if(totalPortNum >= 128) AddTreeNode(routersGroup, CFG_TYPE_ROUTER_128);
    if(totalPortNum >= 192) AddTreeNode(routersGroup, CFG_TYPE_ROUTER_192);
    if(totalPortNum >= 256) AddTreeNode(routersGroup, CFG_TYPE_ROUTER_256);

    AddTreeNode(audioCompGroup, CFG_TYPE_GAIN_1);

    AddTreeNode(audioCompGroup, CFG_TYPE_GAINRAMP_1);

    QTreeWidgetItem *delaysGroup = AddTreeRoot(tr("Delays"));
    audioCompGroup->addChild(delaysGroup);
    AddTreeNode(delaysGroup, CFG_TYPE_HPDELY_1);
    AddTreeNode(delaysGroup, CFG_TYPE_DELAY_100);
    AddTreeNode(delaysGroup, CFG_TYPE_DELAY_1000);

    QTreeWidgetItem *aecGroup = AddTreeRoot(tr("AEC"));
    audioCompGroup->addChild(aecGroup);
    //下载算法自动分配主核、独立核
    AddTreeNode(aecGroup, CFG_TYPE_SOFTAEC_8K128);
//    AddTreeNode(aecGroup, CFG_TYPE_SOFTAEC_IK_8K128);

    QTreeWidgetItem *generatorsGroup = AddTreeRoot(tr("Generators"));
    audioCompGroup->addChild(generatorsGroup);
    AddTreeNode(generatorsGroup, CFG_TYPE_SIG_SINE);
    AddTreeNode(generatorsGroup, CFG_TYPE_SIG_PINKNOISE);
    AddTreeNode(generatorsGroup, CFG_TYPE_SIG_NOISE);

    QTreeWidgetItem *metersGroup = AddTreeRoot(tr("Meters"));
    audioCompGroup->addChild(metersGroup);
    AddTreeNode(metersGroup, CFG_TYPE_METER_1);
    AddTreeNode(metersGroup, CFG_TYPE_METER_2);
    AddTreeNode(metersGroup, CFG_TYPE_METER_4);
    AddTreeNode(metersGroup, CFG_TYPE_METER_8);

    AddTreeNode(audioCompGroup, CFG_TYPE_GPI);
    AddTreeNode(audioCompGroup, CFG_TYPE_CROSSFADER_2X1);

    if(Config::curMfrsCfg().CurMfrs == Mfrs_BeiJingHuiSheng)
    {
        AddTreeNode(audioCompGroup, CFG_TYPE_VOICETUBE_2);
    }

    if(Config::isLuaSupported()){
        QTreeWidgetItem* luaCompGroup = AddTreeRoot(tr("Lua Components"));
        itemList.append(luaCompGroup);
        AddTreeNode(luaCompGroup, CFG_TYPE_CTRL_LUA);
    }

    if(!pScene) return itemList;
    itemList.append(ToolKitTree::GetSnapshotItems(pScene));

    return itemList;
}


QList<QTreeWidgetItem *> ToolKitTree::GetInOutItems(ConfigScene *pScene, bool groupby_Analog_Net)
{
    QList<QTreeWidgetItem *> itemList;
    if(pScene != NULL)
    {
        Trn_Interface *trnINT = pScene->getTrnInterface();
        QList<ConfigItem*> inModelList = trnINT->getInputModelList();
        QList<ConfigItem*> outModelList = trnINT->getOutputModelList();
        QList<ConfigItem*> controlModelList = trnINT->getControlModelList();
        if(groupby_Analog_Net)
        {
            QTreeWidgetItem* A_inModels = AddTreeRoot(tr("Analog Audio Input"));
            QTreeWidgetItem* N_inModels = AddTreeRoot(tr("Network Audio Input"));
            QTreeWidgetItem* A_outModels = AddTreeRoot(tr("Analog Audio Output"));
            QTreeWidgetItem* N_outModels = AddTreeRoot(tr("Network Audio Output"));
            QTreeWidgetItem* U_Models = AddTreeRoot(tr("USB Audio Channel"));
            for(ConfigItem* inItem : inModelList)
            {
                if(inItem->configTypeName().startsWith("A") ) {
                    AddTreeNode(A_inModels,inItem->configTypeName().split(" ")[0], inItem->getCfgType(), inItem);
                }
                else if(inItem->configTypeName().startsWith("N")) {
                    AddTreeNode(N_inModels,inItem->configTypeName().split(" ")[0], inItem->getCfgType(), inItem);
                }
                else if(inItem->configTypeName().startsWith("U")){
                    AddTreeNode(U_Models,inItem->configTypeName(), inItem->getCfgType(), inItem);
                }
            }

            for(ConfigItem* outItem : outModelList)
            {
                if(outItem->configTypeName().startsWith("A")) {
                    AddTreeNode(A_outModels,outItem->configTypeName().split(" ")[0], outItem->getCfgType(), outItem);
                }else if(outItem->configTypeName().startsWith("N")) {
                    AddTreeNode(N_outModels,outItem->configTypeName().split(" ")[0], outItem->getCfgType(), outItem);
                }
                else if(outItem->configTypeName().startsWith("U")){
                    AddTreeNode(U_Models,outItem->configTypeName(), outItem->getCfgType(), outItem);
                }
            }

            //显示有子节点的模块
            if(A_inModels->childCount()>0) itemList.append(A_inModels);
            if(N_inModels->childCount()>0) itemList.append(N_inModels);
            if(A_outModels->childCount()>0) itemList.append(A_outModels);
            if(N_outModels->childCount()>0) itemList.append(N_outModels);
            if(U_Models->childCount()>0)  itemList.append(U_Models);
            else delete U_Models;
        }
        else{
            QTreeWidgetItem* inModels = AddTreeRoot(tr("Input Models"));
            itemList.append(inModels);
            for(ConfigItem* inItem : inModelList)
            {
                AddTreeNode(inModels,inItem->configTypeName(), inItem->getCfgType(), inItem);
            }

            QTreeWidgetItem* outModels = AddTreeRoot(tr("Output Models"));
            itemList.append(outModels);
            for(ConfigItem* outItem : outModelList)
            {
                AddTreeNode(outModels,outItem->configTypeName(), outItem->getCfgType(), outItem);
            }
        }

        //显示控制模块
        if(controlModelList.count()> 0){
            QTreeWidgetItem* ctrlModels = AddTreeRoot(tr("Control Models"));
            itemList.append(ctrlModels);
            for(ConfigItem* ctrlItem : controlModelList)
            {
                AddTreeNode(ctrlModels, ctrlItem->configTypeName(), ctrlItem->getCfgType(), ctrlItem);
            }
        }
    }
    return itemList;
}



QList<QTreeWidgetItem *> ToolKitTree::GetGuiItems()
{
    QList<QTreeWidgetItem *> itemList;
    QTreeWidgetItem* dispItem = AddTreeRoot(tr("Display"));
    itemList.append(dispItem);
    AddTreeNode(dispItem, CFG_TYPE_1);
    AddTreeNode(dispItem, CFG_TYPE_4);
    AddTreeNode(dispItem, CFG_TYPE_5);
    QTreeWidgetItem* ctrlItem = AddTreeRoot(tr("Control"));
    itemList.append(ctrlItem);
    AddTreeNode(ctrlItem, CFG_TYPE_2);
    AddTreeNode(ctrlItem, CFG_TYPE_3);
    //    _Monitor.append(AddTreeRoot(tr("Scene")));
    //    AddTreeNode(_Monitor.last(), CFG_TYPE_SCENE);
    return itemList;
}




QList<QTreeWidgetItem *> ToolKitTree::GetSnapshotItems(ConfigScene *pScene) // hrp   ---add snapshot
{
    QList<QTreeWidgetItem *> itemList;
    if(pScene != NULL) {
        QList<ConfigItem *> snapshotList = pScene->getTrnInterface()->getSnapshotList();
        QTreeWidgetItem* SnapshotsModels = AddTreeRoot(tr("Snapshots Models"));
        itemList.append(SnapshotsModels);

        QMap<QString, ConfigItem *> sortList;
        sortList.clear();
        foreach (ConfigItem *item, snapshotList) {
            if(item->getCfgType() == CFG_TYPE_SNAPSHOT) {
                QTreeWidgetItem* Global = AddTreeRoot(tr("Global( %1 )").arg(item->customName()));
#if Logo_FangTu
                Global->setIcon(0, QIcon(":/Image/Camera_16x.png"));
#else
                Global->setIcon(0, QIcon(":/Image/global.png"));
#endif
                itemList.last()->addChild(Global);
                AddTreeNode(Global,item->configTypeName(), CFG_TYPE_SNAPSHOT, item);
            }else if(item->getCfgType() == CFG_TYPE_SNAPSHOT_BANK) {
                sortList.insert(item->customName(), item);
            }
        }
        QMapIterator<QString, ConfigItem *> i(sortList);
        while(i.hasNext()) {
            i.next();
            ToolKitTreeItem* snapshotBank = AddToolKitTreeItem(QString("Snapshot Bank( %1 )").arg(i.key()), i.value(), ToolKitTreeItem::SnapType_Bank);
#if Logo_FangTu
                snapshotBank->setIcon(0, QIcon(":/Image/Camera_16x.png"));
#else
                snapshotBank->setIcon(0, QIcon(":/Image/global.png"));
#endif
            itemList.last()->addChild(snapshotBank);
        }
    }
//#endif
    return itemList;
}

QList<QTreeWidgetItem *> ToolKitTree::GetUserControlInterfacesItems(ConfigScene *pScene)
{
    QList<QTreeWidgetItem *> itemList;
    QTreeWidgetItem *uciItem = AddTreeRoot(tr("User Control Interfaces"));
    itemList.append(uciItem);
    foreach(ConfigItem *uciItem, pScene->getUciList()) {
        ToolKitTreeItem *tktItem = AddToolKitTreeItem(uciItem->customName(), uciItem, ToolKitTreeItem::UciType_CfgUCI);
        itemList.last()->addChild(tktItem);
    }
    return itemList;
}

//QList<QTreeWidgetItem *> ToolKitTree::GetConfigLeftTree(ConfigScene *pScene)// hrp
//{
//    QList<QTreeWidgetItem *> list;
//    list.append(GetInOutItems(pScene));
//    list.append(GetSnapshotItems(pScene));
//    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) {
//        list.append(GetUserControlInterfacesItems(pScene));
//    }
//    return list;
//}

#include "configItem/configitemuci.h"
QList<QTreeWidgetItem *> ToolKitTree::GetUciLoadProcItems(QHash<QString, ConfigItemUCI *> list)
{
    QList<QTreeWidgetItem *> itemList;
    QHashIterator<QString, ConfigItemUCI *> i(list);
    while(i.hasNext()) {
        i.next();
        ToolKitTreeItem *procItem = AddToolKitTreeItem("", i.value(), ToolKitTreeItem::UciType_Proc);
        itemList.append(procItem);
    }
    return itemList;
}

void ToolKitTree::LoadingCollapsedNodes(QList<QTreeWidgetItem *> items)
{
    disconnect(SIGNAL(itemExpanded(QTreeWidgetItem *)));
    disconnect(SIGNAL(itemCollapsed(QTreeWidgetItem *)));
    expandAll();
    if(!_closeNodes.isEmpty()) {
        foreach(QString text, _closeNodes) {
            foreach(QTreeWidgetItem *treeItem, items) {
                QTreeWidgetItem *tWItem = getTreeItemName(treeItem, text);
                if(tWItem == NULL) continue;
                QModelIndex index = indexAt(visualItemRect(tWItem).center());
                setExpanded(index, false);
                break;
            }
        }
    }
    connect(this, &ToolKitTree::itemExpanded, this, &ToolKitTree::onOpenNodesSlots);
    connect(this, &ToolKitTree::itemCollapsed, this, &ToolKitTree::onCloseNodesSlots);
}

QTreeWidgetItem *ToolKitTree::getTreeItemName(QTreeWidgetItem *treeWidItem, QString text)
{
    if(treeWidItem->text(0) == text) {
        return treeWidItem;
    }
    else if(treeWidItem->childCount() > 0){
        for(int idx = 0; idx < treeWidItem->childCount(); idx++) {
            if(treeWidItem->child(idx)->text(0) == text)
                return treeWidItem->child(idx);
            getTreeItemName(treeWidItem->child(idx), text);
        }
    }
    return NULL;
}


//==============拖拽改变顺序==============//
//void ToolKitTree::dragEnterEvent(QDragEnterEvent *e)
//{
//    ToolKitTree *source = qobject_cast<ToolKitTree *>(e->source());
//    if(source)
//    {
//        e->setDropAction(Qt::MoveAction);
//        e->accept();
//    }
//}

//void ToolKitTree::dragMoveEvent(QDragMoveEvent *e)
//{
//    ToolKitTree *source = qobject_cast<ToolKitTree *>(e->source());
//    if(source)
//    {
//        e->setDropAction(Qt::MoveAction);
//        e->accept();
//    }
//}

//void ToolKitTree::dropEvent(QDropEvent *e)
//{
//    ToolKitTree *source = qobject_cast<ToolKitTree *>(e->source());
//    if(source)
//    {
//        QTreeWidgetItem *item = this->itemAt(e->pos()); //当前位置的item
//        if( item == nullptr)   //如果放下的位置没有item，则退出，没有这句话死机！
//            return;
//        if( -1 == this->indexOfTopLevelItem(item) && (-1 == this->indexOfTopLevelItem(currentItem())) ||
//            -1 != this->indexOfTopLevelItem(item) && (-1 != this->indexOfTopLevelItem(currentItem()))
//           )  //如果“放下位置的item是顶层item，且原来的是顶层”或者“放下的不是顶层，且原来也不是顶层”
//        {
//            item->setText(currentColumn(),e->mimeData()->text());
//            e->setDropAction(Qt::MoveAction);
//            e->accept();
//        }
//    }
//}
