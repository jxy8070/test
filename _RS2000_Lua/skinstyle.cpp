#include "skinstyle.h"
#include "mfrscfg.h"
#include "deviceItem/deviceItem.h"
#include <QFontDatabase>

SkinStyle::SkinStyle(SkinType skinType)
{
    _skinType = skinType;
    if(_skinType == Skin_FangTu) {
        SceneFillColor    = QColor(78, 82, 92);
        OnlineTextColor   = QColor(69, 153, 99);
        DevItemTitilColor = Qt::white;
        VolumnLableColor  = Qt::white;
    }
    else if(_skinType == Skin_KemuGrey){
        SceneFillColor = QColor(161, 161, 161);
        OnlineTextColor = QColor(39, 242, 147);
        LineColor = QColor(20, 25, 30);
        DevItemTitilColor = Qt::white;
        VolumnLableColor = Qt::white;
    }
    else if(_skinType == Skin_KemuBlack){
        SceneFillColor = QColor(69, 73, 83);
        OnlineTextColor = Qt::green;
        LineColor = QColor(180, 185, 190);
        DevItemTitilColor = Qt::white;
        VolumnLableColor = Qt::white;
    }
    else if(_skinType == Skin_KemuWhite){
        SceneFillColor = QColor(240, 245, 250);
        OnlineTextColor = Qt::blue;
        LineColor = QColor(40, 45, 50);
        DevItemTitilColor = Qt::black;
        VolumnLableColor = Qt::black;
    }
    else if(_skinType == Skin_Default){
        SceneFillColor = QColor(240, 245, 250);
        OnlineTextColor = Qt::blue;
        LineColor = QColor(40, 45, 50);
        DevItemTitilColor = Qt::black;
        VolumnLableColor = Qt::black;
    }
    else{
        SceneFillColor = QColor(255, 0, 0);
    }
}

//算法模块颜色
const QColor SkinStyle::getConfigItemColor(CfgType type)
{
    if(_skinType == SkinType::Skin_FangTu) {
        return getConfigItemColor_FangTu(type);
    }
//    else if(_CurSkinType == SkinType::Skin_KemuBlack){
//        return getConfigItemColorKemuBlack(type);
//    }
    else{
        return getConfigItemColor_Default(type);
    }
}


QString SkinStyle::getSkinIconDir()
{
    switch (_skinType) {
    case Skin_FangTu: return "Fangtu"; break;
    case Skin_KemuGrey: return "KemuGrey"; break;
    case Skin_KemuWhite: return "KemuWhite"; break;
    case Skin_KemuBlack: return "KemuBlack"; break;
    default: return "Default"; break;
    }
}


QString SkinStyle::getSkinName(SkinType skinType)
{
    switch (skinType) {
    case Skin_FangTu: return tr("FangTu(black)"); break;
    case Skin_KemuGrey: return tr("Grey"); break;
    case Skin_KemuWhite: return tr("White"); break;
    case Skin_KemuBlack: return tr("Black"); break;
    case Skin_Default: return tr("Default(White)"); break;
    }
}


QString SkinStyle::getAppStyleSheet()
{
    QString styleFile;
    if(Config::curSkinType() == Skin_FangTu){
        styleFile = ":/WidgetStyle/AppStyle_FangTu.css";
    }
    else if(Config::curSkinType() == Skin_KemuGrey){
        styleFile = ":/WidgetStyle/AppStyle_KemuGrey.css";
    }
    else if(Config::curSkinType() == Skin_KemuBlack){
        styleFile = ":/WidgetStyle/AppStyle_KemuBlack.css";
    }
    else if(Config::curSkinType() == Skin_KemuWhite){
        styleFile = ":/WidgetStyle/AppStyle_KemuWhite.css";
    }
    else if(Config::curSkinType() == Skin_Default){
        styleFile = ":/WidgetStyle/AppStyle_Default.css";
    }
    QString style;
    QFile file(styleFile);
    if (file.open(QFile::ReadOnly)) {
        style = QLatin1String(file.readAll());
        file.close();
    }
    return style;
}


QString SkinStyle::getCfgSettingStyleSheet()
{
    static QString style;
    static SkinType lastSkinType = Skin_None;
    if(Config::curSkinType() == lastSkinType && !style.isEmpty()){
        return style;
    }
    QString styleFile;
    if(Config::curSkinType() == Skin_FangTu){
        styleFile = ":/WidgetStyle/WidgetStyle_FangTu.css";
    }
    else if(Config::curSkinType() == Skin_KemuGrey){
        styleFile = ":/WidgetStyle/WidgetStyle_KemuGrey.css";
    }
    else if(Config::curSkinType() == Skin_KemuBlack){
        styleFile = ":/WidgetStyle/WidgetStyle_KemuBlack.css";
    }
    else if(Config::curSkinType() == Skin_KemuWhite){
        styleFile = ":/WidgetStyle/WidgetStyle_KemuWhite.css";
    }
    else if(Config::curSkinType() == Skin_Default){
        styleFile = ":/WidgetStyle/WidgetStyle_Default.css";
    }
    QFile file(styleFile);
    if (file.open(QFile::ReadOnly)) {
        style = QLatin1String(file.readAll());
        lastSkinType = Config::curSkinType();
        file.close();
    }else {
        qDebug() << __FUNCTION__ << __LINE__ << "Open the failure!";
    }
    return style;
}


void SkinStyle::loadAppTranslator(QApplication& app)
{
    QString qmName;
    LanguageEnum curLang = Config::instance()->getLanguage();
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
    {
        if (curLang == LANGUAGE_ENGLISH) {
            qmName = "FangTu_English.qm";
        }
        else {
            qmName = "FangTu_SimpliedChinese.qm";
        }
    }
    else{
        if (curLang == LANGUAGE_ENGLISH) {
            qmName = "English.qm";
        }
        else {
            qmName = "SimpliedChinese.qm";
        }
    }

    static QTranslator qtTranslator;
    bool isLoad = qtTranslator.load(qmName, QApplication::applicationDirPath() + "./language");
    if(isLoad)
        app.installTranslator(&qtTranslator);
}


void SkinStyle::loadAppFont()
{
    if(Config::curSkinType() == Skin_FangTu){
        int nfontId = QFontDatabase::addApplicationFont("config/Font_FangTu-2.ttf");
//        qDebug()<<"fontId = "<< nfontId << QApplication::applicationDirPath() + "/config/Font_FangTu.otf";//等于-1加载失败
        if(nfontId>=0) {
          QString msyh = QFontDatabase::applicationFontFamilies(nfontId).at(0);
          QFont font(msyh);
          font.setPointSize(8);
          QApplication::setFont(font);
        }
    }
    else{
        QFont font("Microsoft YaHei");
        font.setPointSize(9);
        QApplication::setFont(font);
    }
}


const QColor SkinStyle::getConfigItemColor_Default(CfgType type)
{
    quint16 cfgGroup = type & FCT_FAMILY_MASK;
    int alpha = 0xFF;
    switch(cfgGroup) {
    case CFG_TYPE_DELAY_5:          return QColor( 50, 160, 190, alpha);
    case CFG_TYPE_ROUTER_4:         return QColor( 60, 190, 100, alpha);
    case CFG_TYPE_MIXER_4X1:        return QColor( 70, 190, 150, alpha);
    case CFG_TYPE_DYN_AUTO:         return QColor( 70, 190, 190, alpha);
    case CFG_TYPE_PDUCKER_4:        return QColor( 70, 190, 190, alpha);
    case CFG_TYPE_EQ_1:             return QColor( 80, 200, 130, alpha);
    case CFG_TYPE_FILTE_LP:         return QColor( 90, 160, 140, alpha);
    case CFG_TYPE_SPLIT_2:          return QColor( 90, 170, 160, alpha);
    case CFG_TYPE_SIG_SINE:         return QColor(100, 200, 200, alpha);
    case CFG_TYPE_METER_1:          return QColor(100, 120, 180, alpha);
    case CFG_TYPE_INPUT_1:          return QColor(110, 150, 150, alpha);
    case CFG_TYPE_OUTPUT_1:         return QColor(110, 150, 150, alpha);
    case CFG_TYPE_GPI:              return QColor(150, 180,  70, alpha);
    case CFG_TYPE_FREQSHIFT:        return QColor(160,  80,  70, alpha);
    case CFG_TYPE_AUTOMIX_GS_4:     return QColor(160, 160,  90, alpha);
    case CFG_TYPE_AEC_2_2:          return QColor(170, 120,  70, alpha);
    case CFG_TYPE_SOFTAEC_8K64:     return QColor(170, 120,  70, alpha);
    case CFG_TYPE_SOFTAEC_IK_8K64:  return QColor(170, 120,  70, alpha);
    case CFG_TYPE_SOFTFIR_MONO256:  return QColor(180,  70,  70, alpha);
    case CFG_TYPE_NFC1_8NF:         return QColor(180,  70, 180, alpha);
    case CFG_TYPE_GAINRAMP_1:       return QColor(180,  80, 120, alpha);
    case CFG_TYPE_GAIN_1:           return QColor(180,  90, 120, alpha);
    case CFG_TYPE_CROSSFADER_2X1:   return QColor(180, 100, 120, alpha);
    case CFG_TYPE_AUDIO_PLAYER1:    return QColor(120, 170, 190, alpha);
    case CFG_TYPE_AUDIO_RECORD1:    return QColor(130, 200, 180, alpha);
    case CFG_TYPE_CTRL_LUA:         return QColor(140, 180, 180, alpha);
    case CFG_TYPE_VOICETUBE_1:      return QColor(180,  80, 130, alpha);
    case CFG_TYPE_SNAPSHOT:         return QColor(180,  80, 180, alpha);
    default:
        qDebug() << __FUNCTION__ << __LINE__ << cfgGroup;
        return QColor(150, 150, 150, 255);
    }
}


const QColor SkinStyle::getConfigItemColor_FangTu(CfgType type)
{
//    quint16 cfgGroup = type & 0xFFFF;
//       const uint8_t alpha[21] = {
//           /* 100% */0xFF, 0xF2, 0xE6, 0xD9, 0xCC, 0xBF, 0xB3, 0xA6, 0x99, 0x8C,
//           /*  50% */0x80, 0x73, 0x66, 0x59, 0x4D, 0x40, 0x33, 0x26, 0x1A, 0x0D,
//           /*   0% */0x00
//       };
//       uint8_t alpha_offset = (type & 0xF) + ((type >> 4) & 0xF);
       switch(type) {
       case CFG_TYPE_DELAY_1000:       return QColor(223, 213, 129);
       case CFG_TYPE_DELAY_100:       return QColor(215,202,99);
       case CFG_TYPE_HPDELY_1:        return QColor(217, 200,  57);
       case CFG_TYPE_DYN_AUTO:         return QColor( 81, 184, 104);
       case CFG_TYPE_DYN_COMP:         return QColor(104, 193, 124);
       case CFG_TYPE_DYN_EXPAND:       return QColor(137, 201, 151);
       case CFG_TYPE_DYN_NOISE:        return QColor(172, 213, 152);
       case CFG_TYPE_DYN_LIMIT:        return QColor(204, 225, 152);
       case CFG_TYPE_PDUCKER_4:
       case CFG_TYPE_PDUCKER_8:       return QColor(224, 241, 181);
       case CFG_TYPE_EQ_3:
       case CFG_TYPE_EQ_5:
       case CFG_TYPE_EQ_10:
       case CFG_TYPE_EQ_16:            return QColor(132,  86, 128);
       case CFG_TYPE_EQ_1X3:
       case CFG_TYPE_EQ_2X3:
       case CFG_TYPE_EQ_3X3:           return QColor(152,  81, 146);
       case CFG_TYPE_SPLIT_2:
       case CFG_TYPE_SPLIT_3:
       case CFG_TYPE_SPLIT_4:          return QColor(175,  98, 168);
       case CFG_TYPE_FILTE_HP:         return QColor(185, 121, 179);
       case CFG_TYPE_FILTE_LP:         return QColor(196, 144, 191);
       case CFG_TYPE_FILTER_BP:       return QColor(241, 158, 194);
       case CFG_TYPE_FILTER_AP:       return QColor(244, 188, 212);
       case CFG_TYPE_SOFTFIR_MONO256:
       case CFG_TYPE_SOFTFIR_MONO512:
       case CFG_TYPE_SOFTFIR_MONO1024:
       case CFG_TYPE_SOFTFIR_STEREO256:
       case CFG_TYPE_SOFTFIR_STEREO512:
       case CFG_TYPE_SOFTFIR_STEREO1024:
       case CFG_TYPE_SOFTFIR_4R256:
       case CFG_TYPE_SOFTFIR_4R512:
       case CFG_TYPE_SOFTFIR_4R1024:   return QColor(244, 188, 212);
       case CFG_TYPE_FREQSHIFT:
       case CFG_TYPE_FREQSHIFT_PLUS:   return QColor( 61, 149, 147);
       case CFG_TYPE_NFC1_8NF:
       case CFG_TYPE_NFC1_16NF:
       case CFG_TYPE_NFC1_24NF:
       case CFG_TYPE_NFC1_32NF:        return QColor( 19, 181, 177);
       case CFG_TYPE_MIXER_4X1:
       case CFG_TYPE_MIXER_4X2:
       case CFG_TYPE_MIXER_4X4:        return QColor(203, 122,  87);
       case CFG_TYPE_MIXER_8X1:
       case CFG_TYPE_MIXER_8X2:
       case CFG_TYPE_MIXER_8X4:
       case CFG_TYPE_MIXER_8X8:        return QColor(244, 166, 133);
       case CFG_TYPE_MIXER_16X16:      return QColor( 70, 190, 150);
       case CFG_TYPE_MIXER_32X32:      return QColor( 70, 190, 150);
       case CFG_TYPE_MIXER_64X64:      return QColor( 70, 190, 150);
       case CFG_TYPE_AUTOMIX_GS_4:
       case CFG_TYPE_AUTOMIX_GS_8:
       case CFG_TYPE_AUTOMIXNGAT_4:
       case CFG_TYPE_AUTOMIXNGAT_8:    return QColor(160, 160,  90);

       case CFG_TYPE_ROUTER_4:         return QColor(181, 124, 130);
       case CFG_TYPE_ROUTER_8:         return QColor(226, 164, 170);
       case CFG_TYPE_ROUTER_16:        return QColor( 60, 190, 100);
       case CFG_TYPE_ROUTER_32:        return QColor( 60, 190, 100);
       case CFG_TYPE_ROUTER_64:        return QColor( 60, 190, 100);
       case CFG_TYPE_GAIN_1:
       case CFG_TYPE_GAIN_2:
       case CFG_TYPE_GAIN_4:
       case CFG_TYPE_GAIN_8:
       case CFG_TYPE_GAIN_16:
       case CFG_TYPE_GAIN_32:         return QColor(188, 142,  74);
       case CFG_TYPE_GAINRAMP_1:
       case CFG_TYPE_GAINRAMP_2:
       case CFG_TYPE_GAINRAMP_4:
       case CFG_TYPE_GAINRAMP_8:
       case CFG_TYPE_GAINRAMP_16:
       case CFG_TYPE_GAINRAMP_32:     return QColor(221, 165,  81);
       case CFG_TYPE_AEC_2_2:          return QColor(240, 191, 116);
       case CFG_TYPE_SIG_SINE:         return QColor( 60, 103, 182);
       case CFG_TYPE_SIG_PINKNOISE:    return QColor( 78, 128, 220);
       case CFG_TYPE_SIG_NOISE:        return QColor(103, 155, 251);
       case CFG_TYPE_METER_1:          return QColor(170, 137, 189);
       case CFG_TYPE_METER_2:          return QColor(143, 130, 188);
       case CFG_TYPE_METER_4:          return QColor(140, 151, 203);
       case CFG_TYPE_METER_8:          return QColor(136, 171, 218);
       case CFG_TYPE_SNAPSHOT:
       case CFG_TYPE_SNAPSHOT_BANK:    return QColor(112,  73,  73);
       case CFG_TYPE_CTRL_RS232:            return QColor(141,  69,  69);
       case CFG_TYPE_CTRL_RS485:            return QColor(188,  81,  81);
       case CFG_TYPE_CTRL_GPI:             return QColor(238, 104, 104);
       case CFG_TYPE_INPUT_4:
       case CFG_TYPE_OUTPUT_4:        return QColor(110, 150, 150);

       default:
           qDebug() << __FUNCTION__ << __LINE__ << type;
           return QColor(150, 150, 150, 255);
       }
}


const QFont SkinStyle::getFont(int size, bool isBord)
{
    QFont font = QFont("Microsoft YaHei", size);
    font.setBold(isBord);
    return font;
}
