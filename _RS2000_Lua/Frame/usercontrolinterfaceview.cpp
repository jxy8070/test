#include "usercontrolinterfaceview.h"
#include "usercontrolinterfacescene.h"
#include "configItem/configitemuci.h"
#include "mainwindow.h"
#include "device/device_trn.h"

UserControlInterfaceView::UserControlInterfaceView(QString title, QGraphicsItem *item, QWidget *parent)
    : AbsGraphView{VIEW_USERCONTROLINTERFACE, title, parent}
{
    _scene = new UserControlInterfaceScene(item, this);
    _GrapItem = item;

    setScene(_scene);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);//当场景中的任何可见部分发生变化时，更新整个窗口
//    setAcceptDrops(true);//此小部件启用拖放事件
//    setMouseTracking(true);//此小部件启用鼠标跟踪事件
    setDragMode(QGraphicsView::RubberBandDrag);/* 使用view中方法来实现区域选择功能 */ //保存在鼠标左键按下的时候，在场景中拖动鼠标的行为（使用橡皮筋选择设备）
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
}

QUndoStack *UserControlInterfaceView::getUndoStack()
{
    return _scene->getUndoStack();
}

void UserControlInterfaceView::saveScene(bool bMsg)
{
//    qDebug() << __FUNCTION__ << __LINE__ << bMsg;
    Device_Trn *trnDev = _scene->getUciItem()->getAES70Dev();
    QString roomName = trnDev->getCurRoomName();
    roomName.chop(4);
    QString defDir = MainWindow::instance()->getSolutionPath("UCI/" + roomName);
    QDir dir(defDir);
    if(!dir.exists()) {
        if(!dir.mkpath(defDir)) {
            Utils::MessageBoxInfo(tr("Infomation"), tr("The solution path is invalid!"));
            return ;
        }
    }
//    qDebug() << __FUNCTION__ << __LINE__ << defDir;
//    _scene->saveAs("config/userControlInterface.uci");
    _scene->saveAs(defDir + "/" + trnDev->getDeviceName() + ".uci");
}

void UserControlInterfaceView::saveAsScene()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("save as..."), QDir::currentPath(), tr("file (*.uci)"));
    if(fileName == NULL) {
        return ;
    }
    _scene->saveAs(fileName);
//    _scene->saveAs("config/userControlInterface.uci");
}

QString UserControlInterfaceView::getWndName()
{
    return NULL;
}

void UserControlInterfaceView::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit viewClosed();
}
