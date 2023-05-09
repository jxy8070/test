#include "mytitlebar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>
#include <QDebug>

#define BUTTON_HEIGHT 30		// 按钮高度;
#define BUTTON_WIDTH 30			// 按钮宽度;
#define TITLE_HEIGHT 30			// 标题栏高度;

MyTitleBar::MyTitleBar(QWidget *parent)
    : QWidget(parent)
    , m_isPressed(false)
    , m_buttonType(ONLY_CLOSE_BUTTON)
{
    // 初始化;
    initControl();
    initConnections();
}

MyTitleBar::~MyTitleBar()
{

}

//重写paintEven使自定义类支持QSS
void MyTitleBar::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, & opt, & painter, this);
}

// 初始化控件;
void MyTitleBar::initControl()
{
    setObjectName("customTitle");
    m_pIcon = new QLabel;
    m_pTitleContent = new QLabel;

    m_pButtonMin = new QPushButton;
    m_pButtonRestore = new QPushButton;
    m_pButtonMax = new QPushButton;
    m_pButtonClose = new QPushButton;

    m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

    m_pTitleContent->setObjectName("TitleContent");
    m_pButtonMin->setObjectName("ButtonMin");
    m_pButtonRestore->setObjectName("ButtonRestore");
    m_pButtonMax->setObjectName("ButtonMax");
    m_pButtonClose->setObjectName("ButtonClose");

    QHBoxLayout* mylayout = new QHBoxLayout(this);
    mylayout->addWidget(m_pIcon);
    mylayout->addWidget(m_pTitleContent);

    mylayout->addWidget(m_pButtonMin);
    mylayout->addWidget(m_pButtonRestore);
    mylayout->addWidget(m_pButtonMax);
    mylayout->addWidget(m_pButtonClose);

    mylayout->setContentsMargins(5, 0, 0, 0);
    mylayout->setSpacing(0);

    m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setFixedHeight(TITLE_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

// 信号槽的绑定;
void MyTitleBar::initConnections()
{
    connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
    connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
    connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
}



// 设置标题栏图标;
void MyTitleBar::setTitleIcon(QString filePath)
{
    QPixmap titleIcon(filePath);
    m_pIcon->setPixmap(titleIcon.scaled(25 , 25));
}

// 设置标题内容;
void MyTitleBar::setTitleContent(QString titleContent)
{
    QFont font("Microsoft YaHei", 10);
    font.setBold(true);
    m_pTitleContent->setText(titleContent);
    m_pTitleContent->setFont(font);
    m_titleContent = titleContent;
}

// 设置标题栏长度;
void MyTitleBar::setTitleWidth(int width)
{
    this->setFixedWidth(width);
}

// 设置标题栏上按钮类型;
// 由于不同窗口标题栏上的按钮都不一样，所以可以自定义标题栏中的按钮;
// 这里提供了四个按钮，分别为最小化、还原、最大化、关闭按钮，如果需要其他按钮可自行添加设置;
void MyTitleBar::setButtonType(ButtonType buttonType)
{
    m_buttonType = buttonType;

    switch (buttonType)
    {
    case MIN_BUTTON:
        {
            m_pButtonRestore->setVisible(false);
            m_pButtonMax->setVisible(false);
        }
        break;
    case MIN_MAX_BUTTON:
        {
            m_pButtonRestore->setVisible(false);
        }
        break;
    case ONLY_CLOSE_BUTTON:
        {
            m_pButtonMin->setVisible(false);
            m_pButtonRestore->setVisible(false);
            m_pButtonMax->setVisible(false);
        }
        break;
    default:
        break;
    }
}

// 设置标题栏中的标题是否会自动滚动，跑马灯的效果;
// 一般情况下标题栏中的标题内容是不滚动的，但是既然自定义就看自己需要嘛，想怎么设计就怎么搞O(∩_∩)O！
void MyTitleBar::setTitleRoll()
{
    connect(&m_titleRollTimer, SIGNAL(timeout()), this, SLOT(onRollTitle()));
    m_titleRollTimer.start(200);
}

// 保存窗口最大化前窗口的位置以及大小;
void MyTitleBar::saveRestoreInfo(const QPoint point, const QSize size)
{
    m_restorePos = point;
    m_restoreSize = size;
}

// 获取窗口最大化前窗口的位置以及大小;
void MyTitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
    point = m_restorePos;
    size = m_restoreSize;
}


// 双击响应事件，主要是实现双击标题栏进行最大化和最小化操作;
void MyTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
//    if (m_buttonType == MIN_MAX_BUTTON)
//    {
//        // 通过最大化按钮的状态判断当前窗口是处于最大化还是原始大小状态;
//        // 或者通过单独设置变量来表示当前窗口状态;
//        if (m_pButtonMax->isVisible())
//        {
//            onButtonMaxClicked();
//        }
//        else
//        {
//            onButtonRestoreClicked();
//        }
//    }
    emit signalButtonCloseClicked();
}

// 以下通过mousePressEvent、mouseMoveEvent、mouseReleaseEvent三个事件实现了鼠标拖动标题栏移动窗口的效果;
void MyTitleBar::mousePressEvent(QMouseEvent *event)
{
    if (m_buttonType == MIN_MAX_BUTTON)
    {
        // 在窗口最大化时禁止拖动窗口;
        if (m_pButtonMax->isVisible())
        {
            m_isPressed = true;
            m_startMovePos = event->globalPos();
        }
    }
    else
    {
        m_isPressed = true;
        m_startMovePos = event->globalPos();
    }

    return QWidget::mousePressEvent(event);
}
#include <QGraphicsProxyWidget>
#include "Frame/configview.h"
#include "Frame/configscene.h"
void MyTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed)
    {
        QPoint movePoint = event->globalPos() - m_startMovePos;
        QPoint widgetPos = this->parentWidget()->pos();
        m_startMovePos = event->globalPos();
        this->parentWidget()->setWindowOpacity(0.6);
        int x = widgetPos.x() + movePoint.x();
        int y = widgetPos.y() + movePoint.y();
        if (y < 0) y = 0;
        if (x < 0) x = 0;
//        QGraphicsProxyWidget *ProxyWidget = this->parentWidget()->graphicsProxyWidget();
//        if(ProxyWidget != 0) {
//            ConfigScene *scene = qobject_cast<ConfigScene *>(ProxyWidget->scene());
//        }
        this->parentWidget()->move(x, y);
    }
    return QWidget::mouseMoveEvent(event);
}

void MyTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    this->parentWidget()->setWindowOpacity(1.0);
    return QWidget::mouseReleaseEvent(event);
}

// 以下为按钮操作响应的槽;
void MyTitleBar::onButtonMinClicked()
{
    emit signalButtonMinClicked();
}

void MyTitleBar::onButtonRestoreClicked()
{
    m_pButtonRestore->setVisible(false);
    m_pButtonMax->setVisible(true);
    emit signalButtonRestoreClicked();
}

void MyTitleBar::onButtonMaxClicked()
{
    m_pButtonMax->setVisible(false);
    m_pButtonRestore->setVisible(true);
    emit signalButtonMaxClicked();
}

void MyTitleBar::onButtonCloseClicked()
{
    emit signalButtonCloseClicked();
}

// 该方法主要是让标题栏中的标题显示为滚动的效果;
void MyTitleBar::onRollTitle()
{
    static int nPos = 0;
    QString titleContent = m_titleContent;
    // 当截取的位置比字符串长时，从头开始;
    if (nPos > titleContent.length())
        nPos = 0;

    m_pTitleContent->setText(titleContent.mid(nPos));
    nPos++;
}
