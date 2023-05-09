#ifndef MONITORPROPERTY_H
#define MONITORPROPERTY_H

#include <QDialog>
#include <QStringList>
#include "device/devicemodel.h"
#include "device/devicemanager.h"
#include "device/AES70device.h"
#include "deviceItem/deviceItem.h"
#include "configItem/configitem.h"
#include "monitorblackstage.h"
#include "monitoritem.h"

namespace Ui {
class MonitorProperty;
}

class MonitorProperty : public QDialog
{
    Q_OBJECT

public:
#if 0
    explicit MonitorProperty(ConfigItem *cfgItem, QWidget *parent = 0);
#else
    explicit MonitorProperty(MonitorItem *cfgItem, MonitoringView *view, QWidget *parent = 0);
#endif
    ~MonitorProperty();

    void initDev();
    void closeEvent(QCloseEvent *);

    void addDevType(QStringList list);
signals:
    void sendSetData(CfgType, DevType, QString);
    void onLineClicked(bool);
private slots:
    void on_devChangeClicked(QString ip);
    void on_combox_portClicked(QString data);
    void on_ButtonClicked();
    void on_getySettingClicked(QString, QString);
    void on_settingCompoundData(QString, QString, QString, QString, QString, QString);
    void on_settingLiveStreamingData(QString, QString);
private:
    Ui::MonitorProperty *ui;

    MonitorBlackStage *_instance;
    QList<AES70Device *> _devList;
//    DeviceItem *_devItem;

    QStringList _list;  //端口列表
#if 0
    ConfigItem *_cfgItem;  //监控设备
#else
    MonitorItem *_monitorItem;  //监控设备
#endif
    AES70Device *_device;  //当前选择的网络设备类型
    DevType _devType;      //网络设备了类型
    QString _currentPort;  //当前选择的port
    MonitoringView *_view; //所在当前画布

};

#endif // MONITORPROPERTY_H
