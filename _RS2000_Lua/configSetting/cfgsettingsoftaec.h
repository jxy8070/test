#ifndef CFGSETTINGSOFTAEC_H
#define CFGSETTINGSOFTAEC_H

#include <QWidget>

namespace Ui {
class CfgSettingSoftAEC;
}
class ConfigItemSoftAEC;
class CfgSettingSoftAEC : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingSoftAEC(ConfigItemSoftAEC *cfgItem, QWidget *parent = nullptr);
    ~CfgSettingSoftAEC();
    void updateRtnData(double vuERLE, double vuRMLR);
    void updateUiData();
private slots:
    void pb_clicked(bool value);
    void spin_valueChanged(double value);
protected:
    void initUi();

private:
    Ui::CfgSettingSoftAEC *ui;
    ConfigItemSoftAEC *_cfgItem;
};

#endif // CFGSETTINGSOFTAEC_H
