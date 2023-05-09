#ifndef CTRLITEMGPI_H
#define CTRLITEMGPI_H
#include "configItem/configitem.h"

class CtrlSettingGpi;
class CtrlItemGpi : public ConfigItem
{
    Q_OBJECT
public:
    CtrlItemGpi(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 100);

    QWidget *getSettingForm() override;
    QByteArray getDetailData() override {return QByteArray();}
    void setDetailData(unsigned int *d) override {return;}
    void setRtnData(unsigned int *d) override {return;}
    void updateCfgSettingLoadedData() override {return;}
    DesignItem* clone(DesignItem* toItem) override {return NULL;}
    void loadParaFromSetting(QSettings *ConfigIni) override {return;}
    void saveParaToSetting(QSettings *ConfigIni) override {return;}

protected:
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
//    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    CtrlSettingGpi* _setting = nullptr;
    friend class ConfigScene;

};
#endif // CTRLITEMGPI_H
