#include "variantmanagerex.h"

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

class QtLuaPinPropertyType
{
};

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

Q_DECLARE_METATYPE(QtLuaPinPropertyType)

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif


VariantManagerEx::VariantManagerEx(QObject *parent)
    : QtVariantPropertyManager(parent)
{
    connect(this, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(slotValueChanged(QtProperty *, const QVariant &)));
    connect(this, SIGNAL(propertyDestroyed(QtProperty *)),
                this, SLOT(slotPropertyDestroyed(QtProperty *)));
}

VariantManagerEx::~VariantManagerEx()
{

}

int VariantManagerEx::luaPinTypeId()
{
    return qMetaTypeId<QtLuaPinPropertyType>();
}

void VariantManagerEx::slotValueChanged(QtProperty *property, const QVariant &value)
{
    if (subPinMap.contains(property)) {
        QtProperty *luaPins = subPinMap[property];
        if (propertyDataMap.contains(luaPins)){
            LuaPinData& pinData = propertyDataMap[luaPins];
            if(pinData.subPinNameMap.contains(property))
            {
                QString subPinName = pinData.subPinNameMap[property];
                bool visible = value.toBool();
                QString newValue = ";" + subPinName + (visible ? ":1" : ":0");
                QString oldValue = ";" + subPinName + (visible ? ":0" : ":1");
                pinData.pinDefs.replace(oldValue, newValue);
            }
            emit propertyChanged(luaPins);
            emit valueChanged(luaPins, pinData.pinDefs);
        }
    }
}

void VariantManagerEx::slotPropertyDestroyed(QtProperty *property)
{
    if (subPinMap.contains(property)) {
        subPinMap.remove(property);
    }
}

bool VariantManagerEx::isPropertyTypeSupported(int propertyType) const
{
    if (propertyType == luaPinTypeId())
        return true;
    return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}

int VariantManagerEx::valueType(int propertyType) const
{
    if (propertyType == luaPinTypeId()){
        return QVariant::String;
    }
    return QtVariantPropertyManager::valueType(propertyType);
}

QVariant VariantManagerEx::value(const QtProperty *property) const
{
    if (propertyDataMap.contains(property)){
        return propertyDataMap[property].pinDefs;
    }
    return QtVariantPropertyManager::value(property);
}

QString VariantManagerEx::valueText(const QtProperty *property) const
{
    if(propertyType(property) == luaPinTypeId()) {
        return QString();
    }
    return QtVariantPropertyManager::valueText(property);
}

void getPinDefInfo(const QString& pinDef, QString& group1, QString& group2, QString& pinName, bool& visible)
{
    QStringList strList = pinDef.split(":");
    group2 = QString();
    if(strList.count() == 2)
    {
        group1 = QString();
        pinName = strList[0].trimmed();
        visible = strList[1].trimmed() == "1" ? true : false;
    }
    else if(strList.count() == 3)
    {
        group1 = strList[0].trimmed();
        pinName = strList[1].trimmed();
        visible = strList[2].trimmed() == "1" ? true : false;
    }
    else if(strList.count() == 4)
    {
        group1 = strList[0].trimmed();
        group2 = strList[1].trimmed();
        pinName = strList[2].trimmed();
        visible = strList[3].trimmed() == "1" ? true : false;
    }
}

void VariantManagerEx::setValue(QtProperty *property, const QVariant &val)
{
    if (propertyType(property) == luaPinTypeId()) {
        if (val.type() != QVariant::String && !val.canConvert(QVariant::String))
            return;
        QString pinDefs = val.value<QString>();
        QStringList pinDefList = pinDefs.split(";", QString::SkipEmptyParts);
        QString group1, group2, pinName;
        bool visible;
        if (propertyDataMap.contains(property)) //更新pin
        {
           for(QString pinDef : pinDefList)
           {
               getPinDefInfo(pinDef, group1, group2, pinName, visible);
           }
           emit propertyChanged(property);
           emit valueChanged(property, val);
           return;
        }
        else {//创建pin及所属组
            LuaPinData pinsData;
            pinsData.pinDefs = pinDefs;
            for(QString pinDef : pinDefList)
            {
                getPinDefInfo(pinDef, group1, group2, pinName, visible);
                QtVariantProperty* pGroup1 = NULL;
                QtVariantProperty* pGroup = NULL;
                if(group1.length()> 1) //一级Group
                {
                    if(pinsData.groupMap.contains(group1)){
                        pGroup = (QtVariantProperty*)pinsData.groupMap[group1];
                    }
                    else{
                        pGroup = this->addProperty(groupTypeId(), group1);
                        property->addSubProperty(pGroup);
                        pinsData.groupMap[group1] = pGroup;
                    }
                    if(group2.length()> 1){ //二级Group
                        pGroup1 = pGroup;
                        QString groupKey = group1 + ":" + group2;
                        if(pinsData.groupMap.contains(groupKey)){
                            pGroup = (QtVariantProperty*)pinsData.groupMap[groupKey];
                        }
                        else{
                            pGroup = this->addProperty(groupTypeId(), group2);
                            pGroup1->addSubProperty(pGroup);
                            pinsData.groupMap[groupKey] = pGroup;
                        }
                    }
                }

                QtVariantProperty* pin = this->addProperty(QVariant::Bool,  pinName);
                if(pGroup != NULL){
                    pGroup->addSubProperty(pin);
                }
                else{
                    property->addSubProperty(pin);
                }
                pinDef.replace(":0", "").replace(":1", "");
                pinsData.subPinNameMap[pin] = pinDef;
                subPinMap[pin] = property;
                setValue(pin, visible);
            }
            propertyDataMap[property] = pinsData;
        }
    }
    QtVariantPropertyManager::setValue(property, val);
}

void VariantManagerEx::initializeProperty(QtProperty *property)
{
    if(propertyType(property) == luaPinTypeId())
    {
        ;
    }
    QtVariantPropertyManager::initializeProperty(property);
}

void VariantManagerEx::uninitializeProperty(QtProperty *property)
{
    if(propertyType(property) == luaPinTypeId() && propertyDataMap.contains(property)) {
        LuaPinData& pinData = propertyDataMap[property];
        pinData.groupMap.clear();
        pinData.subPinNameMap.clear();
        propertyDataMap.remove(property);
    }
    QtVariantPropertyManager::uninitializeProperty(property);
}
