#include "cfgsettinggpi.h"
#include "configItem/configitemgpi.h"

CfgSettingGPI::CfgSettingGPI(ConfigItemGPI *cfgItem, QWidget *parent) :
    QWidget(parent)
{
    _cfgItem = cfgItem;
    DevType devTyp = _cfgItem->getDevType();
    int gpiCnt = 4;
    if(devTyp == Dev_Type_NAP_X86_16Analog_128AES67)
    {
        gpiCnt = 8;
    }
    setupUi(gpiCnt);
    _cboxGpi->setCurrentIndex(_cfgItem->getGpiIdx());
}

CfgSettingGPI::~CfgSettingGPI()
{
    ;
}

void CfgSettingGPI::setupUi(int gpiCnt)
{
    QHBoxLayout *horizontalLayout;
    QLabel *label;

    setObjectName(QString::fromUtf8("CfgSettingGPI"));
    resize(205, 79);
    horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(10, -1, 16, -1);
    label = new QLabel(this);
    label->setObjectName(QString::fromUtf8("label"));
    label->setAlignment(Qt::AlignCenter);
    label->setText(tr("GPI"));
    horizontalLayout->addWidget(label);

    _cboxGpi = new QComboBox(this);
    for(int i=1; i<= gpiCnt; i++){
        _cboxGpi->addItem(QString("Gpi%1").arg(i));
    }
    _cboxGpi->setObjectName(QString::fromUtf8("comboBoxGpi"));
    connect(_cboxGpi, SIGNAL(currentIndexChanged(int)), this, SLOT(onGpiIndexChanged(int)));
    horizontalLayout->addWidget(_cboxGpi);
    horizontalLayout->setStretch(0, 1);
    horizontalLayout->setStretch(1, 2);

}

void CfgSettingGPI::updateUiData()
{
    _cboxGpi->setCurrentIndex(_cfgItem->getGpiIdx());
}

void CfgSettingGPI::onGpiIndexChanged(int index)
{
    _cfgItem->setGpiIdx(index);
}
