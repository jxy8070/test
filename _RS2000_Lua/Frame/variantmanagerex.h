#ifndef VARIANTMANAGEREX_H
#define VARIANTMANAGEREX_H

#include "qtvariantproperty.h"
#include <QtBoolPropertyManager>

class VariantManagerEx : public QtVariantPropertyManager
{
    Q_OBJECT
public:
    VariantManagerEx(QObject *parent = 0);
    ~VariantManagerEx();

    virtual QVariant value(const QtProperty *property) const;
    virtual int valueType(int propertyType) const;
    virtual bool isPropertyTypeSupported(int propertyType) const;
    static int luaPinTypeId();
    QString valueText(const QtProperty *property) const;

public slots:
    virtual void setValue(QtProperty *property, const QVariant &val);
protected:
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
private slots:
    void slotValueChanged(QtProperty *property, const QVariant &value);
    void slotPropertyDestroyed(QtProperty *property);
private:
    struct LuaPinData{
        QString pinDefs;
        QMap<QtProperty *, QString> subPinNameMap;
        QMap<QString, QtProperty *> groupMap;
    };

    QMap<const QtProperty *, LuaPinData> propertyDataMap;
    QMap<const QtProperty *, QtProperty *> subPinMap;
};

#endif // VARIANTMANAGEREX_H
