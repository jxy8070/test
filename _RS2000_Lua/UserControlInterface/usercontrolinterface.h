#ifndef USERCONTROLINTERFACE_H
#define USERCONTROLINTERFACE_H

#include <QMainWindow>
#include "configItem/configitemuci.h"
#include <QHash>
#include <QTreeWidget>

class TabWidget;
class ConfigItemUCI;
class UciItem_port;
class DeviceItem;
class AES70Device;
class DeviceStartusWindow;
class UserControlInterface : public QMainWindow
{
    Q_OBJECT
    enum {RoomDirPath = 1,};
public:
    void openProcData(QString fileName);
    void open(QString fileName, QString Mac, ConfigItemUCI *uciItem);
    bool devIsOnline();
protected:
    void resizeEvent(QResizeEvent *event);
signals:
    void loadDataSignal(QString ip);
private slots:
    void onDeviceStatusChangedSlots();
    void updateDeviceState(AES70Device *);
    void InOutLevelSlot(unsigned int *);
    void tabWidgetCurrentChangedSlot(int);
    void openActionTriggeredSlot();
    void removeLoadUciItem(ConfigItemUCI *cfgUci);
    void viewOnlineStatus();
    void onSidePushClickedSlot();
    void onTreeWidgetDoubleClickedSlot(QTreeWidgetItem* treeItem, int col);
    void afterOpeningTheFile();
private:
    void initTreeWidget(QString filePath);
    void initUi(void);

public:
    static UserControlInterface *instance();
private:
    UserControlInterface(QWidget *parent = nullptr);
    UserControlInterface(const UserControlInterface&);
    UserControlInterface& operator=(const UserControlInterface&);
    static QMutex Mutex;
    static UserControlInterface *_instance;

    TabWidget *_tabWidget;
    QList<UciItem_port *> _inAndOutList;
    QHash<QString, ConfigItemUCI *> _loadUCiItemList;
    QList<ProcessorData_t> _peripheralDeviceList;

    DeviceStartusWindow *_devStatusWin;

    QPushButton *_sidePush;
    QTreeWidget *_sideBar;
    QPropertyAnimation *_pptyAnimation_sBar;
    QPropertyAnimation *_pptyAnimation_sBtn;
    bool _sideFlag = false;
};

#include <QStylePainter>
class TabBar: public QTabBar {
public:
    QSize tabSizeHint(int index) const override{
        QSize s = QTabBar::tabSizeHint(index);
        s.transpose();
        return QSize(s.width(), s.width()+1);
    }
protected:
    void paintEvent(QPaintEvent *event) override{
        QStylePainter painter(this);
        QStyleOptionTab opt;

        for(int i = 0;i < count();i++)
        {
            initStyleOption(&opt,i);
            painter.drawControl(QStyle::CE_TabBarTabShape, opt);
            painter.save();

            QSize s = opt.rect.size();
            s.transpose();
            QRect r(QPoint(), s);
            r.moveCenter(opt.rect.center());
            opt.rect = r;

            QPoint c = tabRect(i).center();
            painter.translate(c);
            painter.rotate(90);
            painter.translate(-c);
            painter.drawControl(QStyle::CE_TabBarTabLabel,opt);
            painter.restore();
        }

    }
};

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *parent=0):QTabWidget(parent){
        TabBar *tabBar = new TabBar;
        tabBar->resize(100, 100);
        tabBar->setContextMenuPolicy(Qt::CustomContextMenu);
        QAction *closeAction = new QAction(tr("Close"));
        _menu = new QMenu(tabBar);
        _menu->addAction(closeAction);
        connect(tabBar, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onMenuExec(QPoint)));
        connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(onCloseActionSlot(bool)));
        tabBar->setStyleSheet("QTabBar { "
                              "     border: 1px solid transparent; "
                              "     background: transparent; "
                              "     color: transparent; "
                              "}"
                              "QTabBar::tab {\
                                  border: 1px solid rgb(69,156,99);\
                                  background: transparent;\
                                  height: 100px;\
                                  width: 100px;\
                                  color: rgb(69,156,99);\
                              }"
                              "QTabBar::tab::selected {"
                              "    border: 1px solid rgb(69,156,99);"
                              "    background: rgba(78,82,92,255);"
                              "    height: 101px;"
                              "    width: 101px;"
                              "    color: rgb(69,156,99);"
                              "}"
//                              "QTabBar::tab:hover { min-width: 120px; }"
//                              "QTabBar::tab:!selected { margin-top: 2px; }"
        );
        setTabBar(tabBar);
        setTabPosition(QTabWidget::West);
    }
signals:
    void deleteUciItem(ConfigItemUCI *);
public slots:
    void onMenuExec(QPoint pos) {
        TabBar *tabBar = static_cast<TabBar *>(sender());
        this->setProperty("currentIdx", tabBar->tabAt(pos));
        _menu->exec(mapToGlobal(pos));
    }
    void onCloseActionSlot(bool) {
        QWidget *wid = this->widget(this->property("currentIdx").toInt());
        ConfigItemUCI *uciItem = (ConfigItemUCI *)wid->property("uciItem").toULongLong();
        if(uciItem) {
            emit deleteUciItem(uciItem);
        }
        this->removeTab(this->property("currentIdx").toInt());
        delete wid;
    }
private:
    QMenu *_menu;
};


#define QTabWidgetSyle (\
"QTabBar { \
    border: 1px solid transparent; \
    background: transparent; \
    color: transparent; \
    padding-bottom: 0px;\
}\
QTabBar::tab {\
    border: 1px solid rgb(69,156,99);\
    border-top-left-radius: 4px;\
    border-top-right-radius: 4px;\
    background: transparent;\
    height: 20px;\
    min-width: 50px;\
    color: rgb(69,156,99);\
    padding-bottom: 1px;\
}\
QTabBar::tab::selected {\
    border: 1px solid rgb(69,156,99);\
    background: rgba(78,82,92,255);\
    height: 22px;\
    min-width: 50px;\
    color: rgb(69,156,99);\
    padding-bottom: 1px;\
}\
QTabBar::tab:hover { min-width: 22px; }\
QTabBar::tab:!selected { margin-top: 2px; }\
QTabWidget::pane {\
        padding-top: 0px;\
        border: 1px solid rgb(69,156,99);\
        background: transparent;\
        position: absolute;\
        top: 0em;\
}\
QHeaderView::section {\
   color: white;\
   background: rgba(78,82,92,255);\
}\
QTableView {\
    color: white;\
    gridline-color: rgb(105, 106, 110);\
    background: rgb(53, 57, 66);\
    selection-color: white;\
    selection-background-color: rgb(53, 53, 53);\
    border: 0px;\
    border-radius: 0px;\
}\
QSlider::groove:vertical {\
    background: red;\
    position: absolute;\
    width: 4px; border-radius: 2px;\
}\
QSlider::handle:vertical {\
    height: 30px;\
    background: qlineargradient(x1:0, y1:1, x2:1, y2:1, stop:0 #6F7573, stop:1 #CBCBCB);\
    margin: 0 -6px;\
    border-radius: 2px;\
}\
QSlider::add-page:vertical {\
    background: #6483DE;\
    width: 4px; border-radius: 2px;\
}\
QSlider::sub-page:vertical {\
    background: #E8EDF3;\
    width: 4px; border-radius: 2px;\
}")

#endif // USERCONTROLINTERFACE_H
