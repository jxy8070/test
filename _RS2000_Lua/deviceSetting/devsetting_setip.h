#ifndef DEVSETTING_SETIP_H
#define DEVSETTING_SETIP_H

#include "customWidget/overwriteitem.h"
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include "Protocal/firmwareudpserver.h"

namespace Ui {
class DevSetting_SetIP;
}

class DeviceItem;
class DevSetting_SetIP : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit DevSetting_SetIP(AES70Device *device, QWidget *parent = 0);
    ~DevSetting_SetIP();
private slots:
    void on_btnSetIp_clicked();
    void on_btnClose_clicked();
    void on_btnGetIp_clicked();

private:
    Ui::DevSetting_SetIP *ui;
    FirmwareUdpServer *_udp;
    QString _devIp;
    QString _devMac;
};

#endif //DEVSETTING_SETIP_H
