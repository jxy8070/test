#include "amp_4_4_route.h"
#include "ui_amp_4_4_route.h"
#include "device/device_amp_quad.h"
#include "device/device_rt1052_poe_60w_2_4.h"

Amp_4_4_Route::Amp_4_4_Route(Device_Amp_Quad *devAmp, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::Amp_4_4_Route)
{
    ui->setupUi(this);
    _devAmp = devAmp;

    ConfigData_Amp4_4_route route = _devAmp->getConfigData().Nout_route;

    if (route.r[0] == DESTINATION_N_SOURCE_A_IN)
    {
        ui->NOut1_AIn->setChecked(true);
    } else {
        ui->NOut1_AOut->setChecked(true);
    }

    if (route.r[1] == DESTINATION_N_SOURCE_A_IN)
    {
        ui->NOut2_AIn->setChecked(true);
    } else {
        ui->NOut2_AOut->setChecked(true);
    }

    if (route.r[2] == DESTINATION_N_SOURCE_A_IN)
    {
        ui->NOut3_AIn->setChecked(true);
    } else {
        ui->NOut3_AOut->setChecked(true);
    }

    if (route.r[3] == DESTINATION_N_SOURCE_A_IN)
    {
        ui->NOut4_AIn->setChecked(true);
    } else {
        ui->NOut4_AOut->setChecked(true);
    }

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui->NOut1_AIn, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut1_AOut, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut2_AIn, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut2_AOut, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut3_AIn, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut3_AOut, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut4_AIn, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut4_AOut, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
}

Amp_4_4_Route::~Amp_4_4_Route()
{
    delete ui;
}

void Amp_4_4_Route::pb_clicked(bool enable)
{
    quint8 ch = 0, mode = DESTINATION_N_SOURCE_A_IN;
    if (sender() == ui->NOut1_AIn)
    {
        ch = 0; mode = DESTINATION_N_SOURCE_A_IN;
    }
    else if (sender() == ui->NOut1_AOut)
    {
        ch = 0, mode = DESTINATION_N_SOURCE_A_OUT;
    }
    else if (sender() == ui->NOut2_AIn)
    {
        ch = 1; mode = DESTINATION_N_SOURCE_A_IN;
    }
    else if (sender() == ui->NOut2_AOut)
    {
        ch = 1, mode = DESTINATION_N_SOURCE_A_OUT;
    }
    else if (sender() == ui->NOut3_AIn)
    {
        ch = 2; mode = DESTINATION_N_SOURCE_A_IN;
    }
    else if (sender() == ui->NOut3_AOut)
    {
        ch = 2, mode = DESTINATION_N_SOURCE_A_OUT;
    }
    else if (sender() == ui->NOut4_AIn)
    {
        ch = 3; mode = DESTINATION_N_SOURCE_A_IN;
    }
    else if (sender() == ui->NOut4_AOut)
    {
        ch = 3, mode = DESTINATION_N_SOURCE_A_OUT;
    }

    _devAmp->sendRoute(ch, mode);
}
