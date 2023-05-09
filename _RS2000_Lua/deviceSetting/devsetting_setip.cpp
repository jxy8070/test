#include "devsetting_setip.h"
#include "ui_devsetting_setip.h"
#include "device/AES70device.h"
#include "deviceItem/deviceitem_trn.h"
#include <QHostAddress>
#include <QMessageBox>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include "Protocal/packetbase.h"
#include "Frame/mainwindow.h"

//交换机状态：redundant状态 B网口是备网，switched是ABCD网口互通
//ip修改成功后，重启设备
DevSetting_SetIP::DevSetting_SetIP(AES70Device *device, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSetting_SetIP)
{
    ui->setupUi(this);
    _devIp = device->getDeviceIp();
    _devMac = device->getAES70Mac();
    setWindowTitle(tr("Ip Set Form"));
    _udp = new FirmwareUdpServer(Config::instance()->getCurIp(), PacketBase::DEVICE_UDP_PORT1);
}

DevSetting_SetIP::~DevSetting_SetIP()
{
    delete _udp;
    delete ui;
}


void DevSetting_SetIP::on_btnSetIp_clicked()
{
    //校验输入
    QString IP0 = ui->lineEdit->text();
    QString Netmask0 = ui->lineEdit_2->text();
    QString Gateway0 = ui->lineEdit_3->text();
    QString IP1 = ui->lineEdit_6->text();
    QString Netmask1 = ui->lineEdit_5->text();
    QString Gateway1 = ui->lineEdit_4->text();
    if(!Utils::RegCheck_IsIp(IP0) || !Utils::RegCheck_IsIp(Netmask0) || !Utils::RegCheck_IsIp(Gateway0)
            ||!Utils::RegCheck_IsIp(IP1)||!Utils::RegCheck_IsIp(Netmask1)||!Utils::RegCheck_IsIp(Gateway1))
    {
        return;
    }

    //初始化新Ip参数
    setIP_t ipNew;
    memset(&ipNew, 0, sizeof(ipNew));
    int data_len = sizeof(Update_Online_Udp_Header_t) + sizeof(setIP_t);
    ipNew.current_ip = QHostAddress(_devIp).toIPv4Address();
    ipNew.dhcp_0     = false;
    ipNew.ip_0       = QHostAddress(IP0).toIPv4Address();
    ipNew.netmask_0  = QHostAddress(Netmask0).toIPv4Address();
    ipNew.gateway_0  = QHostAddress(Gateway0).toIPv4Address();
    ipNew.dhcp_1     = false;
    ipNew.ip_1       = QHostAddress(IP1).toIPv4Address();
    ipNew.netmask_1  = QHostAddress(Netmask1).toIPv4Address();
    ipNew.gateway_1  = QHostAddress(Gateway1).toIPv4Address();
    ipNew.redundancy = ui->comboBox->currentIndex();

    QByteArray macHex = FirmwareUdpServer::parserMacStr(_devMac);
    memcpy(ipNew.current_MAC, macHex.data(), macHex.length());

    bool rs = _udp->setIpInfo(_devIp, ipNew);
    if(!rs){
        MainWindow::instance()->showNotice(tr("set ip failed!"));
    }
    else{
        MainWindow::instance()->showNotice(tr("IP set success."));
    }
}

void DevSetting_SetIP::on_btnClose_clicked()
{
    close();
}

void DevSetting_SetIP::on_btnGetIp_clicked()
{
    Update_Online_Udp_t recvPack;
    bool rs = _udp->getDevInfo(_devIp, recvPack);
    if(rs){
        devInfo_t devInfo = recvPack.b.searchDev;
        QString devIP0 = QHostAddress(devInfo.ip_0).toString();
        QString devNetmask0 = QHostAddress(devInfo.netmask_0).toString();
        QString devgw0 = QHostAddress(devInfo.gateway_0).toString();

        QString devIP1 = QHostAddress(devInfo.ip_1).toString();
        QString devNetmask1 = QHostAddress(devInfo.netmask_1).toString();
        QString devgw1 = QHostAddress(devInfo.gateway_1).toString();

        ui->lineEdit->setText(devIP0);
        ui->lineEdit_2->setText(devNetmask0);
        ui->lineEdit_3->setText(devgw0);
        ui->lineEdit_6->setText(devIP1);
        ui->lineEdit_5->setText(devNetmask1);
        ui->lineEdit_4->setText(devgw1);
        if (devInfo.redundancy < 2)
        {
            ui->comboBox->setCurrentIndex(devInfo.redundancy);
        }
    }
    else{
        MainWindow::instance()->showNotice(tr("get ip failed!"));
    }
}


