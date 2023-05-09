#ifndef AMP_ROUTER_H
#define AMP_ROUTER_H

#include "customWidget/overwriteitem.h"

namespace Ui {
class AmpRouter;
}
class Device_Amp;
class AmpRouter : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit AmpRouter(Device_Amp *devAmp, QWidget *parent = 0);
    QString getRouterInfo();

public slots:
    void onRouterChanged();

private:
    Ui::AmpRouter *ui;
    Device_Amp *_devAmp;
};

#endif // AMP_ROUTER_H
