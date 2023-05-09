#include "devsetting_firmwareupdate.h"
#include "ui_devsetting_firmwareupdate.h"
#include <QInputDialog>
#include <QFileDialog>
#include "device/AES70device.h"
#include "deviceItem/deviceItem.h"
#include "Protocal/packetbase.h"

#include "customWidget/overwriteitem.h"

DevSetting_FirmwareUpdate::DevSetting_FirmwareUpdate(AES70Device* device, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSetting_FirmwareUpdate)
{
    ui->setupUi(this);

    ui->lab_result->setText("");
    ui->progressBar->setValue(0);

    _devIp = device->getDeviceIp();
    _devMac = device->getAES70Mac();
    _udp = new FirmwareUdpServer(Config::instance()->getCurIp(), PacketBase::DEVICE_UDP_PORT1);
    connect(this, &DevSetting_FirmwareUpdate::progressValueChanged, ui->progressBar, &QProgressBar::setValue);

    ui->pBtn_update->setEnabled(false);
    ui->label_description->setText("");
}

DevSetting_FirmwareUpdate::~DevSetting_FirmwareUpdate()
{
    delete _udp;
    delete ui;
}

void DevSetting_FirmwareUpdate::AutoUpdate(QString fileName, uint *FW, quint8 *devVer)
{
    QString description = tr("Firmware Version: %1.%2.%3.%4 ---> %5.%6.%7.%8")
                          .arg(devVer[0]).arg(devVer[1]).arg(devVer[2]).arg(devVer[3])
                          .arg(FW[0]).arg(FW[1]).arg(FW[2]).arg(FW[3]);
    //ui
    setWindowFlags(Qt::FramelessWindowHint);
    ui->label_file->setHidden(true);
    ui->lineEdit->setHidden(true);
    ui->pBtn_open->setHidden(true);
    ui->pBtn_update->setHidden(true);
    ui->horizontalSpacer->changeSize(0,0);
    ui->horizontalLayout->update();
    ui->label_description->setText(description);

    //update
    getSoursceFile(fileName);
    on_pBtn_update_clicked();
}



void DevSetting_FirmwareUpdate::on_pBtn_open_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("open file"),
                                                    "",
                                                    tr("All files (*.ftcn)"));
    if(!fileName.isEmpty())
    {
        ui->lineEdit->setText(fileName);
        getSoursceFile(fileName);
        ui->pBtn_update->setEnabled(true);
    }
}

#include <QtConcurrent/QtConcurrent>
void DevSetting_FirmwareUpdate::on_pBtn_update_clicked()
{
//    if(!ui->lineEdit->text().endsWith(".ftcn")) return;

    // 检测升级文件与设备是否匹配
    Update_Online_Udp_t recvPack;
    bool rs = _udp->getDevInfo(_devIp, recvPack);
    if (!rs || recvPack.h.dev_type != _fileHeader.dev_type)
    {
        ui->pBtn_update->setEnabled(true);
        ui->pBtn_close->setEnabled(true);
        ui->pBtn_open->setEnabled(true);
        ui->lab_result->setText("This File not support update this device!");
        return;
    }

    //开始升级
    ui->pBtn_update->setEnabled(false);
    ui->pBtn_close->setEnabled(false);
    ui->pBtn_open->setEnabled(false);
    ui->lab_result->setText(tr("Update, Do not operate!"));
    ui->progressBar->setValue(0);

    bool isSucceed = true;
    QtConcurrent::run([&](bool isSucceed){
        for(quint32 blkIdx = 1; blkIdx <= _totalBlkCount; blkIdx++)
        {
            QByteArray fileBlkBuff = _fileBodyArray.mid(1024*(blkIdx-1), 1024);
            if(fileBlkBuff.length()< 1024){
                fileBlkBuff.append(1024-fileBlkBuff.length(), 0xFF);
            }
            firmSecion_t blkBody;
            memset(&blkBody, 0, sizeof(Update_Online_Udp_Body_t));
            blkBody.total_blk = _totalBlkCount;
            blkBody.current_blk = blkIdx;
            blkBody.update_type = _fileHeader.update_type;
            blkBody.src_ip      = QHostAddress(Config::instance()->getCurIp()).toIPv4Address();
            memcpy(blkBody.data, fileBlkBuff.data(), fileBlkBuff.size());

            int retryCnt = 0;
            while(retryCnt < MAX_RETRY) { //最多MAX_RETRY次重发
                bool rs = _udp->sendFirmUpdateBlock(_devIp, blkBody);
                if(rs){ //成功则发送下一个包
//                    ui->progressBar->setValue(blkIdx);
                    emit progressValueChanged(blkIdx);
                    isSucceed = true;
                    break;
                }
                else{   //失败则再次尝试
                    qDebug()<<QString("update block(%1) failed(%2Times)").arg(blkIdx).arg(retryCnt +1);
                    retryCnt++;
                }
            }
            if(retryCnt == MAX_RETRY){
                isSucceed = false;
                break;
            }
        }

        ui->pBtn_update->setEnabled(true);
        ui->pBtn_close->setEnabled(true);
        ui->pBtn_open->setEnabled(true);
        if(isSucceed) {
            ui->lab_result->setText(tr("Device Update Success, Please reboot it!"));
        }
        else{
            ui->lab_result->setText(tr("Device Update Failed, Please retry!"));
        }
    }, isSucceed);

}

void DevSetting_FirmwareUpdate::on_pBtn_close_clicked()
{
    close();
}


void DevSetting_FirmwareUpdate::getSoursceFile(QString fileName)
{
    QFile src(fileName);
    src.open(QIODevice::ReadOnly);
    QByteArray fileHeader = src.read(sizeof(Update_Online_File_Header_t));
    memset(&_fileHeader, 0, sizeof(Update_Online_File_Header_t));
    memcpy(&_fileHeader, fileHeader.data(), fileHeader.length());
    _fileBodyArray   = src.readAll();
    _totalBlkCount    = (_fileBodyArray.size() + 1023) / 1024;
    ui->progressBar->setRange(0, _totalBlkCount-1);
}




