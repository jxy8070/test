#ifndef DEVSETTINGBASE_XOVER_H
#define DEVSETTINGBASE_XOVER_H

#include "protocal/packet.h"
#include "customWidget/overwriteitem.h"

namespace Ui {
class DevSettingBase_Xover;
}

class DevSettingBase_Xover : public DevSettingDlg
{
    Q_OBJECT

public:
    enum {
        DevSettingBase_Xover_LP = 0,
        DevSettingBase_Xover_HP,
        DevSettingBase_Xover_BandPass,
    };
    explicit DevSettingBase_Xover(uint8_t ch, uint8_t type, struct ConfigData_Xover d, QWidget *parent = 0);
    ~DevSettingBase_Xover();

private slots:
    void spinBoxValueChanged(int value);
    void comboBoxValueChanged(int value);
    void pushButtonClicked(bool enable);

signals:
    void valueChanged(uint8_t, struct ConfigData_Xover);

private:
    Ui::DevSettingBase_Xover *ui;
    uint8_t _ch;
};

#endif // DEVSETTINGBASE_XOVER_H
