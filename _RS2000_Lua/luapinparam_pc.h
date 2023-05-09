#ifndef CLUAPINPARAM_PC_H
#define CLUAPINPARAM_PC_H

#include "Lua/Packages/luapinparam.h"
#include "configItem/configitem.h"
#include "ItemPort.h"

struct IG_PARACTL_LUAPIN;

class CLuaPinParam_PC : public CLuaPinParam
{
public:
    CLuaPinParam_PC(ConfigItem* parentItem, QString itemName, QString pinName, quint32 getCmdKey, quint32 setCmdKey);
    CLuaPinParam_PC(ConfigItem* parentItem, QString itemName, QString pinName, quint32 getCmdKey, quint32 setCmdKey,
                    quint8 exData1, quint8 exData2, QString groupName1 = QString(), QString groupName2 = QString());
    void initData(ConfigItem* parentItem, QString itemName, QString pinName, quint32 getCmdKey, quint32 setCmdKey,
                  quint8 exData1, quint8 exData2, QString groupName1, QString groupName2);
    QVariant getValue() override;
    void setValue(QVariant val) override;
    ConfigItem* getCfgItem();
    QString getTrnDevIp();
    void getLuaPinList(QList<IG_PARACTL_LUAPIN*>& luaPinList, quint32 parentId, quint32 id);

public:
    inline void setChildren(QList<CLuaPinParam_PC*> children) { _children = children;}
    inline quint32 GetCmdKey() {return _getCmdKey;}
    inline quint32 SetCmdKey() {return _setCmdKey;}
    inline quint8 GetExData1() {return _exData1;}
    inline quint8 GetExData2() {return _exData2;}
    inline void setName(QString itemName) {_itemName = itemName;}

public:
    ItemPort* pinPortIn;
    ItemPort* pinPortOut;
    static quint32 _LuaPinStartIndex;
private:
    QString _itemName;
    QString _pinName;
    QString _pinNameWithGroup;
    quint32 _getCmdKey;
    quint32 _setCmdKey;
    bool _bVisible;
    quint8 _exData1;
    quint8 _exData2;
    QString _groupName1;
    QString _groupName2;
    ConfigItem* _parentItem;

    QList<CLuaPinParam_PC*> _children;
    friend class ConfigItem;
};

#endif // CLUAPINPARAM_PC_H
