#include "utils.h"
#include "config.h"

bool Utils::ShowMsgBoxFrameless = true;
quint8 Utils::maxMasterVersion[4] = {0};


void Utils::MessageBoxInfo(const QString &title, const QString &text, QPixmap pixmap)
{
    QMessageBox msgBox(QMessageBox::Information, title, text, NULL);

    if(!pixmap.isNull())
        msgBox.setIconPixmap(pixmap);
    if(ShowMsgBoxFrameless) {
        msgBox.setWindowFlags(Qt::FramelessWindowHint|Qt::SubWindow);
//        msgBox.setAttribute(Qt::WA_TranslucentBackground);
    }
    msgBox.exec();
}

void Utils::MessageBoxWarning(const QString &title, const QString &text)
{
    QMessageBox msgBox(QMessageBox::Warning, title, text, NULL);

    if(ShowMsgBoxFrameless) {
        msgBox.setWindowFlags(Qt::FramelessWindowHint|Qt::SubWindow);
    }

    msgBox.exec();
}


QMessageBox::StandardButton Utils::MessageBoxQuestion(const QString &title, const QString &text,
                           QMessageBox::StandardButtons buttons)
{
    QMessageBox msgBox(QMessageBox::Question, title, text, buttons);

    if(ShowMsgBoxFrameless) {
        msgBox.setWindowFlags(Qt::FramelessWindowHint|Qt::SubWindow);
    }
    msgBox.exec();
    return msgBox.standardButton(msgBox.clickedButton());
}

PlatformType Utils::getPlatformType(DevType devType)
{
    if(devType == Dev_Type_NAP_X86_16Analog_128AES67
            || devType == Dev_Type_NAP_X86_32Analog_256AES67){
        return Platform_X86;
    }
    else if(devType == Dev_Type_NAP_FT2000_256AES67){
        return Platform_ARM;
    }
    else {
        return Platform_DSP;
    }
}

//模块显示第一行
QString Utils::getCfgTypeName(CfgType type, bool isTr)
{
    const char* name;
    switch(type) {
    //延时器
    case CFG_TYPE_DELAY_5:    name = QT_TR_NOOP("Delay(0-5ms)"); break;
    case CFG_TYPE_DELAY_10:   name = QT_TR_NOOP("Delay(0-10ms)"); break;
    case CFG_TYPE_DELAY_20:   name = QT_TR_NOOP("Delay(0-20ms)"); break;
    case CFG_TYPE_DELAY_50:   name = QT_TR_NOOP("Delay(0-50ms)"); break;
    case CFG_TYPE_DELAY_100:  name = QT_TR_NOOP("Delay(0-100ms)"); break;
    case CFG_TYPE_DELAY_500:  name = QT_TR_NOOP("Delay(0-500ms)"); break;
    case CFG_TYPE_DELAY_1000: name = QT_TR_NOOP("Delay(0-1s)"); break;
    case CFG_TYPE_HPDELY_1:  name = QT_TR_NOOP("Delay(1.00ms)"); break;
    case CFG_TYPE_HPDELY_5:  name = QT_TR_NOOP("Delay(5.00ms)"); break;
    case CFG_TYPE_HPDELY_10: name = QT_TR_NOOP("Delay(10.00ms)"); break;
    case CFG_TYPE_HPDELY_20: name = QT_TR_NOOP("Delay(20.00ms)"); break;
    case CFG_TYPE_HPDELY_50: name = QT_TR_NOOP("Delay(50.00ms)"); break;
    //路由器
    case CFG_TYPE_ROUTER_4:  name = QT_TR_NOOP("Router(4*4)"); break;
    case CFG_TYPE_ROUTER_8:  name = QT_TR_NOOP("Router(8*8)"); break;
    case CFG_TYPE_ROUTER_16: name = QT_TR_NOOP("Router(16*16)"); break;
    case CFG_TYPE_ROUTER_32: name = QT_TR_NOOP("Router(32*32)"); break;
    case CFG_TYPE_ROUTER_48: name = QT_TR_NOOP("Router(48*48)"); break;
    case CFG_TYPE_ROUTER_64: name = QT_TR_NOOP("Router(64*64)"); break;
    case CFG_TYPE_ROUTER_96: name = QT_TR_NOOP("Router(96*96)"); break;
    case CFG_TYPE_ROUTER_128: name = QT_TR_NOOP("Router(128*128)"); break;
    case CFG_TYPE_ROUTER_192: name = QT_TR_NOOP("Router(192*192)"); break;
    case CFG_TYPE_ROUTER_256: name = QT_TR_NOOP("Router(256*256)"); break;
    //混音器
    case CFG_TYPE_MIXER_4X1: name = QT_TR_NOOP("Mixer(4*1)"); break;
    case CFG_TYPE_MIXER_4X2: name = QT_TR_NOOP("Mixer(4*2)"); break;
    case CFG_TYPE_MIXER_4X4: name = QT_TR_NOOP("Mixer(4*4)"); break;
    case CFG_TYPE_MIXER_8X1: name = QT_TR_NOOP("Mixer(8*1)"); break;
    case CFG_TYPE_MIXER_8X2: name = QT_TR_NOOP("Mixer(8*2)"); break;
    case CFG_TYPE_MIXER_8X4: name = QT_TR_NOOP("Mixer(8*4)"); break;
    case CFG_TYPE_MIXER_8X8: name = QT_TR_NOOP("Mixer(8*8)"); break;
    case CFG_TYPE_MIXER_16X16: name = QT_TR_NOOP("Mixer(16*16)"); break;
    case CFG_TYPE_MIXER_32X32: name = QT_TR_NOOP("Mixer(32x32)"); break;
    case CFG_TYPE_MIXER_64X64: name = QT_TR_NOOP("Mixer(64x64)"); break;
    //动态处理
    case CFG_TYPE_DYN_AUTO:   name = QT_TR_NOOP("Dyn(Auto)"); break;
    case CFG_TYPE_DYN_COMP:   name = QT_TR_NOOP("Dyn(Compressor)"); break;
    case CFG_TYPE_DYN_LIMIT:  name = QT_TR_NOOP("Dyn(Peak Limiter)"); break;
    case CFG_TYPE_DYN_NOISE:  name = QT_TR_NOOP("Dyn(Gate)"); break;
    case CFG_TYPE_DYN_EXPAND: name = QT_TR_NOOP("Dyn(Expander)"); break;
    case CFG_TYPE_DYN_PEAK:   name = QT_TR_NOOP("Dyn(Peak)"); break;
    //均衡器
    case CFG_TYPE_EQ_1:   name = QT_TR_NOOP("EQ(1 band)"); break;
    case CFG_TYPE_EQ_3:   name = QT_TR_NOOP("EQ(3 band)"); break;
    case CFG_TYPE_EQ_5:   name = QT_TR_NOOP("EQ(5 band)"); break;
    case CFG_TYPE_EQ_7:   name = QT_TR_NOOP("EQ(7 band)"); break;
    case CFG_TYPE_EQ_10:  name = QT_TR_NOOP("EQ(10 band)"); break;
    case CFG_TYPE_EQ_16:  name = QT_TR_NOOP("EQ(16 band)"); break;
    case CFG_TYPE_EQ_1X3: name = QT_TR_NOOP("EQ(1/3 SHG)"); break;
    case CFG_TYPE_EQ_2X3: name = QT_TR_NOOP("EQ(2/3 SHG)"); break;
    case CFG_TYPE_EQ_3X3: name = QT_TR_NOOP("EQ(1 SHG)"); break;
    //滤波器
    case CFG_TYPE_FILTE_LP: name = QT_TR_NOOP("Filter(L Pass)"); break;
    case CFG_TYPE_FILTE_HP: name = QT_TR_NOOP("Filter(H Pass)"); break;
    case CFG_TYPE_FILTE_LS: name = QT_TR_NOOP("Filter(L Shelf)"); break;
    case CFG_TYPE_FILTE_HS: name = QT_TR_NOOP("Filter(H Shelf)"); break;
    case CFG_TYPE_FILTER_BP: name = QT_TR_NOOP("Filter(Band Pass)"); break;
    case CFG_TYPE_FILTER_AP: name = QT_TR_NOOP("Filter(All Pass)"); break;
    //分频器
    case CFG_TYPE_SPLIT_2: name = QT_TR_NOOP("Crossover(2 Way)"); break;
    case CFG_TYPE_SPLIT_3: name = QT_TR_NOOP("Crossover(3 Way)"); break;
    case CFG_TYPE_SPLIT_4: name = QT_TR_NOOP("Crossover(4 Way)"); break;
    //信号发生器
    case CFG_TYPE_SIG_SINE:      name = QT_TR_NOOP("Signal(Sine)"); break;
    case CFG_TYPE_SIG_NOISE:     name = QT_TR_NOOP("Signal(White)"); break;
    case CFG_TYPE_SIG_PINKNOISE: name = QT_TR_NOOP("Signal(Pink)"); break;
    //仪表显示
    case CFG_TYPE_METER_1: name = QT_TR_NOOP("Meter(1 Ch)"); break;
    case CFG_TYPE_METER_2: name = QT_TR_NOOP("Meter(2 Ch)"); break;
    case CFG_TYPE_METER_4: name = QT_TR_NOOP("Meter(4 Ch)"); break;
    case CFG_TYPE_METER_8: name = QT_TR_NOOP("Meter(8 Ch)"); break;
    //输入输出端口
    case CFG_TYPE_INPUT_1:  name = "Input(1 Ch)";  break;
    case CFG_TYPE_INPUT_2:  name = "Input(2 Ch)";  break;
    case CFG_TYPE_INPUT_4:  name = "Input(4 Ch)";  break;
    case CFG_TYPE_INPUT_8:  name = "Input(8 Ch)";  break;
    case CFG_TYPE_INPUT_16: name = "Input(16 Ch)";  break;

    case CFG_TYPE_OUTPUT_1:  name = "Output(1 Ch)";  break;
    case CFG_TYPE_OUTPUT_2:  name = "Output(2 Ch)";  break;
    case CFG_TYPE_OUTPUT_4:  name = "Output(4 Ch)";  break;
    case CFG_TYPE_OUTPUT_8:  name = "Output(8 Ch)";  break;
    case CFG_TYPE_OUTPUT_16: name = "Output(16 Ch)";  break;
    //其它
    case CFG_TYPE_GPI: name = "Gpi(2->1)"; break;

    case CFG_TYPE_FREQSHIFT:      name = QT_TR_NOOP("Freq Shift"); break;
    case CFG_TYPE_FREQSHIFT_PLUS: name = QT_TR_NOOP("Freq Shift Plus"); break;

    case CFG_TYPE_AUTOMIX_GS_4:    name = QT_TR_NOOP("Gain-Sharing(4)"); break;
    case CFG_TYPE_AUTOMIX_GS_8:    name = QT_TR_NOOP("Gain-Sharing(8)"); break;
    case CFG_TYPE_AUTOMIX_GS_16:   name = QT_TR_NOOP("Gain-Sharing(16)"); break;
    case CFG_TYPE_AUTOMIX_GS_32:   name = QT_TR_NOOP("Gain-Sharing(32)"); break;
    case CFG_TYPE_AUTOMIXNGAT_4:  name = QT_TR_NOOP("Gating(4)(Absolute Threshold)"); break;
    case CFG_TYPE_AUTOMIXNGAT_8:  name = QT_TR_NOOP("Gating(8)(Absolute Threshold)"); break;
    case CFG_TYPE_AUTOMIXNGAT_16: name = QT_TR_NOOP("Gating(16)(Absolute Threshold)"); break;
    case CFG_TYPE_AUTOMIXNGAT_32: name = QT_TR_NOOP("Gating(32)(Absolute Threshold)"); break;
    case CFG_TYPE_AUTOMIXNGRT_4:  name = QT_TR_NOOP("Gating(4)(Relative Threshold)"); break;
    case CFG_TYPE_AUTOMIXNGRT_8:  name = QT_TR_NOOP("Gating(8)(Relative Threshold)"); break;
    case CFG_TYPE_AUTOMIXNGRT_16: name = QT_TR_NOOP("Gating(16)(Relative Threshold)"); break;
    case CFG_TYPE_AUTOMIXNGRT_32: name = QT_TR_NOOP("Gating(32)(Relative Threshold)"); break;

    case CFG_TYPE_AEC_2_2: name = "AEC-NR"; break;

    case CFG_TYPE_VOICETUBE_1: name = "Microphone control"; break;
    case CFG_TYPE_VOICETUBE_2: name = "Microphone control v2"; break;
    //FIR Custom
    case CFG_TYPE_SOFTFIR_MONO256:      name = QT_TR_NOOP("FIR(mono 256)"); break;
    case CFG_TYPE_SOFTFIR_MONO512:      name = QT_TR_NOOP("FIR(mono 512)"); break;
    case CFG_TYPE_SOFTFIR_MONO1024:     name = QT_TR_NOOP("FIR(mono 1024)"); break;
    case CFG_TYPE_SOFTFIR_STEREO256:    name = QT_TR_NOOP("FIR(stereo 256)"); break;
    case CFG_TYPE_SOFTFIR_STEREO512:    name = QT_TR_NOOP("FIR(stereo 512)"); break;
    case CFG_TYPE_SOFTFIR_STEREO1024:   name = QT_TR_NOOP("FIR(stereo 1024)"); break;
    case CFG_TYPE_SOFTFIR_4R256:        name = QT_TR_NOOP("FIR(Quad 256)"); break;
    case CFG_TYPE_SOFTFIR_4R512:        name = QT_TR_NOOP("FIR(Quad 512)"); break;
    case CFG_TYPE_SOFTFIR_4R1024:       name = QT_TR_NOOP("FIR(Quad 1024)"); break;
    //啸叫抑制
    case CFG_TYPE_NFC1_8NF:  name = QT_TR_NOOP("NFC(8)"); break;
    case CFG_TYPE_NFC1_16NF: name = QT_TR_NOOP("NFC(16)"); break;
    case CFG_TYPE_NFC1_24NF: name = QT_TR_NOOP("NFC(24)"); break;
    case CFG_TYPE_NFC1_32NF: name = QT_TR_NOOP("NFC(32)"); break;
    //PRIORITY DUCKER
    case CFG_TYPE_PDUCKER_4:  name = QT_TR_NOOP("ducker(4)"); break;
    case CFG_TYPE_PDUCKER_8:  name = QT_TR_NOOP("ducker(8)"); break;
    case CFG_TYPE_PDUCKER_16: name = QT_TR_NOOP("ducker(16)"); break;
    case CFG_TYPE_PDUCKER_32: name = QT_TR_NOOP("ducker(32)"); break;
    //GAIN RAMP
    case CFG_TYPE_GAINRAMP_1:  name = QT_TR_NOOP("ramp 1"); break;
    case CFG_TYPE_GAINRAMP_2:  name = QT_TR_NOOP("ramp 2"); break;
    case CFG_TYPE_GAINRAMP_4:  name = QT_TR_NOOP("ramp 4"); break;
    case CFG_TYPE_GAINRAMP_8:  name = QT_TR_NOOP("ramp 8"); break;
    case CFG_TYPE_GAINRAMP_16: name = QT_TR_NOOP("ramp 16"); break;
    case CFG_TYPE_GAINRAMP_32: name = QT_TR_NOOP("ramp 32"); break;
    //Gain
    case CFG_TYPE_GAIN_1:  name = QT_TR_NOOP("Gain 1");  break;
    case CFG_TYPE_GAIN_2:  name = QT_TR_NOOP("Gain 2");  break;
    case CFG_TYPE_GAIN_4:  name = QT_TR_NOOP("Gain 4");  break;
    case CFG_TYPE_GAIN_8:  name = QT_TR_NOOP("Gain 8");  break;
    case CFG_TYPE_GAIN_16: name = QT_TR_NOOP("Gain 16"); break;
    case CFG_TYPE_GAIN_32: name = QT_TR_NOOP("Gain 32"); break;
    //Crossfader
    case CFG_TYPE_CROSSFADER_2X1:   name = QT_TR_NOOP("Crossfader 2*1");   break;
    case CFG_TYPE_CROSSFADER_4X2:   name = QT_TR_NOOP("Crossfader 4*2");   break;
    case CFG_TYPE_CROSSFADER_8X4:   name = QT_TR_NOOP("Crossfader 8*4");   break;
    case CFG_TYPE_CROSSFADER_16X8:  name = QT_TR_NOOP("Crossfader 16*8");  break;
    case CFG_TYPE_CROSSFADER_32X16: name = QT_TR_NOOP("Crossfader 32*16"); break;
    //AEC-Soft 主核
    case CFG_TYPE_SOFTAEC_8K64: return tr("AEC-8K64");
    case CFG_TYPE_SOFTAEC_8K96: return tr("AEC-8K96");
    case CFG_TYPE_SOFTAEC_8K128: return tr("AEC-8K128");
    case CFG_TYPE_SOFTAEC_8K160: return tr("AEC-8K160");
    case CFG_TYPE_SOFTAEC_8K192: return tr("AEC-8K192");
    case CFG_TYPE_SOFTAEC_8K224: return tr("AEC-8K224");
    case CFG_TYPE_SOFTAEC_8K256: return tr("AEC-8K256");
    case CFG_TYPE_SOFTAEC_16K64: return tr("AEC-16K64");
    case CFG_TYPE_SOFTAEC_16K96: return tr("AEC-16K96");
    case CFG_TYPE_SOFTAEC_16K128: return tr("AEC-16K128");
    case CFG_TYPE_SOFTAEC_16K160: return tr("AEC-16K160");
    case CFG_TYPE_SOFTAEC_16K192: return tr("AEC-16K192");
    case CFG_TYPE_SOFTAEC_16K224: return tr("AEC-16K224");
    case CFG_TYPE_SOFTAEC_16K256: return tr("AEC-16K256");
    //AEC-Soft 独立核
    case CFG_TYPE_SOFTAEC_IK_8K64: return tr("AEC-8K64IK");
    case CFG_TYPE_SOFTAEC_IK_8K96: return tr("AEC-8K96IK");
    case CFG_TYPE_SOFTAEC_IK_8K128: return tr("AEC-8K128IK");
    case CFG_TYPE_SOFTAEC_IK_8K160: return tr("AEC-8K160IK");
    case CFG_TYPE_SOFTAEC_IK_8K192: return tr("AEC-8K192IK");
    case CFG_TYPE_SOFTAEC_IK_8K224: return tr("AEC-8K224IK");
    case CFG_TYPE_SOFTAEC_IK_8K256: return tr("AEC-8K256IK");
    case CFG_TYPE_SOFTAEC_IK_16K64: return tr("AEC-16K64IK");
    case CFG_TYPE_SOFTAEC_IK_16K96: return tr("AEC-16K96IK");
    case CFG_TYPE_SOFTAEC_IK_16K128: return tr("AEC-16K128IK");
    case CFG_TYPE_SOFTAEC_IK_16K160: return tr("AEC-16K160IK");
    case CFG_TYPE_SOFTAEC_IK_16K192: return tr("AEC-16K192IK");
    case CFG_TYPE_SOFTAEC_IK_16K224: return tr("AEC-16K224IK");
    case CFG_TYPE_SOFTAEC_IK_16K256: return tr("AEC-16K256IK");
//    case CFG_TYPE_SOFTAEC_IK_48K64: return tr("AEC-48K64IK");
//    case CFG_TYPE_SOFTAEC_IK_48K96: return tr("AEC-48K96IK");
//    case CFG_TYPE_SOFTAEC_IK_48K128: return tr("AEC-48K128IK");
//    case CFG_TYPE_SOFTAEC_IK_48K160: return tr("AEC-48K160IK");
//    case CFG_TYPE_SOFTAEC_IK_48K192: return tr("AEC-48K192IK");
//    case CFG_TYPE_SOFTAEC_IK_48K256: return tr("AEC-48K256IK");
    //AudioPlayer
    case CFG_TYPE_AUDIO_PLAYER1:    name = QT_TR_NOOP("Player 1ch"); break;
    case CFG_TYPE_AUDIO_PLAYER2:    name = QT_TR_NOOP("Player 2ch"); break;
    case CFG_TYPE_AUDIO_PLAYER4:    name = QT_TR_NOOP("Player 4ch"); break;
    case CFG_TYPE_AUDIO_PLAYER8:    name = QT_TR_NOOP("Player 8ch"); break;
    case CFG_TYPE_AUDIO_PLAYER16:   name = QT_TR_NOOP("Player 16ch"); break;
    case CFG_TYPE_AUDIO_RECORD1:    name = QT_TR_NOOP("Record 1ch"); break;
    case CFG_TYPE_AUDIO_RECORD2:    name = QT_TR_NOOP("Record 2ch"); break;
    case CFG_TYPE_AUDIO_RECORD4:    name = QT_TR_NOOP("Record 4ch"); break;
    case CFG_TYPE_CTRL_LUA: name = "Lua Control"; break;
    case CFG_TYPE_SNAPSHOT: name = QT_TR_NOOP("Snapshot"); break;
    case CFG_TYPE_SNAPSHOT_BANK: name = QT_TR_NOOP("Snapshot Bank"); break;
    case CFG_TYPE_CTRL_GPI: name = QT_TR_NOOP("GPI"); break;
    case CFG_TYPE_CTRL_GPO: name = QT_TR_NOOP("GPO"); break;
    case CFG_TYPE_CTRL_AD: name = QT_TR_NOOP("ADC"); break;
    case CFG_TYPE_CTRL_RS232: name = QT_TR_NOOP("RS232"); break;
    case CFG_TYPE_CTRL_RS485: name = QT_TR_NOOP("RS485"); break;

//////////////////////监控设备///////////////////////////
    case CFG_TYPE_1: name = "level meter";  break;
    case CFG_TYPE_2: name = "clippers"; break;
    case CFG_TYPE_3: name = "button"; break;
    case CFG_TYPE_4: name = "label"; break;
    case CFG_TYPE_5: name = "live streaming"; break;
    case CFG_TYPE_SCENE: name = "Scene"; break;
    case CFG_TYPE_NETWORKDEVICE: name = "Network device"; break;

    default: name = "Unknow"; break;
    }
    if(isTr) return tr(name);
    else return QString(tr(name));
}


QString Utils::getCfgPortModelName(bool isInput, DevPortType devType, int logicCh, int portGroupNum)
{
    QString itemName = "unknown";
    QString subfix = tr("Input");
    if(!isInput) {
        subfix = tr("Output");
    }
    QString perfix = "A";
    if(devType == PortType_Dante) perfix = "D";
    else if(devType == PortType_AES67) perfix = "N";
    else if(devType == PortType_USB) perfix = "U";

    int sPort = logicCh + 1;
    int ePort = logicCh + portGroupNum;

    if(portGroupNum == 1) itemName = QString("%1%2 %3").arg(perfix).arg(sPort,2,10,QLatin1Char('0')).arg(subfix);
    else itemName = QString("%1%2~%3 %4").arg(perfix).arg(sPort,2,10,QLatin1Char('0')).arg(ePort,2,10,QLatin1Char('0')).arg(subfix);
    return itemName;
}


QString Utils::getCfgPortChannelName(DevPortType devType, int logicCh, QString subfix)
{
    QString perfix = "A";
    if(devType == PortType_Dante) perfix = "D";
    else if(devType == PortType_AES67) perfix = "N";
    else if(devType == PortType_USB) perfix = "U";

    QString itemName = QString("%1%2").arg(perfix).arg(logicCh + 1,2,10,QLatin1Char('0'));
    if(!subfix.isEmpty()){
        itemName += " " + subfix;
    }
    return itemName;
}

QString Utils::getCfgModelName_FangTu(CfgType type)
{
    switch(type) {
    case CFG_TYPE_DELAY_5:
    case CFG_TYPE_DELAY_10:
    case CFG_TYPE_DELAY_20:
    case CFG_TYPE_DELAY_50:     return tr("Delay");
    case CFG_TYPE_DELAY_100:    return tr("0-100ms");
    case CFG_TYPE_DELAY_500:    return tr("0-500ms");
    case CFG_TYPE_DELAY_1000:   return tr("0-1000ms");
    case CFG_TYPE_HPDELY_1:    return tr("0-1ms");
    case CFG_TYPE_HPDELY_5:
    case CFG_TYPE_HPDELY_10:
    case CFG_TYPE_HPDELY_20:
    case CFG_TYPE_HPDELY_50:   return tr("Delay");    //延时器

    case CFG_TYPE_ROUTER_4:     return tr("4-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_8:     return tr("8-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_16:    return tr("16-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_32:    return tr("32-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_48:    return tr("48-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_64:    return tr("64-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_96:    return tr("96-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_128:    return tr("128-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_192:    return tr("192-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_256:    return tr("256-Channel inputs");   //路由器

    case CFG_TYPE_MIXER_4X1:
    case CFG_TYPE_MIXER_4X2:
    case CFG_TYPE_MIXER_4X4:    return tr("4-Channel inputs");
    case CFG_TYPE_MIXER_8X1:
    case CFG_TYPE_MIXER_8X2:
    case CFG_TYPE_MIXER_8X4:
    case CFG_TYPE_MIXER_8X8:    return tr("8-Channel inputs");
    case CFG_TYPE_MIXER_16X16:  return tr("16-Channel inputs");
    case CFG_TYPE_MIXER_32X32:  return tr("32-Channel inputs");
    case CFG_TYPE_MIXER_64X64:  return tr("64-Channel inputs");    //混音器

    case CFG_TYPE_DYN_AUTO:     return tr("Automatic Gain Control");
    case CFG_TYPE_DYN_COMP:     return tr("Compressor");
    case CFG_TYPE_DYN_LIMIT:    return tr("Peak Limiter");
    case CFG_TYPE_DYN_NOISE:    return tr("Gate");
    case CFG_TYPE_DYN_EXPAND:   return tr("Expander");
    case CFG_TYPE_DYN_PEAK:     return tr("Peak");

    case CFG_TYPE_EQ_1:
    case CFG_TYPE_EQ_3:
    case CFG_TYPE_EQ_5:
    case CFG_TYPE_EQ_7:
    case CFG_TYPE_EQ_10:
    case CFG_TYPE_EQ_16:        return tr("Parametric Equalizer");         //均衡器
    case CFG_TYPE_EQ_1X3:
    case CFG_TYPE_EQ_2X3:
    case CFG_TYPE_EQ_3X3:       return tr("Graphic Equalizer");         //均衡器

    case CFG_TYPE_FILTE_LP:     return tr("Low-Pass Filter");    //滤波器
    case CFG_TYPE_FILTE_HP:     return tr("High-Pass Filter");    //滤波器
    case CFG_TYPE_FILTE_LS:
    case CFG_TYPE_FILTE_HS:     return tr("Specialty Filter");    //滤波器
    case CFG_TYPE_FILTER_BP:   return tr("Band-Pass Filter");    //滤波器
    case CFG_TYPE_FILTER_AP:   return tr("All-Pass Filter");

    case CFG_TYPE_FREQSHIFT:
    case CFG_TYPE_FREQSHIFT_PLUS:   return tr("Frequency Shifter");

    case CFG_TYPE_NFC1_8NF:
    case CFG_TYPE_NFC1_16NF:
    case CFG_TYPE_NFC1_24NF:
    case CFG_TYPE_NFC1_32NF:        return tr("Notch Feedback Controller");

    case CFG_TYPE_AUTOMIX_GS_4:
    case CFG_TYPE_AUTOMIX_GS_8:
    case CFG_TYPE_AUTOMIX_GS_16:
    case CFG_TYPE_AUTOMIX_GS_32:
    case CFG_TYPE_AUTOMIXNGAT_4:
    case CFG_TYPE_AUTOMIXNGAT_8:
    case CFG_TYPE_AUTOMIXNGAT_16:
    case CFG_TYPE_AUTOMIXNGAT_32:
    case CFG_TYPE_AUTOMIXNGRT_4:
    case CFG_TYPE_AUTOMIXNGRT_8:
    case CFG_TYPE_AUTOMIXNGRT_16:
    case CFG_TYPE_AUTOMIXNGRT_32:  return tr("Automatic Mic Mixer");

    case CFG_TYPE_SIG_SINE:      return tr("Sine");      //信号发生器
    case CFG_TYPE_SIG_NOISE:     return tr("White Noise");      //信号发生器
    case CFG_TYPE_SIG_PINKNOISE: return tr("Pink Noise");      //信号发生器

    case CFG_TYPE_METER_1: return tr("1Channel-Meter");
    case CFG_TYPE_METER_2: return tr("2Channel-Meter");
    case CFG_TYPE_METER_4: return tr("4Channel-Meter");
    case CFG_TYPE_METER_8: return tr("8Channel-Meter");    //仪表显示

    case CFG_TYPE_AEC_2_2: return tr("Acoustic Echo Canceler");

    case CFG_TYPE_SPLIT_2:
    case CFG_TYPE_SPLIT_3:
    case CFG_TYPE_SPLIT_4:      return tr("Crossover");    //分频器

    case CFG_TYPE_INPUT_1:
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
    case CFG_TYPE_OUTPUT_1:
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:   return tr("Channel");  //输入输出端口

    case CFG_TYPE_GPI:          return tr("Gpi");

    case CFG_TYPE_SOFTFIR_MONO256:
    case CFG_TYPE_SOFTFIR_MONO512:
    case CFG_TYPE_SOFTFIR_MONO1024:
    case CFG_TYPE_SOFTFIR_STEREO256:
    case CFG_TYPE_SOFTFIR_STEREO512:
    case CFG_TYPE_SOFTFIR_STEREO1024:
    case CFG_TYPE_SOFTFIR_4R256:
    case CFG_TYPE_SOFTFIR_4R512:
    case CFG_TYPE_SOFTFIR_4R1024:   return tr("FIR Filter");

    case CFG_TYPE_PDUCKER_4:
    case CFG_TYPE_PDUCKER_8:
    case CFG_TYPE_PDUCKER_16:
    case CFG_TYPE_PDUCKER_32:      return tr("Priority Ducker");

    case CFG_TYPE_GAINRAMP_1:
    case CFG_TYPE_GAINRAMP_2:
    case CFG_TYPE_GAINRAMP_4:
    case CFG_TYPE_GAINRAMP_8:
    case CFG_TYPE_GAINRAMP_16:
    case CFG_TYPE_GAINRAMP_32:     return tr("Gain Ramp");
    //Gain
    case CFG_TYPE_GAIN_1:
    case CFG_TYPE_GAIN_2:
    case CFG_TYPE_GAIN_4:
    case CFG_TYPE_GAIN_8:
    case CFG_TYPE_GAIN_16:
    case CFG_TYPE_GAIN_32: return tr("Gain");
    //Crossfader
    case CFG_TYPE_CROSSFADER_2X1:
    case CFG_TYPE_CROSSFADER_4X2:
    case CFG_TYPE_CROSSFADER_8X4:
    case CFG_TYPE_CROSSFADER_16X8:
    case CFG_TYPE_CROSSFADER_32X16: return tr("Crossfader");
    //AEC-Soft 主核
    case CFG_TYPE_SOFTAEC_8K64: return tr("AEC-8K64");
    case CFG_TYPE_SOFTAEC_8K96: return tr("AEC-8K96");
    case CFG_TYPE_SOFTAEC_8K128: return tr("AEC-8K128");
    case CFG_TYPE_SOFTAEC_8K160: return tr("AEC-8K160");
    case CFG_TYPE_SOFTAEC_8K192: return tr("AEC-8K192");
    case CFG_TYPE_SOFTAEC_8K224: return tr("AEC-8K224");
    case CFG_TYPE_SOFTAEC_8K256: return tr("AEC-8K256");
    case CFG_TYPE_SOFTAEC_16K64: return tr("AEC-16K64");
    case CFG_TYPE_SOFTAEC_16K96: return tr("AEC-16K96");
    case CFG_TYPE_SOFTAEC_16K128: return tr("AEC-16K128");
    case CFG_TYPE_SOFTAEC_16K160: return tr("AEC-16K160");
    case CFG_TYPE_SOFTAEC_16K192: return tr("AEC-16K192");
    case CFG_TYPE_SOFTAEC_16K224: return tr("AEC-16K224");
    case CFG_TYPE_SOFTAEC_16K256: return tr("AEC-16K256");
    //AEC-Soft 独立核
    case CFG_TYPE_SOFTAEC_IK_8K64: return tr("AEC-8K64IK");
    case CFG_TYPE_SOFTAEC_IK_8K96: return tr("AEC-8K96IK");
    case CFG_TYPE_SOFTAEC_IK_8K128: return tr("AEC-8K128IK");
    case CFG_TYPE_SOFTAEC_IK_8K160: return tr("AEC-8K160IK");
    case CFG_TYPE_SOFTAEC_IK_8K192: return tr("AEC-8K192IK");
    case CFG_TYPE_SOFTAEC_IK_8K224: return tr("AEC-8K224IK");
    case CFG_TYPE_SOFTAEC_IK_8K256: return tr("AEC-8K256IK");
    case CFG_TYPE_SOFTAEC_IK_16K64: return tr("AEC-16K64IK");
    case CFG_TYPE_SOFTAEC_IK_16K96: return tr("AEC-16K96IK");
    case CFG_TYPE_SOFTAEC_IK_16K128: return tr("AEC-16K128IK");
    case CFG_TYPE_SOFTAEC_IK_16K160: return tr("AEC-16K160IK");
    case CFG_TYPE_SOFTAEC_IK_16K192: return tr("AEC-16K192IK");
    case CFG_TYPE_SOFTAEC_IK_16K224: return tr("AEC-16K224IK");
    case CFG_TYPE_SOFTAEC_IK_16K256: return tr("AEC-16K256IK");
//    case CFG_TYPE_SOFTAEC_IK_48K64: return tr("AEC-48K64IK");
//    case CFG_TYPE_SOFTAEC_IK_48K96: return tr("AEC-48K96IK");
//    case CFG_TYPE_SOFTAEC_IK_48K128: return tr("AEC-48K128IK");
//    case CFG_TYPE_SOFTAEC_IK_48K160: return tr("AEC-48K160IK");
//    case CFG_TYPE_SOFTAEC_IK_48K192: return tr("AEC-48K192IK");
//    case CFG_TYPE_SOFTAEC_IK_48K256: return tr("AEC-48K256IK");

    case CFG_TYPE_AUDIO_PLAYER1:
    case CFG_TYPE_AUDIO_PLAYER2:
    case CFG_TYPE_AUDIO_PLAYER4:
    case CFG_TYPE_AUDIO_PLAYER8:
    case CFG_TYPE_AUDIO_PLAYER16:           return tr("Audio Player");
    case CFG_TYPE_AUDIO_RECORD1:
    case CFG_TYPE_AUDIO_RECORD2:
    case CFG_TYPE_AUDIO_RECORD4:            return tr("Audio Record");
    case CFG_TYPE_CTRL_LUA:          return tr("Lua Control");
    case CFG_TYPE_VOICETUBE_1:
    case CFG_TYPE_VOICETUBE_2:      return tr("Mic Control");

    case CFG_TYPE_SNAPSHOT: return tr("Snapshot");
    case CFG_TYPE_CTRL_GPI: return tr("GPI");
    case CFG_TYPE_CTRL_GPO: return tr("GPO");
    case CFG_TYPE_CTRL_AD: return tr("ADC");
    case CFG_TYPE_CTRL_RS232: return tr("RS232");
    case CFG_TYPE_CTRL_RS485: return tr("RS485");

    default: return QString("Unknow:%1").arg(type); //其它
    }
}

QString Utils::getCfgModelName_Other(CfgType type)
{
    switch(type) {
    case CFG_TYPE_DELAY_5:
    case CFG_TYPE_DELAY_10:
    case CFG_TYPE_DELAY_20:
    case CFG_TYPE_DELAY_50:     return tr("Delay");
    case CFG_TYPE_DELAY_100:    return tr("0-100ms");
    case CFG_TYPE_DELAY_500:    return tr("0-500ms");
    case CFG_TYPE_DELAY_1000:   return tr("0-1000ms");
    case CFG_TYPE_HPDELY_1:    return tr("0-1ms");
    case CFG_TYPE_HPDELY_5:
    case CFG_TYPE_HPDELY_10:
    case CFG_TYPE_HPDELY_20:
    case CFG_TYPE_HPDELY_50:   return tr("Delay");    //延时器

    case CFG_TYPE_ROUTER_4:     return tr("4-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_8:     return tr("8-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_16:    return tr("16-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_32:    return tr("32-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_48:    return tr("48-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_64:    return tr("64-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_96:    return tr("96-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_128:    return tr("128-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_192:    return tr("192-Channel inputs");   //路由器
    case CFG_TYPE_ROUTER_256:    return tr("256-Channel inputs");   //路由器

    case CFG_TYPE_MIXER_4X1:
    case CFG_TYPE_MIXER_4X2:
    case CFG_TYPE_MIXER_4X4:    return tr("4-Channel inputs");
    case CFG_TYPE_MIXER_8X1:
    case CFG_TYPE_MIXER_8X2:
    case CFG_TYPE_MIXER_8X4:
    case CFG_TYPE_MIXER_8X8:    return tr("8-Channel inputs");
    case CFG_TYPE_MIXER_16X16:  return tr("16-Channel inputs");
    case CFG_TYPE_MIXER_32X32:  return tr("32-Channel inputs");
    case CFG_TYPE_MIXER_64X64:  return tr("64-Channel inputs");    //混音器

    case CFG_TYPE_DYN_AUTO:     return tr("Automatic Gain Control");
    case CFG_TYPE_DYN_COMP:     return tr("Compressor");
    case CFG_TYPE_DYN_LIMIT:    return tr("Peak Limiter");
    case CFG_TYPE_DYN_NOISE:    return tr("Gate");
    case CFG_TYPE_DYN_EXPAND:   return tr("Expander");
    case CFG_TYPE_DYN_PEAK:     return tr("Peak");

    case CFG_TYPE_EQ_1:
    case CFG_TYPE_EQ_3:
    case CFG_TYPE_EQ_5:
    case CFG_TYPE_EQ_7:
    case CFG_TYPE_EQ_10:
    case CFG_TYPE_EQ_16:        return tr("Parametric Equalizer");         //均衡器
    case CFG_TYPE_EQ_1X3:
    case CFG_TYPE_EQ_2X3:
    case CFG_TYPE_EQ_3X3:       return tr("Graphic Equalizer");         //均衡器

    case CFG_TYPE_FILTE_LP:     return tr("Low-Pass Filter");    //滤波器
    case CFG_TYPE_FILTE_HP:     return tr("High-Pass Filter");    //滤波器
    case CFG_TYPE_FILTE_LS:
    case CFG_TYPE_FILTE_HS:     return tr("Specialty Filter");    //滤波器
    case CFG_TYPE_FILTER_BP:   return tr("Band-Pass Filter");    //滤波器
    case CFG_TYPE_FILTER_AP:   return tr("All-Pass Filter");

    case CFG_TYPE_FREQSHIFT:
    case CFG_TYPE_FREQSHIFT_PLUS:   return tr("Frequency Shifter");

    case CFG_TYPE_NFC1_8NF:
    case CFG_TYPE_NFC1_16NF:
    case CFG_TYPE_NFC1_24NF:
    case CFG_TYPE_NFC1_32NF:        return tr("Notch Feedback Controller");

    case CFG_TYPE_AUTOMIX_GS_4:
    case CFG_TYPE_AUTOMIX_GS_8:
    case CFG_TYPE_AUTOMIX_GS_16:
    case CFG_TYPE_AUTOMIX_GS_32:
    case CFG_TYPE_AUTOMIXNGAT_4:
    case CFG_TYPE_AUTOMIXNGAT_8:
    case CFG_TYPE_AUTOMIXNGAT_16:
    case CFG_TYPE_AUTOMIXNGAT_32:
    case CFG_TYPE_AUTOMIXNGRT_4:
    case CFG_TYPE_AUTOMIXNGRT_8:
    case CFG_TYPE_AUTOMIXNGRT_16:
    case CFG_TYPE_AUTOMIXNGRT_32:  return tr("Automatic Mic Mixer");

    case CFG_TYPE_SIG_SINE:      return tr("Sine");      //信号发生器
    case CFG_TYPE_SIG_NOISE:     return tr("White Noise");      //信号发生器
    case CFG_TYPE_SIG_PINKNOISE: return tr("Pink Noise");      //信号发生器

    case CFG_TYPE_METER_1: return tr("1Channel-Meter");
    case CFG_TYPE_METER_2: return tr("2Channel-Meter");
    case CFG_TYPE_METER_4: return tr("4Channel-Meter");
    case CFG_TYPE_METER_8: return tr("8Channel-Meter");    //仪表显示

    case CFG_TYPE_AEC_2_2: return tr("Acoustic Echo Canceler");

    case CFG_TYPE_SPLIT_2:
    case CFG_TYPE_SPLIT_3:
    case CFG_TYPE_SPLIT_4:      return tr("Crossover");    //分频器

    case CFG_TYPE_INPUT_1:
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
    case CFG_TYPE_OUTPUT_1:
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:   return tr("Channel");  //输入输出端口

    case CFG_TYPE_GPI:          return tr("Gpi");

    case CFG_TYPE_SOFTFIR_MONO256:
    case CFG_TYPE_SOFTFIR_MONO512:
    case CFG_TYPE_SOFTFIR_MONO1024:
    case CFG_TYPE_SOFTFIR_STEREO256:
    case CFG_TYPE_SOFTFIR_STEREO512:
    case CFG_TYPE_SOFTFIR_STEREO1024:
    case CFG_TYPE_SOFTFIR_4R256:
    case CFG_TYPE_SOFTFIR_4R512:
    case CFG_TYPE_SOFTFIR_4R1024:   return tr("FIR Filter");

    case CFG_TYPE_PDUCKER_4:
    case CFG_TYPE_PDUCKER_8:
    case CFG_TYPE_PDUCKER_16:
    case CFG_TYPE_PDUCKER_32:      return tr("Priority Ducker");

    case CFG_TYPE_GAINRAMP_1:
    case CFG_TYPE_GAINRAMP_2:
    case CFG_TYPE_GAINRAMP_4:
    case CFG_TYPE_GAINRAMP_8:
    case CFG_TYPE_GAINRAMP_16:
    case CFG_TYPE_GAINRAMP_32:     return tr("Gain Ramp");
    //Gain
    case CFG_TYPE_GAIN_1:
    case CFG_TYPE_GAIN_2:
    case CFG_TYPE_GAIN_4:
    case CFG_TYPE_GAIN_8:
    case CFG_TYPE_GAIN_16:
    case CFG_TYPE_GAIN_32: return tr("Gain");
    //Crossfader
    case CFG_TYPE_CROSSFADER_2X1:
    case CFG_TYPE_CROSSFADER_4X2:
    case CFG_TYPE_CROSSFADER_8X4:
    case CFG_TYPE_CROSSFADER_16X8:
    case CFG_TYPE_CROSSFADER_32X16: return tr("Crossfader");
    //AEC-Soft 主核
    case CFG_TYPE_SOFTAEC_8K64: return tr("AEC-8K64");
    case CFG_TYPE_SOFTAEC_8K96: return tr("AEC-8K96");
    case CFG_TYPE_SOFTAEC_8K128: return tr("AEC-8K128");
    case CFG_TYPE_SOFTAEC_8K160: return tr("AEC-8K160");
    case CFG_TYPE_SOFTAEC_8K192: return tr("AEC-8K192");
    case CFG_TYPE_SOFTAEC_8K224: return tr("AEC-8K224");
    case CFG_TYPE_SOFTAEC_8K256: return tr("AEC-8K256");
    case CFG_TYPE_SOFTAEC_16K64: return tr("AEC-16K64");
    case CFG_TYPE_SOFTAEC_16K96: return tr("AEC-16K96");
    case CFG_TYPE_SOFTAEC_16K128: return tr("AEC-16K128");
    case CFG_TYPE_SOFTAEC_16K160: return tr("AEC-16K160");
    case CFG_TYPE_SOFTAEC_16K192: return tr("AEC-16K192");
    case CFG_TYPE_SOFTAEC_16K224: return tr("AEC-16K224");
    case CFG_TYPE_SOFTAEC_16K256: return tr("AEC-16K256");
    //AEC-Soft 独立核
    case CFG_TYPE_SOFTAEC_IK_8K64: return tr("AEC-8K64IK");
    case CFG_TYPE_SOFTAEC_IK_8K96: return tr("AEC-8K96IK");
    case CFG_TYPE_SOFTAEC_IK_8K128: return tr("AEC-8K128IK");
    case CFG_TYPE_SOFTAEC_IK_8K160: return tr("AEC-8K160IK");
    case CFG_TYPE_SOFTAEC_IK_8K192: return tr("AEC-8K192IK");
    case CFG_TYPE_SOFTAEC_IK_8K224: return tr("AEC-8K224IK");
    case CFG_TYPE_SOFTAEC_IK_8K256: return tr("AEC-8K256IK");
    case CFG_TYPE_SOFTAEC_IK_16K64: return tr("AEC-16K64IK");
    case CFG_TYPE_SOFTAEC_IK_16K96: return tr("AEC-16K96IK");
    case CFG_TYPE_SOFTAEC_IK_16K128: return tr("AEC-16K128IK");
    case CFG_TYPE_SOFTAEC_IK_16K160: return tr("AEC-16K160IK");
    case CFG_TYPE_SOFTAEC_IK_16K192: return tr("AEC-16K192IK");
    case CFG_TYPE_SOFTAEC_IK_16K224: return tr("AEC-16K224IK");
    case CFG_TYPE_SOFTAEC_IK_16K256: return tr("AEC-16K256IK");
//    case CFG_TYPE_SOFTAEC_IK_48K64: return tr("AEC-48K64IK");
//    case CFG_TYPE_SOFTAEC_IK_48K96: return tr("AEC-48K96IK");
//    case CFG_TYPE_SOFTAEC_IK_48K128: return tr("AEC-48K128IK");
//    case CFG_TYPE_SOFTAEC_IK_48K160: return tr("AEC-48K160IK");
//    case CFG_TYPE_SOFTAEC_IK_48K192: return tr("AEC-48K192IK");
//    case CFG_TYPE_SOFTAEC_IK_48K256: return tr("AEC-48K256IK");
    case CFG_TYPE_AUDIO_PLAYER1:
    case CFG_TYPE_AUDIO_PLAYER2:
    case CFG_TYPE_AUDIO_PLAYER4:
    case CFG_TYPE_AUDIO_PLAYER8:
    case CFG_TYPE_AUDIO_PLAYER16:           return tr("Audio Player");
    case CFG_TYPE_AUDIO_RECORD1:
    case CFG_TYPE_AUDIO_RECORD2:
    case CFG_TYPE_AUDIO_RECORD4:            return tr("Audio Record");
    case CFG_TYPE_CTRL_LUA:          return tr("Lua Control");
    case CFG_TYPE_VOICETUBE_1:
    case CFG_TYPE_VOICETUBE_2:      return tr("Mic Control");

    case CFG_TYPE_SNAPSHOT: return tr("Snapshot");

    case CFG_TYPE_CTRL_GPI: return tr("GPI");
    case CFG_TYPE_CTRL_GPO: return tr("GPO");
    case CFG_TYPE_CTRL_AD: return tr("ADC");
    case CFG_TYPE_CTRL_RS232: return tr("RS232");
    case CFG_TYPE_CTRL_RS485: return tr("RS485");

    default: return QString("Unknow:%1").arg(type); //其它
    }
}



//Components中显示
QString Utils::getCfgGroupName(CfgType type)
{
    uint16_t cfgGroup = type & FCT_FAMILY_MASK;
    switch(cfgGroup) {
    case CFG_TYPE_DELAY_5:         return QString("Delay");     //0x0100延时器
    case CFG_TYPE_ROUTER_4:        return QString("Router");     //0x0200路由器
    case CFG_TYPE_MIXER_4X1:       return QString("Mixer");     //0x0300混音器
    case CFG_TYPE_DYN_AUTO:        return QString("Dyn");     //0x0400动态处理
    case CFG_TYPE_EQ_1:            return QString("Eq");      //0x0500均衡器
    case CFG_TYPE_FILTE_LP:        return QString("Specialty Filter");     //0x0600滤波器
    case CFG_TYPE_SPLIT_2:         return QString("Crossover");     //0x0700分频器
    case CFG_TYPE_SIG_SINE:        return QString("Signal");     //0x0800信号发生器
    case CFG_TYPE_METER_1:         return QString("Meter");     //0x0900仪表显示
    case CFG_TYPE_INPUT_1:        return QString("Channel");     //0x0A00输入端口
    case CFG_TYPE_OUTPUT_1:       return QString("Channel");     //0x0B00输出端口
    case CFG_TYPE_GPI:             return QString("Gpi");     //0x0C00GPI模块
    case CFG_TYPE_FREQSHIFT:       return QString("Freq Shift");     //0x0D00啸叫抑制
    case CFG_TYPE_AUTOMIX_GS_4:    return QString("Automatic Mic Mixers");     //0x0E00自动混音
    case CFG_TYPE_AEC_2_2:         return QString("AEC-NR");     //0x0F00
    case CFG_TYPE_SOFTFIR_MONO256: return QString("FIR Filter");     //0x1000
    case CFG_TYPE_NFC1_8NF:        return QString("NFC");     //0x1100
    case CFG_TYPE_PDUCKER_4:      return QString("Priority Ducker");     //0x1200
    case CFG_TYPE_GAINRAMP_1:     return QString("Gain Ramp");     //0x1300
    case CFG_TYPE_GAIN_1:         return QString("Gain");     //0x1400
    case CFG_TYPE_CROSSFADER_2X1: return QString("Crossfader");     //0x1500
    case CFG_TYPE_SOFTAEC_8K64:   return QString("AEC-Soft");     //0x1600
    case CFG_TYPE_SOFTAEC_IK_8K64:return QString("AEC-Soft");     //0x1700
    case CFG_TYPE_AUDIO_PLAYER1:   return QString("Audio Player");     //0xD000
    case CFG_TYPE_AUDIO_RECORD1:   return QString("Audio Record");     //0xD100
    case CFG_TYPE_CTRL_LUA:         return QString("Lua Control");     //0x4000
    case CFG_TYPE_VOICETUBE_1:     return QString("Mic Control");     //0xE000
    case CFG_TYPE_SNAPSHOT:        return QString("Snapshot");     //0xF000
    case CFG_TYPE_SNAPSHOT_BANK:   return QString("Snapshot");

    case CFG_TYPE_CTRL_GPI:         return tr("GPI");
    case CFG_TYPE_CTRL_GPO:         return tr("GPO");
    case CFG_TYPE_CTRL_AD:          return tr("ADC");
    case CFG_TYPE_CTRL_RS232:           return QString("RS232");
    case CFG_TYPE_CTRL_RS485:           return QString("RS485");
    default: return QString("Unknow2:%1").arg(type); //其它
    }
}

//模块显示第二行
QString Utils::getCfgShortName(CfgType type)
{
    uint16_t cfgGroup = type & FCT_FAMILY_MASK;
    switch(cfgGroup) {
    case CFG_TYPE_DELAY_5:         return QString("Dly");     //0x0100延时器
    case CFG_TYPE_ROUTER_4:        return QString("Rtr");     //0x0200路由器
    case CFG_TYPE_MIXER_4X1:       return QString("Mxr");     //0x0300混音器
    case CFG_TYPE_DYN_AUTO:        return QString("Dyn");     //0x0400动态处理
    case CFG_TYPE_EQ_1:            return QString("Eq");      //0x0500均衡器
    case CFG_TYPE_FILTE_LP:        return QString("Flt");     //0x0600滤波器
    case CFG_TYPE_SPLIT_2:         return QString("Spl");     //0x0700分频器
    case CFG_TYPE_SIG_SINE:        return QString("Sig");     //0x0800信号发生器
    case CFG_TYPE_METER_1:         return QString("Met");     //0x0900仪表显示
    case CFG_TYPE_INPUT_1:        return QString("Inp");     //0x0A00输入端口
    case CFG_TYPE_OUTPUT_1:       return QString("Out");     //0x0B00输出端口
    case CFG_TYPE_GPI:             return QString("Gpi");     //0x0C00GPI模块
    case CFG_TYPE_FREQSHIFT:       return QString("Frq");     //0x0D00啸叫抑制
    case CFG_TYPE_AUTOMIX_GS_4:    return QString("AMC");     //0x0E00自动混音
    case CFG_TYPE_AEC_2_2:         return QString("AEC");     //0x0F00
    case CFG_TYPE_SOFTFIR_MONO256: return QString("Fir");     //0x1000
    case CFG_TYPE_NFC1_8NF:        return QString("NFC");     //0x1100
    case CFG_TYPE_PDUCKER_4:      return QString("DCK");     //0x1200
    case CFG_TYPE_GAINRAMP_1:     return QString("GNR");     //0x1300
    case CFG_TYPE_GAIN_1:         return QString("GAN");     //0x1400
    case CFG_TYPE_CROSSFADER_2X1: return QString("CRF");     //0x1500
    case CFG_TYPE_SOFTAEC_8K64:   return QString("AEC");     //0x1600
    case CFG_TYPE_SOFTAEC_IK_8K64:return QString("AEC");     //0x1700
    case CFG_TYPE_AUDIO_PLAYER1:   return QString("Ply");     //0xD000
    case CFG_TYPE_AUDIO_RECORD1:   return QString("Rec");     //0xD100
    case CFG_TYPE_CTRL_LUA:         return QString("Lua");     //0x4000
    case CFG_TYPE_VOICETUBE_1:     return QString("Mic");     //0xE000
    case CFG_TYPE_SNAPSHOT:        return QString("Snp");     //0xF000
    case CFG_TYPE_SNAPSHOT_BANK:   return QString("Snp");
    case CFG_TYPE_CTRL_GPI:         return QString("GPI");
    case CFG_TYPE_CTRL_GPO:         return QString("GPO");
    case CFG_TYPE_CTRL_AD:          return QString("ADC");
    case CFG_TYPE_CTRL_RS232:           return QString("RS232");
    case CFG_TYPE_CTRL_RS485:           return QString("RS485");
    default: return QString("Unknow2:%1").arg(type); //其它
    }
}

Utils::devItemPorts_t Utils::getDeviceItemPortNum(DevType devType, uint8_t isUsing)
{
    switch(devType) {
    case Dev_Type_NAP_Zynq7020_16Analog_16AES67:
    {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_BeiJingHuiSheng)
            return devItemPorts_t(isUsing, {16,16,0,0}, {16,16,0,0});
//        else if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
//            return devItemPorts_t(isUsing, {16,48,0,0}, {16,48,0,0});
        else
            return devItemPorts_t(isUsing, {16,32,0,0}, {16,32,0,0});
    }
    case Dev_Type_NAP_Zynq7020_16Analog_48AES67:
    {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FTYX) //富通亚讯 显示64个网络接口，后面16个没有效果
            return devItemPorts_t(isUsing, {16,64,0,0},  {16,64,0,0});
        else
            return devItemPorts_t(isUsing, {16,48,0,0},  {16,48,0,0});
    }
    case Dev_Type_NAP_Zynq7020_64AES67:                    return devItemPorts_t(isUsing, {0, 64,0,0},  {0, 64,0,0});
#ifdef  DanteSupported
    case Dev_Type_NAP_Zynq7020_16Analog_16AES67DanteMixer: return devItemPorts_t(isUsing, {16,16,0,0},  {16,16,0,0});
    case Dev_Type_NAP_Zynq7020_16Analog_16Dante:
    case Dev_Type_NAP_Zynq7020_16Analog_16Dante_Type1:     return devItemPorts_t(isUsing, {16,0,16,0},  {16,0,16,0});
    case Dev_Type_NAP_Zynq7020_16Analog_32Dante:           return devItemPorts_t(isUsing, {16,0,32,0},  {16,0,32,0});
#endif
    case Dev_Type_NAP_X86_16Analog_128AES67:               return devItemPorts_t(isUsing, {16,128,0,2}, {16,128,0,2});
    case Dev_Type_NAP_X86_32Analog_256AES67:               return devItemPorts_t(isUsing, {32,256,0,2}, {32,256,0,2});
    case Dev_Type_NAP_FT2000_256AES67:                     return devItemPorts_t(isUsing, {0,256,0,0}, {0,256,0,0});
    case Dev_Type_Mixer_128_128_FangTu:              return devItemPorts_t(isUsing, {0,128,0,0}, {0,128,0,0});
    case Dev_Type_Mixer_256_256_FangTu:              return devItemPorts_t(isUsing, {32,224,0,0}, {32,224,0,0});
    case Dev_Type_NAP_STM32H750_8A8N:         return devItemPorts_t(isUsing, {8,16,0,0}, {8,16,0,0});
    case Dev_Type_NAP_150_FangTu:                    return devItemPorts_t(isUsing, {16,64,0,0}, {16,64,0,0});

    case Dev_Type_VirtualSoundCard_Dante:    return devItemPorts_t(isUsing, {0,0,16,0}, {0,0,16,0});
    case Dev_Type_VirtualSoundCard_AES67:    return devItemPorts_t(isUsing, {0,16,0,0}, {0,16,0,0});
    case Dev_Type_Zynq7010_AES3ToAES67_DongFangJiaLian:  return devItemPorts_t(isUsing, {0,0,0,0}, {0,8,0,0});
    case Dev_Type_RT1052_DR3020:                         return devItemPorts_t(isUsing, {0,4,0,0}, {0,4,0,0});
    case Dev_Type_RT1052_Amplifier_DongFangJiaLian:      return devItemPorts_t(isUsing, {0,8,0,0}, {0,0,0,0});

    case Dev_Type_STM32_POE_AES67_SPK_0_1:   return devItemPorts_t(isUsing, {0,1,0,0}, {0,0,0,0});
    case Dev_Type_STM32_POE_AES67_SPK_0_2:   return devItemPorts_t(isUsing, {0,2,0,0}, {0,0,0,0});
    case Dev_Type_DNTE_AMP:                     return devItemPorts_t(isUsing, {0,0,2,0}, {2,0,0,0});
    case Dev_Type_STM32_POE_AES67_MIC_2_0:   return devItemPorts_t(isUsing, {0,0,0,0}, {0,1,0,0});
    case Dev_Type_RT1052_POE_AES67_MIC_4_0:
    {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
            return devItemPorts_t(isUsing, {4,0,0,0}, {0,4,0,0});
        else
            return devItemPorts_t(isUsing, {0,0,0,0}, {0,4,0,0});
    }
    case Dev_Type_RT1052_POE_AES67_LINE_4_4: return devItemPorts_t(isUsing, {4,4,0,0}, {4,4,0,0});
    case Dev_Type_RT1052_POE_AES67_MIC_2_0:
    case Dev_Type_RT1052_POE_AES67_MIC_2_0_Type1:
    case Dev_Type_RT1052_POE_AES67_MIC_2_0_Type2:
    case Dev_Type_RT1052_POE_AES67_MIC_2_0_Type3:
    {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
            return devItemPorts_t(isUsing, {2,0,0,0}, {0,2,0,0});
        else
            return devItemPorts_t(isUsing, {0,0,0,0}, {0,2,0,0});
    }
    case Dev_Type_RT1052_POE_AES67_MIC_8_0:
    {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
            return devItemPorts_t(isUsing, {8,0,0,0}, {0,8,0,0});
        else
            return devItemPorts_t(isUsing, {0,0,0,0}, {0,8,0,0});
    }
    case Dev_Type_RT1052_POE_AES67_LINE_0_2:
    case Dev_Type_AAF_CS_6_FangTu:
    case Dev_Type_AAF_CS_8_FangTu:
    case Dev_Type_AAF_CS_10_FangTu:
    case Dev_Type_AAF_CS_12_FangTu:
    case Dev_Type_AAF_CS_15_FangTu:
    case Dev_Type_AAF_CS_18_FangTu:
    case Dev_Type_DBN_4u_FangTu:
    case Dev_Type_DBN_8_10u_FangTu:
    {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
            return devItemPorts_t(isUsing, {0,2,0,0}, {2,0,0,0});
        else
            return devItemPorts_t(isUsing, {0,2,0,0}, {0,0,0,0});
    }
    case Dev_Type_RT1052_POE_AES67_LINE_0_8:
    {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
            return devItemPorts_t(isUsing, {0,8,0,0}, {8,0,0,0});
        else
            return devItemPorts_t(isUsing, {0,8,0,0}, {0,0,0,0});
    }

    case Dev_Type_RT1052_AP60:
    case Dev_Type_RT1052_AP60_Type1:
    case Dev_Type_RT1052_AP60_Type2:
    case Dev_Type_CS50_FangTu:
    case Dev_Type_CS80_FangTu:           return devItemPorts_t(isUsing, {0,1,0,0}, {0,1,0,0});
    case Dev_Type_CS50A_FangTu:
    case Dev_Type_CS60A_FangTu:
    case Dev_Type_CS80A_FangTu:          return devItemPorts_t(isUsing, {1,1,0,0}, {0,1,0,0});
    case Dev_Type_RT1052_AP60_Bluetooth: return devItemPorts_t(isUsing, {1,1,0,0}, {0,0,0,0});

    case Dev_Type_RT1052_AMPLIFIER_AA4500:
    case Dev_Type_RT1052_AMPLIFIER_AA4500_Type1:
    case Dev_Type_RT1052_AMPLIFIER_AA2500:
    case Dev_Type_FT_4D300N_FangTu:
    case Dev_Type_FT_4D600N_FangTu:
    case Dev_Type_FT_4D800N_FangTu:
    case Dev_Type_FT_4A1200N_FangTu:
    case Dev_Type_FT_4A2000N_FangTu:      return devItemPorts_t(isUsing, {4,4,0,0}, {0,4,0,0});

    case Dev_Type_RT1052_AP400:
    case Dev_Type_Dir_L445_FangTu:
    case Dev_Type_Dir_X_10_12_16_FangTu:  return devItemPorts_t(isUsing, {1,1,0,0}, {0,1,0,0});
    case Dev_Type_AES67_MONITOR_SPK:         return devItemPorts_t(isUsing, {4,4,0,0}, {4,4,0,0});
//    case DEV_TYPE_DNTE_SPK:
//    case DEV_TYPE_AES67_SPK:

    case Dev_Type_FionTu_WMIC_FangTu:
    case Dev_Type_I_Mic_Center_DN_FangTu: return devItemPorts_t(isUsing, {0,0,0,0}, {0,4,0,0});
//    case Dev_Type_Switch_16x32_FangTu:
//    case Dev_Type_Switch_POE_16x32_FangTu: return devItemPorts_t(isUsing, {0,16,0,0}, {0,32,0,0});
    case Dev_Type_Room: return devItemPorts_t();
    default:
        qDebug() << __FUNCTION__ << __LINE__ << "Unknown type, Unable to get Port Number!";
        return devItemPorts_t();
    }
}


int Utils::getDevTatalPortNum(DevType devType)
{
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FTYX && devType == Dev_Type_NAP_Zynq7020_16Analog_48AES67)
    {   //富通亚讯设备显示64路网络端口，实际只有48路
        return 64;
    }

    devItemPorts_t portDef = getDeviceItemPortNum(devType);
    int totolPortNum = portDef.s_In.s_ANum + portDef.s_In.s_DNum + portDef.s_In.s_NNum + portDef.s_In.s_UNum;
    return totolPortNum;
}


bool Utils::RegCheck_IsIp(QString ipStr)
{
    QRegExp rx("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    if(!rx.exactMatch(ipStr)) {
        Utils::MessageBoxWarning(tr("warning"), tr("slave gateway not valid"));
        return false;
    }
    else {
        return true;
    }
}


#include "customWidget/overwriteitem.h"
QString Utils::getNewName(QString oldName)
{
    static QLineEdit* txtNewName = NULL;
    static QString newName;
    DevSettingDlg* newDlg = new DevSettingDlg();
    newDlg->setWindowFlags(Qt::FramelessWindowHint);
    QVBoxLayout* mainLayout = new QVBoxLayout(newDlg);
    int lblFixWidth = 80;
    int txtFixWidth = 150;
    int btnFixWidth = 60;
    //根据需要显示oldName
    if(!oldName.isEmpty()){
        QHBoxLayout* originLayout = new QHBoxLayout(newDlg);
        QLabel* originlbl = new QLabel(tr("Old Name"));
        originlbl->setFixedWidth(lblFixWidth);
        originLayout->addWidget(originlbl);
        QLineEdit* originPorjName = new QLineEdit(newDlg);
        originPorjName->setFixedWidth(txtFixWidth);
        originPorjName->setText(oldName);
        originPorjName->setEnabled(false);
        originLayout->addWidget(originPorjName);
        mainLayout->addLayout(originLayout);
    }

    //显示新名称
    QHBoxLayout* inputLayout = new QHBoxLayout(newDlg);
    QLabel* lbl = new QLabel(tr("New Name"));
    lbl->setFixedWidth(lblFixWidth);
    inputLayout->addWidget(lbl);
    txtNewName = new QLineEdit(newDlg);
    txtNewName->setFixedWidth(txtFixWidth);
    inputLayout->addWidget(txtNewName);
    mainLayout->addLayout(inputLayout);

    //显示按钮
    QHBoxLayout* actionLayout = new QHBoxLayout(newDlg);
    QPushButton* confirmButton = new QPushButton(newDlg);
    QPushButton* cancelButton = new QPushButton(newDlg);
    actionLayout->setMargin(10);
    confirmButton->setText(tr("Ok"));
    confirmButton->setFixedWidth(btnFixWidth);
    cancelButton->setText(tr("Cancel"));
    cancelButton->setFixedWidth(btnFixWidth);
    actionLayout->addWidget(confirmButton);
    actionLayout->addWidget(cancelButton);
    mainLayout->addLayout(actionLayout);

    connect(confirmButton, &QPushButton::clicked, [newDlg]() {
        if (!txtNewName->text().isEmpty()) {
            newName = txtNewName->text();
            newDlg->close();
            delete newDlg;
        }
    });

    connect(cancelButton, &QPushButton::clicked, [newDlg]() {
       newName = QString();
       newDlg->close();
       delete newDlg;
    });

    newDlg->exec();
    return newName;
}

#include "Frame/mainwindow.h"
#include "linelabel.h"
void Utils::saveRoomInputOutputForSolutionFile(QString roomName, QMap<QString, LineLabel *> inLineLab,
                                               QMap<QString, LineLabel *> outLineLab)
{
    QString slnPath = MainWindow::instance()->getSolutionPath();
    QSettings *SolutionIni = new QSettings(slnPath, QSettings::IniFormat, 0);
    SolutionIni->beginGroup(PROJECT_GROUP);
    SolutionIni->beginGroup(QString(PROJECT_INFOMATION_GROUP));
    quint32 devCount = SolutionIni->value(QString(PROJECT_DEVICE_COUNT)).toUInt();
    SolutionIni->endGroup();

    bool isFind = false;
    for(quint32 i = 0; i < devCount; i++) {
        SolutionIni->beginGroup(QString(PROJECT_DEVICE_GROUP).arg(i));
        QString rName = SolutionIni->value(QString(PROJECT_DEVICE_DEVNAME)).toString();
//        qDebug() << __FUNCTION__ << __LINE__ << roomName << rName;
        if(QString::compare(roomName, rName)) {
            SolutionIni->endGroup();
            continue;
        }
        isFind = true;
        quint32 count = SolutionIni->value(QString(SOLUTION_ROOM_INPUT_COUNT)).toUInt();
        for(quint32 i = 0; i < count; i++) {//clear in out
            SolutionIni->remove(QString(SOLUTION_ROOM_INPUT_GROUP).arg(i));
            SolutionIni->remove(QString(SOLUTION_ROOM_OUTPUT_GROUP).arg(i));
        }

        SolutionIni->setValue(QString(SOLUTION_ROOM_INPUT_COUNT), inLineLab.count());
        SolutionIni->setValue(QString(SOLUTION_ROOM_OUTPUT_COUNT), outLineLab.count());

        int idx = 0;
        QMapIterator<QString, LineLabel *> inIter(inLineLab);
        while(inIter.hasNext()) {
            inIter.next();
            SolutionIni->beginGroup(QString(SOLUTION_ROOM_INPUT_GROUP).arg(idx++));
            SolutionIni->setValue(QString(SOLUTION_ROOM_PORT_NAME), inIter.value()->name());
            SolutionIni->setValue(QString(SOLUTION_ROOM_PORT_TYPE), inIter.value()->getPortType());
            SolutionIni->endGroup();
        }

        idx = 0;
        QMapIterator<QString, LineLabel *> outIter(outLineLab);
        while(outIter.hasNext()) {
            outIter.next();
            SolutionIni->beginGroup(QString(SOLUTION_ROOM_OUTPUT_GROUP).arg(idx++));
            SolutionIni->setValue(QString(SOLUTION_ROOM_PORT_NAME), outIter.value()->name());
            SolutionIni->setValue(QString(SOLUTION_ROOM_PORT_TYPE), outIter.value()->getPortType());
            SolutionIni->endGroup();
        }
        SolutionIni->endGroup();
    }
    if(!isFind) {
        SolutionIni->beginGroup(QString(PROJECT_DEVICE_GROUP).arg(devCount++));
        SolutionIni->setValue(QString(PROJECT_DEVICE_DEVNAME), roomName);
        int idx = 0;
        foreach (LineLabel *lLabItem, inLineLab) {
            SolutionIni->beginGroup(QString(SOLUTION_ROOM_INPUT_GROUP).arg(idx++));
            SolutionIni->setValue(QString(SOLUTION_ROOM_PORT_NAME), lLabItem->name());
            SolutionIni->setValue(QString(SOLUTION_ROOM_PORT_TYPE), lLabItem->getPortType());
            SolutionIni->endGroup();
        }
        idx = 0;
        foreach (LineLabel *lLabItem, outLineLab) {
            SolutionIni->beginGroup(QString(SOLUTION_ROOM_OUTPUT_GROUP).arg(idx++));
            SolutionIni->setValue(QString(SOLUTION_ROOM_PORT_NAME), lLabItem->name());
            SolutionIni->setValue(QString(SOLUTION_ROOM_PORT_TYPE), lLabItem->getPortType());
            SolutionIni->endGroup();
        }
        SolutionIni->setValue(QString(SOLUTION_ROOM_INPUT_COUNT), inLineLab.count());
        SolutionIni->setValue(QString(SOLUTION_ROOM_OUTPUT_COUNT), outLineLab.count());
        SolutionIni->endGroup();

        SolutionIni->beginGroup(QString(PROJECT_INFOMATION_GROUP));
        SolutionIni->setValue(QString(PROJECT_DEVICE_COUNT), devCount);
        SolutionIni->endGroup();
    }


    delete SolutionIni;
}

QList<QString> Utils::insertionSort(QList<QString>& list)
{
    QCollator cltr;
    cltr.setNumericMode(true);
    for(int i = 1; i < list.size(); i++) {
        QString key = list[i];
        int j = i - 1;
        while(j >= 0 && cltr.compare(list[j], key) > 0) {
            list[j+1] = list.at(j);
            j--;
        }
//        if(j < 0) continue;
        list[j+1] = key;
    }
    return list;
}

void Utils::setMaxMasterVersion(CfgType type)
{
    if((type & FCT_FAMILY_MASK) < 0x1600) {
        quint8 version[4] = {2, 0, 2, 0};
        for(int i = 0; i < sizeof(maxMasterVersion); i++)
            maxMasterVersion[i] = maxMasterVersion[i] > version[i] ? maxMasterVersion[i] : version[i];
        return ;
    }
    switch (type & FCT_FAMILY_MASK) {
    case CFG_TYPE_SOFTAEC_8K64:
    case CFG_TYPE_SOFTAEC_IK_8K64:
    {
        quint8 version[4] = {3, 0, 1, 0};
        for(int i = 0; i < sizeof(maxMasterVersion); i++)
            maxMasterVersion[i] = maxMasterVersion[i] > version[i] ? maxMasterVersion[i] : version[i];
        break;
    }
    default:
        break;
    }
}

void Utils::saveProjectExplorer(QString RecentProject)
{
    QString appDataPath = QFileInfo(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first()).absolutePath();
    QDir dir(appDataPath + "/" + qApp->applicationName());
    if(!dir.exists())
        dir.mkdir(dir.absolutePath());
    QString filePath = dir.absolutePath() + "/Designer.ini";
    QSettings *settings = new QSettings(filePath, QSettings::IniFormat, 0);
    settings->beginGroup(APPDATA_PROJECTEXPLORER);
    settings->beginGroup(APPDATA_RECENTPROJECTS);
    QStringList FileNames = settings->value(APPDATA_FILENAMES).toStringList();
    if(!FileNames.contains(RecentProject))
        FileNames.append(RecentProject);
    settings->setValue(APPDATA_FILENAMES, FileNames);
    settings->endGroup();
    settings->endGroup();
}

void Utils::loadProjectExplorer(QStringList &RecentProjects)
{
    QString appDataPath = QFileInfo(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first()).absolutePath();
    QDir dir(appDataPath + "/" + qApp->applicationName());
    if(!dir.exists())
        dir.mkdir(dir.absolutePath());
    QString filePath = dir.absolutePath() + "/Designer.ini";
    QSettings *settings = new QSettings(filePath, QSettings::IniFormat, 0);
    settings->beginGroup(APPDATA_PROJECTEXPLORER);
    settings->beginGroup(APPDATA_RECENTPROJECTS);
    RecentProjects = settings->value(APPDATA_FILENAMES).toStringList();
    settings->endGroup();
    settings->endGroup();
}

void Utils::removeProjectExplorer(QString RecentProject)
{
    QString appDataPath = QFileInfo(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first()).absolutePath();
    QDir dir(appDataPath + "/" + qApp->applicationName());
    if(!dir.exists())
        dir.mkdir(dir.absolutePath());
    QString filePath = dir.absolutePath() + "/Designer.ini";
    QSettings *settings = new QSettings(filePath, QSettings::IniFormat, 0);
    settings->beginGroup(APPDATA_PROJECTEXPLORER);
    settings->beginGroup(APPDATA_RECENTPROJECTS);
    QStringList FileNames = settings->value(APPDATA_FILENAMES).toStringList();
    if(FileNames.contains(RecentProject))
        FileNames.removeAll(RecentProject);
    settings->setValue(APPDATA_FILENAMES, FileNames);
    settings->endGroup();
    settings->endGroup();
}

void Utils::saveDefaultWorkingPath(QString defPath)
{
    QString appDataPath = QFileInfo(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first()).absolutePath();
    QDir dir(appDataPath + "/" + qApp->applicationName());
    if(!dir.exists())
        dir.mkdir(dir.absolutePath());
    QString filePath = dir.absolutePath() + "/Designer.ini";
    QSettings *settings = new QSettings(filePath, QSettings::IniFormat, 0);
    settings->beginGroup(APPDATA_WORINGPATH);
    settings->setValue(APPDATA_DEFAULTWORKINGPATH, defPath);
    settings->endGroup();
    delete settings;
}

void Utils::loadDefaultWorkingPath(QString &defPath)
{
    QString appDataPath = QFileInfo(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first()).absolutePath();
    QDir dir(appDataPath + "/" + qApp->applicationName());
    if(!dir.exists())
        dir.mkdir(dir.absolutePath());
    QString filePath = dir.absolutePath() + "/Designer.ini";
    QSettings *settings = new QSettings(filePath, QSettings::IniFormat, 0);
    settings->beginGroup(APPDATA_WORINGPATH);
    defPath = settings->value(APPDATA_DEFAULTWORKINGPATH).toString();
    if(defPath.isEmpty()) {
        defPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        defPath += "/" + qApp->applicationName();
        QDir docDir(defPath);
        if(!docDir.exists())
            docDir.mkdir(docDir.absolutePath());
        settings->setValue(APPDATA_DEFAULTWORKINGPATH, defPath);
    }
    settings->endGroup();
    delete settings;
}

void Utils::saveCurOpenPagesData(const QStringList &pageNames, const QString& activeWinName)
{
    QString path = MainWindow::instance()->getSolutionPath();
    QSettings *settings = new QSettings(path, QSettings::IniFormat, 0);
    settings->beginGroup(QString(SOLUTION_DATA_GROUP));
    settings->beginGroup(QString(SOLUTION_DATA_OPENPAGES));
    settings->setValue(QString(SOLUTION_DATA_CUROPENPAGES), pageNames);
    settings->setValue(QString(SOLUTION_DATA_ACTIVEWINNAME), activeWinName);
    settings->endGroup();
    settings->endGroup();
}

void Utils::loadCurOpenPagesData(QStringList &pageNames, QString& activeWinName)
{
    QString path = MainWindow::instance()->getSolutionPath();
    QSettings *settings = new QSettings(path, QSettings::IniFormat, 0);
    settings->beginGroup(QString(SOLUTION_DATA_GROUP));
    settings->beginGroup(QString(SOLUTION_DATA_OPENPAGES));
    pageNames     = settings->value(QString(SOLUTION_DATA_CUROPENPAGES)).toStringList();
    activeWinName = settings->value(QString(SOLUTION_DATA_ACTIVEWINNAME)).toString();
    settings->endGroup();
    settings->endGroup();
}

QString Utils::getSoftwareVersion()
{
    SoftwareVersion_t swVer;
    MfrsType curMfrs = Config::curMfrsCfg().CurMfrs;

    switch (curMfrs) {
    case Mfrs_FangTu: swVer = {'V', 3, 0, 0, 0}; break;
    case Mfrs_KEMU: break;
    default:
        break;
    }

    return swVer.structToString();
}
