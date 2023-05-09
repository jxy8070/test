#include "uciitemview.h"
#include "uciitem_mixer.h"
UciItemView::UciItemView(QString devName,bool nodesSwitch, CfgType type, ConfigItemUCI *uciItem, QWidget *parent)
    : QGraphicsView{parent}, _scale(1), _type(type)
{
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);//当场景中的任何可见部分发生变化时，更新整个窗口
    switch(type&FCT_FAMILY_MASK) {
    case CFG_TYPE_MIXER_4X1:
        _scene = new UciItem_mixer(devName, nodesSwitch, type, uciItem, this);
        setScene(_scene);
        break;
    default: _scene = NULL; break;
    }
    setBackgroundBrush(QColor(78,92,90));
}

void UciItemView::changedScaling(qreal newScale)
{
    if(newScale > 1.5f) return;
    if(newScale < 0.5f) return;
    qreal scaleValue = newScale / _scale;
    scale(scaleValue, scaleValue);
    _scale = newScale;
}
#include "configItem/configitemuci.h"
void UciItemView::initData(QString ip)
{
    qDebug() << __FUNCTION__ << __LINE__ << qobject_cast<UciItem_mixer *>(_scene)->getCfgItemUci()->getLoadUciFileProcData().s_ip << ip;
    if(qobject_cast<UciItem_mixer *>(_scene)->getCfgItemUci()->getLoadUciFileProcData().s_ip != ip) return;
    switch(_type&FCT_FAMILY_MASK) {
    case CFG_TYPE_MIXER_4X1:
        qobject_cast<UciItem_mixer *>(_scene)->initDisplayRange();
        break;
    }
}

void UciItemView::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier){
        if(event->delta() > 0) {
            changedScaling(_scale+0.01f);
        }else {
            changedScaling(_scale-0.01f);
        }
    }
}
