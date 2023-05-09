#include "stable.h"
#include "ui_monitorwindows.h"
#include "monitorwindows.h"
#include "monitor_spk.h"
#include "monitor_spk_1.h"
#include "monitor_spk_2.h"
#include "monitor_amp.h"
#include "frame/projectscene.h"
#include "deviceItem/deviceItem.h"
#include "device/device_spk.h"
#include "device/device_stm32_poe_0_2.h"

MonitorWindows::MonitorWindows(ProjectScene *projScene, QWidget *parent) :
    QScrollArea(parent)
{
    _projScene = projScene;
    connect(projScene, SIGNAL(monitorChanged()), this, SLOT(flashMonitor()));
//    setAttribute(Qt::WA_DeleteOnClose);
//    resize(1150,750);
//    QWidget *widget = new QWidget(this);
//    _mainLayout = new QHBoxLayout(widget);
//    widget->resize(1150,750);
//    setWidget(widget);

    ui = new Ui::MonitorWindows;
    ui->setupUi(this);

    connect(ui->next, SIGNAL(clicked()), this, SLOT(onPushbuttonClicked()));
    connect(ui->pre, SIGNAL(clicked()), this, SLOT(onPushbuttonClicked()));

    flashMonitor();
}

void MonitorWindows::flashMonitor()
{
    qDeleteAll(_monitorList);//使用c++中的delete操作符删除范围[开始，结束]中的所有项，项目类型必须是指针类型
    _monitorList.clear();

    _curIndex = 0;

    foreach (QGraphicsItem *item, _projScene->items())//获得场景中的所有项目
    {
        DeviceItem* devItem = qgraphicsitem_cast<DeviceItem *>(item);
        if(devItem!= NULL && devItem->isMonitor())
        {
            if(devItem->getType() == Dev_Type_AES67_SPK)
            {
                Device_Spk *devSpk = qobject_cast<Device_Spk *>(devItem->getAES70Device());
                Monitor_Spk *spkMonitor = new Monitor_Spk(devSpk, devItem, this);
                spkMonitor->setFixedSize(210, 693);
//                _mainLayout->addWidget(spkMonitor);
                ui->horizontalLayout->addWidget(spkMonitor);
                _monitorList.append(spkMonitor);
            }
            else if(devItem->getType() == Dev_Type_STM32_POE_AES67_SPK_0_2)
            {
                Device_Amp *devAmp = qobject_cast<Device_Amp *>(devItem->getAES70Device());
                Q_ASSERT(devAmp != NULL);
                Monitor_Amp *ampMonitor = new Monitor_Amp(devAmp, devItem, this);
                ampMonitor->setFixedSize(210, 693);
//                _mainLayout->addWidget(ampMonitor);
                ui->horizontalLayout->addWidget(ampMonitor, Qt::AlignLeft);
                _monitorList.append(ampMonitor);
            }
            else if (devItem->getType() == Dev_Type_STM32_POE_AES67_SPK_0_1)
            {

            }
            else if (devItem->getType() == Dev_Type_RT1052_POE_AES67_LINE_4_4)
            {

            }
            else if (devItem->getType() == Dev_Type_RT1052_POE_AES67_MIC_4_0)
            {

            }
            else if (devItem->getType() == Dev_Type_AES67_MONITOR_SPK)
            {

            }
            _monitorList.last()->hide();
        }
    }

    monitorShow();
}

void MonitorWindows::onPushbuttonClicked()
{
    int endi = _monitorList.count();
    if (_monitorList.count() > ((_curIndex + 1) * 5))
    {
        endi = ((_curIndex + 1) * 5);
    }
    for (int i = _curIndex * 5; i < endi; ++i)
    {
        _monitorList.at(i)->hide();
    }

    if (sender() == ui->next)
    {
        _curIndex++;
    }
    else if (sender() == ui->pre)
    {
        _curIndex--;
    }

    monitorShow();
}

void MonitorWindows::monitorShow()
{
    int endi = _monitorList.count();
    if (_monitorList.count() > ((_curIndex + 1) * 5))
    {
        endi = ((_curIndex + 1) * 5);
    }
    for (int i = _curIndex * 5; i < endi; ++i)
    {
        _monitorList.at(i)->show();
    }

    ui->label->setText(QString("%1/%2")
                       .arg(_curIndex+1)
                       .arg(_monitorList.count()/5 +
                            (_monitorList.count() % 5 == 0 ? 0 : 1)
                            )
                       );

    quint32 idxEnd = _monitorList.count() / 5 + (_monitorList.count() % 5 == 0 ? 0 : 1);

    ui->next->setHidden(false);
    ui->pre->setHidden(false);
    if (_curIndex == 0)
    {
        ui->pre->setHidden(true);

        if (_monitorList.count() <= 5)
        {
            ui->next->setHidden(true);
        } else {
            ui->next->setHidden(false);
        }
    }
    else if ((_curIndex + 1) == idxEnd)
    {
        ui->next->setHidden(true);
    }
}
