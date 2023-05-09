#include "modeldockwidget.h"
#include "config.h"

ModelDockWidget::ModelDockWidget(const QString &title) : DockWidget(title)
{
    QWidget* mainWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setObjectName("mainLayout");
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    //work box
    QWidget *toolWidet = new QWidget;
    toolWidet->setObjectName("toolWidet");
    toolWidet->setStyleSheet("border: 1px solid #828790");
    QGridLayout *toolLayout = new QGridLayout(toolWidet);
    toolLayout->setContentsMargins(2, 2, 2, 2);
    toolLayout->setSpacing(2);

    QPushButton *textBtn    = new QPushButton;
    QPushButton *groupBoxBtn = new QPushButton;
    QPushButton *polylineBtn = new QPushButton;

    textBtn->setProperty("toolkitType", ConfigScene::ToolkitType_text);
    groupBoxBtn->setProperty("toolkitType", ConfigScene::ToolkitType_GroupBox);
    polylineBtn->setProperty("toolkitType", ConfigScene::ToolkitType_Polyline);

    textBtn->setCheckable(true);
    groupBoxBtn->setCheckable(true);
    polylineBtn->setCheckable(true);

    textBtn->setFixedSize(18, 18);
    groupBoxBtn->setFixedSize(18, 18);
    polylineBtn->setFixedSize(18, 18);

    textBtn->setObjectName("TextBtn");
    groupBoxBtn->setObjectName("GroupBoxBtn");
    polylineBtn->setObjectName("PolylineBtn");
    connect(textBtn, &QPushButton::clicked, this, &ModelDockWidget::onToolBtnCheckedSlot);
    connect(groupBoxBtn, &QPushButton::clicked, this, &ModelDockWidget::onToolBtnCheckedSlot);
    connect(polylineBtn, &QPushButton::clicked, this, &ModelDockWidget::onToolBtnCheckedSlot);
    _toolKitBtnList.append(textBtn);
    _toolKitBtnList.append(groupBoxBtn);
    _toolKitBtnList.append(polylineBtn);


    toolLayout->addWidget(textBtn, 0, 0);
    toolLayout->addWidget(groupBoxBtn, 0, 1);
    toolLayout->addWidget(polylineBtn, 0, 2);

    _SearchText = new QLineEdit();
    _SearchText->setObjectName("searchText");
    _SearchText->setPlaceholderText("Search");
    _SearchText->setFixedHeight(22);
    toolLayout->addWidget(_SearchText, 1, 0, 1, 5);

    QPushButton *pSearchButton = new QPushButton(this);
    pSearchButton->setObjectName("searchButton");
    pSearchButton->setCursor(Qt::PointingHandCursor);
    pSearchButton->setFixedSize(22, 22);
    pSearchButton->setToolTip(QStringLiteral("Search"));
    toolLayout->addWidget(pSearchButton, 1, 6, 1, 1);
    connect(pSearchButton, SIGNAL(clicked(bool)), this, SLOT(search()));
    connect(_SearchText, SIGNAL(returnPressed()), this, SLOT(search()));
    connect(_SearchText, SIGNAL(textChanged(const QString &)), this, SLOT(search()));
    toolWidet->setFixedHeight(46);
    mainLayout->addWidget(toolWidet, Qt::AlignLeft);
    _modelTree = new ToolKitTree(ToolKitTree::TreeStrType_component);
    mainLayout->addWidget(_modelTree);
    this->setWidget(mainWidget);
}

void ModelDockWidget::UpdatePlatformModel(PlatformType platform, int totalPortNum, ConfigScene *pScene)
{
    //    QList<QTreeWidgetItem *> modelTreeItems;
    if(platform == Platform_DSP){
        _modelTree->SetTreeItems(ToolKitTree::GetModelItems_Dsp(totalPortNum, pScene), pScene);
    }
    else if(platform == Platform_X86 || platform == Platform_ARM){
        _modelTree->SetTreeItems(ToolKitTree::GetModelItems_X86(totalPortNum, pScene), pScene);
    }
}

bool ModelDockWidget::FilterModel(CfgType cfg, QString filter)
{
    if(_filterMap.contains(cfg)) {
        return _filterMap[cfg].contains(filter, Qt::CaseInsensitive);
    }
    else{
        return false;
    }
}

void ModelDockWidget::search()
{
    QString filterStr = _SearchText->text();
    QTreeWidgetItemIterator itReset(_modelTree);
    while (*itReset)
    {
        (*itReset++)->setHidden(false);
        continue;
    }
    if(filterStr.isEmpty()) return;

    QTreeWidgetItemIterator it(_modelTree);
    while (*it)
    {
        CfgType cfgType = CfgType((*it)->data(0, DataKey_Type).toInt());
        if(FilterModel(cfgType, filterStr))
        {
            QTreeWidgetItem *item = *it;
            while (item->parent()) //显示父节点
            {
                if(item!=NULL)
                {
                    bool isExpanded = item->isExpanded();
                    if (! isExpanded)
                    {
                        _modelTree->expandItem(item);
                    }
                }
                parentExpand(*it);
                item->parent()->setHidden(false);
                item = item->parent();
            }
        }
        else
        {
            (*it)->setHidden(true); //先隐藏，子项目满足条件时会显示它
        }
        ++it;
    }
}

#include "mainwindow.h"
#include <QMdiSubWindow>
void ModelDockWidget::onToolBtnCheckedSlot(bool checked)
{
    if(checked) {
        foreach(QPushButton *btn, _toolKitBtnList) {
            if(sender() != btn && btn->isChecked()) {
                btn->disconnect(SIGNAL(clicked(bool)));
                btn->setChecked(false);
                connect(btn, &QPushButton::clicked, this, &ModelDockWidget::onToolBtnCheckedSlot);
            }
        }
    }

    ConfigScene::ToolkitType type = ConfigScene::ToolkitType_invalid;

    foreach(QPushButton *btn, _toolKitBtnList) {
        if(sender() == btn) {
            if(btn->isChecked()) {
                QVariant var = btn->property("toolkitType");
                emit sendToolBtnSignal(var.value<ConfigScene::ToolkitType>());
                return ;
            }
        }
    }
    emit sendToolBtnSignal(type);
}

void ModelDockWidget::onToolkitBtnStatusChangedSlot(bool status)
{
    foreach(QPushButton *btn, _toolKitBtnList){
        btn->setChecked(status);
        if(status) {
            QVariant var = btn->property("toolkitType");
            emit sendToolBtnSignal(var.value<ConfigScene::ToolkitType>());
        }else {
            emit sendToolBtnSignal(ConfigScene::ToolkitType_invalid);
        }
    }
}

void ModelDockWidget::parentExpand(QTreeWidgetItem *item)
{
    if (item->parent() != NULL)
    {
        QTreeWidgetItem *pItem = item->parent();
        if (! pItem->isExpanded())
        {
            pItem->setExpanded(true);
        }
        parentExpand(pItem);
    }
}

#define CHS(x) QStringLiteral(x)
QMap<CfgType, QString> ModelDockWidget::_filterMap =
{
    {CFG_TYPE_AUDIO_PLAYER2, CHS("Audio Player 2ch  音频播放器")},
    {CFG_TYPE_AUDIO_RECORD2, CHS("Audio Record 2ch  音频录制器")},
    {CFG_TYPE_DYN_AUTO, CHS("Dynamic automatic gain control 自动增益控制")},
    {CFG_TYPE_DYN_COMP, CHS("Dynamic compressor 压缩器")},
    {CFG_TYPE_DYN_EXPAND, CHS("Dynamic expander 扩展器")},
    {CFG_TYPE_DYN_NOISE, CHS("Dynamic noise gate 噪声门")},
    {CFG_TYPE_DYN_LIMIT, CHS("Dynamic peak limit 限制器")},
    {CFG_TYPE_PDUCKER_4, CHS("Priority Ducker 优先闪避器")},
    {CFG_TYPE_EQ_3, CHS("Parametric Equalizer 3 参量均衡")},
    {CFG_TYPE_EQ_5, CHS("Parametric Equalizer 5 参量均衡")},
    {CFG_TYPE_EQ_1X3, CHS("Graphic Equalizer 图示均衡")},
    {CFG_TYPE_SPLIT_2, CHS("Crossover Spliter frequnzy  分频器")},
    {CFG_TYPE_FILTE_HP, CHS("High-pass Filter 高通滤波器")},
    {CFG_TYPE_FILTE_LP, CHS("Low-pass Filter 低通滤波器")},
    {CFG_TYPE_FILTER_BP, CHS("Band-pass Filter 带通滤波器")},
    {CFG_TYPE_FILTER_AP, CHS("All-Pass Filter 全通滤波器")},
    {CFG_TYPE_SOFTFIR_MONO256, CHS("FIR Filter FIR滤波器")},
    {CFG_TYPE_FREQSHIFT, CHS("Frequnzy shifter Notch Feedback Controller(NFC) 移频型反馈抑制器")},
    {CFG_TYPE_NFC1_8NF, CHS("Notch Feedback Controller(NFC) 增强型反馈抑制器")},
    {CFG_TYPE_MIXER_4X1, CHS("4-Channel inputs Mixers 4通道混音器")},
    {CFG_TYPE_MIXER_8X1, CHS("8-Channel inputs Mixers 8通道混音器")},
    {CFG_TYPE_MIXER_16X16, CHS("16-Channel inputs Mixers 16通道混音器")},
    {CFG_TYPE_MIXER_32X32, CHS("32-Channel inputs Mixers 32通道混音器")},
    {CFG_TYPE_MIXER_64X64, CHS("64-Channel inputs Mixers 64通道混音器")},
    {CFG_TYPE_AUTOMIXNGAT_4, CHS("Gating Automatic Mixer 4通道自动混音器")},

    {CFG_TYPE_ROUTER_4, CHS("4-Channel inputs Routers 4X4通道路由器")},
    {CFG_TYPE_ROUTER_8, CHS("8-Channel inputs Routers 8X8通道路由器")},
    {CFG_TYPE_ROUTER_16, CHS("16-Channel inputs Routers 16X16通道路由器")},
    {CFG_TYPE_ROUTER_32, CHS("32-Channel inputs Routers 32X32通道路由器")},
    {CFG_TYPE_ROUTER_64, CHS("64-Channel inputs Routers 64X64通道路由器")},
    {CFG_TYPE_ROUTER_96, CHS("96-Channel inputs Routers 96X96通道路由器")},
    {CFG_TYPE_ROUTER_128, CHS("128-Channel inputs Routers 128X128通道路由器")},
    {CFG_TYPE_ROUTER_192, CHS("192-Channel inputs Routers 192X192通道路由器")},
    {CFG_TYPE_ROUTER_256, CHS("256-Channel inputs Routers 256X256通道路由器")},
    {CFG_TYPE_GAIN_1, CHS("Automatic Gain 增益调节")},
    {CFG_TYPE_GAINRAMP_1, CHS("Gain Ramp 淡入淡出")},
    {CFG_TYPE_HPDELY_1, CHS("Delay 1ms 高精度延时器")},
    {CFG_TYPE_DELAY_100, CHS("Delay 100ms 延时器")},
    {CFG_TYPE_DELAY_1000, CHS("Delay 1000ms 延时器")},
    {CFG_TYPE_AEC_2_2, CHS("Acoustic Echo Canceler(AEC-NR) 硬件回声消除")},
    {CFG_TYPE_SOFTAEC_8K128, CHS("Acoustic Echo Canceler(AEC) 软件回声消除")},
    {CFG_TYPE_SIG_SINE, CHS("Sine Generators 正弦波 信号发生器")},
    {CFG_TYPE_SIG_PINKNOISE, CHS("Pink Noise Generators 粉噪声 信号发生器")},
    {CFG_TYPE_SIG_NOISE, CHS("White Noise Generators 白噪声 信号发生器")},
    {CFG_TYPE_METER_1, CHS("1Channel-Meter 1通道电平表")},
    {CFG_TYPE_METER_2, CHS("2Channel-Meter 2通道电平表")},
    {CFG_TYPE_METER_4, CHS("4Channel-Meter 4通道电平表")},
    {CFG_TYPE_METER_8, CHS("8Channel-Meter 8通道电平表")},
    {CFG_TYPE_VOICETUBE_1, CHS("Microphone control 话筒控制")},
    {CFG_TYPE_VOICETUBE_2, CHS("Microphone control 话筒控制")},
    {CFG_TYPE_GPI, CHS("Gpi 消防强切")},
    {CFG_TYPE_CROSSFADER_2X1, CHS("Crossfader 交叉衰减器")},
    {CFG_TYPE_CTRL_LUA, CHS("Lua Script Control Lua控制模块")},
};


