#ifndef CFGSETTINGINPUT_H
#define CFGSETTINGINPUT_H

#include <QTextEdit>

namespace Ui {
class CfgSettingPort;
class CfgSettingPort_FangTu;
}
class ConfigItemInput;
class QPushButton;
class QComboBox;
class QLabel;
class QDoubleSpinBox;
class QWidget;
class VolumeWidget;
class configDelayNewDly;
class ConfigGainWidget;
class CfgSettingInput : public QWidget
{
    Q_OBJECT
public:
    CfgSettingInput(ConfigItemInput *input, QWidget *parent = 0);
    ~CfgSettingInput();

    void setVuDisp(unsigned char *d);
    void setGainFromUser(double g);
    void setPhaseFromUser(bool b);

    void updateUiData();
private:
    void initUI();
    void initUI_FangTu();
private slots:
    void onPhantomClicked(bool enable);
    void onPregainValueChanged(int value);
    void onGainValueChanged(double value);
    void onMuteClicked(bool enable);
    void onPhaseClicked(bool enable);
    void onUserLabelChanged();

    void onLevelFadeSlot();
private:
    ConfigItemInput *_inputItem;
    Ui::CfgSettingPort *_ui = NULL;
    Ui::CfgSettingPort_FangTu *_ui_FangTu = NULL;
    QList<QWidget *> _channelWidgetList;
    QList<QPushButton *> _muteList;
    QList<QPushButton *> _phaseList;
    QList<QPushButton *> _phantomList;
    QList<QComboBox *> _pregainList;
    QList<QLabel *> _channelList;
    QList<ConfigGainWidget *> _gainList;
    QList<VolumeWidget *> _vuList;
    QList<QLabel *> _vuLabelList;
    QList<QLabel *> _vuLabel_R_list;
    QStringList _preAmpGains;
    QList<QTextEdit *> _textEditList;//hrp
    QList<QWidget *> _channelWidgetSetList;

    QTimer *_levelFade;
    QList<int> _fadeValue;
};

#endif // CFGSETTINGINPUT_H



