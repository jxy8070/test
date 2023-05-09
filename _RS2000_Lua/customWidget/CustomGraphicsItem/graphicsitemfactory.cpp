#include "graphicsitemfactory.h"
#include "graphicsgroupboxitem.h"
#include "graphicspolylineitem.h"
#include "graphicstextitem.h"


GraphicsItem *GraphicsItemFactory::CreateGraphicsItem(GraphicsItem::GraphicsItemType type, QPainterPath path, QGraphicsItem *parent)
{
    GraphicsItem *grapItem = NULL;
    switch (type) {
    case GraphicsItem::GrapItemType_text:
        grapItem = new GraphicsTextItem(path, parent);
        break;
    case GraphicsItem::GrapItemType_groupBox:
        grapItem = new GraphicsGroupBoxItem(path, parent);
        break;
    case GraphicsItem::GrapItemType_polyLine:
        grapItem = new graphicsPolylineItem(path, parent);
        break;
    default:
        break;
    }
    return grapItem;
}
