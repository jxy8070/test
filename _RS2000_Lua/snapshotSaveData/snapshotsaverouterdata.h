#ifndef SNAPSHOTSAVEROUTERDATA_H
#define SNAPSHOTSAVEROUTERDATA_H

#include "configItem/configitemsnapshot.h"
#include "snapshotSaveData/snapshotsaveobject.h"

class SnapshotSaveRouterData : public SnapshotSaveObject
{
public:
//    SnapshotSaveRouterData();

    void saveRouterData(ConfigItem *, int);
    void setRouterData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);


private:
    //router4
    typedef struct{
        struct{
            __OPACK32_T1S0(
                OPACKU,itoo,8
            );
        }routerch[4];
    }IG_PARACTL_VROUTER4;

    //router8
    typedef struct{
        struct{
            __OPACK32_T1S0(
                OPACKU,itoo,8
            );
        }routerch[8];
    }IG_PARACTL_VROUTER8;

    //router16
    typedef struct{
        struct{
            __OPACK32_T1S0(
                OPACKU,itoo,8
            );
        }routerch[16];
    }IG_PARACTL_VROUTER16;

private:

    QHash<QString, IG_PARACTL_VROUTER4> _router_4_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_VROUTER8> _router_8_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_VROUTER16> _router_16_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVEROUTERDATA_H
