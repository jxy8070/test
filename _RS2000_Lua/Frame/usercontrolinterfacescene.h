#ifndef USERCONTROLINTERFACESCENE_H
#define USERCONTROLINTERFACESCENE_H

#include <QWidget>
#include "absgraphscene.h"
#include <QStandardItemModel>

class ConfigItem;
class ConfigItemUCI;
class UserControlInterfaceScene : public AbsGraphScene
{
    Q_OBJECT
public:
    explicit UserControlInterfaceScene(QGraphicsItem *item, AbsGraphView *pView);

    ConfigItemUCI *getUciItem() { return _uciItem; }
    void saveAs(QString fileName);
    void open(QString fileName, QString Mac);

    void initData();
signals:

public slots:
    void tableWidgetCellChangedSlots(int row, int col);
private:
    void saveHander(QString fileName, QString Mac);

private:
    ConfigItemUCI *_uciItem;

    QTableWidget *_mainTable;
};





#endif // USERCONTROLINTERFACESCENE_H
