#include "stable.h"
#include "solutionview.h"
#include "solutionscene.h"
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

SolutionView::SolutionView(QString title, QWidget *parent) :
    AbsGraphView(VIEW_SOLUTION, title, parent)
{
    _slnScene = new SolutionScene(this);
    setScene(_slnScene);//设置当前的场景

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);//当场景中的任何小部件重新改变的时候，刷新窗口内容
    setAcceptDrops(true);//启用拖放事件
    setMouseTracking(true);//启用鼠标跟踪事件
    setDragMode(QGraphicsView::RubberBandDrag);
//    setDragMode(QGraphicsView::RubberBandDrag);//保存鼠标在视图中拖拽的行为（启用橡皮筋框选事件）
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(_slnScene, SIGNAL(needSaveChanged(bool)), this, SLOT(onTitleStateChanged(bool)));
}


SolutionScene *SolutionView::scene()
{
    return _slnScene;
}

void SolutionView::saveScene(bool bMsg)
{
    if(_slnScene->fileName() != NULL)
    {
        _slnScene->save(false);
    }
    else
    {
        //crrentPath() 返回应用程序的当前目录的绝对路径
        QString fileName = QFileDialog::getSaveFileName(this, tr("save as..."), QDir::currentPath(), tr("file (*.pro)"));
        if(fileName == NULL)
        {
            return;
        }
        _slnScene->saveAs(fileName);
    }
    if (bMsg) {
        MainWindow::instance()->showNotice(tr("save to %1 Success").arg(_slnScene->fileName()));
    }
    emit isSaved();
}

void SolutionView::saveAsScene()
{
    return;
}

QString SolutionView::getWndName()
{
    QStringList strlist = _slnScene->fileName().split("/");//分割

    return strlist.last();//返回名称
}


void SolutionView::onDownloadSolutionRoute()
{
    DeviceManager::instance()->downloadSolutionRoute(_slnScene);
}

void SolutionView::showEvent(QShowEvent *)//显示事件
{
    setAlignment(Qt::AlignLeft | Qt::AlignTop);//保存场景在视图中左对齐，且与上级保持一致
}

void SolutionView::closeEvent(QCloseEvent *event)//关闭事件
{
    if (_slnScene->isNeedSave())//如果需要保存
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


QUndoStack *SolutionView::getUndoStack()
{
    return _slnScene->getUndoStack();
}
