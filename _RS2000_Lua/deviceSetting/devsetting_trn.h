#ifndef DEVSETTING_TRN_H
#define DEVSETTING_TRN_H

#include <QScrollArea>
#include "protocal/packet.h"


struct PushButtons
{
    QPushButton *micGain;
    QPushButton *phantom;
    QPushButton *phase;
//    QPushButton *inDelay;
//    QPushButton *inLevle;
    QPushButton *outDelay;
    QPushButton *outLevel;
};

class AES70Device;
class DeviceItem_Trn;
class QButtonGroup;
class QLabel;
class QGroupBox;
class QPushButton;
class Trn_GainWidget;
class Trn_DelayWidget;
class Trn_CopyWidget;
class Trn_LevelWidget;
class Trn_MixerWidget;
class Trn_DynWidget;
class Trn_MeterWidget;
class Trn_SceneWidget;
class EqView_AllBand;
class FilterView;
class EqViewSHG;
class DevSetting_TRN : public QScrollArea
{
    Q_OBJECT
public:
    explicit DevSetting_TRN(DeviceItem_Trn *devItem, QWidget *parent = 0);
    ~DevSetting_TRN();

    static QString getChannelName(int ch, bool isInput);
protected:

private slots:
    void onMixerClicked();
    void onInDelayClicked();
    void onOutDelayClicked();
    void onInLevelClick();
    void onOutLevelClick();
    void onInDynClicked(bool isChecked);
    void onOutDynClicked(bool isChecked);
    void onInEqClicked(bool isChecked);
    void onOutEq15BClicked(bool isChecked);
    void onOutSplitClicked(bool isChecked);
    void onInMeterClicked();
    void onOutMeterClicked();
    void onMicGainClicked();

    void onGainWidgetChanged(quint8 ch, qint8 value);
    void onPhantomPowerClicked(bool isOpen);

    void onSave2DeviceClicked();
    void onSave2LocalClicked();
    void onLoadCfgClicked();
    void saveToScene(quint8);
    void dispMeter(quint16 *meterI, quint16 *meterO);

    void initConfig(const ConfigData_Trn &cfgData);
private:
    void clearCheckedButton();
    void childWidgetClose();
    void initUi();
    void initInfoUi(QGroupBox *infoLayout);
    void initParaUi(QGroupBox *groupBoxPara);
    void AddSeparate(QHBoxLayout *horizontalLayout, bool isRepeat = false);
    void initParaHeaderUi(QGridLayout *gridLayoutPara);
    void initInputUi(QGridLayout *gridLayoutPara, QString inputText, int rowIdx);
    void initOutputUi(QGridLayout *gridLayoutPara, QString outputText, int rowIdx);
    void setInputLayoutStretch(QHBoxLayout *horizontalLayout);
    void setOutputLayoutStretch(QHBoxLayout *horizontalLayout);

private:
    QWidget *_subWidget;
    QFont _fontBord;
    QFont _fontNormal;
    QGroupBox *_groupBoxSub;
    QList<PushButtons *> _buttonsList;
    QLabel *_nameLabel;
    QLabel *_ipLabel;
    QLabel *_macLabel;
    QLabel *_sceneLabel;    
    QButtonGroup *_btnGroup;
    QPushButton *_btnHide;

    DeviceItem_Trn *_devItem;
    AES70Device *_devBase;
    Trn_CopyWidget *_copyWidget;
    Trn_GainWidget *_gainWidget;
    Trn_DelayWidget *_inDelayWidget;
    Trn_DelayWidget *_outDelayWidget;
    Trn_LevelWidget *_inLevelWidget;
    Trn_LevelWidget *_outLevelWidget;
    Trn_MixerWidget *_mixerWidget;
    Trn_DynWidget *_dynWidget;
    EqView_AllBand *_inEqWidget;
    EqViewSHG *_outEq15BWidget;
    FilterView *_outSplitWidget;
    Trn_MeterWidget *_inMeterWidget;
    Trn_MeterWidget *_outMeterWidget;
    Trn_SceneWidget *_sceneSelectWidget;
};



#endif // DEVSETTING_TRN_H
