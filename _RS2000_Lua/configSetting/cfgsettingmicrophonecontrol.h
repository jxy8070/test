#ifndef CFGSETTINGMICROPHONECONTROL_H
#define CFGSETTINGMICROPHONECONTROL_H


class ConfigItemMicrophoneControl;
class QGridLayout;
class QComboBox;
class CfgSettingMicrophoneControl : public QWidget
{
    Q_OBJECT
public:
    CfgSettingMicrophoneControl(ConfigItemMicrophoneControl *cfgItem, QWidget *parent = 0);

private:
    void setupUi();
    void addHeader(QGridLayout *);
    void setMicrophoneControl();
public slots:
    void onMicrophoneNoClicked(int);

private:
//    int _rowNum;
//    int _colNum;
    int _microphoneNum;
    int _maxNum;
    ConfigItemMicrophoneControl *_cfgItem;
    QComboBox *_box_microphoneNo;
    QComboBox *_box_MaxNum;
};

#endif // CFGSETTINGMICROPHONECONTROL_H
