#include "setmulticast.h"
#include "ui_setmulticast.h"
#include "deviceItem/deviceItem.h"
#include <QMessageBox>

SetMulticast::SetMulticast(QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::SetMulticast)
{
    ui->setupUi(this);
    _port_num = 0;
    _device = NULL;
    _num = NULL;
    _id = NULL;
}

SetMulticast::SetMulticast(quint16 *num_slots, quint32 *id, quint8 port_num, DeviceItem *device, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::SetMulticast)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, true);
    setWindowTitle("Multicast Config");
    _checkbox.append(ui->checkBox);
    _checkbox.append(ui->checkBox_2);
    _checkbox.append(ui->checkBox_3);
    _checkbox.append(ui->checkBox_4);
    _checkbox.append(ui->checkBox_5);
    _checkbox.append(ui->checkBox_6);
    _checkbox.append(ui->checkBox_7);
    _checkbox.append(ui->checkBox_8);
    _checkbox.append(ui->checkBox_9);
    _checkbox.append(ui->checkBox_10);
    _checkbox.append(ui->checkBox_11);
    _checkbox.append(ui->checkBox_12);
    _checkbox.append(ui->checkBox_13);
    _checkbox.append(ui->checkBox_14);
    _checkbox.append(ui->checkBox_15);
    _checkbox.append(ui->checkBox_16);

    for (int i = 0; i < 16; ++i)
    {
        _checkbox.at(i)->setVisible(false);
    }

    for (int i = 0; i < device->getMulticastPortList().count(); ++i)
    {
        _checkbox.at(device->getMulticastPortList().at(i))->setChecked(true);
    }

    _port_num = port_num;
    _device = device;
    _num = num_slots;
    _id = id;

    for (int i = 0; i < _port_num; ++i)
    {
        _checkbox.at(i)->setVisible(true);
    }
}

SetMulticast::~SetMulticast()
{
    delete ui;
}

void SetMulticast::on_pushButton_clicked()
{
//    int ret = QMessageBox::warning(NULL, tr("Multicast Config"), tr("Please power off the SPK for 1 minute!"), QMessageBox::Yes | QMessageBox::No);
//    if (ret == QMessageBox::Yes)
    {
        int offset = 0;
        _device->MulticastPortClear();
        for (int i = 0; i < _port_num; ++i)
        {
            if (_checkbox.at(i)->isChecked())
            {
                _id[offset++] = i+1;
                _device->MulticastPortAppend(i);
            }
        }

        *_num = offset;
    }
    close();
}

void SetMulticast::on_pushButton_2_clicked()
{
    close();
}
