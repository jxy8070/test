#include "pagemanager.h"
#include "Frame/mainwindow.h"

PageManager::PageManager(QWidget *parent) : QWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAttribute(Qt::WA_StyledBackground,true); //使QSS对该控件有效
    initUi();
}

void PageManager::initUi()
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(11, 11, 11, 11);
    layout->setObjectName(QString::fromUtf8("horizontalLayout"));
    layout->setContentsMargins(0, 0, 0, 0);
    _tabWidget = new QTabWidget(this);
    _tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    _tabWidget->setTabPosition(QTabWidget::South);
    _tabWidget->setStyleSheet("QTabWidget::pane{background:#FF0000;}");

    layout->addWidget(_tabWidget);

    _contextMenu = new QMenu(this);
    _actionAdd = new QAction(tr("Add page"), this);
    connect(_actionAdd, SIGNAL(triggered()), this, SLOT(onActionTriggered()));
    _contextMenu->addAction(_actionAdd);
    _actionDel = new QAction("Delete cur page", this);
    connect(_actionDel, SIGNAL(triggered()), this, SLOT(onActionTriggered()));
    _contextMenu->addAction(_actionDel);
    _actionRename = new QAction("Rename cur page", this);
    connect(_actionRename, SIGNAL(triggered()), this, SLOT(onActionTriggered()));
    _contextMenu->addAction(_actionRename);
    connect(_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onPageChanged(int)));
}


void PageManager::mousePressEvent(QMouseEvent *event)
{
    QWidget *child = _tabWidget->childAt(event->pos());
    if(child != NULL){
        _actionDel->setVisible(true);
        _actionRename->setVisible(true);
    }
    else{
        _actionDel->setVisible(false);
        _actionRename->setVisible(false);
    }
    _contextMenu->exec(QCursor::pos());
}

void PageManager::onActionTriggered()
{
    if (sender() == _actionAdd)
    {
        QString pageName = Utils::getNewName();
        if(!pageName.isEmpty()){
            _scene->addPage(pageName);
            reflashPage(_scene);
            _scene->setCurPage(pageName);
        }
    }
    else if(sender() == _actionDel) {
        QStringList pageList = _scene->getPageList();
        if(pageList.length() == 1) {
            MainWindow::instance()->showNotice(tr("Can't delete lastest page"));
            return;
        }
        int curIdx = _tabWidget->currentIndex();
        if(curIdx >= 0 && curIdx < pageList.length())
        {
            _scene->deletePage(pageList[curIdx]);
            reflashPage(_scene);
            _scene->setCurPage(pageList[0]);
        }
    }
    else if(sender() == _actionRename) {
        QStringList pageList = _scene->getPageList();
        int curIdx = _tabWidget->currentIndex();
        if(curIdx >= 0 && curIdx < pageList.length())
        {
            QString oldPageName = pageList[curIdx];
            QString newPageName = Utils::getNewName(oldPageName);
            _scene->renamePage(oldPageName, newPageName);
            reflashPage(_scene);
            _scene->setCurPage(newPageName);
        }
    }
}

void PageManager::onPageChanged(int index)
{
    if(_scene == NULL)
        return;
    QStringList pageList = _scene->getPageList();
    if(index < pageList.length())
    {
        _scene->setCurPage(pageList[index]);
    }
}


void PageManager::reflashPage(AbsGraphScene* scene)
{
    _scene = NULL;
    QStringList pageList;
    if(scene != NULL){
        pageList = scene->getPageList();
    }
    if(pageList.length() == 0){
        hide();
    }
    else{
        _tabWidget->clear();
        int index = 0;
        QString curPageName = scene->getCurPage();
        for(QString page : pageList){
            QWidget* tabPage = new QWidget();
            tabPage->setProperty("tabIndex", index++);
            _tabWidget->addTab(tabPage, page);
            if(page == curPageName){
                _tabWidget->setCurrentWidget(tabPage);
            }
        }

        _scene = scene;
        show();
    }
}

