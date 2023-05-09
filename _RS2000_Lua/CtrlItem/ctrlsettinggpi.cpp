#include "ctrlsettinggpi.h"
#include "ctrlitemgpi.h"
#include "device/device_trn.h"

CtrlSettingGpi::CtrlSettingGpi(CtrlItemGpi* ctrlItem, QWidget *parent) :
    QWidget(parent)
{
    _ctrlItem = ctrlItem;
    setupUi();
    ConfigScene* pScene = qobject_cast<ConfigScene*> (ctrlItem->scene());
    if(pScene != nullptr){
        Device_Trn* trnDev = pScene->getTrnDev();
        if(trnDev) {
            connect(trnDev, SIGNAL(InOutLevelSig(unsigned int *)), this, SLOT(setCtrlInfo(unsigned int *)));
        }
    }
}

CtrlSettingGpi::~CtrlSettingGpi()
{
    ;
}

void CtrlSettingGpi::setupUi()
{
    setObjectName(tr("CtrlSettingGpi"));
    resize(322, 80);
    QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(-1, 6, -1, -1);
    QGroupBox* groupBox = new QGroupBox(this);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    QGridLayout* gridLayout = new QGridLayout(groupBox);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    QLabel* labelIdx = new QLabel(groupBox);
    labelIdx->setObjectName(QString::fromUtf8("labelIdx"));
    labelIdx->setText(tr("Pin"));
    labelIdx->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(labelIdx, 0, 0, 1, 1);

    QLabel* labelState = new QLabel(groupBox);
    labelState->setObjectName(QString::fromUtf8("labelState"));
    labelState->setText(tr("State"));
    labelState->setMinimumSize(QSize(40, 0));
    gridLayout->addWidget(labelState, 1, 0, 1, 1);
    for(int i = 0; i< 8; i++)
    {
        QLabel* label = new QLabel(groupBox);
        label->setObjectName(QString("label%1").arg(i));
        label->setText(QString("%1").arg(i+1));
        label->setAlignment(Qt::AlignCenter);
        gridLayout->addWidget(label, 0, i+1, 1, 1);

        QPushButton* pin = new QPushButton(groupBox);
        pin->setObjectName(QString("pin%1").arg(i));
        pin->setEnabled(false);
        pin->setMinimumSize(QSize(24, 24));
        pin->setMaximumSize(QSize(24, 24));
        pin->setCheckable(true);
        gridLayout->addWidget(pin, 1, i+1, 1, 1);
        _pinList.append(pin);
    }
    horizontalLayout->addWidget(groupBox);
}


void CtrlSettingGpi::setCtrlInfo(unsigned int * pData)
{
    quint8 * pGpi = nullptr;
    DevType devType = _ctrlItem->getDevType();
    if(devType == Dev_Type_NAP_X86_16Analog_128AES67) {
        FC_RTU_InOutLevel128 *uvData = (FC_RTU_InOutLevel128 *)pData;
        pGpi = &uvData->gpi[0];
    }
    else if(devType == Dev_Type_NAP_X86_32Analog_256AES67) {
//        FC_RTU_InOutLevel256 *level = (FC_RTU_InOutLevel256 *)d;
//        pGpi = &level->gpi[0];
    }

    for(int i = 0; i< 8; i++){
        _pinList[i]->setChecked(pGpi[i]);
    }
}



