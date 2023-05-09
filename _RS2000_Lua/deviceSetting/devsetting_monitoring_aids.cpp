#include "devsetting_monitoring_aids.h"
#include "ui_devsetting_monitoring_aids.h"
#include "frame/monitoringview.h"
#include "frame/monitoringscene.h"
#include "monitoring_aids/monitorblackstage.h"
#include <QMessageBox>
#include <QStatusBar>
#include <QLabel>
#include <QToolBar>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QSettings>
#include <QSplitter>
#include "Frame/mainwindow.h"


DevSetting_Monitoring_Aids::DevSetting_Monitoring_Aids(DeviceItem_Monitoring_Aids *devItem,QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSetting_Monitoring_Aids)
{
    ui->setupUi(this);

    _monitorView = NULL;
    _subWindowIndex = 0;
    _save = false;
    _devItem = devItem;
    _device = qobject_cast<Device_Monitoring_Aids *>(devItem->getAES70Device());
    ui->mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation);
    ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    ui->mdiArea->setTabShape(QTabWidget::Triangular);

    connect(ui->pushButton_new, SIGNAL(clicked(bool)), this, SLOT(onPushBuottonClicked()));
    connect(ui->pushButton_open, SIGNAL(clicked(bool)), this, SLOT(onPushBuottonClicked()));
    connect(ui->pushButton_save, SIGNAL(clicked(bool)), this, SLOT(onPushBuottonClicked()));
    connect(ui->pushButton_saveAs, SIGNAL(clicked(bool)), this, SLOT(onPushBuottonClicked()));
    connect(ui->pushButton_undo, SIGNAL(clicked(bool)), this, SLOT(onPushBuottonClicked()));
    connect(ui->pushButton_redo, SIGNAL(clicked(bool)), this, SLOT(onPushBuottonClicked()));
    connect(ui->pushButton_close, SIGNAL(clicked(bool)), this, SLOT(onPushBuottonClicked()));
    connect(ui->pushButton_Down, SIGNAL(clicked(bool)), this, SLOT(onPushBuottonClicked()));
    connect(ui->mdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(onSubWindowActivated(QMdiSubWindow*)));
    connect(MonitorBlackStage::instance(), SIGNAL(onDownSuccess(bool)), this, SLOT(onShowDown(bool)));
    connect(MonitorBlackStage::instance(), SIGNAL(sendStatusSignal(QString)), this, SLOT(setStatus(QString)));

    QList<QTreeWidgetItem *> monitorItems = ToolKitTree::GetGuiItems();
    ui->treeWidget->SetTreeItems(monitorItems);

    ui->pushButton_Down->setFocus();
    qDebug()<<"VIEW_MONITOR_AIDS";
    //初始化状态栏
    setStatus("Ready");
//    _device->sendStartMoniter();
    if(!MonitorBlackStage::instance()->getViewList().isEmpty()){
        setInitialize();
    }else{
        addProjectView(QString());
    }
    //QSplitter
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(5);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(ui->treeWidget);
    splitter->addWidget(ui->mdiArea);
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->gridLayout->addWidget(splitter,1,0,1,9);//设置拆解工具的位置,所占行列
    splitter->show();

    //////////////////最大化最小化按钮/////////////
    Qt::WindowFlags ture = Qt::Dialog;
    ture |= Qt::WindowMinimizeButtonHint;
    ture |= Qt::WindowMaximizeButtonHint;
    ture |= Qt::WindowCloseButtonHint;
    setWindowFlags(ture);
//    showMaximized();//打开即为最大化
//    setModal(true);
}

DevSetting_Monitoring_Aids::~DevSetting_Monitoring_Aids()
{
//    _device->sendStopMoniter();
//    delete ui;
}

void DevSetting_Monitoring_Aids::addProjectView(QString fileName)
{
    qDebug()<<"==================addProjectView";
    QString viewName = NULL;
    if(fileName.isEmpty())
    {
        viewName = QString("Monitor_Untitled%1").arg(++_subWindowIndex);
    }else
    {
        viewName = fileName;
    }
    _monitorView = new MonitoringView(viewName, fileName, this);
    connect(_monitorView, SIGNAL(viewClosed()), this, SLOT(onSuWindowClosed()));
    connect(this, SIGNAL(closeView()), _monitorView, SLOT(onCloseSignal()));
    connect(this, SIGNAL(savePro(QString)),_monitorView, SLOT(onSavePro(QString)));
//    _monitorView->openProjectFile(fileName);
    ui->mdiArea->addSubWindow(_monitorView);
    _monitorView->showMaximized();
    ui->pushButton_Down->setFocus();
}

void DevSetting_Monitoring_Aids::setInitialize()
{
    MonitorBlackStage::instance()->getOpenView();
    MonitorBlackStage::instance()->clearViewList();
    for(int i = MonitorBlackStage::instance()->getOpenViewList().count() - 1; i >=0; i--){
        addProjectView(QString());
        qDebug()<<"MonitorBlackStage::instance()->getOpenViewList().at(i):"<<&MonitorBlackStage::instance()->getOpenViewList().at(i);
//        MonitorBlackStage::instance()->setView(MonitorBlackStage::instance()->getOpenViewList().at(i), _monitorView);
    }
    MonitorBlackStage::instance()->clearOpenViewData();
}

void DevSetting_Monitoring_Aids::onSubWindowActivated(QMdiSubWindow *activeWindow)
{
    QList<QTreeWidgetItem *> monitorItems = ToolKitTree::GetGuiItems();
    ui->treeWidget->SetTreeItems(monitorItems);
    qDebug()<<"VIEW_MONITOR_AIDS";
}

void DevSetting_Monitoring_Aids::onPushBuottonClicked()
{
    if(sender() == ui->pushButton_Down)
    {
        MonitorBlackStage::instance()->upDateOnlineDev();
        MonitorBlackStage::instance()->getDownData();
    }
    if(sender() == ui->pushButton_close)
    {
        close();
    }
    if(sender() == ui->pushButton_new)
    {
       if(_monitorView != NULL)
       {
           QMessageBox msgBox(QMessageBox::Question, tr("Save"), tr("Do you want to close the project?"), NULL);
           msgBox.setWindowFlags(Qt::FramelessWindowHint);
           QPushButton *okButton = msgBox.addButton(tr("Yes"), QMessageBox::AcceptRole);
           QPushButton *cancelButton = msgBox.addButton(tr("No"), QMessageBox::RejectRole);
           msgBox.exec();
           if ((QPushButton*)msgBox.clickedButton() == okButton)
           {

           }
       }
       addProjectView(QString());
    }
    if(sender() == ui->pushButton_open)
    {
        QString fileName = NULL;
        if(ui->mdiArea->subWindowList().isEmpty())
        {
             qDebug()<<"mdiArea is Empty!";
        }else
        {
            qDebug()<<"mdiArea is not Empty!";
//            AbsGraphView *subwnd = qobject_cast<AbsGraphView *>(ui->mdiArea->currentSubWindow()->widget());
//            if(subwnd->getType() == VIEW_MONITOR_AIDS)
            {
                if(_monitorView != NULL)
                {
                    QMessageBox msgBox(QMessageBox::Question, tr("Save"), tr("Do you want to close the project?"), NULL);
                    msgBox.setWindowFlags(Qt::FramelessWindowHint);
                    QPushButton *okButton = msgBox.addButton(tr("Yes"), QMessageBox::AcceptRole);
                    QPushButton *cancelButton = msgBox.addButton(tr("No"), QMessageBox::RejectRole);
                    msgBox.exec();
                    if ((QPushButton*)msgBox.clickedButton() == okButton)
                    {
                        foreach(QMdiSubWindow *subWnd, ui->mdiArea->subWindowList())
                        {
                            subWnd->close();
                            _monitorView = NULL;
                        }
                    }
                }
            }
        }
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), tr("monitor project(*.mon)"));
        qDebug()<<"fileName :::"<<fileName;
        if(fileName == NULL) return ;
        _fileName = fileName;

        QSettings *monitorIni = new QSettings(_fileName, QSettings::IniFormat, 0);
        monitorIni->beginGroup(MONITOR_GROUP);
        monitorIni->beginGroup(MONITOR_INFOMATION_GROUP);
        quint32 viewCount = monitorIni->value(MONITOR_GROUP_COUNT).toUInt();
        monitorIni->endGroup();
        for(quint32 i = 1; i <= viewCount; i++){
                addProjectView(QString("Monitor_Untitled/%1").arg(i) + "/" + _fileName);
        }
        foreach(QMdiSubWindow *curSubWud, ui->mdiArea->subWindowList()){
            AbsGraphView *MonitorView = qobject_cast<AbsGraphView *>(curSubWud->widget());
            MonitoringView *monView = qobject_cast<MonitoringView *>(MonitorView);
            monView->openProjectFile(_fileName);
        }

    }
    if(sender() == ui->pushButton_save)
    {
        if(_monitorView != NULL)
//        if(ui->mdiArea->currentSubWindow())//返回当前子窗口的指针，如果没有则返回0
        {
//            MonitorBlackStage::instance()->setViewNum(ui->mdiArea->subWindowList().count());
            MonitorBlackStage::instance()->setViewNum(0);
            foreach(QMdiSubWindow *curSubWud, ui->mdiArea->subWindowList())
            {
//                qDebug()<<ui->mdiArea->subWindowList().count();
                AbsGraphView *MonitorView = qobject_cast<AbsGraphView *>(curSubWud->widget());
                if(MonitorView->getType() == VIEW_MONITOR_AIDS){
                    if(_fileName == NULL)
                    {
                        QString currentPath = QCoreApplication::applicationDirPath();//返回包含应用程序可执行文件的目录
                        QString dirTitle = "Save File";
                        QString filter = "mon文件(*.mon);;所有文件(*.*)";
                        QString fileName = QFileDialog::getSaveFileName(NULL, dirTitle, currentPath, filter);
                        if(fileName == NULL) return ;
                        qDebug()<<"fileName:"<<fileName;
                        _fileName = fileName;
                        emit savePro(fileName);
                        MonitorView->saveScene();
                    }else if(_fileName != NULL)
                    {
                        emit savePro(_fileName);
                        MonitorView->saveScene();
                    }
                }
            }
        }
        else {
            MainWindow::instance()->showNotice(tr("There is no project!"));
        }
    }
    if(sender() == ui->pushButton_saveAs)
    {
        if(_monitorView != NULL)
        {

            foreach(QMdiSubWindow *curSubWud, ui->mdiArea->subWindowList())
            {
//                qDebug()<<ui->mdiArea->subWindowList().count();
                AbsGraphView *MonitorView = qobject_cast<AbsGraphView *>(curSubWud->widget());
                if(MonitorView->getType() == VIEW_MONITOR_AIDS){

                        QString currentPath = QCoreApplication::applicationDirPath();//返回包含应用程序可执行文件的目录
                        QString dirTitle = "SaveAs File";
                        QString filter = "mon文件(*.mon);;所有文件(*.*)";
                        QString fileName = QFileDialog::getSaveFileName(NULL, dirTitle, currentPath, filter);
                        if(fileName == NULL) return ;
                        qDebug()<<"fileName:"<<fileName;
                        _fileName = fileName;
                        emit savePro(fileName);
                        MonitorView->saveScene();
                }
            }
        }
    }
    if(sender() == ui->pushButton_undo)
    {

    }
    if(sender() == ui->pushButton_redo)
    {

    }
}

void DevSetting_Monitoring_Aids::onShowDown(bool b)
{
    if(b) {
        MainWindow::instance()->showNotice( tr("Down success!"));
    }
    else {
        MainWindow::instance()->showNotice( tr("Down is failed!"));
    }
    ui->pushButton_Down->setFocus();
}

void DevSetting_Monitoring_Aids::closeEvent(QCloseEvent *)
{
//    if(_monitorView != NULL)
//    {
//        if(!_save)//如果没有保存
//        {
//            foreach(QMdiSubWindow *curSubWud, ui->mdiArea->subWindowList()){
//                AbsGraphView *monitorView = qobject_cast<AbsGraphView *>(curSubWud->widget());
//                if(monitorView->getType() == VIEW_MONITOR_AIDS){
//                    MonitoringView *view = qobject_cast<MonitoringView *>(monitorView);
//                    view->saveSceneItem();
//                    qDebug()<<"88888888888888888888";
//                }
//            }

////            int press = QMessageBox::question(NULL, tr("Save"), tr("Do you want to save the project?"), QMessageBox::Yes, QMessageBox::No);
////            if(press == QMessageBox::Yes)
////            {
////                if(_fileName != NULL){
////                    emit savePro(_fileName);
////                }else{
////                    QString currentPath = QCoreApplication::applicationDirPath();
////                    QString dirTitle = "Save File";
////                    QString filter = "mon文件(*.mon);;所有文件(*.*)";
////                    QString fileName = QFileDialog::getSaveFileName(NULL, dirTitle, currentPath, filter);
////                    if(fileName == NULL) return ;
////                    _fileName = fileName;
////                    emit savePro(_fileName);
////                }
////            }
//        }
//    }
////    MonitorBlackStage::instance()->clearData();
////    MonitorBlackStage::instance()->timeStop();
//    emit closeView();
}

void DevSetting_Monitoring_Aids::setStatus(QString statusText)
{
    qDebug()<<"setStatus:"<<statusText;
    ui->label_statusBar->setText(statusText);
    ui->label_statusBar->setFrameShape(QFrame::WinPanel);
    ui->label_statusBar->setFrameShadow(QFrame::Sunken);
//    update();
}

void DevSetting_Monitoring_Aids::setRectSize(QRectF rect)
{

    ui->label_rect->setText(tr("Sheet Size: %1x%2").arg(rect.width()).arg(rect.height()));
    ui->label_rect->setFrameShape(QFrame::WinPanel);
    ui->label_rect->setFrameShadow(QFrame::Sunken);
    ui->pushButton_Down->setFocus();
}

void DevSetting_Monitoring_Aids::onSuWindowClosed()
{
    for(int idx = 0; idx < ui->mdiArea->subWindowList().count(); idx++)
    {
        QMdiSubWindow *sub = ui->mdiArea->subWindowList().at(idx);
        if(sub == ui->mdiArea->currentSubWindow())
        {
            ui->mdiArea->removeSubWindow(sub);
            AbsGraphView *rssub = qobject_cast<AbsGraphView *>(sub->widget());
            if(rssub == _monitorView)
            {
                _monitorView = NULL;
            }
            delete rssub;
        }
    }
}

