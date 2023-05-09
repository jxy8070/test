#ifndef SNAPSHOTSAVEOBJECT_H
#define SNAPSHOTSAVEOBJECT_H

#include <QHash>
#include "configItem/configitem.h"



class SnapshotSaveObject : public QObject
{
public:
    SnapshotSaveObject();

    virtual void saveToSetting(QSettings *, int, CfgType) = 0;
    virtual void SettingToSnap(QSettings *, int, CfgType) = 0;

    template<typename t_value>
    bool findCfgItemData(QHash<QString, t_value> dataList, ConfigItem *cfgItem) {
        if(dataList.find(cfgItem->customName()) == dataList.end())
        {
            Utils::MessageBoxWarning(QObject::tr("Warning"), QString("%1 isn't save!").arg(cfgItem->customName()));
            return false;
        }
        return true;
    }

};

#endif // SNAPSHOTSAVEOBJECT_H
