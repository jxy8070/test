#include "noticewidget.h"

NoticeWidget::NoticeWidget(QWidget *parent)
    : QLabel(parent)
    , mTimerCount(NOTICE_DEF_DELAY_CNT)
    , mTransparentVal(TRANSPARENT_MAX_VAL)
{
    setObjectName("noticeWidget");
    //文字居中
    setAlignment(Qt::AlignCenter);
    setWordWrap(true);
    //定时器，定时消失
    mTimerPtr = new QTimer(this);
    connect(mTimerPtr, SIGNAL(timeout()), this, SLOT(onTimeout()), Qt::QueuedConnection);
}

NoticeWidget::~NoticeWidget()
{
    if (mTimerPtr->isActive()) {
        mTimerPtr->stop();
    }
}

void NoticeWidget::onTimeout()
{
    --mTimerCount;
    bool v = isVisible();
    QString qss = styleSheet();
    if (0 < mTimerCount) {
        //最后1s开始渐变消失
        if (mTimerCount <= NOTICE_DEF_DELAY_CNT && 0 < mTransparentVal) {
            mTransparentVal -= TRANSPARENT_CUT_VAL;
            if (0 > mTransparentVal) {
                mTransparentVal = 0;
            }
            //控制透明度
            setCurStyleSheet(mTransparentVal);
        }
    } else {//显示结束
        mTimerPtr->stop();
        delete this;
    }
}

#include "skinstyle.h"
#include "config.h"
void NoticeWidget::setCurStyleSheet(int transparentVal)
{
    QString qss;
    if(Config::curSkinType() == Skin_FangTu){
        qss = "border:3px solid rgba(80, 128, 80, %1);color:rgba(255, 255, 255, %1);border-radius:6px;background-color:rgba(80, 80, 80, %1);";
    }
    else{
        qss = "border:3px solid rgba(80, 128, 80, %1);color:rgba(255, 255, 255, %1);border-radius:6px;background-color:rgba(80, 80, 80, %1);";
    }
    setStyleSheet(qss.arg(transparentVal));//设置样式，不透明
}


//显示消息，可通过设置delay_ms=0来立即关闭显示
void NoticeWidget::showNotice(const QString &msg, const int delay_ms)
{
    if (msg.isEmpty() || 0 >= delay_ms) { //消息为空直接返回
        return;
    }

    if (mTimerPtr->isActive()) {
        mTimerPtr->stop();
        setVisible(false);
    }

    QFont txtFont = SkinStyle::getFont(12);
    setFont(txtFont);
    QApplication::beep();
    QFontMetrics fontMetrics(txtFont);
    int txtWidth = fontMetrics.horizontalAdvance(msg);
    if(txtWidth < 300) txtWidth = 300;
    if(txtWidth > 500) txtWidth = 500;
    setFixedSize(txtWidth, 80);

    //设置显示内容
    setText(msg);

    QWidget* parent = parentWidget();//居中显示notice
    if (nullptr != parent) {
        move((parent->width() - width()) >> 1, (parent->height() - height()) >> 1);
    }

    setCurStyleSheet(TRANSPARENT_MAX_VAL);
    mTimerCount = delay_ms/TIMER_INTERVAL_MS + 1;//延时计数计算
    mTransparentVal = TRANSPARENT_MAX_VAL;
    setVisible(true);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    mTimerPtr->start(TIMER_INTERVAL_MS);//开始计数
}
