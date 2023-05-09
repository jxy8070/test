#ifndef CFGSETTINGGPI_H
#define CFGSETTINGGPI_H


class ConfigItemGPI;
class CfgSettingGPI : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingGPI(ConfigItemGPI *cfgItem, QWidget *parent = 0);
    ~CfgSettingGPI();
    void setupUi(int gpiCnt);
    void updateUiData();
private slots:
    void onGpiIndexChanged(int index);

private:
    QComboBox *_cboxGpi;
    ConfigItemGPI *_cfgItem;
};

#endif // CFGSETTINGGPI_H
