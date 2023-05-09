#ifndef AMP_4_4_ROUTE_H
#define AMP_4_4_ROUTE_H

#include "customWidget/overwriteitem.h"

namespace Ui {
class Amp_4_4_Route;
}
class Device_Amp_Quad;
class Amp_4_4_Route : public DevSettingDlg
{
    Q_OBJECT

public:
    enum {
        DESTINATION_N_SOURCE_A_IN = 0,
        DESTINATION_N_SOURCE_A_OUT = 1,
    };
    explicit Amp_4_4_Route(Device_Amp_Quad *devAmp, QWidget *parent = 0);
    ~Amp_4_4_Route();

private slots:
    void pb_clicked(bool enable);

private:
    Ui::Amp_4_4_Route *ui;
    Device_Amp_Quad *_devAmp;
};
#endif // AMP_4_4_ROUTE_H
