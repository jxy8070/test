#ifndef CLUAPINPARAM_H
#define CLUAPINPARAM_H

struct IG_PARACTL_LUAPIN{
    IG_PARACTL_LUAPIN();
    IG_PARACTL_LUAPIN(const char* name, uint32 pId, uint32 id, uint32 exData1,
                       uint32 exData2, uint32 getKey, uint32 setKey);
    char itemName[8];
    __OPACK32_T2(
        OPACKU,parentId,16,				//父节点ID
        OPACKU,pinId,16				//
    );
    __OPACK32_T2(
        OPACKU,exData1,16,
        OPACKU,exData2,16
    );
    uint32 getCmdKey;
    uint32 setCmdKey;
};


//lua变量设置基类
class CLuaPinParam
{
public:
    CLuaPinParam();
    virtual QVariant getValue() = 0;
    virtual void setValue(QVariant val) = 0;
    static QVariant getValueImpl(QString devIp, QString itemName, quint32 getCmdKey, quint8 exData1, quint8 exData2);
    static void setValueImpl(QString devIp, QString itemName, quint32 setCmdKey, quint8 exData1, quint8 exData2, QVariant val);
};

#endif // CLUAPINPARAM_H
