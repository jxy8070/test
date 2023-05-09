#include "stable.h"
#include "cfgsettingsplite.h"
#include "ui_CfgSettingSplite.h"
#include "ui_CfgSettingSplite_FangTu.h"
#include "configItem/configitemsplit.h"
#include "Protocal/packet.h"


CfgSettingSplite::CfgSettingSplite(ConfigItemSplit* cfgItem, QWidget *parent)
    : QWidget(parent)
{
    _cfgItem = cfgItem;

#if Logo_FangTu
    initUi_FangTu();
    initSignalConnect_FangTu();
    initData_FangTu();
#else
    initUi();
    initSignalConnect();
    initData();
#endif
}

void CfgSettingSplite::updateUiData()
{
#if Logo_FangTu
    initData_FangTu();
#else
    initData();
#endif
}

CfgSettingSplite::~CfgSettingSplite()
{
    if(_uiSplite)
        delete _uiSplite;
    if(_ui_FangTu)
        delete _ui_FangTu;
}

void CfgSettingSplite::pointChange(int idx)
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        _uiSplite->eqWidget->setCurrentIdx(idx); // 手动回传点击事件
        break;
    case CFG_TYPE_SPLIT_3:
        switch (idx) {
        case 0:
            _uiSplite->eqWidget->setCurrentIdx(idx);
            break;
        case 1:
        case 2:
            _uiSplite->eqWidget->setCurrentIdx(1);
            break;
        case 3:
            _uiSplite->eqWidget->setCurrentIdx(idx);
            break;
        default:
            break;
        }
        break;
    case CFG_TYPE_SPLIT_4:
        switch (idx) {
        case 0:
            _uiSplite->eqWidget->setCurrentIdx(idx);
            break;
        case 1:
        case 2:
            _uiSplite->eqWidget->setCurrentIdx(1);
            break;
        case 3:
        case 4:
            _uiSplite->eqWidget->setCurrentIdx(3);
            break;
        case 5:
            _uiSplite->eqWidget->setCurrentIdx(idx);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void CfgSettingSplite::pointMove(int idx, float fs)
{
//    qDebug() << __FUNCTION__ << __LINE__ << idx << fs;
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        switch (idx) {
        case 0:
            _uiSplite->dsb_Filter_L_Freq->setValue(fs, true);
            break;
        case 1:
            _uiSplite->dsb_Filter_R_Freq->setValue(fs, true);
            break;
        default:
            break;
        }
        break;
    case CFG_TYPE_SPLIT_3:
        switch (idx) {
        case 0:
            _uiSplite->dsb_Filter_L_Freq->setValue(fs, true);
            break;
        case 1:
            _uiSplite->dsb_Filter_L_Freq_M->setValue(fs, true);
            break;
        case 2:
            _uiSplite->dsb_Filter_R_Freq_M->setValue(fs, true);
            break;
        case 3:
            _uiSplite->dsb_Filter_R_Freq->setValue(fs, true);
            break;
        default:
            break;
        }
        break;
    case CFG_TYPE_SPLIT_4:
        switch (idx) {
        case 0:
            _uiSplite->dsb_Filter_L_Freq->setValue(fs, true);
            break;
        case 1:
            _uiSplite->dsb_Filter_L_Freq_LM->setValue(fs, true);
            break;
        case 3:
            _uiSplite->dsb_Filter_L_Freq_HM->setValue(fs, true);
            break;
        case 2:
            _uiSplite->dsb_Filter_R_Freq_LM->setValue(fs, true);
            break;
        case 4:
            _uiSplite->dsb_Filter_R_Freq_HM->setValue(fs, true);
            break;
        case 5:
            _uiSplite->dsb_Filter_R_Freq->setValue(fs, true);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void CfgSettingSplite::processPushButtonSlot(bool flag)
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        if (sender() == _uiSplite->pb_Input_Mute)
        {
            _cfgItem->setInputMute(flag);
        }
        else if (sender() == _uiSplite->pb_Filter_LSetSync)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setFilterSetSyncL(_curIdx, flag);
            _cfgItem->setFilterSetSyncR(CURIDX_NEXT, flag);
        }
        else if (sender() == _uiSplite->pb_Filter_RSetSync)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setFilterSetSyncL(CURIDX_PRE, flag);
            _cfgItem->setFilterSetSyncR(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Mute_L)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Mute_R)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Invert_L)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Invert_R)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        break;
    case CFG_TYPE_SPLIT_3:
        if (sender() == _uiSplite->pb_Input_Mute)
        {
            _cfgItem->setInputMute(flag);
        }
        else if (sender() == _uiSplite->pb_Filter_LSetSync)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setFilterSetSyncL(_curIdx, flag);
            _cfgItem->setFilterSetSyncL(CURIDX_NEXT, flag);
        }
        else if (sender() == _uiSplite->pb_Filter_RSetSync)
        {
            _curIdx = 2;
            _uiSplite->eqWidget->setCurrentIdx(3);
            _cfgItem->setFilterSetSyncR(CURIDX_PRE, flag);
            _cfgItem->setFilterSetSyncR(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Mute_L)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Mute_R)
        {
            _curIdx = 2;
            _uiSplite->eqWidget->setCurrentIdx(3);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Invert_L)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Invert_R)
        {
            _curIdx = 2;
            _uiSplite->eqWidget->setCurrentIdx(3);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Filter_LSetSync_M)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setFilterSetSyncL(0/*_curIdx*/, flag);
            _cfgItem->setFilterSetSyncL(1/*_curIdx*/, flag);
        }
        else if (sender() == _uiSplite->pb_Filter_RSetSync_M)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setFilterSetSyncR(1/*_curIdx*/, flag);
            _cfgItem->setFilterSetSyncR(2/*_curIdx*/, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Mute_M)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Invert_M)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        break;
    case CFG_TYPE_SPLIT_4:
        if (sender() == _uiSplite->pb_Input_Mute)
        {
            _cfgItem->setInputMute(flag);
        }
        else if (sender() == _uiSplite->pb_Filter_LSetSync)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setFilterSetSyncL(_curIdx, flag);
            _cfgItem->setFilterSetSyncL(CURIDX_NEXT, flag);
        }
        else if (sender() == _uiSplite->pb_Filter_RSetSync)
        {
            _curIdx = 3;
            _uiSplite->eqWidget->setCurrentIdx(5);
            _cfgItem->setFilterSetSyncR(CURIDX_PRE, flag);
            _cfgItem->setFilterSetSyncR(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Filter_LSetSync_LM)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(1);
            _cfgItem->setFilterSetSyncL(_curIdx, flag);
            _cfgItem->setFilterSetSyncL(CURIDX_PRE, flag);
        }
        else if (sender() == _uiSplite->pb_Filter_RSetSync_LM)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(1);
            _cfgItem->setFilterSetSyncR(_curIdx, flag);
            _cfgItem->setFilterSetSyncR(CURIDX_NEXT, flag);
        }
        else if (sender() == _uiSplite->pb_Filter_LSetSync_HM)
        {
            _curIdx = 2;
            _uiSplite->eqWidget->setCurrentIdx(3);
            _cfgItem->setFilterSetSyncL(_curIdx, flag);
            _cfgItem->setFilterSetSyncL(CURIDX_PRE, flag);
        }
        else if (sender() == _uiSplite->pb_Filter_RSetSync_HM)
        {
            _curIdx = 2;
            _uiSplite->eqWidget->setCurrentIdx(3);
            _cfgItem->setFilterSetSyncR(_curIdx, flag);
            _cfgItem->setFilterSetSyncR(CURIDX_NEXT, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Mute_L)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Mute_R)
        {
            _curIdx = 3;
            _uiSplite->eqWidget->setCurrentIdx(5);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Mute_LM)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(1);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Mute_HM)
        {
            _curIdx = 2;
            _uiSplite->eqWidget->setCurrentIdx(3);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Invert_L)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Invert_R)
        {
            _curIdx = 3;
            _uiSplite->eqWidget->setCurrentIdx(5);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Invert_LM)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(1);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        else if (sender() == _uiSplite->pb_Output_Invert_HM)
        {
            _curIdx = 2;
            _uiSplite->eqWidget->setCurrentIdx(3);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSplite::processDoubleSpinBoxSlot(double value)
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        if (sender() == _uiSplite->dsb_Input_Level)
        {
            _cfgItem->setInputLevel(value);
        }
        else if (sender() == _uiSplite->dsb_Filter_L_Freq)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            if (_uiSplite->pb_Filter_LSetSync->isChecked())
            {
                _uiSplite->dsb_Filter_R_Freq->setValue(value);

                _uiSplite->eqWidget->fsChanged(_curIdx, value);
//                _cfgItem->setFilterFreqL(_curIdx, value);

                _uiSplite->eqWidget->fsChanged(CURIDX_NEXT, value);
//                _cfgItem->setFilterFreqR(CURIDX_NEXT, value);
                 _cfgItem->setFilterFreqLR(_curIdx, CURIDX_NEXT, value);
            }
            else
            {
                _uiSplite->eqWidget->fsChanged(_curIdx, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }

        }
        else if (sender() == _uiSplite->dsb_Filter_R_Freq)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            if (_uiSplite->pb_Filter_RSetSync->isChecked())
            {
                _uiSplite->dsb_Filter_L_Freq->setValue(value);

                _uiSplite->eqWidget->fsChanged(_curIdx, value);
//                _cfgItem->setFilterFreqR(_curIdx, value);

                _uiSplite->eqWidget->fsChanged(CURIDX_PRE, value);
//                _cfgItem->setFilterFreqL(CURIDX_PRE, value);
                _cfgItem->setFilterFreqLR(CURIDX_PRE, _curIdx, value);
//                _cfgItem->setFilterFreqLR(CURIDX_PRE, Right, value);
            }
            else
            {
                _uiSplite->eqWidget->fsChanged(_curIdx, value);
                _cfgItem->setFilterFreqR(_curIdx, value);
            }
        }
        else if (sender() == _uiSplite->dsb_Output_Level_L)
        {
            _curIdx = 0;
            _cfgItem->setOutputLevel(0, value);
        }
        else if (sender() == _uiSplite->dsb_Output_Level_R)
        {
            _curIdx = 1;
            _cfgItem->setOutputLevel(1, value);
        }
        break;
    case CFG_TYPE_SPLIT_3:
        if (sender() == _uiSplite->dsb_Input_Level)
        {
            _cfgItem->setInputLevel(value);
        }
        else if (sender() == _uiSplite->dsb_Filter_L_Freq)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            if (_uiSplite->pb_Filter_LSetSync->isChecked())
            {
                _uiSplite->dsb_Filter_L_Freq_M->setValue(value);

                _uiSplite->eqWidget->fsChanged(_curIdx, value);

                _uiSplite->eqWidget->fsChanged(CURIDX_NEXT, value);

                _cfgItem->setFilterFreqLL(_curIdx, CURIDX_NEXT, value);
            }
            else
            {
                _uiSplite->eqWidget->fsChanged(_curIdx, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }

        }
        else if (sender() == _uiSplite->dsb_Filter_R_Freq)
        {
            _curIdx = 2;
            _uiSplite->eqWidget->setCurrentIdx(3);
            if (_uiSplite->pb_Filter_RSetSync->isChecked())
            {
                _uiSplite->dsb_Filter_R_Freq_M->setValue(value);

                _uiSplite->eqWidget->fsChanged(3, value);

                _uiSplite->eqWidget->fsChanged(2, value);

                _cfgItem->setFilterFreqRR(CURIDX_PRE, _curIdx, value);
            }
            else
            {
                _uiSplite->eqWidget->fsChanged(3, value);
                _cfgItem->setFilterFreqR(_curIdx, value);
            }
        }
        else if (sender() == _uiSplite->dsb_Output_Level_L)
        {
            _curIdx = 0;
            _cfgItem->setOutputLevel(0, value);
        }
        else if (sender() == _uiSplite->dsb_Output_Level_R)
        {
            _curIdx = 2;
            _cfgItem->setOutputLevel(2, value);
        }
        else if (sender() == _uiSplite->dsb_Filter_L_Freq_M)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(1);
            if (_uiSplite->pb_Filter_LSetSync_M->isChecked())
            {
                _uiSplite->dsb_Filter_L_Freq->setValue(value);
                _uiSplite->eqWidget->fsChanged(0, value);
                _uiSplite->eqWidget->fsChanged(1, value);
                _cfgItem->setFilterFreqLL(CURIDX_PRE, _curIdx, value);
            }
            else
            {
                _uiSplite->eqWidget->fsChanged(1, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }

        }
        else if (sender() == _uiSplite->dsb_Filter_R_Freq_M)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(1);
            if (_uiSplite->pb_Filter_RSetSync_M->isChecked())
            {
                _uiSplite->dsb_Filter_R_Freq->setValue(value);
                _uiSplite->eqWidget->fsChanged(2, value);
                _uiSplite->eqWidget->fsChanged(3, value);
                _cfgItem->setFilterFreqRR(_curIdx, CURIDX_NEXT, value);

            }
            else
            {
                _uiSplite->eqWidget->fsChanged(2, value);
                _cfgItem->setFilterFreqR(_curIdx, value);

            }
        }
        else if (sender() == _uiSplite->dsb_Output_Level_M)
        {
            _curIdx = 1;
            _cfgItem->setOutputLevel(_curIdx, value);
        }
        break;
    case CFG_TYPE_SPLIT_4:
        if (sender() == _uiSplite->dsb_Input_Level)
        {
            _cfgItem->setInputLevel(value);
        }
        else if (sender() == _uiSplite->dsb_Filter_L_Freq)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            if (_uiSplite->pb_Filter_LSetSync->isChecked())
            {
                _uiSplite->dsb_Filter_L_Freq_LM->setValue(value);

                _uiSplite->eqWidget->fsChanged(_curIdx, value);

                _uiSplite->eqWidget->fsChanged(CURIDX_NEXT, value);

                _cfgItem->setFilterFreqLL(_curIdx, CURIDX_NEXT, value);
            }
            else
            {
                _uiSplite->eqWidget->fsChanged(_curIdx, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }

        }
        if(sender() == _uiSplite->dsb_Filter_L_Freq_LM)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            if (_uiSplite->pb_Filter_LSetSync_LM->isChecked())
            {
                _uiSplite->dsb_Filter_L_Freq->setValue(value);

                _uiSplite->eqWidget->fsChanged(_curIdx, value);

                _uiSplite->eqWidget->fsChanged(CURIDX_PRE, value);

                _cfgItem->setFilterFreqLL(CURIDX_PRE, _curIdx, value);
            }
            else
            {
                _uiSplite->eqWidget->fsChanged(_curIdx, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }
        }
        if(sender() == _uiSplite->dsb_Filter_R_Freq_LM)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            if (_uiSplite->pb_Filter_RSetSync_LM->isChecked())
            {
                _uiSplite->dsb_Filter_L_Freq_HM->setValue(value);

                _uiSplite->eqWidget->fsChanged(2, value);

                _uiSplite->eqWidget->fsChanged(3, value);

                _cfgItem->setFilterFreqLR(CURIDX_NEXT, _curIdx, value);
            }
            else
            {
                _uiSplite->eqWidget->fsChanged(2, value);
                _cfgItem->setFilterFreqR(_curIdx, value);
            }
        }
        if(sender() == _uiSplite->dsb_Filter_L_Freq_HM)
        {
            _curIdx = 2;
            _uiSplite->eqWidget->setCurrentIdx(3);
            if (_uiSplite->pb_Filter_LSetSync_HM->isChecked())
            {
                _uiSplite->dsb_Filter_R_Freq_LM->setValue(value);

                _uiSplite->eqWidget->fsChanged(2, value);

                _uiSplite->eqWidget->fsChanged(3, value);

                _cfgItem->setFilterFreqLR(_curIdx, CURIDX_PRE, value);
            }
            else
            {
                _uiSplite->eqWidget->fsChanged(3, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }
        }
        if(sender() == _uiSplite->dsb_Filter_R_Freq_HM)
        {
            _curIdx = 2;
            _uiSplite->eqWidget->setCurrentIdx(3);
            if (_uiSplite->pb_Filter_RSetSync_HM->isChecked())
            {
                _uiSplite->dsb_Filter_R_Freq_LM->setValue(value);

                _uiSplite->eqWidget->fsChanged(4, value);

                _uiSplite->eqWidget->fsChanged(5, value);

                _cfgItem->setFilterFreqRR(CURIDX_NEXT, _curIdx, value);
            }
            else
            {
                _uiSplite->eqWidget->fsChanged(4, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }
        }
        else if (sender() == _uiSplite->dsb_Filter_R_Freq)
        {
            _curIdx = 3;
            _uiSplite->eqWidget->setCurrentIdx(5);
            if (_uiSplite->pb_Filter_RSetSync->isChecked())
            {
                _uiSplite->dsb_Filter_R_Freq_HM->setValue(value);

                _uiSplite->eqWidget->fsChanged(4, value);

                _uiSplite->eqWidget->fsChanged(5, value);

                _cfgItem->setFilterFreqRR(CURIDX_PRE, _curIdx, value);
            }
            else
            {
                _uiSplite->eqWidget->fsChanged(5, value);
                _cfgItem->setFilterFreqR(_curIdx, value);
            }
        }
        else if (sender() == _uiSplite->dsb_Output_Level_L)
        {
            _curIdx = 0;
            _cfgItem->setOutputLevel(0, value);
        }
        else if (sender() == _uiSplite->dsb_Output_Level_LM)
        {
            _curIdx = 1;
            _cfgItem->setOutputLevel(1, value);
        }
        else if (sender() == _uiSplite->dsb_Output_Level_HM)
        {
            _curIdx = 2;
            _cfgItem->setOutputLevel(2, value);
        }
        else if (sender() == _uiSplite->dsb_Output_Level_R)
        {
            _curIdx = 3;
            _cfgItem->setOutputLevel(3, value);
        }

        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSplite::processComboBoxSlot(int idx)
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        if (sender() == _uiSplite->cb_Filter_L)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            if (_uiSplite->pb_Filter_LSetSync->isChecked())
            {
                disconnect(_uiSplite->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->cb_Filter_R->setCurrentIndex(idx);
                connect(_uiSplite->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->eqWidget->filterTypeChanged(_curIdx, idx);
                _uiSplite->eqWidget->filterTypeChanged(CURIDX_NEXT, idx);
                _cfgItem->setFilterFilterTypeLR(_curIdx, CURIDX_NEXT, idx);
            }
            else
            {
                _uiSplite->eqWidget->filterTypeChanged(_curIdx, idx);
                _cfgItem->setFilterFilterTypeL(_curIdx, idx);
            }
        }
        else if (sender() == _uiSplite->cb_Filter_R)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            if (_uiSplite->pb_Filter_RSetSync->isChecked())
            {
                disconnect(_uiSplite->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->cb_Filter_L->setCurrentIndex(idx);
                connect(_uiSplite->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->eqWidget->filterTypeChanged(1, idx);

                _uiSplite->eqWidget->filterTypeChanged(0, idx);
                _cfgItem->setFilterFilterTypeLR(CURIDX_PRE, _curIdx, idx);
            }
            else
            {
                _uiSplite->eqWidget->filterTypeChanged(1, idx);
                _cfgItem->setFilterFilterTypeR(_curIdx, idx);
            }
        }
        break;
    case CFG_TYPE_SPLIT_3:
        if (sender() == _uiSplite->cb_Filter_L)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            if (_uiSplite->pb_Filter_LSetSync->isChecked())
            {
                disconnect(_uiSplite->cb_Filter_L_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->cb_Filter_L_M->setCurrentIndex(idx);
                connect(_uiSplite->cb_Filter_L_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->eqWidget->filterTypeChanged(_curIdx, idx);
                _uiSplite->eqWidget->filterTypeChanged(CURIDX_NEXT, idx);
                _cfgItem->setFilterFilterTypeLL(_curIdx, CURIDX_NEXT, idx);
            }
            else
            {
                _uiSplite->eqWidget->filterTypeChanged(_curIdx, idx);
                _cfgItem->setFilterFilterTypeL(_curIdx, idx);
            }
        }
        else if (sender() == _uiSplite->cb_Filter_R)
        {
            _curIdx = 2;
            _uiSplite->eqWidget->setCurrentIdx(3);
            if (_uiSplite->pb_Filter_RSetSync->isChecked())
            {
                disconnect(_uiSplite->cb_Filter_R_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->cb_Filter_R_M->setCurrentIndex(idx);
                connect(_uiSplite->cb_Filter_R_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));

                _uiSplite->eqWidget->filterTypeChanged(2, idx);
                _uiSplite->eqWidget->filterTypeChanged(3, idx);
                _cfgItem->setFilterFilterTypeRR(CURIDX_PRE, _curIdx, idx);
            }
            else
            {
                _uiSplite->eqWidget->filterTypeChanged(3, idx);
                _cfgItem->setFilterFilterTypeR(_curIdx, idx);
            }
        }


        if (sender() == _uiSplite->cb_Filter_L_M)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(1);
            if (_uiSplite->pb_Filter_LSetSync_M->isChecked())
            {
                disconnect(_uiSplite->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->cb_Filter_L->setCurrentIndex(idx);
                connect(_uiSplite->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->eqWidget->filterTypeChanged(0, idx);
                _uiSplite->eqWidget->filterTypeChanged(1, idx);
                _cfgItem->setFilterFilterTypeLL(_curIdx, CURIDX_NEXT, idx);

            }
            else
            {
                _uiSplite->eqWidget->filterTypeChanged(_curIdx, idx);
                _cfgItem->setFilterFilterTypeL(_curIdx, idx);
            }
        }
        else if (sender() == _uiSplite->cb_Filter_R_M)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(1);
            if (_uiSplite->pb_Filter_RSetSync_M->isChecked())
            {
                disconnect(_uiSplite->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->cb_Filter_R->setCurrentIndex(idx);
                connect(_uiSplite->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->eqWidget->filterTypeChanged(2, idx);
                _uiSplite->eqWidget->filterTypeChanged(3, idx);
                _cfgItem->setFilterFilterTypeRR(_curIdx, CURIDX_NEXT, idx);
            }else
            {
                _uiSplite->eqWidget->filterTypeChanged(2, idx);
                _cfgItem->setFilterFilterTypeR(_curIdx, idx);
            }
        }
        break;
    case CFG_TYPE_SPLIT_4:
        if (sender() == _uiSplite->cb_Filter_L)
        {
            _curIdx = 0;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            if (_uiSplite->pb_Filter_LSetSync->isChecked())
            {
                disconnect(_uiSplite->cb_Filter_L_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->cb_Filter_L_LM->setCurrentIndex(idx);
                connect(_uiSplite->cb_Filter_L_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->eqWidget->filterTypeChanged(_curIdx, idx);
                _uiSplite->eqWidget->filterTypeChanged(CURIDX_NEXT, idx);
                _cfgItem->setFilterFilterTypeLL(_curIdx, CURIDX_NEXT, idx);
            }
            else
            {
                _uiSplite->eqWidget->filterTypeChanged(_curIdx, idx);
                _cfgItem->setFilterFilterTypeL(_curIdx, idx);
            }
        }
        if (sender() == _uiSplite->cb_Filter_L_LM)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            if (_uiSplite->pb_Filter_LSetSync_LM->isChecked())
            {
                disconnect(_uiSplite->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->cb_Filter_L->setCurrentIndex(idx);
                connect(_uiSplite->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->eqWidget->filterTypeChanged(CURIDX_PRE, idx);
                _uiSplite->eqWidget->filterTypeChanged(_curIdx, idx);
                _cfgItem->setFilterFilterTypeLL(CURIDX_PRE, _curIdx, idx);
            }
            else
            {
                _uiSplite->eqWidget->filterTypeChanged(_curIdx, idx);
                _cfgItem->setFilterFilterTypeL(_curIdx, idx);
            }
        }
        if (sender() == _uiSplite->cb_Filter_R_LM)
        {
            _curIdx = 1;
            _uiSplite->eqWidget->setCurrentIdx(_curIdx);
            if (_uiSplite->pb_Filter_RSetSync_LM->isChecked())
            {
                disconnect(_uiSplite->cb_Filter_L_HM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->cb_Filter_L_HM->setCurrentIndex(idx);
                connect(_uiSplite->cb_Filter_L_HM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->eqWidget->filterTypeChanged(2, idx);
                _uiSplite->eqWidget->filterTypeChanged(3, idx);
                _cfgItem->setFilterFilterTypeLR(_curIdx, CURIDX_NEXT, idx);
            }
            else
            {
                _uiSplite->eqWidget->filterTypeChanged(2, idx);
                _cfgItem->setFilterFilterTypeR(_curIdx, idx);
            }
        }
        if (sender() == _uiSplite->cb_Filter_L_HM)
        {
            _curIdx = 2;
            _uiSplite->eqWidget->setCurrentIdx(3);
            if (_uiSplite->pb_Filter_LSetSync_HM->isChecked())
            {
                disconnect(_uiSplite->cb_Filter_R_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->cb_Filter_R_LM->setCurrentIndex(idx);
                connect(_uiSplite->cb_Filter_R_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->eqWidget->filterTypeChanged(2, idx);
                _uiSplite->eqWidget->filterTypeChanged(3, idx);
                _cfgItem->setFilterFilterTypeLR(_curIdx, CURIDX_PRE, idx);
            }
            else
            {
                _uiSplite->eqWidget->filterTypeChanged(3, idx);
                _cfgItem->setFilterFilterTypeL(_curIdx, idx);
            }
        }
        if (sender() == _uiSplite->cb_Filter_R_HM)
        {
            _curIdx = 2;
            _uiSplite->eqWidget->setCurrentIdx(3);
            if (_uiSplite->pb_Filter_RSetSync_HM->isChecked())
            {
                disconnect(_uiSplite->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->cb_Filter_R->setCurrentIndex(idx);
                connect(_uiSplite->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->eqWidget->filterTypeChanged(4, idx);
                _uiSplite->eqWidget->filterTypeChanged(5, idx);
                _cfgItem->setFilterFilterTypeRR(_curIdx, CURIDX_NEXT, idx);
            }
            else
            {
                _uiSplite->eqWidget->filterTypeChanged(4, idx);
                _cfgItem->setFilterFilterTypeR(_curIdx, idx);
            }
        }
        else if (sender() == _uiSplite->cb_Filter_R)
        {
            _curIdx = 3;
            _uiSplite->eqWidget->setCurrentIdx(5);
            if (_uiSplite->pb_Filter_RSetSync->isChecked())
            {
                disconnect(_uiSplite->cb_Filter_R_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->cb_Filter_R_HM->setCurrentIndex(idx);
                disconnect(_uiSplite->cb_Filter_R_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
                _uiSplite->eqWidget->filterTypeChanged(4, idx);
                _uiSplite->eqWidget->filterTypeChanged(5, idx);
                _cfgItem->setFilterFilterTypeRR(CURIDX_PRE, _curIdx, idx);
            }
            else
            {
                _uiSplite->eqWidget->filterTypeChanged(5, idx);
                _cfgItem->setFilterFilterTypeR(_curIdx, idx);
            }
        }
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSplite::initUi()
{
    _uiSplite = new Ui::CfgSettingSplite;
    _uiSplite->setupUi(this);

    _uiSplite->dsb_Input_Level->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    _uiSplite->dsb_Input_Level->setDecimalsNum(1);
    //L & H
    _uiSplite->dsb_Output_Level_L->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    _uiSplite->dsb_Output_Level_L->setDecimalsNum(1);

    _uiSplite->dsb_Output_Level_R->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    _uiSplite->dsb_Output_Level_R->setDecimalsNum(1);

    _uiSplite->dsb_Filter_L_Freq->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _uiSplite->dsb_Filter_L_Freq->setDecimalsNum(1);

    _uiSplite->dsb_Filter_R_Freq->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _uiSplite->dsb_Filter_R_Freq->setDecimalsNum(1);
    //M
    _uiSplite->dsb_Output_Level_M->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    _uiSplite->dsb_Output_Level_M->setDecimalsNum(1);

    _uiSplite->dsb_Filter_L_Freq_M->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _uiSplite->dsb_Filter_L_Freq_M->setDecimalsNum(1);

    _uiSplite->dsb_Filter_R_Freq_M->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _uiSplite->dsb_Filter_R_Freq_M->setDecimalsNum(1);

    //LM
    _uiSplite->dsb_Output_Level_LM->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    _uiSplite->dsb_Output_Level_LM->setDecimalsNum(1);

    _uiSplite->dsb_Filter_L_Freq_LM->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _uiSplite->dsb_Filter_L_Freq_LM->setDecimalsNum(1);

    _uiSplite->dsb_Filter_R_Freq_LM->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _uiSplite->dsb_Filter_R_Freq_LM->setDecimalsNum(1);

    //HM
    _uiSplite->dsb_Output_Level_HM->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    _uiSplite->dsb_Output_Level_HM->setDecimalsNum(1);

    _uiSplite->dsb_Filter_L_Freq_HM->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _uiSplite->dsb_Filter_L_Freq_HM->setDecimalsNum(1);

    _uiSplite->dsb_Filter_R_Freq_HM->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _uiSplite->dsb_Filter_R_Freq_HM->setDecimalsNum(1);

    _uiSplite->pb_Input_Mute->setProperty("form", "Mute");
    _uiSplite->pb_Output_Mute_L->setProperty("form", "Mute");
    _uiSplite->pb_Output_Mute_LM->setProperty("form", "Mute");
    _uiSplite->pb_Output_Mute_HM->setProperty("form", "Mute");
    _uiSplite->pb_Output_Mute_M->setProperty("form", "Mute");
    _uiSplite->pb_Output_Mute_R->setProperty("form", "Mute");

    switch(_cfgItem->getCfgType()) {
    case CFG_TYPE_SPLIT_2:
        _uiSplite->groupBox_M->setHidden(true);
        _uiSplite->groupBox_LM->setHidden(true);
        _uiSplite->groupBox_HM->setHidden(true);
        break;
    case CFG_TYPE_SPLIT_3:
        _uiSplite->groupBox_LM->setHidden(true);
        _uiSplite->groupBox_HM->setHidden(true);
        break;
    case CFG_TYPE_SPLIT_4:
        _uiSplite->groupBox_M->setHidden(true);
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSplite::initData()
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
    {
        //input
        _uiSplite->pb_Input_Mute->setChecked(_cfgItem->getInputMute());
        _uiSplite->dsb_Input_Level->setValue(_cfgItem->getInputLevel());

        initCurrentDisplay();

        crossOverPoint tmp[2] = {
            {(float)_cfgItem->getFilterFreqL(0), _cfgItem->getFilterFilterTypeL(0)},
            {(float)_cfgItem->getFilterFreqR(1), _cfgItem->getFilterFilterTypeR(1)}
        };
        _uiSplite->eqWidget->initType(CrossOverType_2Way, tmp);
    }
        break;
    case CFG_TYPE_SPLIT_3:
    {
        _uiSplite->pb_Input_Mute->setChecked(_cfgItem->getInputMute());
        _uiSplite->dsb_Input_Level->setValue(_cfgItem->getInputLevel());

        initCurrentDisplay();


        crossOverPoint tmp[4] = {
            {(float)_cfgItem->getFilterFreqL(0), _cfgItem->getFilterFilterTypeL(0)},
            {(float)_cfgItem->getFilterFreqL(1), _cfgItem->getFilterFilterTypeL(1)},
            {(float)_cfgItem->getFilterFreqR(1), _cfgItem->getFilterFilterTypeR(1)},
            {(float)_cfgItem->getFilterFreqR(2), _cfgItem->getFilterFilterTypeR(2)}
        };
        _uiSplite->eqWidget->initType(CrossOverType_3Way, tmp);
    }
        break;
    case CFG_TYPE_SPLIT_4:
    {
        _uiSplite->pb_Input_Mute->setChecked(_cfgItem->getInputMute());
        _uiSplite->dsb_Input_Level->setValue(_cfgItem->getInputLevel());

        initCurrentDisplay();

        crossOverPoint tmp[6] = {
            {(float)_cfgItem->getFilterFreqL(0), _cfgItem->getFilterFilterTypeL(0)},
            {(float)_cfgItem->getFilterFreqL(1), _cfgItem->getFilterFilterTypeL(1)},
            {(float)_cfgItem->getFilterFreqR(1), _cfgItem->getFilterFilterTypeR(1)},
            {(float)_cfgItem->getFilterFreqL(2), _cfgItem->getFilterFilterTypeL(2)},
            {(float)_cfgItem->getFilterFreqR(2), _cfgItem->getFilterFilterTypeR(2)},
            {(float)_cfgItem->getFilterFreqR(3), _cfgItem->getFilterFilterTypeR(3)}
        };
        _uiSplite->eqWidget->initType(CrossOverType_4Way, tmp);
    }
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSplite::initSignalConnect()
{
    //input
    connect(_uiSplite->pb_Input_Mute, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->dsb_Input_Level, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    //eqWidget
    connect(_uiSplite->eqWidget, SIGNAL(pointChange(int)), this, SLOT(pointChange(int)));
    connect(_uiSplite->eqWidget, SIGNAL(pointMove(int,float)), this, SLOT(pointMove(int, float)));
    //L & H
    connect(_uiSplite->pb_Filter_LSetSync, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Filter_RSetSync, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Output_Mute_L, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Output_Invert_L, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Output_Mute_R, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Output_Invert_R, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));

    connect(_uiSplite->dsb_Filter_L_Freq, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    connect(_uiSplite->dsb_Filter_R_Freq, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    connect(_uiSplite->dsb_Output_Level_L, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    connect(_uiSplite->dsb_Output_Level_R, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));

    connect(_uiSplite->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
    connect(_uiSplite->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));

    // M
    connect(_uiSplite->pb_Filter_LSetSync_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Filter_RSetSync_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Output_Mute_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Output_Invert_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));

    connect(_uiSplite->dsb_Filter_L_Freq_M, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    connect(_uiSplite->dsb_Filter_R_Freq_M, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    connect(_uiSplite->dsb_Output_Level_M, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));

    connect(_uiSplite->cb_Filter_L_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
    connect(_uiSplite->cb_Filter_R_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));

    //LM
    connect(_uiSplite->pb_Filter_LSetSync_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Filter_RSetSync_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Output_Mute_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Output_Invert_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));

    connect(_uiSplite->dsb_Filter_L_Freq_LM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    connect(_uiSplite->dsb_Filter_R_Freq_LM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    connect(_uiSplite->dsb_Output_Level_LM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));

    connect(_uiSplite->cb_Filter_L_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
    connect(_uiSplite->cb_Filter_R_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));

    //HM
    connect(_uiSplite->pb_Filter_LSetSync_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Filter_RSetSync_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Output_Mute_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    connect(_uiSplite->pb_Output_Invert_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));

    connect(_uiSplite->dsb_Filter_L_Freq_HM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    connect(_uiSplite->dsb_Filter_R_Freq_HM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    connect(_uiSplite->dsb_Output_Level_HM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));

    connect(_uiSplite->cb_Filter_L_HM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
    connect(_uiSplite->cb_Filter_R_HM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));

}

void CfgSettingSplite::disableSignalConnect()
{
    //input
    disconnect(_uiSplite->pb_Input_Mute, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->dsb_Input_Level, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    //eqWidget
    disconnect(_uiSplite->eqWidget, SIGNAL(pointChange(int)), this, SLOT(pointChange(int)));
    disconnect(_uiSplite->eqWidget, SIGNAL(pointMove(int,float)), this, SLOT(pointMove(int, float)));
    //L & H
    disconnect(_uiSplite->pb_Filter_LSetSync, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Filter_RSetSync, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Output_Mute_L, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Output_Invert_L, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Output_Mute_R, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Output_Invert_R, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));

    disconnect(_uiSplite->dsb_Filter_L_Freq, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    disconnect(_uiSplite->dsb_Filter_R_Freq, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    disconnect(_uiSplite->dsb_Output_Level_L, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    disconnect(_uiSplite->dsb_Output_Level_R, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));

    disconnect(_uiSplite->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
    disconnect(_uiSplite->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));

    // M
    disconnect(_uiSplite->pb_Filter_LSetSync_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Filter_RSetSync_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Output_Mute_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Output_Invert_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));

    disconnect(_uiSplite->dsb_Filter_L_Freq_M, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    disconnect(_uiSplite->dsb_Filter_R_Freq_M, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    disconnect(_uiSplite->dsb_Output_Level_M, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));

    disconnect(_uiSplite->cb_Filter_L_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
    disconnect(_uiSplite->cb_Filter_R_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));

    //LM
    disconnect(_uiSplite->pb_Filter_LSetSync_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Filter_RSetSync_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Output_Mute_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Output_Invert_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));

    disconnect(_uiSplite->dsb_Filter_L_Freq_LM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    disconnect(_uiSplite->dsb_Filter_R_Freq_LM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    disconnect(_uiSplite->dsb_Output_Level_LM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));

    disconnect(_uiSplite->cb_Filter_L_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
    disconnect(_uiSplite->cb_Filter_R_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));

    //HM
    disconnect(_uiSplite->pb_Filter_LSetSync_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Filter_RSetSync_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Output_Mute_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
    disconnect(_uiSplite->pb_Output_Invert_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));

    disconnect(_uiSplite->dsb_Filter_L_Freq_HM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    disconnect(_uiSplite->dsb_Filter_R_Freq_HM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
    disconnect(_uiSplite->dsb_Output_Level_HM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));

    disconnect(_uiSplite->cb_Filter_L_HM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
    disconnect(_uiSplite->cb_Filter_R_HM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot(int)));
}

void CfgSettingSplite::initCurrentDisplay()
{
    disableSignalConnect();
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        _uiSplite->pb_Filter_LSetSync->setChecked(_cfgItem->getFilterSetSyncL(0));
        _uiSplite->pb_Filter_RSetSync->setChecked(_cfgItem->getFilterSetSyncR(1));
        _uiSplite->cb_Filter_L->setCurrentIndex(_cfgItem->getFilterFilterTypeL(0));
        _uiSplite->cb_Filter_R->setCurrentIndex(_cfgItem->getFilterFilterTypeR(1));
        _uiSplite->dsb_Filter_L_Freq->setValue(_cfgItem->getFilterFreqL(0));
        _uiSplite->dsb_Filter_R_Freq->setValue(_cfgItem->getFilterFreqR(1));

        _uiSplite->pb_Output_Mute_L->setChecked(_cfgItem->getOutputMute(0));
        _uiSplite->pb_Output_Invert_L->setChecked(_cfgItem->getOutputInvert(0));
        _uiSplite->dsb_Output_Level_L->setValue(_cfgItem->getOutputLevel(0));

        _uiSplite->pb_Output_Mute_R->setChecked(_cfgItem->getOutputMute(1));
        _uiSplite->pb_Output_Invert_R->setChecked(_cfgItem->getOutputInvert(1));
        _uiSplite->dsb_Output_Level_R->setValue(_cfgItem->getOutputLevel(1));
        break;
    case CFG_TYPE_SPLIT_3:////////////////////////////////////////
        _uiSplite->pb_Filter_LSetSync->setChecked(_cfgItem->getFilterSetSyncL(0));
        _uiSplite->pb_Filter_RSetSync->setChecked(_cfgItem->getFilterSetSyncR(2));
        _uiSplite->cb_Filter_L->setCurrentIndex(_cfgItem->getFilterFilterTypeL(0));
        _uiSplite->cb_Filter_R->setCurrentIndex(_cfgItem->getFilterFilterTypeR(2));
        _uiSplite->dsb_Filter_L_Freq->setValue(_cfgItem->getFilterFreqL(0));
        _uiSplite->dsb_Filter_R_Freq->setValue(_cfgItem->getFilterFreqR(2));

        _uiSplite->pb_Output_Mute_L->setChecked(_cfgItem->getOutputMute(0));
        _uiSplite->pb_Output_Invert_L->setChecked(_cfgItem->getOutputInvert(0));
        _uiSplite->dsb_Output_Level_L->setValue(_cfgItem->getOutputLevel(0));

        _uiSplite->pb_Output_Mute_R->setChecked(_cfgItem->getOutputMute(2));
        _uiSplite->pb_Output_Invert_R->setChecked(_cfgItem->getOutputInvert(2));
        _uiSplite->dsb_Output_Level_R->setValue(_cfgItem->getOutputLevel(2));

        _uiSplite->pb_Filter_LSetSync_M->setChecked(_cfgItem->getFilterSetSyncL(1));
        _uiSplite->pb_Filter_RSetSync_M->setChecked(_cfgItem->getFilterSetSyncR(1));
        _uiSplite->cb_Filter_L_M->setCurrentIndex(_cfgItem->getFilterFilterTypeL(1));
        _uiSplite->cb_Filter_R_M->setCurrentIndex(_cfgItem->getFilterFilterTypeR(1));
        _uiSplite->dsb_Filter_L_Freq_M->setValue(_cfgItem->getFilterFreqL(1));
        _uiSplite->dsb_Filter_R_Freq_M->setValue(_cfgItem->getFilterFreqR(1));

        _uiSplite->pb_Output_Mute_M->setChecked(_cfgItem->getOutputMute(1));
        _uiSplite->pb_Output_Invert_M->setChecked(_cfgItem->getOutputInvert(1));
        _uiSplite->dsb_Output_Level_M->setValue(_cfgItem->getOutputLevel(1));
        break;
    case CFG_TYPE_SPLIT_4:
        _uiSplite->pb_Filter_LSetSync->setChecked(_cfgItem->getFilterSetSyncL(0));
        _uiSplite->pb_Filter_RSetSync->setChecked(_cfgItem->getFilterSetSyncR(3));
        _uiSplite->cb_Filter_L->setCurrentIndex(_cfgItem->getFilterFilterTypeL(0));
        _uiSplite->cb_Filter_R->setCurrentIndex(_cfgItem->getFilterFilterTypeR(3));
        _uiSplite->dsb_Filter_L_Freq->setValue(_cfgItem->getFilterFreqL(0));
        _uiSplite->dsb_Filter_R_Freq->setValue(_cfgItem->getFilterFreqR(3));

        _uiSplite->pb_Output_Mute_L->setChecked(_cfgItem->getOutputMute(0));
        _uiSplite->pb_Output_Invert_L->setChecked(_cfgItem->getOutputInvert(0));
        _uiSplite->dsb_Output_Level_L->setValue(_cfgItem->getOutputLevel(0));

        _uiSplite->pb_Output_Mute_R->setChecked(_cfgItem->getOutputMute(3));
        _uiSplite->pb_Output_Invert_R->setChecked(_cfgItem->getOutputInvert(3));
        _uiSplite->dsb_Output_Level_R->setValue(_cfgItem->getOutputLevel(3));

        _uiSplite->pb_Filter_LSetSync_LM->setChecked(_cfgItem->getFilterSetSyncL(1));
        _uiSplite->pb_Filter_RSetSync_LM->setChecked(_cfgItem->getFilterSetSyncR(1));
        _uiSplite->cb_Filter_L_LM->setCurrentIndex(_cfgItem->getFilterFilterTypeL(1));
        _uiSplite->cb_Filter_R_LM->setCurrentIndex(_cfgItem->getFilterFilterTypeR(1));
        _uiSplite->dsb_Filter_L_Freq_LM->setValue(_cfgItem->getFilterFreqL(1));
        _uiSplite->dsb_Filter_R_Freq_LM->setValue(_cfgItem->getFilterFreqR(1));

        _uiSplite->pb_Output_Mute_LM->setChecked(_cfgItem->getOutputMute(1));
        _uiSplite->pb_Output_Invert_LM->setChecked(_cfgItem->getOutputInvert(1));

        _uiSplite->dsb_Output_Level_LM->setValue(_cfgItem->getOutputLevel(1));

        _uiSplite->pb_Filter_LSetSync_HM->setChecked(_cfgItem->getFilterSetSyncL(2));
        _uiSplite->pb_Filter_RSetSync_HM->setChecked(_cfgItem->getFilterSetSyncR(2));
        _uiSplite->cb_Filter_L_HM->setCurrentIndex(_cfgItem->getFilterFilterTypeL(2));
        _uiSplite->cb_Filter_R_HM->setCurrentIndex(_cfgItem->getFilterFilterTypeR(2));
        _uiSplite->dsb_Filter_L_Freq_HM->setValue(_cfgItem->getFilterFreqL(2));
        _uiSplite->dsb_Filter_R_Freq_HM->setValue(_cfgItem->getFilterFreqR(2));

        _uiSplite->pb_Output_Mute_HM->setChecked(_cfgItem->getOutputMute(2));
        _uiSplite->pb_Output_Invert_HM->setChecked(_cfgItem->getOutputInvert(2));

        _uiSplite->dsb_Output_Level_HM->setValue(_cfgItem->getOutputLevel(2));
        break;
    default:
        Q_ASSERT(false);
    }
    initSignalConnect();
}


void CfgSettingSplite::pointChange_FangTu(int idx)
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        _ui_FangTu->eqWidget->setCurrentIdx(idx); // 手动回传点击事件
        break;
    case CFG_TYPE_SPLIT_3:
        switch (idx) {
        case 0:
            _ui_FangTu->eqWidget->setCurrentIdx(idx);
            break;
        case 1:
        case 2:
            _ui_FangTu->eqWidget->setCurrentIdx(1);
            break;
        case 3:
            _ui_FangTu->eqWidget->setCurrentIdx(idx);
            break;
        default:
            break;
        }
        break;
    case CFG_TYPE_SPLIT_4:
        switch (idx) {
        case 0:
            _ui_FangTu->eqWidget->setCurrentIdx(idx);
            break;
        case 1:
        case 2:
            _ui_FangTu->eqWidget->setCurrentIdx(1);
            break;
        case 3:
        case 4:
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            break;
        case 5:
            _ui_FangTu->eqWidget->setCurrentIdx(idx);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void CfgSettingSplite::pointMove_FangTu(int idx, float fs)
{
//    qDebug() << __FUNCTION__ << __LINE__ << idx << fs;
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        switch (idx) {
        case 0:
            _ui_FangTu->dsb_Filter_L_Freq->setValue(fs, true);
            break;
        case 1:
            _ui_FangTu->dsb_Filter_R_Freq->setValue(fs, true);
            break;
        default:
            break;
        }
        break;
    case CFG_TYPE_SPLIT_3:
        switch (idx) {
        case 0:
            _ui_FangTu->dsb_Filter_L_Freq->setValue(fs, true);
            break;
        case 1:
            _ui_FangTu->dsb_Filter_L_Freq_M->setValue(fs, true);
            break;
        case 2:
            _ui_FangTu->dsb_Filter_R_Freq_M->setValue(fs, true);
            break;
        case 3:
            _ui_FangTu->dsb_Filter_R_Freq->setValue(fs, true);
            break;
        default:
            break;
        }
        break;
    case CFG_TYPE_SPLIT_4:
        switch (idx) {
        case 0:
            _ui_FangTu->dsb_Filter_L_Freq->setValue(fs, true);
            break;
        case 1:
            _ui_FangTu->dsb_Filter_L_Freq_LM->setValue(fs, true);
            break;
        case 3:
            _ui_FangTu->dsb_Filter_L_Freq_HM->setValue(fs, true);
            break;
        case 2:
            _ui_FangTu->dsb_Filter_R_Freq_LM->setValue(fs, true);
            break;
        case 4:
            _ui_FangTu->dsb_Filter_R_Freq_HM->setValue(fs, true);
            break;
        case 5:
            _ui_FangTu->dsb_Filter_R_Freq->setValue(fs, true);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void CfgSettingSplite::processPushButtonSlot_FangTu(bool flag)
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        if (sender() == _ui_FangTu->pb_Input_Mute)
        {
            _cfgItem->setInputMute(flag);
        }
        else if (sender() == _ui_FangTu->pb_Filter_LSetSync)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setFilterSetSyncL(_curIdx, flag);
            _cfgItem->setFilterSetSyncR(CURIDX_NEXT, flag);
        }
        else if (sender() == _ui_FangTu->pb_Filter_RSetSync)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setFilterSetSyncL(CURIDX_PRE, flag);
            _cfgItem->setFilterSetSyncR(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Mute_L)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Mute_R)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Invert_L)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Invert_R)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        break;
    case CFG_TYPE_SPLIT_3:
        if (sender() == _ui_FangTu->pb_Input_Mute)
        {
            _cfgItem->setInputMute(flag);
        }
        else if (sender() == _ui_FangTu->pb_Filter_LSetSync)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setFilterSetSyncL(_curIdx, flag);
            _cfgItem->setFilterSetSyncL(CURIDX_NEXT, flag);
        }
        else if (sender() == _ui_FangTu->pb_Filter_RSetSync)
        {
            _curIdx = 2;
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            _cfgItem->setFilterSetSyncR(CURIDX_PRE, flag);
            _cfgItem->setFilterSetSyncR(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Mute_L)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Mute_R)
        {
            _curIdx = 2;
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Invert_L)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Invert_R)
        {
            _curIdx = 2;
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Filter_LSetSync_M)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setFilterSetSyncL(0/*_curIdx*/, flag);
            _cfgItem->setFilterSetSyncL(1/*_curIdx*/, flag);
        }
        else if (sender() == _ui_FangTu->pb_Filter_RSetSync_M)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setFilterSetSyncR(1/*_curIdx*/, flag);
            _cfgItem->setFilterSetSyncR(2/*_curIdx*/, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Mute_M)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Invert_M)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        break;
    case CFG_TYPE_SPLIT_4:
        if (sender() == _ui_FangTu->pb_Input_Mute)
        {
            _cfgItem->setInputMute(flag);
        }
        else if (sender() == _ui_FangTu->pb_Filter_LSetSync)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setFilterSetSyncL(_curIdx, flag);
            _cfgItem->setFilterSetSyncL(CURIDX_NEXT, flag);
        }
        else if (sender() == _ui_FangTu->pb_Filter_RSetSync)
        {
            _curIdx = 3;
            _ui_FangTu->eqWidget->setCurrentIdx(5);
            _cfgItem->setFilterSetSyncR(CURIDX_PRE, flag);
            _cfgItem->setFilterSetSyncR(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Filter_LSetSync_LM)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(1);
            _cfgItem->setFilterSetSyncL(_curIdx, flag);
            _cfgItem->setFilterSetSyncL(CURIDX_PRE, flag);
        }
        else if (sender() == _ui_FangTu->pb_Filter_RSetSync_LM)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(1);
            _cfgItem->setFilterSetSyncR(_curIdx, flag);
            _cfgItem->setFilterSetSyncR(CURIDX_NEXT, flag);
        }
        else if (sender() == _ui_FangTu->pb_Filter_LSetSync_HM)
        {
            _curIdx = 2;
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            _cfgItem->setFilterSetSyncL(_curIdx, flag);
            _cfgItem->setFilterSetSyncL(CURIDX_PRE, flag);
        }
        else if (sender() == _ui_FangTu->pb_Filter_RSetSync_HM)
        {
            _curIdx = 2;
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            _cfgItem->setFilterSetSyncR(_curIdx, flag);
            _cfgItem->setFilterSetSyncR(CURIDX_NEXT, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Mute_L)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Mute_R)
        {
            _curIdx = 3;
            _ui_FangTu->eqWidget->setCurrentIdx(5);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Mute_LM)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(1);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Mute_HM)
        {
            _curIdx = 2;
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            _cfgItem->setOutputMute(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Invert_L)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Invert_R)
        {
            _curIdx = 3;
            _ui_FangTu->eqWidget->setCurrentIdx(5);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Invert_LM)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(1);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        else if (sender() == _ui_FangTu->pb_Output_Invert_HM)
        {
            _curIdx = 2;
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            _cfgItem->setOutputInvert(_curIdx, flag);
        }
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSplite::processDoubleSpinBoxSlot_FangTu(double value)
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        if (sender() == _ui_FangTu->dsb_Input_Level)
        {
            _cfgItem->setInputLevel(value);
        }
        else if (sender() == _ui_FangTu->dsb_Filter_L_Freq)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            if (_ui_FangTu->pb_Filter_LSetSync->isChecked())
            {
                _ui_FangTu->dsb_Filter_R_Freq->setValue(value);

                _ui_FangTu->eqWidget->fsChanged(_curIdx, value);
//                _cfgItem->setFilterFreqL(_curIdx, value);

                _ui_FangTu->eqWidget->fsChanged(CURIDX_NEXT, value);
//                _cfgItem->setFilterFreqR(CURIDX_NEXT, value);
                 _cfgItem->setFilterFreqLR(_curIdx, CURIDX_NEXT, value);
            }
            else
            {
                _ui_FangTu->eqWidget->fsChanged(_curIdx, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }

        }
        else if (sender() == _ui_FangTu->dsb_Filter_R_Freq)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            if (_ui_FangTu->pb_Filter_RSetSync->isChecked())
            {
                _ui_FangTu->dsb_Filter_L_Freq->setValue(value);

                _ui_FangTu->eqWidget->fsChanged(_curIdx, value);
//                _cfgItem->setFilterFreqR(_curIdx, value);

                _ui_FangTu->eqWidget->fsChanged(CURIDX_PRE, value);
//                _cfgItem->setFilterFreqL(CURIDX_PRE, value);
                _cfgItem->setFilterFreqLR(CURIDX_PRE, _curIdx, value);
//                _cfgItem->setFilterFreqLR(CURIDX_PRE, Right, value);
            }
            else
            {
                _ui_FangTu->eqWidget->fsChanged(_curIdx, value);
                _cfgItem->setFilterFreqR(_curIdx, value);
            }
        }
        else if (sender() == _ui_FangTu->dsb_Output_Level_L)
        {
            _curIdx = 0;
            _cfgItem->setOutputLevel(0, value);
        }
        else if (sender() == _ui_FangTu->dsb_Output_Level_R)
        {
            _curIdx = 1;
            _cfgItem->setOutputLevel(1, value);
        }
        break;
    case CFG_TYPE_SPLIT_3:
        if (sender() == _ui_FangTu->dsb_Input_Level)
        {
            _cfgItem->setInputLevel(value);
        }
        else if (sender() == _ui_FangTu->dsb_Filter_L_Freq)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            if (_ui_FangTu->pb_Filter_LSetSync->isChecked())
            {
                _ui_FangTu->dsb_Filter_L_Freq_M->setValue(value);

                _ui_FangTu->eqWidget->fsChanged(_curIdx, value);

                _ui_FangTu->eqWidget->fsChanged(CURIDX_NEXT, value);

                _cfgItem->setFilterFreqLL(_curIdx, CURIDX_NEXT, value);
            }
            else
            {
                _ui_FangTu->eqWidget->fsChanged(_curIdx, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }

        }
        else if (sender() == _ui_FangTu->dsb_Filter_R_Freq)
        {
            _curIdx = 2;
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            if (_ui_FangTu->pb_Filter_RSetSync->isChecked())
            {
                _ui_FangTu->dsb_Filter_R_Freq_M->setValue(value);

                _ui_FangTu->eqWidget->fsChanged(3, value);

                _ui_FangTu->eqWidget->fsChanged(2, value);

                _cfgItem->setFilterFreqRR(CURIDX_PRE, _curIdx, value);
            }
            else
            {
                _ui_FangTu->eqWidget->fsChanged(3, value);
                _cfgItem->setFilterFreqR(_curIdx, value);
            }
        }
        else if (sender() == _ui_FangTu->dsb_Output_Level_L)
        {
            _curIdx = 0;
            _cfgItem->setOutputLevel(0, value);
        }
        else if (sender() == _ui_FangTu->dsb_Output_Level_R)
        {
            _curIdx = 2;
            _cfgItem->setOutputLevel(2, value);
        }
        else if (sender() == _ui_FangTu->dsb_Filter_L_Freq_M)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(1);
            if (_ui_FangTu->pb_Filter_LSetSync_M->isChecked())
            {
                _ui_FangTu->dsb_Filter_L_Freq->setValue(value);
                _ui_FangTu->eqWidget->fsChanged(0, value);
                _ui_FangTu->eqWidget->fsChanged(1, value);
                _cfgItem->setFilterFreqLL(CURIDX_PRE, _curIdx, value);
            }
            else
            {
                _ui_FangTu->eqWidget->fsChanged(1, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }

        }
        else if (sender() == _ui_FangTu->dsb_Filter_R_Freq_M)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(1);
            if (_ui_FangTu->pb_Filter_RSetSync_M->isChecked())
            {
                _ui_FangTu->dsb_Filter_R_Freq->setValue(value);
                _ui_FangTu->eqWidget->fsChanged(2, value);
                _ui_FangTu->eqWidget->fsChanged(3, value);
                _cfgItem->setFilterFreqRR(_curIdx, CURIDX_NEXT, value);

            }
            else
            {
                _ui_FangTu->eqWidget->fsChanged(2, value);
                _cfgItem->setFilterFreqR(_curIdx, value);

            }
        }
        else if (sender() == _ui_FangTu->dsb_Output_Level_M)
        {
            _curIdx = 1;
            _cfgItem->setOutputLevel(_curIdx, value);
        }
        break;
    case CFG_TYPE_SPLIT_4:
        if (sender() == _ui_FangTu->dsb_Input_Level)
        {
            _cfgItem->setInputLevel(value);
        }
        else if (sender() == _ui_FangTu->dsb_Filter_L_Freq)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            if (_ui_FangTu->pb_Filter_LSetSync->isChecked())
            {
                _ui_FangTu->dsb_Filter_L_Freq_LM->setValue(value);

                _ui_FangTu->eqWidget->fsChanged(_curIdx, value);

                _ui_FangTu->eqWidget->fsChanged(CURIDX_NEXT, value);

                _cfgItem->setFilterFreqLL(_curIdx, CURIDX_NEXT, value);
            }
            else
            {
                _ui_FangTu->eqWidget->fsChanged(_curIdx, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }

        }
        if(sender() == _ui_FangTu->dsb_Filter_L_Freq_LM)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            if (_ui_FangTu->pb_Filter_LSetSync_LM->isChecked())
            {
                _ui_FangTu->dsb_Filter_L_Freq->setValue(value);

                _ui_FangTu->eqWidget->fsChanged(_curIdx, value);

                _ui_FangTu->eqWidget->fsChanged(CURIDX_PRE, value);

                _cfgItem->setFilterFreqLL(CURIDX_PRE, _curIdx, value);
            }
            else
            {
                _ui_FangTu->eqWidget->fsChanged(_curIdx, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }
        }
        if(sender() == _ui_FangTu->dsb_Filter_R_Freq_LM)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            if (_ui_FangTu->pb_Filter_RSetSync_LM->isChecked())
            {
                _ui_FangTu->dsb_Filter_L_Freq_HM->setValue(value);

                _ui_FangTu->eqWidget->fsChanged(2, value);

                _ui_FangTu->eqWidget->fsChanged(3, value);

                _cfgItem->setFilterFreqLR(CURIDX_NEXT, _curIdx, value);
            }
            else
            {
                _ui_FangTu->eqWidget->fsChanged(2, value);
                _cfgItem->setFilterFreqR(_curIdx, value);
            }
        }
        if(sender() == _ui_FangTu->dsb_Filter_L_Freq_HM)
        {
            _curIdx = 2;
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            if (_ui_FangTu->pb_Filter_LSetSync_HM->isChecked())
            {
                _ui_FangTu->dsb_Filter_R_Freq_LM->setValue(value);

                _ui_FangTu->eqWidget->fsChanged(2, value);

                _ui_FangTu->eqWidget->fsChanged(3, value);

                _cfgItem->setFilterFreqLR(_curIdx, CURIDX_PRE, value);
            }
            else
            {
                _ui_FangTu->eqWidget->fsChanged(3, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }
        }
        if(sender() == _ui_FangTu->dsb_Filter_R_Freq_HM)
        {
            _curIdx = 2;
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            if (_ui_FangTu->pb_Filter_RSetSync_HM->isChecked())
            {
                _ui_FangTu->dsb_Filter_R_Freq_LM->setValue(value);

                _ui_FangTu->eqWidget->fsChanged(4, value);

                _ui_FangTu->eqWidget->fsChanged(5, value);

                _cfgItem->setFilterFreqRR(CURIDX_NEXT, _curIdx, value);
            }
            else
            {
                _ui_FangTu->eqWidget->fsChanged(4, value);
                _cfgItem->setFilterFreqL(_curIdx, value);
            }
        }
        else if (sender() == _ui_FangTu->dsb_Filter_R_Freq)
        {
            _curIdx = 3;
            _ui_FangTu->eqWidget->setCurrentIdx(5);
            if (_ui_FangTu->pb_Filter_RSetSync->isChecked())
            {
                _ui_FangTu->dsb_Filter_R_Freq_HM->setValue(value);

                _ui_FangTu->eqWidget->fsChanged(4, value);

                _ui_FangTu->eqWidget->fsChanged(5, value);

                _cfgItem->setFilterFreqRR(CURIDX_PRE, _curIdx, value);
            }
            else
            {
                _ui_FangTu->eqWidget->fsChanged(5, value);
                _cfgItem->setFilterFreqR(_curIdx, value);
            }
        }
        else if (sender() == _ui_FangTu->dsb_Output_Level_L)
        {
            _curIdx = 0;
            _cfgItem->setOutputLevel(0, value);
        }
        else if (sender() == _ui_FangTu->dsb_Output_Level_LM)
        {
            _curIdx = 1;
            _cfgItem->setOutputLevel(1, value);
        }
        else if (sender() == _ui_FangTu->dsb_Output_Level_HM)
        {
            _curIdx = 2;
            _cfgItem->setOutputLevel(2, value);
        }
        else if (sender() == _ui_FangTu->dsb_Output_Level_R)
        {
            _curIdx = 3;
            _cfgItem->setOutputLevel(3, value);
        }



        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSplite::processComboBoxSlot_FangTu(int idx)
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        if (sender() == _ui_FangTu->cb_Filter_L)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            if (_ui_FangTu->pb_Filter_LSetSync->isChecked())
            {
                disconnect(_ui_FangTu->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->cb_Filter_R->setCurrentIndex(idx);
                connect(_ui_FangTu->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->eqWidget->filterTypeChanged(_curIdx, idx);
//                _cfgItem->setFilterFilterTypeL(_curIdx, idx);

                _ui_FangTu->eqWidget->filterTypeChanged(CURIDX_NEXT, idx);
//                _cfgItem->setFilterFilterTypeR(CURIDX_NEXT, idx);
                _cfgItem->setFilterFilterTypeLR(_curIdx, CURIDX_NEXT, idx);
//                _cfgItem->setFilterFilterTypeLR(Left, CURIDX_NEXT, idx);
            }
            else
            {
                _ui_FangTu->eqWidget->filterTypeChanged(_curIdx, idx);
                _cfgItem->setFilterFilterTypeL(_curIdx, idx);
            }
        }
        else if (sender() == _ui_FangTu->cb_Filter_R)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            if (_ui_FangTu->pb_Filter_RSetSync->isChecked())
            {
                disconnect(_ui_FangTu->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->cb_Filter_L->setCurrentIndex(idx);
                connect(_ui_FangTu->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->eqWidget->filterTypeChanged(1, idx);
//                _cfgItem->setFilterFilterTypeR(_curIdx, idx);

                _ui_FangTu->eqWidget->filterTypeChanged(0, idx);
//                _cfgItem->setFilterFilterTypeL(CURIDX_PRE, idx);
                _cfgItem->setFilterFilterTypeLR(CURIDX_PRE, _curIdx, idx);
//                _cfgItem->setFilterFilterTypeLR(CURIDX_PRE, Right, idx);
            }
            else
            {
                _ui_FangTu->eqWidget->filterTypeChanged(1, idx);
                _cfgItem->setFilterFilterTypeR(_curIdx, idx);
            }
        }
        break;
    case CFG_TYPE_SPLIT_3:
        if (sender() == _ui_FangTu->cb_Filter_L)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            if (_ui_FangTu->pb_Filter_LSetSync->isChecked())
            {
                disconnect(_ui_FangTu->cb_Filter_L_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->cb_Filter_L_M->setCurrentIndex(idx);
                connect(_ui_FangTu->cb_Filter_L_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->eqWidget->filterTypeChanged(_curIdx, idx);
//                _cfgItem->setFilterFilterTypeL(_curIdx, idx);
                _ui_FangTu->eqWidget->filterTypeChanged(CURIDX_NEXT, idx);
//                _cfgItem->setFilterFilterTypeR(CURIDX_NEXT, idx);
                _cfgItem->setFilterFilterTypeLL(_curIdx, CURIDX_NEXT, idx);
//                _cfgItem->setFilterFilterTypeLR(Left, CURIDX_NEXT, idx);
            }
            else
            {
                _ui_FangTu->eqWidget->filterTypeChanged(_curIdx, idx);
                _cfgItem->setFilterFilterTypeL(_curIdx, idx);
            }
        }
        else if (sender() == _ui_FangTu->cb_Filter_R)
        {
            _curIdx = 2;
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            if (_ui_FangTu->pb_Filter_RSetSync->isChecked())
            {
                disconnect(_ui_FangTu->cb_Filter_R_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->cb_Filter_R_M->setCurrentIndex(idx);
                connect(_ui_FangTu->cb_Filter_R_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));

                _ui_FangTu->eqWidget->filterTypeChanged(2, idx);
//                _cfgItem->setFilterFilterTypeR(_curIdx, idx);

                _ui_FangTu->eqWidget->filterTypeChanged(3, idx);
//                _cfgItem->setFilterFilterTypeL(CURIDX_PRE, idx);
                _cfgItem->setFilterFilterTypeRR(CURIDX_PRE, _curIdx, idx);
//                _cfgItem->setFilterFilterTypeLR(CURIDX_PRE, Right, idx);
            }
            else
            {
                _ui_FangTu->eqWidget->filterTypeChanged(3, idx);
                _cfgItem->setFilterFilterTypeR(_curIdx, idx);
            }
        }


        if (sender() == _ui_FangTu->cb_Filter_L_M)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(1);
            if (_ui_FangTu->pb_Filter_LSetSync_M->isChecked())
            {
                disconnect(_ui_FangTu->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->cb_Filter_L->setCurrentIndex(idx);
                connect(_ui_FangTu->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->eqWidget->filterTypeChanged(0, idx);
                _ui_FangTu->eqWidget->filterTypeChanged(1, idx);
                _cfgItem->setFilterFilterTypeLL(_curIdx, CURIDX_NEXT, idx);

            }
            else
            {
                _ui_FangTu->eqWidget->filterTypeChanged(_curIdx, idx);
                _cfgItem->setFilterFilterTypeL(_curIdx, idx);
            }
        }
        else if (sender() == _ui_FangTu->cb_Filter_R_M)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(1);
            if (_ui_FangTu->pb_Filter_RSetSync_M->isChecked())
            {
                disconnect(_ui_FangTu->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->cb_Filter_R->setCurrentIndex(idx);
                connect(_ui_FangTu->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->eqWidget->filterTypeChanged(2, idx);
                _ui_FangTu->eqWidget->filterTypeChanged(3, idx);
                _cfgItem->setFilterFilterTypeRR(_curIdx, CURIDX_NEXT, idx);
            }else
            {
                _ui_FangTu->eqWidget->filterTypeChanged(2, idx);
                _cfgItem->setFilterFilterTypeR(_curIdx, idx);
            }
        }
        break;
    case CFG_TYPE_SPLIT_4:
        if (sender() == _ui_FangTu->cb_Filter_L)
        {
            _curIdx = 0;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            if (_ui_FangTu->pb_Filter_LSetSync->isChecked())
            {
                disconnect(_ui_FangTu->cb_Filter_L_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->cb_Filter_L_LM->setCurrentIndex(idx);
                connect(_ui_FangTu->cb_Filter_L_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->eqWidget->filterTypeChanged(_curIdx, idx);
                _ui_FangTu->eqWidget->filterTypeChanged(CURIDX_NEXT, idx);
                _cfgItem->setFilterFilterTypeLL(_curIdx, CURIDX_NEXT, idx);
            }
            else
            {
                _ui_FangTu->eqWidget->filterTypeChanged(_curIdx, idx);
                _cfgItem->setFilterFilterTypeL(_curIdx, idx);
            }
        }
        if (sender() == _ui_FangTu->cb_Filter_L_LM)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            if (_ui_FangTu->pb_Filter_LSetSync_LM->isChecked())
            {
                disconnect(_ui_FangTu->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->cb_Filter_L->setCurrentIndex(idx);
                connect(_ui_FangTu->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->eqWidget->filterTypeChanged(CURIDX_PRE, idx);
                _ui_FangTu->eqWidget->filterTypeChanged(_curIdx, idx);
                _cfgItem->setFilterFilterTypeLL(CURIDX_PRE, _curIdx, idx);
            }
            else
            {
                _ui_FangTu->eqWidget->filterTypeChanged(_curIdx, idx);
                _cfgItem->setFilterFilterTypeL(_curIdx, idx);
            }
        }
        if (sender() == _ui_FangTu->cb_Filter_R_LM)
        {
            _curIdx = 1;
            _ui_FangTu->eqWidget->setCurrentIdx(_curIdx);
            if (_ui_FangTu->pb_Filter_RSetSync_LM->isChecked())
            {
                disconnect(_ui_FangTu->cb_Filter_L_HM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->cb_Filter_L_HM->setCurrentIndex(idx);
                connect(_ui_FangTu->cb_Filter_L_HM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->eqWidget->filterTypeChanged(2, idx);
                _ui_FangTu->eqWidget->filterTypeChanged(3, idx);
                _cfgItem->setFilterFilterTypeLR(_curIdx, CURIDX_NEXT, idx);
            }
            else
            {
                _ui_FangTu->eqWidget->filterTypeChanged(2, idx);
                _cfgItem->setFilterFilterTypeR(_curIdx, idx);
            }
        }
        if (sender() == _ui_FangTu->cb_Filter_L_HM)
        {
            _curIdx = 2;
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            if (_ui_FangTu->pb_Filter_LSetSync_HM->isChecked())
            {
                disconnect(_ui_FangTu->cb_Filter_R_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->cb_Filter_R_LM->setCurrentIndex(idx);
                connect(_ui_FangTu->cb_Filter_R_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->eqWidget->filterTypeChanged(2, idx);
                _ui_FangTu->eqWidget->filterTypeChanged(3, idx);
                _cfgItem->setFilterFilterTypeLR(_curIdx, CURIDX_PRE, idx);
            }
            else
            {
                _ui_FangTu->eqWidget->filterTypeChanged(3, idx);
                _cfgItem->setFilterFilterTypeL(_curIdx, idx);
            }
        }
        if (sender() == _ui_FangTu->cb_Filter_R_HM)
        {
            _curIdx = 2;
            _ui_FangTu->eqWidget->setCurrentIdx(3);
            if (_ui_FangTu->pb_Filter_RSetSync_HM->isChecked())
            {
                disconnect(_ui_FangTu->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->cb_Filter_R->setCurrentIndex(idx);
                connect(_ui_FangTu->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->eqWidget->filterTypeChanged(4, idx);
                _ui_FangTu->eqWidget->filterTypeChanged(5, idx);
                _cfgItem->setFilterFilterTypeRR(_curIdx, CURIDX_NEXT, idx);
            }
            else
            {
                _ui_FangTu->eqWidget->filterTypeChanged(4, idx);
                _cfgItem->setFilterFilterTypeR(_curIdx, idx);
            }
        }
        else if (sender() == _ui_FangTu->cb_Filter_R)
        {
            _curIdx = 3;
            _ui_FangTu->eqWidget->setCurrentIdx(5);
            if (_ui_FangTu->pb_Filter_RSetSync->isChecked())
            {
                disconnect(_ui_FangTu->cb_Filter_R_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->cb_Filter_R_HM->setCurrentIndex(idx);
                disconnect(_ui_FangTu->cb_Filter_R_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
                _ui_FangTu->eqWidget->filterTypeChanged(4, idx);
                _ui_FangTu->eqWidget->filterTypeChanged(5, idx);
                _cfgItem->setFilterFilterTypeRR(CURIDX_PRE, _curIdx, idx);
            }
            else
            {
                _ui_FangTu->eqWidget->filterTypeChanged(5, idx);
                _cfgItem->setFilterFilterTypeR(_curIdx, idx);
            }
        }
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSplite::initUi_FangTu()
{
    _ui_FangTu = new Ui::CfgSettingSplite_FangTu;
    _ui_FangTu->setupUi(this);

    _ui_FangTu->dsb_Input_Level->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    _ui_FangTu->dsb_Input_Level->setDecimalsNum(1);
    //L & H
    _ui_FangTu->dsb_Output_Level_L->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    _ui_FangTu->dsb_Output_Level_L->setDecimalsNum(1);

    _ui_FangTu->dsb_Output_Level_R->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    _ui_FangTu->dsb_Output_Level_R->setDecimalsNum(1);

    _ui_FangTu->dsb_Filter_L_Freq->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _ui_FangTu->dsb_Filter_L_Freq->setDecimalsNum(1);

    _ui_FangTu->dsb_Filter_R_Freq->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _ui_FangTu->dsb_Filter_R_Freq->setDecimalsNum(1);
    //M
    _ui_FangTu->dsb_Output_Level_M->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    _ui_FangTu->dsb_Output_Level_M->setDecimalsNum(1);

    _ui_FangTu->dsb_Filter_L_Freq_M->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _ui_FangTu->dsb_Filter_L_Freq_M->setDecimalsNum(1);

    _ui_FangTu->dsb_Filter_R_Freq_M->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _ui_FangTu->dsb_Filter_R_Freq_M->setDecimalsNum(1);

    //LM
    _ui_FangTu->dsb_Output_Level_LM->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    _ui_FangTu->dsb_Output_Level_LM->setDecimalsNum(1);

    _ui_FangTu->dsb_Filter_L_Freq_LM->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _ui_FangTu->dsb_Filter_L_Freq_LM->setDecimalsNum(1);

    _ui_FangTu->dsb_Filter_R_Freq_LM->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _ui_FangTu->dsb_Filter_R_Freq_LM->setDecimalsNum(1);

    //HM
    _ui_FangTu->dsb_Output_Level_HM->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
    _ui_FangTu->dsb_Output_Level_HM->setDecimalsNum(1);

    _ui_FangTu->dsb_Filter_L_Freq_HM->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _ui_FangTu->dsb_Filter_L_Freq_HM->setDecimalsNum(1);

    _ui_FangTu->dsb_Filter_R_Freq_HM->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    _ui_FangTu->dsb_Filter_R_Freq_HM->setDecimalsNum(1);

    _ui_FangTu->pb_Input_Mute->setProperty("form", "Mute");
    _ui_FangTu->pb_Output_Mute_L->setProperty("form", "Mute");
    _ui_FangTu->pb_Output_Mute_LM->setProperty("form", "Mute");
    _ui_FangTu->pb_Output_Mute_HM->setProperty("form", "Mute");
    _ui_FangTu->pb_Output_Mute_M->setProperty("form", "Mute");
    _ui_FangTu->pb_Output_Mute_R->setProperty("form", "Mute");

//    _ui_FangTu->label_30->setVisible(false);
    _ui_FangTu->label_30->setFixedHeight(8);
    switch(_cfgItem->getCfgType()) {
    case CFG_TYPE_SPLIT_2:
        _ui_FangTu->groupBox_M->setHidden(true);
        _ui_FangTu->groupBox_LM->setHidden(true);
        _ui_FangTu->groupBox_HM->setHidden(true);
        break;
    case CFG_TYPE_SPLIT_3:
        _ui_FangTu->groupBox_LM->setHidden(true);
        _ui_FangTu->groupBox_HM->setHidden(true);
        break;
    case CFG_TYPE_SPLIT_4:
        _ui_FangTu->groupBox_M->setHidden(true);
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSplite::initData_FangTu()
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
    {
        //input
        _ui_FangTu->pb_Input_Mute->setChecked(_cfgItem->getInputMute());
        _ui_FangTu->dsb_Input_Level->setValue(_cfgItem->getInputLevel());

        initCurrentDisplay_FangTu();

        crossOverPoint tmp[2] = {
            {(float)_cfgItem->getFilterFreqL(0), _cfgItem->getFilterFilterTypeL(0)},
            {(float)_cfgItem->getFilterFreqR(1), _cfgItem->getFilterFilterTypeR(1)}
        };
        _ui_FangTu->eqWidget->initType(CrossOverType_2Way, tmp);
    }
        break;
    case CFG_TYPE_SPLIT_3:
    {
        _ui_FangTu->pb_Input_Mute->setChecked(_cfgItem->getInputMute());
        _ui_FangTu->dsb_Input_Level->setValue(_cfgItem->getInputLevel());

        initCurrentDisplay_FangTu();


        crossOverPoint tmp[4] = {
            {(float)_cfgItem->getFilterFreqL(0), _cfgItem->getFilterFilterTypeL(0)},
            {(float)_cfgItem->getFilterFreqL(1), _cfgItem->getFilterFilterTypeL(1)},
            {(float)_cfgItem->getFilterFreqR(1), _cfgItem->getFilterFilterTypeR(1)},
            {(float)_cfgItem->getFilterFreqR(2), _cfgItem->getFilterFilterTypeR(2)}
        };
        _ui_FangTu->eqWidget->initType(CrossOverType_3Way, tmp);
    }
        break;
    case CFG_TYPE_SPLIT_4:
    {
        _ui_FangTu->pb_Input_Mute->setChecked(_cfgItem->getInputMute());
        _ui_FangTu->dsb_Input_Level->setValue(_cfgItem->getInputLevel());

        initCurrentDisplay_FangTu();

        crossOverPoint tmp[6] = {
            {(float)_cfgItem->getFilterFreqL(0), _cfgItem->getFilterFilterTypeL(0)},
            {(float)_cfgItem->getFilterFreqL(1), _cfgItem->getFilterFilterTypeL(1)},
            {(float)_cfgItem->getFilterFreqR(1), _cfgItem->getFilterFilterTypeR(1)},
            {(float)_cfgItem->getFilterFreqL(2), _cfgItem->getFilterFilterTypeL(2)},
            {(float)_cfgItem->getFilterFreqR(2), _cfgItem->getFilterFilterTypeR(2)},
            {(float)_cfgItem->getFilterFreqR(3), _cfgItem->getFilterFilterTypeR(3)}
        };
        _ui_FangTu->eqWidget->initType(CrossOverType_4Way, tmp);
    }
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSplite::initSignalConnect_FangTu()
{
    //input
    connect(_ui_FangTu->pb_Input_Mute, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->dsb_Input_Level, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    //eqWidget
    connect(_ui_FangTu->eqWidget, SIGNAL(pointChange(int)), this, SLOT(pointChange(int)));
    connect(_ui_FangTu->eqWidget, SIGNAL(pointMove(int,float)), this, SLOT(pointMove(int, float)));
    //L & H
    connect(_ui_FangTu->pb_Filter_LSetSync, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Filter_RSetSync, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Output_Mute_L, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Output_Invert_L, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Output_Mute_R, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Output_Invert_R, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));

    connect(_ui_FangTu->dsb_Filter_L_Freq, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    connect(_ui_FangTu->dsb_Filter_R_Freq, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    connect(_ui_FangTu->dsb_Output_Level_L, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    connect(_ui_FangTu->dsb_Output_Level_R, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));

    connect(_ui_FangTu->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
    connect(_ui_FangTu->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));

    // M
    connect(_ui_FangTu->pb_Filter_LSetSync_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Filter_RSetSync_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Output_Mute_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Output_Invert_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));

    connect(_ui_FangTu->dsb_Filter_L_Freq_M, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    connect(_ui_FangTu->dsb_Filter_R_Freq_M, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    connect(_ui_FangTu->dsb_Output_Level_M, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));

    connect(_ui_FangTu->cb_Filter_L_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
    connect(_ui_FangTu->cb_Filter_R_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));

    //LM
    connect(_ui_FangTu->pb_Filter_LSetSync_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Filter_RSetSync_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Output_Mute_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Output_Invert_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));

    connect(_ui_FangTu->dsb_Filter_L_Freq_LM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    connect(_ui_FangTu->dsb_Filter_R_Freq_LM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    connect(_ui_FangTu->dsb_Output_Level_LM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));

    connect(_ui_FangTu->cb_Filter_L_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
    connect(_ui_FangTu->cb_Filter_R_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));

    //HM
    connect(_ui_FangTu->pb_Filter_LSetSync_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Filter_RSetSync_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Output_Mute_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    connect(_ui_FangTu->pb_Output_Invert_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));

    connect(_ui_FangTu->dsb_Filter_L_Freq_HM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    connect(_ui_FangTu->dsb_Filter_R_Freq_HM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    connect(_ui_FangTu->dsb_Output_Level_HM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));

    connect(_ui_FangTu->cb_Filter_L_HM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
    connect(_ui_FangTu->cb_Filter_R_HM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));

}

void CfgSettingSplite::disableSignalConnect_FangTu()
{
    //input
    disconnect(_ui_FangTu->pb_Input_Mute, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->dsb_Input_Level, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    //eqWidget
    disconnect(_ui_FangTu->eqWidget, SIGNAL(pointChange(int)), this, SLOT(pointChange(int)));
    disconnect(_ui_FangTu->eqWidget, SIGNAL(pointMove(int,float)), this, SLOT(pointMove(int, float)));
    //L & H
    disconnect(_ui_FangTu->pb_Filter_LSetSync, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Filter_RSetSync, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Output_Mute_L, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Output_Invert_L, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Output_Mute_R, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Output_Invert_R, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));

    disconnect(_ui_FangTu->dsb_Filter_L_Freq, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    disconnect(_ui_FangTu->dsb_Filter_R_Freq, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    disconnect(_ui_FangTu->dsb_Output_Level_L, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    disconnect(_ui_FangTu->dsb_Output_Level_R, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));

    disconnect(_ui_FangTu->cb_Filter_L, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
    disconnect(_ui_FangTu->cb_Filter_R, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));

    // M
    disconnect(_ui_FangTu->pb_Filter_LSetSync_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Filter_RSetSync_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Output_Mute_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Output_Invert_M, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));

    disconnect(_ui_FangTu->dsb_Filter_L_Freq_M, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    disconnect(_ui_FangTu->dsb_Filter_R_Freq_M, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    disconnect(_ui_FangTu->dsb_Output_Level_M, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));

    disconnect(_ui_FangTu->cb_Filter_L_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
    disconnect(_ui_FangTu->cb_Filter_R_M, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));

    //LM
    disconnect(_ui_FangTu->pb_Filter_LSetSync_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Filter_RSetSync_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Output_Mute_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Output_Invert_LM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));

    disconnect(_ui_FangTu->dsb_Filter_L_Freq_LM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    disconnect(_ui_FangTu->dsb_Filter_R_Freq_LM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    disconnect(_ui_FangTu->dsb_Output_Level_LM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));

    disconnect(_ui_FangTu->cb_Filter_L_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
    disconnect(_ui_FangTu->cb_Filter_R_LM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));

    //HM
    disconnect(_ui_FangTu->pb_Filter_LSetSync_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Filter_RSetSync_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Output_Mute_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));
    disconnect(_ui_FangTu->pb_Output_Invert_HM, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot_FangTu(bool)));

    disconnect(_ui_FangTu->dsb_Filter_L_Freq_HM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    disconnect(_ui_FangTu->dsb_Filter_R_Freq_HM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));
    disconnect(_ui_FangTu->dsb_Output_Level_HM, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot_FangTu(double)));

    disconnect(_ui_FangTu->cb_Filter_L_HM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
    disconnect(_ui_FangTu->cb_Filter_R_HM, SIGNAL(currentIndexChanged(int)), this, SLOT(processComboBoxSlot_FangTu(int)));
}

void CfgSettingSplite::initCurrentDisplay_FangTu()
{
    disableSignalConnect_FangTu();
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SPLIT_2:
        _ui_FangTu->pb_Filter_LSetSync->setChecked(_cfgItem->getFilterSetSyncL(0));
        _ui_FangTu->pb_Filter_RSetSync->setChecked(_cfgItem->getFilterSetSyncR(1));
        _ui_FangTu->cb_Filter_L->setCurrentIndex(_cfgItem->getFilterFilterTypeL(0));
        _ui_FangTu->cb_Filter_R->setCurrentIndex(_cfgItem->getFilterFilterTypeR(1));
        _ui_FangTu->dsb_Filter_L_Freq->setValue(_cfgItem->getFilterFreqL(0));
        _ui_FangTu->dsb_Filter_R_Freq->setValue(_cfgItem->getFilterFreqR(1));

        _ui_FangTu->pb_Output_Mute_L->setChecked(_cfgItem->getOutputMute(0));
        _ui_FangTu->pb_Output_Invert_L->setChecked(_cfgItem->getOutputInvert(0));
        _ui_FangTu->dsb_Output_Level_L->setValue(_cfgItem->getOutputLevel(0));

        _ui_FangTu->pb_Output_Mute_R->setChecked(_cfgItem->getOutputMute(1));
        _ui_FangTu->pb_Output_Invert_R->setChecked(_cfgItem->getOutputInvert(1));
        _ui_FangTu->dsb_Output_Level_R->setValue(_cfgItem->getOutputLevel(1));
        break;
    case CFG_TYPE_SPLIT_3:////////////////////////////////////////
        _ui_FangTu->pb_Filter_LSetSync->setChecked(_cfgItem->getFilterSetSyncL(0));
        _ui_FangTu->pb_Filter_RSetSync->setChecked(_cfgItem->getFilterSetSyncR(2));
        _ui_FangTu->cb_Filter_L->setCurrentIndex(_cfgItem->getFilterFilterTypeL(0));
        _ui_FangTu->cb_Filter_R->setCurrentIndex(_cfgItem->getFilterFilterTypeR(2));
        _ui_FangTu->dsb_Filter_L_Freq->setValue(_cfgItem->getFilterFreqL(0));
        _ui_FangTu->dsb_Filter_R_Freq->setValue(_cfgItem->getFilterFreqR(2));

        _ui_FangTu->pb_Output_Mute_L->setChecked(_cfgItem->getOutputMute(0));
        _ui_FangTu->pb_Output_Invert_L->setChecked(_cfgItem->getOutputInvert(0));
        _ui_FangTu->dsb_Output_Level_L->setValue(_cfgItem->getOutputLevel(0));

        _ui_FangTu->pb_Output_Mute_R->setChecked(_cfgItem->getOutputMute(2));
        _ui_FangTu->pb_Output_Invert_R->setChecked(_cfgItem->getOutputInvert(2));
        _ui_FangTu->dsb_Output_Level_R->setValue(_cfgItem->getOutputLevel(2));

        _ui_FangTu->pb_Filter_LSetSync_M->setChecked(_cfgItem->getFilterSetSyncL(1));
        _ui_FangTu->pb_Filter_RSetSync_M->setChecked(_cfgItem->getFilterSetSyncR(1));
        _ui_FangTu->cb_Filter_L_M->setCurrentIndex(_cfgItem->getFilterFilterTypeL(1));
        _ui_FangTu->cb_Filter_R_M->setCurrentIndex(_cfgItem->getFilterFilterTypeR(1));
        _ui_FangTu->dsb_Filter_L_Freq_M->setValue(_cfgItem->getFilterFreqL(1));
        _ui_FangTu->dsb_Filter_R_Freq_M->setValue(_cfgItem->getFilterFreqR(1));

        _ui_FangTu->pb_Output_Mute_M->setChecked(_cfgItem->getOutputMute(1));
        _ui_FangTu->pb_Output_Invert_M->setChecked(_cfgItem->getOutputInvert(1));
        _ui_FangTu->dsb_Output_Level_M->setValue(_cfgItem->getOutputLevel(1));
        break;
    case CFG_TYPE_SPLIT_4:
        _ui_FangTu->pb_Filter_LSetSync->setChecked(_cfgItem->getFilterSetSyncL(0));
        _ui_FangTu->pb_Filter_RSetSync->setChecked(_cfgItem->getFilterSetSyncR(3));
        _ui_FangTu->cb_Filter_L->setCurrentIndex(_cfgItem->getFilterFilterTypeL(0));
        _ui_FangTu->cb_Filter_R->setCurrentIndex(_cfgItem->getFilterFilterTypeR(3));
        _ui_FangTu->dsb_Filter_L_Freq->setValue(_cfgItem->getFilterFreqL(0));
        _ui_FangTu->dsb_Filter_R_Freq->setValue(_cfgItem->getFilterFreqR(3));

        _ui_FangTu->pb_Output_Mute_L->setChecked(_cfgItem->getOutputMute(0));
        _ui_FangTu->pb_Output_Invert_L->setChecked(_cfgItem->getOutputInvert(0));
        _ui_FangTu->dsb_Output_Level_L->setValue(_cfgItem->getOutputLevel(0));

        _ui_FangTu->pb_Output_Mute_R->setChecked(_cfgItem->getOutputMute(3));
        _ui_FangTu->pb_Output_Invert_R->setChecked(_cfgItem->getOutputInvert(3));
        _ui_FangTu->dsb_Output_Level_R->setValue(_cfgItem->getOutputLevel(3));

        _ui_FangTu->pb_Filter_LSetSync_LM->setChecked(_cfgItem->getFilterSetSyncL(1));
        _ui_FangTu->pb_Filter_RSetSync_LM->setChecked(_cfgItem->getFilterSetSyncR(1));
        _ui_FangTu->cb_Filter_L_LM->setCurrentIndex(_cfgItem->getFilterFilterTypeL(1));
        _ui_FangTu->cb_Filter_R_LM->setCurrentIndex(_cfgItem->getFilterFilterTypeR(1));
        _ui_FangTu->dsb_Filter_L_Freq_LM->setValue(_cfgItem->getFilterFreqL(1));
        _ui_FangTu->dsb_Filter_R_Freq_LM->setValue(_cfgItem->getFilterFreqR(1));

        _ui_FangTu->pb_Output_Mute_LM->setChecked(_cfgItem->getOutputMute(1));
        _ui_FangTu->pb_Output_Invert_LM->setChecked(_cfgItem->getOutputInvert(1));

        _ui_FangTu->dsb_Output_Level_LM->setValue(_cfgItem->getOutputLevel(1));

        _ui_FangTu->pb_Filter_LSetSync_HM->setChecked(_cfgItem->getFilterSetSyncL(2));
        _ui_FangTu->pb_Filter_RSetSync_HM->setChecked(_cfgItem->getFilterSetSyncR(2));
        _ui_FangTu->cb_Filter_L_HM->setCurrentIndex(_cfgItem->getFilterFilterTypeL(2));
        _ui_FangTu->cb_Filter_R_HM->setCurrentIndex(_cfgItem->getFilterFilterTypeR(2));
        _ui_FangTu->dsb_Filter_L_Freq_HM->setValue(_cfgItem->getFilterFreqL(2));
        _ui_FangTu->dsb_Filter_R_Freq_HM->setValue(_cfgItem->getFilterFreqR(2));

        _ui_FangTu->pb_Output_Mute_HM->setChecked(_cfgItem->getOutputMute(2));
        _ui_FangTu->pb_Output_Invert_HM->setChecked(_cfgItem->getOutputInvert(2));

        _ui_FangTu->dsb_Output_Level_HM->setValue(_cfgItem->getOutputLevel(2));
        break;
    default:
        Q_ASSERT(false);
    }
    initSignalConnect_FangTu();
}
