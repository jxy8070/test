#include "stable.h"
#include <windows.h>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QTreeWidget>
#include <QFileIconProvider>
#include "config.h"
#include "mainwindow.h"
#include "projectview.h"
#include "projectscene.h"
#include "configview.h"
#include "configscene.h"
#include "solutionview.h"
#include "solutionscene.h"
#include "configItem/configitem.h"
#include "devicetableview.h"
#include "toolkittree.h"
#include "UserManager/usermanagement.h"
#include "UserManager/logindialog.h"
#include "UserManager/passwordcheck.h"
#include "common.h"
#include "monitor/monitorwindows.h"
#include "device/devicetable.h"
#include "devicelinkview.h"
#include "deviceSetting/trn/trn_scenewidget.h"
#include "deviceItem/deviceitem_trn.h"
#include "device/AES70device.h"
#include "device/device_trn.h"
#include <QSplitter>
#include "sysdbutility.h"
#include "propertywidget.h"
#include "solutiondockwidget.h"
#include "customWidget/borderdock.h"
#include "usercontrolinterfaceview.h"
#include "usercontrolinterfacescene.h"
#include "monitor/resourcemanager.h"
#include "logsmanager.h"
#include "device/devicemanager.h"
#include "deviceItem/deviceitem_room.h"
#include "UserManager/newsolution.h"


#define HOT_KEY_TEST_SKIN 100 /*Ctrl+Shift+Q*/

MainWindow::MainWindow()
{
    _lastViewType = VIEW_NONE;
    setContextMenuPolicy(Qt::NoContextMenu);
    setupUi(this);
    bool rs = RegisterHotKey(HWND(winId()), HOT_KEY_TEST_SKIN, MOD_SHIFT | MOD_CONTROL, 0x51); //热键功能Ctrl+Shift+Q
    initDockWidgets();

    setWindowTitle(Config::curMfrsCfg().TitleHeader);
    //初始化状态栏
    initStatusBar();
    hadeMenuToolbars();

    //设置undo redo
    _undoGroup = new QUndoGroup(this);
    _actionUndo = _undoGroup->createUndoAction(this, tr("Undo"));
    _actionRedo = _undoGroup->createRedoAction(this, tr("Redo"));
    _actionUndo->setShortcut(QKeySequence::Undo);
    _actionRedo->setShortcut(QKeySequence::Redo);
    _actionUndo->setIcon(QIcon(":/Image/undo.png"));
    _actionRedo->setIcon(QIcon(":/Image/redo.png"));
    _menuEdit->addAction(_actionUndo);
    _menuEdit->addAction(_actionRedo);
    setSkinStyle();
    //根据配置显示Skin菜单
    if(Config::curMfrsCfg().SkinTypeList.count() > 1)
    {
        QMenu* skinMenu = new QMenu(tr("Skin Style"));
        _menuSetting->insertMenu(_actionDownRouter, skinMenu);
        QActionGroup* skinActionGroup = new QActionGroup(this); //样式设置Group实现互斥
        for(SkinType skinType : Config::curMfrsCfg().SkinTypeList)
        {
            QString skinName = SkinStyle::getSkinName(skinType);
            QAction* skinAction = new QAction(skinName);
            skinActionGroup->addAction(skinAction);
            skinAction->setCheckable(true);
            if(skinType == Config::curSkinType()){
                skinAction->setChecked(true);
            }
            skinAction->setData(skinType);
            skinMenu->addAction(skinAction);
            connect(skinAction, SIGNAL(triggered()), this, SLOT(setSkinStyle()));
        }
    }
    //文件菜单
    connect(_actionNew, SIGNAL(triggered()), this, SLOT(fileNewClicked()));
    connect(_actionImport, SIGNAL(triggered()), this, SLOT(fileImportClicked()));
    connect(_actionClose, SIGNAL(triggered()), this, SLOT(fileCloseClicked()));
    connect(_actionSave, SIGNAL(triggered()), this, SLOT(fileSaveClicked()));
    connect(_actionSaveAll, SIGNAL(triggered()), this, SLOT(saveAllSubWindow()));
    connect(_actionSaveToDev, SIGNAL(triggered()), this, SLOT(fileSaveToDeviceClicked()));
    connect(_actionLoadFromDev, SIGNAL(triggered()), this, SLOT(loadFromDeviceClicked()));
    connect(_actionExport, SIGNAL(triggered()), this, SLOT(fileExportClicked()));
    connect(_actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(_action_ResourceManager, SIGNAL(triggered()), this, SLOT(resourceManagerClicked()));
    //配置菜单
    connect(_actionLoadCfgSPK, SIGNAL(triggered()), this, SLOT(loadAllSpeadkerConfig()));
    connect(_actionLoadCfgAMP, SIGNAL(triggered()), this, SLOT(loadAllAmplifierConfig()));
    connect(_actionLoadCfgTRN, SIGNAL(triggered()), this, SLOT(loadAllControllerConfig()));
    //查看菜单
    connect(_actionResetViews, SIGNAL(triggered()), this, SLOT(onResetViews()));
    connect(_actionStandard, SIGNAL(triggered(bool)), _mainToolBar, SLOT(setVisible(bool)));
    connect(_actionStatusBar, SIGNAL(triggered(bool)), _mainStatusBar, SLOT(setVisible(bool)));
    connect(_actionDeviceLine, SIGNAL(triggered()), this, SLOT(viewDeviceLineItem()));

    //防火墙操作
    connect(actionAdd_to_whitelist, &QAction::triggered, this, &MainWindow::Firewall_Settings);
    connect(actionRemove_whitelist, &QAction::triggered, this, &MainWindow::Firewall_Settings);
    QActionGroup* langActionGroup = new QActionGroup(this); //语言设置Group实现互斥
    langActionGroup->addAction(actionChinese);
    langActionGroup->addAction(actionEnglish);
    if (Config::instance()->getLanguage() == LANGUAGE_ENGLISH) {
        actionEnglish->setChecked(true);
    }else{
        actionChinese->setChecked(true);
    }
    connect(actionChinese, SIGNAL(triggered()), this, SLOT(setLanguage()));
    connect(actionEnglish, SIGNAL(triggered()), this, SLOT(setLanguage()));

    connect(_actionSolutionManage, SIGNAL(triggered()), this, SLOT(openWelcomeDlg()));
    connect(_actionUserLock, SIGNAL(triggered()), this, SLOT(settingUserLockClicked()));
    connect(_actionUserManage, SIGNAL(triggered()), this, SLOT(settingUserManageClicked()));
    connect(_actionUserPassword, SIGNAL(triggered()), this, SLOT(settingUserPasswordClicked()));
    connect(_actionDownRouter, SIGNAL(triggered()), this, SLOT(downloadRouter()));
    connect(_actionDownAlgorithm, SIGNAL(triggered()), this, SLOT(downloadAlgorithm()));
    connect(_actionAudioMute, SIGNAL(triggered(bool)), this, SLOT(setAudioMute(bool)));
    connect(_actionClearOfflineDev, SIGNAL(triggered()), this, SLOT(clearOfflineDevice()));
    connect(_actionCtrlCenterUtil, SIGNAL(triggered()), this, SLOT(openCtrlCenterUtil()));

    //窗口菜单
    connect(_actionCascade, SIGNAL(triggered()), _mdiArea, SLOT(cascadeSubWindows()));
    connect(_actionTile, SIGNAL(triggered()), _mdiArea, SLOT(tileSubWindows()));
    connect(_actionMaximize, SIGNAL(triggered()), this, SLOT(subWindowShowMaximize()));

    /* about菜单 */
    connect(_actionAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));
    //Help
    connect(_actionSoftwareOperationGuide, SIGNAL(triggered()), this, SLOT(helpDocuments()));
    connect(_actionFunctionModule, SIGNAL(triggered()), this, SLOT(helpDocuments()));
    _mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation);//取消下一个被激活的子窗口的最大化行为
    _mdiArea->setViewMode(QMdiArea::TabbedView);        //再选项卡中显示带有选项卡的子窗口
    _mdiArea->setTabShape(QTabWidget::Rounded);         //设置table标签样式 Rounded  or Triangular
    _mdiArea->setTabsClosable(true);                    //显示table标签的关闭按钮
    _mdiArea->setAttribute(Qt::WA_StyledBackground);
    _mdiArea->setTabsMovable(true);
    _mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);
    QTabBar* tabBar = _mdiArea->findChild<QTabBar*>();  //设置tab标签不扩展
    tabBar->setExpanding(false);
    connect(_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onSubWindowActivated(QMdiSubWindow*)));

    /* 最大化窗口，解决小屏幕显示问题 */
    showMaximized();

    connect(this, SIGNAL(needInsertAlarm(QString,int,QString)), Config::instance()->getAlarmDBBase(),
            SLOT(insertAlarmInfo(QString,int,QString)));

    //对齐控件
    connect(actionAlignLeft, SIGNAL(triggered()), this, SLOT(setItemsAlign()));
    connect(actionAlignRight, SIGNAL(triggered()), this, SLOT(setItemsAlign()));
    connect(actionAlignTop, SIGNAL(triggered()), this, SLOT(setItemsAlign()));
    connect(actionAlignBottom, SIGNAL(triggered()), this, SLOT(setItemsAlign()));
    connect(actionAlignEqHSpace, SIGNAL(triggered()), this, SLOT(setItemsAlign()));
    connect(actionAlignEqVSpace, SIGNAL(triggered()), this, SLOT(setItemsAlign()));

    //缩放控件
    connect(actionZoomIn, &QAction::triggered, this, &MainWindow::onZoomView_slots);
    connect(actionZoomOut, &QAction::triggered, this, &MainWindow::onZoomView_slots);

    actionCursor->setVisible(true);
    actionHandle->setVisible(true);
    connect(actionCursor, &QAction::triggered, this, &MainWindow::mouseStateSet);
    connect(actionHandle, &QAction::triggered, this, &MainWindow::mouseStateSet);
}

void MainWindow::hadeMenuToolbars()
{
    _actionNew->setEnabled(false);
    _actionSave->setEnabled(false);
    _actionSaveAll->setEnabled(false);
    _actionImport->setEnabled(false);
    _actionExport->setEnabled(false);
    _actionSaveToDev->setVisible(false);
    _actionLoadFromDev->setVisible(false);
    _action_ResourceManager->setVisible(false);
    _actionLoadCfgSPK->setVisible(false);
    _actionLoadCfgAMP->setVisible(false);
    _actionLoadCfgTRN->setVisible(false);
    _menuEdit->menuAction()->setVisible(false);
    _actionDeviceLine->setVisible(false);
    _actionDownRouter->setVisible(false);
    _actionDownAlgorithm->setVisible(false);
    _actionAudioMute->setVisible(false);
    _menuWindow->menuAction()->setVisible(false);

    _actionLogs->setVisible(false);
    _actionDbDev->setVisible(false);
    _actionDevType->setVisible(false);
    _actionProperties->setVisible(false);
    _actionProperties_cfg->setVisible(false);
    _actionDevModel->setVisible(false);
    _actionComponents->setVisible(false);
    _actionComponentControls->setVisible(false);
}

void MainWindow::onResetViews()
{
    _actionLogs->setChecked(true);
    _actionDbDev->setChecked(true);
    _actionDevType->setChecked(true);
    _actionProperties->setChecked(true);
    _actionProperties_cfg->setChecked(true);
    _actionDevModel->setChecked(true);
    _actionComponents->setChecked(true);
    _actionComponentControls->setChecked(true);

    SubWnd_Type viewType = VIEW_NONE;
    ConfigScene *cfgScene = NULL;
    QMdiSubWindow *curSubWnd =  _mdiArea->currentSubWindow();
    if (curSubWnd != NULL)//返回当前子窗口的指针
    {
       AbsGraphView *subView = qobject_cast<AbsGraphView *>(curSubWnd->widget());
       if(!subView) return;
       viewType = subView->getType();
       if(viewType == VIEW_CONFIG){
           ConfigView* cfgView = qobject_cast<ConfigView*>(subView);
           cfgScene = cfgView->scene();
       }
    }
    updateDockWidgets(viewType, cfgScene);
}


//初始化状态栏
void MainWindow::initStatusBar()
{
    for(int i = 0; i < STATUSBAR_COUNT; ++i)
    {
        QLabel* qlable = new QLabel;
        if(i ==0)
        {
            statusBar()->addWidget(qlable, true);
        }
        else{
            statusBar()->addWidget(qlable, false);
        }
        _statusLabelList.append(qlable);
    }
    _statusLabelList.at(STATUSBAR_STATE)->setText(tr("Ready"));
    _statusLabelList.at(STATUSBAR_SHEET_SIZE)->setText(tr("Sheet Size: 1130x1600"));
    _statusLabelList.at(STATUSBAR_DEVICE_STATUS)->setText(tr(""));
    _statusLabelList.at(STATUSBAR_SPACE_USAGE)->setText(tr(""));
    _statusLabelList.at(STATUSBAR_TIME_USAGE)->setText(tr(""));
    _statusLabelList.at(STATUSBAR_SCALING)->setText(tr("Scaling:%1%").arg(100));

    //在状态栏显示网络信息
    foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        QList<QNetworkAddressEntry>entryList = netInterface.addressEntries();
        foreach(QNetworkAddressEntry entry, entryList)
        {
            if (entry.ip().toString() == Config::instance()->getCurIp())
            {
                quint32 netMark = entry.netmask().toIPv4Address();
                quint32 localIp32 = entry.ip().toIPv4Address();
                quint32 startIp32 = (localIp32 & netMark) + 1;
                quint32 endIp32 = startIp32 - 1 + (~netMark) &0xFFFFFFFF;
                QString startIp = QHostAddress(startIp32).toString();
                QString endIp =QHostAddress(endIp32).toString();
                _statusLabelList.at(STATUSBAR_START_IP)->setText(tr("Start IP:%1").arg(startIp));
                _statusLabelList.at(STATUSBAR_END_IP)->setText(tr("End IP:%1").arg(endIp));
                _statusLabelList.at(STATUSBAR_ADAPTER_IP)->setText(tr("Adapter IP:%1").arg(entry.ip().toString()));
            }
        }
    }
}

void MainWindow::saveAllSubWindow()
{
    for(QMdiSubWindow *subWind : _mdiArea->subWindowList())
    {
        AbsGraphView* subView = qobject_cast<AbsGraphView*>(subWind->widget());
        if(subView != NULL) {
            subView->saveScene(false);
        }
    }
}

void MainWindow::closeAllSubWindow()
{
    bool isEdit = false;
    for(QMdiSubWindow *subWind : _mdiArea->subWindowList())
    {
        AbsGraphView* subView = qobject_cast<AbsGraphView*>(subWind->widget());
        if(subView != NULL) {
            if(subView->windowTitle().endsWith("*")) {
                isEdit = true;
                break;
            }
        }
    }
    if(isEdit){
        QMessageBox::StandardButton rsBtn = Utils::MessageBoxQuestion(tr("Question"), tr("Do you want to save?"),
                               QMessageBox::Yes  | QMessageBox::No );
        if (rsBtn == QMessageBox::Yes) {
            saveAllSubWindow();
        }
    }

    for(QMdiSubWindow *subWind : _mdiArea->subWindowList(QMdiArea::CreationOrder))
    {
        subWind->close();
    }
}

void MainWindow::openSolution(QString slnPath)
{
    if(_slnDockBox->hasOpendSolution())
    {
        //选择不关闭已打开解决方案，则退出
        if(Utils::MessageBoxQuestion(tr("Infomation"),  tr("Whether to close the current solution?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
            return ;
        }
    }
    closeAllSubWindow();
    QFileInfo fileInfo(slnPath);
    QString sfx = fileInfo.suffix();
    if(sfx == "sln") {
        _slnDockBox->setSlnFileName(fileInfo.fileName());
        _slnDockBox->setSolutionDir(fileInfo.absolutePath());
    }else {
        _slnDockBox->setSlnFileName("Solution.sln");
        _slnDockBox->setSolutionDir(slnPath);
    }
    QStringList pageNames;
    QString activeWinName;
    Utils::loadCurOpenPagesData(pageNames, activeWinName);
    if(pageNames.length() == 0)
        pageNames.append(_slnDockBox->getSolutionFilePath());


    Config::ShowProgressDialog = false;
    QProgressDialog *progressDlg =  new QProgressDialog;//关于缓慢操作的一个进度反馈
    progressDlg->setWindowModality(Qt::ApplicationModal);//模态窗口
    progressDlg->setMinimumDuration(1);//以1秒的延时进入进度条
    progressDlg->setWindowTitle(tr("please wait"));
    progressDlg->setLabelText(tr("Loading file......"));
    progressDlg->setCancelButton(0);//取消按钮不会被显示
    progressDlg->setRange(0, pageNames.count()+1);
    int proCount = 1;

    foreach (QString pageName, pageNames) {
        QString filePath = getSolutionPath(pageName);
        if(QFile::exists(filePath)) {
            openSubWindow(getSolutionPath(pageName));
        }
        progressDlg->setValue(proCount++);
    }
    foreach (QMdiSubWindow *subWin, _mdiArea->subWindowList()) {
        if(subWin->windowTitle() == activeWinName) {
            _mdiArea->setActiveSubWindow(subWin);
            break;
        }
    }
    progressDlg->setValue(proCount++);
    Config::ShowProgressDialog = true;

    Utils::saveProjectExplorer(slnPath);
}


void MainWindow::reflashSolution()
{
    _slnDockBox->updateSlnTree();
}

void MainWindow::initDockWidgets()
{
    QString logInfo = QString(tr("%1 login disigner")).arg(Config::instance()->getCurUser().name);
    LogsManager::instance()->addLogs(tr("Info"), tr("System"), logInfo);

    //解决方案
    _slnDockBox = new SolutionDockWidget();
    _slnDockBox->setWindowTitle(tr("Solution"));
    connect(_slnDockBox, SIGNAL(openSubWindow(QString)), this, SLOT(openSubWindow(QString)));
    connect(_slnDockBox, SIGNAL(deleteSubWindow(QString)), this, SLOT(deleteSubWindow(QString)));
    connect(_slnDockBox, SIGNAL(renameSubWindow(QString, QString)), this, SLOT(renameSubWindow(QString, QString)));

    //属性
    _propertyDockBox = new PropertyWidget();
    _propertyDockBox->setWindowTitle(tr("Properties"));
    connect(_actionProperties, SIGNAL(triggered(bool)), _propertyDockBox, SLOT(setVisible(bool)));      //设备模块属性栏
    connect(_actionProperties_cfg, SIGNAL(triggered(bool)), _propertyDockBox, SLOT(setVisible(bool)));  //算法模块属性栏
    connect(_propertyDockBox, &PropertyWidget::closeSignal, this, [=](bool b){
        AbsGraphView *qView = qobject_cast<AbsGraphView *>(_mdiArea->currentSubWindow()->widget());
        if(!qView) return ;
        if(qView->getType() == VIEW_PROJECT) {
            _actionProperties->setChecked(b);
        }else if(qView->getType() == VIEW_CONFIG) {
            _actionProperties_cfg->setChecked(b);
        }
    });

    //运行日志
    _logsDockBox = new DockWidget(tr("Logs"), this);
    _logsDockBox->move(600, 300);
    QTableView* logView = LogsManager::instance()->getLogsView();
    _logsDockBox->setWidget(logView);
    connect(_actionLogs, SIGNAL(triggered(bool)), _logsDockBox, SLOT(setVisible(bool)));
    connect(_logsDockBox, SIGNAL(closeSignal(bool)), _actionLogs, SLOT(setChecked(bool)));

    //数据库保存及网络发现的设备
    _dbDevDockBox = new DockWidget(tr("devices finded"));
    _dbDevDockBox->setObjectName("dbDevDockBox");
    QWidget * titleBarWidget = _dbDevDockBox->titleBarWidget(); //隐藏QDockWidget标题
    _dbDevDockBox->setTitleBarWidget( NULL );
    delete titleBarWidget;
    DeviceTableView* dbDevView = new DeviceTableView();
    dbDevView->horizontalHeader()->setVisible(true);        //设备列表的水平标题栏可见
    dbDevView->verticalHeader()->setVisible(true);
    _dbDevDockBox->setWidget(dbDevView);
    connect(_actionDbDev, SIGNAL(triggered(bool)), _dbDevDockBox, SLOT(setVisible(bool)));
    connect(_dbDevDockBox, SIGNAL(closeSignal(bool)), _actionDbDev, SLOT(setChecked(bool)));

    connect(_slnDockBox, SIGNAL(renameSubWindow(QString, QString)), dbDevView, SLOT(deviceCurRoomNameChangeds(QString, QString)));
    connect(_slnDockBox, SIGNAL(deleteSubWindow(QString)), dbDevView, SLOT(deleteDeviceCurRoomName(QString)));

    //按类型分类的设备
    _devTypeDockBox = new DockWidget(tr("Devices"));
    //    _deviceDockBox->setMinimumHeight(400);
    ToolKitTree *devTree = new ToolKitTree(ToolKitTree::TreeStrType_DevModule);
    devTree->SetTreeItems(ToolKitTree::GetDeviceItems());
    _devTypeDockBox->setWidget(devTree);
    _devTypeDockBox->setVisible(_actionDevType->isChecked());
    connect(_actionDevType, SIGNAL(triggered(bool)), _devTypeDockBox, SLOT(setVisible(bool)));
    connect(_devTypeDockBox, SIGNAL(closeSignal(bool)), _actionDevType, SLOT(setChecked(bool)));

    //设备固定模块，输入输出等
    _devModelDockBox = new DockWidget(tr("Device Interface"));
    //    _deviceDockBox->setMinimumHeight(400);
    ToolKitTree *treeWidget = new ToolKitTree(ToolKitTree::TreeStrType_DevInterface);
    treeWidget->setSelectionMode(QAbstractItemView::ContiguousSelection); //MultiSelection   ExtendedSelection
    _devModelDockBox->setWidget(treeWidget);
    connect(_actionDevModel, SIGNAL(triggered(bool)), _devModelDockBox, SLOT(setVisible(bool)));
    connect(_devModelDockBox, SIGNAL(closeSignal(bool)), _actionDevModel, SLOT(setChecked(bool)));

    //算法模块
    _modelDockBox = new ModelDockWidget(tr("Components"));
//    _modelDockBox->setMinimumHeight(400);
    connect(_actionComponents, SIGNAL(triggered(bool)), _modelDockBox, SLOT(setVisible(bool)));
    connect(_modelDockBox, SIGNAL(closeSignal(bool)), _actionComponents, SLOT(setChecked(bool)));
    connect(_modelDockBox, SIGNAL(sendToolBtnSignal(ConfigScene::ToolkitType)), this, SLOT(onToolBtnCheckedSlot(ConfigScene::ToolkitType)));

    //模块开放的控制信息
    _pinsViewerDockBox = new DockWidget(tr("Component Controls Info"), this);
    _pinsViewerDockBox->move(600, 300);
    QWidget* infoWidget = ConfigItem::getControlsInfoWidget(NULL);
    _pinsViewerDockBox->setWidget(infoWidget);
    connect(_actionComponentControls, SIGNAL(triggered(bool)), _pinsViewerDockBox, SLOT(setVisible(bool)));
    connect(_pinsViewerDockBox, SIGNAL(closeSignal(bool)), _actionComponentControls, SLOT(setChecked(bool)));

    if(Config::instance()->curMfrsCfg().CurMfrs == Mfrs_FangTu) {
        //左边dockWidget
        addDockWidget(Qt::LeftDockWidgetArea, _slnDockBox);
        splitDockWidget(_slnDockBox, _devTypeDockBox, Qt::Vertical);
        splitDockWidget(_slnDockBox, _devModelDockBox, Qt::Vertical);
        //右边dockWidget
        addDockWidget(Qt::RightDockWidgetArea, _propertyDockBox);
        splitDockWidget(_propertyDockBox, _modelDockBox, Qt::Vertical);
        tabifyDockWidget(_modelDockBox, _pinsViewerDockBox);
        //下边dockWidget
        addDockWidget(Qt::BottomDockWidgetArea, _dbDevDockBox);
        tabifyDockWidget(_dbDevDockBox, _logsDockBox);
        _dbDevDockBox->raise();

        //侧边停靠栏
        _toolBarL = new BorderDock(Qt::LeftToolBarArea, this);
        _toolBarL->addDockBtn(_slnDockBox, VIEW_SOLUTION | VIEW_PROJECT | VIEW_CONFIG | VIEW_NONE | VIEW_USERCONTROLINTERFACE);
        _toolBarL->addDockBtn(_devTypeDockBox, VIEW_PROJECT);
        _toolBarL->addDockBtn(_devModelDockBox, VIEW_CONFIG | VIEW_USERCONTROLINTERFACE);

        _toolBarR = new BorderDock(Qt::RightToolBarArea, this);
        _toolBarR->addDockBtn(_propertyDockBox, VIEW_SOLUTION | VIEW_PROJECT | VIEW_CONFIG | VIEW_NONE | VIEW_USERCONTROLINTERFACE);
        _toolBarR->addDockBtn(_modelDockBox, VIEW_CONFIG | VIEW_USERCONTROLINTERFACE);
    }
    else {
        setDockNestingEnabled(true);
        //左边dockWidget
        addDockWidget(Qt::LeftDockWidgetArea, _slnDockBox);
        tabifyDockWidget(_slnDockBox, _devTypeDockBox);
        tabifyDockWidget(_slnDockBox, _devModelDockBox);
        //右边dockWidget
        addDockWidget(Qt::RightDockWidgetArea, _propertyDockBox);
        splitDockWidget(_propertyDockBox, _modelDockBox, Qt::Vertical);
        tabifyDockWidget(_modelDockBox, _pinsViewerDockBox);
        //下边dockWidget
        addDockWidget(Qt::BottomDockWidgetArea, _dbDevDockBox);
        tabifyDockWidget(_dbDevDockBox, _logsDockBox);
        _dbDevDockBox->raise();
    }
}

void MainWindow::updateDockWidgets(SubWnd_Type viewType, ConfigScene *cfgScene)
{
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
        updateDockWidgets_Fangtu(viewType, cfgScene);
    else
        updateDockWidgets_Others(viewType, cfgScene);
}

void MainWindow::updateDockWidgets_Fangtu(SubWnd_Type viewType, ConfigScene *cfgScene)
{
    _slnDockBox     ->hide();
    _devTypeDockBox ->hide();
    _devModelDockBox->hide();
    _propertyDockBox->hide();
    _modelDockBox   ->hide();
    _pinsViewerDockBox->hide();
    _dbDevDockBox   ->hide();
    _logsDockBox    ->hide();

    if(viewType == VIEW_NONE || viewType == VIEW_SOLUTION)
    {
        _slnDockBox->setVisible(true);
        _propertyDockBox->setVisible(_actionProperties->isChecked());
        _logsDockBox->setVisible(_actionLogs->isChecked());
        _dbDevDockBox->setVisible(_actionDbDev->isChecked());
        if(_logsDockBox->isVisible()) _logsDockBox->raise();
    }
    else if(viewType == VIEW_PROJECT)
    {
        _devTypeDockBox->setVisible(_actionDevType->isVisible() && _actionDevType->isChecked());
        _propertyDockBox->setVisible(_actionProperties->isVisible() && _actionProperties->isChecked());
        _logsDockBox->setVisible(_actionLogs->isVisible() && _actionLogs->isChecked());
        _dbDevDockBox->setVisible(_actionDbDev->isVisible() && _actionDbDev->isChecked());
        if(_dbDevDockBox->isVisible()) _dbDevDockBox->raise();
    }
    else if(viewType == VIEW_CONFIG)
    {
        bool isDebug = false;
        if(cfgScene != NULL) isDebug = cfgScene->isDebuging();
        if(!isDebug) {
            _propertyDockBox->setVisible(_actionProperties_cfg->isVisible() && _actionProperties_cfg->isChecked());
            _modelDockBox->setVisible(_actionComponents->isVisible() && _actionComponents->isChecked());
            _pinsViewerDockBox->setVisible(_actionComponentControls->isVisible() && _actionComponentControls->isChecked());
            _devModelDockBox->setVisible(_actionDevModel->isVisible() && _actionDevModel->isChecked());
            //根据设备绑定情况处理接口DockBox及算法DockBox
            if(cfgScene->getTrnDev() != NULL)
            {
                ToolKitTree* devTree = qobject_cast<ToolKitTree*>(_devModelDockBox->widget());
                devTree->SetTreeItems(ToolKitTree::GetInOutItems(cfgScene), cfgScene);

                PlatformType pType = cfgScene->getTrnDev()->getPlatformType();
                int totalPortNum = Utils::getDevTatalPortNum(cfgScene->getTrnDev()->getDeviceType());
                _modelDockBox->UpdatePlatformModel(pType, totalPortNum, cfgScene);
            }
        }
    }
    else if(viewType == VIEW_USERCONTROLINTERFACE){//暂时
        _propertyDockBox->setVisible(_actionProperties_cfg->isChecked());
        _modelDockBox->setVisible(_actionComponents->isChecked());
        _pinsViewerDockBox->setVisible(_actionComponentControls->isChecked());
        _devModelDockBox->setVisible(_actionDevModel->isChecked());
    }

    _propertyDockBox->initTreeProperty(nullptr); //清空属性栏
    int hight = centralWidget()->height()*0.5;
    resizeDocks({_propertyDockBox, _modelDockBox}, {hight, hight}, Qt::Vertical);//等比例排列属性窗口和算法模块窗口
    resizeDocks({_devModelDockBox, _slnDockBox}, {hight, hight}, Qt::Vertical);

    _toolBarL->updateBorderDock(viewType);
    _toolBarR->updateBorderDock(viewType);
}

void MainWindow::updateDockWidgets_Others(SubWnd_Type viewType, ConfigScene *cfgScene)
{
    if(viewType == VIEW_NONE || viewType == VIEW_SOLUTION)
    {
        _slnDockBox->setVisible(true);
        _devTypeDockBox->setVisible(_actionDevType->isChecked());
        _slnDockBox->raise();
        _logsDockBox->setVisible(_actionLogs->isChecked());
        _dbDevDockBox->setVisible(_actionDbDev->isChecked());
        if(_actionLogs->isChecked())  _logsDockBox->raise();
        _devModelDockBox->hide();
        _modelDockBox->hide();
        _pinsViewerDockBox->hide();
    }
    else if(viewType == VIEW_PROJECT)
    {
        _logsDockBox->setVisible(_actionLogs->isChecked());
        _dbDevDockBox->setVisible(_actionDbDev->isChecked());
        if(_actionDbDev->isChecked())  _dbDevDockBox->raise();
        _slnDockBox->setVisible(true);
        _devTypeDockBox->setVisible(_actionDevType->isChecked());
        _devTypeDockBox->raise();
        _propertyDockBox->setVisible(_actionProperties->isChecked());
        _devModelDockBox->hide();
        _modelDockBox->hide();
        _pinsViewerDockBox->hide();
    }
    else if(viewType == VIEW_CONFIG)
    {
        _devTypeDockBox->hide();
        _dbDevDockBox->hide();
        _logsDockBox->hide();
        bool isDebug = false;
        if(cfgScene != NULL) isDebug = cfgScene->isDebuging();
        if(isDebug){
            _slnDockBox->hide();
            _propertyDockBox->hide();
            _devModelDockBox->hide();
            _modelDockBox->hide();
            _pinsViewerDockBox->hide();
        }
        else{
            _propertyDockBox->setVisible(_actionProperties_cfg->isChecked());
            //根据设备绑定情况处理接口DockBox及算法DockBox
            if(cfgScene->getTrnDev() != NULL)
            {
                ToolKitTree* devTree = qobject_cast<ToolKitTree*>(_devModelDockBox->widget());
                devTree->SetTreeItems(ToolKitTree::GetInOutItems(cfgScene), cfgScene);
                _devModelDockBox->setVisible(_actionDevModel->isChecked());
                if(_actionDevModel->isChecked()) _devModelDockBox->raise();

                PlatformType pType = cfgScene->getTrnDev()->getPlatformType();
                int totalPortNum = Utils::getDevTatalPortNum(cfgScene->getTrnDev()->getDeviceType());
                _modelDockBox->UpdatePlatformModel(pType, totalPortNum, cfgScene);
            }
            _modelDockBox->setVisible(_actionComponents->isChecked());
            _pinsViewerDockBox->setVisible(_actionComponentControls->isVisible() && _actionComponentControls->isChecked());
        }
    }

    _propertyDockBox->initTreeProperty(nullptr); //清空属性栏
    int hight = _mdiArea->height()*0.5;
    resizeDocks({_propertyDockBox, _modelDockBox}, {hight, hight}, Qt::Vertical);//等比例排列属性窗口和算法模块窗口
}

void MainWindow::helpAbout()
{
    QString VERSION_NUM = Utils::getSoftwareVersion();
//    QString VERSION_NUM = QString("%1%2.%3.%4.%5")
//            .arg(PC_PREFIX)
//            .arg(TRANSCTRL_MAJOR)
//            .arg(TRANSCTRL_MINOR)
//            .arg(TRANSCTRL_BUILD)
//            .arg(PC_BUILD);
    UserInfo& user = Config::instance()->getCurUser();
    QString versionStr = QString("%1 %2").arg(Config::curMfrsCfg().TitleHeader)
            .arg(VERSION_NUM);
    QString userStr = tr("Current User: %1(%2)").arg(user.name).arg(user.type);
    QFileIconProvider seekIcon;
    QIcon icon = seekIcon.icon(QCoreApplication::applicationFilePath());
    QPixmap appIcon = icon.pixmap(QSize(35, 35));
    Utils::MessageBoxInfo(tr("Information"), versionStr + "\r\n" + userStr, appIcon);
}

#define CHS(x) QStringLiteral(x)
#include <QWebEngineView>
#include <QWebEngineSettings>
void MainWindow::helpDocuments()
{
    //调试模式跳过
#ifdef QT_DEBUG
    return;
#endif
    QUrl helpUrl;
    QString title;
    bool isEmpty = helpUrl.isEmpty();
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) {
        if(sender() == _actionSoftwareOperationGuide) {
            helpUrl.setUrl(CHS("file:/Docs/Fangtu/FT-Designer软件操作指南.pdf"));
            title = tr("Software Operation Guide");
        }
        if(sender() == _actionFunctionModule) {
            helpUrl.setUrl(CHS("file:/Docs/Fangtu/MBP100音频处理模块功能说明书.pdf"));
            title = tr("Function module");
        }
    }
    else if (Config::curMfrsCfg().CurMfrs == Mfrs_KEMU
             || Config::curMfrsCfg().CurMfrs == Mfrs_ControlMax){
        if(sender() == _actionSoftwareOperationGuide) {
            helpUrl.setUrl(CHS("file:/Docs/KEMU/KEMU-Designer软件操作指南.pdf"));
            title = tr("Software Operation Guide");
        }
        if(sender() == _actionFunctionModule) {
            helpUrl.setUrl(CHS("file:/Docs/KEMU/音频处理模块功能说明书.pdf"));
            title = tr("Function module");
        }
    }
    if(!helpUrl.isEmpty()){
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        QWebEngineView *view = new QWebEngineView;
        view->setUrl(helpUrl);
        view->setWindowTitle(title);
        view->resize(1024, 750);
        view->show();
    }
    else{
        MainWindow::instance()->showNotice(tr("not set help Info"));
    }
}


void MainWindow::updateMenuToolbars(SubWnd_Type viewType, ConfigScene *cfgScene)
{
    //如果子窗口类型没有变化，则跳过
    if( _lastViewType == viewType ) return;
    _lastViewType = viewType;

    hadeMenuToolbars(); //先隐藏所有menu、action等； 再根据窗口类型显示需要的

    if( viewType == VIEW_NONE)
    {
    }
    else if(viewType== VIEW_SOLUTION)
    {
        _actionNew->setEnabled(true);
        _actionSave->setEnabled(true);
        _actionSaveAll->setEnabled(true);
        _actionLogs->setVisible(true);
        _actionDbDev->setVisible(true);
        _actionProperties->setVisible(true);
    }
    else if(viewType== VIEW_PROJECT)
    {
        _actionNew->setEnabled(true);
        _actionSave->setEnabled(true);
        _actionSaveAll->setEnabled(true);
        _actionImport->setEnabled(true);
        _actionExport->setEnabled(true);
        _actionLoadCfgSPK->setVisible(true);
        _actionLoadCfgAMP->setVisible(true);
        _menuEdit->menuAction()->setVisible(true);
        _actionDeviceLine->setVisible(true);
        _actionDownRouter->setVisible(true);
        _actionLogs->setVisible(true);
        _actionDbDev->setVisible(true);
        _actionDevType->setVisible(true);
        _actionProperties->setVisible(true);
    }
    else if(viewType== VIEW_CONFIG)
    {
        _actionSave->setEnabled(true);
        _actionSaveAll->setEnabled(true);
        _actionImport->setEnabled(true);
        _actionExport->setEnabled(true);
        _actionSaveToDev->setVisible(true);
        _actionLoadFromDev->setVisible(true);
        _action_ResourceManager->setVisible(true);
        _actionDownAlgorithm->setVisible(true);
        _menuEdit->menuAction()->setVisible(true);

        _actionDevModel->setVisible(true);
        _actionProperties_cfg->setVisible(true);
        _actionComponents->setVisible(true);

        bool isLuaDev = false;
        if(cfgScene->getTrnDev() != NULL){
            DevType devType = cfgScene->getTrnDev()->getDeviceType();
            isLuaDev = Config::curMfrsCfg().isLuaDevice(devType);
        }
        _actionComponentControls->setVisible(isLuaDev);
        _actionAudioMute->setVisible(true);
        bool isDebug = cfgScene->isDebuging();
        _actionAudioMute->setEnabled(isDebug);//只在调试模式下静音按钮有效
    }
    else if(viewType == VIEW_MONITOR_AIDS) {
        ;
    }
    else if(viewType == VIEW_USERCONTROLINTERFACE) {
        _actionSave->setEnabled(true);
        _actionSaveAll->setEnabled(true);
        _actionImport->setEnabled(true);
        _actionExport->setEnabled(true);
    }
}


void MainWindow::onZoomView_slots()
{
    QWidget *currrentWidget = _mdiArea->currentSubWindow()->widget();
    if(!currrentWidget) return ;
    AbsGraphView *absGrapView = qobject_cast<AbsGraphView *>(currrentWidget);
    if(!absGrapView) return ;
    int scaling = absGrapView->getScaleValue();
    if(sender() == actionZoomIn) {
        scaling += 5;
    }
    else if(sender() == actionZoomOut)
    {
        scaling -= 5;
    }
    absGrapView->changedScale(scaling);
    _statusLabelList.at(STATUSBAR_SCALING)->setText(tr("Scaling:%1%").arg(absGrapView->getScaleValue()));
}

void MainWindow::onViewScaleValueChanged_slot(qreal ScaleValue)
{
    _statusLabelList.at(STATUSBAR_SCALING)->setText(tr("Scaling:%1%").arg(ScaleValue));
}

void MainWindow::mouseStateSet(bool enable)
{
    QWidget *currrentWidget = _mdiArea->currentSubWindow()->widget();
    if(!currrentWidget) return ;
    AbsGraphView *absGrapView = qobject_cast<AbsGraphView *>(currrentWidget);
    if(!absGrapView) return ;
    if (sender() == actionCursor) {
        if (enable) {
            actionHandle->setChecked(false);
            absGrapView->setDragMode(QGraphicsView::RubberBandDrag);
        }
        else {
            actionHandle->setChecked(true);
            absGrapView->setDragMode(QGraphicsView::ScrollHandDrag);
        }
    }
    else if (sender() == actionHandle) {
        if (enable) {
            actionCursor->setChecked(false);
            absGrapView->setDragMode(QGraphicsView::ScrollHandDrag);
        }
        else {
            actionCursor->setChecked(true);
            absGrapView->setDragMode(QGraphicsView::RubberBandDrag);
        }
    }
}

void MainWindow::Firewall_Settings()
{
    QProcess p(0);
    QString exeFile = QCoreApplication::applicationFilePath();
    QString exeName = QApplication::applicationDisplayName();
    QStringList command;
    command.append("/c");
    exeFile.replace("/", "\\");
    if(sender() == actionAdd_to_whitelist) {
        command.append(QString("netsh advfirewall firewall add rule name=\"%1\" dir=in action=allow program=\"%2\" enable=yes")
                       .arg(exeName).arg(exeFile));
    }
    else if(sender() == actionRemove_whitelist) {
        command.append(QString("netsh advfirewall firewall delete rule name=\"%1\" dir=in program=\"%2\"")
                       .arg(exeName).arg(exeFile));
    }
//    qDebug() << __FUNCTION__ << __LINE__ << exeFile << exeName << command;
    p.start("cmd.exe", command);
    p.waitForStarted();
    p.waitForFinished();
//    qDebug() << __FUNCTION__ << __LINE__ << QString::fromLocal8Bit(p.readAllStandardError())
//             << QString::fromLocal8Bit(p.readAllStandardOutput());
    QString result = QString::fromLocal8Bit(p.readAllStandardError()).isEmpty() ?
                QString::fromLocal8Bit(p.readAllStandardOutput()) : QString::fromLocal8Bit(p.readAllStandardError());
    showNotice(result);
    p.close();
}

#include "deviceItem/deviceitemfactory.h"
//#include "deviceItem/deviceitem_room.h"
void MainWindow::fileNewClicked()
{
    QString projName = Utils::getNewName();
    if(projName.isEmpty()) return;
    QString projFile = _slnDockBox->getAbsolutePathOfSln(projName);
    if(!projFile.contains(".pro")) projFile += ".pro";

    if(_slnDockBox->getAllProjectFiles().contains(projFile)) {        
        MainWindow::instance()->showNotice(tr("%1 already exists!").arg(projFile));
        return ;
    }

    ProjectView* tempProjView = new ProjectView(projName, this);
    tempProjView->scene()->saveAs(projFile);
    delete tempProjView;
    openSubWindow(projFile);
    _slnDockBox->updateSlnTree();

    SolutionView *slnView = getSubWindowForTitle<SolutionView>(QFileInfo(_slnDockBox->getSolutionFilePath()).fileName());
    if(slnView) {
        DeviceItem_Room *roomItem = slnView->scene()->getRoomItem(projName);
        if(roomItem)  {
            roomItem->setValid(true);
            return ;
        }
    }
}


void MainWindow::addUserControlInterfaceView(QGraphicsItem *item, QString name)
{
    static int InterfaceIndex = 0;      //子窗口序号
    QString uciTitle;
    if(name.isEmpty()) {
        uciTitle = tr("Interface %1").arg(++InterfaceIndex);
    }else {
        uciTitle = name;
    }
    UserControlInterfaceView *uciItem = new UserControlInterfaceView(uciTitle, item, this);
    _uciSubWindowsList.append(_mdiArea->addSubWindow(uciItem));
    uciItem->showMaximized();
    connect(uciItem, SIGNAL(viewClosed()), this, SLOT(onSubWindowClosed()));
}

void MainWindow::delUserControlInterfaceView(QGraphicsItem *item)
{
    foreach(QMdiSubWindow *uciSubWinItem, _uciSubWindowsList) {
        if(_mdiArea->subWindowList().contains(uciSubWinItem)) {
            UserControlInterfaceView *uciItem = qobject_cast<UserControlInterfaceView *>(uciSubWinItem->widget());
            if(!uciItem) continue;
            if(uciItem->getGrapItem() == item) {
                uciSubWinItem->close();
            }
        }else {
            _uciSubWindowsList.removeAll(uciSubWinItem);
        }
    }
}

void MainWindow::setCurrentSubWindow_UCI(QGraphicsItem *item, QString name)
{
    foreach (QMdiSubWindow *subw, _uciSubWindowsList) {
        if(subw->windowTitle() == name) {
            _mdiArea->setActiveSubWindow(subw);
            subw->showMaximized();
            return ;
        }
    }
    addUserControlInterfaceView(item, name);
}

void MainWindow::onToolBtnCheckedSlot(ConfigScene::ToolkitType type)
{
    QWidget *currentWidget = MainWindow::instance()->getMdiArea()->currentSubWindow()->widget();
    if(!currentWidget) return ;
    AbsGraphView *absView = qobject_cast<AbsGraphView *>(currentWidget);
    if(!absView) return ;
    if(absView->getType() != VIEW_CONFIG) return ;
    ConfigScene *cfgscene = qobject_cast<ConfigView *>(absView)->scene();
    cfgscene->setToolkitType(type);
}

void MainWindow::setModelDockBoxToolkitBtnStatus(bool status)
{
    _modelDockBox->onToolkitBtnStatusChangedSlot(status);
}


void MainWindow::fileImportClicked()
{
    QString fileName = NULL;
    AbsGraphView *subView = qobject_cast<AbsGraphView *>(_mdiArea->currentSubWindow()->widget());
    switch (subView->getType()) {
    case VIEW_PROJECT:
        fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("Project Files(*.pro)"));
        if(fileName == NULL) return;
        _slnDockBox->importFile(fileName);
        break;
    case VIEW_CONFIG:
    {
        fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("Config Files(*.nmu)"));
        if(fileName == NULL) return;
        ConfigScene* cfgScene = qobject_cast<ConfigScene*>(subView->scene());
        if(cfgScene->checkEditable()){
            cfgScene->open(fileName, true);
        }
        break;
    }
    case VIEW_USERCONTROLINTERFACE:
    {
        fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("UCI Files(*.uci)"));
        if(fileName == NULL) return ;
        UserControlInterfaceScene* uciScn = qobject_cast<UserControlInterfaceView *>(subView)->scene();
        uciScn->open(fileName, "");
        break;
    }
    default:
        break;
    }
}

void MainWindow::fileCloseClicked()
{
    QMdiSubWindow * curWindow = _mdiArea->currentSubWindow();
    if (curWindow == NULL) {
        return;
    }
    curWindow->close();
    //    AbsGraphView *subWnd = qobject_cast<AbsGraphView *>(curWindow->widget());
    //    if(subWnd != NULL){
    //        subWnd->close();
    //    }
}

void MainWindow::resourceManagerClicked()
{
    if(_resourceManager) {
        _resourceManager->show();
    }
    else {
        AbsGraphView *subWnd = qobject_cast<AbsGraphView *>(_mdiArea->currentSubWindow()->widget());
        if(subWnd->getType() == VIEW_CONFIG) {
            ConfigScene *cfgScene = qobject_cast<ConfigView *>(subWnd)->scene();
            _resourceManager = new ResourceManager(cfgScene, this);
            _resourceManager->show();
        }
    }
}


void MainWindow::fileSaveClicked()
{
    AbsGraphView *subWnd = qobject_cast<AbsGraphView *>(_mdiArea->currentSubWindow()->widget());
    if(subWnd != NULL){
        subWnd->saveScene(false);
    }
}

void MainWindow::fileSaveToDeviceClicked()
{
    if(Config::instance()->hasNormalPriv()){
        ConfigView *subWnd = qobject_cast<ConfigView *>(_mdiArea->currentSubWindow()->widget());
        if(subWnd)
        {
            ConfigScene *scene = subWnd->scene();
            Device_Trn *trnDev = scene->getTrnDev();
            if(trnDev == NULL) return;
            if(trnDev->getPlatformType()== Platform_DSP)
            {
                Trn_SceneWidget* selectWidget = new Trn_SceneWidget;
                connect(selectWidget, SIGNAL(sceneSelected(quint8)), scene, SLOT(saveSceneToDevice(quint8)));
                selectWidget->show();
            }
            else
            {
                scene->saveSceneToDevice(0);
            }
        }
    }
}

#include "Lua/Packages/centerctrl.h"
void MainWindow::loadFromDeviceClicked()
{
    if(Config::instance()->hasNormalPriv()){
        //hrp暂时  通过中控协议先设置当前的场景号，再从设备中调出当前场景
        Trn_SceneWidget* selectWidget = new Trn_SceneWidget;
        selectWidget->show();
        connect(selectWidget, QOverload<quint8>::of(
                    &Trn_SceneWidget::sceneSelected), this, [=](quint8 sceneIdx){
            ConfigView *cfgView = qobject_cast<ConfigView *>(_mdiArea->currentSubWindow()->widget());
            Q_ASSERT(cfgView !=NULL);

            CenterCtrl::instance()->setDevAddress(cfgView->scene()->getTrnDev()->getDeviceIp());
            bool rs = CenterCtrl::instance()->setCurrentSceneNum(++sceneIdx);//中控协议文档中，场景号从1开始
#ifndef QT_NO_DEBUG
            if(!rs)
                qDebug() << __FUNCTION__ << __LINE__ << "idx: " << sceneIdx << "Scene setting failure!!!";
            else
                qDebug() << __FUNCTION__ << __LINE__ << "idx: " << sceneIdx << "Scene setting is successful!!!";
#endif
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            cfgView->scene()->loadFromDevice();
            QApplication::restoreOverrideCursor();
        });
    }
}

void MainWindow::fileExportClicked()
{
    if( _mdiArea->currentSubWindow() == NULL) return;

    AbsGraphView *subView = qobject_cast<AbsGraphView *>(_mdiArea->currentSubWindow()->widget());
    Q_ASSERT(subView !=NULL);
    if (subView->getType() == VIEW_CONFIG)
    {
        subView->saveAsScene();
    }
    else if (subView->getType() == VIEW_PROJECT)
    {
        subView->saveAsScene();
    }
    else if(subView->getType() == VIEW_USERCONTROLINTERFACE) {
        subView->saveAsScene();
    }
}

void MainWindow::openSubWindow(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    QString title = fileInfo.fileName();
    if(!fileInfo.exists()) {
        showNotice(tr("%1 does not exist!").arg(filePath));
        return ;
    }
qDebug() << __FUNCTION__ << __LINE__ << filePath;
    //如果已打开，激活对应窗口
    foreach (QMdiSubWindow *subWnd, _mdiArea->subWindowList())
    {
        QString roomName = subWnd->windowTitle();
        if(roomName.endsWith('*')) roomName.chop(1);
        if(roomName == title){
            _mdiArea->setActiveSubWindow(subWnd);
            return;
        }
    }

    if(fileInfo.suffix().startsWith("pro", Qt::CaseInsensitive))
    {
        ProjectView* projView = new ProjectView(title, this);
        projView->scene()->open(filePath);
        QMdiSubWindow *subWnd = _mdiArea->addSubWindow(projView);
        subWnd->setSystemMenu(NULL);
        _undoGroup->addStack(projView->getUndoStack());
        projView->showMaximized();

        connect(projView, SIGNAL(viewClosed()), this, SLOT(onSubWindowClosed()));
        connect(projView, SIGNAL(isSaved()), this, SLOT(onSubWindowSaved()));
        connect(projView, SIGNAL(onScaleValueChanged(qreal)), this, SLOT(onViewScaleValueChanged_slot(qreal)));
    }
    else if(fileInfo.suffix().startsWith("nmu", Qt::CaseInsensitive))
    {
        if(QFile::exists(filePath))
        {
            Device_Trn* trnDev = NULL;
            QString mac;
            DevType devType;
            bool rs = ConfigScene::getConfigFileMac(filePath, mac, devType);
            if(rs) {
                trnDev = qobject_cast<Device_Trn*>(DeviceManager::instance()->getDeviceByMac(mac));
            }
            ConfigView *cfgView = new ConfigView(title, trnDev,  this);
            cfgView->scene()->open(filePath, false);
            QMdiSubWindow *subWnd = _mdiArea->addSubWindow(cfgView);
            subWnd->setSystemMenu(NULL);
            _undoGroup->addStack(cfgView->getUndoStack());
            cfgView->showMaximized();
            connect(cfgView, SIGNAL(viewClosed()), this, SLOT(onSubWindowClosed()));
            connect(cfgView, SIGNAL(onScaleValueChanged(qreal)), this, SLOT(onViewScaleValueChanged_slot(qreal)));
        }
    }
    else if(fileInfo.suffix().startsWith("sln", Qt::CaseInsensitive))
    {
        SolutionView* slnView = new SolutionView(title, this);
        slnView->scene()->open(filePath, _slnDockBox->getAllProjectFiles());
        QMdiSubWindow *subWnd = _mdiArea->addSubWindow(slnView);
        subWnd->setSystemMenu(NULL);
        _undoGroup->addStack(slnView->getUndoStack());
        slnView->showMaximized();

        connect(slnView, SIGNAL(viewClosed()), this, SLOT(onSubWindowClosed()));
        connect(slnView, SIGNAL(isSaved()), this, SLOT(onSubWindowSaved()));
        connect(slnView, SIGNAL(onScaleValueChanged(qreal)), this, SLOT(onViewScaleValueChanged_slot(qreal)));
    }
}

void MainWindow::openWelcomeDlg()
{
    NewSolution *slnManageDlg = new NewSolution;
    connect(slnManageDlg, SIGNAL(solutionOpened(QString)), this, SLOT(openSolution(QString)));
    QMdiSubWindow *subWnd = _mdiArea->addSubWindow(slnManageDlg);
    subWnd->setSystemMenu(NULL);
    slnManageDlg->showMaximized();
}

void MainWindow::deleteSubWindow(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    QString title = fileInfo.fileName();
    int idx = title.indexOf(QChar('('));    //隐藏(mac)
    if(idx != -1) title = title.left(idx);
    foreach (QMdiSubWindow *subWnd, _mdiArea->subWindowList())
    {
        QString fileName = subWnd->windowTitle();
        if(fileName.endsWith('*')) fileName.chop(1);
        if(fileName == title) {
            _mdiArea->removeSubWindow(subWnd);
            subWnd->close();
        }
        else if(fileName == getSolutionPath().split('/').last()) {
            SolutionView * slnView = qobject_cast<SolutionView *>(subWnd->widget());
            if(!slnView) continue;
            DeviceItem_Room * roomItem = slnView->scene()->getRoomItem(title);
            if(roomItem) roomItem->setValid(false);
        }
    }
    _slnDockBox->updateSlnTree();
}

void MainWindow::renameSubWindow(const QString& oldFilePath, const QString& newFilePath)
{
    QFileInfo fileOld(oldFilePath);
    QString titleOld = fileOld.fileName();

    QFileInfo fileNew(newFilePath);
    QString titleNew = fileNew.fileName();
    foreach (QMdiSubWindow *subWnd, _mdiArea->subWindowList())
    {
        QString curTitle = subWnd->windowTitle();
        if(curTitle.endsWith('*')) curTitle.chop(1);
        if(curTitle == titleOld) {
            subWnd->widget()->setWindowTitle(titleNew);
        }
        else if(curTitle == QFileInfo(getSolutionPath()).fileName()) {
            SolutionView * slnView = qobject_cast<SolutionView *>(subWnd->widget());
            if(!slnView) continue;
            slnView->scene()->renameRoomItem(titleOld, titleNew);
        }
    }

    _slnDockBox->updateSlnTree();
}


void MainWindow::setItemsAlign()
{
    if(_mdiArea->currentSubWindow()== NULL) return;
    AbsGraphView* pView = qobject_cast<AbsGraphView *>(_mdiArea->currentSubWindow()->widget());
    if(pView == NULL) return;
    AbsGraphScene* pScene = qobject_cast<AbsGraphScene*>(pView->scene());
    if(pScene)
    {
        QAction *act = qobject_cast<QAction *>(sender());
        AlignMode alignMode;
        if(act == actionAlignLeft) alignMode = AlignLeft;
        else if(act == actionAlignRight) alignMode = AlignRight;
        else if(act == actionAlignTop) alignMode = AlignTop;
        else if(act == actionAlignBottom) alignMode = AlignBottom;
        else if(act == actionAlignEqHSpace) alignMode = AlignEqHSpace;
        else if(act == actionAlignEqVSpace) alignMode = AlignEqVSpace;

        if(alignMode == AlignEqHSpace || alignMode == AlignEqVSpace)
        {
            pScene->setItemsEqSpace(alignMode);
        }
        else
        {
            pScene->setItemsAlign(alignMode);
        }
    }
}

void MainWindow::onSubWindowClosed()
{
    AbsGraphView *subView = qobject_cast<AbsGraphView *>(sender());
    if(subView) {
        _undoGroup->removeStack(subView->getUndoStack());//从此组中移除

        if(subView->getType() == VIEW_USERCONTROLINTERFACE) {
            _uciSubWindowsList.removeAll(qobject_cast<QMdiSubWindow *>(subView->parent()));
        }
    }

    //关闭最后一个页面时刷新DockBox及菜单状态
    if(_mdiArea->subWindowList().count()== 1) {
        updateDockWidgets(VIEW_NONE, NULL); //根据视图刷新Dock控件
        updateMenuToolbars(VIEW_NONE, NULL);     //根据视图刷新菜单按钮
//        initTreeProperty(NULL);
    }
}

void MainWindow::onSubWindowSaved()
{
    _mdiArea->currentSubWindow()->setFocus();//设置键盘输入焦点
}

void MainWindow::settingUserLockClicked()
{
    LoginDialog  lockSys(true/*, this*/);
    lockSys.exec();
}

void MainWindow::settingUserManageClicked()
{
    if(Config::instance()->hasAdminPriv()){
        UserManagement* userManagement = new UserManagement;
        userManagement->show();
    }
}

#include "UserManager/changepassword.h"
void MainWindow::settingUserPasswordClicked()
{
    UserInfo curUser = Config::instance()->getCurUser();
    QString tp = curUser.type;
    QString nm = curUser.name;
    QString ps;

    foreach (UserInfo info, Config::instance()->getUserList())
    {
        if (info.type == tp && info.name == nm)
        {
            ps = info.password;
        }
    }

    ChangePassword* changePassword = new ChangePassword;
    changePassword->setChangePasswordInfo(tp, nm, ps);
    changePassword->show();
}

QList<ConfigItem*> MainWindow::getCfgItemListOfCurWindow()
{
    QMdiSubWindow* curSubWnd= _mdiArea->currentSubWindow();
    if(curSubWnd != NULL){
        ConfigView* cfgView = qobject_cast<ConfigView*> (curSubWnd->widget());
        if(cfgView != NULL)  {
            return cfgView->scene()->getConfigItemList();
        }
    }

    return QList<ConfigItem*>();
}


void MainWindow::showNotice(QString notice, int second)
{
    NoticeWidget* noticeWgt = new NoticeWidget(this);
    noticeWgt->showNotice(notice, second);
}


#include "customWidget/controlcenterutil.h"
void MainWindow::openCtrlCenterUtil()
{
    QWidget* utilForm = new ControlCenterUtil();
    utilForm->show();
}

#include "device/devicemanager.h"
#include "device/devicemodel.h"
void MainWindow::clearOfflineDevice()
{
    DeviceManager::instance()->getDeviceModel()->deleteOfflineDevice();
    Utils::MessageBoxWarning(tr("Warning"), QString("Please Restart Software!"));
}


void MainWindow::setLanguage()
{
    bool langChanged = false;
    LanguageEnum curLang = Config::instance()->getLanguage();
    if (sender() == actionChinese && curLang != LANGUAGE_SIMPLIED_CHINESE)
    {
        Config::instance()->setLanguage(LANGUAGE_SIMPLIED_CHINESE);
        langChanged = true;
    }
    else if(sender() == actionEnglish && curLang != LANGUAGE_ENGLISH) {
        Config::instance()->setLanguage(LANGUAGE_ENGLISH);
        langChanged = true;
    }

    if(langChanged){
        MainWindow::instance()->showNotice(tr("Language set ok, please restart software!"));
    }
}

void MainWindow::setSkinStyle()
{
    SkinType skinType = Config::instance()->curSkinType();
    if(sender() != NULL)
    {
        QAction* skinAction = qobject_cast<QAction*>(sender());
        skinType = (SkinType)skinAction->data().toInt();
        if(skinType == Config::curSkinType()){
            return;
        }

        Config::ChangeSkin(skinType);
        qApp->setStyleSheet(SkinStyle::getAppStyleSheet());
        SkinStyle::loadAppFont();
        emit onSkinChanged();
    }

    if(skinType == Skin_FangTu) {_mainToolBar->setIconSize(QSize(16, 16));}
    else {_mainToolBar->setIconSize(QSize(22, 22));}
    QString iconDir = Config::curSkinStyle()->getSkinIconDir();
    _actionNew->setIcon(QIcon(QString(":/Image/%1/new.png").arg(iconDir)));
    _actionSave->setIcon(QIcon(QString(":/Image/%1/save.png").arg(iconDir)));
    _actionSaveAll->setIcon(QIcon(QString(":/Image/%1/saveAll.png").arg(iconDir)));
    _actionImport->setIcon(QIcon(QString(":/Image/%1/import.png").arg(iconDir)));
    _actionExport->setIcon(QIcon(QString(":/Image/%1/export.png").arg(iconDir)));
    _actionDownRouter->setIcon(QIcon(QString(":/Image/%1/downRouter.png").arg(iconDir)));
    _actionDownAlgorithm->setIcon(QIcon(QString(":/Image/%1/debugMode.png").arg(iconDir)));
    actionZoomIn->setIcon(QIcon(QString(":/Image/%1/ZoomIn.png").arg(iconDir)));
    actionZoomOut->setIcon(QIcon(QString(":/Image/%1/ZoomOut.png").arg(iconDir)));    
    actionAlignLeft->setIcon(QIcon(QString(":/Image/%1/AlignLeft.png").arg(iconDir)));
    actionAlignRight->setIcon(QIcon(QString(":/Image/%1/AlignRight.png").arg(iconDir)));
    actionAlignTop->setIcon(QIcon(QString(":/Image/%1/AlignTop.png").arg(iconDir)));
    actionAlignBottom->setIcon(QIcon(QString(":/Image/%1/AlignBottom.png").arg(iconDir)));
    actionAlignEqHSpace->setIcon(QIcon(QString(":/Image/%1/AlignEqSpaceH.png").arg(iconDir)));
    actionAlignEqVSpace->setIcon(QIcon(QString(":/Image/%1/AlignEqSpaceV.png").arg(iconDir)));
    _actionAbout->setIcon(QIcon(QString(":/Image/%1/About.png").arg(iconDir)));
    if(_actionAudioMute->isChecked()){
        _actionAudioMute->setIcon(QIcon(QString(":/Image/%1/AudioMute.png").arg(iconDir)));
    }
    else{
        _actionAudioMute->setIcon(QIcon(QString(":/Image/%1/AudioOn.png").arg(iconDir)));
    }
}

void MainWindow::viewDeviceLineItem()
{
    ProjectView* curProjView = qobject_cast<ProjectView*>(_mdiArea->currentSubWindow()->widget());
    if(curProjView == NULL) return;
    DeviceLinkView *linkView = new DeviceLinkView(curProjView->scene());
    linkView->exec();
}

#include "device/devicemanager.h"
void MainWindow::closeEvent(QCloseEvent *event)
{
    QString curTitle;
    if(_mdiArea->currentSubWindow() != NULL) {
        curTitle = _mdiArea->currentSubWindow()->windowTitle();
    }
    QStringList pageNames;
    for(QMdiSubWindow *subWind : _mdiArea->subWindowList(QMdiArea::CreationOrder))
    {
        QString pageName = subWind->windowTitle();
        if(pageName.endsWith('*')) pageName.chop(1);
        pageNames << pageName;
    }
    Utils::saveCurOpenPagesData(pageNames, curTitle);

    closeAllSubWindow();
    QString logInfo = QString(tr("%1 exit disigner")).arg(Config::instance()->getCurUser().name);
    LogsManager::instance()->addLogs(tr("Info"), tr("System"), logInfo);
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_HOTKEY)
    {
        switch (msg->wParam) {
        case HOT_KEY_TEST_SKIN:
            break;
        default:
            qDebug() << "input other key";
        }
        return true;
    }
    return false;
}

void MainWindow::loadAllSpeadkerConfig()
{
    ProjectView* curProjView = qobject_cast<ProjectView*>(_mdiArea->currentSubWindow()->widget());
    if(curProjView == NULL) return;
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("file (*.spk)"));
    if (fileName != NULL)
    {
        curProjView->scene()->fileLoad(fileName, Dev_Type_AES67_SPK);
    }
}

void MainWindow::loadAllAmplifierConfig()
{
    ProjectView* curProjView = qobject_cast<ProjectView*>(_mdiArea->currentSubWindow()->widget());
    if(curProjView == NULL) return;
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("file (*.amp)"));
    if (fileName != NULL)
    {
        curProjView->scene()->fileLoad(fileName, Dev_Type_STM32_POE_AES67_SPK_0_2);
    }
}

void MainWindow::loadAllControllerConfig()
{
    ProjectView* curProjView = qobject_cast<ProjectView*>(_mdiArea->currentSubWindow()->widget());
    if(curProjView == NULL) return;
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("file (*.ctrl)"));
    if (fileName != NULL)
    {
        curProjView->scene()->fileLoad(fileName, Dev_Type_DNTE_TRN);
    }
}


void MainWindow::subWindowShowMaximize()
{
    _mdiArea->currentSubWindow()->showMaximized();
}

void MainWindow::onSubWindowActivated(QMdiSubWindow *activeWnd)
{
    SubWnd_Type viewType = VIEW_NONE;
    ConfigScene *cfgScene = NULL;
    ConfigGainWidget::LastFocusedWidget = NULL;
    if (activeWnd != NULL)
    {
        AbsGraphView* pView = qobject_cast<AbsGraphView*> (activeWnd->widget());
        if(pView == NULL)  {
            updateDockWidgets(VIEW_NONE, NULL);
            pageManager->reflashPage(NULL);
            return;
        }
        _statusLabelList.at(STATUSBAR_SHEET_SIZE)->setText(pView->getSheetSize());
        _statusLabelList.at(STATUSBAR_SCALING)->setText(tr("Scaling:%1%").arg(pView->getScaleValue()));
        _undoGroup->setActiveStack(pView->getUndoStack());
//        qDebug()<< pView->windowTitle() << _undoGroup->stacks().count();

        viewType = pView->getType();
        if (viewType == VIEW_PROJECT)
        {
            _statusLabelList.at(STATUSBAR_DEVICE_STATUS)->setVisible(false);
            _statusLabelList.at(STATUSBAR_SPACE_USAGE)->setVisible(false);
            _statusLabelList.at(STATUSBAR_TIME_USAGE)->setVisible(false);
        }
        else if (viewType == VIEW_CONFIG)
        {
            cfgScene = qobject_cast<ConfigView *>(pView)->scene();
            _statusLabelList.at(STATUSBAR_DEVICE_STATUS)->setText(tr("Status:%1").arg(cfgScene->isDebuging() ? tr("run") : tr("edit")));
            _statusLabelList.at(STATUSBAR_SPACE_USAGE)->setText(tr("Total Space Usage %1%").arg(cfgScene->getSpaceUsage()));
            _statusLabelList.at(STATUSBAR_TIME_USAGE)->setText(tr("Total Time Usage %1%").arg(cfgScene->getTimeUsage()));
            updateDeviceModeStatus(cfgScene);
            if(_resourceManager) {
                _resourceManager->refreshData(cfgScene);
                _resourceManager->refreshSceneCon(cfgScene);
            }
        }else if(viewType == VIEW_USERCONTROLINTERFACE) {
        }

    }

    //解决在设置模块属性时，切换其他软件导致程序闪退
    if(!_currentWin || (activeWnd && _currentWin != activeWnd)) {
        _currentWin = activeWnd;

        updateMenuToolbars(viewType, cfgScene);     //根据视图刷新菜单按钮
        updateDockWidgets(viewType, cfgScene);      //根据视图刷新Dock控件
        pageManager->reflashPage(cfgScene);
        initTreeProperty(NULL);
    }
}

void MainWindow::downloadRouter()
{
    if(Config::instance()->hasNormalPriv()){
        ProjectView *projView = qobject_cast<ProjectView*> (_mdiArea->activeSubWindow()->widget());
        Q_ASSERT(projView!=NULL);
        projView->onDownloadProjectRoute();
    }
}

void MainWindow::downloadAlgorithm()
{
    if(Config::instance()->hasNormalPriv()){
        ConfigView *cfgView = qobject_cast<ConfigView*> (_mdiArea->activeSubWindow()->widget());
        if(cfgView != NULL){
            ConfigScene *pScene = cfgView->scene();
            bool isToDebug = !pScene->isDebuging();
            pScene->setDebugMode(isToDebug, isToDebug);
            if (isToDebug) {
                actionCursor->setChecked(true);
                actionHandle->setChecked(false);
                cfgView->setDragMode(QGraphicsView::RubberBandDrag);
            }
            else {
                actionCursor->setVisible(false);
                actionHandle->setVisible(false);
                cfgView->setDragMode(QGraphicsView::RubberBandDrag);
            }
        }
    }
}

void MainWindow::setAudioMute(bool isMute)
{
    ConfigView *cfgView = qobject_cast<ConfigView*> (_mdiArea->activeSubWindow()->widget());
    if(cfgView != NULL){
        ConfigScene *pScene = cfgView->scene();
        bool succeed = pScene->setOutputMute(isMute);
        if(!succeed) return;
        QString iconDir = Config::instance()->curSkinStyle()->getSkinIconDir();
        if(isMute){
            _actionAudioMute->setIcon(QIcon(QString(":/Image/%1/AudioMute.png").arg(iconDir)));
        }
        else{
            _actionAudioMute->setIcon(QIcon(QString(":/Image/%1/AudioOn.png").arg(iconDir)));
        }
    }
}

void MainWindow::setSheetSize(QRectF rect)
{
    _statusLabelList.at(STATUSBAR_SHEET_SIZE)->setText(tr("Sheet Size: %1x%2").arg(rect.width()).arg(rect.height()));
}

void MainWindow::updateDeviceModeStatus(ConfigScene* cfgScene)
{
    bool isDebug = cfgScene->isDebuging();
    _statusLabelList.at(STATUSBAR_DEVICE_STATUS)->setText(tr("Status:%1").arg(isDebug ? tr("run") : tr("edit")));

    //刷新模式切换按钮及输出静音按钮
    _actionAudioMute->setEnabled(isDebug);

    QString iconDir = Config::curSkinStyle()->getSkinIconDir();
    if(isDebug) {
        _actionDownAlgorithm->setIcon(QIcon(QString(":/Image/%1/editMode.png").arg(iconDir)));
        _actionDownAlgorithm->setText(tr("to edit mode"));
    }
    else {
        _actionDownAlgorithm->setIcon(QIcon(QString(":/Image/%1/debugMode.png").arg(iconDir)));
        _actionDownAlgorithm->setText(tr("to debug mode"));
    }

    //改变dock窗口显示按钮
    updateDockWidgets(VIEW_CONFIG, cfgScene); //根据视图刷新Dock控件
}

//校验算法时更新使用率
void MainWindow::setUsagePercent(float spacePercent, float timePercent)
{
    _statusLabelList.at(STATUSBAR_SPACE_USAGE)->setText(tr("Total Space Usage %1%").arg(spacePercent));
    _statusLabelList.at(STATUSBAR_TIME_USAGE)->setText(tr("Total Time Usage %1%").arg(timePercent));
}



#include "customWidget/CustomGraphicsItem/graphicsitem.h"
#include "serpentineconnector.h"
void MainWindow::initTreeProperty(QGraphicsItem* item)
{
    DesignItem* devItem = qgraphicsitem_cast<DesignItem*>(item);
    GraphicsItem *grapItem = qgraphicsitem_cast<GraphicsItem *>(item);
    SerpentineConnector *serConItem = qgraphicsitem_cast<SerpentineConnector *>(item);
    if(grapItem) {
        _propertyDockBox->initTreeProperty(grapItem);
    }else if(devItem) {
        _propertyDockBox->initTreeProperty(devItem);
    }else if(serConItem) {
        _propertyDockBox->initTreeProperty(serConItem);
    }else {//清空属性栏
        _propertyDockBox->initTreeProperty(NULL);
    }

    if(_pinsViewerDockBox->isVisible())
    {
        ConfigItem* cfgItem = qobject_cast<ConfigItem*>(devItem);
        QWidget* infoWidget = ConfigItem::getControlsInfoWidget(cfgItem);
        _pinsViewerDockBox->setWidget(infoWidget);
    }
}



QString MainWindow::getSolutionPath(QString fileName)
{
    if(fileName.isEmpty())
        return _slnDockBox->getSolutionFilePath();
    else
        return _slnDockBox->getAbsolutePathOfSln(fileName);
}

void MainWindow::openDevModulePath(QString Mac, QString fileName)
{
    QString projFile = _slnDockBox->getAbsolutePathOfSln(fileName);
    openSubWindow(projFile);
    ProjectView *proView = qobject_cast<ProjectView *>(_mdiArea->currentSubWindow()->widget());
    if(!proView) return;
    foreach(QGraphicsItem *item, proView->scene()->items()) {
        if(item->type() != DeviceItem::Type) continue;
        DeviceItem *devItem = qgraphicsitem_cast<DeviceItem *>(item);
        if(devItem->getAES70Device()->getAES70Mac() == Mac) {
            proView->scene()->clearSelection();
            proView->centerOn(item);
            devItem->setSelected(true);
            break;
        }
    }
}


void MainWindow::updateSolutionRoomPorts(QString fileName, QMap<int, QList<QString>> roomInports, QMap<int, QList<QString>> roomOutports)
{
    if(fileName.endsWith('*')) fileName.chop(1);
    SolutionView *slnView = getSubWindowForTitle<SolutionView>(QFileInfo(getSolutionPath()).fileName());
    if(!slnView) return ;

    QMapIterator<int, QList<QString>> inIter(roomInports);
    while(inIter.hasNext()) {
        inIter.next();
        QList<QString> list = inIter.value();
        roomInports.insert(inIter.key(), Utils::insertionSort(list));
    }
    QMapIterator<int, QList<QString>> outIter(roomOutports);
    while(outIter.hasNext()) {
        outIter.next();
        QList<QString> list = outIter.value();
        roomOutports.insert(outIter.key(), Utils::insertionSort(list));
    }

    foreach(QGraphicsItem *item, slnView->scene()->items()) {
        DeviceItem_Room *roomItem = slnView->scene()->castToRoomItem(item);
        if(!roomItem) continue;
        if(!QString::compare(fileName, roomItem->roomName() + ".pro")) {
            roomItem->setInputOutPorts(roomInports, roomOutports);
            break;
        }
    }
}


template<typename View>
View *MainWindow::getSubWindowForTitle(QString title)
{
    View *view = 0;
    foreach(QMdiSubWindow *subWnd, _mdiArea->subWindowList()) {
//        qDebug() << __FUNCTION__ << __LINE__ << subWin->windowTitle() << title;
        QString roomName = subWnd->windowTitle();
        if(roomName.endsWith('*')) roomName.chop(1);
        if(roomName == title) {
            view = qobject_cast<View *>(subWnd->widget());
            break;
        }
    }
    return view;
}


////////////////////////////////////////////////////////
/// \brief Singleton模式相关代码
QMutex MainWindow::Mutex;//实例互斥锁。
MainWindow* MainWindow::_Instance; //<使用原子指针,默认初始化为0。

MainWindow* MainWindow::instance()
{
    //使用双重检测。
    if(!_Instance)//第一次检测
    {
        QMutexLocker locker(&Mutex);//加互斥锁。

        if(!_Instance)//第二次检测。
        {
            _Instance = new MainWindow();
        }
    }

    return _Instance;
}


