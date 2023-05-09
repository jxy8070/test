#ifndef LINELABLE_H
#define LINELABLE_H
#include <QAbstractGraphicsShapeItem>
#include "ItemPort.h"
class ItemPort;
enum DevPortType;
class LineLabel : public QAbstractGraphicsShapeItem
{
public:
    LineLabel(ItemPort* port);
    enum { Type = UserType + 4, Lbl_Height = 12 };
    int type() const override;
    QRectF boundingRect() const override;
    void addToScene();
    void setName(QString text);
    bool isInput();
    DevPortType getPortType();
    int getUserZValue() { return _UserZValue; }
    void setUserZValue(int zValue) { _UserZValue = zValue; }

    int getMatchOutLabelNum() { return _matchOutLabelNum; };
    void setMatchOutLabelNum(int num) { _matchOutLabelNum = num; }
public:
    inline bool isMatched() {return scene() != NULL && _isMatched == true;}
    inline void setIsMatched(bool isMatched) {_isMatched = isMatched; }
    inline QString name() {return _name;}
    inline ItemPort* getLabelPort() {return _itemPort;}

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:    
    QPainterPath _painterPathS; //单独
    QPainterPath _painterPathM; //匹配
    QGraphicsItem* _parent;
    QFont _font;
    QString _name;
    ItemPort* _itemPort;
    bool _isMatched;

    int _UserZValue = 0;        //加载设计文件的分层索引
    int _matchOutLabelNum = 0;  //作为输出端口的标签时，记录在设计界面上有多少个输入端口的标签与之对应
};

#endif // LINELABLE_H
