#ifndef DESIGNCOMMANDS_H
#define DESIGNCOMMANDS_H

#include <QUndoCommand>

class AbsGraphScene;
class QGraphicsItem;
class DesignItem;
class ConfigItem;
class QTreeWidgetItem;

class ItemsAddCommand : public QUndoCommand
{
public:
    ItemsAddCommand(AbsGraphScene *scene, QList<QGraphicsItem*> itemList);
protected:
    void undo();
    void redo();

private:
    AbsGraphScene *_scene;
    QList<QGraphicsItem*> _itemList;
};


class ItemsDeleteCommand : public QUndoCommand
{
public:
    ItemsDeleteCommand(AbsGraphScene *scene, QList<QGraphicsItem*> itemList);
protected:
    void undo();
    void redo();

private:
    AbsGraphScene *_scene;
    QList<QGraphicsItem*> _itemList;
};


class ItemsMoveCommand : public QUndoCommand
{
public:
    ItemsMoveCommand(AbsGraphScene *scene, QList<QGraphicsItem*> itemList, QPointF moveDiff);
    ItemsMoveCommand(AbsGraphScene *scene, QList<DesignItem*> itemList, QList<QPointF> diffList);
protected:
    void undo();
    void redo();

private:
    AbsGraphScene *_scene;
    QList<QGraphicsItem*> _itemList;
    QList<QPointF> _diffList;
    bool _skipFirstTime;
};


class ItemPropChangedCommand : public QUndoCommand
{
public:
    ItemPropChangedCommand(DesignItem* item, const char* propName, const QVariant& newVal, const QVariant& oldVal);
protected:
    void undo();
    void redo();

private:
//    AbsGraphScene *_scene;
    DesignItem* _designItem;
    QString _propName;
    QVariant _newValue;
    QVariant _oldValue;
    bool _isFirstTime;
};

class ConfigScene;
class SubTypeChangedCommand : public QUndoCommand
{
public:
    SubTypeChangedCommand(ConfigScene* pScene, ConfigItem* item, CfgType newType, CfgType oldType);
    ~SubTypeChangedCommand();
protected:
    void undo();
    void redo();

private:
    QUndoCommand* _removeCmd;
    QUndoCommand* _addCmd;
    ConfigScene* _cfgScene;
    ConfigItem* _cfgItem;
    ConfigItem* _cfgItemNew;
    CfgType _newType;
    CfgType _oldType;
};

class LineLabel;
class LineLabelSetCommand : public QUndoCommand
{
public:
    LineLabelSetCommand(AbsGraphScene* scene, QList<LineLabel*> labelList, QString newName);
    ~LineLabelSetCommand();
protected:
    void undo();
    void redo();
private:
    void changeLabelName(QStringList newNameList, QStringList oldNameList);
    int getPerfixAndIndex(QString newName, QString& perfix);  //返回名称对应的前缀和序号 ch01 =》 perfix = ch and return 01

private:
    AbsGraphScene* _pScene;
    QList<LineLabel*> _labelList;
    QStringList _oldNameList;
    QStringList _newNameList;
};

class ItemSnapBankAddCommand : public QUndoCommand
{
public:
    ItemSnapBankAddCommand(AbsGraphScene *scene, QList<QGraphicsItem*> itemList);

protected:
    void undo();
    void redo();

private:
    AbsGraphScene *_scene;
    QList<QGraphicsItem*> _itemList;

};

class ItemSnapBankDelCommand : public QUndoCommand
{
public:
    ItemSnapBankDelCommand(AbsGraphScene *scene, QList<QGraphicsItem*> itemList);

protected:
    void undo();
    void redo();

private:
    AbsGraphScene *_scene;
    QList<QGraphicsItem*> _itemList;
    bool _sceneStatus;

};

/////////////////////hrp  待完善//////////////////////
class ItemSnapControllerDelCommand : public QUndoCommand
{
public:
    ItemSnapControllerDelCommand(AbsGraphScene *scene, QGraphicsItem *parentItem, QList<QGraphicsItem*> itemList);
protected:
    void undo();
    void redo();

private:
    AbsGraphScene *_scene;
    QGraphicsItem *_parentItem;
    QList<QGraphicsItem*> _itemList;
};

//class LinePointAddCommand : public QUndoCommand
//{
//public:
//    LinePointAddCommand(AbsGraphScene *scene, LineItem *line, ConfigItem *pointDevice);
//protected:
//    void undo();
//    void redo();

//private:
//    AbsGraphScene *_scene;
//    LineItem *_Line;
//    ConfigItem *_PointDevice;
//    QPointF _PointDevicePos;
//    CfgPortItem *_before;
//    CfgPortItem *_after;
//};

//class LinePointDeleteCommand : public QUndoCommand

#endif // DESIGNCOMMANDS_H
