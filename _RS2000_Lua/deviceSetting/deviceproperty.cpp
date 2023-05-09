#include "stable.h"
#include "deviceproperty.h"
#include "Frame/projectscene.h"
#include "deviceItem/deviceItem.h"
#include "device/devicemanager.h"
#include "device/AES70device.h"
#include <QListWidget>

DeviceProperty::DeviceProperty(DeviceItem *devItem, QWidget *parent) :
    DevSettingDlg(parent),
    _ui(new Ui::DeviceProperty)
{
    _ui->setupUi(this);
    setModal(true);//模态对话框

    _devItem = devItem;
    _bindDev = _devItem->getAES70Device();

    _ui->InputNum->setText(QString::number(_devItem->getInPortCount()));
    _ui->Type->setText(_devItem->getTypeName());
    _ui->OutputNum->setText(QString::number(_devItem->getOutPortCount()));

    if(_bindDev != NULL) {
        _ui->txtMac->setText(_bindDev->getAES70Mac());
        _ui->txtIP->setText(_bindDev->getDeviceIp());
        _ui->txtName->setText(_bindDev->getDeviceName());
        _ui->MfrsName->setText(_devItem->getMfrsName());
        _ui->lineEdit_FMWVersion->setText(tr("Loading..."));
        if(_bindDev->isOnline()){
            _ui->btnBindDev->setEnabled(false);
        }
    }
    else {
        _ui->btnBindDev->setEnabled(true);
    }

    connect(_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onOkCancel(QAbstractButton*)));
    if(_devItem->getAES70Device()){
        connect(_devItem->getAES70Device(), SIGNAL(sendDSPVersion(QString)), _ui->lineEdit_FMWVersion, SLOT(setText(QString)));
    }
}


DeviceProperty::~DeviceProperty()
{

}

void DeviceProperty::closeEvent(QCloseEvent *event)
{
    QDialog::closeEvent(event);
}

void DeviceProperty::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    default:
        break;
    }

    QDialog::keyPressEvent(event);
}



void DeviceProperty::onOkCancel(QAbstractButton *button)
{
    Q_UNUSED(button)
    Q_ASSERT(_devItem->isNetworkDevice());
    if (button == _ui->buttonBox->button(QDialogButtonBox::Ok)) {
        AES70Device* oldDev = NULL;
        if (_devItem->isBind()) {
            oldDev = _devItem->getAES70Device();
        }
        //把旧设备从Porject移除,
        if(oldDev != NULL && oldDev != _bindDev){
            oldDev->addedToProject(false);
            oldDev->setCurRoomName(QString());
        }

        //绑定设备
        if(_bindDev == NULL){
            Utils::MessageBoxWarning(tr("Warning"),tr("no device to bind!"));
            return;
        }
        else{
            _devItem->bindAES70Device(_bindDev);
        }

        _devItem->update();
    }
    else if (button == _ui->buttonBox->button(QDialogButtonBox::Cancel)) {

    }

    close();
}


void DeviceProperty::on_btnBindDev_clicked()
{
    ProjectScene * scene = qobject_cast<ProjectScene *>(_devItem->scene());
    _devList = scene->getNotBindDevList(_devItem->getType());
    _devSelectTabel = new QTableWidget();
    _devSelectTabel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _devSelectTabel->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _devSelectTabel->setFixedSize(600,300);
    _devSelectTabel->setAttribute(Qt::WA_ShowModal, true);
    _devSelectTabel->setSortingEnabled(false);
    _devSelectTabel->setStyleSheet("background:transparent; *{background:transparent;}");
    _devSelectTabel->setSelectionMode(QTableWidget::SingleSelection);
    _devSelectTabel->setSelectionBehavior(QTableWidget::SelectRows);
    _devSelectTabel->setAttribute(Qt::WA_ShowModal,true);
    _devSelectTabel->setColumnCount(3);
//    _devSelectTabel->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    _devSelectTabel->setHorizontalHeaderLabels(QStringList()<<tr("devName")<<tr("IP")<<tr("MAC"));
    _devSelectTabel->setRowCount(_devList.count());
    int rowIdx = 0;
    for(AES70Device* dev : _devList)
    {
        _devSelectTabel->setItem(rowIdx, 0, new QTableWidgetItem(dev->getDeviceName()));
        _devSelectTabel->setItem(rowIdx, 1, new QTableWidgetItem(dev->getDeviceIp()));
        _devSelectTabel->setItem(rowIdx, 2, new QTableWidgetItem(dev->getAES70Mac()));
        rowIdx++;
    }
    connect(_devSelectTabel, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onDeviceSelected(int, int)));

    _devSelectTabel->show();
}


void DeviceProperty::onDeviceSelected(int row, int column)
{
    if(row >= 0 && row < _devList.count())
    {
        _bindDev = _devList[row];
        _ui->txtMac->setText(_bindDev->getAES70Mac());
        _ui->txtIP->setText(_bindDev->getDeviceIp());
        _ui->txtName->setText(_bindDev->getDeviceName());
        _ui->MfrsName->setText(_bindDev->getMfrsName());
        if(_bindDev->isOnline()){
            _ui->btnBindDev->setEnabled(false);
        }
    }
    _devSelectTabel->close();
    _devSelectTabel= NULL;
}
