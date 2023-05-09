#include "luapinparam_pc.h"
#include "Lua/Packages/centerctrl.h"
#include "device/device_trn.h"

quint32 CLuaPinParam_PC::_LuaPinStartIndex = 20;


CLuaPinParam_PC::CLuaPinParam_PC(ConfigItem* parentItem, QString itemName, QString pinName, quint32 getCmdKey, quint32 setCmdKey)
{
    initData(parentItem, itemName, pinName, getCmdKey, setCmdKey, 0xFF, 0xFF, QString(), QString());
}

CLuaPinParam_PC::CLuaPinParam_PC(ConfigItem* parentItem, QString itemName, QString pinName, quint32 getCmdKey, quint32 setCmdKey,
                                 quint8 exData1, quint8 exData2, QString groupName1, QString groupName2)
{
    initData(parentItem, itemName, pinName, getCmdKey, setCmdKey, exData1, exData2, groupName1, groupName2);
}


void CLuaPinParam_PC::initData(ConfigItem* parentItem, QString itemName, QString pinName, quint32 getCmdKey, quint32 setCmdKey,
                               quint8 exData1, quint8 exData2, QString groupName1, QString groupName2)
{
    _itemName = itemName;
    _pinName = pinName;
    _bVisible = false;
    _getCmdKey = getCmdKey;
    _setCmdKey = setCmdKey;
    _exData1 = exData1;
    _exData2 = exData2;
    pinPortIn = NULL;
    pinPortOut = NULL;
    _groupName1 = groupName1;
    _groupName2 = groupName2;
    _parentItem = parentItem;

    _pinNameWithGroup = _pinName;
    if(!_groupName2.isEmpty()) {
        _pinNameWithGroup = _groupName2 + ":" + _pinNameWithGroup;
    }
    if(!_groupName1.isEmpty()) {
        _pinNameWithGroup = _groupName1 + ":" + _pinNameWithGroup;
    }
}

ConfigItem* CLuaPinParam_PC::getCfgItem()
{
    DesignItem* designItem = NULL;
    if(pinPortIn != NULL)
    {
        designItem = qgraphicsitem_cast<DesignItem*>(pinPortIn->parentItem());
    }
    if(designItem == NULL && pinPortOut != NULL)
    {
        designItem = qgraphicsitem_cast<DesignItem*>(pinPortOut->parentItem());
    }
    return qobject_cast<ConfigItem*>(designItem);
}


QString CLuaPinParam_PC::getTrnDevIp()
{
    ConfigScene* cfgScene = qobject_cast<ConfigScene*>(_parentItem->scene());
    if(cfgScene != NULL)
    {
        Device_Trn* dev = cfgScene->getTrnDev();
        QString devIP = dev->getDeviceIp();
        return devIP;
    }
    return QString();
}


void CLuaPinParam_PC::getLuaPinList(QList<IG_PARACTL_LUAPIN*>& luaPinList, quint32 parentId, quint32 id)
{
    QByteArray nameBa = _itemName.toUtf8().data();
    IG_PARACTL_LUAPIN* luaPin = new IG_PARACTL_LUAPIN(nameBa.data(), parentId, id,
                                                      _exData1, _exData2, _getCmdKey, _setCmdKey);

    luaPinList.append(luaPin);
    for(CLuaPinParam_PC* child :_children)
    {
        child->getLuaPinList(luaPinList, id, _LuaPinStartIndex++);
    }
}



QVariant CLuaPinParam_PC::getValue()
{
    QString itemName = _itemName;
    quint32 getCmdKey = _getCmdKey;
    quint8 exData1 = _exData1;
    quint8 exData2 = _exData2;

    if(_getCmdKey == 0xFFFFFFFF)
    {
        if(_children.count() == 0){
            return QVariant();
        }
        itemName = _children[0]->_itemName;
        getCmdKey = _children[0]->_getCmdKey;
        exData1 = _children[0]->_exData1;
        exData2 = _children[0]->_exData2;
    }

    QString devIp = getTrnDevIp();
    return getValueImpl(devIp, itemName, getCmdKey, exData1, exData2);
}


void CLuaPinParam_PC::setValue(QVariant val)
{
    QVariant newVal = val;
    ConfigItem* cfgItem =getCfgItem();
    if(cfgItem != NULL)
    {
        newVal = cfgItem->setLuaPinValue(this, val);
        if(!newVal.isNull()){
            cfgItem->updateCfgSettingLoadedData();
        }
        else {
            return;
        }
    }

    for(CLuaPinParam_PC* pinDef : _children)
    {
        pinDef->setValue(newVal);
    }
}

#if 0

#endif
