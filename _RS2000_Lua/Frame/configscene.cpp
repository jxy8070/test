#include "stable.h"
#include <QInputDialog>
#include "configscene.h"
#include "configview.h"
#include "sceneresizebox.h"
#include "deviceItem/deviceitem_trn.h"
#include "device/device_trn.h"
#include "configItem/configitem.h"
#include "LineItem.h"
#include "ItemPort.h"
#include "customWidget/eqgraph.h"
#include "mainwindow.h"
#include "deviceSetting/trn/trn_scenewidget.h"
#include "configItem/configitemfactory.h"
#include "configItem/configiteminput.h"
#include "configItem/configitemoutput.h"
#include "configItem/configitemsnapshot.h"
#include "configItem/configitemuci.h"
#include "configItem/configitemmixer.h"
#include "configItem/configitemmeter.h"
#include "customWidget/tooltip.h"
#include <QHash>
#include "designcommands.h"
#include "utils.h"
#include "linelabel.h"
#include "customWidget/toolkittreeitem.h"
#include "CtrlItem/ctrlitemlua.h"
#include "../serpentineconnector.h"
#include "customWidget/CustomGraphicsItem/graphicsitemfactory.h"
#include "Frame/logsmanager.h"

ConfigScene::ConfigScene(Device_Trn *trnDev, AbsGraphView *pView) :
    AbsGraphScene(pView)
{
    //sceneRectChanged(QRectF)信号是在场景矩形发生变化的是时候发出（其中的参数的是新的场景矩形）
    connect(this, SIGNAL(sceneRectChanged(QRectF)), MainWindow::instance(), SLOT(setSheetSize(QRectF)));

    bindTrnDevice(trnDev);
    _spaceUsage = 0;
    _timeUsage  = 0;

    setDebugMode(false, false);
    initMenuActions();
    _copyItem = NULL;
//    _pageList << "page1" << "page2" << "page3" << "page4";
}

ConfigScene::~ConfigScene()
{
    //退出调试模式，停止刷新电平表等操作
    if (_trnDev != NULL){
        _trnDev->setDebugMode(false);
        _isDebuging = false;
        clearDebugModeData();
    }

    deleteGraphicsItems(items());
}


void ConfigScene::bindTrnDevice(Device_Trn *trnDev)
{
    if(trnDev != NULL){
        //清除原设备的信号槽
        if(_trnDev != NULL) _trnDev->disconnect(this);

        //绑定新设备
        _trnDev = trnDev;
        _trnINF = new Trn_Interface(trnDev);
        if (_trnDev->getDeviceType() == Dev_Type_DNTE_TRN) {
            connect(_trnDev, SIGNAL(deviceOffline()), this, SLOT(deviceOffline()));
        }

        connect(_trnDev, SIGNAL(fileFromDeviceSig(unsigned char *, int)), this, SLOT(fileFromDeviceSlot(unsigned char *, int)));
        connect(_trnDev, SIGNAL(fileFromDeviceEndSig()), this, SLOT(fileFromDeviceEndSlot()));
        connect(_trnDev, SIGNAL(ParaChangedFromUserSig(unsigned int *)), this, SLOT(ParaChangedFromUserSlot(unsigned int *)));
        connect(_trnDev, SIGNAL(SceneChangedFromUserSig()), this, SLOT(loadFromDevice()));
        connect(_trnDev, SIGNAL(ModuleRtnValueSig(unsigned int *)), this, SLOT(ModuleRtnValueSlot(unsigned int *)));
        connect(_trnDev, SIGNAL(InOutLevelSig(unsigned int *)), this, SLOT(InOutLevelSlot(unsigned int *)));
    }
}

QGraphicsItem *ConfigScene::CopyGraphicsLineItem(QGraphicsItem *graphItem, QMap<QGraphicsItem *, QGraphicsItem *> itemMap)
{
    if(graphItem->type() == LineItem::Type)
    {
        LineItem* line = qgraphicsitem_cast<LineItem *>(graphItem);

        QList<ItemPort*> Item0_outPortList;
        QList<ItemPort*> Item1_InPortList;

        //start port
        if(line->getStartDevice()->type() == ConfigItem::Type) {
            ConfigItem *cfgItem0 = castToConfigItem(itemMap.value(line->getStartDevice()));
            if(cfgItem0 != NULL) Item0_outPortList = cfgItem0->getOutPortList();
        }else if(line->getStartDevice()->type() == SerpentineConnector::Type) {
            SerpentineConnector *serItem0 = qgraphicsitem_cast<SerpentineConnector*>(itemMap.value(line->getStartDevice()));
            if(serItem0 != NULL) Item0_outPortList = serItem0->getOutPortList();
        }
        //end port
        if(line->getEndDevice()->type() == ConfigItem::Type) {
            ConfigItem *cfgItem1 = castToConfigItem(itemMap.value(line->getEndDevice()));
            if(cfgItem1 != NULL) Item1_InPortList = cfgItem1->getInPortList();
        }
        else if(line->getEndDevice()->type() == SerpentineConnector::Type) {
            SerpentineConnector* serItem1 = qgraphicsitem_cast<SerpentineConnector *>(itemMap.value(line->getEndDevice()));
            if(serItem1 != NULL) Item1_InPortList = serItem1->getInPortList();
        }
        if(Item0_outPortList.isEmpty() || Item1_InPortList.isEmpty()) return NULL;

        for (ItemPort *p0 : Item0_outPortList) {
            if (p0->getPortType() == line->getStartPort()->getPortType() &&
                p0->portNum() == line->getStartPort()->portNum()) {
                for (ItemPort *p1 : Item1_InPortList) {
                    if (p1->getPortType() == line->getEndPort()->getPortType() &&
                        p1->portNum() == line->getEndPort()->portNum())
                    {
                        return line->clone(p0, p1);
                    }
                }
            }
        }
    }

    return NULL;
}

void ConfigScene::initMenuActions()
{
    //右键菜单
    _showSpaceTimePercent = new QAction(tr("Show Space Time Percent"), this);
    _showSpaceTimePercent->setCheckable(true);
    _showSpaceTimePercent->setChecked(false);
    _copyParameterConfig = new QAction(tr("Copy Parameter"), this);
    _copyObject = new QAction(tr("Copy Object"), this);
    _pasteParameterConfig = new QAction(tr("Paste Parameter"), this);
    _pasteObject = new QAction(tr("Paste Object"), this);
    _propertyAction = new QAction(tr("Property"), this);
    _relevance = new QMenu(tr("Relevance"));

    connect(_showSpaceTimePercent, SIGNAL(triggered()), this, SLOT(onCfgContextMenuClicked()));
    connect(_copyParameterConfig, SIGNAL(triggered()), this, SLOT(onCfgContextMenuClicked()));
    connect(_copyObject, SIGNAL(triggered()), this, SLOT(onCfgContextMenuClicked()));
    connect(_pasteParameterConfig, SIGNAL(triggered()), this, SLOT(onCfgContextMenuClicked()));
    connect(_pasteObject, SIGNAL(triggered()), this, SLOT(onCfgContextMenuClicked()));
    connect(_propertyAction, SIGNAL(triggered()), this, SLOT(onCfgContextMenuClicked()));

    //初始化右键菜单
    _contextMenu.addAction(_delAction);
    _contextMenu.addAction(_contextMenu.addSeparator());

    _contextMenu.addAction(_copyParameterConfig);
    _contextMenu.addAction(_copyObject);
    _contextMenu.addAction(_pasteParameterConfig);
    _contextMenu.addAction(_pasteObject);
    _contextMenu.addAction(_contextMenu.addSeparator());

    _contextMenu.addAction(_propertyAction);
    _contextMenu.addMenu(_relevance);
    _contextMenu.addAction(_contextMenu.addSeparator());
    initCommonMenuActions();
    _contextMenu.addAction(_showSpaceTimePercent);

//    _pageList << "page1" << "page2" << "page3" << "page4";
//    QMenu * qMenu = _contextMenu.addMenu("test");
//    int i = 0;
//    foreach(QString page, _pageList) {
//        qMenu->addAction(page, this, SLOT(testSlot()));
//    }
//    _contextMenu.addAction("delePage", this, SLOT(testSlot()));
}


void ConfigScene::updateMenuActions()
{
    _copyObject->setVisible(false);
    _pasteObject->setVisible(false);
    _copyParameterConfig->setVisible(false);
    _pasteParameterConfig->setVisible(false);
    _propertyAction->setVisible(false);
    _relevance->menuAction()->setVisible(false);

    if(_copyItems.count()> 0)
    {
        _pasteObject->setVisible(true);
    }

    if(selectedItems().count() > 0)
    {
        _copyObject->setVisible(true);
    }

    ConfigItem * cfgitem = castToConfigItem(focusItem());
    if(cfgitem != NULL)
    {
        _copyParameterConfig->setVisible(true);
        _propertyAction->setVisible(true);
        if (_copyItem != NULL && _copyItem != cfgitem
                && _copyItem->getCfgType() == cfgitem->getCfgType())
        {
            _pasteParameterConfig->setVisible(true);
        }
    }
    AbsGraphScene::updateMenuActions();
}


void ConfigScene::deviceOffline()
{
    setDebugMode(false, false);    //设备下线退出调试模式
    MainWindow::instance()->showNotice(tr("Device is not online"));
}

void ConfigScene::saveParamToDevice(ConfigItem *cfgItem)
{
    if(_trnDev == NULL || !_trnDev->isOnline()) return;
    if (_isDebuging && cfgItem->isValid())
    {
        bool is16Bit = _trnDev->is16BitProtocal();
        QByteArray paramBuff;
        if (cfgItem->getCfgType() == CFG_TYPE_MIXER_32X32 ||
            cfgItem->getCfgType() == CFG_TYPE_MIXER_64X64)
        {
            paramBuff.append(cfgItem->getOnlineDetailPara(is16Bit));
        } else {
            paramBuff.append(cfgItem->getDetailPackBuff(is16Bit));
        }

        if (cfgItem->getCfgType() == CFG_TYPE_SOFTFIR_MONO512 ||
            cfgItem->getCfgType() == CFG_TYPE_SOFTFIR_STEREO512 ||
            cfgItem->getCfgType() == CFG_TYPE_SOFTFIR_4R512)
        {
            QByteArray temp;
            for(int j = 0; j < 2; j++)
            {
                temp.append(paramBuff.mid(0, 4));
                temp.append(paramBuff.mid(4+j*1032, 1032));
                _trnDev->sendAlgorithmRealtimePack(1, temp);
                temp.clear();
            }
        }
        else if (cfgItem->getCfgType() == CFG_TYPE_SOFTFIR_MONO1024 ||
                 cfgItem->getCfgType() == CFG_TYPE_SOFTFIR_STEREO1024 ||
                 cfgItem->getCfgType() == CFG_TYPE_SOFTFIR_4R1024)
        {
            QByteArray temp;
            for(int j = 0; j < 4; j++)
            {
                temp.append(paramBuff.mid(0, 4));
                temp.append(paramBuff.mid(4+j*1032, 1032));
                _trnDev->sendAlgorithmRealtimePack(1, temp);
                temp.clear();
            }
        } else {
            _trnDev->sendAlgorithmRealtimePack(1, paramBuff);
        }
    }
    setNeedSaveState(true);
}

void ConfigScene::subTypeChanged(ConfigItem* item, int newTypeIdx, int oldTypeIdx)
{
    CfgType newType = item->subType(newTypeIdx);
    CfgType oldType = item->subType(oldTypeIdx);
    _undoStack->push(new SubTypeChangedCommand(this, item, newType, oldType));
}

QString ConfigScene::getUniqueName(QString customName)
{
    QString uniqueName = customName;
    int index = 1;
    while(true)
    {
        bool isUnique = true;
        for(QGraphicsItem* item : items())
        {
            if(item->type() == DesignItem::Type)
            {
                ConfigItem* cfgItem = castToConfigItem(item);
                if(!cfgItem) break;
                if(cfgItem->customName() == uniqueName)
                {
                    isUnique = false;
                    break;
                }
            }
        }
        if(isUnique) return uniqueName;
        else uniqueName = QString("%1-%2").arg(customName).arg(index++);
    }
}

void ConfigScene::setPinLineVisible(ItemPort* pinPort, bool visible)
{
    for(QGraphicsItem* item : items(Qt::AscendingOrder))
    {
        if(item->type() == LineItem::Type)
        {
            LineItem* line = qgraphicsitem_cast<LineItem*>(item);
            if(line->getStartPort() == pinPort || line->getEndPort() == pinPort)
            {
                line->flashVisibleByPort();
            }
        }
    }
}

CLuaPinParam_PC* ConfigScene::getInCtrlPinDefTree(ItemPort* pinPort)
{
    CLuaPinParam_PC* inPinDef = NULL;
    ItemPort* linkedPort = getLinkedStartPort(pinPort);
    if(linkedPort != NULL)
    {
        inPinDef = linkedPort->getControlPinDef();
        Q_ASSERT(inPinDef != NULL);
    }

    return inPinDef;
}

QList<CLuaPinParam_PC*> ConfigScene::getOutCtrlPinDefTree(ItemPort* pinPort)
{
    QList<CLuaPinParam_PC*> pinDefList;
    QList<ItemPort*> linkedPortList = getLinkedEndPort(pinPort);
    for(ItemPort* port : linkedPortList)
    {
        CLuaPinParam_PC* pinDef = port->getControlPinDef();
        Q_ASSERT(pinDef != NULL);
        if(pinDef->pinPortOut != NULL)
        {
            QList<CLuaPinParam_PC*> children = getOutCtrlPinDefTree(pinDef->pinPortOut);
            pinDef->setChildren(children);
        }
        pinDefList.append(pinDef);
    }

    return pinDefList;
}

QList<ConfigItem*> ConfigScene::getConfigItemList()
{
    QList<ConfigItem*> itemList;
    for(QGraphicsItem* item : items(Qt::AscendingOrder))
    {
        if(item->type() == DesignItem::Type)
        {
            ConfigItem* cItem = castToConfigItem(item);
            if(cItem != NULL) {
                itemList.append(cItem);
            }
        }
    }
    return itemList;
}


void ConfigScene::setDebugMode(bool isDebug, bool needDownload)
{
    if(_trnDev == NULL) return;
    if(needDownload){
        bool rs = onDownloadAlgorithm();
        if(!rs) return;     //改变状态需确认下载成功
    }
    _isDebuging = isDebug;
    _trnDev->setDebugMode(isDebug);
    if(!isDebug){
        clearDebugModeData();
    }
    MainWindow::instance()->updateDeviceModeStatus(this);
}

void ConfigScene::clearDebugModeData()
{
    unsigned int zeroVU[512]; //512*4=2048 >数据包长度
    memset(&zeroVU, 0, sizeof(zeroVU));
    for (ConfigItem *item : _cfgItemList)
    {
        int cfgGroup = item->getCfgType() & FCT_FAMILY_MASK;
        if (cfgGroup == CFG_TYPE_INPUT_1 || cfgGroup == CFG_TYPE_OUTPUT_1)
        {
            item->setRtnData((unsigned int*)&zeroVU);
        }
        else if(cfgGroup == CFG_TYPE_METER_1)
        {
            ConfigItemMeter* meter = qobject_cast<ConfigItemMeter*>(item);
            meter->clearVuData();
        }
    }
}

bool ConfigScene::setOutputMute(bool isMute)
{
    if(isDebuging()) //需调试模式下
    {
        for (ConfigItem *item : _cfgItemList)
        {
            if ((item->getCfgType() & FCT_FAMILY_MASK) == CFG_TYPE_OUTPUT_1)
            {
                ConfigItemOutput* outItem = qobject_cast<ConfigItemOutput*>(item);
                if(outItem != nullptr){
                    outItem->setAllMute(isMute);
                }
            }
        }
        return true;
    }
    return false;
}


void ConfigScene::addSnapBankItems(QList<QGraphicsItem *> itemList, bool b) //hrp
{
    if(b) {
        foreach(QGraphicsItem *item, itemList)
        {
            ConfigItem *snapItem = qgraphicsitem_cast<ConfigItem *>(item);
            if(snapItem !=NULL){
                _trnINF->appendSnapShotItem(snapItem);
            }
        }
    }else {
        foreach(QGraphicsItem *item, itemList)
        {
            ConfigItem *snapItem = qgraphicsitem_cast<ConfigItem *>(item);
            if(snapItem !=NULL){
                _trnINF->removeOneSnapShotItem(snapItem);
            }
        }
    }

    emit designItemChanged(this);
}

void ConfigScene::delSnapBankItems(QList<QGraphicsItem *> itemList, bool b, bool sceneStatus) //hrp
{
    if(!b) {
        foreach(QGraphicsItem *item, itemList)
        {
            ConfigItem *snapItem = qgraphicsitem_cast<ConfigItem *>(item);
            if(snapItem !=NULL){
                _trnINF->appendSnapShotItem(snapItem);
            }
            DesignItem *snap = qgraphicsitem_cast<DesignItem *>(item);
            if(sceneStatus) {
                addItem(item);
                snap->initAfterAdd();
            }
        }
    }else {
        foreach(QGraphicsItem *item, itemList)
        {
            ConfigItem *snapItem = qgraphicsitem_cast<ConfigItem *>(item);
            if(snapItem !=NULL){
                _trnINF->removeOneSnapShotItem(snapItem);
            }
            DesignItem *snap = qgraphicsitem_cast<DesignItem *>(item);
            snap->clearBeforeDelete();
            if(items().contains(item)) {
               removeItem(item);
            }
        }
    }

    emit designItemChanged(this);
}

void ConfigScene::loadFromDevice(void)
{
    if(_trnDev == NULL || !_trnDev->isOnline()){
        MainWindow::instance()->showNotice(tr("Device is not online"));
        return;
    }
    if(!checkEditable()){
        return;
    }

    _devSceneBuff.clear();
    _trnDev->sendFileFromDevicePack();
}


#include "customWidget/CustomGraphicsItem/controltemplate.h"
//////////////////////////////////////////////////
/// \brief 鼠标事件
/// \param event
void ConfigScene::dragEnterEvent(QGraphicsSceneDragDropEvent *e)
{
    if(_trnDev == NULL) return;
    resetTempGroup();
    QString typeStr = QString::fromUtf8(e->mimeData()->data("type"));
    QString ptrStr = QString::fromUtf8(e->mimeData()->data("itemPtr"));
    QStringList typeList = typeStr.split(",");
    QStringList ptrList = ptrStr.split(",");

    QList<ConfigItem*> childItems;
    for(int i= 0; i<typeList.length(); i++)
    {
        CfgType cfgType = CfgType(typeList[i].toInt());
        ConfigItem *cfgItem = (ConfigItem *) (ptrList[i].toULongLong());
        if(cfgItem != NULL) {
            cfgItem->setAllPortEnable();
            if(cfgItem->scene() != NULL) {
                continue;
            }
        }
        else {
            QString customName =  QString("%1_%2").arg(Utils::getCfgShortName(CfgType(cfgType))).arg(_trnINF->getCfgItemIndex());
            customName = getUniqueName(customName);
            cfgItem = ConfigItemFactory::createConfigItem(_trnDev, CfgType(cfgType), customName, QPointF(0, 0));
        }

        if(cfgItem != NULL)
        {
            childItems.append(cfgItem);
        }
    }
    if(childItems.count() == 0) {
        e->ignore();
    }
    else {
        int posX = 0;
        int posY = 0;
        for(ConfigItem* child : childItems)
        {
            child->setOpacity(0.5);
            child->setPos(QPointF(posX, posY));
            posY += child->rect().height();
            _tempGroup->addToGroup(child);
        }

        addTempGroup();
        MainWindow::instance()->setModelDockBoxToolkitBtnStatus(false);
        e->acceptProposedAction();
    }
}


bool ConfigScene::onDownloadAlgorithm()
{
    if(_needSave) {save();} //先保存场景
    if (_trnDev == NULL || _trnDev->checkFirmwareVersion() == false) {
        return false;
    }

    if(!checkDesign()) return false;        //校验算法模块。
    if(!setAlgorithmMatrix()) return false; //将算法模块列行化存储
    //发送算法数据包
    QList<SubPack> modulenamePacks, overviewPacks, detailPacks;
    prepareAlgorithmPack(modulenamePacks, overviewPacks, detailPacks);
    int totalPacks = modulenamePacks.count() + overviewPacks.count() + detailPacks.count();
    if(totalPacks == 0){
        if(getConfigItemList().length() == 0){
            QMessageBox::StandardButton rsBtn = Utils::MessageBoxQuestion(tr("Question"), tr("Do you want to clear scene?"),
                                   QMessageBox::Yes  | QMessageBox::No );
            if (rsBtn == QMessageBox::Yes) {
                _trnDev->sendClearScenePack();
            }
        }
        else{
            MainWindow::instance()->showNotice(tr("No valid model need download."));
        }
        return false;
    }

    totalPacks += 10+10;    //输入输出的增益及相位
    quint32 process_count = 0;
    QProgressDialog process(Q_NULLPTR,
                            Qt::Widget | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    process.setWindowModality(Qt::ApplicationModal);
    process.setMinimumDuration(1);
    process.setLabelText(tr("Downloading..."));
    process.setRange(0, totalPacks);
    process.show();

    for (int i = 0; i < modulenamePacks.count(); ++i)
    {
        _trnDev->sendModuleNamePack(i == 0 ? true : false, modulenamePacks.at(i).count, modulenamePacks.at(i).dataBuff);
        process.setValue(process_count++);
    }

    for (int i = 0; i < overviewPacks.count(); ++i)
    {
        _trnDev->sendAlgorithmOverviewPack(i == 0 ? true : false, overviewPacks.at(i).count, overviewPacks.at(i).dataBuff);
        process.setValue(process_count++);
    }

    for (int i = 0; i < detailPacks.count(); ++i)
    {
        _trnDev->sendAlgorithmDetailPack(detailPacks.at(i).count, detailPacks.at(i).dataBuff);
        process.setValue(process_count++);
    }

    QThread::msleep(200);
    _trnDev->sendAlgorithmStartPack();//必须先启动算法，再设置模拟前置增益

    /* 下载模块时将当前的Gain和48V一起下载进去 */
    for (int i = 0; i < TRN_ANA_IN_NUM; ++i)
    {
        _trnDev->sendMicGainPack(i, _trnDev->getConfigData().InMicGain[i]);
        _trnDev->sendPhantomPack(i, _trnDev->getConfigData().InMic48V[i]);
    }
    process_count += 10;
    process.setValue(process_count);
    //下载Lua模块代码
    static int DownIdx = 0;
    DownIdx++;
    QList<CtrlItemLua*> luaItemList = prepareLuaAlgorithm();
    int luaItemIdx = 1;
    for(CtrlItemLua* luaCtrl : luaItemList)
    {
        QByteArray luaPackData = luaCtrl->getDetailData();
        _trnDev->sendLuaPack(DownIdx, luaItemIdx++, luaPackData);
    }
    QString logInfo = QString(tr("%1 download algorithm to device:%2"))
            .arg(Config::instance()->getCurUser().name).arg(_trnDev->getDeviceName());
    LogsManager::instance()->addLogs(tr("Info"), tr("DeviceManager"), logInfo);
    process_count += 10;
    process.setValue(process_count);
    return true;
}


void ConfigScene::prepareAlgorithmPack(QList<SubPack> &modulenamePacks, QList<SubPack> &overviewPacks, QList<SubPack> &detailPacks)
{
    //将网格化算法分包    
    bool is16Bit = _trnDev->is16BitProtocal();
    int MaxPackSize = 1000;
    SubPack curOverview, curModulename, curDetail;
    for (int curLayer = 0; curLayer < _maxConfigLayer; ++curLayer)
    {
        for (QMap<quint16,ConfigItem *>::iterator iter = _algorithmMap.begin();
                iter != _algorithmMap.end();  ++iter)
        {
            if ((iter.key() & 0xFF) != curLayer) continue; //跳过非当前Layer
            ConfigItem *item = iter.value();

            //处理overview包, 合包发送
            QByteArray overviewBuff = item->getOverviewPackBuff(is16Bit);
            if(curOverview.dataBuff.count() + overviewBuff.count() > MaxPackSize) {
               overviewPacks.append(curOverview);
               curOverview.Clear();
            }
            curOverview.count++;
            curOverview.dataBuff.append(overviewBuff);

            //处理modulename包，合包发送
            QByteArray modulenameBuff = item->getModuleNamePackBuff(is16Bit);
            if(curModulename.dataBuff.count() + modulenameBuff.count() > MaxPackSize) {
               modulenamePacks.append(curModulename);
               curModulename.Clear();
            }
            curModulename.count++;
            curModulename.dataBuff.append(modulenameBuff);

            //处理detail包, 某些算法需要拆分大包
            QByteArray detailBuff = item->getDetailPackBuff(is16Bit);

            if(item->getCfgType() == CFG_TYPE_MIXER_32X32) {
                for(int j = 0; j < 64; j++) {
                    curDetail.count = 1;
                    curDetail.dataBuff.append(detailBuff.mid(0, 4));
                    curDetail.dataBuff.append(detailBuff.mid(4+j*72, 72));
                    detailPacks.append(curDetail);
                    curDetail.Clear();
                }
            }
            else if(item->getCfgType() == CFG_TYPE_MIXER_64X64) {
                for(int j = 0; j < 128; j++) {
                    curDetail.count = 1;
                    curDetail.dataBuff.append(detailBuff.mid(0, 4));
                    curDetail.dataBuff.append(detailBuff.mid(4+j*140, 140));
                    detailPacks.append(curDetail);
                    curDetail.Clear();
                }
            }
            else if (item->getCfgType() == CFG_TYPE_SOFTFIR_MONO512 ||
                     item->getCfgType() == CFG_TYPE_SOFTFIR_STEREO512 ||
                     item->getCfgType() == CFG_TYPE_SOFTFIR_4R512) {
                for(int j = 0; j < 2; j++) {
                    curDetail.count = 1;
                    curDetail.dataBuff.append(detailBuff.mid(0, 4));
                    curDetail.dataBuff.append(detailBuff.mid(4+j*1032, 1032));
                    detailPacks.append(curDetail);
                    curDetail.Clear();
                }
            }
            else if (item->getCfgType() == CFG_TYPE_SOFTFIR_MONO1024 ||
                     item->getCfgType() == CFG_TYPE_SOFTFIR_STEREO1024 ||
                     item->getCfgType() == CFG_TYPE_SOFTFIR_4R1024) {
                for(int j = 0; j < 4; j++) {
                    curDetail.count = 1;
                    curDetail.dataBuff.append(detailBuff.mid(0, 4));
                    curDetail.dataBuff.append(detailBuff.mid(4+j*1032, 1032));
                    detailPacks.append(curDetail);
                    curDetail.Clear();
                }
            }
            else {
                curDetail.count = 1;
                curDetail.dataBuff.append(detailBuff);
                detailPacks.append(curDetail);
                curDetail.Clear();
            }
        }
    }

    //处理最后一个包
    if (curModulename.count >0) {
        modulenamePacks.append(curModulename);
    }
    if (curOverview.count >0){
        overviewPacks.append(curOverview);
    }

    return;
}


void ConfigScene::onCfgContextMenuClicked()
{
    if (sender() == _copyParameterConfig)
    {
        _copyItem = qgraphicsitem_cast<ConfigItem *>(focusItem());
    }
    else if (sender() == _copyObject)
    {
        onItemsCopy();
    }
    else if (sender() == _pasteObject)
    {
        onItemsPaste();
    }
    else if (sender() == _pasteParameterConfig)
    {
        ConfigItem *pasteItem = qgraphicsitem_cast<ConfigItem *>(focusItem());
        pasteItem->clone(_copyItem);
        setNeedSaveState(true);
    }
    else if (sender() == _propertyAction)
    {
        ConfigItem *cfgItem = qgraphicsitem_cast<ConfigItem *>(focusItem());
        if(cfgItem != NULL){
            QPointF pos = cfgItem->mapFromScene(cfgItem->scenePos());
            QPointF posNew(pos.x() + cfgItem->rect().width()/2, pos.y() + cfgItem->rect().height()/2);
            cfgItem->showSettingForm(posNew);
        }
    }
    else if (sender() == _showSpaceTimePercent)
    {        
        bool isShow = _showSpaceTimePercent->isChecked();
        ConfigItem::ShowSpaceTimePercent = isShow;
        update();
    }
}

//兼容两个版本的Mac存储位置
bool ConfigScene::getConfigFileMac(QString filePath, QString& mac, DevType& devType)
{
    if(!QFile::exists(filePath)) return false;

    QSettings *iniFile = new QSettings(filePath, QSettings::IniFormat,0);
    if(iniFile->contains(QString(CONFIG_DEV_TYPE))){
        devType = DevType(iniFile->value(QString(CONFIG_DEV_TYPE)).toInt());
    }
    else{
        devType = Dev_Type_Unknow;
    }

    if(iniFile->contains(QString(CONFIG_DEV_MAC))){
        mac = iniFile->value(QString(CONFIG_DEV_MAC)).toString();
        return true;
    }
    else{
        QStringList groupList = iniFile->childGroups();
        for(QString groupName : groupList)
        {
            if(groupName.startsWith("Config_")){
                mac = groupName.right(groupName.length() - 7);
                return true;
            }
        }
    }
    return false;
}

void ConfigScene::open(QString filePath, bool isImport)
{
    if(_trnDev ==NULL) {
        //wj2 提示绑定设备
        return;
    }
    if(!isImport){
        _filePath = filePath;
    }
    bool nmuValid = true;
    QSettings *ConfigIni = new QSettings(filePath, QSettings::IniFormat,0);
    QScopedPointer<QSettings> iniPtr(ConfigIni);
    QString configGroupKey = QString();
    QStringList groupList = iniPtr->childGroups();
    for(QString groupName : groupList)
    {
        if(groupName.startsWith("Config_")){
            configGroupKey = groupName;
            break;
        }
    }

    _pageList = ConfigIni->value(QString(CONFIG_PAGELIST)).toStringList();
    _curPageName = ConfigIni->value(QString(CONFIG_CURPAGE)).toString();
    if(_curPageName.isEmpty()) _curPageName = "page1";
    if(_pageList.isEmpty()) _pageList << _curPageName;
    setCurPage(getCurPage());

    if (configGroupKey.isEmpty()) {
        return;
    }

    int nmuVersion = ConfigIni->value(QString(CONFIG_NMU_VERSION)).toInt();
    int nmuDevType = ConfigIni->value(QString(CONFIG_DEV_TYPE)).toInt();
    QString nmuDevMac    = ConfigIni->value(QString(CONFIG_DEV_MAC)).toString();
    if(_trnDev!= NULL && _trnDev->getDeviceType() != nmuDevType){
        QMessageBox::StandardButton rsBtn = Utils::MessageBoxQuestion(tr("Question"),
                               tr("Device type is diffrent, Do you want to import?"),
                               QMessageBox::Yes  | QMessageBox::No );
        if (rsBtn == QMessageBox::No) {
            return;
        }
    }

    //清空场景
    foreach (QGraphicsItem *item, items(Qt::AscendingOrder))
    {
        if(item->type() == ConfigItem::Type || item->type() == LineItem::Type) {
            removeItem(item);
        }
    }
    _undoStack->clear();
    _spaceUsage = 0;
    _timeUsage  = 0;



    /* 如果文件记录内容的时间或者空间超限，则不加载 */
    ConfigIni->beginGroup(configGroupKey);
    ConfigIni->beginGroup(QString(CONFIG_INFOMATION_GROUP));
    QSizeF sz = ConfigIni->value(QString(CONFIG_SCENESIZE)).toSizeF();//废弃SceneSize，改为自适应
    quint32 devCount = ConfigIni->value(QString(CONFIG_DEVICE_COUNT)).toUInt();
    quint32 lineCount = ConfigIni->value(QString(CONFIG_LINE_COUNT)).toUInt();
    quint32 linelabelCount = ConfigIni->value(QString(CONFIG_LINELABEL_COUNT)).toUInt();
    quint32 grapItemCount   = ConfigIni->value(QString(CONFIG_GRAPITEM_COUNT)).toUInt();
    quint32 serConItemCount = ConfigIni->value(QString(CONFIG_SERCONITEM_COUNT)).toUInt();
    ConfigIni->endGroup();

    for (quint32 i = 0; i < devCount; ++i)
    {
        bool rs = readConfigItem(nmuVersion, ConfigIni, QString(CONFIG_DEVICE_GROUP).arg(i));
        if(!rs){
            nmuValid = false;
            break;
        }
    }
    emit designItemChanged(this);

    for(quint32 i = 0; i < serConItemCount; i++) {
        ConfigIni->beginGroup(QString(CONFIG_SERCONITEM_GROUP).arg(i));
        int type     = ConfigIni->value(QString(CONFIG_SERCONiTEM_TYPE)).toInt();
        int chaCount = ConfigIni->value(QString(CONFIG_SERCONITEM_CHANNELCOUNT)).toInt();
        int chaType  = ConfigIni->value(QString(CONFIG_SERCONITEM_CHAPORTTYPE)).toInt();
        int zValue   = ConfigIni->value(QString(CONFIG_SERCONITEM_ZVALUE)).toInt();

        SerpentineConnector *serConItem = new SerpentineConnector(
                    (SerConType)type, chaCount, (DevPortType)chaType);
        if(serConItem) {
            serConItem->loadDataFromSettings(ConfigIni);
            if(zValue > _trnDev->getItemUserZValueNum(false))
                 _trnDev->setItemUserZValueNum(zValue);
            if(nmuVersion)
                serConItem->setUserZValue(zValue);
            else
                serConItem->setUserZValue(_trnDev->getItemUserZValueNum(true));
            addItem(serConItem);
        }
        ConfigIni->endGroup();
    }

    for (quint32 i = 0; i < lineCount; ++i)
    {
        bool rs = readLineItem(nmuVersion, ConfigIni, QString(CONFIG_LINE_GROUP).arg(i));
        if(!rs){
            nmuValid = false;
            break;
        }
    }

    for (quint32 i = 0; i < linelabelCount; ++i)
    {
        ConfigIni->beginGroup(QString(CONFIG_LINELABEL_GROUP).arg(i));
        QPointF portPoint = ConfigIni->value(QString(CONFIG_LINELABEL_POS)).toPointF();
        QString portName  = ConfigIni->value(QString(CONFIG_LINELABEL_NAME)).toString();
        int zValue        = ConfigIni->value(QString(CONFIG_LINELABEL_ZVALUE)).toInt();
        ConfigIni->endGroup();

        ItemPort *portLabel = NULL;

        foreach (QGraphicsItem *item, items(portPoint))
        {
            if (item->type() == ItemPort::Type)
            {
                bool same = qgraphicsitem_cast<ConfigItem *>(item->parentItem())->checkZValue() == zValue;
                if(nmuVersion == 0 ? true : same) {
                    portLabel = qgraphicsitem_cast<ItemPort *>(item);
                    break;
                }
            }
        }
        if(portLabel) {
            LineLabel *lineLabel = new LineLabel(portLabel);
            lineLabel->setName(portName);
            lineLabel->setUserZValue(qgraphicsitem_cast<ConfigItem *>(portLabel->parentItem())->checkZValue());
            portLabel->setLineLabel(lineLabel);
            _labelList.append(lineLabel);
        }
    }

    for(quint32 i = 0; i < grapItemCount; i++) {
        ConfigIni->beginGroup(QString(CONFIG_GRAPHICSIETM_GROUP).arg(i));
        int type = ConfigIni->value(QString(CONFIG_GRAPHICSITEM_TYPE)).toInt();
        uint pathCount = ConfigIni->value(QString(CONFIG_GRAPHICSITEM_BORDERPATHCOUNT)).toUInt();
        QPainterPath path;
        for(uint i = 0; i < pathCount; i++) {
            if(!i) {
                path.moveTo(ConfigIni->value(QString(CONFIG_GRAPHICSITEM_BORDERPATH).arg(i)).toPointF());
            }
            path.lineTo(ConfigIni->value(QString(CONFIG_GRAPHICSITEM_BORDERPATH).arg(i)).toPointF());
        }
        GraphicsItem *grapItem = GraphicsItemFactory::CreateGraphicsItem(
                    (GraphicsItem::GraphicsItemType)type, path);
        if(grapItem) {
            grapItem->loadDataFromSettings(ConfigIni);
            addItem(grapItem);
        }
        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();

    if(!nmuValid ){
        Utils::MessageBoxWarning(tr("Warning"), QString("NMU file is valid!"));
        return;
    }
    matchLineLabels();
    checkDesign();
    setDebugMode(false, false);
    autoFitSceneSize();

    if(isImport){//导入nmu(包括从设备加载),需要保存
       saveHandler(_filePath);
    }
    setNeedSaveState(false);
}


void ConfigScene::saveHandler(QString filePath)
{
    QString mac;
    if(_trnDev != NULL) mac = _trnDev->getAES70Mac();
    QSettings *ConfigIni = new QSettings(filePath, QSettings::IniFormat, 0);

    DevType devType = Dev_Type_Unknow;
    if(_trnDev != NULL) devType = _trnDev->getDeviceType();
    ConfigIni->setValue(QString(CONFIG_NMU_VERSION), 1);
    ConfigIni->setValue(QString(CONFIG_DEV_TYPE), devType);
    ConfigIni->setValue(QString(CONFIG_DEV_MAC), mac);
    //pageList
    ConfigIni->setValue(QString(CONFIG_PAGELIST), getPageList());
    ConfigIni->setValue(QString(CONFIG_CURPAGE), getCurPage());
    quint32 devCount = 0;
    quint32 lineCount = 0;
    quint32 linelabelCount = 0;
    quint32 graphicsItemCount = 0;
    quint32 serConItemCount = 0;
    ConfigIni->remove(QString(CONFIG_GROUP));
    ConfigIni->beginGroup(QString(CONFIG_GROUP));

    foreach(QGraphicsItem* item, items(Qt::AscendingOrder))
    {
       if (item->type() == ConfigItem::Type)
       {
           ConfigItem * cfgItem =  qgraphicsitem_cast<ConfigItem*>(item);
           ConfigIni->beginGroup(QString(CONFIG_DEVICE_GROUP).arg(devCount++));
           ConfigIni->setValue(QString(CONFIG_DEVICE_DEVTYPE), cfgItem->getCfgType());
           ConfigIni->setValue(QString(CONFIG_DEVICE_DEVPOS), cfgItem->scenePos());
           ConfigIni->setValue(QString(CONFIG_DEVICE_DEVNAME), cfgItem->customName());
           ConfigIni->setValue(QString(CONFIG_DEVICE_LABEL_COLOR), cfgItem->getFillColor());
           ConfigIni->setValue(QString(CONFIG_DEVICE_MODULE_ZVALUE), cfgItem->checkZValue());
           cfgItem->saveParaToSetting(ConfigIni);
           ConfigIni->endGroup();
       }
    }

    foreach (QGraphicsItem *item, items(Qt::AscendingOrder))
    {
        if (item->type() == LineItem::Type)
        {
            LineItem * cfgLine =  qgraphicsitem_cast<LineItem*>(item);
            ItemPort *startPort = cfgLine->getStartPort();
            ItemPort *endPort   = cfgLine->getEndPort();
            //hrp
            int startZValue = cfgLine->getUserStartZValue();
            int endZValue   = cfgLine->getUserEndZValue();

            QPointF startPortPoint = startPort->scenePos();
            QPointF endPortPoint = endPort->scenePos();
            ConfigIni->beginGroup(QString(CONFIG_LINE_GROUP).arg(lineCount++));
            ConfigIni->setValue(QString(CONFIG_LINE_STARTPOS), startPortPoint);
            ConfigIni->setValue(QString(CONFIG_LINE_ENDPOS), endPortPoint);
            ConfigIni->setValue(QString(CONFIG_LINE_STARTZVALUE), startZValue);
            ConfigIni->setValue(QString(CONFIG_LINE_ENDZVALUE), endZValue);
            ConfigIni->endGroup();
        }
    }

    foreach (QGraphicsItem *item, items(Qt::AscendingOrder))
    {
        if (item->type() == LineLabel::Type)
        {
            LineLabel * cfgLineLabel =  qgraphicsitem_cast<LineLabel*>(item);
//            if (!cfgLineLabel->isMatched()) continue;
            ItemPort *port = cfgLineLabel->getLabelPort();
            QPointF PortPoint = port->scenePos();

            ConfigIni->beginGroup(QString(CONFIG_LINELABEL_GROUP).arg(linelabelCount++));
            ConfigIni->setValue(QString(CONFIG_LINELABEL_POS), PortPoint);
            ConfigIni->setValue(QString(CONFIG_LINELABEL_NAME), cfgLineLabel->name());
            ConfigIni->setValue(QString(CONFIG_LINELABEL_ZVALUE), cfgLineLabel->getUserZValue());
            ConfigIni->endGroup();
        }
    }

    foreach(QGraphicsItem *item, items(Qt::AscendingOrder)) {
        if(item->type() == GraphicsItem::Type) {
            GraphicsItem *grapItem = qgraphicsitem_cast<GraphicsItem *>(item);
            ConfigIni->beginGroup(QString(CONFIG_GRAPHICSIETM_GROUP).arg(graphicsItemCount++));
            grapItem->saveDataToSettings(ConfigIni);

            ConfigIni->endGroup();
        }
    }
    foreach(QGraphicsItem *item, items(Qt::AscendingOrder)) {
        if(item->type() == SerpentineConnector::Type) {
            SerpentineConnector *serConItem = qgraphicsitem_cast<SerpentineConnector *>(item);
            ConfigIni->beginGroup(QString(CONFIG_SERCONITEM_GROUP).arg(serConItemCount++));
            serConItem->saveDataToSettings(ConfigIni);
            ConfigIni->endGroup();
        }
    }

    ConfigIni->beginGroup(QString(CONFIG_INFOMATION_GROUP));
    ConfigIni->setValue(QString(CONFIG_SCENESIZE), sceneRect().size());
    ConfigIni->setValue(QString(CONFIG_DEVICE_COUNT), devCount);
    ConfigIni->setValue(QString(CONFIG_LINE_COUNT), lineCount);
    ConfigIni->setValue(QString(CONFIG_LINELABEL_COUNT), linelabelCount);
    ConfigIni->setValue(QString(CONFIG_GRAPITEM_COUNT), graphicsItemCount);
    ConfigIni->setValue(QString(CONFIG_SERCONITEM_COUNT), serConItemCount);
    ConfigIni->endGroup();

    ConfigIni->endGroup();

    delete ConfigIni;
}

void ConfigScene::save()
{
    foreach (QGraphicsItem *item, items()) //删除隐藏的toolTip
    {
        if(item->type() == ToolTip::Type) removeItem(item);
    }
    saveHandler(_filePath);
    setNeedSaveState(false);
}

void ConfigScene::saveAs(QString filePath)
{
    saveHandler(filePath);

    if(!_filePath.isEmpty() && filePath != _filePath){
        QString info = tr("save to ");
        info += filePath;
        info += tr(" succeed. ");
        MainWindow::instance()->showNotice(info);
    }
    else {
        setNeedSaveState(false);
    }
}

void ConfigScene::createSceneIni(QString filePath, Device_Trn *trnDev)
{
    QSettings *ConfigIni = new QSettings(filePath, QSettings::IniFormat, 0);

    DevType devType = trnDev->getDeviceType();
    QString mac = trnDev->getAES70Mac();
    ConfigIni->setValue(QString(CONFIG_NMU_VERSION), 1);
    ConfigIni->setValue(QString(CONFIG_DEV_TYPE), devType);
    ConfigIni->setValue(QString(CONFIG_DEV_MAC), mac);

    ConfigIni->beginGroup(QString(CONFIG_GROUP));
    ConfigIni->endGroup();
    delete ConfigIni;
}

void ConfigScene::saveSceneToDevice(quint8 scene_idx)
{
    if (_trnDev == NULL || _trnDev->checkFirmwareVersion() == false) {
        return;
    }

    if(!checkDesign()) return;              //校验算法模块。
    if(!setAlgorithmMatrix()) return;       //将算法模块列行化存储

    QString tempSaveName = DEFAULT_DEBUG_DIR"temp_w";
    QFile file(tempSaveName);
    file.remove();

    saveHandler(tempSaveName);
    file.open(QIODevice::ReadOnly);
    QByteArray fileBuff = file.readAll();
    file.close();
    quint32 filePacks = (fileBuff.count() + 1023)  / 1024 ;

    QList<SubPack> modulenamePacks, overviewPacks, detailPacks;
    prepareAlgorithmPack(modulenamePacks, overviewPacks, detailPacks);

    quint32 pro_count = 0;
    QProgressDialog process(Q_NULLPTR,
                            Qt::Widget | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    process.setWindowModality(Qt::ApplicationModal);
    process.setMinimumDuration(1);
    process.setLabelText(tr("Downloading..."));
    process.setRange(0, modulenamePacks.count() + overviewPacks.count() + detailPacks.count() + filePacks);
    process.show();

    //顺序暂不能调整，需和设备程序同步调整，和下载算法循序不一致
    for (int i = 0; i < overviewPacks.count(); ++i)
    {
        _trnDev->sendAlgorithmSavePack(i == 0 ? true : false, true, scene_idx, overviewPacks.at(i).count, overviewPacks.at(i).dataBuff);
        process.setValue(pro_count++);
    }

    for (int i = 0; i < modulenamePacks.count(); ++i)
    {
        _trnDev->sendModuleNameSavePack(i == 0 ? true : false, scene_idx, modulenamePacks.at(i).count, modulenamePacks.at(i).dataBuff);
        process.setValue(pro_count++);
    }

    for (int i = 0; i < detailPacks.count(); ++i)
    {
        _trnDev->sendAlgorithmSavePack(false, false, scene_idx, detailPacks.at(i).count, detailPacks.at(i).dataBuff);
        process.setValue(pro_count++);
    }

    _trnDev->sendFileToDeviceStartPack(scene_idx);
    int fileLen =  fileBuff.length();
    for (quint32 i = 0; i < filePacks; ++i)
    {
        int packLen = 1024;
        if(fileLen< (1024*i +1024))  packLen = fileLen -1024*i;
        _trnDev->sendFileToDevicePack(fileBuff.mid(1024*i, packLen));
        process.setValue(pro_count++);
    }

    QByteArray extraBuff;
    QDataStream stream(&extraBuff, QIODevice::ReadWrite);
    stream.setByteOrder(QDataStream::BigEndian);
    quint32 checkSum = 0;
    for(int i = 0; i< fileLen; i++)
        checkSum += (quint8)(fileBuff[i]);
    stream << fileLen;
    stream << checkSum;
    _trnDev->sendFileToDevicePack(extraBuff);
    _trnDev->sendFileToDeviceEndPack();

    QChar sceneChar = 'A' +scene_idx;
    QString logInfo = QString(tr("%1 download algorithm to device:%2,sceneNo:%3"))
            .arg(Config::instance()->getCurUser().name).arg(_trnDev->getDeviceName()).arg(sceneChar);
    LogsManager::instance()->addLogs(tr("Info"), tr("DeviceManager"), logInfo);
}

void ConfigScene::fileFromDeviceSlot(unsigned char *data, int len)
{
    _devSceneBuff.append((const char*)data, len);
}

void ConfigScene::fileFromDeviceEndSlot()
{
    if(!checkEditable()) return;
    //校验
    bool fileValid = false;
    int bufLen = _devSceneBuff.length();
    if(bufLen >= 8) {
        QByteArray extraBuff = _devSceneBuff.right(8);
        QDataStream stream(extraBuff);
        stream.setByteOrder(QDataStream::BigEndian);
        int fileLen;
        quint32 checkSumOld;
        stream>>fileLen>>checkSumOld;
        if(fileLen == bufLen-8)
        {
            int checkSum = 0;
            for(int i = 0; i< bufLen-8; i++){
                checkSum += (quint8) _devSceneBuff[i];
            }
            if(checkSum == checkSumOld) fileValid = true;
        }
    }
    if(!fileValid) {        
        MainWindow::instance()->showNotice(tr("Scene is invalid, try again."));
        return;
    }
    QFile tempSceneFile(DEFAULT_DEBUG_DIR"temp_r");
    if(tempSceneFile.exists()) tempSceneFile.remove();
    tempSceneFile.open(QIODevice::ReadWrite);
    tempSceneFile.write(_devSceneBuff.constData(), bufLen-8);
    tempSceneFile.close();

    open(DEFAULT_DEBUG_DIR"temp_r", true);
    //将算法模块列行存储
    if(!checkDesign()) return;          //校验算法模块。
    if(!setAlgorithmMatrix()) return;   //将算法模块列行化存储

    setDebugMode(true, false);
}

void ConfigScene::ParaChangedFromUserSlot(unsigned int *para)
{
    if(_trnDev == NULL) return;
    unsigned int *pParam = 0;
    bool is16BitProtocal = _trnDev->is16BitProtocal();
    if(is16BitProtocal){
        pParam = para + (sizeof(IG_Algorithm_Head) + sizeof(IG_Global_HEAD16B))/sizeof(quint32);
    }
    else{
        pParam = para + (sizeof(IG_Algorithm_Head) + sizeof(IG_Global_HEAD8B))/sizeof(quint32);
    }
    for (QMap<quint16,ConfigItem *>::iterator iter = _algorithmMap.begin();
            iter != _algorithmMap.end();  ++iter)
    {
        ConfigItem *item = iter.value();
        int offset;
        quint32 moduleNo = ConfigItem::getPacketModuleNum(&pParam[0], is16BitProtocal, offset);
        if (item->getModuleNum() == moduleNo)
        {
            item->setDetailData(&pParam[offset]);
            return;
        }
    }
}

void ConfigScene::ModuleRtnValueSlot(unsigned int *para)
{
    if(_isDebuging && _trnDev != NULL) //需调试模式下
    {
        int dataIdx = 1;
        bool is16BitProtocal = _trnDev->is16BitProtocal();
        quint32 moduleNo = ConfigItem::getPacketModuleNum(para, is16BitProtocal, dataIdx);
        for (QMap<quint16,ConfigItem *>::iterator iter = _algorithmMap.begin();
                iter != _algorithmMap.end();  ++iter)
        {
//            qDebug()<<__FUNCTION__<<__LINE__<< "moduleNo:" << moduleNo << "  data:" <<para[dataIdx]/65535 << para[dataIdx]%65535;
            ConfigItem *item = iter.value();
            if (item->getModuleNum() == moduleNo &&
                    (item->getCfgType() & FCT_FAMILY_MASK) != CFG_TYPE_INPUT_1 &&
                    (item->getCfgType() & FCT_FAMILY_MASK) != CFG_TYPE_OUTPUT_1)
            {
                item->setRtnData(&para[dataIdx]);
                return;
            }
        }
    }
}

void ConfigScene::InOutLevelSlot(unsigned int *level)
{
    if(_isDebuging) //需调试模式下
    {
        for (ConfigItem *item : _cfgItemList)
        {
            quint32 typeGroup = (item->getCfgType() & FCT_FAMILY_MASK);
            if (typeGroup == CFG_TYPE_INPUT_1 ||
                typeGroup == CFG_TYPE_OUTPUT_1) {
                item->setRtnData(level);
            }
        }
    }
}

void ConfigScene::onActionTriggeredSlots()//hrp  待完善
{
    if(_trnDev == NULL)
    {
        //wj2 提醒绑定设备
        return;
    }
    QAction *actionItem = qobject_cast<QAction *>(sender());
    if(actionItem->text() == "Create new Snapshot Bank") {
        QPointF pos(20, 20*(90 + 20));
        QString customNameSnapshot =  QString("%1_%2").arg(Utils::getCfgShortName(CFG_TYPE_SNAPSHOT_BANK)).arg(_trnINF->getCfgItemIndex());
        ConfigItem *snapshot = ConfigItemFactory::createConfigItem(_trnDev, CFG_TYPE_SNAPSHOT_BANK, customNameSnapshot, pos);
        qgraphicsitem_cast<ConfigItemSnapshot *>(snapshot)->setCreateType(ConfigItemSnapshot::Create_Snap_Bank);
        if(snapshot != NULL)
        {
            QList<QGraphicsItem *> itemList({snapshot});
            _undoStack->push(new ItemSnapBankAddCommand(this, itemList));
        }
    }else if(actionItem->text() == "Create new Snapshot Bank form selection") {
        QPointF pos(20, 20*(90 + 20));
        QString customNameSnapshot =  QString("%1_%2").arg(Utils::getCfgShortName(CFG_TYPE_SNAPSHOT_BANK)).arg(_trnINF->getCfgItemIndex());
        ConfigItem *snapshotParent = ConfigItemFactory::createConfigItem(_trnDev, CFG_TYPE_SNAPSHOT_BANK, customNameSnapshot, pos);
        ConfigItemSnapshot *snap = qgraphicsitem_cast<ConfigItemSnapshot *>(snapshotParent);
        snap->setCreateType(ConfigItemSnapshot::Create_snap_Bank_FromSel);
        foreach(QGraphicsItem *item, this->selectedItems()) {
            if(item->type() == DesignItem::Type) {
                ConfigItem *cfgItem = qgraphicsitem_cast<ConfigItem *>(item);
                if(cfgItem) {
                    if(cfgItem->getCfgType() != CFG_TYPE_SNAPSHOT && cfgItem->getCfgType() != CFG_TYPE_SNAPSHOT_BANK)
                        snap->insertFromSel_CtrlsList(cfgItem);
                }
            }
        }
        if(snapshotParent != NULL && !snap->getFromSel_CtrlsList().empty())
        {
            QList<QGraphicsItem *> itemList({snapshotParent});
            _undoStack->push(new ItemSnapBankAddCommand(this, itemList));
        }
    }
}

void ConfigScene::onTreeWidgetKey(Qt::Key key, QTreeWidgetItem *treeWidItem)//hrp  待完善
{
    QList<QGraphicsItem *> itemList;
    switch(key) {
    case Qt::Key_Delete:{
        if(treeWidItem->type() == ToolKitTreeItem::SnapType_Bank) {
            ToolKitTreeItem *snapBank = static_cast<ToolKitTreeItem *>(treeWidItem);
            if(!snapBank) break;
            ConfigItem *cfgItem = snapBank->getcfgItem();
            if(_trnINF->containsSnapShotItem(cfgItem)) {
                itemList.append(cfgItem);
                _trnINF->removeAllSnapShotItem(cfgItem);
            }
            if(!itemList.isEmpty()) {
                _undoStack->push(new ItemSnapBankDelCommand(this, itemList));
            }
        }else if(treeWidItem->type() == ToolKitTreeItem::SnapType_controller) {
            ToolKitTreeItem *snapBank = static_cast<ToolKitTreeItem *>(treeWidItem->parent());
            ConfigItem *cfgItem = (ConfigItem *)treeWidItem->data(0, ToolKitTreeItem::SnapType_ItemPtr).toULongLong();
            itemList.append(cfgItem);
            _undoStack->push(new ItemSnapControllerDelCommand(this, snapBank->getcfgItem(), itemList));
        }else if(treeWidItem->type() == ToolKitTreeItem::UciType_CfgUCI) {
            ToolKitTreeItem *uci = static_cast<ToolKitTreeItem *>(treeWidItem);
            _UCIList.removeAll(uci->getcfgItem());
            MainWindow::instance()->delUserControlInterfaceView(uci->getcfgItem());
            emit designItemChanged(this);
        }
        break;
    }
    default:
        break;
    }
}

void ConfigScene::addUserControlInterfaceItem()
{
    if(_trnDev == NULL) return;
    QPointF pos(20, 20*(90 + 20));
    QString customNameUCI =  tr("Interface %1").arg(_trnINF->getCfgItemIndex());
    ConfigItem *uciParent = ConfigItemFactory::createConfigItem(_trnDev, CFG_TYPE_UCI, customNameUCI, pos);
    ConfigItemUCI *uci = qgraphicsitem_cast<ConfigItemUCI *>(uciParent);

    uci->setAES70Device(_trnDev);
    int idx = 0;

    foreach(QGraphicsItem *item, this->items(Qt::AscendingOrder)) {
        ConfigItem *cfgItem = qgraphicsitem_cast<ConfigItem *>(item);
        if(!cfgItem) continue;

        if((cfgItem->getCfgType()&FCT_FAMILY_MASK) == CFG_TYPE_INPUT_1) {
            ConfigItemInput *inputItem = qgraphicsitem_cast<ConfigItemInput *>(cfgItem);
            if(!inputItem) continue;
            for(int i = 0; i < inputItem->getMaxPort(); i++) {
                ConfigItemUCI::Uci_data_t *inputData = new ConfigItemUCI::Uci_data_t(inputItem->getCfgType(),
                                                                                     inputItem->configTypeName().toUtf8(),
                                                                                     inputItem->customName().toUtf8(),
                                                                                     inputItem->getUserLabel(i).toUtf8(),
                                                                                     inputItem->getRealChannel(i),
                                                                                     idx/16, idx%16);
                inputData->s_gainRange.maxValue = 12;
                inputData->s_gainRange.minValue = -100;
                uci->addTabWidgetData(inputData);
                idx++;
            }
        }
        else if((cfgItem->getCfgType()&FCT_FAMILY_MASK) == CFG_TYPE_OUTPUT_1) {
            ConfigItemOutput *outputItem = qgraphicsitem_cast<ConfigItemOutput *>(cfgItem);
            if(!cfgItem) continue;
            for(int i = 0; i < outputItem->getMaxPort(); i++) {
                ConfigItemUCI::Uci_data_t *outputData = new ConfigItemUCI::Uci_data_t(outputItem->getCfgType(),
                                                                                         outputItem->configTypeName().toUtf8(),
                                                                                         outputItem->customName().toUtf8(),
                                                                                         outputItem->getUserLabel(i).toUtf8(),
                                                                                         outputItem->getRealChannel(i),
                                                                                         idx/16, idx%16);
                outputData->s_gainRange.maxValue = 12;
                outputData->s_gainRange.minValue = -100;
                uci->addTabWidgetData(outputData);
                idx++;
            }
        }else {
            int page = idx/16;
            if(idx%16) page++;
            ConfigItemUCI::Uci_data_t *data = new ConfigItemUCI::Uci_data_t(cfgItem->getCfgType(),
                                                                             cfgItem->configTypeName().toUtf8(),
                                                                             cfgItem->customName().toUtf8(),
                                                                             QString("").toUtf8(),
                                                                             0, page, 0);
            uci->addTabWidgetData(data);
            if((cfgItem->getCfgType()&FCT_FAMILY_MASK) == CFG_TYPE_MIXER_4X1) {
                ConfigItemMixer *mixer = qobject_cast<ConfigItemMixer *>(cfgItem);
                if(cfgItem) {
                    data->setMixerNodesSwitch(mixer->getNodesSwitch());
                }
            }
            idx+=16;
        }
    }

    _UCIList.append(uci);

    MainWindow::instance()->addUserControlInterfaceView(uci, uci->customName());
    emit designItemChanged(this);
}

void ConfigScene::deleteCfgItems(QList<QGraphicsItem *> cfgItemList)
{
    _undoStack->push(new ItemsDeleteCommand(this, cfgItemList));
}

void ConfigScene::testSlot()
{
    QAction *act = qobject_cast<QAction *>(sender());
    if(act->text() == "delePage")
        deletePage(_pageList[0]);
    else
        setCurPage(act->text());
}

void ConfigScene::initConfigItems(bool bClearLayer)
{
    _cfgItemList.clear();
    _lineList.clear();
    _cpuGroupMap.clear();
    _cpuGroupIdx = 0;
    for(QGraphicsItem* item : items(Qt::AscendingOrder))
    {
        if(item->type() == DesignItem::Type)
        {
            ConfigItem* cItem = castToConfigItem(item);
            if(cItem != NULL)
            {
                if(bClearLayer){
                    cItem->clearLayer();
                }
                _cfgItemList.append(cItem);
            }
        }
        else if(item->type() == LineItem::Type)
        {
            LineItem* line = qgraphicsitem_cast<LineItem*>(item);
            if(line != NULL) _lineList.append(line);
        }
    }
}


QList<ConfigItem*> ConfigScene::getFirstLayerItems()
{
    QList<ConfigItem*> itemList;
    for(ConfigItem* cItem : _cfgItemList)
    {
        CfgType cType = cItem->getCfgType();
        if(ConfigItemFactory::isStartCfgItem(cType))
        {
            itemList.append(cItem);
        }
    }
    return itemList;
}


QList<ConfigItem*> ConfigScene::getNextLayerItems(ConfigItem* cfgItem)
{
    QSet<ConfigItem*> itemSet;
    for(ItemPort* port : cfgItem->getOutPortList())
    {
        //线条连接确定的下一层Item
        for(LineItem* line : _lineList)
        {
            if(port == line->getStartPort())
            {
                if(!line->getEndPort()) continue;
                ConfigItem* cItem = castToConfigItem(line->getEndPort()->parentItem());
                if(cItem != NULL) {
                    itemSet.insert(cItem);
                }else {
                    QSet<ConfigItem *> cfgList = foundByNextSerConItem(line->getEndPort());
                    if(cfgList.count()) {
                        itemSet|=cfgList;
                    }
                }
            }
        }
        //标签匹配确定的下一层
        LineLabel* label = port->getLineLabel();
        if(label!=NULL && label->isMatched())
        {
             QList<ConfigItem*> ItemList = getLabelMatchedItems(label);
             itemSet |= QSet<ConfigItem *> (ItemList.begin(), ItemList.end());
//             for(ConfigItem* cItem : ItemList) {
//                 itemSet.insert(cItem);
//             }
        }
    }
    return QList<ConfigItem *> (itemSet.begin(), itemSet.end());
//    return itemSet.toList();
}


void ConfigScene::setCfgItemLayer(ConfigItem* cfgItem, QList<ConfigItem*> pathItemList)
{
    if(pathItemList.length() >= _trnDev->maxConfigLayer()) {
        _maxConfigLayer = -1;
        return;
    }

    if(ConfigItemFactory::isEndCfgItem(cfgItem->getCfgType())) //链路完整结束
    {
        //设置Layer
        int layer = 0;
        for(; layer<pathItemList.count(); layer++)
        {
            pathItemList[layer]->setLayer(layer);
        }
        cfgItem->setLayer(layer);
        layer++;
        if(_maxConfigLayer < layer){
            _maxConfigLayer = layer;
        }

        //设置cpu分组
        pathItemList.append(cfgItem);
        setCpuGroup(pathItemList);
    }
    else
    {
        QList<ConfigItem*> nextItems = getNextLayerItems(cfgItem);
        if(nextItems.count() > 0)
        {
            QList<ConfigItem*> newCfgPath(pathItemList);
            newCfgPath.append(cfgItem);
            for(ConfigItem* item : nextItems)
            {
//                qDebug() << __FUNCTION__ << __LINE__ << item->customName();
                setCfgItemLayer(item, newCfgPath);
            }
        }
    }
}

void ConfigScene::setCpuGroup(QList<ConfigItem*> pathItemList)
{
    QList<int> groupIdxList;
    for(ConfigItem* item : pathItemList)
    {
        int cpuGroup = item->cpuGroupIdx();
        if(cpuGroup >= 0 && !groupIdxList.contains(cpuGroup) ) {
            groupIdxList.append(cpuGroup);
        }
    }

    if(groupIdxList.count()==0) //链中没有已分配的组，设置一个新组并保存
    {
        int newCpuGroup = _cpuGroupIdx++;
        for(ConfigItem* item : pathItemList)
        {
            item->setCpuGroupIdx(newCpuGroup);
        }
        _cpuGroupMap.insert(newCpuGroup, pathItemList.toSet());
    }
    else //链中有一个或多个已分配的组，合并到第一个分组
    {
        std::sort(groupIdxList.begin(), groupIdxList.end(), std::less<int>());
        int firstGroupIdx = groupIdxList[0];
        QSet<ConfigItem*> newItemsSet = _cpuGroupMap[firstGroupIdx];
        //合并当前链到第一个分组
        for(ConfigItem* item : pathItemList) {
            item->setCpuGroupIdx(firstGroupIdx);
            newItemsSet.insert(item);
        }
        //合并其它链到第一个组
        for(int i = 1; i<groupIdxList.count(); i++)
        {
            QSet<ConfigItem*> itemsSet = _cpuGroupMap[groupIdxList[i]];
            for(ConfigItem* item : itemsSet) {
                item->setCpuGroupIdx(firstGroupIdx);
            }
            newItemsSet.unite(itemsSet);
        }
    }
}

//设置Layer、CpuIndex;并计算
bool ConfigScene::checkDesign()
{
    if(_trnDev == NULL) return false;
    bool isValid = true;
    _maxConfigLayer = 0;
    //清除数据并设置算法Layer
    initConfigItems(true);
    QList<ConfigItem*> inputItems = getFirstLayerItems();
    QList<ConfigItem*> cfgPath;
    for(ConfigItem* cfgItem : inputItems)
    {
        setCfgItemLayer(cfgItem, cfgPath);
    }

    //校验算法的时间空间占用
    if(_maxConfigLayer != -1) {
        //设置AEC模块的CpuIndex，优先使用独立核
        int curCpuIndex = _trnDev->getCpuCount() -1;
        qreal cupTimeUsage = 0;
        for(ConfigItem* cItem : _cfgItemList)
        {
            int cfgTypeGroup = cItem->getCfgType()& 0x0000FF00;
            if(cfgTypeGroup == CFG_TYPE_SOFTAEC_8K64 || cfgTypeGroup == CFG_TYPE_SOFTAEC_IK_8K64 )
            {
                int timeUsage = Utils::getTimeUsage(_trnDev->getDeviceType(), cItem->getCfgType());
                if(cupTimeUsage + timeUsage > 100)
                {
                    if(curCpuIndex > 0){
                        curCpuIndex--;
                        cupTimeUsage = 0;
                    }
                }
                cItem->setCpuIndex(curCpuIndex);
                cupTimeUsage += Utils::getTimeUsage(_trnDev->getDeviceType(), cItem->getCfgType());
            }
        }

        qreal spaceUsage = 0;
        qreal timeUsage = 0;
        //如果AEC用到了主核，累加timeUsage
        if(curCpuIndex == 0){
            timeUsage += cupTimeUsage;
        }
        for(ConfigItem* cfgItem : _cfgItemList)
        {
            if(!cfgItem->isValid()) continue;
            spaceUsage += Utils::getSpaceUsage(_trnDev->getDeviceType(), cfgItem->getCfgType());
            timeUsage += Utils::getTimeUsage(_trnDev->getDeviceType(), cfgItem->getCfgType());
        }
//        qDebug() << __FUNCTION__ << __LINE__ << spaceUsage << timeUsage;
        if (spaceUsage > 100)
        {
            isValid = false;
            Utils::MessageBoxWarning(tr("Warning"),
                                  tr("The space usage of total module expeed the upper limit!"));
        }
        else if (timeUsage > 100)
        {
            isValid = false;
            Utils::MessageBoxWarning(tr("Warning"),
                                  tr("The time usage of total module expeed the upper limit!"));
        }

        if(isValid)
        {
           _spaceUsage = spaceUsage;
           _timeUsage = timeUsage;
           MainWindow::instance()->setUsagePercent(_spaceUsage, _timeUsage);
        }
    }
    else {
        isValid = false;
        Utils::MessageBoxWarning(tr("Warning"), tr("Exceed Max Layer:%1").arg(_trnDev->maxConfigLayer()));
    }
    return isValid;
}

QList<CtrlItemLua*> ConfigScene::prepareLuaAlgorithm()
{
    initConfigItems(false); //初始化线条
    QList<CtrlItemLua*> luaItemList;
    for(ConfigItem* cItem : _cfgItemList)
    {
        if(cItem->getCfgType() == CFG_TYPE_CTRL_LUA)
        {
            CtrlItemLua* luaItem = qobject_cast<CtrlItemLua*>(cItem);
            for(int i = 0; i< CtrlItemLua::MaxPinCount; i++)
            {
                CLuaPinParam_PC* inPinDef = getInCtrlPinDefTree(luaItem->_pinInList[i]);
                if(inPinDef != NULL)
                {
                    luaItem->_pinDefIn[i] = inPinDef;
                }
            }
            for(int i = 0; i< CtrlItemLua::MaxPinCount; i++)
            {
                QList<CLuaPinParam_PC*> pinDefList = getOutCtrlPinDefTree(luaItem->_pinOutList[i]);
                if(pinDefList.length()> 0)
                {
                    CLuaPinParam_PC* rootPinDef = new CLuaPinParam_PC(luaItem, "LuaItem", "LuaPin", 0xFFFFFFFF, 0xFFFFFFFF);
                    rootPinDef->setChildren(pinDefList);
                    luaItem->_pinDefOut[i] = rootPinDef;
                }
            }
            luaItemList.append(luaItem);
        }
    }

    return luaItemList;
}

bool ConfigScene::setAlgorithmMatrix()
{
    for(ConfigItem* cfgItem : _cfgItemList) {
        cfgItem->clearPortPara();
    }
    //将算法模块列行存储
    _algorithmMap.clear();
    quint32 moduleNum = 0;      //整个场景内，算法模块的唯一序号
    for (int layer = 0; layer < _maxConfigLayer; ++layer) /* 确保行列从最小的开始 */
    {
        int portRowIdx = 0;     //每个Layer内，连接端口序号
        int itemRowIdx = 0;     //每个Layer内，算法模块序号
        for(ConfigItem* cfgItem : _cfgItemList)
        {
            if (cfgItem->isValid() == false || cfgItem->getLayer() != layer) continue;
            cfgItem->setModuleNum(moduleNum++);
            quint16 key = (itemRowIdx << 8) | layer;
            itemRowIdx++;
            _algorithmMap.insert(key, cfgItem);
            int rows = setAlgorithmPortMatrix(cfgItem, layer, portRowIdx);
            portRowIdx += rows;
        }
        int MaxRowCnt = 256;
        if(_trnDev->is16BitProtocal()){
            MaxRowCnt = 65536;
        }
        if(portRowIdx > MaxRowCnt) {
            Utils::MessageBoxWarning(tr("Warning"), tr("Exceed Max Row:%1").arg(MaxRowCnt));
            return false;
        }
    }
    return true;
}

ItemPort* ConfigScene::getLinkedStartPort(ItemPort* port) //获取连接的起点端口
{
    for(LineItem* line : _lineList)
    {
//        qDebug() << __FUNCTION__ << __LINE__ << port->portName() << (port == line->getEndPort())
//                 << line->getStartPort()->portName() << line->getEndPort()->portName();
        if(port == line->getEndPort())
        {
            ConfigItem *cfgItem = castToConfigItem(line->getStartPort()->parentItem());
            if(cfgItem)
            {
                return line->getStartPort();
            }else {
                ItemPort *SerConPort = getLinkedStartPortBySerCon(line->getStartPort(), line->getStartPort()->portNum());
                if(SerConPort)
                    return SerConPort;
                else
                    qDebug() << __FUNCTION__ << __LINE__ << "port is Null!";
            }
        }
    }

    LineLabel* label = port->getLineLabel();
//    if(label != NULL){
//        qDebug() << __FUNCTION__ << __LINE__ << label->name() << label->isInput() << label->isMatched();
//    }
    if(label!=NULL && label->isMatched())
    {
        ItemPort *startPort = getLabelMatchedStartPort(label);
        if(startPort->parentItem()->type() == ConfigItem::Type)
            return startPort;
        else if (startPort->parentItem()->type() == SerpentineConnector::Type)
            return getLinkedStartPortBySerCon(startPort, startPort->portNum());

    }
    return NULL;
}

QList<ItemPort*> ConfigScene::getLinkedEndPort(ItemPort* port) //获取连接的终点端口
{
    QList<ItemPort*> endPortList;
    for(LineItem* line : _lineList)
    {
        if(port == line->getStartPort())
        {
            endPortList.append(line->getEndPort());
        }
    }

    LineLabel* label = port->getLineLabel();
    if(label!=NULL && label->isMatched())
    {
         endPortList.append(getLabelMatchedEndPort(label));
    }
    return endPortList;
}


QList<ConfigItem*> ConfigScene::getLabelMatchedItems(LineLabel* labelOut)
{
    Q_ASSERT(labelOut->isInput() == false);
    QList<ConfigItem*> itemList;
    QString labelName = labelOut->name();
    for(LineLabel* label : _labelList)
    {
        if(label->isMatched() && label->isInput())
        {
            if(label->name() != labelName) continue;
            ConfigItem* cItem = castToConfigItem(label->parentItem());
            if(cItem != NULL) {
                itemList.append(cItem);
            }
            else {
                QSet<ConfigItem *> cfgList = foundByNextSerConItem(label->getLabelPort());
                if(cfgList.count()) {
                    itemList += QList<ConfigItem *> (cfgList.begin(), cfgList.end());
                }
            }
        }
    }
//    foreach(ConfigItem *item, itemList)
//        qDebug() << __FUNCTION__ << __LINE__ << item->customName();
    return itemList;
}



ItemPort* ConfigScene::getLabelMatchedStartPort(LineLabel* label)
{
    Q_ASSERT(label->isInput());
    QString labelName = label->name();
    for(LineLabel* label : _labelList)
    {
        if(label->isMatched() && !label->isInput())
        {
            if(label->name() == labelName) {
                return label->getLabelPort();
            }
        }
    }
    return NULL;
}

QList<ItemPort*> ConfigScene::getLabelMatchedEndPort(LineLabel* label)
{
    Q_ASSERT(label->isInput() == false);
    QList<ItemPort*> endPortList;
    QString labelName = label->name();
    for(LineLabel* label : _labelList)
    {
        if(label->isMatched() && label->isInput())
        {
            if(label->name() == labelName) {
                endPortList.append(label->getLabelPort());
            }
        }
    }
    return endPortList;
}

int ConfigScene::setAlgorithmPortMatrix(ConfigItem* item,  int colIdx, int rowIdx)
{
    int rowIdxNew = rowIdx;
    QList<ItemPort*> portList = item->getInPortList();
    for(ItemPort* port : portList)
    {
        ItemPort* linkedPort =  getLinkedStartPort(port);

        if(linkedPort == NULL) continue;
        ConfigItem*  linkedItem = castToConfigItem(linkedPort->parentItem());
        if(linkedItem == NULL) continue;

        int colFrom, rowFrom;
        linkedItem->getOutputDef(linkedPort->portNum(), &colFrom, &rowFrom);
//        qDebug() << __FUNCTION__ << __LINE__ << (colFrom != 0xFFFF) << (rowFrom != 0xFFFF);
        //每个端口占用输出行列中的一行, 多条连线的共用一行
        if (colFrom != 0xFFFF && rowFrom != 0xFFFF)
        {
            item->setInputDef(port->portNum(), colFrom, rowFrom);
        }
        else
        {
            linkedItem->setOutputDef(linkedPort->portNum(), colIdx, rowIdxNew);
            item->setInputDef(port->portNum(), colIdx, rowIdxNew);
            rowIdxNew++;
        }
    }

    return rowIdxNew - rowIdx; //返回新增数量
}

QSet<ConfigItem *> ConfigScene::foundByNextSerConItem(ItemPort *portItem)
{
    QSet<ConfigItem *> cfgList;
    SerpentineConnector *serConItem = qgraphicsitem_cast<SerpentineConnector*>(portItem->parentItem());

    if(!serConItem) return cfgList;

    foreach(ItemPort *Pitem, serConItem->getOutPortList()) {
        //通过连线确定下一层模块
        for (LineItem *Litem : _lineList) {
            if(Litem->getStartPort() == Pitem) {
                ConfigItem *cfgItem = castToConfigItem(Litem->getEndPort()->parentItem());
                if(cfgItem)
                    cfgList.insert(cfgItem);
                else
                    cfgList|=foundByNextSerConItem(Litem->getEndPort());
            }
        }
        //通过标签确定下一层模块
        LineLabel *label = Pitem->getLineLabel();
        if(label != NULL && label->isMatched()) {
            QList<ConfigItem*> ItemList = getLabelMatchedItems(label);
            cfgList |= QSet<ConfigItem *> (ItemList.begin(), ItemList.end());
        }
    }

    return cfgList;
}

ItemPort *ConfigScene::getLinkedStartPortBySerCon(ItemPort *portItem, int portNum)
{
    SerpentineConnector *serConItem = qgraphicsitem_cast<SerpentineConnector*>(portItem->parentItem());
    if(!serConItem) return NULL;
    ItemPort *ChannelBus = NULL;
    if(serConItem->getInPortList().count() == 1)
        ChannelBus = serConItem->getInPortList().at(0);
    if(serConItem->getInPortList().count() > 1)
        ChannelBus = serConItem->getInPortList().at(portNum);
    if(!ChannelBus) return NULL;

    for(LineItem *Litem : _lineList) {
        if(Litem->getEndPort() == ChannelBus){
            ConfigItem *cfgItem = castToConfigItem(Litem->getStartPort()->parentItem());
            if(cfgItem) return Litem->getStartPort();
            else {
                ItemPort *serConPort = getLinkedStartPortBySerCon(Litem->getStartPort(), portNum);
                if(serConPort) return serConPort;
            }
        }
    }

    LineLabel* label = ChannelBus->getLineLabel();
    if(label!=NULL && label->isMatched())
    {
         ItemPort *port = getLabelMatchedStartPort(label);

         if(port != NULL && port->parentItem()->type() == ConfigItem::Type)
             return port;
         else if(port->parentItem()->type() == SerpentineConnector::Type) {
             ItemPort *serConPort = getLinkedStartPortBySerCon(port, portNum);
             if(serConPort) return serConPort;
         }

    }

    return NULL;
}

bool ConfigScene::isPointInArea(QRectF area, QPointF point)
{
    if (point.x() < area.left()) return false;
    if (point.y() < area.top()) return false;
    if (point.x() > area.right()) return false;
    if (point.y() > area.bottom()) return false;

    return true;
}


ConfigItem* ConfigScene::castToConfigItem(QGraphicsItem* item)
{
    DesignItem* dItem = qgraphicsitem_cast<DesignItem*>(item);
    ConfigItem* cItem = qobject_cast<ConfigItem*>(dItem);
    return cItem;
}


bool ConfigScene::readConfigItem(int nmuVersion, QSettings *ConfigIni, QString itemGroup)
{
    ConfigIni->beginGroup(itemGroup);
    CfgType cfgType    = (CfgType)ConfigIni->value(QString(CONFIG_DEVICE_DEVTYPE)).toInt();
    QPointF scenePos   = ConfigIni->value(QString(CONFIG_DEVICE_DEVPOS)).toPointF();
    QString configname = ConfigIni->value(QString(CONFIG_DEVICE_DEVNAME)).toString();
    QColor color       = ConfigIni->value(QString(CONFIG_DEVICE_LABEL_COLOR)).value<QColor>();
    int zValue         = ConfigIni->value(QString(CONFIG_DEVICE_MODULE_ZVALUE)).toInt();
    if(zValue > _trnDev->getItemUserZValueNum(false))
         _trnDev->setItemUserZValueNum(zValue);
    ConfigItem *cfgItem;
    if ((cfgType & FCT_FAMILY_MASK) == CFG_TYPE_INPUT_1) {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_INPUT_GROUP));
        int channel = ConfigIni->value(QString(CONFIG_DEVICE_DEV_INPUT_CHANNEL)).toInt();
        ConfigIni->endGroup();
        cfgItem = _trnINF->getInOutItemByChannel(nmuVersion, cfgType, channel);

        if(cfgItem ==nullptr) return false;
        cfgItem->setPos(scenePos);
        cfgItem->setCustomName(configname);
    }
    else if ((cfgType & FCT_FAMILY_MASK) == CFG_TYPE_OUTPUT_1) {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_OUTPUT_GROUP));
        int channel = ConfigIni->value(QString(CONFIG_DEVICE_DEV_OUTPUT_CHANNEL)).toInt();
        ConfigIni->endGroup();
        cfgItem = _trnINF->getInOutItemByChannel(nmuVersion, cfgType, channel);
        if(cfgItem ==nullptr) return false;
        cfgItem->setPos(scenePos);
        cfgItem->setCustomName(configname);
    }
    else if((cfgType & FCT_FAMILY_MASK) == CFG_TYPE_SNAPSHOT) {
        foreach(ConfigItem *item, _trnINF->getSnapshotList()) {
            if(item->getCfgType() == cfgType && item->customName() == configname) {
                cfgItem = item;
                break;
            }
        }
        if(cfgItem == nullptr) return false;
        cfgItem->setPos(scenePos);
    }
    else {
        cfgItem = ConfigItemFactory::createConfigItem(_trnDev, CfgType(cfgType), configname, scenePos);
    }
//    cfgItem->initAfterAdd();
    if(cfgItem) {
        addItem(cfgItem);
        cfgItem->loadParaFromSetting(ConfigIni);
        ConfigIni->endGroup();
        cfgItem->setFillColor(color);
        if(nmuVersion)
            cfgItem->setUserZValue(zValue);
    }
    return true;
}


bool ConfigScene::readLineItem(int nmuVersion, QSettings *ConfigIni, QString itemGroup)
{
    ConfigIni->beginGroup(itemGroup);
    QPointF startPoint = ConfigIni->value(QString(CONFIG_LINE_STARTPOS)).toPointF();
    QPointF endPoint   = ConfigIni->value(QString(CONFIG_LINE_ENDPOS)).toPointF();
    int startZValue = ConfigIni->value(QString(CONFIG_LINE_STARTZVALUE)).toInt();
    int endZValue   = ConfigIni->value(QString(CONFIG_LINE_ENDZVALUE)).toInt();
    ConfigIni->endGroup();

    ItemPort *startPort = NULL;
    ItemPort *endPort = NULL;
    foreach (QGraphicsItem *item, items(startPoint))
    {
        if (item->type() == ItemPort::Type)
        {
            bool same = false;
            if(item->parentItem()->type() == ConfigItem::Type)
                same = qgraphicsitem_cast<ConfigItem *>(item->parentItem())->checkZValue() == startZValue;
            else if(item->parentItem()->type() == SerpentineConnector::Type)
                same = qgraphicsitem_cast<SerpentineConnector *>(item->parentItem())->checkZValue() == startZValue;
            if(nmuVersion == 0 ? true : same) {
                startPort = qgraphicsitem_cast<ItemPort *>(item);
                break;
            }
        }
    }
    foreach (QGraphicsItem *item, items(endPoint))
    {
        if (item->type() == ItemPort::Type)
        {
            bool same = false;
            if(item->parentItem()->type() == ConfigItem::Type)
                same = qgraphicsitem_cast<ConfigItem *>(item->parentItem())->checkZValue() == endZValue;
            else if(item->parentItem()->type() == SerpentineConnector::Type)
                same = qgraphicsitem_cast<SerpentineConnector *>(item->parentItem())->checkZValue() == endZValue;
            if(nmuVersion == 0 ? true : same) {
                endPort = qgraphicsitem_cast<ItemPort *>(item);
                break;
            }
        }
    }

    if(startPort== nullptr || endPort== nullptr){
        return false;
    }

    LineItem *line = new LineItem(this, startPort, endPort);
    addItem(line);
    line->initAfterAdd();
    line->setNormalization();//hrp
    return true;
}



