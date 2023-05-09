#ifndef DEVSETTING_FIRMWAREUPDATE_H
#define DEVSETTING_FIRMWAREUPDATE_H

#include "customWidget/overwriteitem.h"
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include "Protocal/firmwareudpserver.h"

class DeviceItem;
namespace Ui {
class DevSetting_FirmwareUpdate;
}

class AES70Device;
class DevSetting_FirmwareUpdate : public DevSettingDlg
{
    Q_OBJECT
public:
    explicit DevSetting_FirmwareUpdate(AES70Device *device, QWidget *parent = 0);
    ~DevSetting_FirmwareUpdate();

    void AutoUpdate(QString fileName, uint *FW, quint8 *devVer);

signals:
    void progressValueChanged(int);
private slots:
    void on_pBtn_open_clicked();
    void on_pBtn_update_clicked();
    void on_pBtn_close_clicked();
    void getSoursceFile(QString fileName);

private:
    FirmwareUdpServer *_udp;
    QString _devIp;
    QString _devMac;
    Update_Online_File_Header_t _fileHeader;
    QByteArray _fileBodyArray;
    quint32 _totalBlkCount;

    Ui::DevSetting_FirmwareUpdate *ui;
};

#endif // DEVSETTING_FIRMWAREUPDATE_H
