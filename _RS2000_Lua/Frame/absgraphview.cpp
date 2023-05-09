#include "stable.h"
#include "absgraphview.h"
#include "absgraphscene.h"
#include "DesignItem.h"
#include "common.h"
#include "mainwindow.h"

AbsGraphView::AbsGraphView(SubWnd_Type type, QString wndTitle, QWidget *parent)
    :QGraphicsView(parent)
{
    _type = type;
    _scalePercent = 100;
    _wndTitle = wndTitle;
    setWindowTitle(wndTitle);//设置标题
}

QString AbsGraphView::getSheetSize() const
{
    return  tr("Sheet Size: %1x%2").arg(width()).arg(height());
}

void AbsGraphView::changedScale(int newScalePercent)
{
    if(newScalePercent%5 != 0)
    {
        qDebug() <<newScalePercent;
    }
    if (newScalePercent > 200) return;
    if (newScalePercent < 20) return;
    double scaleVal = 1.0/_scalePercent * newScalePercent;
    _scalePercent = newScalePercent;
    scale(scaleVal, scaleVal);
}


void AbsGraphView::onTitleStateChanged(bool needSave)
{
    QString title = _wndTitle;
    if(needSave){
        title += "*";
    }
    setWindowTitle(title);
}


void AbsGraphView::mousePressEvent(QMouseEvent *event)
{
    _mousePressedPoint = event->pos();
    QGraphicsView::mousePressEvent(event);
}

void AbsGraphView::mouseMoveEvent(QMouseEvent *event)
{
    if (dragMode() == QGraphicsView::RubberBandDrag)
    {
        Qt::ItemSelectionMode mode = Qt::ItemSelectionMode::ContainsItemShape;
        if(_mousePressedPoint.x() > event->x())
            mode = Qt::ItemSelectionMode::IntersectsItemShape;
        setRubberBandSelectionMode(mode);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void AbsGraphView::mouseReleaseEvent(QMouseEvent *event)
{
    _rubberRect = QRectF(mapToScene(rubberBandRect().topLeft()), rubberBandRect().size());
    AbsGraphScene* pScene = qobject_cast<AbsGraphScene*>(scene());
    pScene->focusSingleItemInRect(_rubberRect);
    QGraphicsView::mouseReleaseEvent(event);
}


#include "customWidget/configgainwidget.h"
void AbsGraphView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        ConfigGainWidget* focusGain = ConfigGainWidget::LastFocusedWidget;
        if(focusGain != NULL && focusGain->hasFocus()){
            focusGain->adjustOnePercent(event->delta() > 0);
        }
        else{
            int newScale = _scalePercent;
            if(event->delta() > 0) {//deita()获取滚轮移动的距离
                newScale = newScale + 5;
            }
            else {
                newScale = newScale - 5;
            }

            changedScale(newScale);
            emit onScaleValueChanged(newScale);
        }
    }
    else {        
        QGraphicsView::wheelEvent(event);
    }


}
