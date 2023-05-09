#include "configview.h"
#include "configscene.h"
#include "config.h"
#include "device/devicemanager.h"
#include "deviceItem/deviceitem_trn.h"
#include <QMessageBox>
#include <QFile>
#include "mainwindow.h"
#include "projectview.h"
#include "projectscene.h"
#include "device/AES70device.h"
#include "device/device_trn.h"
#include "configItem/configitem.h"
#include "configItem/configitemmeter.h"
#include "configSetting/cfgsettingmeter.h"
#include "customWidget/tooltip.h"
#include "configItem/configitemsnapshot.h"

ConfigView::ConfigView(QString title, Device_Trn* trnDev,  QWidget *parent) :
    AbsGraphView(VIEW_CONFIG, title, parent)
{
    _cfgScene = new ConfigScene(trnDev, this);
    setScene(_cfgScene);//设置当前场景

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);//当场景中的任何可见部分发生变化时，更新整个窗口
    setAcceptDrops(true);//此小部件启用拖放事件
    setMouseTracking(true);//此小部件启用鼠标跟踪事件
    setDragMode(QGraphicsView::RubberBandDrag);//区域选择
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    setAttribute(Qt::WA_DeleteOnClose);

    QObject::connect(trnDev, SIGNAL(deviceNameChanged(const QString&)), this, SLOT(setWindowTitle(const QString&)));
    connect(_cfgScene, SIGNAL(needSaveChanged(bool)), this, SLOT(onTitleStateChanged(bool)));
}

ConfigView::~ConfigView()
{
    delete _cfgScene;
}

ConfigScene *ConfigView::scene()
{
    return _cfgScene;
}

void ConfigView::showEvent(QShowEvent *)
{
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void ConfigView::closeEvent(QCloseEvent *event)
{
    if (_cfgScene->isNeedSave())
    {
        QMessageBox::StandardButton rsBtn = Utils::MessageBoxQuestion(tr("Save"), tr("Do you want to save the change?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (rsBtn == QMessageBox::Save) {
            saveScene();
            _cfgScene->getTrnDev()->disconnect(this); //disconnect设备名字改变事件
        }
        else if (rsBtn == QMessageBox::Discard) {
            event->accept();
            _cfgScene->getTrnDev()->disconnect(this); //disconnect设备名字改变事件
        }
        else {
            event->ignore();
            return;
        }
    }
    event->accept();
    emit viewClosed();
}

QUndoStack *ConfigView::getUndoStack()
{
    return _cfgScene->getUndoStack();
}

void ConfigView::saveScene(bool bMsg)
{
    _cfgScene->save();

    if (bMsg) {        
        MainWindow::instance()->showNotice(tr("Save Success"));
    }
}

void ConfigView::saveAsScene()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, tr("save as..."), QDir::currentPath(), tr("file (*.nmu)"));

    if(fileName == NULL) {
        return;
    }
    if(QFile::exists(fileName)){
        QFile::remove(fileName);
    }
    _cfgScene->saveAs(fileName);
}

QString ConfigView::getWndName()
{
    QStringList strlist = _cfgScene->filePath().split("/");//分割
    return strlist.last();//返回名称
}
