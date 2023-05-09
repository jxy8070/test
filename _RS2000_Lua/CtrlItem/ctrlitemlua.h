#ifndef CTRLITEMLUA_H
#define CTRLITEMLUA_H

#include "configItem/configitem.h"

class CtrlItemLua : public ConfigItem
{
    Q_OBJECT
    Q_PROPERTY(int PinInCount READ gePinInCount WRITE setPinInCount)
    Q_PROPERTY(int PinOutCount READ gePinOutCount WRITE setPinOutCount)
    Q_PROPERTY(int RsInCount READ geRsPortInCount WRITE setRsPortInCount)
    Q_PROPERTY(int RsOutCount READ geRsPortOutCount WRITE setRsPortOutCount)
public:
    enum {MaxPinCount = 8, MaxRs232Count = 2};
    CtrlItemLua(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 100);
    ~CtrlItemLua();
    QWidget *getSettingForm() override {return NULL;}
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override {return;}
    void setRtnData(unsigned int *d) override {return;}
    void updateCfgSettingLoadedData() override {return;}
    DesignItem* clone(DesignItem* toItem) override {return NULL;}
    void loadParaFromSetting(QSettings *ConfigIni) override {return;}
    void saveParaToSetting(QSettings *ConfigIni) override {return;}

    inline int gePinInCount() {return _portInCount;}
    inline int gePinOutCount() {return _portOutCount;}
    inline int geRsPortInCount() {return _rsPortInCount;}
    inline int geRsPortOutCount() {return _rsPortOutCount;}

    inline CLuaPinParam_PC* getPinIn(int idx) {return _pinDefIn[idx];}
    inline CLuaPinParam_PC* getPinOut(int idx) {return _pinDefOut[idx];}

    void setPinInCount(int portInNum);
    void setPinOutCount(int portOutNum);
    void setRsPortInCount(int rsPortInCount);
    void setRsPortOutCount(int rsPortOutCount);
    void updateControlPins();
    inline QString luaScript(){ return _luaScript;}
    inline void setLuaScript(QString luaScript) {_luaScript = luaScript; }

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QString _luaScript;
    int _portInCount;
    int _portOutCount;
    int _rsPortInCount;
    int _rsPortOutCount;
    CLuaPinParam_PC* _pinDefIn[8];
    CLuaPinParam_PC* _pinDefOut[8];
    friend class ConfigScene;

};

#endif // CTRLITEMLUA_H
