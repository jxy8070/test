#ifndef MFRSCFG_H
#define MFRSCFG_H

#include  "skinstyle.h"


enum MfrsType{
    Mfrs_UNKNOWN = 0x0000,
    Mfrs_Fitcan = 0x0001,
    Mfrs_FangTu = 0x0002,
    Mfrs_KEMU = 0x0003,  //东方佳联Kemu
    Mfrs_GuangZhouShengTuo = 0x0004,
    Mfrs_Hushan = 0x0005,
    Mfrs_BeiJingHuiSheng = 0x0006,  //三所
    Mfrs_GuRui = 0x0007,
    Mfrs_YiChuang = 0x0008,
    Mfrs_BaiLianDa = 0x0009,        //百联达
    Mfrs_ControlMax = 0x0010,       //东方佳联ControlMax
    Mfrs_FTYX = 0x0011,             //富通亚讯

};

struct MfrsDev
{
    QString GroupNameEn;
    QString GroupNameChs;
    QString TypeNameEn;
    QString TypeNameChs;
    bool IsNet;
    bool HasLua;
    DevType Type;
};

class QTreeWidgetItem;
class MfrsCfg
{
public:
    void InitDeviceTree();
    bool isLuaDevice(DevType type);
    bool isValidDevice(DevType type);
    QString getDevTypeName(DevType type);
    QString getDevGroupName(DevType type);
    QString getCfgModelName(CfgType type);
    QColor getDeviceFillColor(DevType type);
public:
    bool CheckDevMfrs;                  //只管理该客户设备
    int DeviceGroupDis;                 //设备8分组的间隔
    QString TitleHeader;                //主窗口标题
    MfrsType CurMfrs;   //所属客户
    QList<SkinType> SkinTypeList;       //支持的Skin
    QList<MfrsDev> MfrsDevList;

public:
    static MfrsCfg _Fitcan;
    static MfrsCfg _FangTu;
    static MfrsCfg _DongFangJiaLian;
    static MfrsCfg _ControlMax;
    static MfrsCfg _GuangZhouShengTuo;
    static MfrsCfg _Hushan;
    static MfrsCfg _BeiJingHuiSheng;
    static MfrsCfg _GuRui;
    static MfrsCfg _YiChuang;
    static MfrsCfg _BaiLianDa;
    static MfrsCfg _FTYX;
};

#endif // MFRSCFG_H
