#include "basewindow.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QFile>
#include <QGraphicsItem>
#include <Frame/configscene.h>
#include "configSetting/cfgsettingsnapshotbank.h"
#include "Frame/mainwindow.h"

BaseWindow::BaseWindow(QWidget *ui, QWidget *parent)
    : QWidget(parent)
{
    // FramelessWindowHint窗口去除边框; WindowMinimizeButtonHint 窗口最小化时显示在任务栏上;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    initUi(ui);
    _ui = ui;
    connect(MainWindow::instance(), SIGNAL(onSkinChanged()), this, SLOT(onSkinChanged()));
    setFocusPolicy(Qt::ClickFocus);

    setStyleSheet(SkinStyle::getCfgSettingStyleSheet());
}

BaseWindow::~BaseWindow()
{
}


void BaseWindow::initUi(QWidget *cfgWidget)
{
    _titleBar = new MyTitleBar();
    _titleBar->setButtonType(ONLY_CLOSE_BUTTON);

    connect(_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);
    layout->addWidget(_titleBar);
    cfgWidget->setObjectName("cfgContent");
    cfgWidget->setAttribute(Qt::WA_StyledBackground,true); //使BaseWindow的QSS对cfgWidget有效
    layout->addWidget(cfgWidget);
    this->setLayout(layout);
    adjustSize();
}


void BaseWindow::onButtonMinClicked()
{
	if (Qt::Tool == (windowFlags() & Qt::Tool))
	{
		hide();    //设置了Qt::Tool 如果调用showMinimized()则窗口就销毁了？？？
	}
	else
	{
		showMinimized();
	}
}

void BaseWindow::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize windowSize;
    _titleBar->getRestoreInfo(windowPos, windowSize);
	this->setGeometry(QRect(windowPos, windowSize));
}

void BaseWindow::onButtonMaxClicked()
{
    _titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(FactRect);
}

void BaseWindow::onButtonCloseClicked()
{
    close();
}

void BaseWindow::onSkinChanged()
{
    setStyleSheet(SkinStyle::getCfgSettingStyleSheet());
}


//一下鼠标消息处理函数必须保留，用以改变默认鼠标消息的下发
void BaseWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
}

void BaseWindow::mousePressEvent(QMouseEvent *event)
{
}

void BaseWindow::mouseMoveEvent(QMouseEvent *event)
{
}

void BaseWindow::mouseReleaseEvent(QMouseEvent *event)
{
}

void BaseWindow::paintEvent(QPaintEvent *event)
{
    //绘制边框
    QPainter painter(this);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, & opt, & painter, this);

//    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) return ;
//    painter.setPen(QColor(255,255,255));
//    if(hasFocus())
//        painter.setPen(Qt::red);
//    else
//        painter.setPen(Config::curSkinStyle()->LineColor);
//    QRectF rcBord(rect().left()+0.5, rect().top()+0.5,rect().right(),rect().bottom());
//    painter.drawRoundedRect(rcBord, 5, 5);
}

