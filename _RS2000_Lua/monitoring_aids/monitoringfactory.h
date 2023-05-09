#ifndef MONITORINGFACTORY_H
#define MONITORINGFACTORY_H

#include <QStringList>
#include <QString>
#include "common.h"
#include "monitorproperty.h"

class MonitoringFactory
{
public:
    static QStringList CreateInstance(DevType devType, /*DevType type*/CfgType type);
private:
    MonitoringFactory();
};

#endif // MONITORINGFACTORY_H
