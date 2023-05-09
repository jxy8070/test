#include "base_route.h"

#include "ui_base_route_ap400.h"
#include "device/device_rt1052_AP400.h"
Base_Route_AP400::Base_Route_AP400(Device_RT1052_AP400 *dev, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::Base_Route_AP400)
{
    ui->setupUi(this);

    _dev = dev;
    switch (_dev->getConfigData().route.r[0]) {
    case ROUTE_SOURCE_A_IN1:
        ui->A_In1->setChecked(true);
        break;
    case ROUTE_SOURCE_N_IN1:
        ui->N_In1->setChecked(true);
        break;
    default:
        break;
    }

    connect(ui->A_In1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->N_In1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
}

Base_Route_AP400::~Base_Route_AP400()
{
    delete ui;
}

void Base_Route_AP400::pb_clicked(bool enable)
{
    if (sender() == ui->A_In1)
    {
        _dev->sendRoute(0, ROUTE_SOURCE_A_IN1);
    }
    else if (sender() == ui->N_In1)
    {
        _dev->sendRoute(0, ROUTE_SOURCE_N_IN1);
    }
}
