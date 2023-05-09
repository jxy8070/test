#ifndef EQPACKET_H
#define EQPACKET_H

#include <QtGlobal>

struct EqData_Net;
struct EqData{
    friend QDataStream& operator<<(QDataStream& stream, const EqData& str);
    friend QDataStream& operator>>(QDataStream& stream, EqData& str);
    quint16 freq;
    qint16 gain;
    quint16 q;
    uint eqType;
    void init(int idx);
    EqData_Net getNetPack();
};

struct EqData_Net{
    quint16 freq;
    qint16 gain;
    quint16 q;
    EqData getDataPack();
};

struct EqData1B_Net;
struct EqData1B{
    EqData eq;
    bool bypass;
    EqData1B_Net getNetPack();
};

struct EqData1B_Net{
    EqData_Net eq;
    bool bypass;
    EqData1B getDataPack();
};

struct EqData3B_Net;
struct EqData3B{
    friend QDataStream& operator<<(QDataStream& stream, const EqData3B& str);
    friend QDataStream& operator>>(QDataStream& stream, EqData3B& str);
    EqData eq[3];
    bool bypass;
    EqData3B_Net getNetPack();
};

struct EqData3B_Net{
    EqData_Net eq[3];
    bool bypass;
    EqData3B getDataPack();
};

struct EqData4B_Net;
struct EqData4B{
    friend QDataStream& operator<<(QDataStream& stream, const EqData4B& str);
    friend QDataStream& operator>>(QDataStream& stream, EqData4B& str);
    EqData eq[4];
    bool bypass;
    EqData4B_Net getNetPack();
};


struct EqData4B_Net{
    EqData_Net eq[4];
    bool bypass;
    EqData4B getDataPack();
};

struct EqData15B_Net;
struct EqData15B{
    friend QDataStream& operator<<(QDataStream& stream, const EqData15B& str);
    friend QDataStream& operator>>(QDataStream& stream, EqData15B& str);
    EqData eq[15];
    bool bypass;
    EqData15B_Net getNetPack();
};


struct EqData15B_Net{
    EqData_Net eq[15];
    bool bypass;
    EqData15B getDataPack();
};

struct EqData8B_Net;
struct EqData8B {
    friend QDataStream& operator<<(QDataStream& stream, const EqData8B& str);
    friend QDataStream& operator>>(QDataStream& stream, EqData8B& str);
    EqData eq[8];
    bool bypass;
    EqData8B_Net getNetPack();
};

struct EqData8B_Net{
    EqData_Net eq[8];
    bool bypass;
    EqData8B getDataPack();
};

struct EqData5B_Net;
struct EqData5B{
    EqData eq[5];
    bool bypass;
    EqData5B_Net getNetPack();
};

struct EqData5B_Net{
    EqData_Net eq[5];
    bool bypass;
    EqData5B getDataPack();
};

struct EqData7B_Net;
struct EqData7B{
    EqData eq[5];
    bool bypass;
    EqData7B_Net getNetPack();
};

struct EqData7B_Net{
    EqData_Net eq[5];
    bool bypass;
    EqData7B getDataPack();
};
#endif // EQPACKET_H
