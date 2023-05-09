#include "devsetting_amp_dn8000.h"
#include "ui_devsetting_amp_dn8000.h"
#include "deviceItem/deviceitem_amp_dn8000.h"
#include "device/Device_Amp_DN8000.h"

DevSetting_Amp_DN8000::DevSetting_Amp_DN8000(DeviceItem_Amp_DN8000 *devItem, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSetting_Amp_DN8000)
{
    _devItem = devItem;
    _devAmp = qobject_cast<Device_Amp_DN8000*>(devItem->getAES70Device());
    ui->setupUi(this);
    initChannelUi();
    connect(ui->btnBlinkLed, &QPushButton::clicked, _devAmp, &Device_Amp_DN8000::sendBlinkLedCmd);
    connect(ui->btnStandby, &QPushButton::clicked, _devAmp, &Device_Amp_DN8000::setStandbyState);
    connect(_devAmp, &Device_Amp_DN8000::channelStateChanged, this, &DevSetting_Amp_DN8000::onChannelStateChanged);
    connect(_devAmp, &Device_Amp_DN8000::amplifierParamChanged, this, &DevSetting_Amp_DN8000::onAmpParamChanged);
    connect(_devAmp, &Device_Amp_DN8000::deviceStatusChanged, this, &DevSetting_Amp_DN8000::onDeviceStateChanged);
    if(_devAmp->isOnline()){
        sendInitPackets();
    }
}

DevSetting_Amp_DN8000::~DevSetting_Amp_DN8000()
{
    delete ui;
}


void DevSetting_Amp_DN8000::initChannelUi()
{
    for(int i = 0; i< ChannelCount; i++)
    {
        QString chName = QString("Channel %1").arg((char)('A' +i));
        ui->chGridLayout->addWidget(new QLabel(chName), i+1, 0, 1, 1, Qt::AlignHCenter);
        QPushButton* muteBtn = new QPushButton();
        muteBtn->setCheckable(true);
        muteBtn->setFixedSize(40, 20);
        muteBtn->setProperty("chNum", i);
        _muteBtnList.append(muteBtn);
        connect(muteBtn, &QPushButton::clicked, this, &DevSetting_Amp_DN8000::onChannelMuteClick);
        ui->chGridLayout->addWidget(muteBtn, i+1, 1, 1, 1, Qt::AlignHCenter);

        QPushButton* faultBtn = new QPushButton();
        faultBtn->setCheckable(true);
        faultBtn->setEnabled(false);
        faultBtn->setFixedSize(20, 20);
        _faultBtnList.append(faultBtn);
        ui->chGridLayout->addWidget(faultBtn, i+1, 2, 1, 1, Qt::AlignHCenter);

        QPushButton* clipBtn = new QPushButton();
        clipBtn->setCheckable(true);
        clipBtn->setEnabled(false);
        clipBtn->setFixedSize(20, 20);
        _clipBtnList.append(clipBtn);
        ui->chGridLayout->addWidget(clipBtn, i+1, 3, 1, 1, Qt::AlignHCenter);

        QPushButton* sigBtn = new QPushButton();
        sigBtn->setCheckable(true);
        sigBtn->setEnabled(false);
        sigBtn->setFixedSize(20, 20);
        _sigBtnList.append(sigBtn);
        ui->chGridLayout->addWidget(sigBtn, i+1, 4, 1, 1, Qt::AlignHCenter);

        QPushButton* protectBtn = new QPushButton();
        protectBtn->setCheckable(true);
        sigBtn->setEnabled(false);
        protectBtn->setFixedSize(20, 20);
        _protectBtnList.append(protectBtn);
        ui->chGridLayout->addWidget(protectBtn, i+1, 5, 1, 1, Qt::AlignHCenter);

        QLineEdit* tempEdit = new QLineEdit();
        tempEdit->setReadOnly(true);
        tempEdit->setFixedWidth(50);
        _tempEditList.append(tempEdit);
        ui->chGridLayout->addWidget(tempEdit, i+1, 6, 1, 1, Qt::AlignHCenter);

        QLineEdit* valEdit = new QLineEdit();
        valEdit->setReadOnly(true);
        valEdit->setFixedWidth(50);
        _valEditList.append(valEdit);
        ui->chGridLayout->addWidget(valEdit, i+1, 7, 1, 1, Qt::AlignHCenter);

        QLineEdit* elecEdit = new QLineEdit();
        elecEdit->setReadOnly(true);
        elecEdit->setFixedWidth(50);
        _elecEditList.append(elecEdit);
        ui->chGridLayout->addWidget(elecEdit, i+1, 8, 1, 1, Qt::AlignHCenter);
    }
}


void DevSetting_Amp_DN8000::sendInitPackets()
{
    _devAmp->sendGetVersionCmd();
    _devAmp->getStandbyState();
    for(int i = 0; i< ChannelCount; i++){
//        _devAmp->setChannelProtect(i+1, i%3==0);
        _devAmp->getChannelMuteState(i+1);
    }
}

void DevSetting_Amp_DN8000::onChannelStateChanged(ChState* pData)
{
    bool mute[8] = {true, true, true, true, true, true, true, true};
    for(int i = 0; i< ChannelCount; i++)
    {
        mute[i] = pData[i].isMute;
        _muteBtnList[i]->setChecked(pData[i].isMute);
        _faultBtnList[i]->setChecked(pData[i].fault);
        _clipBtnList[i]->setChecked(pData[i].isClip);
        _sigBtnList[i]->setChecked(pData[i].hasSignel);
        _protectBtnList[i]->setChecked(pData[i].protect);
        _tempEditList[i]->setText(QString::number(pData[i].temprate));
        _valEditList[i]->setText(QString::number(pData[i].valtage));
        _elecEditList[i]->setText(QString::number(pData[i].electricity));
    }
}

void DevSetting_Amp_DN8000::onAmpParamChanged()
{
    ui->btnStandby->setChecked(_devAmp->isAmpStandby());
    ui->txtVersion->setText(_devAmp->getVersion());
    ui->txtAmpVersion->setText(_devAmp->getAmpVersion());
    //显示PTP信息
    Device_Amp_DN8000::PTP_State& ptpInfo = _devAmp->getPTPState();
    unsigned char* pMac = ptpInfo.GrandMasterID;
    QString grandId = QString("%1-%2-%3-%4-%5-%6").arg(pMac[0], 2, 16, QChar('0')).arg(pMac[1], 2, 16, QChar('0'))
            .arg(pMac[2], 2, 16, QChar('0')).arg(pMac[5], 2, 16, QChar('0'))
            .arg(pMac[6], 2, 16, QChar('0')).arg(pMac[7], 2, 16, QChar('0'));
    ui->txtGrandMaster->setText(grandId);
    QString masterType;
    if(ptpInfo.State == 0) masterType = "PTP_INITIALIZING";
    else if(ptpInfo.State == 1) masterType = "PTP_FAULTY";
    else if(ptpInfo.State == 2) masterType = "PTP_DISABLED";
    else if(ptpInfo.State == 3) masterType = "PTP_LISTENING";
    else if(ptpInfo.State == 4) masterType = "PTP_PRE_MASTER";
    else if(ptpInfo.State == 5) masterType = "PTP_MASTER";
    else if(ptpInfo.State == 6) masterType = "PTP_PASSIVE";
    else if(ptpInfo.State == 7) masterType = "PTP_UNCALIBRATED";
    else if(ptpInfo.State == 8) masterType = "PTP_SLAVE";
    ui->txtClockMaster->setText(masterType);
    QString offset = QString("%1PPM").arg(ptpInfo.Offset);
    ui->txtOffset->setText(offset);
}

void DevSetting_Amp_DN8000::onChannelMuteClick(bool checked)
{
    if(_devAmp->isOnline()){
        QPushButton* muteBtn = qobject_cast<QPushButton*>(sender());
        quint8 ch = muteBtn->property("chNum").toUInt();
        _devAmp->setChannelMuteState(ch + 1, checked);
    }
}

void DevSetting_Amp_DN8000::onDeviceStateChanged(DevStatus devStatus)
{
    if(devStatus == Dev_Online){
        sendInitPackets();
    }
}


