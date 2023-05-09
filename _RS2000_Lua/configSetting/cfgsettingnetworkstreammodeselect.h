#ifndef CFGSETTINGNETWORKSTREAMMODESELECT_H
#define CFGSETTINGNETWORKSTREAMMODESELECT_H

#include <QDialog>

namespace Ui {
class CfgSettingNetworkStreamModeSelect;
}

class DeviceItem;
class CfgSettingNetworkStreamModeSelect : public QDialog
{
    Q_OBJECT

public:
    explicit CfgSettingNetworkStreamModeSelect(DeviceItem *devItemBase, QDialog *parent = 0);
    ~CfgSettingNetworkStreamModeSelect();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::CfgSettingNetworkStreamModeSelect *ui;
    DeviceItem *_devItemBase;
};

#endif // CFGSETTINGNETWORKSTREAMMODESELECT_H
