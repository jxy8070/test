#ifndef DEVICEITEMFACTORY_H
#define DEVICEITEMFACTORY_H

class DeviceItem;

class DeviceItemFactory
{
public:
    static DeviceItem* CreateInstance(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent = NULL);
private:
    DeviceItemFactory();
};

#endif // DEVICEITEMFACTORY_H
