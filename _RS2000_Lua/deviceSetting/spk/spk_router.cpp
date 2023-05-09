#include "stable.h"
#include "spk_router.h"
#include "ui_spk_router.h"
#include "device/device_spk.h"

SpkRouter::SpkRouter(Device_Spk *devSpk, QWidget *parent) :
    DevSettingDlg(parent),
    _ui(new Ui::Spk_Router)
{
    _ui->setupUi(this);
    setFixedSize(size());
    _devSpk = devSpk;

    ControlValue cmmData = _devSpk->getConfigData().ch1_ControlValue;
    if(cmmData.channel == 0x1) _ui->line1Out->setChecked(true);
    else if(cmmData.channel == 0x2) _ui->net1Out->setChecked(true);
    else if(cmmData.channel == 0x3) _ui->net2Out->setChecked(true);
    else if(cmmData.channel == 0x4) _ui->net12Out->setChecked(true);

    connect(_ui->line1Out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
    connect(_ui->net1Out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
    connect(_ui->net2Out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
    connect(_ui->net12Out, SIGNAL(clicked()), this, SLOT(onRouterChanged()));
}

void SpkRouter::onRouterChanged()
{
    if(sender() == _ui->line1Out) _devSpk->sendRouter(0x01);
    if(sender() == _ui->net1Out)  _devSpk->sendRouter(0x02);
    if(sender() == _ui->net2Out)  _devSpk->sendRouter(0x03);
    if(sender() == _ui->net12Out) _devSpk->sendRouter(0x04);
}
