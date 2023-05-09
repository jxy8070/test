#ifndef SETMULTICAST_H
#define SETMULTICAST_H

#include "customWidget/overwriteitem.h"
#include <QCheckBox>

namespace Ui {
class SetMulticast;
}

class DeviceItem;
class SetMulticast : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit SetMulticast(QWidget *parent = 0);
    SetMulticast(quint16 *num_slots, quint32 *id, quint8 port_num, DeviceItem *device, QWidget *parent = 0);
    ~SetMulticast();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::SetMulticast *ui;
    QList<QCheckBox *> _checkbox;
    DeviceItem *_device;
    quint8 _port_num;
    quint16 *_num;
    quint32 *_id;
};

#endif // SETMULTICAST_H
