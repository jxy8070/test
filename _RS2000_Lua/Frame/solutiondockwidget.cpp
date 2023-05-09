#include "solutiondockwidget.h"
#include "projectview.h"
#include "configview.h"
#include "mainwindow.h"

#define FilePathRole (Qt::UserRole+1)

SolutionDockWidget::SolutionDockWidget()
{
    _slnTree = new QTreeWidget(this);    
    _slnTree->setContextMenuPolicy(Qt::CustomContextMenu);
    _slnTree->setHeaderHidden(true);
    _slnTree->setDragDropMode(QAbstractItemView::DragOnly);
    _slnTree->setSelectionMode(QAbstractItemView::ContiguousSelection);
    _slnTree->setAnimated(true);
    this->setWidget(_slnTree);

    connect(_slnTree, &QTreeView::customContextMenuRequested, this, &SolutionDockWidget::onTreeViewMenuRequested);
    connect(_slnTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));
}


void SolutionDockWidget::setSolutionDir(QString slnPath)
{
    _solutionDir = QDir(slnPath);
    QString dirName = _solutionDir.dirName();
    updateSlnTree();
}

void SolutionDockWidget::importFile(QString filePath)
{
    QFileInfo fileInfo(filePath);
    QString newFile = filePath;
    if(fileInfo.absoluteDir() != _solutionDir)
    {
        newFile = _solutionDir.absoluteFilePath(fileInfo.fileName());
        if(QFile::exists(newFile)){
            QMessageBox::StandardButton rsBtn = Utils::MessageBoxQuestion(tr("Question"), tr("Do you want to overwrite file?"),
                                   QMessageBox::Yes  | QMessageBox::No );
            if (rsBtn == QMessageBox::Yes) {
                QFile::remove(newFile);
                QFile::copy(filePath, newFile);
            }
            else{
                return;
            }
        }
        else{
            QFile::copy(filePath, newFile);
        }
        updateSlnTree();
    }
    emit openSubWindow(newFile);
}

void SolutionDockWidget::updateSlnTree()
{
    //清空数据
    _slnTree->clear();
    _allProjectList.clear();
    _allConfigList.clear();

    QDir slnDir(_solutionDir);
    if(!slnDir.exists()) {
        return;
    }
    slnDir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList fileList = slnDir.entryInfoList();
    for(QFileInfo fileInfo : fileList)
    {
        QString suffix = fileInfo.suffix();
        if(suffix.startsWith("pro", Qt::CaseInsensitive))
        {
            _allProjectList.append(fileInfo.absoluteFilePath());
        }
        else if(suffix.startsWith("nmu", Qt::CaseInsensitive))
        {
            _allConfigList.append(fileInfo.absoluteFilePath());
        }
        else if(suffix.startsWith("sln", Qt::CaseInsensitive))
        {
            _slnFileName = fileInfo.fileName();
        }
    }

    //更新solution树
    QList<QTreeWidgetItem *> itemList;
    QTreeWidgetItem * solutionItemsR = new QTreeWidgetItem(QStringList() << _slnFileName);
    solutionItemsR->setData(0, FilePathRole, getSolutionFilePath());
    itemList.append(solutionItemsR);
    QTreeWidgetItem * projItemsR = new QTreeWidgetItem(QStringList() << tr("Projects"));
    itemList.append(projItemsR);
    int i = 0;
    for(QString projFile : _allProjectList)
    {
        QString title = QFileInfo(projFile).fileName();
        QTreeWidgetItem* pItem =new QTreeWidgetItem(QStringList() << title);
        pItem->setData(0, FilePathRole, projFile);
        projItemsR->addChild(pItem);
    }

    QTreeWidgetItem * cfgItemsR = new QTreeWidgetItem(QStringList() << tr("Configs"));
    itemList.append(cfgItemsR);
    for(QString cfgFile : _allConfigList)
    {
        QString title = QFileInfo(cfgFile).fileName();
        QTreeWidgetItem* cItem =new QTreeWidgetItem(QStringList() << title);
        cItem->setData(0, FilePathRole, cfgFile);
        cfgItemsR->addChild(cItem);
        cItem->setFlags(cItem->flags() & ~Qt::ItemIsDragEnabled);
    }
    foreach(QTreeWidgetItem *item, itemList)
        item->setFlags(item->flags() & ~Qt::ItemIsDragEnabled);

    _slnTree->addTopLevelItems(itemList);
    _slnTree->expandAll();
}


void SolutionDockWidget::onTreeViewMenuRequested(const QPoint &pos)
{
    QModelIndex curIndex = _slnTree->indexAt(pos);
    QIcon flashIcon = QApplication::style()->standardIcon(QStyle::SP_BrowserReload);
    QMenu menu;
    menu.addAction(flashIcon, tr("flash"), this, &SolutionDockWidget::updateSlnTree);
    if (curIndex.isValid()) // 右键选中有效Item,创建菜单
    {
        QIcon deleteIcon = QApplication::style()->standardIcon(QStyle::SP_BrowserStop);
        QIcon renameIcon = QApplication::style()->standardIcon(QStyle::SP_DesktopIcon);
        menu.addAction(deleteIcon, tr("delete"), this, &SolutionDockWidget::onDeleteItem);
        menu.addAction(renameIcon, tr("rename"), this, &SolutionDockWidget::onRenameItem);
    }
    menu.exec(QCursor::pos());
}

void SolutionDockWidget::onItemDoubleClicked(QTreeWidgetItem* item, int column)
{
    QString filePath = item->data(0, FilePathRole).value<QString>();
    if(!filePath.isEmpty()){
        emit openSubWindow(filePath);
    }
}

void SolutionDockWidget::onDeleteItem()
{
    QTreeWidgetItem* curItem = _slnTree->currentItem();
    if(curItem != NULL){
        QString filePath = curItem->data(0, FilePathRole).value<QString>();
        if(QFile::exists(filePath)){
            QFile::remove(filePath);
            emit deleteSubWindow(filePath);
        }
    }
}

void SolutionDockWidget::onRenameItem()
{
    QTreeWidgetItem* curItem = _slnTree->currentItem();
    if(curItem != NULL){
        QString oldFilePath = curItem->data(0, FilePathRole).value<QString>();
        QFileInfo oldFile(oldFilePath);
        if(oldFile.exists()){
            QString oldName = oldFile.baseName();
            QString newName = Utils::getNewName(oldName);
            if(!newName.isEmpty()){
                newName += "." + oldFile.suffix();
                QString newFilePath = oldFile.absoluteDir().absoluteFilePath(newName);
                QFile::rename(oldFilePath, newFilePath);
                emit renameSubWindow(oldFilePath, newFilePath);
            }
        }
    }
}

