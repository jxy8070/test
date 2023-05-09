#ifndef NOTICEWIDGET_H
#define NOTICEWIDGET_H

#include <QLabel>
#include <QTimer>

//定时器间隔，单位ms
#define TIMER_INTERVAL_MS   50

//默认提示时间1s
#define NOTICE_DEF_DELAY_CNT     (500/TIMER_INTERVAL_MS)

//透明度最大值255，也就是不透明
#define TRANSPARENT_MAX_VAL 255

//透明度递减值
#define TRANSPARENT_CUT_VAL (TRANSPARENT_MAX_VAL/NOTICE_DEF_DELAY_CNT + 1)


//间距调整
#define PADDING     10


class NoticeWidget :public QLabel
{
    Q_OBJECT
public:
    explicit NoticeWidget(QWidget *parent = 0);
    ~NoticeWidget();

    void showNotice(const QString &msg, const int delay_ms = 2000);

public slots:
    void onTimeout();

private:
    void setCurStyleSheet(int transparentVal);

private:
    QTimer  *mTimerPtr = NULL;
    int mTimerCount;
    int mTransparentVal;//透明度0~255，值越小越透明
};

#endif // NOTICEWIDGET_H
