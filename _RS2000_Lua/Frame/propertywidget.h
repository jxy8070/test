#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include "customWidget/overwriteitem.h"

class QtVariantPropertyManager;
class QtTreePropertyBrowser;
class QtEnumPropertyManager;
class QtVariantProperty;
class QtProperty;
class DesignItem;
class GraphicsItem;

class PropertyWidget : public DockWidget
{
    Q_OBJECT
public:
    PropertyWidget( QWidget* parent = 0);
    ~PropertyWidget();
public:
    void initTreeProperty(DesignItem* dtem);
private:
    void addProperty(const QMetaProperty& metaProp);

public slots:
    void valueChanged(QtProperty *property, const QVariant &value);
    void subTypeChanged(QtProperty *property, int value);

//protected:
//    void closeEvent(QCloseEvent *event) override {emit closeSignal(false);}
//signals:
//    void closeSignal(bool);

private:
    QtVariantPropertyManager* _variantManager;
    QtTreePropertyBrowser* _propertyEditor;
    QtEnumPropertyManager* _enumManager;
    QtProperty* _subTypeProp;
    QtVariantProperty* _ctrlPinProp;
    DesignItem* _designItem;
    QMetaObject::Connection _propChangedCon;
    QMetaObject::Connection _subTypeChangedCon;
    QList<QtProperty *> _propList;

    GraphicsItem *_customGrapItem;
};

#endif // PROPERTYWIDGET_H
