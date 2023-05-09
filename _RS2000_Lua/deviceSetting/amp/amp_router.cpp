#include "amp_router.h"
#include "ui_amp_router.h"
#include "stable.h"
#include "device/device_stm32_poe_0_2.h"

AmpRouter::AmpRouter(Device_Amp *devAmp, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::AmpRouter)
{
    ui->setupUi(this);
    setFixedSize(size());
    _devAmp = devAmp;

//    ControlValue cntvlu = _devAmp->getConfigData().ch1_ControlValue;
//    if(cntvlu.channel == 0x0) ui->ch1line1out->setChecked(true);
//    else if(cntvlu.channel == 0x1) ui->ch1line2out->setChecked(true);
//    else if(cntvlu.channel == 0x2) ui->ch1line12out->setChecked(true);
//    else if(cntvlu.channel == 0x3) ui->ch1net1out->setChecked(true);
//    else if(cntvlu.channel == 0x4) ui->ch1net2out->setChecked(true);
//    else if(cntvlu.channel == 0x5) ui->ch1net12out->setChecked(true);

//    cntvlu = _devAmp->getConfigData().ch2_ControlValue;
//    if(cntvlu.channel == 0x0) ui->ch2line1out->setChecked(true);
//    else if(cntvlu.channel == 0x1) ui->ch2line2out->setChecked(true);
//    else if(cntvlu.channel == 0x2) ui->ch2line12out->setChecked(true);
//    else if(cntvlu.channel == 0x3) ui->ch2net1out->setChecked(true);
//    else if(cntvlu.channel == 0x4) ui->ch2net2out->setChecked(true);
//    else if(cntvlu.channel == 0x5) ui->ch2net12out->setChecked(true);

    connect(ui->ch1line1out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
    connect(ui->ch1line2out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
    connect(ui->ch1line12out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
    connect(ui->ch1net1out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
    connect(ui->ch1net2out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
    connect(ui->ch1net12out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));

    connect(ui->ch2line1out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
    connect(ui->ch2line2out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
    connect(ui->ch2line12out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
    connect(ui->ch2net1out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
    connect(ui->ch2net2out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
    connect(ui->ch2net12out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
}

void AmpRouter::onRouterChanged()
{
    if(sender() == ui->ch1line1out)
    {
        _devAmp->sendRouter(Device_Amp::AMP_CH1, 0x00);
    }
    else if(sender() == ui->ch1line2out)
    {
        _devAmp->sendRouter(Device_Amp::AMP_CH1, 0x01);
    }
    else if(sender() == ui->ch1line12out)
    {
        _devAmp->sendRouter(Device_Amp::AMP_CH1, 0x02);
    }
    else if(sender() == ui->ch1net1out)
    {
        _devAmp->sendRouter(Device_Amp::AMP_CH1, 0x03);
    }
    else if(sender() == ui->ch1net2out)
    {
        _devAmp->sendRouter(Device_Amp::AMP_CH1, 0x04);
    }
    else if(sender() == ui->ch1net12out)
    {
        _devAmp->sendRouter(Device_Amp::AMP_CH1, 0x05);
    }

    else if(sender() == ui->ch2line1out)
    {
        _devAmp->sendRouter(Device_Amp::AMP_CH2, 0x00);
    }
    else if(sender() == ui->ch2line2out)
    {
        _devAmp->sendRouter(Device_Amp::AMP_CH2, 0x01);
    }
    else if(sender() == ui->ch2line12out)
    {
        _devAmp->sendRouter(Device_Amp::AMP_CH2, 0x02);
    }
    else if(sender() == ui->ch2net1out)
    {
        _devAmp->sendRouter(Device_Amp::AMP_CH2, 0x03);
    }
    else if(sender() == ui->ch2net2out)
    {
        _devAmp->sendRouter(Device_Amp::AMP_CH2, 0x04);
    }
    else if(sender() == ui->ch2net12out)
    {
        _devAmp->sendRouter(Device_Amp::AMP_CH2, 0x05);
    }
}
