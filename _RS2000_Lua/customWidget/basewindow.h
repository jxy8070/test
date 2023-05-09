#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include "mytitlebar.h"

class ConfigScene;
class QGraphicsItem;
class BaseWindow : public QWidget
{
    Q_OBJECT

public:
    BaseWindow(QWidget *ui, QWidget *parent = NULL);
	~BaseWindow();
    void setTitle(QString str) { _titleBar->setTitleContent(str); }

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    void initUi(QWidget *ui);

private slots:
	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
    void onButtonCloseClicked();
    void onSkinChanged();


protected:
    MyTitleBar* _titleBar;
    QWidget *_ui;
};

#endif // BASEWINDOW_H
