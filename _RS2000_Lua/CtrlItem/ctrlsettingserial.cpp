#include "ctrlsettingserial.h"
#include "ui_ctrlsettingserial.h"

CtrlSettingSerial::CtrlSettingSerial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CtrlSettingSerial)
{
    ui->setupUi(this);
}

CtrlSettingSerial::~CtrlSettingSerial()
{
    delete ui;
}
