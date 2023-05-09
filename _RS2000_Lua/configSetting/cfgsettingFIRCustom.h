#ifndef CFGSETTINGFIRCUSTOM_H
#define CFGSETTINGFIRCUSTOM_H


namespace Ui {
class CfgSettingFIRCustom;
}

class ConfigItemFIRCustom;
class CfgSettingFIRCustom : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingFIRCustom(ConfigItemFIRCustom *cfgItem, QWidget *parent = 0);
    ~CfgSettingFIRCustom();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked(bool checked);

private:
    Ui::CfgSettingFIRCustom *ui;
    ConfigItemFIRCustom *_cfgItem;
};

#endif // CFGSETTINGFIRCUSTOM_H
