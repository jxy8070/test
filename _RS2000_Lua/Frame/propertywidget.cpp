#include "propertywidget.h"

#include <QDate>
#include <QLocale>
#include <QDockWidget>
#include <QSizePolicy>
#include <QMetaProperty>
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "qtgroupboxpropertybrowser.h"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"
#include "DesignItem.h"
#include "configItem/configitem.h"
#include "absgraphscene.h"
#include "variantmanagerex.h"

PropertyWidget::PropertyWidget( QWidget * parent) :DockWidget(parent)
{
    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory(this);
    _variantManager = new VariantManagerEx();
    _propertyEditor = new QtTreePropertyBrowser(this); //QtGroupBoxPropertyBrowser
    _propertyEditor->setFactoryForManager(_variantManager, variantFactory);
    _propertyEditor->setAlternatingRowColors(false);
    this->setWidget(_propertyEditor);
    _designItem = NULL;
    _customGrapItem = NULL;
    _enumManager = new QtEnumPropertyManager(this);
     QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory(this);
    _propertyEditor->setFactoryForManager(_enumManager, comboBoxFactory);
}

PropertyWidget::~PropertyWidget()
{
    delete _variantManager;
}


void PropertyWidget::addProperty(const QMetaProperty&  metaProp)
{
    const char *propName = metaProp.name();
    QString pName(metaProp.name());
    if(strcmp(propName, "objectName") == 0 ) return;
    else if(strcmp(propName, "NodesSwitch") == 0 && Config::curSkinType() != Skin_FangTu) return;
    else if(strcmp(propName, "SubType") == 0)
    {
        ConfigItem* cfgItem = qobject_cast<ConfigItem*>(_designItem);
        QStringList subTypes = cfgItem->subTypNames();
        if(subTypes.count() > 0)
        {
            _subTypeProp = _enumManager->addProperty("SubType");
            _enumManager->setEnumNames(_subTypeProp, subTypes);
            _enumManager->setValue(_subTypeProp, cfgItem->subTypeIndex());
            _propertyEditor->addProperty(_subTypeProp);
        }
    }
    else if(strcmp(propName, "CtrlPins") == 0)
    {       
        ConfigItem* cfgItem = qobject_cast<ConfigItem*>(_designItem);
        if(cfgItem != NULL) {
            bool isLuaDev =  Config::curMfrsCfg().isLuaDevice(cfgItem->getDevType());
            QString pinSettings = cfgItem->getCtrlPins();
            if(isLuaDev && pinSettings.length() > 3){
                _ctrlPinProp = _variantManager->addProperty(VariantManagerEx::luaPinTypeId(), "CtrlPins");
                _ctrlPinProp->setValue(pinSettings);
                _propertyEditor->addProperty(_ctrlPinProp); //最耗时操作
                _propList.append(_ctrlPinProp);
            }
        }
    }
    else
    {
        QVariant value = _designItem->property(propName);
        QtVariantProperty* qtProp = _variantManager->addProperty(metaProp.userType(), propName);
        if(!metaProp.isWritable()) {
            qtProp->setAttribute(QLatin1String("readOnly"), true);
        }
        qtProp->setValue(value);
        if(strcmp(propName, "PinInCount") == 0 || strcmp(propName, "PinOutCount") == 0)
        {
            qtProp->setAttribute(QLatin1String("minimum"), 0);
            qtProp->setAttribute(QLatin1String("maximum"), 8);
        }
        _propList.append(qtProp);
        _propertyEditor->addProperty(qtProp);

        //端口可调范围提示
        int maxNum = 0;
        if(strcmp(propName, "InAPortCount" ) == 0) maxNum = _designItem->data(0).toInt();
        if(strcmp(propName, "InNPortCount" ) == 0) maxNum = _designItem->data(1).toInt();
        if(strcmp(propName, "InDPortCount" ) == 0) maxNum = _designItem->data(2).toInt();
        if(strcmp(propName, "InUPortCount" ) == 0) maxNum = _designItem->data(3).toInt();

        if(strcmp(propName, "OutAPortCount") == 0) maxNum = _designItem->data(4).toInt();
        if(strcmp(propName, "OutNPortCount") == 0) maxNum = _designItem->data(5).toInt();
        if(strcmp(propName, "OutDPortCount") == 0) maxNum = _designItem->data(6).toInt();
        if(strcmp(propName, "OutUPortCount") == 0) maxNum = _designItem->data(7).toInt();

        if(strstr(propName, "PortCount") != NULL) {
            qtProp->setToolTip(QString("min: 0, Max: %1").arg(maxNum));
            qtProp->setAttribute(QLatin1String("minimum"), 0);
            qtProp->setAttribute(QLatin1String("maximum"), maxNum);
        }

        //初始端口数量为0，删除该属性
        if((strcmp(propName, "InAPortCount" ) == 0 && !_designItem->data(0).toInt())
        || (strcmp(propName, "InNPortCount" ) == 0 && !_designItem->data(1).toInt())
        || (strcmp(propName, "InDPortCount" ) == 0 && !_designItem->data(2).toInt())
        || (strcmp(propName, "InUPortCount" ) == 0 && !_designItem->data(3).toInt())
        || (strcmp(propName, "OutAPortCount") == 0 && !_designItem->data(4).toInt())
        || (strcmp(propName, "OutNPortCount") == 0 && !_designItem->data(5).toInt())
        || (strcmp(propName, "OutDPortCount") == 0 && !_designItem->data(6).toInt())
        || (strcmp(propName, "OutUPortCount") == 0 && !_designItem->data(7).toInt())) {
            _propertyEditor->removeProperty(qtProp);
            _propList.removeAll(qtProp);
        }
    }

}

void PropertyWidget::initTreeProperty(DesignItem* item)
{
    for(QtProperty* qtProp : _propList)
    {
        delete qtProp;
    }
    _propList.clear();
    _propertyEditor->clear();
    _designItem = NULL;
    if(item != NULL)
    {
        disconnect(_propChangedCon);
        disconnect(_subTypeChangedCon);
        _designItem = item;
        const QMetaObject *metaobject = _designItem->metaObject();
        int count = metaobject->propertyCount();

        for (int i = 0; i < count; ++i) {
            QMetaProperty metaProp = metaobject->property(i);
            addProperty(metaProp);
        }
		//注册属性改变事件
        _propChangedCon = connect(_variantManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                    this, SLOT(valueChanged(QtProperty *, const QVariant &)));
        _subTypeChangedCon = connect(_enumManager, SIGNAL(valueChanged(QtProperty *, int)),
                    this, SLOT(subTypeChanged(QtProperty *, int)));

    }

}

void PropertyWidget::valueChanged(QtProperty *qtProp, const QVariant &newVal)
{
    QByteArray ba = qtProp->propertyName().toLatin1();
    const char *propName = ba.data();

    bool isPropChild = true;
    for(QtProperty* pProp : _propList)
    {
        if(pProp == qtProp)
        {
            isPropChild  = false;
            break;
        }
    }
    if(isPropChild) return;

    QVariant oldVal = _designItem->property(propName);
    AbsGraphScene* pScene = qobject_cast<AbsGraphScene*>(_designItem->scene());
    if(pScene != NULL)
    {
        pScene->itemPropChanged(_designItem, propName, newVal, oldVal);
    }
}

void PropertyWidget::subTypeChanged(QtProperty *qtProp, int val)
{
    QByteArray ba = qtProp->propertyName().toLatin1();
    const char *propName = ba.data();

    if(strcmp(propName, "SubType") == 0)
    {
        ConfigItem* cfgItem = qobject_cast<ConfigItem*>(_designItem);
        ConfigScene* cfgScene = qobject_cast<ConfigScene*>(_designItem->scene());
        if(cfgScene != NULL && cfgItem != NULL)
        {
            int oldTypeIdx = cfgItem->subTypeIndex();
            int newTypeIdx = _enumManager->value(_subTypeProp);
            cfgScene->subTypeChanged(cfgItem, newTypeIdx, oldTypeIdx);
        }
    }
}



