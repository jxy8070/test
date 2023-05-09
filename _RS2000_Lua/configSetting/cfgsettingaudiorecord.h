#ifndef CFGSETTINGAUDIORECORD_H
#define CFGSETTINGAUDIORECORD_H


namespace Ui {
class CfgSettingAudioRecord;
}

class ConfigItemAudioRecord;
class CfgSettingAudioRecord : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingAudioRecord(ConfigItemAudioRecord *cfgItem, QWidget *parent = nullptr);
    ~CfgSettingAudioRecord();

private:
    void initUI(void);
    void connectSlots(void);

private slots:
    void onCheckBoxBtnClicked_slots();
    void onComboBoxCurrentIndexChanged_slots(int);

private:
    Ui::CfgSettingAudioRecord *ui;

    ConfigItemAudioRecord *_cfgItem;
};

#endif // CFGSETTINGAUDIORECORD_H
