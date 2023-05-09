#ifndef CFGSETTINGOUTPUT_H
#define CFGSETTINGOUTPUT_H

#include <QTextEdit>
namespace Ui {
class CfgSettingPort;
class CfgSettingPort_FangTu;
}
class ConfigItemOutput;
class QPushButton;
class QLabel;
class QDoubleSpinBox;
class QWidget;
class VolumeWidget;
class configDelayNewDly;
class ConfigGainWidget;
class CfgSettingOutput : public QWidget
{
    Q_OBJECT
public:
    CfgSettingOutput(ConfigItemOutput *outputItem, QWidget *parent = 0);
    ~CfgSettingOutput();

    void setVuDisp(unsigned char *d);
    void updateUiData();
private slots:
    void onGainValueChanged(double value);
    void onMuteClicked(bool enable);
    void onPhaseClicked(bool enable);
    void onUserLabelChanged();

private:
    void initUI();
    void initUI_FangTu();
private:
    ConfigItemOutput *_outputItem;
    Ui::CfgSettingPort *_ui = NULL;
    Ui::CfgSettingPort_FangTu *_ui_FangTu = NULL;
    QList<QWidget *> _channelWidgetList;
    QList<QLabel *> _channelList;
    QList<ConfigGainWidget *> _gainList;
    QList<QPushButton *> _muteList;
    QList<QPushButton *> _phaseList;
    QList<VolumeWidget *> _vuList;
    QList<QLabel *> _vuLabelList;

    QList<QTextEdit *> _textEditList;//hrp
    QList<QWidget *> _channelWidgetSetList;
};

#endif // CFGSETTINGOUTPUT_H


