#ifndef BORDERDOCK_H
#define BORDERDOCK_H

#include <QWidget>
#include <QToolBar>
#include <QDockWidget>
#include <QPaintEvent>

//hrp: 2023-03-08
Q_DECLARE_FLAGS(SubWnd_Types, SubWnd_Type)
Q_DECLARE_OPERATORS_FOR_FLAGS(SubWnd_Types)

class VerticalPushButton;
class DockWidget;
class BorderDock : public QToolBar
{
    Q_OBJECT

public:
    explicit BorderDock(Qt::ToolBarArea barArea, QMainWindow *parent = nullptr);
    void addDockBtn(DockWidget *docWgt, SubWnd_Types subWinTypes);
    void updateBorderDock(SubWnd_Type viewType);
private:

public slots:

private:
    QMultiMap<SubWnd_Types, DockWidget *> _dockWidgets; //需要键值相同
};

class VerticalPushButton : public QPushButton
{
public:
    VerticalPushButton(DockWidget *duckWid, Qt::ToolBarAreas barArea, QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEvent *ev) override;
    void leaveEvent(QEvent *ev) override;
//    void resizeEvent(QResizeEvent *event);

private:
    int getTextPoint();
    int getPos();
private:
    QLabel *_lab;
    QFont _font;
    bool _isInside;
    Qt::ToolBarAreas _barArea;
    DockWidget *_dockWid;
};

#endif // BORDERDOCK_H
