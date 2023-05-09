#ifndef CFGSETTINGAEC_H
#define CFGSETTINGAEC_H


namespace Ui {
class CfgSettingAEC;
class CfgSettingAEC_FangTu;
}

class ConfigItemAEC;
class CfgSettingAEC : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingAEC(ConfigItemAEC *cfgItem, QWidget *parent = 0);
    ~CfgSettingAEC();

    void updateUiData();
private slots:
    void pb_clicked(bool value);
    void spin_valueChanged(int value);

    void on_comboBox_currentIndexChanged(int index);
    void on_NRGainSwitch_clicked(bool checked);

private:
    void init(Ui::CfgSettingAEC *ui);
    void init(Ui::CfgSettingAEC_FangTu *ui);



private:
    Ui::CfgSettingAEC *_ui = NULL;
    Ui::CfgSettingAEC_FangTu *_ui_FangTu = NULL;
    ConfigItemAEC *_cfgItem;
};

#endif // CFGSETTINGAEC_H
