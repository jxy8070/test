#include "resourcemanager.h"
#include "Frame/configscene.h"
#include "device/device_trn.h"
#include "configItem/configitem.h"

#include "Lua/Packages/centerctrl.h"

#define CfgPointer Qt::UserRole+1

ResourceManager::ResourceManager(AbsGraphScene *scene, QWidget *parent) :
    QDialog(parent)/*, _cfgScene(scene)*/
{
    _tableWidget = new QTableWidget(this);
    _tableWidget->sortItems(1);
    _tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _tableWidget->horizontalHeader()->setVisible(true);
    _tableWidget->verticalHeader()->setVisible(false);
    _tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    _action_refresh = new QAction(tr("Refresh"));
    _action_Del = new QAction(tr("Delete"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(5,5,5,5);
    mainLayout->addWidget(_tableWidget);
    this->setLayout(mainLayout);

    ////中控协议场景调用测试
//    QPushButton *get = new QPushButton("get");
//    QComboBox *save = new QComboBox;
//    CenterCtrl::instance()->setDevAddress(QString("192.168.1.204"));
//    for(int i = 1; i < 10; i++)
//        save->addItem(QString::number(i));
//    connect(get, &QPushButton::clicked, this, [=](){
//        quint8 num = 0;
//        bool rs = CenterCtrl::instance()->getCurrentSceneNum(num);
//        qDebug() << __FUNCTION__ << __LINE__ << "rs: " << rs << "num: " << num;
//    });
//    connect(save, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int idx){
//        bool rs = CenterCtrl::instance()->setCurrentSceneNum(++idx);
//        qDebug() << __FUNCTION__ << __LINE__ << "rs: " << rs << "idx: " << idx;
//    });
//    mainLayout->addWidget(get);
//    mainLayout->addWidget(save);

    resize(450, 600);
    refreshData(scene);
    refreshSceneCon(scene);

    //connect
    connect(_action_Del, SIGNAL(triggered()), this, SLOT(actionDeleteSlot()));
    connect(_action_refresh, SIGNAL(triggered()), this, SLOT(actionRefreshSlot()));
    _tableWidget->setStyleSheet(QTableWidgetSyle);
}

ResourceManager::~ResourceManager()
{
    delete  _tableWidget;
}

void ResourceManager::refreshData(AbsGraphScene *scene)
{
    ConfigScene *cfgscene = qobject_cast<ConfigScene *>(scene);
    if(!cfgscene) return ;
    setWindowTitle(tr("Resource Manager (%1)").arg(cfgscene->getTrnDev()->getDeviceName()));
    DevType devType = cfgscene->getTrnDev()->getDeviceType();
    _tableWidget->clear();
    _tableWidget->setSortingEnabled(false);

    _RMDataList.clear();
    float totalSpaseUsage = 0;
    float totalTimeUsage = 0;
    foreach(QGraphicsItem *item, scene->items()) {
        if(item->type() == DesignItem::Type) {
            DesignItem *desItem = qgraphicsitem_cast<DesignItem *>(item);
            ConfigItem *cfgItem = qobject_cast<ConfigItem *>(desItem);
            if(!cfgItem) continue;
            RMData_t data;
            data.s_moduleType = cfgItem->configTypeName();
            data.s_moduleName = cfgItem->customName();
            data.s_spceUsage  = Utils::getSpaceUsage(devType, cfgItem->getCfgType());
            data.s_timeUsage  = Utils::getTimeUsage(devType, cfgItem->getCfgType());
            data.s_pointer    = cfgItem;
            _RMDataList.append(data);
            totalSpaseUsage += data.s_spceUsage;
            totalTimeUsage  += data.s_timeUsage;
        }
    }
    QStringList fieldsList;
    fieldsList << "\n" + tr("Module type")
               << "\n" + tr("Module Name")
               << QString::number(totalSpaseUsage, 'f', 2) + "\n" + tr("Memery Usage")
               << QString::number(totalTimeUsage, 'f', 2) + "\n" + tr("CPU Usage");

    _tableWidget->setRowCount(5);
    _tableWidget->setColumnCount(fieldsList.length());
    _tableWidget->setHorizontalHeaderLabels(fieldsList);

    _tableWidget->setRowCount(_RMDataList.length());
    for(int row = 0; row < _RMDataList.length(); row++) {
        _tableWidget->setItem(row, 0, new TableWidgetItem(_RMDataList.at(row).s_moduleType));
        TableWidgetItem *nameWid = new TableWidgetItem(_RMDataList.at(row).s_moduleName);
        QVariant var = QVariant::fromValue((void *)_RMDataList.at(row).s_pointer);
        nameWid->setData(CfgPointer, var);//ModuleName列保存指针
        _tableWidget->setItem(row, 1, nameWid);
        _tableWidget->setItem(row, 2, new TableWidgetItem(QString("%1%").arg(QString::number(_RMDataList.at(row).s_spceUsage, 'f', 2))));
        _tableWidget->setItem(row, 3, new TableWidgetItem(QString("%1%").arg(QString::number(_RMDataList.at(row).s_timeUsage, 'f', 2))));
    }
    if(cfgscene->getTrnDev()->getPlatformType() != Platform_DSP)
        _tableWidget->setColumnHidden(2, true);


    _tableWidget->setSortingEnabled(true);
}

#include "Frame/mainwindow.h"
#include "Frame/absgraphview.h"
#include "Frame/configview.h"
#include <QMdiSubWindow>
void ResourceManager::actionDeleteSlot()
{
    QWidget *Curwid = MainWindow::instance()->getMdiArea()->currentSubWindow()->widget();
    QList<QGraphicsItem *> grapItemList;
    foreach(QTableWidgetItem *item, _tableWidget->selectedItems()) {
        if(item->column() == 1) {
            grapItemList.append((QGraphicsItem *)item->data(CfgPointer).value<void *>());
        }
    }
    AbsGraphView *aView = qobject_cast<AbsGraphView *>(Curwid);
    qobject_cast<ConfigView *>(aView)->scene()->deleteCfgItems(grapItemList);
}

void ResourceManager::actionRefreshSlot()
{
    QWidget *Curwid = MainWindow::instance()->getMdiArea()->currentSubWindow()->widget();
    AbsGraphView *aView = qobject_cast<AbsGraphView *>(Curwid);
    if(!aView) return;
    if(aView->getType() == VIEW_CONFIG) {
        refreshData(qobject_cast<ConfigView *>(aView)->scene());
    }
}

void ResourceManager::refreshSceneCon(AbsGraphScene *scene)
{
    disconnect(_sceneCon);
    _sceneCon = connect(scene, SIGNAL(designItemChanged(AbsGraphScene*)),
                        this, SLOT(refreshData(AbsGraphScene*)));
}

void ResourceManager::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu menu;
    menu.addAction(_action_refresh);
    if(!_tableWidget->selectedItems().isEmpty())
    {
        QWidget *wid = MainWindow::instance()->getMdiArea()->currentSubWindow()->widget();
        if(!wid) return ;
        if(!qobject_cast<ConfigView *>(wid)->scene()->isDebuging())
            menu.addAction(_action_Del);
//        foreach(QTableWidgetItem *item, _tableWidget->selectedItems())
//            qDebug() << __FUNCTION__ << __LINE__ << item->text() << item->row() << item->column();
    }

    menu.exec(mapToGlobal(e->pos()));
}
