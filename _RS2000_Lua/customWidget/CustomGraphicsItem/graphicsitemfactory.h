#ifndef GRAPHICSITEMFACTORY_H
#define GRAPHICSITEMFACTORY_H
#include "graphicsitem.h"

class GraphicsItemFactory
{
public:
    static GraphicsItem *CreateGraphicsItem(GraphicsItem::GraphicsItemType type, QPainterPath path, QGraphicsItem *parent = nullptr);
};

#endif // GRAPHICSITEMFACTORY_H
