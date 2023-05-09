#include "packet.h"

QDataStream& operator<<(QDataStream& stream, const DynData& data)
{
    stream << data.atk ;
    stream << data.release;
    stream << data.nRatio;
    stream << data.ratio;
    stream << data.th1;
    stream << data.th2;
    stream << data.offset1;
    stream << data.offset2;
    stream << data.bypass;

    return stream;
}

QDataStream& operator>>(QDataStream& stream, DynData& data)
{
    stream >> data.atk ;
    stream >> data.release;
    stream >> data.nRatio;
    stream >> data.ratio;
    stream >> data.th1;
    stream >> data.th2;
    stream >> data.offset1;
    stream >> data.offset2;
    stream >> data.bypass;

    return stream;
}

DynData_Net DynData::getNetPack()
{
    DynData_Net netPack;

    netPack.atk     = this->atk;
    netPack.release = this->release;
    netPack.ratio   = this->ratio;
    netPack.nRatio  = this->nRatio;
    netPack.th1     = this->th1;
    netPack.th2     = this->th2;
    netPack.offset1 = this->offset1;
    netPack.offset2 = this->offset2;
    netPack.bypass  = this->bypass;

    return netPack;
}

DynData DynData_Net::getDataPack()
{
    DynData dataPack;

    dataPack.atk     = this->atk;
    dataPack.release = this->release;
    dataPack.ratio   = this->ratio;
    dataPack.nRatio  = this->nRatio;
    dataPack.th1     = this->th1;
    dataPack.th2     = this->th2;
    dataPack.offset1 = this->offset1;
    dataPack.offset2 = this->offset2;
    dataPack.bypass  = this->bypass;

    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Trn
///////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_Trn& data)
{
    for (int i = 0; i < 16; ++i)
    {
        stream << data.InMicGain[i]
           << data.InMic48V[i];
    }

    for (int i = 0; i < 32; ++i)
    {
        stream << data.NetworkMode[i];
    }
    return stream;
}
QDataStream& operator>>(QDataStream& stream, ConfigData_Trn& data)
{
    for (int i = 0; i < TRN_ANA_IN_NUM; ++i)
    {
        stream >> data.InMicGain[i]
           >> data.InMic48V[i];
    }

    for (int i = 0; i < 32; ++i)
    {
        stream >> data.NetworkMode[i];
    }

    return stream;
}

void ConfigData_Trn::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_Trn));
//    //初始化ConfigData_Trn
//    for (int i = 0; i < TRN_ANA_IN_NUM; ++i)
//    {
//        InMicGain[i] = 0;
//    }
}

ConfigData_Trn_Net ConfigData_Trn::getNetPack()
{
    ConfigData_Trn_Net netPack;

    netPack.CurrentSceneNum = this->CurrentSceneNum;

    for (int i = 0; i < TRN_ANA_IN_NUM; ++i)
    {
        netPack.InMicGain[i] = this->InMicGain[i];
        netPack.InMic48V[i] = this->InMic48V[i];
    }

    for (int i = 0; i < 32; ++i)
    {
        netPack.NetworkMode[i] = this->NetworkMode[i];
    }
    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Net_Trn
///////////////////////////////////////////////////////////////////////
ConfigData_Trn ConfigData_Trn_Net::getDataPack()
{
    ConfigData_Trn dataPack;

    dataPack.CurrentSceneNum = this->CurrentSceneNum;

    for (int i = 0; i < TRN_ANA_IN_NUM; ++i)
    {
        dataPack.InMicGain[i] = this->InMicGain[i];
        dataPack.InMic48V[i] = this->InMic48V[i];
    }

    for (int i = 0; i < 32; ++i)
    {
        dataPack.NetworkMode[i] = this->NetworkMode[i];
    }
    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief CommonData_Spk
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const CommonData_Spk& data)
{
    stream << data.power <<data.ch <<data.value.volume << data.value.phase
           << data.value.mute << data.value.gain << data.value.delay << data.value.channel;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, CommonData_Spk& data)
{
    stream >> data.power >> data.ch >> data.value.volume >> data.value.phase
           >> data.value.mute >> data.value.gain >> data.value.delay >> data.value.channel;
    return stream;
}

///////////////////////////////////////////////////////////////////////
/// \brief Divider
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const Divider& data)
{
    stream << data.hf.q << data.hf.freq << data.hf.bypass << data.hf.gain
           << data.lf.q << data.lf.freq << data.lf.bypass << data.lf.gain;
    return stream;
}
QDataStream& operator>>(QDataStream& stream, Divider& data)
{
    stream >> data.hf.q >> data.hf.freq >> data.hf.bypass >> data.hf.gain
           >> data.lf.q >> data.lf.freq >> data.lf.bypass >> data.lf.gain;
    return stream;
}

///////////////////////////////////////////////////////////////////////
/// \brief ControlValue
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ControlValue& data)
{
    stream << data.channel
           << data.delay
           << data.gain
           << data.mute
           << data.phase
           << data.volume;
    return stream;
}
QDataStream& operator>>(QDataStream& stream, ControlValue& data)
{
    stream >> data.channel
           >> data.delay
           >> data.gain
           >> data.mute
           >> data.phase
           >> data.volume;
    return stream;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Spk
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_Spk& data)
{
    //暂时不序列化  data.controlValue
    stream << data.ch1_ControlValue
           << data.ch1_Eq
           << data.ch1_Dyn
           << data.ch1_divider;
//    stream << data.Switch  << data.ch1_Eq << data.ch1_Dyn
//           << data.ch1_divider << data.volumeCurrentType;
    return stream;

}
QDataStream& operator>>(QDataStream& stream, ConfigData_Spk& data)
{
    //暂时不序列化  data.controlValue
    stream >> data.ch1_ControlValue
           >> data.ch1_Eq
           >> data.ch1_Dyn
           >> data.ch1_divider;

//    stream >> data.Switch  >> data.ch1_Eq >> data.ch1_Dyn
//           >> data.ch1_divider >> data.volumeCurrentType;
    return stream;
}

void ConfigData_Spk::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_Spk));

    Switch = 1;

    ch1_ControlValue.channel = 1;

    ch1_divider.hf.freq   = 0x07D0;
    ch1_divider.hf.bypass = 0x01;
    ch1_divider.lf.freq   = 0x07D0;
    ch1_divider.lf.bypass = 0x01;

    ch1_Dyn.atk     = 50;
    ch1_Dyn.release = 500;
    ch1_Dyn.nRatio  = 0x0100;
    ch1_Dyn.th1     = (qint16)0xCE00;
    ch1_Dyn.th2     = (qint16)0xEC00;

    for(int i = 0; i < 4; i++)
    {
        ch1_Eq.eq[i].init(i);
    }
}

ConfigData_Spk_Net ConfigData_Spk::getNetPack()
{
    ConfigData_Spk_Net netPack;

    netPack.Switch            = this->Switch;
    netPack.volumeCurrentType = this->volumeCurrentType;

    netPack.ch1_ControlValue  = this->ch1_ControlValue;
    netPack.ch1_Eq            = this->ch1_Eq.getNetPack();
    netPack.ch1_divider       = this->ch1_divider;
    netPack.ch1_Dyn           = this->ch1_Dyn;

    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Spk_Net
///////////////////////////////////////////////////////////////////////
ConfigData_Spk ConfigData_Spk_Net::getDataPack()
{
    ConfigData_Spk dataPack;

    dataPack.Switch            = this->Switch;
    dataPack.volumeCurrentType = this->volumeCurrentType;

    dataPack.ch1_ControlValue  = this->ch1_ControlValue;
    dataPack.ch1_Eq            = this->ch1_Eq.getDataPack();
    dataPack.ch1_divider       = this->ch1_divider;
    dataPack.ch1_Dyn           = this->ch1_Dyn;

    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_STM32_POE_0_1
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_STM32_POE_0_1& data)
{
    stream << data.AD_gain
           << data.AD_Eq
           << data.AD_Dyn;

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_STM32_POE_0_1& data)
{
    stream >> data.AD_gain
           >> data.AD_Eq
           >> data.AD_Dyn;

    return stream;
}

void ConfigData_STM32_POE_0_1::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_STM32_POE_0_1));

    for(int i = 0; i < 4; i++)
    {
        this->AD_Eq.eq[i].init(i);
    }

    this->AD_Dyn.atk     = 50;
    this->AD_Dyn.release = 500;
    this->AD_Dyn.nRatio  = 0x0100;
    this->AD_Dyn.th1     = (qint16)0xCE00;
    this->AD_Dyn.th2     = (qint16)0xEC00;
    this->AD_Dyn.atk     = 50;
    this->AD_Dyn.release = 500;
    this->AD_Dyn.nRatio  = 0x0100;
    this->AD_Dyn.th1     = (qint16)0xCE00;
    this->AD_Dyn.th2     = (qint16)0xEC00;
}

ConfigData_STM32_POE_0_1_Net ConfigData_STM32_POE_0_1::getNetPack()
{
    ConfigData_STM32_POE_0_1_Net netPack;

    netPack.AD_gain = this->AD_gain;
    netPack.AD_Eq   = this->AD_Eq.getNetPack();
    netPack.AD_Dyn  = this->AD_Dyn;

    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_STM32_POE_0_1_Net
///////////////////////////////////////////////////////////////////////
ConfigData_STM32_POE_0_1 ConfigData_STM32_POE_0_1_Net::getDataPack()
{
    ConfigData_STM32_POE_0_1 dataPack;

    dataPack.AD_gain = this->AD_gain;
    dataPack.AD_Eq   = this->AD_Eq.getDataPack();
    dataPack.AD_Dyn  = this->AD_Dyn;

    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_STM32_POE_0_2
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_STM32_POE_0_2& data)
{
    stream << data.AD_gain[0]
           << data.AD_gain[1]
           << data.AD_Eq[0]
           << data.AD_Eq[1]
           << data.AD_Dyn;

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_STM32_POE_0_2& data)
{
    stream >> data.AD_gain[0]
           >> data.AD_gain[1]
           >> data.AD_Eq[0]
           >> data.AD_Eq[1]
           >> data.AD_Dyn;

    return stream;
}

void ConfigData_STM32_POE_0_2::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_STM32_POE_0_2));

    for(int i = 0; i < 4; i++)
    {
        this->AD_Eq[0].eq[i].init(i);
        this->AD_Eq[1].eq[i].init(i);
    }

    this->AD_Dyn.atk     = 50;
    this->AD_Dyn.release = 500;
    this->AD_Dyn.nRatio  = 0x0100;
    this->AD_Dyn.th1     = (qint16)0xCE00;
    this->AD_Dyn.th2     = (qint16)0xEC00;
    this->AD_Dyn.atk     = 50;
    this->AD_Dyn.release = 500;
    this->AD_Dyn.nRatio  = 0x0100;
    this->AD_Dyn.th1     = (qint16)0xCE00;
    this->AD_Dyn.th2     = (qint16)0xEC00;
}

ConfigData_STM32_POE_0_2_Net ConfigData_STM32_POE_0_2::getNetPack()
{
    ConfigData_STM32_POE_0_2_Net netPack;

    netPack.AD_gain[0] = this->AD_gain[0];
    netPack.AD_gain[1] = this->AD_gain[1];
    netPack.AD_Eq[0]   = this->AD_Eq[0].getNetPack();
    netPack.AD_Eq[1]   = this->AD_Eq[1].getNetPack();
    netPack.AD_Dyn  = this->AD_Dyn;

    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_STM32_POE_0_2_Net
///////////////////////////////////////////////////////////////////////
ConfigData_STM32_POE_0_2 ConfigData_STM32_POE_0_2_Net::getDataPack()
{
    ConfigData_STM32_POE_0_2 dataPack;

    dataPack.AD_gain[0] = this->AD_gain[0];
    dataPack.AD_gain[1] = this->AD_gain[1];
    dataPack.AD_Eq[0]   = this->AD_Eq[0].getDataPack();
    dataPack.AD_Eq[1]   = this->AD_Eq[1].getDataPack();
    dataPack.AD_Dyn  = this->AD_Dyn;

    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_STM32_POE_1_0
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_STM32_POE_1_0& data)
{
    stream << data.phantom
           << data.Mic_Gain
           << data.x
           << data.Dyn
           << data.Eq;

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_STM32_POE_1_0& data)
{
    stream >> data.phantom
           >> data.Mic_Gain
           >> data.x
           >> data.Dyn
           >> data.Eq;

    return stream;
}

void ConfigData_STM32_POE_1_0::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_STM32_POE_1_0));
}

ConfigData_STM32_POE_1_0_Net ConfigData_STM32_POE_1_0::getNetPack()
{
    ConfigData_STM32_POE_1_0_Net netPack;

    netPack.phantom  = this->phantom;
    netPack.Mic_Gain = this->Mic_Gain;
    netPack.x = this->x;
    netPack.Dyn = this->Dyn;
    netPack.Eq = this->Eq.getNetPack();

    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_STM32_POE_1_0_Net
///////////////////////////////////////////////////////////////////////
ConfigData_STM32_POE_1_0 ConfigData_STM32_POE_1_0_Net::getDataPack()
{
    ConfigData_STM32_POE_1_0 dataPack;

    dataPack.phantom = this->phantom;
    dataPack.Mic_Gain = this->Mic_Gain;
    dataPack.x = this->x;
    dataPack.Dyn = this->Dyn;
    dataPack.Eq = this->Eq.getDataPack();

    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Amp 4*4
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_Amp4_4& data)
{
    for (int i = 0; i < 8; ++i)
    {
        stream << data.channel_gain[i];
    }

    for (int i = 0; i < 4; ++i)
    {
        stream << data.channel_eq[i]
               << data.Aout_xover[i]
               << data.Aout_eq[i]
               << data.Aout_gain[i]
               << data.Aout_dyn[i]
               << data.Aout_delay[i];
    }

    stream << data.Aout_mixer
           << data.Nout_route;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_Amp4_4& data)
{
    for (int i = 0; i < 8; ++i)
    {
        stream >> data.channel_gain[i];
    }

    for (int i = 0; i < 4; ++i)
    {
        stream >> data.channel_eq[i]
               >> data.Aout_xover[i]
               >> data.Aout_eq[i]
               >> data.Aout_gain[i]
               >> data.Aout_dyn[i]
               >> data.Aout_delay[i];
    }

    stream >> data.Aout_mixer
           >> data.Nout_route;

    return stream;
}

void ConfigData_Amp4_4::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_Amp4_4));

    for (int i = 0; i < 4; ++i)
    {
        this->Aout_dyn[i].atk     = 50;
        this->Aout_dyn[i].release = 500;
        this->Aout_dyn[i].nRatio  = 0x0100;
        this->Aout_dyn[i].th1     = (qint16)0xCE00;
        this->Aout_dyn[i].th2     = (qint16)0xEC00;

        for (int j = 0; j < 4; ++j)
        {
            this->channel_eq[i].eq[j].init(j);
            this->Aout_eq[i].eq[j].init(j);
        }
    }
}

ConfigData_Amp4_4_Net ConfigData_Amp4_4::getNetPack()
{
    ConfigData_Amp4_4_Net netPack;

    for (int i = 0; i < 8; ++i)
    {
        netPack.channel_gain[i] = this->channel_gain[i];
    }

    for (int i = 0; i < 4; ++i)
    {
        netPack.channel_eq[i] = this->channel_eq[i].getNetPack();
        netPack.Aout_xover[i] = this->Aout_xover[i];
        netPack.Aout_eq[i]    = this->Aout_eq[i].getNetPack();
        netPack.Aout_gain[i]  = this->Aout_gain[i];
        netPack.Aout_dyn[i]   = this->Aout_dyn[i];
        netPack.Aout_delay[i] = this->Aout_delay[i];
    }

    netPack.Aout_mixer = this->Aout_mixer;
    netPack.Nout_route = this->Nout_route;

    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Amp4_4_Net
///////////////////////////////////////////////////////////////////////
ConfigData_Amp4_4 ConfigData_Amp4_4_Net::getDataPack()
{
    ConfigData_Amp4_4 dataPack;

    for (int i = 0; i < 8; ++i)
    {
        dataPack.channel_gain[i] = this->channel_gain[i];
    }

    for (int i = 0; i < 4; ++i)
    {
        dataPack.channel_eq[i] = this->channel_eq[i].getDataPack();
        dataPack.Aout_xover[i] = this->Aout_xover[i];
        dataPack.Aout_eq[i]    = this->Aout_eq[i].getDataPack();
        dataPack.Aout_gain[i]  = this->Aout_gain[i];
        dataPack.Aout_dyn[i]   = this->Aout_dyn[i];
        dataPack.Aout_delay[i] = this->Aout_delay[i];
    }

    dataPack.Aout_mixer = this->Aout_mixer;
    dataPack.Nout_route = this->Nout_route;
    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData RT1052 POE 60W 2*4
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Poe_60W_2_4& data)
{
    stream << data.channel_dyn;

    for (int i = 0; i < 3; ++i)
    {
        stream << data.channel_gain[i]
               << data.channel_eq[i];
    }

    for (int i = 0; i < 2; ++i)
    {
        stream << data.Aout_xover[i]
               << data.Aout_eq[i]
               << data.Aout_gain[i]
               << data.Aout_dyn[i]
               << data.Aout_delay[i];
    }

    stream << data.Aout_mixer
           << data.Nout_xover;
		   
#ifdef Logo_BeiJingHuiSheng
	stream << data.mixer_mode;
#endif
    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Poe_60W_2_4& data)
{
    stream >> data.channel_dyn;

    for (int i = 0; i < 3; ++i)
    {
        stream >> data.channel_gain[i]
               >> data.channel_eq[i];
    }

    for (int i = 0; i < 2; ++i)
    {
        stream >> data.Aout_xover[i]
               >> data.Aout_eq[i]
               >> data.Aout_gain[i]
               >> data.Aout_dyn[i]
               >> data.Aout_delay[i];
    }

    stream >> data.Aout_mixer
           >> data.Nout_xover;
		   
#ifdef Logo_BeiJingHuiSheng
	stream >> data.mixer_mode;
#endif

    return stream;
}

void ConfigData_RT1052_Poe_60W_2_4::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_RT1052_Poe_60W_2_4));

    this->channel_dyn.atk     = 50;
    this->channel_dyn.release = 500;
    this->channel_dyn.nRatio  = 0x0100;
    this->channel_dyn.th1     = (qint16)0xCE00;
    this->channel_dyn.th2     = (qint16)0xEC00;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            this->channel_eq[i].eq[j].init(j);
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        this->Aout_dyn[i].atk     = 50;
        this->Aout_dyn[i].release = 500;
        this->Aout_dyn[i].nRatio  = 0x0100;
        this->Aout_dyn[i].th1     = (qint16)0xCE00;
        this->Aout_dyn[i].th2     = (qint16)0xEC00;

        for (int j = 0; j < 15; ++j)
        {
            this->Aout_eq[i].eq[j].init(j);
        }
    }
}

ConfigData_RT1052_Poe_60W_2_4_Net ConfigData_RT1052_Poe_60W_2_4::getNetPack()
{
    ConfigData_RT1052_Poe_60W_2_4_Net netPack;

    netPack.channel_dyn = this->channel_dyn;

    for (int i = 0; i < 3; ++i)
    {
        netPack.channel_gain[i] = this->channel_gain[i];
        netPack.channel_eq[i] = this->channel_eq[i].getNetPack();
    }

    for (int i = 0; i < 2; ++i)
    {
        netPack.Aout_xover[i] = this->Aout_xover[i];
        netPack.Aout_eq[i]    = this->Aout_eq[i].getNetPack();
        netPack.Aout_gain[i]  = this->Aout_gain[i];
        netPack.Aout_dyn[i]   = this->Aout_dyn[i];
        netPack.Aout_delay[i] = this->Aout_delay[i];
    }

    netPack.Aout_mixer = this->Aout_mixer;
    netPack.Nout_xover = this->Nout_xover;
#ifdef Logo_BeiJingHuiSheng
    netPack.mixer_mode = this->mixer_mode;
#endif
    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_RT1052_Poe_60W_2_4_Net
///////////////////////////////////////////////////////////////////////
ConfigData_RT1052_Poe_60W_2_4 ConfigData_RT1052_Poe_60W_2_4_Net::getDataPack()
{
    ConfigData_RT1052_Poe_60W_2_4 dataPack;

    dataPack.channel_dyn = this->channel_dyn;

    for (int i = 0; i < 3; ++i)
    {
        dataPack.channel_gain[i] = this->channel_gain[i];
        dataPack.channel_eq[i] = this->channel_eq[i].getDataPack();
    }

    for (int i = 0; i < 2; ++i)
    {
        dataPack.Aout_xover[i] = this->Aout_xover[i];
        dataPack.Aout_eq[i]    = this->Aout_eq[i].getDataPack();
        dataPack.Aout_gain[i]  = this->Aout_gain[i];
        dataPack.Aout_dyn[i]   = this->Aout_dyn[i];
        dataPack.Aout_delay[i] = this->Aout_delay[i];
    }

    dataPack.Aout_mixer = this->Aout_mixer;
    dataPack.Nout_xover = this->Nout_xover;
#ifdef Logo_BeiJingHuiSheng
    dataPack.mixer_mode = this->mixer_mode;
#endif
    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData RT1052 POE AP60 Bluetooth
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Poe_AP60_Bluetooth& data)
{
    for (int i = 0; i < 3; ++i)
    {
        stream << data.channel_gain[i]
               << data.channel_eq[i];
    }

    for (int i = 0; i < 2; ++i)
    {
        stream << data.Aout_xover[i]
               << data.Aout_eq[i]
               << data.Aout_gain[i]
               << data.Aout_dyn[i]
               << data.Aout_delay[i];
    }

    stream << data.Nout_xover;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Poe_AP60_Bluetooth& data)
{
    for (int i = 0; i < 3; ++i)
    {
        stream >> data.channel_gain[i]
               >> data.channel_eq[i];
    }

    for (int i = 0; i < 2; ++i)
    {
        stream >> data.Aout_xover[i]
               >> data.Aout_eq[i]
               >> data.Aout_gain[i]
               >> data.Aout_dyn[i]
               >> data.Aout_delay[i];
    }

    stream >> data.Nout_xover;

    return stream;
}

void ConfigData_RT1052_Poe_AP60_Bluetooth::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_RT1052_Poe_AP60_Bluetooth));

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            this->channel_eq[i].eq[j].init(j);
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        this->Aout_dyn[i].atk     = 50;
        this->Aout_dyn[i].release = 500;
        this->Aout_dyn[i].nRatio  = 0x0100;
        this->Aout_dyn[i].th1     = (qint16)0xCE00;
        this->Aout_dyn[i].th2     = (qint16)0xEC00;

        for (int j = 0; j < 15; ++j)
        {
            this->Aout_eq[i].eq[j].init(j);
        }
    }
}

ConfigData_RT1052_Poe_AP60_Bluetooth_Net ConfigData_RT1052_Poe_AP60_Bluetooth::getNetPack()
{
    ConfigData_RT1052_Poe_AP60_Bluetooth_Net netPack;

    for (int i = 0; i < 3; ++i)
    {
        netPack.channel_gain[i] = this->channel_gain[i];
        netPack.channel_eq[i] = this->channel_eq[i].getNetPack();
    }

    for (int i = 0; i < 2; ++i)
    {
        netPack.Aout_xover[i] = this->Aout_xover[i];
        netPack.Aout_eq[i]    = this->Aout_eq[i].getNetPack();
        netPack.Aout_gain[i]  = this->Aout_gain[i];
        netPack.Aout_dyn[i]   = this->Aout_dyn[i];
        netPack.Aout_delay[i] = this->Aout_delay[i];
    }

    netPack.Nout_xover = this->Nout_xover;

    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_RT1052_Poe_AP60_Bluetooth_Net
///////////////////////////////////////////////////////////////////////
ConfigData_RT1052_Poe_AP60_Bluetooth ConfigData_RT1052_Poe_AP60_Bluetooth_Net::getDataPack()
{
    ConfigData_RT1052_Poe_AP60_Bluetooth dataPack;

    for (int i = 0; i < 3; ++i)
    {
        dataPack.channel_gain[i] = this->channel_gain[i];
        dataPack.channel_eq[i] = this->channel_eq[i].getDataPack();
    }

    for (int i = 0; i < 2; ++i)
    {
        dataPack.Aout_xover[i] = this->Aout_xover[i];
        dataPack.Aout_eq[i]    = this->Aout_eq[i].getDataPack();
        dataPack.Aout_gain[i]  = this->Aout_gain[i];
        dataPack.Aout_dyn[i]   = this->Aout_dyn[i];
        dataPack.Aout_delay[i] = this->Aout_delay[i];
    }

    dataPack.Nout_xover = this->Nout_xover;
    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData RT1052 AP400
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_AP400& data)
{
    for (int i = 0; i < 3; ++i)
    {
        stream << data.channel_gain[i]
               << data.channel_eq[i];
    }

    for (int i = 0; i < 8; ++i)
    {
        stream << data.Aout_xover[i]
               << data.Aout_eq[i]
               << data.Aout_gain[i]
               << data.Aout_dyn[i]
               << data.Aout_delay[i];
    }

    stream << data.route
           << data.Nout_xover
           << data.directivity;
#ifdef Logo_BeiJingHuiSheng
	stream << data.route_mode
           << data.threshold
           << data.default_route
           << data.auto_recovery;
#endif
    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_AP400& data)
{
    for (int i = 0; i < 3; ++i)
    {
        stream >> data.channel_gain[i]
               >> data.channel_eq[i];
    }

    for (int i = 0; i < 8; ++i)
    {
        stream >> data.Aout_xover[i]
               >> data.Aout_eq[i]
               >> data.Aout_gain[i]
               >> data.Aout_dyn[i]
               >> data.Aout_delay[i];
    }

    stream >> data.route
           >> data.Nout_xover
           >> data.directivity;
#ifdef Logo_BeiJingHuiSheng
	stream >> data.route_mode
           >> data.threshold
           >> data.default_route
           >> data.auto_recovery;
#endif
    return stream;
}

void ConfigData_RT1052_AP400::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_RT1052_AP400));

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            this->channel_eq[i].eq[j].init(j);
        }
    }

    for (int i = 0; i < 8; ++i)
    {
        this->Aout_dyn[i].atk     = 50;
        this->Aout_dyn[i].release = 500;
        this->Aout_dyn[i].nRatio  = 0x0100;
        this->Aout_dyn[i].th1     = (qint16)0xCE00;
        this->Aout_dyn[i].th2     = (qint16)0xEC00;

        for (int j = 0; j < 15; ++j)
        {
            this->Aout_eq[i].eq[j].init(j);
        }
    }
}

ConfigData_RT1052_AP400_Net ConfigData_RT1052_AP400::getNetPack()
{
    ConfigData_RT1052_AP400_Net netPack;

    for (int i = 0; i < 3; ++i)
    {
        netPack.channel_gain[i] = this->channel_gain[i];
        netPack.channel_eq[i] = this->channel_eq[i].getNetPack();
    }

    for (int i = 0; i < 8; ++i)
    {
        netPack.Aout_xover[i] = this->Aout_xover[i];
        netPack.Aout_eq[i]    = this->Aout_eq[i].getNetPack();
        netPack.Aout_gain[i]  = this->Aout_gain[i];
        netPack.Aout_dyn[i]   = this->Aout_dyn[i];
        netPack.Aout_delay[i] = this->Aout_delay[i];
    }

    netPack.route       = this->route;
    netPack.Nout_xover  = this->Nout_xover;
    netPack.directivity = this->directivity;
#ifdef Logo_BeiJingHuiSheng
    netPack.route_mode  = this->route_mode;
    netPack.threshold   = this->threshold;
    netPack.default_route  = this->default_route;
    netPack.auto_recovery = this->auto_recovery;
#endif
    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_RT1052_AP400_Net
///////////////////////////////////////////////////////////////////////
ConfigData_RT1052_AP400 ConfigData_RT1052_AP400_Net::getDataPack()
{
    ConfigData_RT1052_AP400 dataPack;

    for (int i = 0; i < 3; ++i)
    {
        dataPack.channel_gain[i] = this->channel_gain[i];
        dataPack.channel_eq[i] = this->channel_eq[i].getDataPack();
    }

    for (int i = 0; i < 8; ++i)
    {
        dataPack.Aout_xover[i] = this->Aout_xover[i];
        dataPack.Aout_eq[i]    = this->Aout_eq[i].getDataPack();
        dataPack.Aout_gain[i]  = this->Aout_gain[i];
        dataPack.Aout_dyn[i]   = this->Aout_dyn[i];
        dataPack.Aout_delay[i] = this->Aout_delay[i];
    }

    dataPack.route       = this->route;
    dataPack.Nout_xover  = this->Nout_xover;
    dataPack.directivity = this->directivity;
#ifdef Logo_BeiJingHuiSheng
    dataPack.route_mode  = this->route_mode;
    dataPack.threshold   = this->threshold;
    dataPack.default_route  = this->default_route;
    dataPack.auto_recovery = this->auto_recovery;
#endif
    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Mic_Quad
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_Mic_Quad& data)
{
    for (int i = 0; i < 2; ++i)
    {
        stream << data.gain[i];
    }
    for (int i = 0; i < 4; ++i)
    {
        stream << data.phantom[i]
               << data.N_gain[i]
               << data.N_xover[i]
               << data.N_dyn[i]
               << data.N_eq[i]
               << data.NOut_gain[i]
               << data.N_mixer;
    }

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_Mic_Quad& data)
{
    for (int i = 0; i < 2; ++i)
    {
        stream >> data.gain[i];
    }
    for (int i = 0; i < 4; ++i)
    {
        stream >> data.phantom[i]
               >> data.N_gain[i]
               >> data.N_xover[i]
               >> data.N_dyn[i]
               >> data.N_eq[i]
               >> data.NOut_gain[i];
    }
    stream >> data.N_mixer;
    return stream;
}

void ConfigData_Mic_Quad::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_Mic_Quad));

    for (int i = 0; i < 4; ++i)
    {
        this->N_dyn[i].atk     = 50;
        this->N_dyn[i].release = 500;
        this->N_dyn[i].nRatio  = 0x0100;
        this->N_dyn[i].th1     = (qint16)0xCE00;
        this->N_dyn[i].th2     = (qint16)0xEC00;

        for (int j = 0; j < 4; ++j)
        {
            this->N_eq[i].eq[j].init(j);
        }
    }
}

ConfigData_Mic_Quad_Net ConfigData_Mic_Quad::getNetPack()
{
    ConfigData_Mic_Quad_Net netPack;

    for (int i = 0; i < 2; ++i)
    {
        netPack.gain[i] = this->gain[i];
    }

    for (int i = 0; i < 4; ++i)
    {
        netPack.phantom[i] = this->phantom[i];
        netPack.N_gain[i] = this->N_gain[i];
        netPack.N_xover[i] = this->N_xover[i];
        netPack.N_dyn[i] = this->N_dyn[i];
        netPack.N_eq[i] = this->N_eq[i].getNetPack();
        netPack.NOut_gain[i] = this->NOut_gain[i];
    }
    netPack.N_mixer = this->N_mixer;

    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Mic_Quad_Net
///////////////////////////////////////////////////////////////////////
ConfigData_Mic_Quad ConfigData_Mic_Quad_Net::getDataPack()
{
    ConfigData_Mic_Quad dataPack;

    for (int i = 0; i < 2; ++i)
    {
        dataPack.gain[i] = this->gain[i];
    }

    for (int i = 0; i < 4; ++i)
    {
        dataPack.phantom[i] = this->phantom[i];
        dataPack.N_gain[i] = this->N_gain[i];
        dataPack.N_xover[i] = this->N_xover[i];
        dataPack.N_dyn[i] = this->N_dyn[i];
        dataPack.N_eq[i] = this->N_eq[i].getDataPack();
        dataPack.NOut_gain[i] = this->NOut_gain[i];
    }
    dataPack.N_mixer = this->N_mixer;

    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Amp_2_0_Mic_Line
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_Amp_2_0_Mic_Line& data)
{
    stream << data.mic_gain
           << data.mic_phantom
           << data.Nout_mixer;

    for (int i = 0; i < 2; ++i)
    {
        stream << data.gain[i]
               << data.xover[i]
               << data.dyn[i]
               << data.eq[i]
               << data.Nout_gain[i];
    }

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_Amp_2_0_Mic_Line& data)
{
    stream >> data.mic_gain
           >> data.mic_phantom
           >> data.Nout_mixer;

    for (int i = 0; i < 2; ++i)
    {
        stream >> data.gain[i]
               >> data.xover[i]
               >> data.dyn[i]
               >> data.eq[i]
               >> data.Nout_gain[i];
    }
    return stream;
}

void ConfigData_Amp_2_0_Mic_Line::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_Amp_2_0_Mic_Line));

    for (int i = 0; i < 2; ++i)
    {
        this->dyn[i].atk     = 50;
        this->dyn[i].release = 500;
        this->dyn[i].nRatio  = 0x0100;
        this->dyn[i].th1     = (qint16)0xCE00;
        this->dyn[i].th2     = (qint16)0xEC00;

        for (int j = 0; j < 4; ++j)
        {
            this->eq[i].eq[j].init(j);
        }
    }
}

ConfigData_Amp_2_0_Mic_Line_Net ConfigData_Amp_2_0_Mic_Line::getNetPack()
{
    ConfigData_Amp_2_0_Mic_Line_Net netPack;

    netPack.mic_open = this->mic_open;
    netPack.mic_gain = this->mic_gain;
    netPack.mic_phantom = this->mic_phantom;

    for (int i = 0; i < 2; ++i)
    {
        netPack.gain[i] = this->gain[i];
        netPack.xover[i] = this->xover[i];
        netPack.dyn[i] = this->dyn[i];
        netPack.eq[i] = this->eq[i].getNetPack();
        netPack.Nout_gain[i] = this->Nout_gain[i];
    }
    netPack.Nout_mixer = this->Nout_mixer;

    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Amp_2_0_Mic_Line_Net
///////////////////////////////////////////////////////////////////////
ConfigData_Amp_2_0_Mic_Line ConfigData_Amp_2_0_Mic_Line_Net::getDataPack()
{
    ConfigData_Amp_2_0_Mic_Line dataPack;

    dataPack.mic_open = this->mic_open;
    dataPack.mic_gain = this->mic_gain;
    dataPack.mic_phantom = this->mic_phantom;

    for (int i = 0; i < 2; ++i)
    {
        dataPack.gain[i] = this->gain[i];
        dataPack.xover[i] = this->xover[i];
        dataPack.dyn[i] = this->dyn[i];
        dataPack.eq[i] = this->eq[i].getDataPack();
        dataPack.Nout_gain[i] = this->Nout_gain[i];
    }
    dataPack.Nout_mixer = this->Nout_mixer;

    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Transctrl_x86Server_128_128
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_Transctrl_x86Server_128_128& data)
{
    for (int i = 0; i < 128; ++i)
    {
        for (int j = 0; j < 128; ++j)
        {
            stream << data.mixer[i][j];
        }
    }

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_Transctrl_x86Server_128_128& data)
{
    for (int i = 0; i < 128; ++i)
    {
        for (int j = 0; j < 128; ++j)
        {
            stream >> data.mixer[i][j];
        }
    }

    return stream;
}

void ConfigData_Transctrl_x86Server_128_128::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_Transctrl_x86Server_128_128));
}

ConfigData_Transctrl_x86Server_128_128_Net ConfigData_Transctrl_x86Server_128_128::getNetPack(quint8 bank)
{
    ConfigData_Transctrl_x86Server_128_128_Net netPack;

    netPack.bank = bank;
    for (int i = 0; i < 8; ++i)
    {
        memcpy(netPack.mixer[i], this->mixer[8*bank+i], 128);
    }

    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Transctrl_x86Server_128_128_Net
///////////////////////////////////////////////////////////////////////
ConfigData_Transctrl_x86Server_128_128 ConfigData_Transctrl_x86Server_128_128_Net::getDataPack(ConfigData_Transctrl_x86Server_128_128 &dataPack)
{
//    ConfigData_Transctrl_x86Server_128_128 dataPack;

    for (int i = 0; i < 8; ++i)
    {
        memcpy(dataPack.mixer[8*this->bank+i], this->mixer[i], 128);
    }

//    qDebug() << __FUNCTION__ << __LINE__
//             << this->bank;
//    for (int i = 0; i < 8; ++i)
//    {
//        for (int j = 0; j < 128; ++j)
//        {
//            if (this->mixer[i][j])
//            {

//            } else {
//                qDebug() << i << j;
//            }
//        }
//    }

    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Amp_0_2_Line
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_Amp_0_2_Line& data)
{
    for (int i = 0; i < 2; ++i)
    {
        stream << data.channel_gain[i];
    }

    for (int i = 0; i < 2; ++i)
    {
        stream << data.Aout_xover[i]
               << data.Aout_eq[i]
               << data.Aout_gain[i]
               << data.Aout_dyn[i]
               << data.Aout_delay[i];
    }

    stream << data.Aout_mixer;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_Amp_0_2_Line& data)
{
    for (int i = 0; i < 2; ++i)
    {
        stream >> data.channel_gain[i];
    }

    for (int i = 0; i < 2; ++i)
    {
        stream >> data.Aout_xover[i]
               >> data.Aout_eq[i]
               >> data.Aout_gain[i]
               >> data.Aout_dyn[i]
               >> data.Aout_delay[i];
    }

    stream >> data.Aout_mixer;

    return stream;
}

void ConfigData_Amp_0_2_Line::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_Amp_0_2_Line));

    for (int i = 0; i < 2; ++i)
    {
        this->Aout_dyn[i].atk     = 50;
        this->Aout_dyn[i].release = 500;
        this->Aout_dyn[i].nRatio  = 0x0100;
        this->Aout_dyn[i].th1     = (qint16)0xCE00;
        this->Aout_dyn[i].th2     = (qint16)0xEC00;

        for (int j = 0; j < 4; ++j)
        {
            this->Aout_eq[i].eq[j].init(j);
        }
    }
}

ConfigData_Amp_0_2_Line_Net ConfigData_Amp_0_2_Line::getNetPack()
{
    ConfigData_Amp_0_2_Line_Net netPack;

    for (int i = 0; i < 2; ++i)
    {
        netPack.channel_gain[i] = this->channel_gain[i];
    }

    for (int i = 0; i < 2; ++i)
    {
        netPack.Aout_xover[i] = this->Aout_xover[i];
        netPack.Aout_eq[i]    = this->Aout_eq[i].getNetPack();
        netPack.Aout_gain[i]  = this->Aout_gain[i];
        netPack.Aout_dyn[i]   = this->Aout_dyn[i];
        netPack.Aout_delay[i] = this->Aout_delay[i];
    }

    netPack.Aout_mixer = this->Aout_mixer;

    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Amp_0_2_Line_Net
///////////////////////////////////////////////////////////////////////
ConfigData_Amp_0_2_Line ConfigData_Amp_0_2_Line_Net::getDataPack()
{
    ConfigData_Amp_0_2_Line dataPack;

    for (int i = 0; i < 2; ++i)
    {
        dataPack.channel_gain[i] = this->channel_gain[i];
    }

    for (int i = 0; i < 2; ++i)
    {
        dataPack.Aout_xover[i] = this->Aout_xover[i];
        dataPack.Aout_eq[i]    = this->Aout_eq[i].getDataPack();
        dataPack.Aout_gain[i]  = this->Aout_gain[i];
        dataPack.Aout_dyn[i]   = this->Aout_dyn[i];
        dataPack.Aout_delay[i] = this->Aout_delay[i];
    }

    dataPack.Aout_mixer = this->Aout_mixer;

    return dataPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Monitor_SPK
//////////////////////////////////////////////////////////////////////////
QDataStream& operator<<(QDataStream& stream, const ConfigData_Monitor_SPK& data)
{
    for (int i = 0; i < 4; ++i)
    {
        stream << data.CH[i].ControlValue
               << data.CH[i].Eq
               << data.CH[i].Dyn
               << data.CH[i].divider;
    }

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_Monitor_SPK& data)
{
    for (int i = 0; i < 4; ++i)
    {
        stream >> data.CH[i].ControlValue
               >> data.CH[i].Eq
               >> data.CH[i].Dyn
               >> data.CH[i].divider;
    }

    return stream;
}

void ConfigData_Monitor_SPK::initConfigData()
{
    memset(this, 0, sizeof(ConfigData_Monitor_SPK));

    for (int i = 0; i < 4; ++i)
    {
        this->CH[i].ControlValue.channel = 1;

        this->CH[i].divider.hf.freq   = 0x07D0;
        this->CH[i].divider.hf.bypass = 0x01;
        this->CH[i].divider.lf.freq   = 0x07D0;
        this->CH[i].divider.lf.bypass = 0x01;

        this->CH[i].Dyn.atk     = 50;
        this->CH[i].Dyn.release = 500;
        this->CH[i].Dyn.nRatio  = 0x0100;
        this->CH[i].Dyn.th1     = (qint16)0xCE00;
        this->CH[i].Dyn.th2     = (qint16)0xEC00;

        for (int j = 0; j < 4; ++j)
        {
            this->CH[i].Eq.eq[j].init(j);
        }
    }
}

ConfigData_Monitor_SPK_Net ConfigData_Monitor_SPK::getNetPack()
{
    ConfigData_Monitor_SPK_Net netPack;

    for (int i = 0; i < 4; ++i)
    {
        netPack.CH[i].ControlValue = this->CH[i].ControlValue;
        netPack.CH[i].Eq           = this->CH[i].Eq.getNetPack();
        netPack.CH[i].divider      = this->CH[i].divider;
        netPack.CH[i].Dyn          = this->CH[i].Dyn;
    }

    return netPack;
}

///////////////////////////////////////////////////////////////////////
/// \brief ConfigData_Monitor_SPK_Net
///////////////////////////////////////////////////////////////////////
ConfigData_Monitor_SPK ConfigData_Monitor_SPK_Net::getDataPack()
{
    ConfigData_Monitor_SPK dataPack;

    for (int i = 0; i < 4; ++i)
    {
        dataPack.CH[i].ControlValue = this->CH[i].ControlValue;
        dataPack.CH[i].Eq           = this->CH[i].Eq.getDataPack();
        dataPack.CH[i].divider      = this->CH[i].divider;
        dataPack.CH[i].Dyn          = this->CH[i].Dyn;
    }

    return dataPack;
}

QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Gain &data)
{
    stream << data.vol
           << data.mute
           << data.phase;

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Gain& data)
{
    stream >> data.vol
           >> data.mute
           >> data.phase;

    return stream;
}

QDataStream& operator<<(QDataStream& stream, const ConfigData_STM32_Gain &data)
{
    stream << data.vol
           << data.mute;

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_STM32_Gain& data)
{
    stream >> data.vol
           >> data.mute;

    return stream;
}

QDataStream& operator<<(QDataStream& stream, const ConfigData_Xover& data)
{
    stream << data.freq[0] << data.freq[1]
           << data.stages[0] << data.stages[1]
           << data.bypass;

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_Xover& data)
{
    stream >> data.freq[0] >> data.freq[1]
           >> data.stages[0] >> data.stages[1]
           >> data.bypass;

    return stream;
}

QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Mixer_4_8& data)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            stream << data.m[i][j];
        }
    }

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Mixer_4_8& data)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            stream >> data.m[i][j];
        }
    }

    return stream;
}

QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Mixer_4_4& data)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            stream << data.m[i][j];
        }
    }

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Mixer_4_4& data)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            stream >> data.m[i][j];
        }
    }

    return stream;
}

QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Mixer_2_2& data)
{
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            stream << data.m[i][j];
        }
    }

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Mixer_2_2& data)
{
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            stream >> data.m[i][j];
        }
    }

    return stream;
}

QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Delay& data)
{
    stream << data.delay;

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Delay& data)
{
    stream >> data.delay;

    return stream;
}

QDataStream& operator<<(QDataStream& stream, const ConfigData_Amp4_4_route& data)
{
    for (int i = 0; i < 4; ++i)
    {
        stream << data.r[i];
    }

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_Amp4_4_route& data)
{
    for (int i = 0; i < 4; ++i)
    {
        stream >> data.r[i];
    }

    return stream;
}

QDataStream& operator<<(QDataStream& stream, const ConfigData_AP400_Route& data)
{
    stream << data.r[0];

    return stream;
}

QDataStream& operator>>(QDataStream& stream, ConfigData_AP400_Route& data)
{
    stream >> data.r[0];

    return stream;
}
