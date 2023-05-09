#include "stable.h"
#include "cfgsettingeq.h"
#include "configItem/configitemeq.h"
#include "customWidget/eqgraph.h"
#include "customWidget/eqviewnormal.h"
#include "customWidget/eqviewshg.h"
#include "Protocal/packet.h"

CfgSettingEq::CfgSettingEq(ConfigItemEq *cfgItem, QWidget *parent)
    : QWidget(parent)
{
    _cfgItemEq = cfgItem;

    switch(cfgItem->getCfgType()) {
    case CFG_TYPE_EQ_1:
    case CFG_TYPE_EQ_3:
    case CFG_TYPE_EQ_5:
        setupUiNormal(_cfgItemEq->getAllBypass(), _cfgItemEq->getEqDataList());
        break;
    case CFG_TYPE_EQ_7:
        setupUiNormal(_cfgItemEq->getAllBypass(), _cfgItemEq->getEqDataList());
        break;
    case CFG_TYPE_EQ_10:
        setupUiNormal(_cfgItemEq->getAllBypass(), _cfgItemEq->getEqDataList());
        break;
    case CFG_TYPE_EQ_16:
        setupUiNormal(_cfgItemEq->getAllBypass(), _cfgItemEq->getEqDataList());
        break;
    case CFG_TYPE_EQ_1X3:    //1/3倍频        
        setupUiShg(EqViewSHG::SHG_1X3, _cfgItemEq->getAllBypass(), _cfgItemEq->getEqDataList());
        break;
    case CFG_TYPE_EQ_2X3:    //2/3倍频        
        setupUiShg(EqViewSHG::SHG_2X3, _cfgItemEq->getAllBypass(), _cfgItemEq->getEqDataList());
        break;
    case CFG_TYPE_EQ_3X3:    //1倍频
        setupUiShg(EqViewSHG::SHG_3X3, _cfgItemEq->getAllBypass(), _cfgItemEq->getEqDataList());
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingEq::updateUiData()
{
    switch(_cfgItemEq->getCfgType()) {
    case CFG_TYPE_EQ_1:
    case CFG_TYPE_EQ_3:
    case CFG_TYPE_EQ_5:
    case CFG_TYPE_EQ_7:
    case CFG_TYPE_EQ_10:
    case CFG_TYPE_EQ_16:
        _normalView->reinitEqData(_cfgItemEq->getAllBypass(), _cfgItemEq->getEqDataList());
        for (int i = 0; i < _cfgItemEq->getEqDataList().count(); ++i) {
            _normalView->setSegBypass(i, _cfgItemEq->getSegBypass(i));
        }
        break;
    case CFG_TYPE_EQ_1X3:    //1/3倍频
    case CFG_TYPE_EQ_2X3:    //2/3倍频
    case CFG_TYPE_EQ_3X3:    //1倍频
        _shgView->reinitEqData(_cfgItemEq->getAllBypass(), _cfgItemEq->getEqDataList());
        for (int i = 0; i < _cfgItemEq->getEqDataList().count(); ++i) {
            _shgView->setSetBypass(i, _cfgItemEq->getSegBypass(i));
        }
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingEq::setupUiNormal(bool bypass, QList<EqData *> eqList)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    _normalView = new EqViewNormal(bypass, eqList, this);
    mainLayout->addWidget(_normalView);
    setLayout(mainLayout);
    _normalView->showWidget();
    for (int i = 0; i < eqList.count(); ++i) {
        _normalView->setSegBypass(i, _cfgItemEq->getSegBypass(i));
    }
    connect(_normalView, SIGNAL(eqDataChanged(quint8, bool, int, int, int, int)), this, SLOT(onEqDataChanged(quint8, bool, int, int, int, int)));
    connect(_normalView, SIGNAL(eqDataBypassClicked(bool)), this, SLOT(onEqDataBypassClicked(bool)));
    connect(_normalView, SIGNAL(eqDataReset()), this, SLOT(onEqDataResetClicked()));
}

void CfgSettingEq::setupUiShg(EqViewSHG::SHG_Type type, bool bypass, QList<EqData *> eqList)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    qint16 *gainArray = new qint16[eqList.count()] ;
    for(int i = 0; i<eqList.count(); i++)
    {
        gainArray[i] = eqList.at(i)->gain;
    }

    _shgView = new EqViewSHG(type, bypass, eqList, this);
    _shgView->connectSig();
    mainLayout->addWidget(_shgView);
    setLayout(mainLayout);

    for (int i = 0; i < eqList.count(); ++i) {
        _shgView->setSetBypass(i, _cfgItemEq->getSegBypass(i));
    }

    connect(_shgView, SIGNAL(eqDataChanged(quint8, int)), this, SLOT(onEqDataChanged(quint8, int)));
    connect(_shgView, SIGNAL(eqDataSegBypassClicked(int, bool)), this, SLOT(onEqDataSegBypass(int, bool)));
    connect(_shgView, SIGNAL(eqDataBypassClicked(bool)), this, SLOT(onEqDataBypassClicked(bool)));
    connect(_shgView, SIGNAL(eqDataReset()), this, SLOT(onEqDataResetClicked()));
}

void CfgSettingEq::onEqDataChanged(quint8 idx, bool seg_bypass, int freq, int gain, int q, int type)
{
    _cfgItemEq->setEqData(idx, seg_bypass, freq, gain, q, type);
}

void CfgSettingEq::onEqDataChanged(quint8 idx, int gain)
{
    _cfgItemEq->setEqData(idx, gain);
}

void CfgSettingEq::onEqDataSegBypass(int idx, bool enable)
{
    _cfgItemEq->setSegBypass(idx, enable);
}

void CfgSettingEq::onEqDataBypassClicked(bool enabled)
{
    _cfgItemEq->setAllBypass(enabled);
}

void CfgSettingEq::onEqDataResetClicked()
{
    _cfgItemEq->reinit();
    updateUiData();
}

