#ifndef CONFIGITEM_H
#define CONFIGITEM_H

#include "config.h"
#include "DesignItem.h"
#include "Frame/configscene.h"
#include "Protocal/packet.h"
#include "configItem/configitemfactory.h"
#include <QGraphicsProxyWidget>
#include "luapinparam_pc.h"

class AbsGraphView;
class ItemPort;
struct SigFlowDef16;

class ConfigItem : public DesignItem
{
    Q_OBJECT
    Q_PROPERTY(QString Name READ customName WRITE setCustomName)
    Q_PROPERTY(QString SubType READ subTypeIndex)
    Q_PROPERTY(QColor FillColor READ getFillColor WRITE setFillColor)
    Q_PROPERTY(QString CtrlPins READ getCtrlPins WRITE setCtrlPins)

public:
    enum { LAYER_IVALID = -1, PORT_TEXT_LENGTH = 4, PortDist = 12, PortWidth = 12, roundRadius = 4};
public:
    ~ConfigItem();
    DesignItem *clone(DesignItem *from) override;
    int checkZValue(void) override;
    virtual CfgType getCfgType();
    void setCustomName(QString name);
    int subTypeIndex();
    void addSubType(CfgType cfgType);
    CfgType subType(int subTypeIdx);
    QString getCtrlPins();
    void setCtrlPins(QString pinDefs);
    void setPinLineVisible(ItemPort* pin, bool isVisible);
    QString  getControlsInfo();
    static QWidget* getControlsInfoWidget(ConfigItem* cfgItem);
    void setLayer(int layer);
    void clearLayer();
    void setAllPortEnable();

    inline quint32 getModuleNum(void) { return _moduleNum; }
    inline void setModuleNum(quint32 no) { _moduleNum = no;}
    inline QString customName() {return _customName; }
    inline QString configTypeName() {return _cfgTypeName;}
    inline void setcfgTypeName(QString name) { _cfgTypeName = name; }
    inline QStringList subTypNames() { return _subTypeNameList;}
    inline int getLayer() { return _layer; }
    inline void setCpuIndex(int cpuIndex) { _cpuIndex = cpuIndex; }
    inline void setCpuGroupIdx(int groupIndex) { _cpuGroupIdx = groupIndex; }
    inline int cpuGroupIdx() { return _cpuGroupIdx; }
    inline void setUserZValue(int Z) {_UserZValue = Z;}
    inline int getUserZValue(void) { return _UserZValue; };

    bool isValid();
    void clearPortPara();
    void showSettingForm(QPointF mousePos);
    void setInputDef(int portNum, int col, int row);
    void getInputDef(int portNum, int *col, int *row);
    void setOutputDef(int portNum, int col, int row);
    void getOutputDef(int portNum, int *col, int *row);
    QList<ItemPort*> getInPortList()  { return _inPortList; }
    QList<ItemPort*> getOutPortList() { return _outPortList; }

    virtual QWidget *getSettingForm() =0;
    QByteArray getOverviewPackBuff(bool is16Bit);
    QByteArray getModuleNamePackBuff(bool is16Bit);
    QByteArray getDetailPackBuff(bool is16Bit);
    QByteArray getOnlineDetailPara(bool is16Bit);
    virtual QByteArray getOverviewData(bool is16Bit);
    virtual QByteArray getDetailData() =0;
    virtual QByteArray getOnlineData() {return QByteArray();}
    virtual void setDetailData(unsigned int *d) =0;
    virtual void setRtnData(unsigned int *d) =0;
    virtual void updateCfgSettingLoadedData() =0;
    virtual void loadParaFromSetting(QSettings *ConfigIni) = 0;
    virtual void saveParaToSetting(QSettings *ConfigIni) = 0;
    virtual QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val){return QVariant();} //wj2 待删除，改用UDP协议
    void clearBeforeDelete() override;

    int byte2int(QByteArray byte, int offset);
    quint16 byte2Uint16(QByteArray byte, int offset);
    quint32 byte2Uint32(QByteArray byte, int offset);
    bool byte2Bool(QByteArray byte, int offset);
    void saveParamToDevice(void);
    static quint32 getPacketModuleNum(quint32* buff, bool is16Bit, int& cnt32B);
    DevType getDevType();

protected:
    ConfigItem(Device_Trn* devTrn, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void initPort(int inNum, int outNum, QStringList iPortNames = QStringList(), QStringList oPortNames = QStringList());
    void initControlPins(QList<CLuaPinParam_PC*> pinList, bool visible = false);
    void updateControlPins();
private:
    void paint_Default(QPainter *painter);
    void paint_Kemu(QPainter *painter);
    void paint_FangTu(QPainter *painter);
    QPainterPath getPaintPath(int borderWidth, int dx = 0, int dy = 0);
    QLinearGradient getLinearGradient(QRectF rect, QColor fillColor); //线性渐变
    QByteArray getSigFlowHeadBuff(int bufSize, bool is16Bit);  //获取IG_SIGFLOW_MODULEHEAD，兼容8Bit及16Bit
    QByteArray getParaSetHeadBuff(int bufSize, bool is16Bit);  //获取IG_PARASET_MODULEHEAD，兼容8Bit及16Bit
public:
    static bool ShowSpaceTimePercent;

protected:
    quint32 _moduleNum;
    Device_Trn* _trnDev;
    QStringList _subTypeNameList;
    QList<CfgType> _subTypeList;
    int _layer;
    int _cpuIndex = -1;     //独立核ACE的Cpu序号
    int _cpuGroupIdx = -1;  //算法Cpu分组序号
    int _UserZValue;
    QString _customName;
    QString _cfgTypeName;
    int _rectHeight;
    int _rectWidth;
    int _pinsHeight;
    CfgType _cfgType;
    QList<ItemPort*> _inPortList;
    QList<ItemPort*> _outPortList;
    QList<SigFlowDef16*> _outputDefList;
    QList<SigFlowDef16*> _inputDefList;
    QList<ItemPort*> _pinInList;
    QList<ItemPort*> _pinOutList;
    QList<CLuaPinParam_PC*> _pinDefList;

    quint32 _ptrOffset;/* 组件起始地址 */
    int _subTypeIndex;
    QPointF _point;
    QGraphicsProxyWidget *_proxyItem;
};

typedef struct{
    __OPACK32_T2(
        OPACKU,moduleNo,16,
        OPACKU,moduleLen32b,16
    );
}IG_PARAGET_MODULEHEAD;
typedef struct{
    __OPACK32_T3(
        OPACKU,moduleNo,8,
        OPACKU,moduleNoU,8,
        OPACKU,blockLen32b,16
    );
}IG_PARASET_MODULEHEAD8B;
#define IG_PARASET_MODULEHEAD_FLAG 0XA503
typedef struct{
    __OPACK32_T2(
        OPACKU,flag,16,
        OPACKU,moduleNo,16
    );
    __OPACK32_T2(
        OPACKU,protocal,16,
        OPACKU,blockLen32b,16
    );
    OPACKU remain;
}IG_PARASET_MODULEHEAD16B;


typedef struct{
    __OPACK32_T3(
        OPACKU,genus,8,						//小类（属），例如不同种类的延时
        OPACKU,family,8,					//大类（科），例如延时，eq之类
        OPACKU,blockLen32b,16
    );
}IG_SIGFLOW_MODULEHEAD8B;
#define IG_SIGFLOW_MODULEHEAD_FLAG 0XA502
typedef struct{
    __OPACK32_T3(
        OPACKU,flag,16,
        OPACKU,genus,8,						//小类（属），例如不同种类的延时
        OPACKU,family,8						//大类（科），例如延时，eq之类
    );
    __OPACK32_T2(
        OPACKU,protocal,16,
        OPACKU,blockLen32b,16
    );
    __OPACK32_T2(
        OPACKU,auxiliaryInfoA,8,
        OPACKU,remain,24
    );
}IG_SIGFLOW_MODULEHEAD16B;

#endif // DEVICEITEM_H
