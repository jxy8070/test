#include "borderdock.h"
#include <QMainWindow>
#include "Frame/mainwindow.h"
#include <QMdiSubWindow>
#include "Frame/toolkittree.h"
#include "Frame/configview.h"
#include "overwriteitem.h"

BorderDock::BorderDock(Qt::ToolBarArea barArea, QMainWindow *parent)
    : QToolBar(parent)
{
    parent->addToolBar(barArea, this);
    setAllowedAreas(barArea);

    this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->setStyleSheet(QLatin1String("QToolBar { spacing: 6px; border-top: 0px solid rgb(255, 255, 255); }"));
    setFixedWidth(20);
}

void BorderDock::addDockBtn(DockWidget *docWgt, SubWnd_Types subWinTypes)
{
    subWinTypes &= VIEW_Mask;
    _dockWidgets.insert(subWinTypes, docWgt);
}

void BorderDock::updateBorderDock(SubWnd_Type viewType)
{
    this->clear();
    QMapIterator<SubWnd_Types, DockWidget *> iter(_dockWidgets);
    for(iter.toBack(); iter.hasPrevious();) {
        iter.previous();
        SubWnd_Types types = iter.key();
        if(types.testFlag(viewType)) {
            VerticalPushButton *btn = new VerticalPushButton(iter.value(), allowedAreas());
            this->addWidget(btn);
        }
    }
}

VerticalPushButton::VerticalPushButton(DockWidget *dockWid, Qt::ToolBarAreas barArea, QWidget *parent)
    :QPushButton(dockWid->windowTitle(), parent), _barArea(barArea), _dockWid(dockWid)
{
    this->setCheckable(true);
    this->setChecked(dockWid->isVisible());
    _lab = new QLabel;
    _font = SkinStyle::getFont(10);
    _isInside = false;
    connect(this, QOverload<bool>::of(&VerticalPushButton::clicked), dockWid, &DockWidget::setVisible);
    connect(dockWid, QOverload<bool>::of(&DockWidget::closeSignal), this, &VerticalPushButton::setChecked);
}


void VerticalPushButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    setFixedHeight(getTextPoint());

    QPainter paint(this);
    paint.setPen(QPen(Config::curSkinStyle()->VolumnLableColor, 2));
    paint.save();
    paint.rotate(90);
    paint.setFont(_font);
    paint.drawText(0, getPos(), this->text());
    paint.restore();

    if (_isInside) {
        paint.setPen(QPen(QColor(0, 255, 0), 5));
    }else {
        paint.setPen(QPen(Config::curSkinStyle()->OnlineTextColor, 5));
    }
    if (_barArea == Qt::LeftToolBarArea) {
        paint.drawLine(0, 0, 0, height());
    }
    else if (_barArea == Qt::RightToolBarArea) {
        paint.drawLine(width(), 0, width(), height());
    }

//    paint.drawRect(rect());
}

void VerticalPushButton::enterEvent(QEvent *ev)
{
    _isInside = true;
}

void VerticalPushButton::leaveEvent(QEvent *ev)
{
    _isInside = false;
}


int VerticalPushButton::getTextPoint()
{  
    QFontMetrics fm(_font);
    _lab->setText(this->text() + " \0");
    QRect rec = fm.boundingRect(_lab->text());
    return rec.width();
}

int VerticalPushButton::getPos()
{
    if(_barArea == Qt::LeftToolBarArea) {
        return 0 - 6;
    }else if(_barArea == Qt::RightToolBarArea) {
        return -(width() - 20) - 6;
    }else {
        return 0;
    }
}



