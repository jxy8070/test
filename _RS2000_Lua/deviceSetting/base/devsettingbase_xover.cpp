#include "devsettingbase_xover.h"
#include "ui_devsettingbase_xover.h"

DevSettingBase_Xover::DevSettingBase_Xover(uint8_t ch, uint8_t type, struct ConfigData_Xover d, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSettingBase_Xover)
{
    ui->setupUi(this);

    _ch = ch;
    ui->head->setText(tr("ch %1").arg(ch+1));

    if (type == DevSettingBase_Xover_LP)
    {
        ui->high_freq->setHidden(true);
        ui->high_stages->setHidden(true);
    } else if (type == DevSettingBase_Xover_HP) {
        ui->low_freq->setHidden(true);
        ui->low_stages->setHidden(true);
    }

    ui->low_freq->setValue(d.freq[0]);
    ui->low_stages->setCurrentIndex(d.stages[0]/2-1);
    ui->high_freq->setValue(d.freq[1]);
    ui->high_stages->setCurrentIndex(d.stages[1]/2-1);
    ui->bypass->setChecked(d.bypass);

    connect(ui->low_freq, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(ui->high_freq, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(ui->low_stages, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));
    connect(ui->high_stages, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));
    connect(ui->bypass, SIGNAL(clicked(bool)), this, SLOT(pushButtonClicked(bool)));
    connect(ui->close, SIGNAL(clicked(bool)), this, SLOT(pushButtonClicked(bool)));
}

DevSettingBase_Xover::~DevSettingBase_Xover()
{
    delete ui;
}

void DevSettingBase_Xover::spinBoxValueChanged(int value)
{
    if (sender() == ui->low_freq)
    {
        struct ConfigData_Xover xData;
        xData.bypass = ui->bypass->isChecked();
        xData.freq[0] = ui->low_freq->value();
        xData.freq[1] = ui->high_freq->value();
        xData.stages[0] = (ui->low_stages->currentIndex() + 1) * 2;
        xData.stages[1] = (ui->high_stages->currentIndex() + 1) * 2;
        emit valueChanged(_ch, xData);
    }
    else if (sender() == ui->high_freq)
    {
        struct ConfigData_Xover xData;
        xData.bypass = ui->bypass->isChecked();
        xData.freq[0] = ui->low_freq->value();
        xData.freq[1] = ui->high_freq->value();
        xData.stages[0] = (ui->low_stages->currentIndex() + 1) * 2;
        xData.stages[1] = (ui->high_stages->currentIndex() + 1) * 2;
        emit valueChanged(_ch, xData);
    }
}

void DevSettingBase_Xover::comboBoxValueChanged(int value)
{
    if (sender() == ui->low_stages)
    {
        struct ConfigData_Xover xData;
        xData.bypass = ui->bypass->isChecked();
        xData.freq[0] = ui->low_freq->value();
        xData.freq[1] = ui->high_freq->value();
        xData.stages[0] = (ui->low_stages->currentIndex() + 1) * 2;
        xData.stages[1] = (ui->high_stages->currentIndex() + 1) * 2;
        emit valueChanged(_ch, xData);
    }
    else if (sender() == ui->high_stages)
    {
        struct ConfigData_Xover xData;
        xData.bypass = ui->bypass->isChecked();
        xData.freq[0] = ui->low_freq->value();
        xData.freq[1] = ui->high_freq->value();
        xData.stages[0] = (ui->low_stages->currentIndex() + 1) * 2;
        xData.stages[1] = (ui->high_stages->currentIndex() + 1) * 2;
        emit valueChanged(_ch, xData);
    }
}

void DevSettingBase_Xover::pushButtonClicked(bool enable)
{
    if (sender() == ui->close)
    {
        close();
    }
    else if (sender() == ui->bypass)
    {
        struct ConfigData_Xover xData;
        xData.bypass = ui->bypass->isChecked();
        xData.freq[0] = ui->low_freq->value();
        xData.freq[1] = ui->high_freq->value();
        xData.stages[0] = (ui->low_stages->currentIndex() + 1) * 2;
        xData.stages[1] = (ui->high_stages->currentIndex() + 1) * 2;
        emit valueChanged(_ch, xData);
    }
}
