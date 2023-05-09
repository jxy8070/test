#include "stable.h"
#include "designcommands.h"
#include "absgraphscene.h"
#include "configItem/configitem.h"
#include "mainwindow.h"
#include "linelabel.h"
#include <QDebug>
#include "customWidget/toolkittreeitem.h"
#include "configItem/configitemsnapshot.h"
//////////////////////////////////////////////////
/// \brief ItemsAddCommand

ItemsAddCommand::ItemsAddCommand(AbsGraphScene *scene, QList<QGraphicsItem*> itemList) : QUndoCommand()
{
    _scene = scene;
    _itemList = scene->prepareItems(itemList);
}

void ItemsAddCommand::undo()
{
    _scene->deleteGraphicsItems(_itemList);
}

void ItemsAddCommand::redo()
{
    _scene->addGraphicsItems(_itemList);
}


//////////////////////////////////////////////////
/// \brief ItemsDeleteCommand

ItemsDeleteCommand::ItemsDeleteCommand(AbsGraphScene *scene, QList<QGraphicsItem*> itemList) : QUndoCommand()
{
    _scene = scene;
    _itemList = scene->prepareItems(itemList);
}

void ItemsDeleteCommand::undo()
{
    _scene->addGraphicsItems(_itemList);
}

void ItemsDeleteCommand::redo()
{
    _scene->deleteGraphicsItems(_itemList);
}


//////////////////////////////////////////////////
/// \brief ItemsMoveCommand

ItemsMoveCommand::ItemsMoveCommand(AbsGraphScene *scene, QList<QGraphicsItem*> itemList, QPointF moveDiff) : QUndoCommand()
{
    _scene = scene;
    foreach(QGraphicsItem* item, itemList)
    {
        if(item->type() == DesignItem::Type ||
                (item->type() ==  ItemPort::Type && item->parentItem() == NULL) )//转折点
        {
            _itemList.append(item);
            _diffList.append(QPointF(moveDiff));
        }
    }
    _scene->autoFitSceneSize();
    _skipFirstTime = true;
    _scene->setNeedSaveState(true);
}

ItemsMoveCommand::ItemsMoveCommand(AbsGraphScene *scene, QList<DesignItem*> itemList, QList<QPointF> diffList) : QUndoCommand()
{
    assert(itemList.count() == diffList.count());
    _scene = scene;
    for(int i = 0; i < itemList.count(); i++)
    {
        QGraphicsItem* item = itemList[i];
        _itemList.append(item);
        _diffList.append(diffList[i]);
    }
    _skipFirstTime = false;
}

void ItemsMoveCommand::undo()
{
    for(int i = 0; i<_itemList.count(); i++)
    {
        _itemList[i]->moveBy(-_diffList[i].x(), -_diffList[i].y());
    }
    _scene->autoFitSceneSize();
    _scene->setNeedSaveState(true);
}

void ItemsMoveCommand::redo()
{
    if(_skipFirstTime)
    {
        _skipFirstTime = false;
        return;
    }

    for(int i = 0; i<_itemList.count(); i++)
    {
        _itemList[i]->moveBy(_diffList[i].x(), _diffList[i].y());
    }

    _scene->autoFitSceneSize();
    _scene->setNeedSaveState(true);
}

//////////////////////////////////////////////////
/// \brief ItemPropChangedCommand

ItemPropChangedCommand::ItemPropChangedCommand(DesignItem* item, const char* propName, const QVariant& newVal, const QVariant& oldVal)
    : QUndoCommand()
{
    _designItem = item;
    _propName = QString::fromLocal8Bit(propName);
    _newValue = newVal;
    _oldValue = oldVal;
    _isFirstTime = true;
}

void ItemPropChangedCommand::undo()
{    
    _designItem->changePropertyValue(_propName.toStdString().c_str(), _oldValue);
    MainWindow::instance()->initTreeProperty(_designItem);
}

void ItemPropChangedCommand::redo()
{
    _designItem->changePropertyValue(_propName.toStdString().c_str(), _newValue);

    if(_isFirstTime) _isFirstTime = false;
    else  MainWindow::instance()->initTreeProperty(_designItem);
}


//////////////////////////////////////////////////
/// \brief SubTypeChangedCommand

SubTypeChangedCommand::SubTypeChangedCommand(ConfigScene* pScene, ConfigItem* item, CfgType newType, CfgType oldType)
    : QUndoCommand()
{
    _cfgScene = pScene;
    _cfgItem = item;
    _newType = newType;
    _oldType = oldType;
    _cfgItemNew = ConfigItemFactory::createConfigItem(pScene->getTrnDev(), _newType, _cfgItem->customName(), _cfgItem->scenePos());

    _removeCmd = new ItemsDeleteCommand(pScene, QList<QGraphicsItem*>{_cfgItem});
    _addCmd = new ItemsAddCommand(pScene,  QList<QGraphicsItem*>{_cfgItemNew});
}

SubTypeChangedCommand::~SubTypeChangedCommand()
{
    delete _removeCmd;
    delete _addCmd;
}

void SubTypeChangedCommand::undo()
{
    _addCmd->undo();
    _removeCmd->undo();
    _cfgItem->setSelected(true);
    _cfgItem->setFocus();
    MainWindow::instance()->initTreeProperty(_cfgItem);
}

void SubTypeChangedCommand::redo()
{
    _addCmd->redo();
    _removeCmd->redo();
    _cfgItemNew->setSelected(true);
    _cfgItemNew->setFocus();
    MainWindow::instance()->initTreeProperty(_cfgItemNew);
}


//////////////////////////////////////////////////
/// \brief LineLabelSetCommand

LineLabelSetCommand::LineLabelSetCommand(AbsGraphScene* scene, QList<LineLabel*> labelList, QString newName)
    : QUndoCommand()
{
    _pScene = scene;
    _labelList = labelList;
    QString perfix;
    for(LineLabel* label : _labelList)
    {
        label->addToScene();
        _oldNameList << label->name();
    }

//    scene->getNewLabelName(newNamex)//wj2
    int idx = getPerfixAndIndex(newName, perfix);
    if(_labelList.count() == 1){
        _newNameList << newName;
    }
    else {
        for(int i = idx;  i < idx + _labelList.count(); i++)
        {
            if(newName == perfix) _newNameList << QString("%1 %2").arg(perfix).arg(i);
            else _newNameList << QString("%1%2").arg(perfix).arg(i);
        }
    }
}

LineLabelSetCommand::~LineLabelSetCommand()
{
}

void LineLabelSetCommand::undo()
{
    changeLabelName(_oldNameList, _newNameList);
}

void LineLabelSetCommand::redo()
{
    changeLabelName(_newNameList, _oldNameList);
}

void LineLabelSetCommand::changeLabelName(QStringList newNameList, QStringList oldNameList)
{
    for(int i = 0; i< _labelList.count(); i++)
    {
        LineLabel* label = _labelList[i];
        if(newNameList[i]== oldNameList[i]) continue;
//        qDebug() << __FUNCTION__ << __LINE__ << label->name() << label->isInput() << label->isMatched()
//                 << newNameList[i] << oldNameList[i];
        label->setName(newNameList[i]);
    }
    _pScene->matchLineLabels();
}

int LineLabelSetCommand::getPerfixAndIndex(QString newName, QString& perfix)
{
    int index = 1;
    int i = newName.length()-1;
    for(; i>= 0; i-- )
    {
        if(newName[i] < '0' || newName[i] >'9')
        {
            break;
        }
    }
    perfix = newName.left(i + 1);
    if(i < newName.length()-1)
    {
        index = newName.right(newName.length()-1 -i).toInt();
    }

    return index;
}


ItemSnapBankAddCommand::ItemSnapBankAddCommand(AbsGraphScene *scene, QList<QGraphicsItem *> itemList)
    : QUndoCommand()
{
    _scene = scene;
    foreach(QGraphicsItem *item, itemList)
    {
        _itemList.append(item);
    }
}

void ItemSnapBankAddCommand::undo()
{
    qobject_cast<ConfigScene *>(_scene)->addSnapBankItems(_itemList, false);
}

void ItemSnapBankAddCommand::redo()
{
    qobject_cast<ConfigScene *>(_scene)->addSnapBankItems(_itemList, true);
}

ItemSnapBankDelCommand::ItemSnapBankDelCommand(AbsGraphScene *scene, QList<QGraphicsItem *> itemList)
    : QUndoCommand()
{
    _scene = scene;
    _sceneStatus = false;
    foreach(QGraphicsItem *item, itemList)
    {
        _itemList.append(item);
        if(qgraphicsitem_cast<DesignItem *>(item)->scene() == scene) {
            _sceneStatus = true;
        }
    }
}

void ItemSnapBankDelCommand::undo()
{
    qobject_cast<ConfigScene *>(_scene)->delSnapBankItems(_itemList, false, _sceneStatus);
}

void ItemSnapBankDelCommand::redo()
{
    qobject_cast<ConfigScene *>(_scene)->delSnapBankItems(_itemList, true);
}



ItemSnapControllerDelCommand::ItemSnapControllerDelCommand(AbsGraphScene *scene, QGraphicsItem *parentItem, QList<QGraphicsItem *> itemList)
: QUndoCommand(), _scene(scene), _parentItem(parentItem)
{
    _itemList.clear();
    _itemList.append(itemList);
}

void ItemSnapControllerDelCommand::undo()
{
    _scene->addSnapBankController(_parentItem, _itemList);
}

void ItemSnapControllerDelCommand::redo()
{
    _scene->delSnapBankController(_parentItem, _itemList);
}
