#include "usercontrolinterfacescene.h"
#include "configItem/configitem.h"
#include "configItem/configitemmixer.h"
#include "configItem/configitemuci.h"
#include "configSetting/cfgsettingmixer.h"
#include "configItem/configiteminput.h"
#include "configItem/configitemoutput.h"
#include "absgraphview.h"
#include <QCheckBox>
#include "customWidget/overwriteitem.h"
#include "Frame/mainwindow.h"

UserControlInterfaceScene::UserControlInterfaceScene(QGraphicsItem *item, AbsGraphView *pView)
    : AbsGraphScene{pView}
{
    if(_view->getType() == VIEW_USERCONTROLINTERFACE) {
        _uciItem = qgraphicsitem_cast<ConfigItemUCI *>(item);
    }
    _mainTable = new QTableWidget;
    _mainTable->setStyleSheet(SkinStyle::getAppStyleSheet());
    this->addWidget(_mainTable);
    initData();
    connect(_mainTable, SIGNAL(cellChanged(int, int)), this, SLOT(tableWidgetCellChangedSlots(int,int)));
}

void UserControlInterfaceScene::saveAs(QString fileName)
{
    _uciItem->saveDataToUciTable(fileName);
    saveHander(fileName, _uciItem->getAES70DevMac());
//    qDebug() << __FUNCTION__ << __LINE__ << _uciItem->getAES70DevMac();
    QString info = tr("save to %1 succeed.").arg(QApplication::applicationDirPath()+fileName);
    Utils::MessageBoxInfo(tr("Information"), info);
}

void UserControlInterfaceScene::open(QString fileName, QString Mac)
{
    Q_UNUSED(Mac);
//    QList<QString> macList = _uciItem->openDatafromUciFile(fileName);
    QSettings *ConfigIni = new QSettings(fileName, QSettings::IniFormat, 0);
//    ConfigIni->setIniCodec(QTextCodec::codecForName("UTF-8"));
//    foreach(QString mac, macList) {
        ConfigIni->beginGroup(QString(UCI_GROUP).arg(_uciItem->getAES70DevMac()));
        _uciItem->loadParaFromSetting(ConfigIni);
        ConfigIni->endGroup();
//    }
    delete ConfigIni;
    _mainTable->disconnect();
    initData();
    connect(_mainTable, SIGNAL(cellChanged(int, int)), this, SLOT(tableWidgetCellChangedSlots(int,int)));
}

void UserControlInterfaceScene::initData()
{
    _mainTable->clear();

    //设置行列数
    int rowCount = _uciItem->getTabWidgetItemList().count();
    _mainTable->setColumnCount(9);
    _mainTable->setRowCount(rowCount+1);
    //设置表头
    _mainTable->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Module Type Name")));
    _mainTable->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Module Name")));
    _mainTable->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Channel")));
    _mainTable->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Uci Visible")));
    _mainTable->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("Uci Page")));
    _mainTable->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("Uci Position(1~16)")));
    _mainTable->setHorizontalHeaderItem(6, new QTableWidgetItem(tr("Maximum Gain(dB)")));
    _mainTable->setHorizontalHeaderItem(7, new QTableWidgetItem(tr("Minimum Gain(dB)")));
    _mainTable->setHorizontalHeaderItem(8, new QTableWidgetItem(tr("User Label")));
    //设置自定义委托
//    _mainTable->setItemDelegateForColumn(3, new CustomDelegate(CustomDelegate::ComboBox, QStringList() << tr("Yes") << tr("No")));
    _mainTable->setItemDelegateForColumn(4, new CustomDelegate(CustomDelegate::SpinBox, rowCount/16, 1));
    _mainTable->setItemDelegateForColumn(5, new CustomDelegate(CustomDelegate::SpinBox, 16, 1));
    _mainTable->setItemDelegateForColumn(6, new CustomDelegate(CustomDelegate::SpinBox, 12, -100));
    _mainTable->setItemDelegateForColumn(7, new CustomDelegate(CustomDelegate::SpinBox, 12, -100));
    //水平标题自适应以及显示竖列序号
    _mainTable->verticalHeader()->setVisible(true);
    _mainTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//根据标题自动调整水平标题的大小
    _mainTable->resize(width(), height());

    //添加数据
    //方图需求：第0行添加一个标题
    for(int col = 0; col < 9; col++) {
        if(col == 0) {
            _mainTable->setItem(0, col, new QTableWidgetItem(tr("WindowTitle")));
            _mainTable->item(0, col)->setFlags(_mainTable->item(0, col)->flags() & (~Qt::ItemIsEditable));
        }
        else if(col == 8) {
            _mainTable->setItem(0, col, new QTableWidgetItem(_uciItem->getSceneTitle()));
        }
        else {
            _mainTable->setItem(0, col, new QTableWidgetItem('/'));
            _mainTable->item(0, col)->setFlags(_mainTable->item(0, col)->flags() & (~Qt::ItemIsEditable));
        }
    }

    int row = 1;
    foreach(ConfigItemUCI::Uci_data_t *item, _uciItem->getTabWidgetItemList()) {
//        qDebug() << __FUNCTION__ << __LINE__ << item->s_modelType << item->s_name << item->s_channel;

        //module type  name  channel
        _mainTable->setItem(row, 0, new QTableWidgetItem(item->s_modelType));
        _mainTable->setItem(row, 1, new QTableWidgetItem(item->s_name));
        _mainTable->setItem(row, 2, new QTableWidgetItem(QString::number(item->s_channel+1)));
        _mainTable->item(row, 0)->setFlags(_mainTable->item(row, 0)->flags() & (~Qt::ItemIsEditable));
        _mainTable->item(row, 1)->setFlags(_mainTable->item(row, 1)->flags() & (~Qt::ItemIsEditable));
        _mainTable->item(row, 2)->setFlags(_mainTable->item(row, 2)->flags() & (~Qt::ItemIsEditable));
        //uci visible
//        _mainTable->setItem(row, 3, new QTableWidgetItem(tr("Yes")));
        QTableWidgetItem *visItem = new QTableWidgetItem;
        if(item->s_visible) visItem->setCheckState(Qt::Checked);
        else visItem->setCheckState(Qt::Unchecked);
        visItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        visItem->setFlags(visItem->flags() & (~Qt::ItemIsEditable));
        _mainTable->setItem(row, 3, visItem);

        //uci page
        _mainTable->setItem(row  , 4, new QTableWidgetItem(QString::number(item->s_page+1)));
        //uci position
        _mainTable->setItem(row  , 5, new QTableWidgetItem(QString::number(item->s_pos+1)));
        //max gain
        _mainTable->setItem(row  , 6, new QTableWidgetItem(QString::number(item->s_gainRange.maxValue)));
        //min gain
        _mainTable->setItem(row  , 7, new QTableWidgetItem(QString::number(item->s_gainRange.minValue)));
        //user labe
        _mainTable->setItem(row++, 8, new QTableWidgetItem(item->s_userLab));
    }
}

void UserControlInterfaceScene::tableWidgetCellChangedSlots(int row, int col)
{
    QString value = _mainTable->item(row, col)->text();
    switch (col) {
    case 3:
        if(_mainTable->item(row, col)->checkState() == Qt::Checked)
            _uciItem->getTabWidgetItemList()[row]->setVisible(1);
        else if(_mainTable->item(row, col)->checkState() == Qt::Unchecked)
            _uciItem->getTabWidgetItemList()[row]->setVisible(0);
        break;
    case 4:
        _uciItem->getTabWidgetItemList()[row]->setPage(value.toInt()-1);
        break;
    case 5:
        _uciItem->getTabWidgetItemList()[row]->setPos(value.toUInt()-1);
        break;
    case 6:
        _uciItem->getTabWidgetItemList()[row]->setGainRange(_mainTable->item(row, 7)->text().toInt(), value.toInt());
        break;
    case 7:
        _uciItem->getTabWidgetItemList()[row]->setGainRange(value.toInt(), _mainTable->item(row, 6)->text().toInt());
        break;
    case 8:
        if(row)
            _uciItem->getTabWidgetItemList()[row]->setLab(value);
        else
            _uciItem->setSceneTitle(value);
        break;
    default:
        break;
    }
}

void UserControlInterfaceScene::saveHander(QString fileName, QString Mac)
{
    QSettings *ConfigIni = new QSettings(fileName, QSettings::IniFormat, 0);
//    ConfigIni->setIniCodec(QTextCodec::codecForName("UTF-8"));
    ConfigIni->remove(QString(UCI_GROUP).arg(Mac));
    ConfigIni->beginGroup(QString(UCI_GROUP).arg(Mac));
    _uciItem->saveParaToSetting(ConfigIni);

    ConfigIni->endGroup();
    delete ConfigIni;
}



