#ifndef MONITOITEMFACTORY_H
#define MONITOITEMFACTORY_H

#include "monitoritem.h"

class MonitoItemFactory
{
public:
    static MonitorItem *CreateInstance(CfgType cfgtype, QString monitorName/*, QImage image*/,
                                       QPointF scenePos, qreal rectWidth, qreal rectHeight/*, QGraphicsItem *parent*/);

private:
    MonitoItemFactory();
};

#endif // MONITOITEMFACTORY_H
