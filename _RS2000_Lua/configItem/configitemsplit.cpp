#include "configitemsplit.h"
#include "configSetting/cfgsettingsplite.h"
#include "protocal/packet.h"
#include "customWidget/eqgraph.h"

ConfigItemSplit::ConfigItemSplit(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    int inNum =1, outNum=1;
    memset(&_body_2, 0, sizeof(IG_PARACTL_SPLIT2));
    memset(&_body_3, 0, sizeof(IG_PARACTL_SPLIT3));
    memset(&_body_4, 0, sizeof(IG_PARACTL_SPLIT4));

    addSubType(CFG_TYPE_SPLIT_2);
    addSubType(CFG_TYPE_SPLIT_3);
    addSubType(CFG_TYPE_SPLIT_4);
    _smooth = 0;
    QStringList oPortNameList;
    switch(cfgType)
    {
    case CFG_TYPE_SPLIT_2:
        inNum = 1; outNum = 2;
        oPortNameList << "L" << "H";
        break;
    case CFG_TYPE_SPLIT_3:
        inNum = 1; outNum = 3;
        oPortNameList << "L"  << "M" << "H";
        break;
    case CFG_TYPE_SPLIT_4:
        inNum = 1; outNum = 4;
        oPortNameList << "L" << "LM" << "HM" << "H";
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    initPort(inNum, outNum, QStringList(), oPortNameList);
    initData();
    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    QString groupIn("In");
    pinList.append(new CLuaPinParam_PC(this, name, "Mute", 0x00080001, 0x00080002, 0x00, 0x00, groupIn));
    pinList.append(new CLuaPinParam_PC(this, name, "Gain", 0x00080003, 0x00080004, 0x00, 0x00, groupIn));
    QString groupOut("Out");
    pinList.append(new CLuaPinParam_PC(this, name, "Mute", 0x00080009, 0x0008000A, 0x00, 0x00, groupOut));
    pinList.append(new CLuaPinParam_PC(this, name, "Gain", 0x0008000B, 0x0008000C, 0x00, 0x00, groupOut));
    pinList.append(new CLuaPinParam_PC(this, name, "AntiPhase", 0x0008000D, 0x0008000E, 0x00, 0x00, groupOut));

    for(int i = 0; i< outNum; i++)
    {
        QString group1 = QString("Pos%1").arg(i+1);
        pinList.append(new CLuaPinParam_PC(this, name, "Type", 0x00080005, 0x00080006, (quint8)i, 0x00, group1));
        pinList.append(new CLuaPinParam_PC(this, name, "Freq", 0x00080007, 0x00080008, (quint8)i, 0x00, group1));
    }
    initControlPins(pinList);
    _setting = NULL;
}

ConfigItemSplit::~ConfigItemSplit()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

void ConfigItemSplit::initData()
{
    _split.Input.mute = false;
    _split.Input.level = 0.0;

    switch(getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        _split.Segment[0].Filter.setSyncL = true;
        _split.Segment[0].Filter.setSyncR = false;
        _split.Segment[1].Filter.setSyncL = false;
        _split.Segment[1].Filter.setSyncR = true;

        _split.Segment[0].Filter.setSyncLEnable = true;
        _split.Segment[0].Filter.setSyncREnable = false;
        _split.Segment[0].Filter.filterTypeLEnable = true;
        _split.Segment[0].Filter.filterTypeREnable = false;
        _split.Segment[0].Filter.freqLEnable = true;
        _split.Segment[0].Filter.freqREnable = false;
        _split.Segment[1].Filter.setSyncLEnable = false;
        _split.Segment[1].Filter.setSyncREnable = true;
        _split.Segment[1].Filter.filterTypeLEnable = false;
        _split.Segment[1].Filter.filterTypeREnable = true;
        _split.Segment[1].Filter.freqLEnable = false;
        _split.Segment[1].Filter.freqREnable = true;
        for (int i = 0; i < 2; ++i)
        {
            _split.Segment[i].Filter.filterTypeL = 5;
            _split.Segment[i].Filter.filterTypeR = 5;
            _split.Segment[i].Filter.freqL = 1000.0;
            _split.Segment[i].Filter.freqR = 1000.0;
            _split.Segment[i].Output.mute = false;
            _split.Segment[i].Output.invert = false;
            _split.Segment[i].Output.level = 0.0;
        }
        break;
    case CFG_TYPE_SPLIT_3:
        _split.Segment[0].Filter.setSyncL = true;
        _split.Segment[0].Filter.setSyncR = false;
        _split.Segment[1].Filter.setSyncL = true;
        _split.Segment[1].Filter.setSyncR = true;
        _split.Segment[2].Filter.setSyncL = false;
        _split.Segment[2].Filter.setSyncR = true;

        _split.Segment[0].Filter.freqL = 250.0;
        _split.Segment[0].Filter.freqR = 250.0;
        _split.Segment[1].Filter.freqL = 250.0;
        _split.Segment[1].Filter.freqR = 2500.0;
        _split.Segment[2].Filter.freqL = 250.0;
        _split.Segment[2].Filter.freqR = 2500.0;

        _split.Segment[0].Filter.setSyncLEnable = true;
        _split.Segment[0].Filter.setSyncREnable = false;
        _split.Segment[0].Filter.filterTypeLEnable = true;
        _split.Segment[0].Filter.filterTypeREnable = false;
        _split.Segment[0].Filter.freqLEnable = true;
        _split.Segment[0].Filter.freqREnable = false;
        _split.Segment[1].Filter.setSyncLEnable = true;
        _split.Segment[1].Filter.setSyncREnable = true;
        _split.Segment[1].Filter.filterTypeLEnable = true;
        _split.Segment[1].Filter.filterTypeREnable = true;
        _split.Segment[1].Filter.freqLEnable = true;
        _split.Segment[1].Filter.freqREnable = true;
        _split.Segment[2].Filter.setSyncLEnable = false;
        _split.Segment[2].Filter.setSyncREnable = true;
        _split.Segment[2].Filter.filterTypeLEnable = false;
        _split.Segment[2].Filter.filterTypeREnable = true;
        _split.Segment[2].Filter.freqLEnable = false;
        _split.Segment[2].Filter.freqREnable = true;
        for (int i = 0; i < 3; ++i)
        {
            _split.Segment[i].Filter.filterTypeL = 5;
            _split.Segment[i].Filter.filterTypeR = 5;
            _split.Segment[i].Output.mute = false;
            _split.Segment[i].Output.invert = false;
            _split.Segment[i].Output.level = 0.0;
        }
        break;
    case CFG_TYPE_SPLIT_4:
        _split.Segment[0].Filter.setSyncL = true;
        _split.Segment[0].Filter.setSyncR = false;
        _split.Segment[1].Filter.setSyncL = true;
        _split.Segment[1].Filter.setSyncR = true;
        _split.Segment[2].Filter.setSyncL = true;
        _split.Segment[2].Filter.setSyncR = true;
        _split.Segment[3].Filter.setSyncL = false;
        _split.Segment[3].Filter.setSyncR = true;

        _split.Segment[0].Filter.freqL = 160.0;
        _split.Segment[0].Filter.freqR = 160.0;
        _split.Segment[1].Filter.freqL = 160.0;
        _split.Segment[1].Filter.freqR = 800.0;
        _split.Segment[2].Filter.freqL = 800.0;
        _split.Segment[2].Filter.freqR = 4000.0;
        _split.Segment[3].Filter.freqL = 800.0;
        _split.Segment[3].Filter.freqR = 4000.0;

        _split.Segment[0].Filter.setSyncLEnable = true;
        _split.Segment[0].Filter.setSyncREnable = false;
        _split.Segment[0].Filter.filterTypeLEnable = true;
        _split.Segment[0].Filter.filterTypeREnable = false;
        _split.Segment[0].Filter.freqLEnable = true;
        _split.Segment[0].Filter.freqREnable = false;
        _split.Segment[1].Filter.setSyncLEnable = true;
        _split.Segment[1].Filter.setSyncREnable = true;
        _split.Segment[1].Filter.filterTypeLEnable = true;
        _split.Segment[1].Filter.filterTypeREnable = true;
        _split.Segment[1].Filter.freqLEnable = true;
        _split.Segment[1].Filter.freqREnable = true;
        _split.Segment[2].Filter.setSyncLEnable = true;
        _split.Segment[2].Filter.setSyncREnable = true;
        _split.Segment[2].Filter.filterTypeLEnable = true;
        _split.Segment[2].Filter.filterTypeREnable = true;
        _split.Segment[2].Filter.freqLEnable = true;
        _split.Segment[2].Filter.freqREnable = true;
        _split.Segment[3].Filter.setSyncLEnable = false;
        _split.Segment[3].Filter.setSyncREnable = true;
        _split.Segment[3].Filter.filterTypeLEnable = false;
        _split.Segment[3].Filter.filterTypeREnable = true;
        _split.Segment[3].Filter.freqLEnable = false;
        _split.Segment[3].Filter.freqREnable = true;

        for (int i = 0; i < 4; ++i)
        {
            _split.Segment[i].Filter.filterTypeL = 5;
            _split.Segment[i].Filter.filterTypeR = 5;
            _split.Segment[i].Output.mute = false;
            _split.Segment[i].Output.invert = false;
            _split.Segment[i].Output.level = 0.0;
        }
        break;
    default:
        Q_ASSERT(false);
        break;
    }

}

QWidget *ConfigItemSplit::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingSplite(this);
    }
    return _setting;
}

quint16 ConfigItemSplit::calNumType(quint16 num, quint16 type)
{
    //0x01://high_pass_BW
    //0x02://High_pass_L-R
    //0x03://low_pass_BW
    //0x04://Low_pass_L-R
    //0x05://low-shelf
    //0x06://high-shelf

    quint16 ret;

    /* BW */
    if (num > 3)
    {
        ret = (num - 3) << 8 | type;
    }
    /* L-R */
    else
    {
        ret = ((num + 1) * 2) << 8 | (type + 1);
    }

    return ret;
}


QByteArray ConfigItemSplit::getDetailData()
{
    QByteArray paraBuff;
    quint8 order[12] = {
        2, 4, 6, 8, 1, 2, 3, 4, 5, 6, 7, 8
    };

    switch(getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        _body_2.in[0].mute = _split.Input.mute ? 0xFF : 0x00;
        _body_2.in[0].gain = (_split.Input.level + 100) * 100;
        _body_2.in[0].inverse = 0;

        _body_2.filter[0].filterType = _split.Segment[0].Filter.filterTypeL > 3 ? FCT_FILTER_BUTTERWORTH : FCT_FILTER_LINKWITZ;
        _body_2.filter[0].freq_hz = _split.Segment[0].Filter.freqL;
        _body_2.filter[0].orders = order[_split.Segment[0].Filter.filterTypeL];
        _body_2.filter[1].filterType = _split.Segment[1].Filter.filterTypeR > 3 ? FCT_FILTER_BUTTERWORTH : FCT_FILTER_LINKWITZ;
        _body_2.filter[1].freq_hz = _split.Segment[1].Filter.freqR;
        _body_2.filter[1].orders = order[_split.Segment[1].Filter.filterTypeR];

        for (int i = 0; i < 2; ++i)
        {
            _body_2.out[i].mute = _split.Segment[i].Output.mute ? 0xFF : 0x00;
            _body_2.out[i].gain = (_split.Segment[i].Output.level + 100) * 100;
            _body_2.out[i].inverse = _split.Segment[i].Output.invert ? 0xFF : 0x00;
        }

        _body_2.smooth = _smooth;
        paraBuff.append((const char*)&_body_2, sizeof(IG_PARACTL_SPLIT2));
        break;
    case CFG_TYPE_SPLIT_3:
        _body_3.in[0].mute = _split.Input.mute ? 0xFF : 0x00;
        _body_3.in[0].gain = (_split.Input.level + 100) * 100;
        _body_3.in[0].inverse = 0;

        _body_3.filter[0].filterType = _split.Segment[0].Filter.filterTypeL > 3 ? FCT_FILTER_BUTTERWORTH : FCT_FILTER_LINKWITZ;
        _body_3.filter[0].freq_hz = _split.Segment[0].Filter.freqL;
        _body_3.filter[0].orders = order[_split.Segment[0].Filter.filterTypeL];
        _body_3.filter[1].filterType = _split.Segment[1].Filter.filterTypeL > 3 ? FCT_FILTER_BUTTERWORTH : FCT_FILTER_LINKWITZ;
        _body_3.filter[1].freq_hz = _split.Segment[1].Filter.freqL;
        _body_3.filter[1].orders = order[_split.Segment[1].Filter.filterTypeL];
        _body_3.filter[2].filterType = _split.Segment[1].Filter.filterTypeR > 3 ? FCT_FILTER_BUTTERWORTH : FCT_FILTER_LINKWITZ;
        _body_3.filter[2].freq_hz = _split.Segment[1].Filter.freqR;
        _body_3.filter[2].orders = order[_split.Segment[1].Filter.filterTypeR];
        _body_3.filter[3].filterType = _split.Segment[2].Filter.filterTypeR > 3 ? FCT_FILTER_BUTTERWORTH : FCT_FILTER_LINKWITZ;
        _body_3.filter[3].freq_hz = _split.Segment[2].Filter.freqR;
        _body_3.filter[3].orders = order[_split.Segment[2].Filter.filterTypeR];

        for (int i = 0; i < 3; ++i)
        {
            _body_3.out[i].mute = _split.Segment[i].Output.mute ? 0xFF : 0x00;
            _body_3.out[i].gain = (_split.Segment[i].Output.level + 100) * 100;
            _body_3.out[i].inverse = _split.Segment[i].Output.invert ? 0xFF : 0x00;
        }

        _body_3.smooth = _smooth;
        paraBuff.append((const char*)&_body_3, sizeof(IG_PARACTL_SPLIT3));
        break;
    case CFG_TYPE_SPLIT_4:
        _body_4.in[0].mute = _split.Input.mute ? 0xFF : 0x00;
        _body_4.in[0].gain = (_split.Input.level + 100) * 100;
        _body_4.in[0].inverse = 0;

        _body_4.filter[0].filterType = _split.Segment[0].Filter.filterTypeL > 3 ? FCT_FILTER_BUTTERWORTH : FCT_FILTER_LINKWITZ;
        _body_4.filter[0].freq_hz = _split.Segment[0].Filter.freqL;
        _body_4.filter[0].orders = order[_split.Segment[0].Filter.filterTypeL];
        _body_4.filter[1].filterType = _split.Segment[1].Filter.filterTypeL > 3 ? FCT_FILTER_BUTTERWORTH : FCT_FILTER_LINKWITZ;
        _body_4.filter[1].freq_hz = _split.Segment[1].Filter.freqL;
        _body_4.filter[1].orders = order[_split.Segment[1].Filter.filterTypeL];
        _body_4.filter[2].filterType = _split.Segment[1].Filter.filterTypeR > 3 ? FCT_FILTER_BUTTERWORTH : FCT_FILTER_LINKWITZ;
        _body_4.filter[2].freq_hz = _split.Segment[1].Filter.freqR;
        _body_4.filter[2].orders = order[_split.Segment[1].Filter.filterTypeR];
        _body_4.filter[3].filterType = _split.Segment[2].Filter.filterTypeL > 3 ? FCT_FILTER_BUTTERWORTH : FCT_FILTER_LINKWITZ;
        _body_4.filter[3].freq_hz = _split.Segment[2].Filter.freqL;
        _body_4.filter[3].orders = order[_split.Segment[2].Filter.filterTypeL];
        _body_4.filter[4].filterType = _split.Segment[2].Filter.filterTypeR > 3 ? FCT_FILTER_BUTTERWORTH : FCT_FILTER_LINKWITZ;
        _body_4.filter[4].freq_hz = _split.Segment[2].Filter.freqR;
        _body_4.filter[4].orders = order[_split.Segment[2].Filter.filterTypeR];
        _body_4.filter[5].filterType = _split.Segment[3].Filter.filterTypeR > 3 ? FCT_FILTER_BUTTERWORTH : FCT_FILTER_LINKWITZ;
        _body_4.filter[5].freq_hz = _split.Segment[3].Filter.freqR;
        _body_4.filter[5].orders = order[_split.Segment[3].Filter.filterTypeR];

        for (int i = 0; i < 4; ++i)
        {
            _body_4.out[i].mute = _split.Segment[i].Output.mute ? 0xFF : 0x00;
            _body_4.out[i].gain = (_split.Segment[i].Output.level + 100) * 100;
            _body_4.out[i].inverse = _split.Segment[i].Output.invert ? 0xFF : 0x00;
        }

        _body_4.smooth = _smooth;
        paraBuff.append((const char*)&_body_4, sizeof(IG_PARACTL_SPLIT4));
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    _smooth = 0;
    return paraBuff;
}

void ConfigItemSplit::setDetailData(unsigned int *d)
{
    switch(this->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        memcpy(&_body_2, d, sizeof(IG_PARACTL_SPLIT2));
        _split.Input.mute  = _body_2.in[0].mute == 0xFF ? 1 : 0x00;
        _split.Input.level = _body_2.in[0].gain / 100.0f - 100;

        _split.Segment[0].Filter.freqL = _body_2.filter[0].freq_hz;
        if (_body_2.filter[0].filterType == FCT_FILTER_BUTTERWORTH) {
            _split.Segment[0].Filter.filterTypeL = _body_2.filter[0].orders + 3;
        }
        else {
            _split.Segment[0].Filter.filterTypeL = (_body_2.filter[0].orders >> 1) - 1;
        }

        _split.Segment[1].Filter.freqR = _body_2.filter[1].freq_hz;
        if (_body_2.filter[1].filterType == FCT_FILTER_BUTTERWORTH) {
            _split.Segment[1].Filter.filterTypeR = _body_2.filter[1].orders + 3;
        }
        else {
            _split.Segment[1].Filter.filterTypeR = (_body_2.filter[1].orders >> 1) - 1;
        }

        for (int i = 0; i < 2; ++i)
        {
            _split.Segment[i].Output.mute   = _body_2.out[i].mute == 0xFF ? 1 : 0x00;
            _split.Segment[i].Output.level  = _body_2.out[i].gain / 100.0f - 100;
            _split.Segment[i].Output.invert = _body_2.out[i].inverse == 0xFF ? 1 : 0x00;
        }

        _smooth = _body_2.smooth;
        break;
    case CFG_TYPE_SPLIT_3:
        memcpy(&_body_3, d, sizeof(IG_PARACTL_SPLIT3));
        _split.Input.mute  = _body_3.in[0].mute == 0xFF ? 1 : 0x00;
        _split.Input.level = _body_3.in[0].gain / 100.0f - 100;

        _split.Segment[0].Filter.freqL = _body_3.filter[0].freq_hz;
        if (_body_3.filter[0].filterType == FCT_FILTER_BUTTERWORTH) {
            _split.Segment[0].Filter.filterTypeL = _body_3.filter[0].orders + 3;
        }
        else {
            _split.Segment[0].Filter.filterTypeL = (_body_3.filter[0].orders >> 1) - 1;
        }

        _split.Segment[1].Filter.freqL = _body_3.filter[1].freq_hz;
        if (_body_3.filter[1].filterType == FCT_FILTER_BUTTERWORTH) {
            _split.Segment[1].Filter.filterTypeL = _body_3.filter[1].orders + 3;
        }
        else {
            _split.Segment[1].Filter.filterTypeL = (_body_3.filter[1].orders >> 1) - 1;
        }

        _split.Segment[1].Filter.freqR = _body_3.filter[2].freq_hz;
        if (_body_3.filter[2].filterType == FCT_FILTER_BUTTERWORTH) {
            _split.Segment[1].Filter.filterTypeR = _body_3.filter[2].orders + 3;
        }
        else {
            _split.Segment[1].Filter.filterTypeR = (_body_3.filter[2].orders >> 1) - 1;
        }

        _split.Segment[2].Filter.freqR = _body_3.filter[3].freq_hz;
        if (_body_3.filter[0].filterType == FCT_FILTER_BUTTERWORTH) {
            _split.Segment[2].Filter.filterTypeR = _body_3.filter[3].orders + 3;
        }
        else {
            _split.Segment[2].Filter.filterTypeR = (_body_3.filter[3].orders >> 1) - 1;
        }

        for (int i = 0; i < 3; ++i)
        {
            _split.Segment[i].Output.mute   = _body_3.out[i].mute == 0xFF ? 1 : 0x00;
            _split.Segment[i].Output.level  = _body_3.out[i].gain / 100.0f - 100;
            _split.Segment[i].Output.invert = _body_3.out[i].inverse == 0xFF ? 1 : 0x00;
        }

        _smooth = _body_3.smooth;
        break;
    case CFG_TYPE_SPLIT_4:
        memcpy(&_body_4, d, sizeof(IG_PARACTL_SPLIT4));
        _split.Input.mute  = _body_4.in[0].mute == 0xFF ? 1 : 0x00;
        _split.Input.level = _body_4.in[0].gain / 100.0f - 100;

        _split.Segment[0].Filter.freqL = _body_4.filter[0].freq_hz;
        if (_body_4.filter[0].filterType == FCT_FILTER_BUTTERWORTH) {
            _split.Segment[0].Filter.filterTypeL = _body_4.filter[0].orders + 3;
        }
        else {
            _split.Segment[0].Filter.filterTypeL = (_body_4.filter[0].orders >> 1) - 1;
        }

        _split.Segment[1].Filter.freqL = _body_4.filter[1].freq_hz;
        if (_body_4.filter[1].filterType == FCT_FILTER_BUTTERWORTH) {
            _split.Segment[1].Filter.filterTypeL = _body_4.filter[1].orders + 3;
        }
        else {
            _split.Segment[1].Filter.filterTypeL = (_body_4.filter[1].orders >> 1) - 1;
        }

        _split.Segment[1].Filter.freqR = _body_4.filter[2].freq_hz;
        if (_body_4.filter[2].filterType == FCT_FILTER_BUTTERWORTH) {
            _split.Segment[1].Filter.filterTypeR = _body_4.filter[2].orders + 3;
        }
        else {
            _split.Segment[1].Filter.filterTypeR = (_body_4.filter[2].orders >> 1) - 1;
        }

        _split.Segment[2].Filter.freqL = _body_4.filter[3].freq_hz;
        if (_body_4.filter[3].filterType == FCT_FILTER_BUTTERWORTH) {
            _split.Segment[2].Filter.filterTypeL = _body_4.filter[3].orders + 3;
        }
        else {
            _split.Segment[2].Filter.filterTypeL = (_body_4.filter[3].orders >> 1) - 1;
        }

        _split.Segment[2].Filter.freqR = _body_4.filter[4].freq_hz;
        if (_body_4.filter[4].filterType == FCT_FILTER_BUTTERWORTH) {
            _split.Segment[2].Filter.filterTypeR = _body_4.filter[4].orders + 3;
        }
        else {
            _split.Segment[2].Filter.filterTypeR = (_body_4.filter[4].orders >> 1) - 1;
        }

        _split.Segment[3].Filter.freqR = _body_4.filter[5].freq_hz;
        if (_body_4.filter[5].filterType == FCT_FILTER_BUTTERWORTH) {
            _split.Segment[3].Filter.filterTypeR = _body_4.filter[5].orders + 3;
        }
        else {
            _split.Segment[3].Filter.filterTypeR = (_body_4.filter[5].orders >> 1) - 1;
        }

        for (int i = 0; i < 4; ++i)
        {
            _split.Segment[i].Output.mute   = _body_4.out[i].mute == 0xFF ? 1 : 0x00;
            _split.Segment[i].Output.level  = _body_4.out[i].gain / 100.0f - 100;
            _split.Segment[i].Output.invert = _body_4.out[i].inverse == 0xFF ? 1 : 0x00;
        }

        _smooth = _body_4.smooth;
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemSplit::setRtnData(unsigned int *d)
{

}

void ConfigItemSplit::updateCfgSettingLoadedData()
{
    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemSplit::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SPLIT_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    setInputLevel(ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_INPUT_LEVEL)).toDouble());
    setInputMute(ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_INPUT_MUTE)).toBool());
    for (int j = 0; j < 4; ++j)
    {
        setFilterSetSyncL(j, ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_SETSYNC_L).arg(j)).toBool());
        setFilterSetSyncR(j, ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_SETSYNC_R).arg(j)).toBool());
        setFilterFilterTypeL(j, ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_FILTERTYPE_L).arg(j)).toInt());
        setFilterFilterTypeR(j, ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_FILTERTYPE_R).arg(j)).toInt());
        setFilterFreqL(j, ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_FREQ_L).arg(j)).toDouble());
        setFilterFreqR(j, ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_FREQ_R).arg(j)).toDouble());
        setOutputMute(j, ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_OUTPUT_MUTE).arg(j)).toBool());
        setOutputInvert(j, ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_OUTPUT_INVERT).arg(j)).toBool());
        setOutputLevel(j, ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_OUTPUT_LEVLE).arg(j)).toDouble());
    }
    ConfigIni->endGroup();
}


void ConfigItemSplit::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SPLIT_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_INPUT_LEVEL), getInputLevel());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_INPUT_MUTE), getInputMute());
    for (int i = 0; i < 4; ++i)
    {
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_SETSYNC_L).arg(i), getFilterSetSyncL(i));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_SETSYNC_R).arg(i), getFilterSetSyncR(i));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_FILTERTYPE_L).arg(i), getFilterFilterTypeL(i));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_FILTERTYPE_R).arg(i), getFilterFilterTypeR(i));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_FREQ_L).arg(i), getFilterFreqL(i));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_FREQ_R).arg(i), getFilterFreqR(i));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_OUTPUT_MUTE).arg(i), getOutputMute(i));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_OUTPUT_INVERT).arg(i), getOutputInvert(i));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_OUTPUT_LEVLE).arg(i), getOutputLevel(i));
    }
    ConfigIni->endGroup();
}

QVariant ConfigItemSplit::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
//    bool setSplitType(const QString& itemName, quint8 sNo, quint8 type);                //0x00080006
//    bool setSplitFreq(const QString& itemName, quint8 sNo, quint16 freq);               //0x00080008
//    bool setSplitOutMute(const QString& itemName, bool mute);                           //
//    bool setSplitOutGain(const QString& itemName, double gain);                         //0x0008000C
//    bool setSplitOutAntiPhase(const QString& itemName, bool antiPhase);                 //0x0008000E

//    void setInputMute(bool flag);
//    void setInputLevel(double value);
//    void setFilterSetSyncL(int idx, bool flag);
//    void setFilterSetSyncR(int idx, bool flag);

//    void setFilterSetSyncLEnable(int idx, bool flag);
//    void setFilterSetSyncREnable(int idx, bool flag);
//    void setFilterFilterTypeLEnable(int idx, bool flag);
//    void setFilterFilterTypeREnable(int idx, bool flag);
//    void setFilterFreqLEnable(int idx, bool flag);
//    void setFilterFreqREnable(int idx, bool flag);

//    void setFilterFilterTypeL(int idx, int value);
//    void setFilterFilterTypeR(int idx, int value);
//    void setFilterFilterTypeLL(int idx1, int idx2, double value);
//    void setFilterFilterTypeLR(int idx1, int idx2, double value);
//    void setFilterFilterTypeRR(int idx1, int idx2, double value);
//    void setFilterFreqL(int idx, double value);
//    void setFilterFreqR(int idx, double value);
//    void setFilterFreqLL(int idx1, int idx2, double value);
//    void setFilterFreqLR(int idx1, int idx2, double value);
//    void setFilterFreqRR(int idx1, int idx2, double value);
//    void setOutputMute(int idx, bool flag);
//    void setOutputInvert(int idx, bool flag);
//    void setOutputLevel(int idx, double value);

    switch(param->SetCmdKey())
    {
//    case 0x00080002:
//        setInputMute(val.toBool());
//        return QVariant(val.toBool());
//    case 0x00080004:
//        setInputLevel(val.toDouble());
//        return QVariant(val.toDouble());
//    case 0x00070006:
//        setMute(val.toBool());
//        return QVariant(val.toBool());
//    case 0x00070004:
//        setLevel(val.toDouble());
//        return QVariant(val.toDouble());
//    case 0x0008000A:
//        setMute(val.toBool());
//        return QVariant(val.toBool());
//    case 0x00070004:
//        setLevel(val.toDouble());
//        return QVariant(val.toDouble());
//    case 0x00070006:
//        setMute(val.toBool());
//        return QVariant(val.toBool());
    default:
        assert(false);
        return QVariant();
    }
}

void ConfigItemSplit::setInputMute(bool flag)
{
    _split.Input.mute = flag;
    saveParamToDevice();
}

void ConfigItemSplit::setInputLevel(double value)
{
    _split.Input.level = value;
    saveParamToDevice();
}

void ConfigItemSplit::setFilterSetSyncL(int idx, bool flag)
{
    _split.Segment[idx].Filter.setSyncL = flag;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemSplit::setFilterSetSyncR(int idx, bool flag)
{
    _split.Segment[idx].Filter.setSyncR = flag;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemSplit::setFilterSetSyncLEnable(int idx, bool flag)
{
    _split.Segment[idx].Filter.setSyncLEnable = flag;
}

void ConfigItemSplit::setFilterSetSyncREnable(int idx, bool flag)
{
    _split.Segment[idx].Filter.setSyncREnable = flag;
}

void ConfigItemSplit::setFilterFilterTypeLEnable(int idx, bool flag)
{
    _split.Segment[idx].Filter.filterTypeLEnable = flag;
}

void ConfigItemSplit::setFilterFilterTypeREnable(int idx, bool flag)
{
    _split.Segment[idx].Filter.filterTypeREnable = flag;
}

void ConfigItemSplit::setFilterFreqLEnable(int idx, bool flag)
{
    _split.Segment[idx].Filter.freqLEnable = flag;
}

void ConfigItemSplit::setFilterFreqREnable(int idx, bool flag)
{
    _split.Segment[idx].Filter.freqREnable = flag;
}

void ConfigItemSplit::setFilterFilterTypeL(int idx, int value)
{
    _split.Segment[idx].Filter.filterTypeL = value;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemSplit::setFilterFilterTypeR(int idx, int value)
{
    _split.Segment[idx].Filter.filterTypeR = value;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemSplit::setFilterFilterTypeLL(int idx1, int idx2, double value)
{
    _split.Segment[idx1].Filter.filterTypeL = value;
    _split.Segment[idx2].Filter.filterTypeL = value;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemSplit::setFilterFilterTypeLR(int idx1, int idx2, double value)
{
    _split.Segment[idx1].Filter.filterTypeL = value;
    _split.Segment[idx2].Filter.filterTypeR = value;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemSplit::setFilterFilterTypeRR(int idx1, int idx2, double value)
{
    _split.Segment[idx1].Filter.filterTypeR = value;
    _split.Segment[idx2].Filter.filterTypeR = value;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemSplit::setFilterFreqL(int idx, double value)
{
    _split.Segment[idx].Filter.freqL = value;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemSplit::setFilterFreqR(int idx, double value)
{
    _split.Segment[idx].Filter.freqR = value;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemSplit::setFilterFreqLL(int idx1, int idx2, double value)
{
    _split.Segment[idx1].Filter.freqL = value;
    _split.Segment[idx2].Filter.freqL = value;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemSplit::setFilterFreqLR(int idx1, int idx2, double value)
{
    _split.Segment[idx1].Filter.freqL = value;
    _split.Segment[idx2].Filter.freqR = value;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemSplit::setFilterFreqRR(int idx1, int idx2, double value)
{
    _split.Segment[idx1].Filter.freqR = value;
    _split.Segment[idx2].Filter.freqR = value;
    _smooth = 0xFFFFFFFF;
    saveParamToDevice();
}

void ConfigItemSplit::setOutputMute(int idx, bool flag)
{
    _split.Segment[idx].Output.mute = flag;
    saveParamToDevice();
}

void ConfigItemSplit::setOutputInvert(int idx, bool flag)
{
    _split.Segment[idx].Output.invert = flag;
    saveParamToDevice();
}

void ConfigItemSplit::setOutputLevel(int idx, double value)
{
    _split.Segment[idx].Output.level = value;
    saveParamToDevice();
}
