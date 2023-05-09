#ifndef CFGSETTINGFREQSHIFT_H
#define CFGSETTINGFREQSHIFT_H

#include "customWidget/configdelaynewdly.h"

namespace Ui {
class CfgSettingFreqShift;
class CfgSettingFreqShift_Plus;
class CfgSettingFreqShift_Plus_FangTu;
}

class ConfigItemFreqShift;
class CfgSettingFreqShift : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingFreqShift(ConfigItemFreqShift *cfgItem, QWidget *parent = 0);
    ~CfgSettingFreqShift();

    void upDateUiData();
private:
    void initData(void);
    void initData_plus_FangTu();
private slots:
    void SP_valueChanged(int value);
    void SP_valueChanged(double value);

    void on_pushButton_clicked(bool checked);

private:
    ConfigItemFreqShift *_cfgItem;
    Ui::CfgSettingFreqShift *ui;
    Ui::CfgSettingFreqShift_Plus *ui_plus;
    Ui::CfgSettingFreqShift_Plus_FangTu *ui_plus_FangTu;
};

#endif // CFGSETTINGFREQSHIFT_H
