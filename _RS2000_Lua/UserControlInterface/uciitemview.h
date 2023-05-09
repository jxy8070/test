#ifndef UCIITEMVIEW_H
#define UCIITEMVIEW_H

#include <QGraphicsView>

class ConfigItemUCI;
class UciItemView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit UciItemView(QString devName, bool nodesSwitch, CfgType type, ConfigItemUCI *uciItem, QWidget *parent = nullptr);
    void changedScaling(qreal newScale);
    QGraphicsScene *getScene() { return _scene; }

public slots:
    void initData(QString ip);
protected:
    void wheelEvent(QWheelEvent *event) override;
signals:

private:
    QGraphicsScene *_scene;
    qreal _scale;
    CfgType _type;
};

#endif // UCIITEMVIEW_H
