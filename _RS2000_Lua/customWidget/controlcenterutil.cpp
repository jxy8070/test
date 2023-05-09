#include "controlcenterutil.h"
#include "ui_controlcenterutil.h"
#include "Frame/mainwindow.h"
#include "device/devicemanager.h"
#include "device/AES70device.h"
#include "configItem/configitem.h"
#include "Lua/Packages/centerctrl.h"
#include <QTreeWidget>
#include <QTableWidget>
#include <QCheckBox>
#include <QCompleter>
#include <QStandardItemModel>

#define CHS(x) QStringLiteral(x)

ControlCenterUtil::ControlCenterUtil(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlCenterUtil)
{
    ui->setupUi(this);
    connect(ui->btnSelectDev, SIGNAL(clicked()), this, SLOT(onSelectDevice()));
    connect(ui->btnFlashModel, SIGNAL(clicked()), this, SLOT(onFlashCfgItemModel()));
    connect(ui->btnSend, SIGNAL(clicked()), this, SLOT(onSendGram()));
    connect(ui->cboxCmdKey, SIGNAL(currentIndexChanged(int)), this, SLOT(onCmdSelected(int)));
    connect(ui->cboxModelName, SIGNAL(currentIndexChanged(int)), this, SLOT(onModelNameSelected(int)));

    QTableView* cmdView = new QTableView(this);
    cmdView->verticalHeader()->setHidden(true);
    cmdView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    _cmdModel = new CmdKeyModel();
//    cmdView->setModel(_cmdModel);
//    ui->cboxCmdKey->setView(cmdView);
//    ui->cboxCmdKey->setModel(_cmdModel);
    CmdKeyModel* cmdModel = new CmdKeyModel();
    this->cmdFilterModel = new QSortFilterProxyModel(this);
    this->cmdFilterModel->setSourceModel(cmdModel);
    cmdView->setModel(this->cmdFilterModel);
    ui->cboxCmdKey->setView(cmdView);
    ui->cboxCmdKey->setModel(this->cmdFilterModel);

    this->splitFilterType = new QStringList({"Linkwitz_12", "Linkwitz_24", "Linkwitz_36", "Linkwitz_48", "Butterworth_6", "Butterworth_12",
                                             "Butterworth_18", "Butterworth_24", "Butterworth_30", "Butterworth_36", "Butterworth_42", "Butterworth_48"});
    this->automixInputMode = new QStringList({"Mode_Automatic", "Mode_Priority", "Mode_Filibuster"});
    this->lastMicStatus = new QStringList({"Absolute", "Relative"});
    this->attenuationType = new QStringList({"Type_None", "Type_Logarithmic", "Type_Linear"});
    this->nrGainType = new QStringList({"Gain_6", "Gain_9", "Gain_12", "Gain_15"});
    this->rampType = new QStringList({"Type_A", "Type_B", "Type_Mute"});
    this->rampEffect = new QStringList({"Mode_Mute", "Mode_A", "Mode_B"});
    this->specialModal = new QStringList({"SCENE", "CHNNL"});
}

ControlCenterUtil::~ControlCenterUtil()
{
    delete ui;
}

void ControlCenterUtil::onSelectDevice()
{
    _devList = DeviceManager::instance()->getDevListByType(Dev_Type_Unknow, true);
    _devTable = new QTableWidget();
    _devTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _devTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _devTable->setFixedSize(600,300);
    _devTable->setAttribute(Qt::WA_ShowModal, true);
    _devTable->setSortingEnabled(false);
    _devTable->setStyleSheet("background:transparent; *{background:transparent;}");
    _devTable->setSelectionMode(QTableWidget::SingleSelection);
    _devTable->setSelectionBehavior(QTableWidget::SelectRows);
    _devTable->setAttribute(Qt::WA_ShowModal,true);
    _devTable->setColumnCount(3);
    _devTable->setHorizontalHeaderLabels(QStringList()<<tr("devName")<<tr("IP")<<tr("MAC"));
    _devTable->setRowCount(_devList.count());
    int rowIdx = 0;
    for(AES70Device* dev : _devList)
    {
        _devTable->setItem(rowIdx, 0, new QTableWidgetItem(dev->getDeviceName()));
        _devTable->setItem(rowIdx, 1, new QTableWidgetItem(dev->getDeviceIp()));
        _devTable->setItem(rowIdx, 2, new QTableWidgetItem(dev->getAES70Mac()));
        rowIdx++;
    }
    connect(_devTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onDeviceSelected(int, int)));

    _devTable->show();
}

void ControlCenterUtil::onFlashCfgItemModel()
{
    QList<ConfigItem*> itemList = MainWindow::instance()->getCfgItemListOfCurWindow();
    QStandardItemModel* model = new QStandardItemModel();

    for (uint8_t i = 0; i < this->specialModal->length(); i++) {
        QStandardItem* specialItem = new QStandardItem(this->specialModal->at(i));
        model->appendRow(specialItem);
    }

    for (ConfigItem* cfgItem : itemList) {
        QStandardItem* item = new QStandardItem(cfgItem->customName());
        QVariant var = QVariant::fromValue(cfgItem);
        item->setData(var, Qt::UserRole + 1);
        model->appendRow(item);
    }

    ui->cboxModelName->setModel(model);
    QCompleter *pCompleter = new QCompleter(model, this);
    ui->cboxModelName->setCompleter(pCompleter);
}

void ControlCenterUtil::addExtraParamUi(QHBoxLayout* exDataLayout, QString paramType)
{
    if(paramType == "void") {
        exDataLayout->addWidget(new QLabel(tr("no extra data"),this));
    }
    else if(paramType == "ch") {
        exDataLayout->addWidget(new QLabel(tr("channel"),this));
        _exData1 = new QComboBox(this);
        _exData1->setFixedWidth(50);
        for(int i = 0; i< 8; i++){
            _exData1->addItem(QString("%1").arg(i));
        }
        _exData1->setEditable(true);
        exDataLayout->addWidget(_exData1);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    }
    else if(paramType == "ch1") {
        exDataLayout->addWidget(new QLabel(tr("channel"),this));
        _exData2 = new QComboBox(this);
        _exData2->setFixedWidth(50);
        for(int i = 0; i< 8; i++){
            _exData2->addItem(QString("%1").arg(i));
        }
        _exData2->setEditable(true);
        exDataLayout->addWidget(_exData2);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    }
    else if(paramType == "idx") {
        exDataLayout->addWidget(new QLabel(tr("index"),this));
        _exData1 = new QComboBox(this);
        _exData1->setFixedWidth(50);
        for(int i = 0; i< 8; i++){
            _exData1->addItem(QString("%1").arg(i));
        }
        _exData1->setEditable(true);
        exDataLayout->addWidget(_exData1);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    }
    else if(paramType == "state01" || paramType == "state00" ) {
        _stateParam = new QCheckBox(tr("state"),this);
        _stateParam->setLayoutDirection(Qt::RightToLeft);
        _stateParam->setFixedWidth(80);
        exDataLayout->addWidget(_stateParam);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    }
    else if(paramType == "gain") {
        exDataLayout->addWidget(new QLabel(tr("gain"),this));
        _gainParam = new ConfigGainWidget(this);
        _gainParam->setRangeParam(-80, 100, StepMode_Line, ParamType_Gain);
        _gainParam->setDecimalsNum(2);
        exDataLayout->addWidget(_gainParam);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    }
    else if(paramType == "ratio" || paramType == "bgper") {
        exDataLayout->addWidget(new QLabel(paramType == "bgper" ? tr("percentage") : tr("ratio"),this));
        _ratioParam = new ConfigGainWidget(this);
        _ratioParam->setRangeParam(0.2, 100, StepMode_Line, ParamType_Other);
        _ratioParam->setDecimalsNum(2);
        exDataLayout->addWidget(_ratioParam);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    }
    else if(paramType == "time") {
        exDataLayout->addWidget(new QLabel(tr("gain"),this));
        _timeParam = new ConfigGainWidget(this);
        _timeParam->setRangeParam(5, 4000, StepMode_Log10, ParamType_Time);
        _timeParam->setDecimalsNum(2);
        exDataLayout->addWidget(_timeParam);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    }
    else if(paramType == "freq" || paramType == "freq1") {
        exDataLayout->addWidget(new QLabel(tr("freq"),this));
        _fzParam = new ConfigGainWidget(this);
        if (paramType == "freq") {
            _fzParam->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
            _fzParam->setDecimalsNum(1);
        } else {
            _fzParam->setRangeParam(0, 10, StepMode_Line, ParamType_Freq);
            _fzParam->setDecimalsNum(0);
        }
        exDataLayout->addWidget(_fzParam);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    }
    else if(paramType == "qvalue") {
        exDataLayout->addWidget(new QLabel(tr("qvalue"),this));
        _qParam = new ConfigGainWidget(this);
        _qParam->setRangeParam(0.05, 20, StepMode_Log10, ParamType_Q);
        _qParam->setDecimalsNum(2);
        exDataLayout->addWidget(_qParam);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    }
    else if(paramType == "bandwidth") {
        exDataLayout->addWidget(new QLabel(tr("bandwidth"),this));
        _bwParam = new ConfigGainWidget(this);
        _bwParam->setRangeParam(0.01, 3, StepMode_Log10, ParamType_Q);
        _bwParam->setDecimalsNum(2);
        exDataLayout->addWidget(_bwParam);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    }
    else if(paramType == "delay") {
        exDataLayout->addWidget(new QLabel(tr("delay"),this));
        _delayParam = new ConfigGainWidget(this);
        _delayParam->setRangeParam(0, 1000, StepMode_Line, ParamType_Time);
        _delayParam->setDecimalsNum(2);
        exDataLayout->addWidget(_delayParam);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    }
    else if(paramType == "split" || paramType == "filter") {
        exDataLayout->addWidget(new QLabel(tr("type"),this));
        _exData2 = new QComboBox(this);
        _exData2->setFixedWidth(120);
        for(int i = 0; i < this->splitFilterType->length(); i++){
            _exData2->addItem(this->splitFilterType->at(i));
        }
        _exData2->setEditable(true);
        exDataLayout->addWidget(_exData2);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    }
    else if(paramType == "automix") {
        exDataLayout->addWidget(new QLabel(tr("mode"),this));
        _exData2 = new QComboBox(this);
        _exData2->setFixedWidth(120);
        for(int i = 0; i < this->automixInputMode->length(); i++){
            _exData2->addItem(this->automixInputMode->at(i));
        }
        _exData2->setEditable(true);
        exDataLayout->addWidget(_exData2);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));          
    }
    else if(paramType == "lastmic") {
        exDataLayout->addWidget(new QLabel(tr("status"),this));
        _exData1 = new QComboBox(this);
        _exData1->setFixedWidth(120);
        for(int i = 0; i < this->lastMicStatus->length(); i++){
            _exData1->addItem(this->lastMicStatus->at(i));
        }
        _exData1->setEditable(true);
        exDataLayout->addWidget(_exData1);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));               
    }
    else if(paramType == "atttype") {
        exDataLayout->addWidget(new QLabel(tr("type"),this));
        _exData1 = new QComboBox(this);
        _exData1->setFixedWidth(120);
        for(int i = 0; i < this->attenuationType->length(); i++){
            _exData1->addItem(this->attenuationType->at(i));
        }
        _exData1->setEditable(true);
        exDataLayout->addWidget(_exData1);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));         
    }
    else if(paramType == "priority") {
        exDataLayout->addWidget(new QLabel(tr("priority"),this));
        _priorityParam = new QLineEdit(this);
        _priorityParam->setFixedWidth(80);
        exDataLayout->addWidget(_priorityParam);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));         
    }
    else if(paramType == "maxmic") {
        exDataLayout->addWidget(new QLabel(tr("maxmic"),this));
        _maxmicParam = new QLineEdit(this);
        _maxmicParam->setFixedWidth(80);
        exDataLayout->addWidget(_maxmicParam);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));         
    }
    else if(paramType == "attmax" || paramType == "attstep" ) {
        exDataLayout->addWidget(new QLabel(tr("att"),this));
        _attParam = new ConfigGainWidget(this);
        _attParam->setRangeParam(0, 40, StepMode_Line, ParamType_Other);
        _attParam->setDecimalsNum(2);
        exDataLayout->addWidget(_attParam);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    }
    else if(paramType == "nrgain") {
        exDataLayout->addWidget(new QLabel(tr("nr"),this));
        _exData1 = new QComboBox(this);
        _exData1->setFixedWidth(120);
        for(int i = 0; i < this->nrGainType->length(); i++){
            _exData1->addItem(this->nrGainType->at(i));
        }
        _exData1->setEditable(true);
        exDataLayout->addWidget(_exData1);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));         
    }
    else if(paramType == "nrgain") {
        exDataLayout->addWidget(new QLabel(tr("ramp"),this));
        _exData1 = new QComboBox(this);
        _exData1->setFixedWidth(120);
        for(int i = 0; i < this->rampType->length(); i++){
            _exData1->addItem(this->rampType->at(i));
        }
        _exData1->setEditable(true);
        exDataLayout->addWidget(_exData1);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));         
    }
    else if(paramType == "ramp") {
        exDataLayout->addWidget(new QLabel(tr("ramp"),this));
        _exData1 = new QComboBox(this);
        _exData1->setFixedWidth(120);
        for(int i = 0; i < this->rampType->length(); i++){
            _exData1->addItem(this->rampType->at(i));
        }
        _exData1->setEditable(true);
        exDataLayout->addWidget(_exData1);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));         
    }
    else if(paramType == "rpeffect") {
        exDataLayout->addWidget(new QLabel(tr("ramp effect"),this));
        _exData1 = new QComboBox(this);
        _exData1->setFixedWidth(120);
        for(int i = 0; i < this->rampEffect->length(); i++){
            _exData1->addItem(this->rampEffect->at(i));
        }
        _exData1->setEditable(true);
        exDataLayout->addWidget(_exData1);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));         
    }
    else if(paramType == "scene") {
        exDataLayout->addWidget(new QLabel(tr("scene"),this));
        _exData1 = new QComboBox(this);
        _exData1->setFixedWidth(120);
        for(int i = 0; i < 8; i++){
            _exData1->addItem(QString("%1").arg(i + 1));
        }
        _exData1->setEditable(true);
        exDataLayout->addWidget(_exData1);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));         
    }
    else if(paramType == "dir") {
        exDataLayout->addWidget(new QLabel(tr("direction"),this));
        _exData1 = new QComboBox(this);
        _exData1->setFixedWidth(120);
        _exData1->addItem("input");
        _exData1->addItem("output");
        _exData1->setEditable(true);
        exDataLayout->addWidget(_exData1);
        exDataLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));         
    }
    else {
        Utils::MessageBoxWarning(CHS("警告"), CHS("未实现的类型:") + paramType);
    }
}

void ControlCenterUtil::onCmdSelected(int index)
{
    QString curTxt = ui->cboxCmdKey->currentText();
    QString cmdKeyStr =curTxt.mid(2, curTxt.indexOf('(')-2);
    bool rs;
    quint32 curCmdKey = cmdKeyStr.toUInt(&rs, 16);
    for(CenterCtrlCmd cmd : CmdKeyModel::CmdList)
    {
        if(cmd.CmdKey == curCmdKey){
            _curCmd = cmd;
            break;
        }
    }

    if (ui->cboxModelName->currentIndex() <= 1) {
        if (_curCmd.CmdKey == 0xFFFF0002 || _curCmd.CmdKey == 0xFFFF0003)
        {
            ui->cboxModelName->setCurrentIndex(0);
        }
        else if (_curCmd.CmdKey == 0xFFFF0004 || _curCmd.CmdKey == 0xFFFF0005 || _curCmd.CmdKey == 0xFFFF0006)
        {
            ui->cboxModelName->setCurrentIndex(1);
        }
    }

    //根据命令类型初始化控件
    if(_exDataContent !=NULL) delete _exDataContent;
    _exDataContent = new QWidget();
    QHBoxLayout* exDataLayout = new QHBoxLayout(_exDataContent);
    QStringList typeList = _curCmd.formatType.split(QString('_'));
    for(QString paramType : typeList)
    {
        addExtraParamUi(exDataLayout, paramType);
    }

    exDataLayout->addItem(new QSpacerItem(403, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    ui->groupExtraData->layout()->addWidget(_exDataContent);
}

void ControlCenterUtil::onModelNameSelected(int index)
{
    QString filterKey = "";
    if (this->specialModal->contains(ui->cboxModelName->itemText(index))) {
      filterKey = "0xffff";
    }
    else {
        QVariant data = ui->cboxModelName->currentData(Qt::UserRole + 1);
        ConfigItem* cfgItem = data.value<ConfigItem*>();
        quint32 cmdGroup = cfgItem->getCfgType()&0x0000FF00;

        for(CenterCtrlCmd cmd : CmdKeyModel::CmdList)
        {
            if(cmd.CmdGroup == cmdGroup){
                quint32 check = cmd.CmdKey >> 16;
                filterKey = QString("0x%1").arg((cmd.CmdKey >> 16), 4, 16, QLatin1Char('0'));
                break;
            } else {
                filterKey = "0xFFFFFFFF";
            }
        }
    }


//    // 不过滤特殊命令
//    QString specialCmd = "0xffff";
//    QStringList *filterKeys = new QStringList({filterKey, specialCmd});
//    QString filterReg;
//    for (int i = 0; i < filterKeys->count(); ++i) {
//        QString _filter = QRegExp::escape(filterKeys->at(i));
//        if (i > 0)
//            filterReg += '|';
//        filterReg += _filter;
//    }

    this->cmdFilterModel->setFilterRegularExpression(QRegularExpression(filterKey, QRegularExpression::CaseInsensitiveOption));
//    this->cmdFilterModel->setFilterRegExp(QRegExp(filterReg));
    this->cmdFilterModel->setFilterKeyColumn(0);
}

void ControlCenterUtil::onDeviceSelected(int row, int column)
{
    if(row >= 0 && row < _devList.count())
    {
        AES70Device* selectDev = _devList[row];
        ui->txtDevIp->setText(selectDev->getDeviceIp());
    }
    _devTable->close();
    _devTable= NULL;
}

void ControlCenterUtil::onSendGram()
{
    QString devIp = ui->txtDevIp->text();
    QString modelName = ui->cboxModelName->currentText();

    ui->txtSendBuff->setText("");
    ui->txtRecvBuff->setText("");
    ui->txtResult->setText("");
    QThread::msleep(200);

    if (_curCmd.CmdKey == 0xFFFF0002 || _curCmd.CmdKey == 0xFFFF0003) {
        modelName = "SCENE";
    } else if (_curCmd.CmdKey == 0xFFFF0004 || _curCmd.CmdKey == 0xFFFF0005|| _curCmd.CmdKey == 0xFFFF0006) {
        modelName = "CHNNL";
    }
    UdpGram sendGram(modelName, _curCmd.CmdKey);
    QByteArray udpGram = formatDatagram(sendGram);
    ui->txtSendBuff->setText(udpGram.toHex().toUpper());
    QUdpSocket udpClient;
    udpClient.writeDatagram(udpGram, QHostAddress(devIp), 35030);
    bool ready = udpClient.waitForReadyRead(3000);
    if(!ready || !udpClient.hasPendingDatagrams()){ //接收超时
        ui->txtResult->setText(tr("receive timeout!"));
        return;
    }

    QByteArray recvBuff;
    recvBuff.resize(udpClient.pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;
    udpClient.readDatagram(recvBuff.data(), recvBuff.size(), &sender, &senderPort);


    UdpGram recvGram;
    if(recvBuff.length() != 28){ ui->txtResult->setText(tr("Datagram error!"));}
    QDataStream stream(&recvBuff, QIODevice::ReadWrite);
    stream.setByteOrder(QDataStream::BigEndian);
    quint16 head, sum, temp, end;
    stream >> head >> sum;
    stream >> recvGram;
    stream >> temp >> end;
    int calcSum = 0;
    for(int i = 4; i< recvBuff.length() -4; i++ ){
        calcSum += (quint8)recvBuff[i];
    }
    if(head != 0x7B20 || end != 0x207B || sum != calcSum) {
        ui->txtResult->setText(tr("Datagram error!"));
    }
    else{
        ui->txtRecvBuff->setText(recvBuff.toHex().toUpper());
        parserDatagram(recvGram);
    }
}

QByteArray ControlCenterUtil::formatDatagram(UdpGram& sendGram)
{
    //根据命令类型格式化数据
    bool rs;
    if (_curCmd.formatType == "ch" || _curCmd.formatType == "idx") {
        quint8 ch = _exData1->currentText().toUInt();
        sendGram.setUint8(ch, 0);
    }
    else if(_curCmd.formatType == "ch_ch1") {
        quint8 ch0 = _exData1->currentText().toUInt();
        quint8 ch1 = _exData2->currentText().toUInt();
        sendGram.setUint8(ch0, 0);
        sendGram.setUint8(ch1, 1);
    }
    else if(_curCmd.formatType == "ch_state00") {
        quint8 ch = _exData1->currentText().toUInt();
        quint8 state = _stateParam->isChecked()? 0x00 : 0x01;
        sendGram.setUint8(ch, 0);
        sendGram.setUint8(state, 1);
    }
    else if(_curCmd.formatType == "ch_state01") {
        quint8 ch = _exData1->currentText().toUInt();
        quint8 state = _stateParam->isChecked()? 0x01 : 0x00;
        sendGram.setUint8(ch, 0);
        sendGram.setUint8(state, 1);
    }
    else if(_curCmd.formatType == "state00") {
        quint8 state = _stateParam->isChecked()? 0x00 : 0x01;
        sendGram.setUint8(state, 0);
    }
    else if(_curCmd.formatType == "state01") {
        quint8 state = _stateParam->isChecked()? 0x01 : 0x00;
        sendGram.setUint8(state, 0);
    }
    else if(_curCmd.formatType == "gain") {
        double gain = _gainParam->value();
        sendGram.setGain(gain, 0);
    }
    else if(_curCmd.formatType == "ratio" || _curCmd.formatType == "bgper") {
        double value = _ratioParam->value() * 100;
        sendGram.setUint16(value, 0);
    }
    else if(_curCmd.formatType == "time") {
        double time = _timeParam->value();
        sendGram.setUint16(time, 0);
    }
    else if(_curCmd.formatType == "freq" || _curCmd.formatType == "freq1") {
        double freq = _fzParam->value();
        sendGram.setUint16(freq, 0);;
    }
    else if(_curCmd.formatType == "ch_gain" || _curCmd.formatType == "idx_gain") {
        quint8 ch = _exData1->currentText().toUInt();
        double gain = _gainParam->value();
        sendGram.setUint8(ch, 0);
        sendGram.setGain(gain, 2);
    }
    else if(_curCmd.formatType == "ch_freq" || _curCmd.formatType == "idx_freq") {
        quint8 ch = _exData1->currentText().toUInt();
        double freq = _fzParam->value();
        sendGram.setUint8(ch, 0);
        sendGram.setUint16(freq, 2);
    }  
    else if(_curCmd.formatType == "ch_qvalue" || _curCmd.formatType == "idx_qvalue") {
        quint8 ch = _exData1->currentText().toUInt();
        double qvalue = _qParam->value() * 100;
        sendGram.setUint8(ch, 0);
        sendGram.setUint16(qvalue, 2);
    }      
    else if(_curCmd.formatType == "delay") {
        quint16 delay = _delayParam->value();
        sendGram.setUint16(delay, 0);
    }
    else if(_curCmd.formatType == "ch_split") {
        quint8 ch = _exData1->currentText().toUInt();
        quint8 _type = _exData2->currentIndex();
        sendGram.setUint8(ch, 0);
        sendGram.setUint8(_type, 1);
    }
    else if(_curCmd.formatType == "filter") {
        quint8 _type = _exData2->currentIndex();
        sendGram.setUint8(_type, 0);
    }
    else if(_curCmd.formatType == "bandwidth") {
        quint16 bandwidth = _bwParam->value() * 1000;
        sendGram.setUint16(bandwidth, 0);
    }
    else if(_curCmd.formatType == "ch_ch1_state00") {
        quint8 input_ch = _exData1->currentText().toUInt();
        quint8 output_ch = _exData1->currentText().toUInt();
        quint8 state = _stateParam->isChecked()? 0x00 : 0x01;
        sendGram.setUint8(input_ch, 0);
        sendGram.setUint8(output_ch, 1);
        sendGram.setUint8(state, 2);
    }
    else if(_curCmd.formatType == "ch_ch1_state01") {
        quint8 input_ch = _exData1->currentText().toUInt();
        quint8 output_ch = _exData1->currentText().toUInt();
        quint8 state = _stateParam->isChecked()? 0x01 : 0x00;
        sendGram.setUint8(input_ch, 0);
        sendGram.setUint8(output_ch, 1);
        sendGram.setUint8(state, 2);
    }
    else if(_curCmd.formatType == "ch_ch1_gain") {
        quint8 input_ch = _exData1->currentText().toUInt();
        quint8 output_ch = _exData1->currentText().toUInt();
        double gain = _gainParam->value();
        sendGram.setUint8(input_ch, 0);
        sendGram.setUint8(output_ch, 1);
        sendGram.setGain(gain, 2);
    }
    else if(_curCmd.formatType == "ch_automix") {
        quint8 ch = _exData1->currentText().toUInt();
        quint8 mode = _exData2->currentIndex();
        sendGram.setUint8(mode, 0);
    }
    else if(_curCmd.formatType == "automix") {
        quint8 _type = _exData2->currentIndex();
        sendGram.setUint8(_type, 0);
    }
    else if(_curCmd.formatType == "ch_priority") {
        quint8 ch = _exData1->currentText().toUInt();
        quint16 priority = _priorityParam->text().toUInt();
        sendGram.setUint8(ch, 0);
        sendGram.setUint16(priority, 2);
    }
    else if(_curCmd.formatType == "lastmic" || _curCmd.formatType == "atttype" || _curCmd.formatType == "nrgain" || 
        _curCmd.formatType == "ramp" || _curCmd.formatType == "rpeffect") {
        quint8 data = _exData1->currentIndex();
        sendGram.setUint8(data, 0);
    }
    else if(_curCmd.formatType == "maxmic") {
        quint8 maxmic = _maxmicParam->text().toUInt();
        sendGram.setUint8(maxmic, 0);
    }
    else if(_curCmd.formatType == "attstep" || _curCmd.formatType == "attmax") {
        quint16 data = _attParam->value();
        sendGram.setGain(data, 0);
    }
    else if(_curCmd.formatType == "ramp_time") {
        quint8 data = _exData1->currentIndex();
        double time = _timeParam->value();
        sendGram.setUint8(data, 0);
        sendGram.setUint16(time, 2);
    }
    else if(_curCmd.formatType == "ramp_gain") {
        quint8 data = _exData1->currentIndex();
        double gain = _gainParam->value();
        sendGram.setUint8(data, 0);
        sendGram.setGain(gain, 2);
    }
    else if(_curCmd.formatType == "scene") {
        quint8 data = _exData1->currentText().toUInt();
        sendGram.setUint8(data, 0);
    }
    else if(_curCmd.formatType == "dir") {
        quint8 data = _exData1->currentIndex();
        sendGram.setUint8(data, 0);
    }
    else if(_curCmd.formatType == "dir_state01") {
        quint8 data = _exData1->currentIndex();
        quint8 state = _stateParam->isChecked()? 0x01 : 0x00;
        sendGram.setUint8(data, 0);
        sendGram.setUint8(state, 1);
    }    
    else if(_curCmd.formatType == "dir_gain") {
        quint8 data = _exData1->currentIndex();
        double gain = _gainParam->value();
        sendGram.setUint8(data, 0);
        sendGram.setGain(gain, 2);
    } 

    //设置校验
    quint16 sum = 0;
    quint8* pData = (quint8*) &sendGram;
    for(int i = 0; i< sizeof(UdpGram); i++){
        sum += pData[i];
    }
    QByteArray udpGramBa;
    QDataStream stream(&udpGramBa, QIODevice::ReadWrite);
    stream.setByteOrder(QDataStream::BigEndian);

    stream << (quint16)0x7B20;      //帧头0x7B20
    stream << sum;                  //模块名称、功能ID、数据长度的字节累加和
    stream << sendGram;
    stream << (quint16)0x0000;      //4字节对齐预留位0x0000
    stream << (quint16)0x207B;      //帧尾0x207B
    return udpGramBa;
}

QString linePerfix = "\r\n    ";
void ControlCenterUtil::parserDatagram(UdpGram& recvGram)
{
    QString result;
    if(recvGram.getCmdKey() == 0xFFFF0001){
        result = tr("operate failed!");
    }
    else if(recvGram.getCmdKey() == 0x00000000){
        result = tr("model not exist!");
    }
    else if(recvGram.getCmdKey() == 0xFFFF0000){
        result = tr("operate succeed!");
        if(_curCmd.parserType == "gain" || _curCmd.parserType == "attstep" || _curCmd.parserType == "attmax") {
            double gain = recvGram.getGain(0);
            result += linePerfix + tr("data:") + QString("%1").arg(gain);
            double gainMin = recvGram.getGain(2);
            double gainMax = recvGram.getGain(4);
//            if(gainMax!=-100 || gainMin!=-100){
                result += linePerfix + tr("min:") + QString("%1").arg(gainMin);
                result += linePerfix + tr("max:") + QString("%1").arg(gainMax);
//            }
        }
        else if(_curCmd.parserType == "ratio" || _curCmd.parserType == "bgper") {
            double ratio = recvGram.getUint16(0) / 100;
            result += linePerfix + tr("value:") + QString("%1").arg(ratio);
            double ratioMin = recvGram.getUint16(2) / 100;
            double ratioMax = recvGram.getUint16(4) / 100;
                result += linePerfix + tr("min:") + QString("%1").arg(ratioMin);
                result += linePerfix + tr("max:") + QString("%1").arg(ratioMax);
        }
        else if(_curCmd.parserType == "time" || _curCmd.parserType == "freq" || _curCmd.parserType == "priority") {
            double time = recvGram.getUint16(0);
            result += linePerfix + tr("value:") + QString("%1").arg(time);
            double timeMin = recvGram.getUint16(2);
            double timeMax = recvGram.getUint16(4);
                result += linePerfix + tr("min:") + QString("%1").arg(timeMin);
                result += linePerfix + tr("max:") + QString("%1").arg(timeMax);
        }
        else if(_curCmd.parserType == "qvalue") {
            double qvalue = QString::number((double)recvGram.getUint16(0) / 100, 'f', 2).toFloat();
            result += linePerfix + tr("q:") + QString("%1").arg(qvalue);
            double qMin = (double)100 / recvGram.getUint16(2);
            double qMax = (double)100 / recvGram.getUint16(4);
                result += linePerfix + tr("min:") + QString("%1").arg(qMin);
                result += linePerfix + tr("max:") + QString("%1").arg(qMax);
        }
        else if(_curCmd.parserType == "state01") {
            result += linePerfix + tr("state:");
            if(recvGram.getUint8(0) == 0x01) result += tr("On");
            else  result += tr("Off");
        }
        else if(_curCmd.parserType == "state00") {
            result += linePerfix + tr("state:");
            if(recvGram.getUint8(0) == 0x00) result += tr("On");
            else  result += tr("Off");
        }
        else if(_curCmd.parserType == "delay") {
            quint16 delay = recvGram.getUint16(0);
            quint16 max = recvGram.getUint16(2);
            result += linePerfix + tr("delay:")+ QString("%1").arg(delay);
            result += linePerfix + tr("max:")+ QString("%1").arg(max);
        }
        else if (_curCmd.parserType == "ch") {
            quint16 input_ch = recvGram.getUint8(0);
            result += linePerfix + tr("channel:")+ QString("%1").arg(input_ch);
        }
        else if(_curCmd.parserType == "split" || _curCmd.parserType == "filter") {
            result += linePerfix + tr("type:");
            result += this->splitFilterType->at(recvGram.getUint8(0));
        }
        else if(_curCmd.parserType == "bandwidth") {
            double bandwidth = (double)recvGram.getUint16(0) / 1000;
            result += linePerfix + tr("value:") + QString("%1").arg(bandwidth);
            double bandwidthMin = (double)recvGram.getUint16(2) / 1000;
            double bandwidthMax = (double)recvGram.getUint16(4) / 1000;
                result += linePerfix + tr("min:") + QString("%1").arg(bandwidthMin);
                result += linePerfix + tr("max:") + QString("%1").arg(bandwidthMax);
        }
        else if(_curCmd.parserType == "automix") {
            result += linePerfix + tr("mode:");
            result += this->automixInputMode->at(recvGram.getUint8(0));            
        }
        else if(_curCmd.parserType == "lastmic") {
            result += linePerfix + tr("status:");
            result += this->lastMicStatus->at(recvGram.getUint8(0));            
        }
        else if(_curCmd.parserType == "atttype") {
            result += linePerfix + tr("type:");
            result += this->attenuationType->at(recvGram.getUint8(0));            
        }
        else if(_curCmd.parserType == "maxmic") {
            double data = recvGram.getUint8(0);
            result += linePerfix + tr("value:") + QString("%1").arg(data);
            double dataMin = recvGram.getUint8(1);
            double dataMax = recvGram.getUint8(2);
                result += linePerfix + tr("min:") + QString("%1").arg(dataMin);
                result += linePerfix + tr("max:") + QString("%1").arg(dataMax);
        }
        else if(_curCmd.parserType == "nrgain") {
            result += linePerfix + tr("nr:");
            result += this->nrGainType->at(recvGram.getUint8(0));             
        }
        else if(_curCmd.parserType == "ramp") {
            result += linePerfix + tr("ramp type:");
            result += this->rampType->at(recvGram.getUint8(0));             
        }
        else if(_curCmd.parserType == "rpeffect") {
            result += linePerfix + tr("ramp effect:");
            result += this->rampEffect->at(recvGram.getUint8(0));             
        }
        else if (_curCmd.parserType == "scene") {
            result += linePerfix + tr("scene:");
            result += QString::number(recvGram.getUint8(0));;
        }
    }

    ui->txtResult->setText(result);
}

//===============================================//
//==================CmdKeyModel==================//
//===============================================//
QList<CenterCtrlCmd> CmdKeyModel::CmdList = {
        // 特殊命令
//        {0xFFFF0000, CHS("命令执行成功"), CHS("特殊"), 0x0000, "", ""},
//        {0xFFFF0001, CHS("命令执行失败"), CHS("特殊"), 0x0000, "", ""},
        {0xFFFF0002, CHS("获取当前场景号"), CHS("特殊"), 0x0000, "void", "scene"},
        {0xFFFF0003, CHS("设置当前场景"), CHS("特殊"), 0x0000, "scene", "void"},
        {0xFFFF0004, CHS("获取总静音开关状态"), CHS("特殊"), 0x0000, "dir", "state01"},
        {0xFFFF0005, CHS("设置总静音开关状态"), CHS("特殊"), 0x0000, "dir_state01", "void"},
        {0xFFFF0006, CHS("设置所有通道增益增加减少量"), CHS("特殊"), 0x0000, "dir_gain", "void"},
        //输入模块
        {0x00000001, CHS("获取输入反相开关状态"), CHS("输入模块"), 0x0A00, "ch", "state01"},
        {0x00000002, CHS("设置输入反相开关状态"), CHS("输入模块"), 0x0A00, "ch_state01", "void"},
        {0x00000003, CHS("获取输入静音开关状态"), CHS("输入模块"), 0x0A00, "ch", "state01"},
        {0x00000004, CHS("设置输入静音开关状态"), CHS("输入模块"), 0x0A00, "ch_state01", "void"},
        {0x00000005, CHS("获取输入增益"), CHS("输入模块"), 0x0A00, "ch", "gain"},
        {0x00000006, CHS("设置输入增益"), CHS("输入模块"), 0x0A00, "ch_gain", "void"},
        //输出模块
        {0x00010001, CHS("获取输出反相开关状态"), CHS("输出模块"), 0x0B00, "ch", "state01"},
        {0x00010002, CHS("设置输出反相开关状态"), CHS("输出模块"), 0x0B00, "ch_state01", "void"},
        {0x00010003, CHS("获取输出静音开关状态"), CHS("输出模块"), 0x0B00, "ch", "state01"},
        {0x00010004, CHS("设置输出静音开关状态"), CHS("输出模块"), 0x0B00, "ch_state01", "void"},
        {0x00010005, CHS("获取输出增益"), CHS("输出模块"), 0x0B00, "ch", "gain"},
        {0x00010006, CHS("设置输出增益"), CHS("输出模块"), 0x0B00, "ch_gain", "void"},
        //延时模块
        {0x00020001, CHS("获取直通开关状态"), CHS("延时模块"), 0x0100, "void", "state00"},
        {0x00020002, CHS("设置直通开关状态"), CHS("延时模块"), 0x0100, "state00", "void"},
        {0x00020003, CHS("获取延时量"), CHS("延时模块"), 0x0100, "void", "delay"},
        {0x00020004, CHS("设置延时量"), CHS("延时模块"), 0x0100, "delay", "void"},
        //路由模块
        {0x00030001, CHS("获取路由选择状态"), CHS("路由模块"), 0x0200, "ch", "ch"},
        {0x00030002, CHS("设置路由选择状态"), CHS("路由模块"), 0x0200, "ch_ch1", "void"},
        //混音模块     
        {0x00040001, CHS("获取输入通道状态"), CHS("混音模块"), 0x0300, "ch", "state00"},
        {0x00040002, CHS("设置输入通道状态"), CHS("混音模块"), 0x0300, "ch_state00", "void"},
        {0x00040003, CHS("获取输入通道增益"), CHS("混音模块"), 0x0300, "ch", "gain"},
        {0x00040004, CHS("设置输入通道增益"), CHS("混音模块"), 0x0300, "ch_gain", "void"},
        {0x00040005, CHS("获取输出通道状态"), CHS("混音模块"), 0x0300, "ch", "state00"},
        {0x00040006, CHS("设置输出通道状态"), CHS("混音模块"), 0x0300, "ch_state00", "void"},
        {0x00040007, CHS("获取输出通道增益"), CHS("混音模块"), 0x0300, "ch", "gain"},
        {0x00040008, CHS("设置输出通道增益"), CHS("混音模块"), 0x0300, "ch_gain", "void"},
        {0x00040009, CHS("获取混音节点状态"), CHS("混音模块"), 0x0300, "ch_ch1", "state01"},
        {0x0004000A, CHS("设置混音节点状态"), CHS("混音模块"), 0x0300, "ch_ch1_state01", "void"},
        {0x0004000B, CHS("获取混音节点增益"), CHS("混音模块"), 0x0300, "ch_ch1", "gain"},
        {0x0004000C, CHS("设置混音节点增益"), CHS("混音模块"), 0x0300, "ch_ch1_gain", "void"},
        //动态     
        {0x00050001, CHS("获取电平阈值"), CHS("动态"), 0x0400, "void", "gain"},
        {0x00050002, CHS("设置电平阈值"), CHS("动态"), 0x0400, "gain", "void"},
        {0x00050003, CHS("获取压缩比率"), CHS("动态"), 0x0400, "void", "ratio"},
        {0x00050004, CHS("设置压缩比率"), CHS("动态"), 0x0400, "ratio", "void"},
        {0x00050005, CHS("获取启控时间"), CHS("动态"), 0x0400, "void", "time"},
        {0x00050006, CHS("设置启控时间"), CHS("动态"), 0x0400, "time", "void"},
        {0x00050007, CHS("获取释放时间"), CHS("动态"), 0x0400, "void", "time"},
        {0x00050008, CHS("设置释放时间"), CHS("动态"), 0x0400, "time", "void"},
        {0x00050009, CHS("获取电平衰减"), CHS("动态"), 0x0400, "void", "gain"},
        {0x0005000A, CHS("设置电平衰减"), CHS("动态"), 0x0400, "gain", "void"},
        {0x0005000B, CHS("获取输出目标电平"), CHS("动态"), 0x0400, "void", "gain"},
        {0x0005000C, CHS("设置输出目标电平"), CHS("动态"), 0x0400, "gain", "void"},
        {0x0005000D, CHS("获取直通开关状态"), CHS("动态"), 0x0400, "void", "state01"},
        {0x0005000E, CHS("设置直通开关状态"), CHS("动态"), 0x0400, "state01", "void"},
        //均衡模块
        {0x00060001, CHS("获取均衡点频率"), CHS("均衡模块"), 0x0500, "idx", "freq"},
        {0x00060002, CHS("设置均衡点频率"), CHS("均衡模块"), 0x0500, "idx_freq", "void"},
        {0x00060003, CHS("获取均衡点增益"), CHS("均衡模块"), 0x0500, "idx", "gain"},
        {0x00060004, CHS("设置均衡点增益"), CHS("均衡模块"), 0x0500, "idx_gain", "void"},
        {0x00060005, CHS("获取均衡点Q值"), CHS("均衡模块"), 0x0500, "idx", "qvalue"},
        {0x00060006, CHS("设置均衡点Q值"), CHS("均衡模块"), 0x0500, "idx_qvalue", "void"},
        {0x00060007, CHS("获取直通开关状态"), CHS("均衡模块"), 0x0500, "void", "state01"},
        {0x00060008, CHS("设置直通开关状态"), CHS("均衡模块"), 0x0500, "state01", "void"},   
        //信号发生器模块
        {0x00070001, CHS("获取当前频率"), CHS("均衡模块"), 0x0800, "void", "freq"},
        {0x00070002, CHS("设置当前频率"), CHS("均衡模块"), 0x0800, "freq", "void"},
        {0x00070003, CHS("获取增益"), CHS("均衡模块"), 0x0800, "void", "gain"},
        {0x00070004, CHS("设置增益"), CHS("均衡模块"), 0x0800, "gain", "void"},
        {0x00070005, CHS("获取静音开关状态"), CHS("均衡模块"), 0x0800, "void", "state01"},
        {0x00070006, CHS("设置静音开关状态"), CHS("均衡模块"), 0x0800, "state01", "void"},
        //分频器模块     
        {0x00080001, CHS("获取输入静音开关状态"), CHS("分频器模块"), 0x0700, "void", "state01"},
        {0x00080002, CHS("设置输入静音开关状态"), CHS("分频器模块"), 0x0700, "state01", "void"},
        {0x00080003, CHS("获取输入增益"), CHS("分频器模块"), 0x0700, "void", "gain"},
        {0x00080004, CHS("设置输入增益"), CHS("分频器模块"), 0x0700, "gain", "void"},
        {0x00080005, CHS("获取分频滤波器类型"), CHS("分频器模块"), 0x0700, "ch", "split"},
        {0x00080006, CHS("设置分频滤波器类型"), CHS("分频器模块"), 0x0700, "ch_split", "void"},
        {0x00080007, CHS("获取滤波器频率"), CHS("分频器模块"), 0x0700, "ch", "freq"},
        {0x00080008, CHS("设置滤波器频率"), CHS("分频器模块"), 0x0700, "ch_freq", "void"},
        {0x00080009, CHS("获取输出静音开关状态"), CHS("分频器模块"), 0x0700, "ch", "state01"},
        {0x0008000A, CHS("设置输出静音开关状态"), CHS("分频器模块"), 0x0700, "ch_state01", "void"},
        {0x0008000B, CHS("获取输出增益"), CHS("分频器模块"), 0x0700, "void", "gain"},
        {0x0008000C, CHS("设置输出增益"), CHS("分频器模块"), 0x0700, "ch_gain", "void"},
        {0x0008000D, CHS("获取输出反相开关状态"), CHS("分频器模块"), 0x0700, "ch", "state01"},
        {0x0008000E, CHS("设置输出反相开关状态"), CHS("分频器模块"), 0x0700, "ch_state01", "void"},
        //滤波器模块     
        {0x00090001, CHS("获取滤波器类型"), CHS("滤波器模块"), 0x0600, "void", "filter"},
        {0x00090002, CHS("设置滤波器类型"), CHS("滤波器模块"), 0x0600, "filter", "void"},
        {0x00090003, CHS("获取滤波器频率"), CHS("滤波器模块"), 0x0600, "void", "freq"},
        {0x00090004, CHS("设置滤波器频率"), CHS("滤波器模块"), 0x0600, "freq", "void"},
        {0x00090005, CHS("获取增益"), CHS("滤波器模块"), 0x0600, "void", "gain"}, // 暂无该属性
        {0x00090006, CHS("设置增益"), CHS("滤波器模块"), 0x0600, "gain", "void"}, // 暂无该属性
        {0x00090007, CHS("获取直通开关状态"), CHS("滤波器模块"), 0x0600, "void", "state01"},
        {0x00090008, CHS("设置直通开关状态"), CHS("滤波器模块"), 0x0600, "state01", "void"},
        {0x00090009, CHS("获取滤波器带宽"), CHS("滤波器模块"), 0x0600, "void", "bandwidth"},
        {0x0009000A, CHS("设置滤波器带宽"), CHS("滤波器模块"), 0x0600, "bandwidth", "void"},
        //啸叫抑制模块     
        {0x000A0001, CHS("获取偏移频率"), CHS("啸叫抑制模块"), 0x0D00, "void", "freq"},
        {0x000A0002, CHS("设置偏移频率"), CHS("啸叫抑制模块"), 0x0D00, "freq1", "void"},
        {0x000A0003, CHS("获取传输增益"), CHS("啸叫抑制模块"), 0x0D00, "void", "gain"},
        {0x000A0004, CHS("设置传输增益"), CHS("啸叫抑制模块"), 0x0D00, "gain", "void"},
        // {0x000A0005, CHS("获取增益"), CHS("啸叫抑制模块"), 0x0D00, "ch", "split"},
        // {0x000A0006, CHS("设置增益"), CHS("啸叫抑制模块"), 0x0D00, "ch_split", "void"},
        //自动混音模块     
        {0x000B0001, CHS("获取阈值电平"), CHS("自动混音模块"), 0x0E00, "void", "gain"},
        {0x000B0002, CHS("设置阈值电平"), CHS("自动混音模块"), 0x0E00, "gain", "void"},
        {0x000B0003, CHS("获取启控时间"), CHS("自动混音模块"), 0x0E00, "void", "time"},
        {0x000B0004, CHS("设置启控时间"), CHS("自动混音模块"), 0x0E00, "time", "void"},
        {0x000B0005, CHS("获取释放时间"), CHS("自动混音模块"), 0x0E00, "void", "time"}, 
        {0x000B0006, CHS("设置释放时间"), CHS("自动混音模块"), 0x0E00, "time", "void"}, 
        {0x000B0007, CHS("获取保持时间"), CHS("自动混音模块"), 0x0E00, "void", "time"},
        {0x000B0008, CHS("设置保持时间"), CHS("自动混音模块"), 0x0E00, "time", "void"},
        {0x000B000B, CHS("获取Depth增益"), CHS("自动混音模块"), 0x0E00, "void", "gain"},
        {0x000B000C, CHS("设置Depth增益"), CHS("自动混音模块"), 0x0E00, "gain", "void"},
        {0x000B000F, CHS("获取噪声增益"), CHS("自动混音模块"), 0x0E00, "void", "gain"},
        {0x000B0010, CHS("设置噪声增益"), CHS("自动混音模块"), 0x0E00, "gain", "void"},
        {0x000B0011, CHS("获取输入通道静音开关状态"), CHS("自动混音模块"), 0x0E00, "ch", "state01"},
        {0x000B0012, CHS("设置输入通道静音开关状态"), CHS("自动混音模块"), 0x0E00, "ch_state01", "void"},
        {0x000B0013, CHS("获取输入通道增益/阈值"), CHS("自动混音模块"), 0x0E00, "ch", "gain"},
        {0x000B0014, CHS("设置输入通道增益/阈值"), CHS("自动混音模块"), 0x0E00, "ch_gain", "void"},
        {0x000B0015, CHS("获取输出静音开关状态"), CHS("自动混音模块"), 0x0E00, "void", "state01"},
        {0x000B0016, CHS("设置输出静音开关状态"), CHS("自动混音模块"), 0x0E00, "state01", "void"},
        {0x000B0017, CHS("获取输出增益"), CHS("自动混音模块"), 0x0E00, "void", "gain"}, 
        {0x000B0018, CHS("设置输出增益"), CHS("自动混音模块"), 0x0E00, "gain", "void"}, 
        {0x000B0019, CHS("获取输入通道人控开关状态"), CHS("自动混音模块"), 0x0E00, "ch", "state01"},
        {0x000B001A, CHS("设置输入通道人控开关状态"), CHS("自动混音模块"), 0x0E00, "ch_state01", "void"},
        {0x000B001B, CHS("获取输入通道模式"), CHS("自动混音模块"), 0x0E00, "ch", "automix"},
        {0x000B001C, CHS("设置输入通道模式"), CHS("自动混音模块"), 0x0E00, "ch_automix", "void"},
        {0x000B001D, CHS("获取输入通道优先级"), CHS("自动混音模块"), 0x0E00, "ch", "priority"},
        {0x000B001E, CHS("设置输入通道优先级"), CHS("自动混音模块"), 0x0E00, "ch_priority", "void"},
        {0x000B001F, CHS("获取自适应阈值"), CHS("自动混音模块"), 0x0E00, "void", "bgper"},
        {0x000B0020, CHS("设置自适应阈值"), CHS("自动混音模块"), 0x0E00, "bgper", "void"},
        {0x000B0023, CHS("获取保持最后一个麦状态开关"), CHS("自动混音模块"), 0x0E00, "void", "lastmic"},
        {0x000B0024, CHS("设置保持最后一个麦状态开关"), CHS("自动混音模块"), 0x0E00, "lastmic", "void"},
        {0x000B0025, CHS("获取默认通道号"), CHS("自动混音模块"), 0x0E00, "void", "ch"},
        {0x000B0026, CHS("设置默认通道号"), CHS("自动混音模块"), 0x0E00, "ch", "void"},
        {0x000B0027, CHS("获取高于噪声的阈值电平"), CHS("自动混音模块"), 0x0E00, "void", "gain"},
        {0x000B0028, CHS("设置高于噪声的阈值电平"), CHS("自动混音模块"), 0x0E00, "gain", "void"},
        {0x000B0029, CHS("获取最大打开话筒数"), CHS("自动混音模块"), 0x0E00, "void", "maxmic"},
        {0x000B002A, CHS("设置最大打开话筒数"), CHS("自动混音模块"), 0x0E00, "maxmic", "void"},
        {0x000B002B, CHS("获取直接输出开关状态"), CHS("自动混音模块"), 0x0E00, "void", "state01"},
        {0x000B002C, CHS("设置直接输出开关状态"), CHS("自动混音模块"), 0x0E00, "state01", "void"},
        {0x000B002D, CHS("获取衰减类型"), CHS("自动混音模块"), 0x0E00, "void", "atttype"},
        {0x000B002E, CHS("设置衰减类型"), CHS("自动混音模块"), 0x0E00, "atttype", "void"},
        {0x000B002F, CHS("获取衰减幅度"), CHS("自动混音模块"), 0x0E00, "void", "attstep"},
        {0x000B0030, CHS("设置衰减幅度"), CHS("自动混音模块"), 0x0E00, "attstep", "void"},
        {0x000B0031, CHS("获取最大衰减"), CHS("自动混音模块"), 0x0E00, "void", "attmax"},
        {0x000B0032, CHS("设置最大衰减"), CHS("自动混音模块"), 0x0E00, "attmax", "void"},
        {0x000B0033, CHS("获取响应时间"), CHS("自动混音模块"), 0x0E00, "void", "time"},
        {0x000B0034, CHS("设置响应时间"), CHS("自动混音模块"), 0x0E00, "time", "void"},
        //回声消除模块     
        {0x000C0001, CHS("获取本地输入增益"), CHS("回声消除模块"), 0x0D00, "void", "gain"},
        {0x000C0002, CHS("设置本地输入增益"), CHS("回声消除模块"), 0x0D00, "gain", "void"},
        {0x000C0003, CHS("获取远端输入增益"), CHS("回声消除模块"), 0x0D00, "void", "gain"},
        {0x000C0004, CHS("设置远端输入增益"), CHS("回声消除模块"), 0x0D00, "gain", "void"},
        {0x000C0005, CHS("获取本地输出增益"), CHS("回声消除模块"), 0x0D00, "void", "gain"},
        {0x000C0006, CHS("设置本地输出增益"), CHS("回声消除模块"), 0x0D00, "gain", "void"},
        {0x000C0007, CHS("获取远端输出增益"), CHS("回声消除模块"), 0x0D00, "void", "gain"},
        {0x000C0008, CHS("设置远端输出增益"), CHS("回声消除模块"), 0x0D00, "gain", "void"},
        {0x000C0009, CHS("获取本地输入到本地输出增益"), CHS("回声消除模块"), 0x0D00, "void", "gain"},
        {0x000C000A, CHS("设置本地输入到本地输出增益"), CHS("回声消除模块"), 0x0D00, "gain", "void"},
        {0x000C000B, CHS("获取本地输入到本地输出增益开关状态"), CHS("回声消除模块"), 0x0D00, "void", "state00"},
        {0x000C000C, CHS("设置本地输入到本地输出增益开关状态"), CHS("回声消除模块"), 0x0D00, "state00", "void"},
        {0x000C000D, CHS("获取降噪值"), CHS("回声消除模块"), 0x0D00, "void", "nrgain"},
        {0x000C000E, CHS("设置降噪值"), CHS("回声消除模块"), 0x0D00, "nrgain", "void"},
        {0x000C000f, CHS("获取降噪开关状态"), CHS("回声消除模块"), 0x0D00, "void", "state01"},
        {0x000C0010, CHS("设置降噪开关状态"), CHS("回声消除模块"), 0x0D00, "state01", "void"},
        //闪避模块     
        {0x000D0001, CHS("获取启控时间"), CHS("闪避模块"), 0x1200, "void", "time"},
        {0x000D0002, CHS("设置启控时间"), CHS("闪避模块"), 0x1200, "time", "void"},
        {0x000D0003, CHS("获取释放时间"), CHS("闪避模块"), 0x1200, "void", "time"},
        {0x000D0004, CHS("设置释放时间"), CHS("闪避模块"), 0x1200, "time", "void"},
        {0x000D0005, CHS("获取保持时间"), CHS("闪避模块"), 0x1200, "void", "time"},
        {0x000D0006, CHS("设置保持时间"), CHS("闪避模块"), 0x1200, "time", "void"},
        {0x000D0007, CHS("获取检测时间"), CHS("闪避模块"), 0x1200, "void", "time"},
        {0x000D0008, CHS("设置检测时间"), CHS("闪避模块"), 0x1200, "time", "void"},
        {0x000D0009, CHS("获取阈值电平"), CHS("闪避模块"), 0x1200, "void", "gain"},
        {0x000D000A, CHS("设置阈值电平"), CHS("闪避模块"), 0x1200, "gain", "void"},
        {0x000D000B, CHS("获取深度电平"), CHS("闪避模块"), 0x1200, "void", "gain"},
        {0x000D000C, CHS("设置深度电平"), CHS("闪避模块"), 0x1200, "gain", "void"},
        {0x000D000D, CHS("获取输入增益"), CHS("闪避模块"), 0x1200, "void", "gain"},
        {0x000D000E, CHS("设置输入增益"), CHS("闪避模块"), 0x1200, "gain", "void"},
        {0x000D000f, CHS("获取优先级增益"), CHS("闪避模块"), 0x1200, "void", "gain"},
        {0x000D0010, CHS("设置优先级增益"), CHS("闪避模块"), 0x1200, "gain", "void"},
        {0x000D0011, CHS("获取输出增益"), CHS("闪避模块"), 0x1200, "void", "gain"},
        {0x000D0012, CHS("设置输出增益"), CHS("闪避模块"), 0x1200, "gain", "void"},
        {0x000D0013, CHS("获取直通开关状态"), CHS("闪避模块"), 0x1200, "void", "state01"},
        {0x000D0014, CHS("设置直通开关状态"), CHS("闪避模块"), 0x1200, "state01", "void"},
        //淡入淡出模块     
        {0x000E0001, CHS("获取淡入淡出时间"), CHS("淡入淡出模块"), 0x1300, "ramp", "time"},
        {0x000E0002, CHS("设置淡入淡出时间"), CHS("淡入淡出模块"), 0x1300, "ramp_time", "void"},
        {0x000E0003, CHS("获取淡入淡出增益"), CHS("淡入淡出模块"), 0x1300, "ramp", "gain"},
        {0x000E0004, CHS("设置淡入淡出增益"), CHS("淡入淡出模块"), 0x1300, "ramp_gain", "void"},
        {0x000E0005, CHS("获取淡入淡出有效增益模式"), CHS("淡入淡出模块"), 0x1300, "void", "rpeffect"},
        {0x000E0006, CHS("设置淡入淡出有效增益模式"), CHS("淡入淡出模块"), 0x1300, "rpeffect", "void"},
    };



//cmdKeyModel
CmdKeyModel::CmdKeyModel(QObject *parent)
{
    _headeList << tr("CmdKey");// << tr("CmdName") << tr("GroupName");
    // setCmdGroup();
}


CmdKeyModel::~CmdKeyModel()
{

}

int CmdKeyModel::rowCount(const QModelIndex &parent) const
{
    return CmdList.count();
}

int CmdKeyModel::columnCount(const QModelIndex &parent) const
{
    return _headeList.size();
}

QVariant CmdKeyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        if (section < _headeList.size())
        {
            return _headeList[section];
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

//void CmdKeyModel::setCmdGroup(quint32 cmdGroup)
//{
//    _cmdList.clear();
//    if(cmdGroup == 0xFFFFFFFF){
//        _cmdList= SrcCmdList;
//    }
//    else{
//        for(int i = 0; i< SrcCmdList.length(); i++)
//        {
//            if(SrcCmdList[i].CmdGroup == cmdGroup){
//                _cmdList.append(SrcCmdList[i]);
//            }
//        }
//    }
//}


//CenterCtrlCmd CmdKeyModel::getCmdByKey(quint32 cmdKey)
//{
//    for(CenterCtrlCmd cmd : SrcCmdList)
//    {
//        if(cmd.CmdKey == cmdKey){
//            return cmd;
//        }
//    }
//    return SrcCmdList[0];
//}

QVariant CmdKeyModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        const int row=index.row();
        switch(index.column())
        {
        case 0: return QString("0x%1(%2)").arg(CmdList.at(row).CmdKey,8,16,QLatin1Char('0'))
                    .arg(CmdList.at(row).CmdName);
        }
    }
    return QVariant();
}

