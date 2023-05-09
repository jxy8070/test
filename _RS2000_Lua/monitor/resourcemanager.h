#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QDialog>
#include "customWidget/overwriteitem.h"

class AbsGraphScene;
class ResourceManager : public QDialog
{
    Q_OBJECT
public:
    typedef struct RMData{
        QString s_moduleType;   //模块类型
        QString s_moduleName;   //模块名字
        float s_timeUsage;      //时间使用
        float s_spceUsage;      //空间使用
        QGraphicsItem *s_pointer;
    }RMData_t;
public:
    explicit ResourceManager(AbsGraphScene *scene, QWidget *parent = nullptr);
    ~ResourceManager();

    void refreshSceneCon(AbsGraphScene *scene);
protected:
    void contextMenuEvent(QContextMenuEvent *) override;
public slots:
    void refreshData(AbsGraphScene *scene);
    void actionDeleteSlot();
    void actionRefreshSlot();
private:
//    void initUi(void);
private:
    // 表格数据
    QList<RMData_t> _RMDataList;

    QTableWidget *_tableWidget;
    QAction *_action_refresh;
    QAction *_action_Del;

    QMetaObject::Connection _sceneCon;

};

#define QTableWidgetSyle (\
"QHeaderView::section {\
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
}\
QWidget {background: rgba(78,82,92,255);}")

//Q_DECLARE_METATYPE(ResourceManager::RMData_t)

#endif // RESOURCEMANAGER_H
