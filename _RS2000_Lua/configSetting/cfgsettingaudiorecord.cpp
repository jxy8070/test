#include "cfgsettingaudiorecord.h"
#include "ui_cfgsettingaudiorecord.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>

CfgSettingAudioRecord::CfgSettingAudioRecord(ConfigItemAudioRecord *cfgItem, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CfgSettingAudioRecord),
    _cfgItem(cfgItem)
{
    ui->setupUi(this);

    initUI();
    connectSlots();

    QFile file(":/WidgetStyle/AudioPlayer.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        this->setStyleSheet(qss);
        file.close();
    }
}

CfgSettingAudioRecord::~CfgSettingAudioRecord()
{
    delete ui;
}

void CfgSettingAudioRecord::initUI()
{
    QStringList formatList, bitrateList, sampleRateList, LimitFileLengthList, cleanupFilesList, maximumAgeList, totalSizeLimitList;
    formatList << "MP3" << "WAV" << "FLAC";
    bitrateList << "16 bit fixed" << "24 bit fixed" << "32 bit float";
    sampleRateList << "48 KHz" << "44.1 KHz" << "32 KHz" << "16KHz";
    LimitFileLengthList << "max supported" << "1 minute" << "5 minutes" << "15 minutes" << "30 minutes" << "1 hour" << "2 hours";
    cleanupFilesList << "never" << "in directory" << "matching prefix";
    maximumAgeList << "1 day" << "3 days" << "7 days" << "1 month" << "3 months" << "6 months" << "1 year" << "unlimited";
    totalSizeLimitList << "100 MB" << "200 MB" << "500 MB" << "1 GB" << "2 GB" << "5 GB"
                       << "10 GB" << "20 GB" << "50 GB" << "100 GB" << "space available";
    ui->comboBox_format->addItems(formatList);
    ui->comboBox_bitrate->addItems(bitrateList);
    ui->comboBox_sampleRate->addItems(sampleRateList);
    ui->comboBox_limitFileLength->addItems(LimitFileLengthList);
    ui->comboBox_cleanupFiles->addItems(cleanupFilesList);
    ui->comboBox_maximunAge->addItems(maximumAgeList);
    ui->comboBox_TotalSizeLimit->addItems(totalSizeLimitList);

    ui->lineEdit_fileName->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_length->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_timeAveilable->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_spaceAveilable->setFocusPolicy(Qt::NoFocus);


    QGridLayout *layOut = new QGridLayout();

    layOut->addWidget(new QWidget(), 0, 0);
    layOut->addWidget(ui->label_record, 1, 2, Qt::AlignCenter);
    layOut->addWidget(ui->label_stop, 1, 3, Qt::AlignCenter);
    layOut->addWidget(ui->pushButton_record, 2, 2, Qt::AlignCenter);
    layOut->addWidget(ui->pushButton_stop, 2, 3, Qt::AlignCenter);

    layOut->addWidget(ui->label_root, 3, 1, Qt::AlignVCenter);
    layOut->addWidget(ui->comboBox_root, 3, 2, 1, 3, Qt::AlignVCenter);

    layOut->addWidget(ui->label_dir, 4, 1, Qt::AlignVCenter);
    layOut->addWidget(ui->comboBox_dir, 4, 2, 1, 3, Qt::AlignVCenter);

    layOut->addWidget(ui->label_prefix, 5, 1, Qt::AlignVCenter);
    layOut->addWidget(ui->lineEdit_prefix, 5, 2, 1, 3, Qt::AlignVCenter);

    layOut->addWidget(ui->label_include, 6, 1, Qt::AlignVCenter);
    layOut->addWidget(ui->checkBox_data, 6, 2, Qt::AlignLeft);
    layOut->addWidget(ui->checkBox_time, 6, 3, Qt::AlignLeft);
    layOut->addWidget(ui->checkBox_sequence, 6, 4, Qt::AlignLeft);

    layOut->addWidget(ui->label_fileName, 7, 1, Qt::AlignVCenter);
    layOut->addWidget(ui->lineEdit_fileName, 7, 2, 1, 3, Qt::AlignVCenter);

    layOut->addWidget(ui->label_format, 8, 2, Qt::AlignVCenter);
    layOut->addWidget(ui->label_Bitrate, 8, 3, Qt::AlignVCenter);
    layOut->addWidget(ui->label_samplerate, 8, 4, Qt::AlignVCenter);
    layOut->addWidget(ui->comboBox_format, 9, 2, Qt::AlignVCenter);
    layOut->addWidget(ui->comboBox_bitrate, 9, 3, Qt::AlignVCenter);
    layOut->addWidget(ui->comboBox_sampleRate, 9, 4, Qt::AlignVCenter);

    layOut->addWidget(ui->label_length, 10, 2, Qt::AlignVCenter);
    layOut->addWidget(ui->label_timeAvailable, 10, 3, Qt::AlignVCenter);
    layOut->addWidget(ui->label_spaceAvailable, 10, 4, Qt::AlignVCenter);
    layOut->addWidget(ui->lineEdit_length, 11, 2, Qt::AlignVCenter);
    layOut->addWidget(ui->lineEdit_timeAveilable, 11, 3, Qt::AlignVCenter);
    layOut->addWidget(ui->lineEdit_spaceAveilable, 11, 4, Qt::AlignVCenter);

    layOut->addWidget(ui->label_limitFileLength, 12, 2, Qt::AlignVCenter);
    layOut->addWidget(ui->comboBox_limitFileLength, 12, 3, Qt::AlignVCenter);

    layOut->addWidget(ui->checkBox_Continuous, 13, 3, 1, 2, Qt::AlignLeft);
    layOut->addWidget(ui->checkBox_Quantize, 14, 3, 1, 2, Qt::AlignLeft);

    layOut->addWidget(ui->label_cleanupFiles, 15, 2, Qt::AlignVCenter);
    layOut->addWidget(ui->comboBox_cleanupFiles, 15, 3, Qt::AlignVCenter);

    layOut->addWidget(ui->label_maximunAge, 16, 2, Qt::AlignVCenter);
    layOut->addWidget(ui->comboBox_maximunAge, 16, 3, Qt::AlignVCenter);

    layOut->addWidget(ui->label_TotalSizeLimit, 17, 2, Qt::AlignVCenter);
    layOut->addWidget(ui->comboBox_TotalSizeLimit, 17, 3, Qt::AlignVCenter);
    layOut->addWidget(new QWidget(), 18, 5);

    this->setLayout(layOut);

    ui->checkBox_Continuous->setEnabled(false);
    ui->checkBox_Quantize->setEnabled(false);
    ui->comboBox_maximunAge->setEnabled(false);
    ui->comboBox_TotalSizeLimit->setEnabled(false);
}

void CfgSettingAudioRecord::connectSlots()
{
    connect(ui->checkBox_data, &QPushButton::clicked, this, &CfgSettingAudioRecord::onCheckBoxBtnClicked_slots);
    connect(ui->checkBox_time, &QPushButton::clicked, this, &CfgSettingAudioRecord::onCheckBoxBtnClicked_slots);
    connect(ui->checkBox_sequence, &QPushButton::clicked, this, &CfgSettingAudioRecord::onCheckBoxBtnClicked_slots);
    connect(ui->checkBox_Continuous, &QPushButton::clicked, this, &CfgSettingAudioRecord::onCheckBoxBtnClicked_slots);
    connect(ui->checkBox_Quantize, &QPushButton::clicked, this, &CfgSettingAudioRecord::onCheckBoxBtnClicked_slots);

    connect(ui->comboBox_format, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CfgSettingAudioRecord::onComboBoxCurrentIndexChanged_slots);
    connect(ui->comboBox_bitrate, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CfgSettingAudioRecord::onComboBoxCurrentIndexChanged_slots);
    connect(ui->comboBox_sampleRate, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CfgSettingAudioRecord::onComboBoxCurrentIndexChanged_slots);
    connect(ui->comboBox_limitFileLength, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CfgSettingAudioRecord::onComboBoxCurrentIndexChanged_slots);
    connect(ui->comboBox_cleanupFiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CfgSettingAudioRecord::onComboBoxCurrentIndexChanged_slots);
    connect(ui->comboBox_sampleRate, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CfgSettingAudioRecord::onComboBoxCurrentIndexChanged_slots);
    connect(ui->comboBox_maximunAge, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CfgSettingAudioRecord::onComboBoxCurrentIndexChanged_slots);
    connect(ui->comboBox_TotalSizeLimit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CfgSettingAudioRecord::onComboBoxCurrentIndexChanged_slots);
}

void CfgSettingAudioRecord::onCheckBoxBtnClicked_slots()
{
    if(sender() == ui->checkBox_data) {

    }else if(sender() == ui->checkBox_time) {

    }else if(sender() == ui->checkBox_sequence) {

    }else if(sender() == ui->checkBox_Continuous) {
        if(ui->checkBox_Continuous->isChecked()) {
            ui->checkBox_Quantize->setEnabled(true);
        }else {
            ui->checkBox_Quantize->setEnabled(false);
        }
    }else if(sender() == ui->checkBox_Quantize) {

    }
}

void CfgSettingAudioRecord::onComboBoxCurrentIndexChanged_slots(int value)
{
    if(sender() == ui->comboBox_format) {

    }else if(sender() == ui->comboBox_bitrate) {

    }else if(sender() == ui->comboBox_sampleRate) {

    }else if(sender() == ui->comboBox_limitFileLength) {
        if(!value) {
            ui->checkBox_Continuous->setEnabled(false);
            ui->checkBox_Quantize->setEnabled(false);
        }else {
            ui->checkBox_Continuous->setEnabled(true);
            if(!ui->checkBox_Continuous->isChecked())
            {
                ui->checkBox_Quantize->setEnabled(false);
            }else {
                ui->checkBox_Quantize->setEnabled(true);
            }
        }
    }else if(sender() == ui->comboBox_cleanupFiles) {
        if(!value) {
            ui->comboBox_maximunAge->setEnabled(false);
            ui->comboBox_TotalSizeLimit->setEnabled(false);
        }else {
            ui->comboBox_maximunAge->setEnabled(true);
            ui->comboBox_TotalSizeLimit->setEnabled(true);
        }
    }else if(sender() == ui->comboBox_maximunAge) {

    }else if(sender() == ui->comboBox_TotalSizeLimit) {

    }
}
