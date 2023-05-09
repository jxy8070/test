#ifndef CLUAPINPARAM_DEV_H
#define CLUAPINPARAM_DEV_H

#include "luapinparam.h"


class CLuaPinParam_Dev : public CLuaPinParam
{
public:
    CLuaPinParam_Dev(IG_PARACTL_LUAPIN pinData);
    virtual ~CLuaPinParam_Dev();

    QVariant getValue() override;
    void setValue(QVariant val) override;
    inline QList<CLuaPinParam_Dev*> getChildren() {return _children;};
    void setChildren(QList<CLuaPinParam_Dev*> children);

private:
    IG_PARACTL_LUAPIN _pinData;
    QList<CLuaPinParam_Dev*> _children;
};



#endif // CLUAPINPARAM_DEV_H
