#include "cfgsettingaudioplayer.h"
#include "ui_cfgsettingaudioplayer.h"
#include "configItem/configitemaudioplayer.h"
#include <QGridLayout>
#include "common.h"
#include "device/devicemanager.h"
#include "device/device_trn.h"

CfgSettingAudioPlayer::CfgSettingAudioPlayer(ConfigItemAudioPlayer *cfgItem, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CfgSettingAudioPlayer),
    _cfgItem(cfgItem)
{
    ui->setupUi(this);

    initUI();
    initConnectSlots();

    //获取文件列表:发送命令
    Device_Trn* trnDev = getTrnDevice();
    if(trnDev != NULL){
        trnDev->reflashFiles();
    }

    QFile file(":/WidgetStyle/AudioPlayer.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        file.close();
        setStyleSheet(qss);
    }
}

CfgSettingAudioPlayer::~CfgSettingAudioPlayer()
{
    delete ui;
}


void CfgSettingAudioPlayer::updateUiData()
{
    ;
}

void CfgSettingAudioPlayer::initUI()
{
    ui->widget_gain->setFixedSize(40, 40);
    ui->widget_gain->setRangeParam(-100, 20, StepMode_Line, ParamType_Gain);
    ui->widget_gain->setDecimalsNum(1);


    ui->pushButton_loop->setCheckable(true);
    ui->pushButton_pause->setCheckable(true);
    ui->pushButton_mute->setCheckable(true);
    ui->lineEdit_status->setFocusPolicy(Qt::NoFocus);

    ui->pushButton_loop->setVisible(false);//wj 屏蔽暂未实现功能
    ui->pushButton_mute->setVisible(false);
    ui->widget_gain->setVisible(false);
}


Device_Trn* CfgSettingAudioPlayer::getTrnDevice()
{
    ConfigScene* cfgScene = qobject_cast<ConfigScene*>(_cfgItem->scene());
    if(cfgScene != NULL) {
        return cfgScene->getTrnDev();
    }
    return NULL;
}

void CfgSettingAudioPlayer::initConnectSlots()
{
    Device_Trn* trnDev = getTrnDevice();
    if(trnDev != NULL){
        connect(trnDev, SIGNAL(fileListChanged(QStringList)), this, SLOT(onFileListChanged(QStringList)));
    }

    connect(ui->pushButton_play, SIGNAL(clicked()), this, SLOT(onPlay()));
    connect(ui->pushButton_stop, SIGNAL(clicked()), this, SLOT(onStop()));
    connect(ui->pushButton_pause, SIGNAL(toggled(bool)), this, SLOT(onPauseResume(bool)));
    connect(ui->pushButton_reflash, SIGNAL(clicked()), this, SLOT(onReflashFiles()));


//    connect(ui->pushButton_fistBack, SIGNAL(clicked()), this, SLOT(onPushButtonClicked_slots()));
//    connect(ui->pushButton_speed, SIGNAL(clicked()), this, SLOT(onPushButtonClicked_slots()));
//    connect(ui->pushButton_loop, SIGNAL(clicked()), this, SLOT(onPushButtonClicked_slots()));
//    connect(ui->pushButton_startup, SIGNAL(clicked()), this, SLOT(onPushButtonClicked_slots()));
//    connect(ui->widget_gain->getDoubleSpinBox(), SIGNAL(editingFinished()), this, SLOT(onGainEditingFinished_slot()));
//    connect(ui->pushButton_mute, SIGNAL(clicked()), this, SLOT(onPushButtonClicked_slots()));
//    connect(ui->comboBox_root, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxCurrentIndexChanged_slots(int)));
//    connect(ui->comboBox_dir, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxCurrentIndexChanged_slots(int)));
//    connect(ui->comboBox_file, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxCurrentIndexChanged_slots(int)));
}


void CfgSettingAudioPlayer::onPlay()
{
    QString fileName = ui->comboBox_file->currentText();
    int startPos = fileName.indexOf(":") + 1;
    fileName = fileName.right(fileName.length() - startPos);
    _cfgItem->play(fileName);
}


void CfgSettingAudioPlayer::onStop()
{
    _cfgItem->stop();
}

void CfgSettingAudioPlayer::onPause()
{
    _cfgItem->pause();
}

void CfgSettingAudioPlayer::onResume()
{
    _cfgItem->resume();
}

void CfgSettingAudioPlayer::onPauseResume(bool checked)
{
    if(checked) {
        _cfgItem->pause();
    }
    else {
        _cfgItem->resume();
    }
}

void CfgSettingAudioPlayer::onReflashFiles()
{
    ui->pushButton_reflash->setEnabled(false);
    Device_Trn* trnDev = getTrnDevice();
    if(trnDev != NULL){
        trnDev->reflashFiles();
    }
    QPushButton *btn = ui->pushButton_reflash;
    QTimer::singleShot(600, this, [btn]
    {
        btn->setEnabled(true);
    });
}


QString secondsToTimeSpan(int iSeconds)
{
    int iMinute = iSeconds / 60;
    int iSecond = (iSeconds - iMinute * 60);
    QString strTime = QString("%1:%2").arg(iMinute, 2, 10, QChar('0')).arg(iSecond, 2, 10, QChar('0'));
    return strTime;
}

void CfgSettingAudioPlayer::DispRtnData(CfgType type, unsigned int *d)
{
    static QString lastTimeValue = "";
    FC_RTU_PLAYINFO *playInfo = (FC_RTU_PLAYINFO *)d;
    quint16 state = playInfo->state;
    int totalTime = playInfo->playedTime + playInfo->remainsTime;
    QString timeValue = secondsToTimeSpan(playInfo->playedTime) + " / " + secondsToTimeSpan(totalTime);
    if(state == 1 && lastTimeValue != timeValue)
    {
        lastTimeValue = timeValue;
        ui->lineEdit_status->setText(QString("%1").arg(state));

        double value = playInfo->playedTime*100.0 / totalTime;
        ui->progressBar->setValue(value);
        ui->progressBar->setFormat(timeValue);
    }
//    ui->label_remainsTime->setText(remainsTime);
}


void CfgSettingAudioPlayer::onFileListChanged(QStringList playFiles)
{
    ui->comboBox_file->clear();
    for(QString file : playFiles)
    {
        ui->comboBox_file->addItem(file);
    }
}



