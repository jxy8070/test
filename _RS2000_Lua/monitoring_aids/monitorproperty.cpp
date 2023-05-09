#include "monitorproperty.h"
#include "ui_monitorproperty.h"
#include "utils.h"
#include "monitoring_aids/monitoringfactory.h"
#include <QMessageBox>
#include <QFontDialog>


MonitorProperty::MonitorProperty(MonitorItem *monitorItem, MonitoringView *view, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MonitorProperty)
{
    ui->setupUi(this);
    _monitorItem = monitorItem;
    _view = view;
    _devList = DeviceManager::instance()->getDeviceModel()->getDeviceList();
    _instance = MonitorBlackStage::instance();

    connect(ui->pushButton_ok, SIGNAL(clicked(bool)), this, SLOT(on_ButtonClicked()));
    connect(ui->pushButton_close, SIGNAL(clicked(bool)), this, SLOT(on_ButtonClicked()));
//    connect(ui->lineEdit_name, SIGNAL(), this, SLOT(on_ButtonClicked()));
//    connect(ui->pushButton_ForeColor, SIGNAL(clicked(bool)), this, SLOT(on_ButtonClicked()));
//    connect(ui->pushButton_otherColor, SIGNAL(clicked(bool)), this, SLOT(on_ButtonClicked()));
    connect(ui->pushButton_backColor, SIGNAL(clicked(bool)), this, SLOT(on_ButtonClicked()));
    connect(ui->pushButton_titleText, SIGNAL(clicked(bool)), this, SLOT(on_ButtonClicked()));
    connect(ui->pushButton_TextSet, SIGNAL(clicked(bool)), this, SLOT(on_ButtonClicked()));
    connect(_instance, SIGNAL(sendCompoundMeter(QString,QString,QString,QString,QString,QString)),
            this, SLOT(on_settingCompoundData(QString,QString,QString,QString,QString,QString)));
    connect(_instance, SIGNAL(sendLiveStreamingData(QString,QString)), this, SLOT(on_settingLiveStreamingData(QString,QString)));

    if(_monitorItem->getType() != CFG_TYPE_4 && _monitorItem->getType() != CFG_TYPE_5 /*&& _monitorItem->getType() != CFG_TYPE_SCENE*/)
    {
        connect(_instance, SIGNAL(sendSettingData(QString,QString)), this, SLOT(on_getySettingClicked(QString,QString)));
        connect(ui->comboBox_ip, SIGNAL(currentTextChanged(QString)), this, SLOT(on_devChangeClicked(QString)));
        if(_monitorItem->getType() == CFG_TYPE_NETWORKDEVICE || _monitorItem->getType() == CFG_TYPE_SCENE)
            connect(ui->comboBox_type, SIGNAL(currentTextChanged(QString)), this, SLOT(on_combox_portClicked(QString)));
        connect(ui->comboBox_port, SIGNAL(currentTextChanged(QString)), this, SLOT(on_combox_portClicked(QString)));
        connect(ui->comboBox_clipper1, SIGNAL(currentTextChanged(QString)), this, SLOT(on_combox_portClicked(QString)));
        connect(ui->comboBox_clipper2, SIGNAL(currentTextChanged(QString)), this, SLOT(on_combox_portClicked(QString)));
        connect(ui->comboBox_button1, SIGNAL(currentTextChanged(QString)), this, SLOT(on_combox_portClicked(QString)));
        connect(ui->comboBox_button2, SIGNAL(currentTextChanged(QString)), this, SLOT(on_combox_portClicked(QString)));
        connect(ui->comboBox_button3, SIGNAL(currentTextChanged(QString)), this, SLOT(on_combox_portClicked(QString)));
        connect(ui->comboBox_button4, SIGNAL(currentTextChanged(QString)), this, SLOT(on_combox_portClicked(QString)));

        initDev();

        _instance->getSettingData(_monitorItem);
        ui->lineEdit_style->setText(_monitorItem->getDisplayStyle());
    }else{
        if(_monitorItem->getType() == CFG_TYPE_5)
        {
            _instance->getSettingData(_monitorItem);
        }
        ui->comboBox_ip->setEnabled(false);
        ui->comboBox_port->setEnabled(false);
        ui->lineEdit_name->setText(_monitorItem->getTypeName());
        ui->label_style->hide();
        ui->lineEdit_style->hide();
    }
    ////////////color//////////////////
    if(_monitorItem->getType() != CFG_TYPE_4 && _monitorItem->getType() != CFG_TYPE_3)
    {
        ui->groupBox_color->hide();
        ui->groupBox_textSize->hide();
    }else
    {
//        ui->pushButton_ForeColor->setPalette(QPalette(_monitorItem->foreColor()));
        ui->pushButton_backColor->setPalette(QPalette(_monitorItem->getFillColor()));
        ui->pushButton_titleText->setPalette(QPalette(_monitorItem->getTitilColor()));
        ui->label_textFont->setText(_monitorItem->textFont().family());
        ui->label_textsize->setNum(_monitorItem->textFont().pointSize());
    }
    ////////////////复合电平表//////////////////////
    if(_monitorItem->getType() == CFG_TYPE_1 && (_monitorItem->DisplayStyle() == 2 || _monitorItem->DisplayStyle() == 3))
    {
        ui->groupBox_clipper_and_button->setVisible(true);
    }else if(_monitorItem->getType() == CFG_TYPE_1 && _monitorItem->DisplayStyle() == 4)
    {
        ui->groupBox_clipper_and_button->setVisible(true);
        ui->label_button3->hide();
        ui->label_button4->hide();
        ui->comboBox_button3->hide();
        ui->comboBox_button4->hide();
    }else
    {
//        ui->label_port2->setVisible(false);
//        ui->comboBox_port2->setVisible(false);
        ui->groupBox_clipper_and_button->hide();
    }
    //////////////双声道电平表////////////////////
//    if(_monitorItem->getType() == CFG_TYPE_1 && _monitorItem->DisplayStyle() == 5)
//    {
//        ui->label_port2->setVisible(true);
//        ui->comboBox_port2->setVisible(true);
//        ui->groupBox_clipper_and_button->hide();
//    }


    ui->label_videoIP->hide();
    ui->lineEdit_videoIP->hide();
    ui->comboBox_type->setEnabled(false);
    ui->lineEdit_style->setEnabled(false);

    ui->pushButton_ok->setFocus();
    if(_monitorItem->getType() == CFG_TYPE_NETWORKDEVICE || _monitorItem->getType() == CFG_TYPE_SCENE){
        ui->comboBox_type->setEnabled(true);
        ui->comboBox_port->setEnabled(false);
    }
    if(_monitorItem->getType() == CFG_TYPE_5)
    {
        ui->comboBox_ip->hide();
        ui->comboBox_port->hide();
        ui->label_IP->hide();
        ui->label_port->hide();
        ui->label_style->setVisible(true);
        ui->lineEdit_style->setVisible(true);
        ui->lineEdit_style->setEnabled(true);
        ui->label_style->setText("VideoID");
        ui->lineEdit_videoIP->setVisible(true);
        ui->label_videoIP->setVisible(true);
    }
}


MonitorProperty::~MonitorProperty()
{
    delete ui;
}

void MonitorProperty::initDev()
{
//    qDebug()<<__FUNCTION__<<__LINE__<<"monitorItemName:"<<_monitorItem->getTypeName();
    QStringList devTypeList;
     ui->lineEdit_name->setText(_monitorItem->getTypeName());
//     connect(this, SIGNAL(onLineClicked(bool)), _monitorItem, SLOT(setOnLineClicked(bool)));

    foreach(AES70Device *dev, _devList)
    {
        if(!dev->isOnline() || !dev->isAddedToProject()) {
            continue;
        }
        switch(_monitorItem->getType())
        {
        case CFG_TYPE_1:
        case CFG_TYPE_2:
        case CFG_TYPE_3:
        case CFG_TYPE_4:
        case CFG_TYPE_5:
        {
//            qDebug()<<__FUNCTION__<<__LINE__<<dev->getDeviceIp();
            ui->comboBox_ip->addItem(dev->getDeviceIp());
            ui->comboBox_ip->setCurrentText(dev->getDeviceIp());
            ui->comboBox_type->addItem(Config::curMfrsCfg().getDevTypeName(dev->getDeviceType()));
            ui->comboBox_type->setCurrentText(Config::curMfrsCfg().getDevTypeName(dev->getDeviceType()));
            break;
        }
        case CFG_TYPE_SCENE:
//        {

//            break;
//        }
        case CFG_TYPE_NETWORKDEVICE:
        {
            devTypeList<<Config::curMfrsCfg().getDevTypeName(dev->getDeviceType());
            ui->lineEdit_name->setText(dev->getDeviceName());
            break;
        }
        default:
            break;
        }
        if(devTypeList.isEmpty())
        {
            qDebug()<<"No find device!";
//            devTypeList<<"None";
//            addDevType(devTypeList);
//            emit onLineClicked(false);
        }else{
            qDebug()<<"find the device:"<<dev;
            addDevType(devTypeList);
//            emit onLineClicked(true);
        }
    }
}

void MonitorProperty::closeEvent(QCloseEvent *)
{
    _devList.clear();
}

void MonitorProperty::addDevType(QStringList list)
{
//    ui->comboBox_type->setEnabled(true);
    ui->comboBox_ip->setEnabled(false);
//    ui->comboBox_port->setEnabled(false);
    ui->comboBox_type->clear();
    ui->comboBox_type->addItems(list);
    ui->comboBox_type->setCurrentIndex(0);
    if(!_monitorItem->getNetworkDevType().isEmpty())
    {
        ui->comboBox_type->setCurrentText(_monitorItem->getNetworkDevType());
    }
}
void MonitorProperty::on_devChangeClicked(QString ip)
{
//    qDebug()<<"---------------------"<<ip;
    foreach(AES70Device * dev, _devList)
    {
        if(dev->getDeviceIp() == ip)
        {
            _device = dev;
        }
    }
    ui->comboBox_type->addItem(Config::curMfrsCfg().getDevTypeName(_device->getDeviceType()));
    ui->comboBox_type->setCurrentText(Config::curMfrsCfg().getDevTypeName(_device->getDeviceType()));
    ui->lineEdit_name->setText(_monitorItem->getTypeName()/*_device->getDeviceName()*/);
    ui->comboBox_port->clear();

    ui->comboBox_port->addItems(MonitoringFactory::CreateInstance(_device->getDeviceType(), _monitorItem->getType()));
    ui->comboBox_port->setCurrentIndex(0);

    if(_monitorItem->getType() == CFG_TYPE_1 && (_monitorItem->DisplayStyle() == 2 || _monitorItem->DisplayStyle() == 3))
    {
        ui->comboBox_clipper1->clear();
        ui->comboBox_clipper2->clear();
        ui->comboBox_button1->clear();
        ui->comboBox_button2->clear();
        ui->comboBox_button3->clear();
        ui->comboBox_button4->clear();
        ui->comboBox_clipper1->addItems(MonitoringFactory::CreateInstance(_device->getDeviceType(), CFG_TYPE_2));
        ui->comboBox_clipper2->addItems(MonitoringFactory::CreateInstance(_device->getDeviceType(), CFG_TYPE_2));
        ui->comboBox_button1->addItems(MonitoringFactory::CreateInstance(_device->getDeviceType(), CFG_TYPE_3));
        ui->comboBox_button2->addItems(MonitoringFactory::CreateInstance(_device->getDeviceType(), CFG_TYPE_3));
        ui->comboBox_button3->addItems(MonitoringFactory::CreateInstance(_device->getDeviceType(), CFG_TYPE_3));
        ui->comboBox_button4->addItems(MonitoringFactory::CreateInstance(_device->getDeviceType(), CFG_TYPE_3));
    }
    if(_monitorItem->getType() == CFG_TYPE_1 && _monitorItem->DisplayStyle() == 4)
    {
        ui->comboBox_clipper1->clear();
        ui->comboBox_clipper2->clear();
        ui->comboBox_button1->clear();
        ui->comboBox_button2->clear();
        ui->comboBox_clipper1->addItems(MonitoringFactory::CreateInstance(_device->getDeviceType(), CFG_TYPE_2));
        ui->comboBox_clipper2->addItems(MonitoringFactory::CreateInstance(_device->getDeviceType(), CFG_TYPE_2));
        ui->comboBox_button1->addItems(MonitoringFactory::CreateInstance(_device->getDeviceType(), CFG_TYPE_3));
        ui->comboBox_button2->addItems(MonitoringFactory::CreateInstance(_device->getDeviceType(), CFG_TYPE_3));
    }
    ui->pushButton_ok->setFocus();
}

void MonitorProperty::on_combox_portClicked(QString data)
{
    if(sender() == ui->comboBox_port){
//        foreach(AES70Device *dev, _devList)
//        {
//            if(!dev->isOnline() || !dev->isAddedToProject()) { continue; }
//            if(dev->getDeviceName().isEmpty()) { continue; }
//            if(data == Utils::getDeviceModelName(dev->getDeviceType()))
//            {
//                _device = dev;
//                _devType = dev->getDeviceType();
//            }
//        }
//        if(!data.isEmpty())
//        {
//            _currentPort.clear();
//            _currentPort = data;
//        }
//        ui->pushButton_ok->setFocus();
    }
    if(sender() == ui->comboBox_type){
        disconnect(ui->comboBox_ip, SIGNAL(currentTextChanged(QString)), this, SLOT(on_devChangeClicked(QString)));
        foreach(AES70Device *dev, _devList)
        {
            if(!dev->isOnline() || !dev->isAddedToProject()) { continue; }

            if(Config::curMfrsCfg().getDevTypeName(dev->getDeviceType()) == data)
            {
                _device = dev;
                _devType = dev->getDeviceType();

                ui->comboBox_ip->addItem(dev->getDeviceIp());
                ui->comboBox_ip->setCurrentText(dev->getDeviceIp());
                ui->lineEdit_name->setText(dev->getDeviceName());
            }
        }
        connect(ui->comboBox_ip, SIGNAL(currentTextChanged(QString)), this, SLOT(on_devChangeClicked(QString)));
    }
    ui->pushButton_ok->setFocus();
}

void MonitorProperty::on_ButtonClicked()
{
    if(sender() == ui->pushButton_ok)
    {
        _monitorItem->setMonitorName(ui->lineEdit_name->text());
        if(_monitorItem->getType() != CFG_TYPE_NETWORKDEVICE && _monitorItem->getType() != CFG_TYPE_SCENE)
        {
            _monitorItem->setFillColor(ui->pushButton_backColor->palette().color(QPalette::Window));
            _monitorItem->setTitleColor(ui->pushButton_titleText->palette().color(QPalette::Window));
            if(ui->comboBox_ip->count() && _device != NULL){
                _instance->getMonitorData(_monitorItem->getID(), _view, _monitorItem->getType(), _device->getDeviceType(), _device->getDeviceIp(), ui->comboBox_port->currentText());
                //复合电平表
                if(_monitorItem->getType() == CFG_TYPE_1 && (_monitorItem->DisplayStyle() == 2 || _monitorItem->DisplayStyle() == 3))
                {
                    _instance->getMonitorData(_monitorItem->getID(), _view, _monitorItem->getType(), _device->getDeviceType(), _device->getDeviceIp(),ui->comboBox_port->currentText(),
                                              ui->comboBox_clipper1->currentText(), ui->comboBox_clipper2->currentText(),
                                              ui->comboBox_button1->currentText(),ui->comboBox_button2->currentText(), ui->comboBox_button3->currentText(),
                                              ui->comboBox_button4->currentText());
                }
                if(_monitorItem->getType() == CFG_TYPE_1 && _monitorItem->DisplayStyle() == 4)
                {
                    _instance->getMonitorData(_monitorItem->getID(), _view, _monitorItem->getType(), _device->getDeviceType(), _device->getDeviceIp(), ui->comboBox_port->currentText(),
                                              ui->comboBox_clipper1->currentText(), ui->comboBox_clipper2->currentText(), ui->comboBox_button1->currentText(),
                                              ui->comboBox_button2->currentText());
                }
            }else{
                if(_monitorItem->getType() == CFG_TYPE_4)
                {
                    _instance->getMonitorData(_monitorItem->getID(), _view, _monitorItem->getType(), Dev_Type_Unknow);
                }
                else if(_monitorItem->getType() == CFG_TYPE_5)
                {
                    qDebug()<<__FUNCTION__<<__LINE__<<ui->lineEdit_style->text()<<ui->lineEdit_videoIP->text();
                    _monitorItem->setVideoID(ui->lineEdit_style->text());
                    _instance->getMonitorData(_monitorItem->getID(), _view, _monitorItem->getType(), Dev_Type_Unknow, ui->lineEdit_videoIP->text());
                }
                else
                    QMessageBox::information(NULL, tr("Warning"), tr("No device online!"), QMessageBox::Ok);
            }
        }else{
            if(ui->comboBox_ip->count() && _device != NULL && ui->comboBox_type->count())
            {
                _monitorItem->setDisplayStyle(ui->comboBox_type->currentText());
                _monitorItem->setDevIP(ui->comboBox_ip->currentText());
                _monitorItem->setDevType(_devType);
                _instance->addOnlineDev_IP_Type(ui->comboBox_ip->currentText(), ui->comboBox_type->currentText());
                _instance->getMonitorData(_monitorItem->getID(), _view, _monitorItem->getType(), _device->getDeviceType(), _device->getDeviceIp());
            }else
            {
                QMessageBox::information(NULL, tr("warning"), tr("No device online!"), QMessageBox::Ok);
            }

        }

        close();
    }
    if(sender() == ui->pushButton_close)
    {
        this->close();
    }
//    if(sender() == ui->pushButton_ForeColor)
//    {
//        QColor color = _monitorItem->foreColor();
//        color = QColorDialog::getColor(color, this, "Select color");
//        if(color.isValid())
//        {
//           qDebug()<<"ForeColor is valid!";
//           QPushButton *button = qobject_cast<QPushButton *>(sender());
//           button->setPalette(QPalette(color));
//        }
//    }
    if(sender() == ui->pushButton_backColor)
    {
//        QPalette pal = ui->pushButton_backColor->palette();
//        QColor color = pal.color(QPalette::Button);
        QColor color = _monitorItem->getFillColor();
        color = QColorDialog::getColor(color/*, this, "Select color"*/);
        if(color.isValid())
        {
            qDebug()<<"BackColor is valid!";
           QPushButton *button = qobject_cast<QPushButton *>(sender());
           button->setPalette(QPalette(color));
        }
    }
    if(sender() == ui->pushButton_titleText)
    {
        QColor color = _monitorItem->getTitilColor();
        color = QColorDialog::getColor(color/*, this, "Select color"*/);
        if(color.isValid())
        {
           QPushButton *button = qobject_cast<QPushButton *>(sender());
           button->setPalette(QPalette(color));
        }
    }
//    if(sender() == ui->pushButton_otherColor)
//    {
//        QColor color = _monitorItem->foreColor();
//        color = QColorDialog::getColor(color, this, "select color");
//        if(color.isValid())
//        {
//           QPushButton *button = qobject_cast<QPushButton *>(sender());
//           button->setPalette(QPalette(color));
//        }
//    }
    if(sender() ==ui->pushButton_TextSet)
    {
        QFont font = _monitorItem->textFont();
        bool ok = false;
        font = QFontDialog::getFont(&ok, font);
        if(ok)
        {
            qDebug()<<"QfontDiglog";
            ui->label_textFont->setText(font.family());
            ui->label_textsize->setNum(font.pointSize());
            _monitorItem->setTextFout(font);
        }
    }
    if(sender() == ui->lineEdit_name)
    {
        ui->pushButton_ok->setFocus();
    }
}

void MonitorProperty::on_getySettingClicked(QString ip, QString port)
{
    foreach(AES70Device *dev, _devList)
    {
        if(dev->getDeviceIp() == ip)
        {
            _device = dev;
            qDebug()<<__FUNCTION__<<__LINE__<<"  "<<_device->getDeviceIp()<<" "
                   <<_device->getDeviceName()<<" "/*<<_device->getDeviceType*/<<" "
                  <<Config::curMfrsCfg().getDevTypeName(_device->getDeviceType());

            ui->comboBox_ip->setCurrentText(_device->getDeviceIp());
            ui->comboBox_ip->setFocus();
            ui->comboBox_port->clear();
            ui->comboBox_port->addItems(MonitoringFactory::CreateInstance(_device->getDeviceType(), _monitorItem->getType()));
            ui->comboBox_port->setCurrentText(port);
//            ui->comboBox_type->addItem(Utils::getDeviceModelName(_device->getDeviceType()));
            ui->comboBox_type->setCurrentText(Config::curMfrsCfg().getDevTypeName(_device->getDeviceType()));
        }
    }
}

void MonitorProperty::on_settingCompoundData(QString clipper1, QString clipper2, QString button1, QString button2, QString button3, QString button4)
{
    ui->comboBox_clipper1->setCurrentText(clipper1);
    ui->comboBox_clipper2->setCurrentText(clipper2);
    ui->comboBox_button1->setCurrentText(button1);
    ui->comboBox_button2->setCurrentText(button2);
    ui->comboBox_button3->setCurrentText(button3);
    ui->comboBox_button4->setCurrentText(button4);
}

void MonitorProperty::on_settingLiveStreamingData(QString ip, QString videoID)
{
    ui->lineEdit_style->setText(videoID);
    ui->lineEdit_videoIP->setText(ip);
}

