#include "stable.h"
#include "projectview.h"
#include "projectscene.h"
#include "mainwindow.h"
#include "config.h"
#include "device/devicemanager.h"
#include "deviceItem/deviceItem.h"
#include "device/AES70device.h"
#include "device/devicemodel.h"
#include <QMdiSubWindow>
#include "Frame/mainwindow.h"
#include "Frame/configview.h"
#include "Frame/configscene.h"

ProjectView::ProjectView(QString projName, QWidget *parent) :
    AbsGraphView(VIEW_PROJECT, projName, parent)
{
    _proScene = new ProjectScene(this);
    setScene(_proScene);//设置当前的场景

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);//当场景中的任何小部件重新改变的时候，刷新窗口内容
    setAcceptDrops(true);//启用拖放事件
    setMouseTracking(true);//启用鼠标跟踪事件
    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    setAttribute(Qt::WA_DeleteOnClose);

    connect(_proScene, SIGNAL(needSaveChanged(bool)), this, SLOT(onTitleStateChanged(bool)));
}

ProjectView::~ProjectView()
{
    delete _proScene;
}

ProjectScene *ProjectView::scene()
{
    return _proScene;
}

void ProjectView::saveScene(bool bMsg)
{
    _proScene->save(_proScene->filePath(), false);

    if (bMsg) {
        MainWindow::instance()->showNotice(tr("save to %1 Success").arg(_proScene->filePath()));
    }
    emit isSaved();
}

void ProjectView::saveAsScene()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("save as..."), QDir::currentPath(), tr("file (*.pro)"));
    if(fileName == NULL) {
        return;
    }
    _proScene->saveAs(fileName);
    MainWindow::instance()->showNotice(tr("save to %1 Success").arg(fileName));

    emit isSaved();
}

QString ProjectView::getWndName()
{
    QStringList strlist = _proScene->filePath().split("/");//分割

    return strlist.last();//返回名称
}

//void ProjectView::newConfigView(DeviceItem_Trn* trnItem)
//{
//    MainWindow::instance()->addConfigView(trnItem);
//}

void ProjectView::onDownloadProjectRoute()
{
    DeviceManager::instance()->downloadProjectRoute(_proScene);
}

void ProjectView::showEvent(QShowEvent *)//显示事件
{
    setAlignment(Qt::AlignLeft | Qt::AlignTop);//保存场景在视图中左对齐，且与上级保持一致
}

void ProjectView::closeEvent(QCloseEvent *event)//关闭事件
{
    if (_proScene->isNeedSave())//如果需要保存
    {
        QMessageBox msgBox(QMessageBox::Question, tr("Save"), tr("Do you want to save the change?"), NULL);
        msgBox.setWindowFlags(Qt::FramelessWindowHint);
        QPushButton *SaveButton    = msgBox.addButton(tr("Save"), QMessageBox::AcceptRole);
        QPushButton *DiscardButton = msgBox.addButton(tr("Discard"), QMessageBox::RejectRole);
        QPushButton *CancelButton  = msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
        msgBox.exec();

        if ((QPushButton*)msgBox.clickedButton() == SaveButton) {
            saveScene();      
        }
        else if ((QPushButton*)msgBox.clickedButton() == DiscardButton) {
            event->accept();
        }
        else {
            event->ignore();
            return;
        }
    }
    emit viewClosed();
}


QUndoStack *ProjectView::getUndoStack()
{
    return _proScene->getUndoStack();
}
