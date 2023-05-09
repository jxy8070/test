#include "monitoringview.h"
#include "stable.h"
#include "monitoringscene.h"
#include "monitoring_aids/monitorblackstage.h"
#include "deviceSetting/devsetting_monitoring_aids.h"

MonitoringView::MonitoringView(QString windowTitle, QString sceneFieName, QWidget *parent) :
    AbsGraphView(VIEW_MONITOR_AIDS, windowTitle, parent)
{
    qDebug()<<"monitoring_view";
    _MonScene = new MonitoringScene(this);
    qDebug()<<"monitoring_Scene";
    _ViewLabelName = windowTitle;
    DevSetting_Monitoring_Aids *devSetting_Monitor = qobject_cast<DevSetting_Monitoring_Aids *>(parent);
    connect(_MonScene, SIGNAL(sendSaveclick(bool)), devSetting_Monitor, SLOT(onSave(bool)));
    connect(_MonScene, SIGNAL(sceneRectChanged(QRectF)), devSetting_Monitor, SLOT(setRectSize(QRectF)));
//     MonitorBlackStage::instance()->getSceneName(_ViewLabelName);

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setAcceptDrops(true);
    setMouseTracking(true);
    setScene(_MonScene);
    setDragMode(QGraphicsView::RubberBandDrag);
}

QUndoStack *MonitoringView::getUndoStack()
{
    return _MonScene->getUndoStack();
}

void MonitoringView::saveScene(bool bMsg)
{
    QSettings *monitorIni = new QSettings(_fileName, QSettings::IniFormat, 0);
    monitorIni->remove(QString(MONITOR_GROUP));
    _MonScene->SavePro(_fileName);
}

void MonitoringView::saveAsScene()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("save as..."), QDir::currentPath(), tr("file (*.pro)"));
    if(fileName == NULL)
    {
        return;
    }
//    _MonScene->saveAs(fileName);

//    emit isSaved();
}

QString MonitoringView::getWndName()
{
    return QString();
}


void MonitoringView::showEvent(QShowEvent *event)
{
     setAlignment(Qt::AlignLeft | Qt::AlignTop);//保存场景在视图中左对齐，且与上级保持一致
}

void MonitoringView::closeEvent(QCloseEvent *event)
{
//    if (_MonScene->isNeedSave())//如果需要保存
//    {
//        int ret = QMessageBox::question(NULL, tr("Save"),
//                                        tr("Do you want to save the change?"),
//                                        QMessageBox::Save | QMessageBox::No);
//        if (ret == QMessageBox::Save)
//        {
//            saveScene();
////            _proScene->save();
//        }
//    }
    emit viewClosed();
}

void MonitoringView:: openProjectFile(QString fileName)
{
    if(!fileName.isEmpty())
    {
        QStringList strList = _ViewLabelName.split("/");
        _MonScene->open(fileName, strList.at(1).toInt());
    }else
    {

    }
}

void MonitoringView::onGetRectsize(QRectF rect)
{
    _rect = rect;
//    MonitorBlackStage::instance()->getView(this);
}

void MonitoringView::onSavePro(QString fileName)
{
//    _MonScene->SavePro(fileName);
    _fileName = fileName;
}
