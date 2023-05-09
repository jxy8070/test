#include "eqpacket.h"
#include "customWidget/eqgraph.h"
//////////////////////////////////////////////////////////////////
/// \brief EqData
//////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const EqData& data)
{
    stream << data.freq << data.gain << data.q;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, EqData& data)
{
    stream >> data.freq >> data.gain >> data.q;
    return stream;
}

void EqData::init(int idx)
{
    quint16 freq_t[4] = {0x65, 0x0426, 0x0EE3, 0x29F5};
    freq = freq_t[idx];
    gain = 0;
    q = 0x100;
}

EqData_Net EqData::getNetPack()
{
    EqData_Net netPack;
    netPack.freq = this->freq;
    netPack.gain = this->gain;
    netPack.q = this->q;
    return netPack;
}

EqData EqData_Net::getDataPack()
{
    EqData dataPack;
    dataPack.freq = this->freq;
    dataPack.gain = this->gain;
    dataPack.q = this->q;
    return dataPack;
}


//////////////////////////////////////////////////////////////////
/// \brief EqData1B
//////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const EqData1B& data)
{
    stream << data.bypass << data.eq;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, EqData1B& data)
{
    stream >> data.bypass >> data.eq;
    return stream;
}

EqData1B_Net EqData1B::getNetPack()
{
    EqData1B_Net netPack;
    netPack.bypass = this->bypass;
    netPack.eq = this->eq.getNetPack();
    return netPack;
}

EqData1B EqData1B_Net::getDataPack()
{
    EqData1B dataPack;
    dataPack.bypass = this->bypass;
    dataPack.eq = this->eq.getDataPack();
    return dataPack;
}


//////////////////////////////////////////////////////////////////
/// \brief EqData3B
//////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const EqData3B& data)
{
    stream << data.bypass;
    for(int i = 0; i< 3; i ++) stream << data.eq[i];
    return stream;
}

QDataStream& operator>>(QDataStream& stream, EqData3B& data)
{
    stream >> data.bypass;
    for(int i = 0; i< 3; i ++) stream >> data.eq[i];
    return stream;
}

EqData3B_Net EqData3B::getNetPack()
{
    EqData3B_Net netPack;
    netPack.bypass = this->bypass;
    for(int i = 0; i< 3; i ++){
        netPack.eq[i] = this->eq[i].getNetPack();
    }
    return netPack;
}

EqData3B EqData3B_Net::getDataPack()
{
    EqData3B dataPack;
    dataPack.bypass = this->bypass;
    for(int i = 0; i< 3; i ++){
        dataPack.eq[i] = this->eq[i].getDataPack();
    }
    return dataPack;
}

//////////////////////////////////////////////////////////////////
/// \brief EqData4B
//////////////////////////////////////////////////////////////////

QDataStream& operator<<(QDataStream& stream, const EqData4B& data)
{
    stream << data.bypass;
    for(int i = 0; i< 4; i ++) stream << data.eq[i];
    return stream;
}

QDataStream& operator>>(QDataStream& stream, EqData4B& data)
{
    stream >> data.bypass;
    for(int i = 0; i< 4; i ++) stream >> data.eq[i];
    return stream;
}

EqData4B_Net EqData4B::getNetPack()
{
    EqData4B_Net netPack;
    netPack.bypass = this->bypass;
    for(int i = 0; i< 4; i ++){
        netPack.eq[i] = this->eq[i].getNetPack();
    }
    return netPack;
}

EqData4B EqData4B_Net::getDataPack()
{
    EqData4B dataPack;
    dataPack.bypass = this->bypass;
    for(int i = 0; i< 4; i ++){
        dataPack.eq[i] = this->eq[i].getDataPack();
    }
    return dataPack;
}

//////////////////////////////////////////////////////////////////
/// \brief EqData15B
//////////////////////////////////////////////////////////////////

QDataStream& operator<<(QDataStream& stream, const EqData15B& data)
{
    stream << data.bypass;
    for(int i = 0; i< 15; i ++) stream << data.eq[i];
    return stream;
}

QDataStream& operator>>(QDataStream& stream, EqData15B& data)
{
    stream >> data.bypass;
    for(int i = 0; i< 15; i ++) stream >> data.eq[i];
    return stream;
}

EqData15B_Net EqData15B::getNetPack()
{
    EqData15B_Net netPack;
    netPack.bypass = this->bypass;
    for(int i = 0; i< 15; i ++){
        netPack.eq[i] = this->eq[i].getNetPack();
    }
    return netPack;
}

EqData15B EqData15B_Net::getDataPack()
{
    EqData15B dataPack;
    dataPack.bypass = this->bypass;
    for(int i = 0; i< 15; i ++){
        dataPack.eq[i] = this->eq[i].getDataPack();
    }
    return dataPack;
}

//////////////////////////////////////////////////////////////////
/// \brief EqData8B
//////////////////////////////////////////////////////////////////

QDataStream& operator<<(QDataStream& stream, const EqData8B& data)
{
    stream << data.bypass;
    for(int i = 0; i < 8; i ++) stream << data.eq[i];
    return stream;
}

QDataStream& operator>>(QDataStream& stream, EqData8B& data)
{
    stream >> data.bypass;
    for(int i = 0; i < 8; i ++) stream >> data.eq[i];
    return stream;
}

EqData8B_Net EqData8B::getNetPack()
{
    EqData8B_Net netPack;
    netPack.bypass = this->bypass;
    for(int i = 0; i< 8; i ++){
        netPack.eq[i] = this->eq[i].getNetPack();
    }
    return netPack;
}

EqData8B EqData8B_Net::getDataPack()
{
    EqData8B dataPack;
    dataPack.bypass = this->bypass;
    for(int i = 0; i< 8; i ++){
        dataPack.eq[i] = this->eq[i].getDataPack();
    }
    return dataPack;
}

//////////////////////////////////////////////////////////////////
/// \brief EqData5B
//////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
/// \brief EqData1B
//////////////////////////////////////////////////////////////////
