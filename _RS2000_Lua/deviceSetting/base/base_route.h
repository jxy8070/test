#ifndef BASE_AP400_ROUTE_H
#define BASE_AP400_ROUTE_H

#include "customWidget/overwriteitem.h"

namespace Ui {
class Base_Route_AP400;
}
class Device_RT1052_AP400;
class Base_Route_AP400 : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit Base_Route_AP400(Device_RT1052_AP400 *dev, QWidget *parent = 0);
    ~Base_Route_AP400();
private slots:
    void pb_clicked(bool enable);

private:
    enum {
        ROUTE_SOURCE_A_IN1 = 0,
        ROUTE_SOURCE_N_IN1,
    };
    Ui::Base_Route_AP400 *ui;
    Device_RT1052_AP400 *_dev;
};

#endif // BASE_AP400_ROUTE_H
