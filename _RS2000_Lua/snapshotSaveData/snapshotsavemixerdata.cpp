#include "snapshotsavemixerdata.h"
#include "configItem/configitemmixer.h"

void SnapshotSaveMixerData::saveMixerData(ConfigItem *cfgItem, int type)
{
    qDebug() << __FUNCTION__ << __LINE__;
    ConfigItemMixer *mixerItem = qgraphicsitem_cast<ConfigItemMixer *>(cfgItem);

    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_4X1)
    {
        mixer_x_data body;
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            body.body4x1.iToOSwitch[row].sw = mixerItem->getRouterInfo(row);

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int z = col / 2;
                int x = col %2;
                if(x)
                {
                    body.body4x1.iToOGain[row][z].gainB = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body4x1.ingain[z].gainB = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }else {
                    body.body4x1.iToOGain[row][z].gainA =(mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body4x1.ingain[z].gainA = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }

                if(mixerItem->getInGainMute(col).mute)
                {
                    body.body4x1.inSwitch |= (1 << col);
                }else {
                    body.body4x1.inSwitch &= ~(1 << col);
                }
            }
            body.body4x1.outgain[row].gain = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            if(mixerItem->getOutGainMute(row).mute)
            {
                body.body4x1.outSwitch |= (1 << row);
            }else {
                body.body4x1.outSwitch &= ~(1 << row);
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = mixerItem->getLabText(i, j).toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
//                qDebug()<<__FUNCTION__<<__LINE__<<body._inAndOutLabel[i][j];
            }
        }
        _mixer_4x1_DataList[type].insert(mixerItem->customName(), body);
        _moduleNum.insert(mixerItem->customName(), mixerItem->getModuleNum());
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_4X2)
    {
        mixer_x_data body;
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            int rz = row / 2;
            int rx = row % 2;
            if(rx)
            {
                body.body4x2.iToOSwitch[rz].swB = mixerItem->getRouterInfo(row);
                body.body4x2.outgain[rz].gainB = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            }else {
                body.body4x2.iToOSwitch[rz].swA = mixerItem->getRouterInfo(row);
                body.body4x2.outgain[rz].gainA = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            }
            if(mixerItem->getOutGainMute(row).mute)
            {
                body.body4x2.outSwitch |= (1 << row);
            }else {
                body.body4x2.outSwitch &= ~(1 << row);
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    body.body4x2.iToOGain[row][cz].gainB = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body4x2.ingain[cz].gainB = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }else {
                    body.body4x2.iToOGain[row][cz].gainA = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body4x2.ingain[cz].gainA = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }
                if(mixerItem->getInGainMute(col).mute)
                {
                    body.body4x2.inSwitch |= (1 << col);
                }else {
                    body.body4x2.inSwitch &= ~(1 << col);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = mixerItem->getLabText(i, j).toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
            }
        }

        _mixer_4x2_DataList[type].insert(mixerItem->customName(), body);
        _moduleNum.insert(mixerItem->customName(), mixerItem->getModuleNum());
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_4X4)
    {
        mixer_x_data body;
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            int rz = row / 2;
            int rx = row % 2;
            if(rx)
            {
                body.body4x4.iToOSwitch[rz].swB = mixerItem->getRouterInfo(row);
                body.body4x4.outgain[rz].gainB = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            }else {
                body.body4x4.iToOSwitch[rz].swA = mixerItem->getRouterInfo(row);
                body.body4x4.outgain[rz].gainA = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            }
            if(mixerItem->getOutGainMute(row).mute)
            {
                body.body4x4.outSwitch |= (1 << row);
            }else {
                body.body4x4.outSwitch &= ~(1 << row);
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    body.body4x4.iToOGain[row][cz].gainB = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body4x4.ingain[cz].gainB = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }else {
                    body.body4x4.iToOGain[row][cz].gainA = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body4x4.ingain[cz].gainA = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }
                if(mixerItem->getInGainMute(col).mute)
                {
                    body.body4x4.inSwitch |= (1 << col);
                }else {
                    body.body4x4.inSwitch &= ~(1 << col);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = mixerItem->getLabText(i, j).toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
            }
        }

        _mixer_4x4_DataList[type].insert(mixerItem->customName(), body);
        _moduleNum.insert(mixerItem->customName(), mixerItem->getModuleNum());
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_8X1)
    {
        mixer_x_data body;
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            body.body8x1.iToOSwitch[row].sw = mixerItem->getRouterInfo(row);
            body.body8x1.outgain[row].gain = (mixerItem->getOutGainMute(row).gain + 100) * 100;

            if(mixerItem->getOutGainMute(row).mute)
            {
                body.body8x1.outSwitch |= (1 << row);
            }else {
                body.body8x1.outSwitch &= ~(1 << row);
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    body.body8x1.iToOGain[row][cz].gainB = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body8x1.ingain[cz].gainB = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }else {
                    body.body8x1.iToOGain[row][cz].gainA = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body8x1.ingain[cz].gainA = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }
                if(mixerItem->getInGainMute(col).mute)
                {
                    body.body8x1.inSwitch |= (1 << col);
                }else {
                    body.body8x1.inSwitch &= ~(1 << col);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = mixerItem->getLabText(i, j).toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
            }
        }

        _mixer_8x1_DataList[type].insert(mixerItem->customName(), body);
        _moduleNum.insert(mixerItem->customName(), mixerItem->getModuleNum());
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_8X2)
    {
        mixer_x_data body;
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            int rz = row / 2;
            int rx = row % 2;
            if(rx)
            {
                body.body8x2.iToOSwitch[rz].swB = mixerItem->getRouterInfo(row);
                body.body8x2.outgain[rz].gainB = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            }else {
                body.body8x2.iToOSwitch[rz].swA = mixerItem->getRouterInfo(row);
                body.body8x2.outgain[rz].gainA = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            }
            if(mixerItem->getOutGainMute(row).mute)
            {
                body.body8x2.outSwitch |= (1 << row);
            }else {
                body.body8x2.outSwitch &= ~(1 << row);
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    body.body8x2.iToOGain[row][cz].gainB = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body8x2.ingain[cz].gainB = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }else {
                    body.body8x2.iToOGain[row][cz].gainA = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body8x2.ingain[cz].gainA = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }
                if(mixerItem->getInGainMute(col).mute)
                {
                    body.body8x2.inSwitch |= (1 << col);
                }else {
                    body.body8x2.inSwitch &= ~(1 << col);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = mixerItem->getLabText(i, j).toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
            }
        }

        _mixer_8x2_DataList[type].insert(mixerItem->customName(), body);
        _moduleNum.insert(mixerItem->customName(), mixerItem->getModuleNum());
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_8X4)
    {
        mixer_x_data body;
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            int rz = row / 2;
            int rx = row % 2;
            if(rx)
            {
                body.body8x4.iToOSwitch[rz].swB = mixerItem->getRouterInfo(row);
                body.body8x4.outgain[rz].gainB = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            }else {
                body.body8x4.iToOSwitch[rz].swA = mixerItem->getRouterInfo(row);
                body.body8x4.outgain[rz].gainA = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            }
            if(mixerItem->getOutGainMute(row).mute)
            {
                body.body8x4.outSwitch |= (1 << row);
            }else {
                body.body8x4.outSwitch &= ~(1 << row);
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    body.body8x4.iToOGain[row][cz].gainB = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body8x4.ingain[cz].gainB = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }else {
                    body.body8x4.iToOGain[row][cz].gainA = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body8x4.ingain[cz].gainA = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }
                if(mixerItem->getInGainMute(col).mute)
                {
                    body.body8x4.inSwitch |= (1 << col);
                }else {
                    body.body8x4.inSwitch &= ~(1 << col);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = mixerItem->getLabText(i, j).toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
            }
        }

        _mixer_8x4_DataList[type].insert(mixerItem->customName(), body);
        _moduleNum.insert(mixerItem->customName(), mixerItem->getModuleNum());
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_8X8)
    {
        mixer_x_data body;
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            int rz = row / 2;
            int rx = row % 2;
            if(rx)
            {
                body.body8x8.iToOSwitch[rz].swB = mixerItem->getRouterInfo(row);
                body.body8x8.outgain[rz].gainB = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            }else {
                body.body8x8.iToOSwitch[rz].swA = mixerItem->getRouterInfo(row);
                body.body8x8.outgain[rz].gainA = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            }
            if(mixerItem->getOutGainMute(row).mute)
            {
                body.body8x8.outSwitch |= (1 << row);
            }else {
                body.body8x8.outSwitch &= ~(1 << row);
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    body.body8x8.iToOGain[row][cz].gainB = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body8x8.ingain[cz].gainB = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }else {
                    body.body8x8.iToOGain[row][cz].gainA = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body8x8.ingain[cz].gainA = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }
                if(mixerItem->getInGainMute(col).mute)
                {
                    body.body8x8.inSwitch |= (1 << col);
                }else {
                    body.body8x8.inSwitch &= ~(1 << col);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = mixerItem->getLabText(i, j).toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
            }
        }

        _mixer_8x8_DataList[type].insert(mixerItem->customName(), body);
        _moduleNum.insert(mixerItem->customName(), mixerItem->getModuleNum());
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_16X16)
    {
        mixer_x_data body;
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            int rz = row / 2;
            int rx = row % 2;
            if(rx)
            {
                body.body16x16.iToOSwitch[rz].swB = mixerItem->getRouterInfo(row);
                body.body16x16.outgain[rz].gainB = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            }else {
                body.body16x16.iToOSwitch[rz].swA = mixerItem->getRouterInfo(row);
                body.body16x16.outgain[rz].gainA = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            }
            if(mixerItem->getOutGainMute(row).mute)
            {
                body.body16x16.outSwitch |= (1 << row);
            }else {
                body.body16x16.outSwitch &= ~(1 << row);
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    body.body16x16.iToOGain[row][cz].gainB = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body16x16.ingain[cz].gainB = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }else {
                    body.body16x16.iToOGain[row][cz].gainA = (mixerItem->getRouterGain(row, col) + 100) * 100;
                    body.body16x16.ingain[cz].gainA = (mixerItem->getInGainMute(col).gain + 100) * 100;
                }
                if(mixerItem->getInGainMute(col).mute)
                {
                    body.body16x16.inSwitch |= (1 << col);
                }else {
                    body.body16x16.inSwitch &= ~(1 << col);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = mixerItem->getLabText(i, j).toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
            }
        }

        _mixer_16x16_DataList[type].insert(mixerItem->customName(), body);
        _moduleNum.insert(mixerItem->customName(), mixerItem->getModuleNum());
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_32X32)
    {
        mixer_x_data body;
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            body.body32x32._nodeSw[row][NODE_SW_L32] = mixerItem->getRouterInfo(row);
            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                body.body32x32._nodeGain[row][col] = (mixerItem->getRouterGain(row, col) + 100) * 100;
                body.body32x32._inAndOutGain[col][IN_SELECT] = (mixerItem->getInGainMute(col).gain + 100) * 100;
                if(mixerItem->getInGainMute(col).mute)
                {
                    body.body32x32.chSwIn[NODE_SW_L32] |= (1 << col);
                }else {
                    body.body32x32.chSwIn[NODE_SW_L32] &= ~(1 << col);
                }
            }
            body.body32x32._inAndOutGain[row][OUT_SELECT] = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            if(mixerItem->getOutGainMute(row).mute)
            {
                body.body32x32.chSwOut[NODE_SW_L32] |= (1 << row);
            }else {
                body.body32x32.chSwOut[NODE_SW_L32] &= ~(1 << row);
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = mixerItem->getLabText(i, j).toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
            }
        }

        _mixer_32x32_DataList[type].insert(mixerItem->customName(), body);
        _moduleNum.insert(mixerItem->customName(), mixerItem->getModuleNum());
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_64X64)
    {
        mixer_x_data body;
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                body.body64x64._nodeGain[row][col] = (mixerItem->getRouterGain(row, col) + 100) * 100;
                body.body64x64._inAndOutGain[col][IN_SELECT] = (mixerItem->getInGainMute(col).gain + 100) * 100;
                if(col < 32)
                {
                    body.body64x64._nodeSw[((row|0x00)&0x7F)][NODE_SW_L32] = mixerItem->getRouterInfo(row | 0x00);
                    if(mixerItem->getInGainMute(col).mute)
                    {
                        body.body64x64.chSwIn[NODE_SW_L32] |= (1 << col);
                    }else {
                        body.body64x64.chSwIn[NODE_SW_L32] &= ~(1 << col);
                    }
                }else{
                    body.body64x64._nodeSw[((row|0x80)&0x7F)][NODE_SW_H32] = mixerItem->getRouterInfo(row | 0x80);
                    if(mixerItem->getInGainMute(col).mute)
                    {
                        body.body64x64.chSwIn[NODE_SW_H32] |= (1 << (col - 32));
                    }else {
                        body.body64x64.chSwIn[NODE_SW_H32] &= ~(1 << (col - 32));
                    }
                }
            }
            body.body64x64._inAndOutGain[row][OUT_SELECT] = (mixerItem->getOutGainMute(row).gain + 100) * 100;
            if(row < 32)
            {
                if(mixerItem->getOutGainMute(row).mute)
                {
                    body.body64x64.chSwOut[NODE_SW_L32] |= (1 << row);
                }else {
                    body.body64x64.chSwOut[NODE_SW_L32] &= ~(1 << row);
                }
            }else {
                if(mixerItem->getOutGainMute(row).mute)
                {
                    body.body64x64.chSwOut[NODE_SW_H32] |= (1 << (row - 32));
                }else {
                    body.body64x64.chSwOut[NODE_SW_H32] &= ~(1 << (row - 32));
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = mixerItem->getLabText(i, j).toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
            }
        }

        _mixer_64x64_DataList[type].insert(mixerItem->customName(), body);
        _moduleNum.insert(mixerItem->customName(), mixerItem->getModuleNum());
    }

}

void SnapshotSaveMixerData::setMixerData(ConfigItem *cfgItem, int type)
{
    ConfigItemMixer *mixerItem = qgraphicsitem_cast<ConfigItemMixer *>(cfgItem);

    quint32 moduleNum = _moduleNum.find(mixerItem->customName()).value();
    mixerItem->setModuleNum(moduleNum);

    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_4X1)
    {
        if(!findCfgItemData<mixer_x_data>(_mixer_4x1_DataList[type], cfgItem)) return;

        mixer_x_data body = _mixer_4x1_DataList[type].find(mixerItem->customName()).value();

        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            mixerItem->setRouterInfo(row, body.body4x1.iToOSwitch[row].sw);
            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int z = col / 2;
                int x = col % 2;
                if(x)
                {
                    mixerItem->setRouterGain(row, col, body.body4x1.iToOGain[row][z].gainB * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body4x1.ingain[z].gainB * 0.01 - 100, body.body4x1.inSwitch >> col & 0x01);
                }else {
                    mixerItem->setRouterGain(row, col, body.body4x1.iToOGain[row][z].gainA * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body4x1.ingain[z].gainA * 0.01 - 100, body.body4x1.inSwitch >> col & 0x01);
                }
            }
            mixerItem->setOutGainMute(row, body.body4x1.outgain[row].gain * 0.01 - 100, body.body4x1.outSwitch >> row & 0x01);

        }
        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QString name(body._inAndOutLabel[i][j]);
                mixerItem->setLabText(i, j, name);
            }
        }
        mixerItem->updateCfgSettingLoadedData();
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_4X2)
    {
        if(!findCfgItemData<mixer_x_data>(_mixer_4x2_DataList[type], cfgItem)) return;

        mixer_x_data body = _mixer_4x2_DataList[type].find(mixerItem->customName()).value();
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            int rz = row / 2;
            int rx = row % 2;
            if(rx)
            {
                mixerItem->setRouterInfo(row, body.body4x2.iToOSwitch[rz].swB);
                mixerItem->setOutGainMute(row, body.body4x2.outgain[rz].gainB * 0.01 - 100, body.body4x2.outSwitch >> row & 0x01);
            }else {
                mixerItem->setRouterInfo(row, body.body4x2.iToOSwitch[rz].swA);
                mixerItem->setOutGainMute(row, body.body4x2.outgain[rz].gainA * 0.01 - 100, body.body4x2.outSwitch >> row & 0x01);
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    mixerItem->setRouterGain(row, col, body.body4x2.iToOGain[row][cz].gainB * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body4x2.ingain[cz].gainB * 0.01 - 100, body.body4x2.inSwitch >> col & 0x01);
                }else {
                    mixerItem->setRouterGain(row, col, body.body4x2.iToOGain[row][cz].gainA * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body4x2.ingain[cz].gainA * 0.01 - 100, body.body4x2.inSwitch >> col & 0x01);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QString name(body._inAndOutLabel[i][j]);
                mixerItem->setLabText(i, j, name);
            }
        }
        mixerItem->updateCfgSettingLoadedData();
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_4X4)
    {
        if(!findCfgItemData<mixer_x_data>(_mixer_4x4_DataList[type], cfgItem)) return;

        mixer_x_data body = _mixer_4x4_DataList[type].find(mixerItem->customName()).value();
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            int rz = row / 2;
            int rx = row % 2;
            if(rx)
            {
                mixerItem->setRouterInfo(row, body.body4x4.iToOSwitch[rz].swB);
                mixerItem->setOutGainMute(row, body.body4x4.outgain[rz].gainB * 0.01 - 100, body.body4x4.outSwitch >> row & 0x01);
            }else {
                mixerItem->setRouterInfo(row, body.body4x4.iToOSwitch[rz].swA);
                mixerItem->setOutGainMute(row, body.body4x4.outgain[rz].gainA * 0.01 - 100, body.body4x4.outSwitch >> row & 0x01);
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    mixerItem->setRouterGain(row, col, body.body4x4.iToOGain[row][cz].gainB * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body4x4.ingain[cz].gainB * 0.01 - 100, body.body4x4.inSwitch >> col & 0x01);
                }else {
                    mixerItem->setRouterGain(row, col, body.body4x4.iToOGain[row][cz].gainA * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body4x4.ingain[cz].gainA * 0.01 - 100, body.body4x4.inSwitch >> col & 0x01);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QString name(body._inAndOutLabel[i][j]);
                mixerItem->setLabText(i, j, name);
            }
        }
        mixerItem->updateCfgSettingLoadedData();
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_8X1)
    {
        if(!findCfgItemData<mixer_x_data>(_mixer_8x1_DataList[type], cfgItem)) return;

        mixer_x_data body = _mixer_8x1_DataList[type].find(mixerItem->customName()).value();
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            mixerItem->setRouterInfo(row, body.body8x1.iToOSwitch[row].sw);
            mixerItem->setOutGainMute(row, body.body8x1.outgain[row].gain * 0.01 - 100, body.body8x1.outSwitch >> row & 0x01);

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    mixerItem->setRouterGain(row, col, body.body8x1.iToOGain[row][cz].gainB * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body8x1.ingain[cz].gainB * 0.01 - 100, body.body8x1.inSwitch >> col & 0x01);
                }else {
                    mixerItem->setRouterGain(row, col, body.body8x1.iToOGain[row][cz].gainA * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body8x1.ingain[cz].gainA * 0.01 - 100, body.body8x1.inSwitch >> col & 0x01);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QString name(body._inAndOutLabel[i][j]);
                mixerItem->setLabText(i, j, name);
            }
        }
        mixerItem->updateCfgSettingLoadedData();
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_8X2)
    {
        if(!findCfgItemData<mixer_x_data>(_mixer_8x2_DataList[type], cfgItem)) return;

        mixer_x_data body = _mixer_8x2_DataList[type].find(mixerItem->customName()).value();
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            int rz = row / 2;
            int rx = row % 2;
            if(rx)
            {
                mixerItem->setRouterInfo(row, body.body8x2.iToOSwitch[rz].swB);
                mixerItem->setOutGainMute(row, body.body8x2.outgain[rz].gainB * 0.01 - 100, body.body8x2.outSwitch >> row & 0x01);
            }else {
                mixerItem->setRouterInfo(row, body.body8x2.iToOSwitch[rz].swA);
                mixerItem->setOutGainMute(row, body.body8x2.outgain[rz].gainA * 0.01 - 100, body.body8x2.outSwitch >> row & 0x01);
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    mixerItem->setRouterGain(row, col, body.body8x2.iToOGain[row][cz].gainB * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body8x2.ingain[cz].gainB * 0.01 - 100, body.body8x2.inSwitch >> col & 0x01);
                }else {
                    mixerItem->setRouterGain(row, col, body.body8x2.iToOGain[row][cz].gainA * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body8x2.ingain[cz].gainA * 0.01 - 100, body.body8x2.inSwitch >> col & 0x01);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QString name(body._inAndOutLabel[i][j]);
                mixerItem->setLabText(i, j, name);
            }
        }
        mixerItem->updateCfgSettingLoadedData();
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_8X4)
    {
        if(!findCfgItemData<mixer_x_data>(_mixer_8x4_DataList[type], cfgItem)) return;

        mixer_x_data body = _mixer_8x4_DataList[type].find(mixerItem->customName()).value();
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            int rz = row / 2;
            int rx = row % 2;
            if(rx)
            {
                mixerItem->setRouterInfo(row, body.body8x4.iToOSwitch[rz].swB);
                mixerItem->setOutGainMute(row, body.body8x4.outgain[rz].gainB * 0.01 - 100, body.body8x4.outSwitch >> row & 0x01);
            }else {
                mixerItem->setRouterInfo(row, body.body8x4.iToOSwitch[rz].swA);
                mixerItem->setOutGainMute(row, body.body8x4.outgain[rz].gainA * 0.01 - 100, body.body8x4.outSwitch >> row & 0x01);
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    mixerItem->setRouterGain(row, col, body.body8x4.iToOGain[row][cz].gainB * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body8x4.ingain[cz].gainB * 0.01 - 100, body.body8x4.inSwitch >> col & 0x01);
                }else {
                    mixerItem->setRouterGain(row, col, body.body8x4.iToOGain[row][cz].gainA * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body8x4.ingain[cz].gainA * 0.01 - 100, body.body8x4.inSwitch >> col & 0x01);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QString name(body._inAndOutLabel[i][j]);
                mixerItem->setLabText(i, j, name);
            }
        }
        mixerItem->updateCfgSettingLoadedData();
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_8X8)
    {
        if(!findCfgItemData<mixer_x_data>(_mixer_8x8_DataList[type], cfgItem)) return;


        mixer_x_data body = _mixer_8x8_DataList[type].find(mixerItem->customName()).value();
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            int rz = row / 2;
            int rx = row % 2;
            if(rx)
            {
                mixerItem->setRouterInfo(row, body.body8x8.iToOSwitch[rz].swB);
                mixerItem->setOutGainMute(row, body.body8x8.outgain[rz].gainB * 0.01 - 100, body.body8x8.outSwitch >> row & 0x01);
            }else {
                mixerItem->setRouterInfo(row, body.body8x8.iToOSwitch[rz].swA);
                mixerItem->setOutGainMute(row, body.body8x8.outgain[rz].gainA * 0.01 - 100, body.body8x8.outSwitch >> row & 0x01);
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    mixerItem->setRouterGain(row, col, body.body8x8.iToOGain[row][cz].gainB * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body8x8.ingain[cz].gainB * 0.01 - 100, body.body8x8.inSwitch >> col & 0x01);
                }else {
                    mixerItem->setRouterGain(row, col, body.body8x8.iToOGain[row][cz].gainA * 0.01 - 100);
                    mixerItem->setInGainMute(col, body.body8x8.ingain[cz].gainA * 0.01 - 100, body.body8x8.inSwitch >> col & 0x01);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QString name(body._inAndOutLabel[i][j]);
                mixerItem->setLabText(i, j, name);
            }
        }
        mixerItem->updateCfgSettingLoadedData();
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_16X16)
    {
        if(!findCfgItemData<mixer_x_data>(_mixer_16x16_DataList[type], cfgItem)) return;


        mixer_x_data body = _mixer_16x16_DataList[type].find(mixerItem->customName()).value();
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            int rz = row / 2;
            int rx = row % 2;
            if(rx)
            {
                mixerItem->setRouterInfo(row, body.body16x16.iToOSwitch[rz].swB);
                mixerItem->setOutGainMute(row, body.body16x16.outgain[rz].gainB * 0.01 - 100, body.body16x16.outSwitch >> row & 0x01);
//                qDebug()<<__FUNCTION__<<__LINE__<<row<<body.body16x16.iToOSwitch[rz].swB
//                       <<body.body16x16.outgain[rz].gainB * 0.01 - 100
                      /*<<body.body16x16.outSwitch >> row & 0x01*/;
            }else {
                mixerItem->setRouterInfo(row, body.body16x16.iToOSwitch[rz].swA);
                mixerItem->setOutGainMute(row, body.body16x16.outgain[rz].gainA * 0.01 - 100, body.body16x16.outSwitch >> row & 0x01);
//                qDebug()<<__FUNCTION__<<__LINE__<<row<<body.body16x16.iToOSwitch[rz].swA
//                       <<body.body16x16.outgain[rz].gainA * 0.01 - 100
                     /* <<body.body16x16.outSwitch >> row & 0x01*/;
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                int cz = col / 2;
                int cx = col % 2;
                if(cx)
                {
                    mixerItem->setRouterGain(row, col, body.body16x16.iToOGain[row][cz].gainB * 0.01 - 100);
//                    qDebug()<<__FUNCTION__<<__LINE__<<row<<col<<body.body16x16.iToOGain[row][cz].gainB * 0.01 - 100;
                    mixerItem->setInGainMute(col, body.body16x16.ingain[cz].gainB * 0.01 - 100, body.body16x16.inSwitch >> col & 0x01);
                }else {
                    mixerItem->setRouterGain(row, col, body.body16x16.iToOGain[row][cz].gainA * 0.01 - 100);
//                    qDebug()<<__FUNCTION__<<__LINE__<<row<<col<<body.body16x16.iToOGain[row][cz].gainA * 0.01 - 100;
                    mixerItem->setInGainMute(col, body.body16x16.ingain[cz].gainA * 0.01 - 100, body.body16x16.inSwitch >> col & 0x01);
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QString name(body._inAndOutLabel[i][j]);
                mixerItem->setLabText(i, j, name);
            }
        }
        mixerItem->updateCfgSettingLoadedData();
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_32X32)
    {
        if(!findCfgItemData<mixer_x_data>(_mixer_32x32_DataList[type], cfgItem)) return;


        mixer_x_data body = _mixer_32x32_DataList[type].find(mixerItem->customName()).value();
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            mixerItem->setRouterInfo(row, body.body32x32._nodeSw[row][NODE_SW_L32]);
            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                mixerItem->setRouterGain(row, col, body.body32x32._nodeGain[row][col] * 0.01 - 100);
                mixerItem->setInGainMute(col, body.body32x32._inAndOutGain[col][IN_SELECT] * 0.01 - 100, body.body32x32.chSwIn[NODE_SW_L32] >> col & 0x01);
            }
            mixerItem->setOutGainMute(row, body.body32x32._inAndOutGain[row][OUT_SELECT] * 0.01 - 100, body.body32x32.chSwOut[NODE_SW_L32] >> row & 0x01);
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QString name(body._inAndOutLabel[i][j]);
                mixerItem->setLabText(i, j, name);
            }
        }
        mixerItem->updateCfgSettingLoadedData();
    }
    if(mixerItem->getCfgType() == CFG_TYPE_MIXER_64X64)
    {
        if(!findCfgItemData<mixer_x_data>(_mixer_64x64_DataList[type], cfgItem)) return;


        mixer_x_data body = _mixer_64x64_DataList[type].find(mixerItem->customName()).value();
        for(int row = 0; row < mixerItem->getOutNum(); row++)
        {
            if(row < 32)
            {
                mixerItem->setOutGainMute(row, body.body64x64._inAndOutGain[row][OUT_SELECT] * 0.01 - 100, body.body64x64.chSwOut[NODE_SW_L32] >> row & 0x01);
            }else {
                mixerItem->setOutGainMute(row, body.body64x64._inAndOutGain[row][OUT_SELECT] * 0.01 - 100, body.body64x64.chSwOut[NODE_SW_H32] >> (row - 32) & 0x01);
            }

            for(int col = 0; col < mixerItem->getInNum(); col++)
            {
                mixerItem->setRouterGain(row, col, body.body64x64._nodeGain[row][col] * 0.01 - 100);
                int r = 0;
                if(col < 32)
                {
                    r = row | 0x00;
//                    mixerItem->setRouterInfo(r, body.body64x64._nodeSw[r&0x7F][NODE_SW_L32]);

                    mixerItem->setInGainMute(col, body.body64x64._inAndOutGain[col][IN_SELECT] * 0.01 - 100, body.body64x64.chSwIn[NODE_SW_L32] >> col & 0x01);
                }else {
                    r = row | 0x80;
//                    mixerItem->setRouterInfo(r, body.body64x64._nodeSw[r&0x7F][NODE_SW_H32]);

                    mixerItem->setInGainMute(col, body.body64x64._inAndOutGain[col][IN_SELECT] * 0.01 - 100, body.body64x64.chSwIn[NODE_SW_H32] >> (col - 32) & 0x01);
                }
                mixerItem->setRouterInfo(row, body.body64x64._nodeSw[r&0x7F][NODE_SW_L32], body.body64x64._nodeSw[r&0x7F][NODE_SW_H32]);
            }

        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QString name(body._inAndOutLabel[i][j]);
                mixerItem->setLabText(i, j, name);
            }
        }
        mixerItem->updateCfgSettingLoadedData();
    }
}

void SnapshotSaveMixerData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    int inNum = 0, outNum = 0;
    getInAndOutNum(inNum, outNum, type);
//    qDebug()<<__FUNCTION__<<__LINE__<<inNum<<outNum;
    QList<QString> nameList = getMixerDataList(pattern, type).keys();
    int unimoduleNum = getMixerDataList(pattern, type).count();
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_MIXER_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVINCOUNT), inNum);
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVOUTCOUNT), outNum);
    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_MIXER).arg(idx));
        quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
        for(int col = 0; col < inNum; col++)
        {
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_INPUTLAB).arg(col),
                                getMixerInAndOutLab(0, col, type, pattern, nameList.at(idx)));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_INGAIN).arg(col),
                                getInGainMute(col, type, pattern, nameList.at(idx)).gain);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_INMUTE).arg(col),
                                getInGainMute(col, type, pattern, nameList.at(idx)).mute);
        }
        for(int row = 0; row < outNum; row++)
        {
            for(int col = 0; col < inNum; col++)
            {
                ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_ROUTERGAIN).arg(row).arg(col),
                                    getRouterGain(row, col, type, pattern, nameList.at(idx)));
                if(type == CFG_TYPE_MIXER_64X64)
                {
                    if(col < 32)
                    {
                        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_OUTROUTER_L).arg(row),
                                            getRouterInfo((row|0x00), type, pattern, nameList.at(idx)));
                    }else if(col >= 32)
                    {
                        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_OUTROUTER_H).arg(row),
                                            getRouterInfo((row|0x80), type, pattern, nameList.at(idx)));
                    }
                }else {
                    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_OUTROUTER_L).arg(row),
                                        getRouterInfo((row), type, pattern, nameList.at(idx)));
                }
            }
        }
        for(int row = 0; row < outNum; row++)
        {
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUTLAB).arg(row),
                                getMixerInAndOutLab(1, row, type, pattern, nameList.at(idx)));

            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_OUTGAIN).arg(row),
                                getOutGainMute(row, type, pattern, nameList.at(idx)).gain);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MIXER_OUTMUTE).arg(row),
                                getOutGainMute(row, type, pattern, nameList.at(idx)).mute);
        }

        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

void SnapshotSaveMixerData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_MIXER_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();
    int inCount = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVINCOUNT)).toInt();
    int outCount = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVOUTCOUNT)).toInt();
    for(int num = 0; num < unimoduleNum; num++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_MIXER).arg(num));
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();

        GainMute inGainMute[64], outGainMute[64];
        int routerGain[64][64] = {0};
        quint32 routerInfo[64][2] = {0};
        QString inAndOutLab[64][64] = {0};
        memset(inGainMute, 0, 64*sizeof(GainMute));
        memset(outGainMute, 0, 64*sizeof(GainMute));

        for(int col = 0; col < inCount; col++)
        {
            inAndOutLab[0][col]  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_INPUTLAB).arg(col)).toString();
            inGainMute[col].gain = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_INGAIN).arg(col)).toDouble();
            inGainMute[col].mute = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_INMUTE).arg(col)).toBool();
//            qDebug()<<__FUNCTION__<<__LINE__<<inGainMute[col].gain<<inGainMute[col].mute;
        }
        for(int row = 0; row < outCount; row++)
        {
            for(int col = 0; col < inCount; col++)
            {
                routerGain[row][col] = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_ROUTERGAIN).arg(row).arg(col)).toInt();

                if(type == CFG_TYPE_MIXER_64X64)
                {
                    if(col < 32)
                    {
                        routerInfo[row][0] = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_OUTROUTER_L).arg(row)).toUInt();
                    }else {
                        routerInfo[row][1] = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_OUTROUTER_H).arg(row)).toUInt();
                    }
                }else {
                    routerInfo[row][0] = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_OUTROUTER_L).arg(row)).toUInt();
                }
            }
        }
        for(int row = 0; row < outCount; row++)
        {
            inAndOutLab[1][row]  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUTLAB).arg(row)).toString();


            outGainMute[row].gain = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_OUTGAIN).arg(row)).toDouble();
            outGainMute[row].mute = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_OUTMUTE).arg(row)).toBool();
//            qDebug()<<__FUNCTION__<<__LINE__<<row<<ConfigIni->value(QString(CONFIG_DEVICE_DEV_MIXER_OUTMUTE).arg(row)).toBool()<<outGainMute[row].gain<<outGainMute[row].mute;
        }
        ConfigIni->endGroup();
        setMixerData(devName, type, pattern, inCount, outCount, inGainMute, outGainMute, routerGain, routerInfo, inAndOutLab, moduleNum);
    }
    ConfigIni->endGroup();
}

void SnapshotSaveMixerData::getInAndOutNum(int &inNum, int &outNum, CfgType type)
{
    if(type == CFG_TYPE_MIXER_4X1)
    {
        inNum = 4; outNum = 1;
    }
    if(type == CFG_TYPE_MIXER_4X2)
    {
        inNum = 4; outNum = 2;
    }
    if(type == CFG_TYPE_MIXER_4X4)
    {
        inNum = 4; outNum = 4;
    }
    if(type == CFG_TYPE_MIXER_8X1)
    {
        inNum = 8; outNum = 1;
    }
    if(type == CFG_TYPE_MIXER_8X2)
    {
        inNum = 8; outNum = 2;
    }
    if(type == CFG_TYPE_MIXER_8X4)
    {
        inNum = 8; outNum = 4;
    }
    if(type == CFG_TYPE_MIXER_8X8)
    {
        inNum = 8; outNum = 8;
    }
    if(type == CFG_TYPE_MIXER_16X16)
    {
        inNum = 16; outNum = 16;
    }
    if(type == CFG_TYPE_MIXER_32X32)
    {
        inNum = 32; outNum = 32;
    }
    if(type == CFG_TYPE_MIXER_64X64)
    {
        inNum = 64; outNum = 64;
    }

}

QHash<QString, SnapshotSaveMixerData::mixer_x_data> SnapshotSaveMixerData::getMixerDataList(int pattern, CfgType type)
{
    QHash<QString, mixer_x_data> list;
    switch (type) {
    case CFG_TYPE_MIXER_4X1:
        list = _mixer_4x1_DataList[pattern];
        break;
    case CFG_TYPE_MIXER_4X2:
        list = _mixer_4x2_DataList[pattern];
        break;
    case CFG_TYPE_MIXER_4X4:
        list = _mixer_4x4_DataList[pattern];
        break;
    case CFG_TYPE_MIXER_8X1:
        list = _mixer_8x1_DataList[pattern];
        break;
    case CFG_TYPE_MIXER_8X2:
        list = _mixer_8x2_DataList[pattern];
        break;
    case CFG_TYPE_MIXER_8X4:
        list = _mixer_8x4_DataList[pattern];
        break;
    case CFG_TYPE_MIXER_8X8:
        list = _mixer_8x8_DataList[pattern];
        break;
    case CFG_TYPE_MIXER_16X16:
        list = _mixer_16x16_DataList[pattern];
        break;
    case CFG_TYPE_MIXER_32X32:
        list = _mixer_32x32_DataList[pattern];
        break;
    case CFG_TYPE_MIXER_64X64:
        list = _mixer_64x64_DataList[pattern];
        break;
        break;
    default:
        break;
    }
    return list;
}

QString SnapshotSaveMixerData::getMixerInAndOutLab(int t, int idx, CfgType type, int pattern, QString devName)
{
    QString str;
    switch (type) {
    case CFG_TYPE_MIXER_4X1:
        str = QString(_mixer_4x1_DataList[pattern][devName]._inAndOutLabel[t][idx]);
        break;
    case CFG_TYPE_MIXER_4X2:
        str = QString(_mixer_4x2_DataList[pattern][devName]._inAndOutLabel[t][idx]);
        break;
    case CFG_TYPE_MIXER_4X4:
        str = QString(_mixer_4x4_DataList[pattern][devName]._inAndOutLabel[t][idx]);
        break;
    case CFG_TYPE_MIXER_8X1:
        str = QString(_mixer_8x1_DataList[pattern][devName]._inAndOutLabel[t][idx]);
        break;
    case CFG_TYPE_MIXER_8X2:
        str = QString(_mixer_8x2_DataList[pattern][devName]._inAndOutLabel[t][idx]);
        break;
    case CFG_TYPE_MIXER_8X4:
        str = QString(_mixer_8x4_DataList[pattern][devName]._inAndOutLabel[t][idx]);
        break;
    case CFG_TYPE_MIXER_8X8:
        str = QString(_mixer_8x8_DataList[pattern][devName]._inAndOutLabel[t][idx]);
        break;
    case CFG_TYPE_MIXER_16X16:
        str = QString(_mixer_16x16_DataList[pattern][devName]._inAndOutLabel[t][idx]);
        break;
    case CFG_TYPE_MIXER_32X32:
        str = QString(_mixer_32x32_DataList[pattern][devName]._inAndOutLabel[t][idx]);
        break;
    case CFG_TYPE_MIXER_64X64:
        str = QString(_mixer_64x64_DataList[pattern][devName]._inAndOutLabel[t][idx]);
        break;
    default:
        str = "No dev";
        break;
    }
//    qDebug()<<__FUNCTION__<<__LINE__<<str;
    return str;
}

GainMute SnapshotSaveMixerData::getInGainMute(int idx, CfgType type, int pattern, QString devName)
{
//    qDebug()<<__FUNCTION__<<__LINE__<<idx;//行
    GainMute val;
//    QStringList mixerList;
//    mixerList<<"mixer_4x1"<<"mixer_4x2"<<"mixer_4x4"
//                <<"mixer_8x1"<<"mixer_8x2"<<"mixer_8x4"<<"mixer_8x8"
//               <<"mixer_16x16"<<"mixer_32x32"<<"mixer_64x64";

    int z = idx / 2;
    int x = idx % 2;
//    qDebug()<<__FUNCTION__<<__LINE__<<mixerList.indexOf(type);
    switch(type) {
    case CFG_TYPE_MIXER_4X1:
        if (x)
        {
            val.gain = _mixer_4x1_DataList[pattern][devName].body4x1.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_4x1_DataList[pattern][devName].body4x1.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_4x1_DataList[pattern][devName].body4x1.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_4X2:
        if (x)
        {
            val.gain = _mixer_4x2_DataList[pattern][devName].body4x2.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_4x2_DataList[pattern][devName].body4x2.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_4x2_DataList[pattern][devName].body4x2.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_4X4:
        if (x)
        {
            val.gain = _mixer_4x4_DataList[pattern][devName].body4x4.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_4x4_DataList[pattern][devName].body4x4.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_4x4_DataList[pattern][devName].body4x4.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X1:
        if (x)
        {
            val.gain = _mixer_8x1_DataList[pattern][devName].body8x1.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_8x1_DataList[pattern][devName].body8x1.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_8x1_DataList[pattern][devName].body8x1.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X2:
        if (x)
        {
            val.gain = _mixer_8x2_DataList[pattern][devName].body8x2.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_8x2_DataList[pattern][devName].body8x2.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_8x2_DataList[pattern][devName].body8x2.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X4:
        if (x)
        {
            val.gain = _mixer_8x4_DataList[pattern][devName].body8x4.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_8x4_DataList[pattern][devName].body8x4.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_8x4_DataList[pattern][devName].body8x4.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X8:
        if (x)
        {
            val.gain = _mixer_8x8_DataList[pattern][devName].body8x8.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_8x8_DataList[pattern][devName].body8x8.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_8x8_DataList[pattern][devName].body8x8.inSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_16X16:
        if (x)
        {
            val.gain = _mixer_16x16_DataList[pattern][devName].body16x16.ingain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_16x16_DataList[pattern][devName].body16x16.ingain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_16x16_DataList[pattern][devName].body16x16.inSwitch >> idx) & 0x01;
        //qDebug()<<__FUNCTION__<<__LINE__<<((_body_16x16.inSwitch >> idx) & 0x01);
        break;
    case CFG_TYPE_MIXER_32X32:
        val.gain = _mixer_32x32_DataList[pattern][devName].body32x32._inAndOutGain[idx][IN_SELECT] * 0.01 - 100;
        val.mute = (_mixer_32x32_DataList[pattern][devName].body32x32.chSwIn[NODE_SW_L32] >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_64X64:
        val.gain = _mixer_64x64_DataList[pattern][devName].body64x64._inAndOutGain[idx][IN_SELECT] * 0.01 - 100;
        if (idx < 32)
        {
            val.mute = (_mixer_64x64_DataList[pattern][devName].body64x64.chSwIn[NODE_SW_L32] >> idx) & 0x01;
        } else {
            val.mute = (_mixer_64x64_DataList[pattern][devName].body64x64.chSwIn[NODE_SW_H32] >> (idx - 32)) & 0x01;
        }
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    return val;
}

qint8 SnapshotSaveMixerData::getRouterGain(int row, int col, CfgType type, int pattern, QString devName)
{
//    qDebug()<<__FUNCTION__<<__LINE__<<"row:"<<row<<" "<<"col:"<<col;//行列
//    QStringList mixerList;
//    mixerList<<"mixer_4x1"<<"mixer_4x2"<<"mixer_4x4"
//                <<"mixer_8x1"<<"mixer_8x2"<<"mixer_8x4"<<"mixer_8x8"
//               <<"mixer_16x16"<<"mixer_32x32"<<"mixer_64x64";

    int z = col / 2;
    int x = col % 2;

    switch(type) {
    case CFG_TYPE_MIXER_4X1:
        if (x)
        {
            return _mixer_4x1_DataList[pattern][devName].body4x1.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _mixer_4x1_DataList[pattern][devName].body4x1.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_4X2:
        if (x)
        {
            return _mixer_4x2_DataList[pattern][devName].body4x2.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _mixer_4x2_DataList[pattern][devName].body4x2.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_4X4:
        if (x)
        {
            return _mixer_4x4_DataList[pattern][devName].body4x4.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _mixer_4x4_DataList[pattern][devName].body4x4.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_8X1:
        if (x)
        {
            return _mixer_8x1_DataList[pattern][devName].body8x1.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _mixer_8x1_DataList[pattern][devName].body8x1.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_8X2:
        if (x)
        {
            return _mixer_8x2_DataList[pattern][devName].body8x2.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _mixer_8x2_DataList[pattern][devName].body8x2.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_8X4:
        if (x)
        {
            return _mixer_8x4_DataList[pattern][devName].body8x4.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _mixer_8x4_DataList[pattern][devName].body8x4.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_8X8:
        if (x)
        {
            return _mixer_8x8_DataList[pattern][devName].body8x8.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _mixer_8x8_DataList[pattern][devName].body8x8.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_16X16:
        if (x)
        {
            return _mixer_16x16_DataList[pattern][devName].body16x16.iToOGain[row][z].gainB * 0.01 - 100;
        } else {
            return _mixer_16x16_DataList[pattern][devName].body16x16.iToOGain[row][z].gainA * 0.01 - 100;
        }
    case CFG_TYPE_MIXER_32X32:
        return (_mixer_32x32_DataList[pattern][devName].body32x32._nodeGain[row][col] * 0.01 - 100);
    case CFG_TYPE_MIXER_64X64:
        return (_mixer_64x64_DataList[pattern][devName].body64x64._nodeGain[row][col] * 0.01 - 100);
    default:
        Q_ASSERT(false);
        break;
    }

    return 0;
}

quint32 SnapshotSaveMixerData::getRouterInfo(int idx, CfgType type, int pattern, QString devName)
{
//    qDebug()<<__FUNCTION__<<__LINE__<<idx;//行
//    QStringList mixerList;
//    mixerList<<"mixer_4x1"<<"mixer_4x2"<<"mixer_4x4"
//                <<"mixer_8x1"<<"mixer_8x2"<<"mixer_8x4"<<"mixer_8x8"
//               <<"mixer_16x16"<<"mixer_32x32"<<"mixer_64x64";

    int z = idx / 2;
    int x = idx % 2;

    switch(type) {
    case CFG_TYPE_MIXER_4X1:
        return _mixer_4x1_DataList[pattern][devName].body4x1.iToOSwitch[idx].sw;
    case CFG_TYPE_MIXER_4X2:
        if (x)
        {
            return _mixer_4x2_DataList[pattern][devName].body4x2.iToOSwitch[z].swB;
        } else {
            return _mixer_4x2_DataList[pattern][devName].body4x2.iToOSwitch[z].swA;
        }
    case CFG_TYPE_MIXER_4X4:
        if (x)
        {
            return _mixer_4x4_DataList[pattern][devName].body4x4.iToOSwitch[z].swB;
        } else {
            return _mixer_4x4_DataList[pattern][devName].body4x4.iToOSwitch[z].swA;
        }
    case CFG_TYPE_MIXER_8X1:
        return _mixer_8x1_DataList[pattern][devName].body8x1.iToOSwitch[idx].sw;
    case CFG_TYPE_MIXER_8X2:
        if (x)
        {
            return _mixer_8x2_DataList[pattern][devName].body8x2.iToOSwitch[z].swB;
        } else {
            return _mixer_8x2_DataList[pattern][devName].body8x2.iToOSwitch[z].swA;
        }
    case CFG_TYPE_MIXER_8X4:
        if (x)
        {
            return _mixer_8x4_DataList[pattern][devName].body8x4.iToOSwitch[z].swB;
        } else {
            return _mixer_8x4_DataList[pattern][devName].body8x4.iToOSwitch[z].swA;
        }
    case CFG_TYPE_MIXER_8X8:
        if (x)
        {
            return _mixer_8x8_DataList[pattern][devName].body8x8.iToOSwitch[z].swB;
        } else {
            return _mixer_8x8_DataList[pattern][devName].body8x8.iToOSwitch[z].swA;
        }
    case CFG_TYPE_MIXER_16X16:
        if (x)
        {
            return _mixer_16x16_DataList[pattern][devName].body16x16.iToOSwitch[z].swB;
        } else {
            return _mixer_16x16_DataList[pattern][devName].body16x16.iToOSwitch[z].swA;
        }
    case CFG_TYPE_MIXER_32X32:
        return _mixer_32x32_DataList[pattern][devName].body32x32._nodeSw[idx][NODE_SW_L32];
    case CFG_TYPE_MIXER_64X64:
        if (idx & 0x80)
        {
            return _mixer_64x64_DataList[pattern][devName].body64x64._nodeSw[(idx&0x7F)][NODE_SW_H32];
        } else {
            return _mixer_64x64_DataList[pattern][devName].body64x64._nodeSw[(idx&0x7F)][NODE_SW_L32];
        }
    default:
        Q_ASSERT(false);
        break;
    }

    return 0;
}

GainMute SnapshotSaveMixerData::getOutGainMute(int idx, CfgType type, int pattern, QString devName)
{
    GainMute val;
//    QStringList mixerList;
//    mixerList<<"mixer_4x1"<<"mixer_4x2"<<"mixer_4x4"
//                <<"mixer_8x1"<<"mixer_8x2"<<"mixer_8x4"<<"mixer_8x8"
//               <<"mixer_16x16"<<"mixer_32x32"<<"mixer_64x64";

    int z = idx / 2;
    int x = idx % 2;

    switch(type) {
    case CFG_TYPE_MIXER_4X1:
        val.gain = _mixer_4x1_DataList[pattern][devName].body4x1.outgain[idx].gain * 0.01 - 100;
        val.mute = (_mixer_4x1_DataList[pattern][devName].body4x1.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_4X2:
        if (x)
        {
            val.gain = _mixer_4x2_DataList[pattern][devName].body4x2.outgain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_4x2_DataList[pattern][devName].body4x2.outgain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_4x2_DataList[pattern][devName].body4x2.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_4X4:
        if (x)
        {
            val.gain = _mixer_4x4_DataList[pattern][devName].body4x4.outgain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_4x4_DataList[pattern][devName].body4x4.outgain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_4x4_DataList[pattern][devName].body4x4.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X1:
        val.gain = _mixer_8x1_DataList[pattern][devName].body8x1.outgain[idx].gain * 0.01 - 100;
        val.mute = (_mixer_8x1_DataList[pattern][devName].body8x1.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X2:
        if (x)
        {
            val.gain = _mixer_8x2_DataList[pattern][devName].body8x2.outgain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_8x2_DataList[pattern][devName].body8x2.outgain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_8x2_DataList[pattern][devName].body8x2.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X4:
        if (x)
        {
            val.gain = _mixer_8x4_DataList[pattern][devName].body8x4.outgain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_8x4_DataList[pattern][devName].body8x4.outgain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_8x4_DataList[pattern][devName].body8x4.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_8X8:
        if (x)
        {
            val.gain = _mixer_8x8_DataList[pattern][devName].body8x8.outgain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_8x8_DataList[pattern][devName].body8x8.outgain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_8x8_DataList[pattern][devName].body8x8.outSwitch >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_16X16:
        if (x)
        {
            val.gain = _mixer_16x16_DataList[pattern][devName].body16x16.outgain[z].gainB * 0.01 - 100;
        } else {
            val.gain = _mixer_16x16_DataList[pattern][devName].body16x16.outgain[z].gainA * 0.01 - 100;
        }
        val.mute = (_mixer_16x16_DataList[pattern][devName].body16x16.outSwitch >> idx) & 0x01;
        //qDebug()<<__FUNCTION__<<__LINE__<<_body_16x16.outSwitch<<idx<<((_body_16x16.outSwitch >> idx) & 0x01);
        break;
    case CFG_TYPE_MIXER_32X32:
        val.gain = _mixer_32x32_DataList[pattern][devName].body32x32._inAndOutGain[idx][OUT_SELECT] * 0.01 - 100;
        val.mute = (_mixer_32x32_DataList[pattern][devName].body32x32.chSwOut[NODE_SW_L32] >> idx) & 0x01;
        break;
    case CFG_TYPE_MIXER_64X64:
        val.gain = _mixer_64x64_DataList[pattern][devName].body64x64._inAndOutGain[idx][OUT_SELECT] * 0.01 - 100;
        if (idx < 32)
        {
            val.mute = (_mixer_64x64_DataList[pattern][devName].body64x64.chSwOut[NODE_SW_L32] >> idx) & 0x01;
        } else {
            val.mute = (_mixer_64x64_DataList[pattern][devName].body64x64.chSwOut[NODE_SW_H32] >> (idx - 32)) & 0x01;
        }
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    return val;
}

void SnapshotSaveMixerData::setMixerData(QString devName, CfgType type, int pattern, int inNum, int outNum, GainMute *inGainMute,
            GainMute *outGainMute, int (*routerGain)[64], quint32 (*routerInfo)[2], QString (*inAndOutLab)[64], quint32 moduleNum)
{
    if(type == CFG_TYPE_MIXER_4X1)
    {
        mixer_x_data body;

        for(int col = 0; col < inNum; col++)
        {
            int z = col / 2;
            int x = col % 2;
//            qDebug()<<__FUNCTION__<<__LINE__<<inGainMute[col].gain<<inGainMute[col].mute;
            if(x)
            {
                body.body4x1.ingain[z].gainB = (inGainMute[col].gain + 100) * 100;
            }else {
                body.body4x1.ingain[z].gainA = (inGainMute[col].gain + 100) * 100;
            }

            if(inGainMute[col].mute)
            {
                body.body4x1.inSwitch |= (1 << col);
            }else {
                body.body4x1.inSwitch &= ~(1 << col);
            }
        }

        for(int row = 0; row < outNum; row++)
        {
            for(int col = 0; col < inNum; col++)
            {
                int z = col / 2;
                int x = col % 2;
                if(x)
                {
                    body.body4x1.iToOGain[row][z].gainB = (routerGain[row][col] + 100) * 100;
                }else {
                    body.body4x1.iToOGain[row][z].gainA =(routerGain[row][col] + 100) * 100;
                }
            }

        }

        for(int row = 0; row < outNum; row++)
        {
//            qDebug()<<__FUNCTION__<<__LINE__<<routerInfo[row]<<outGainMute[row].gain<<outGainMute[row].mute;
            body.body4x1.iToOSwitch[row].sw = routerInfo[row][NODE_SW_L32];
            body.body4x1.outgain[row].gain = (outGainMute[row].gain + 100) * 100;
            if(outGainMute[row].mute)
            {
                body.body4x1.outSwitch |= (1 << row);
            }else {
                body.body4x1.outSwitch &= ~(1 << row);
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = inAndOutLab[i][j].toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
//                qDebug()<<__FUNCTION__<<__LINE__<<body._inAndOutLabel[i][j];
            }
        }
        _mixer_4x1_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
    }
    if(type == CFG_TYPE_MIXER_4X2)
    {
        mixer_x_data body;

        for(int col = 0; col < inNum; col++)
        {
            int z = col / 2;
            int x = col % 2;

            if(x)
            {
                body.body4x2.ingain[z].gainB = (inGainMute[col].gain + 100) * 100;
            }else {
                body.body4x2.ingain[z].gainA = (inGainMute[col].gain + 100) * 100;
            }

            if(inGainMute[col].mute)
            {
                body.body4x2.inSwitch |= (1 << col);
            }else {
                body.body4x2.inSwitch &= ~(1 << col);
            }
        }

        for(int row = 0; row < outNum; row++)
        {
            for(int col = 0; col < inNum; col++)
            {
                int z = col / 2;
                int x = col % 2;
                if(x)
                {
                    body.body4x2.iToOGain[row][z].gainB = (routerGain[row][col] + 100) * 100;
                }else {
                    body.body4x2.iToOGain[row][z].gainA =(routerGain[row][col] + 100) * 100;
                }
            }

        }

        for(int row = 0; row < outNum; row++)
        {
            int rz = row / 2;
            int rx = row % 2;
//            qDebug()<<__FUNCTION__<<__LINE__<<routerInfo[row]<<outGainMute[row].gain<<outGainMute[row].mute;

            if(rx)
            {
                body.body4x2.iToOSwitch[rz].swB = routerInfo[row][NODE_SW_L32];
                body.body4x2.outgain[rz].gainB = (outGainMute[row].gain + 100) * 100;
            }else {
                body.body4x2.iToOSwitch[rz].swA = routerInfo[row][NODE_SW_L32];
                body.body4x2.outgain[rz].gainA = (outGainMute[row].gain + 100) * 100;
            }

            if(outGainMute[row].mute)
            {
                body.body4x2.outSwitch |= (1 << row);
            }else {
                body.body4x2.outSwitch &= ~(1 << row);
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = inAndOutLab[i][j].toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
//                qDebug()<<__FUNCTION__<<__LINE__<<body._inAndOutLabel[i][j];
            }
        }
        _mixer_4x2_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
    }
    if(type == CFG_TYPE_MIXER_4X4)
    {
        mixer_x_data body;

        for(int col = 0; col < inNum; col++)
        {
            int z = col / 2;
            int x = col % 2;
//            qDebug()<<__FUNCTION__<<__LINE__<<inGainMute[col].gain<<inGainMute[col].mute;
            if(x)
            {
                body.body4x4.ingain[z].gainB = (inGainMute[col].gain + 100) * 100;
            }else {
                body.body4x4.ingain[z].gainA = (inGainMute[col].gain + 100) * 100;
            }

            if(inGainMute[col].mute)
            {
                body.body4x4.inSwitch |= (1 << col);
            }else {
                body.body4x4.inSwitch &= ~(1 << col);
            }
        }

        for(int row = 0; row < outNum; row++)
        {
            for(int col = 0; col < inNum; col++)
            {
                int z = col / 2;
                int x = col % 2;
                if(x)
                {
                    body.body4x4.iToOGain[row][z].gainB = (routerGain[row][col] + 100) * 100;
                }else {
                    body.body4x4.iToOGain[row][z].gainA =(routerGain[row][col] + 100) * 100;
                }
//                qDebug()<<__FUNCTION__<<__LINE__<<routerGain[row][col];
            }

        }

        for(int row = 0; row < outNum; row++)
        {
            int rz = row / 2;
            int rx = row % 2;
//            qDebug()<<__FUNCTION__<<__LINE__<<routerInfo[row]<<outGainMute[row].gain<<outGainMute[row].mute;

            if(rx)
            {
                body.body4x4.iToOSwitch[rz].swB = routerInfo[row][NODE_SW_L32];
                body.body4x4.outgain[rz].gainB = (outGainMute[row].gain + 100) * 100;
            }else {
                body.body4x4.iToOSwitch[rz].swA = routerInfo[row][NODE_SW_L32];
                body.body4x4.outgain[rz].gainA = (outGainMute[row].gain + 100) * 100;
            }
            if(outGainMute[row].mute)
            {
                body.body4x4.outSwitch |= (1 << row);
            }else {
                body.body4x4.outSwitch &= ~(1 << row);
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = inAndOutLab[i][j].toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
//                qDebug()<<__FUNCTION__<<__LINE__<<body._inAndOutLabel[i][j];
            }
        }
        _mixer_4x4_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
    }
    if(type == CFG_TYPE_MIXER_8X1)
    {
        mixer_x_data body;

        for(int col = 0; col < inNum; col++)
        {
            int z = col / 2;
            int x = col % 2;
//            qDebug()<<__FUNCTION__<<__LINE__<<inGainMute[col].gain<<inGainMute[col].mute;
            if(x)
            {
                body.body8x1.ingain[z].gainB = (inGainMute[col].gain + 100) * 100;
            }else {
                body.body8x1.ingain[z].gainA = (inGainMute[col].gain + 100) * 100;
            }

            if(inGainMute[col].mute)
            {
                body.body8x1.inSwitch |= (1 << col);
            }else {
                body.body8x1.inSwitch &= ~(1 << col);
            }
        }

        for(int row = 0; row < outNum; row++)
        {
            for(int col = 0; col < inNum; col++)
            {
                int z = col / 2;
                int x = col % 2;
                if(x)
                {
                    body.body8x1.iToOGain[row][z].gainB = (routerGain[row][col] + 100) * 100;
                }else {
                    body.body8x1.iToOGain[row][z].gainA =(routerGain[row][col] + 100) * 100;
                }
            }

        }

        for(int row = 0; row < outNum; row++)
        {
//            qDebug()<<__FUNCTION__<<__LINE__<<routerInfo[row]<<outGainMute[row].gain<<outGainMute[row].mute;

                body.body8x1.iToOSwitch[row].sw = routerInfo[row][NODE_SW_L32];
                body.body8x1.outgain[row].gain = (outGainMute[row].gain + 100) * 100;

            if(outGainMute[row].mute)
            {
                body.body8x1.outSwitch |= (1 << row);
            }else {
                body.body8x1.outSwitch &= ~(1 << row);
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = inAndOutLab[i][j].toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
//                qDebug()<<__FUNCTION__<<__LINE__<<body._inAndOutLabel[i][j];
            }
        }
        _mixer_8x1_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
    }
    if(type == CFG_TYPE_MIXER_8X2)
    {
        mixer_x_data body;

        for(int col = 0; col < inNum; col++)
        {
            int z = col / 2;
            int x = col % 2;
//            qDebug()<<__FUNCTION__<<__LINE__<<inGainMute[col].gain<<inGainMute[col].mute;
            if(x)
            {
                body.body8x2.ingain[z].gainB = (inGainMute[col].gain + 100) * 100;
            }else {
                body.body8x2.ingain[z].gainA = (inGainMute[col].gain + 100) * 100;
            }

            if(inGainMute[col].mute)
            {
                body.body8x2.inSwitch |= (1 << col);
            }else {
                body.body8x2.inSwitch &= ~(1 << col);
            }
        }

        for(int row = 0; row < outNum; row++)
        {
            for(int col = 0; col < inNum; col++)
            {
                int z = col / 2;
                int x = col % 2;
                if(x)
                {
                    body.body8x2.iToOGain[row][z].gainB = (routerGain[row][col] + 100) * 100;
                }else {
                    body.body8x2.iToOGain[row][z].gainA =(routerGain[row][col] + 100) * 100;
                }
            }

        }

        for(int row = 0; row < outNum; row++)
        {
            int rz = row / 2;
            int rx = row % 2;
//            qDebug()<<__FUNCTION__<<__LINE__<<routerInfo[row]<<outGainMute[row].gain<<outGainMute[row].mute;

            if(rx)
            {
                body.body8x2.iToOSwitch[rz].swB = routerInfo[row][NODE_SW_L32];
                body.body8x2.outgain[rz].gainB = (outGainMute[row].gain + 100) * 100;
            }else {
                body.body8x2.iToOSwitch[rz].swA = routerInfo[row][NODE_SW_L32];
                body.body8x2.outgain[rz].gainA = (outGainMute[row].gain + 100) * 100;
            }
            if(outGainMute[row].mute)
            {
                body.body8x2.outSwitch |= (1 << row);
            }else {
                body.body8x2.outSwitch &= ~(1 << row);
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = inAndOutLab[i][j].toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
//                qDebug()<<__FUNCTION__<<__LINE__<<body._inAndOutLabel[i][j];
            }
        }
        _mixer_8x2_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
    }
    if(type == CFG_TYPE_MIXER_8X4)
    {
        mixer_x_data body;

        for(int col = 0; col < inNum; col++)
        {
            int z = col / 2;
            int x = col % 2;
//            qDebug()<<__FUNCTION__<<__LINE__<<inGainMute[col].gain<<inGainMute[col].mute;
            if(x)
            {
                body.body8x4.ingain[z].gainB = (inGainMute[col].gain + 100) * 100;
            }else {
                body.body8x4.ingain[z].gainA = (inGainMute[col].gain + 100) * 100;
            }

            if(inGainMute[col].mute)
            {
                body.body8x4.inSwitch |= (1 << col);
            }else {
                body.body8x4.inSwitch &= ~(1 << col);
            }
        }

        for(int row = 0; row < outNum; row++)
        {
            for(int col = 0; col < inNum; col++)
            {
                int z = col / 2;
                int x = col % 2;
                if(x)
                {
                    body.body8x4.iToOGain[row][z].gainB = (routerGain[row][col] + 100) * 100;
                }else {
                    body.body8x4.iToOGain[row][z].gainA =(routerGain[row][col] + 100) * 100;
                }
            }

        }

        for(int row = 0; row < outNum; row++)
        {
            int rz = row / 2;
            int rx = row % 2;
//            qDebug()<<__FUNCTION__<<__LINE__<<routerInfo[row]<<outGainMute[row].gain<<outGainMute[row].mute;

            if(rx)
            {
                body.body8x4.iToOSwitch[rz].swB = routerInfo[row][NODE_SW_L32];
                body.body8x4.outgain[rz].gainB = (outGainMute[row].gain + 100) * 100;
            }else {
                body.body8x4.iToOSwitch[rz].swA = routerInfo[row][NODE_SW_L32];
                body.body8x4.outgain[rz].gainA = (outGainMute[row].gain + 100) * 100;
            }
            if(outGainMute[row].mute)
            {
                body.body8x4.outSwitch |= (1 << row);
            }else {
                body.body8x4.outSwitch &= ~(1 << row);
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = inAndOutLab[i][j].toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
//                qDebug()<<__FUNCTION__<<__LINE__<<body._inAndOutLabel[i][j];
            }
        }
        _mixer_8x4_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
    }
    if(type == CFG_TYPE_MIXER_8X8)
    {
        mixer_x_data body;

        for(int col = 0; col < inNum; col++)
        {
            int z = col / 2;
            int x = col % 2;
//            qDebug()<<__FUNCTION__<<__LINE__<<inGainMute[col].gain<<inGainMute[col].mute;
            if(x)
            {
                body.body8x8.ingain[z].gainB = (inGainMute[col].gain + 100) * 100;
            }else {
                body.body8x8.ingain[z].gainA = (inGainMute[col].gain + 100) * 100;
            }

            if(inGainMute[col].mute)
            {
                body.body8x8.inSwitch |= (1 << col);
            }else {
                body.body8x8.inSwitch &= ~(1 << col);
            }
        }

        for(int row = 0; row < outNum; row++)
        {
            for(int col = 0; col < inNum; col++)
            {
                int z = col / 2;
                int x = col % 2;
                if(x)
                {
                    body.body8x8.iToOGain[row][z].gainB = (routerGain[row][col] + 100) * 100;
                }else {
                    body.body8x8.iToOGain[row][z].gainA =(routerGain[row][col] + 100) * 100;
                }
            }

        }

        for(int row = 0; row < outNum; row++)
        {
            int rz = row / 2;
            int rx = row % 2;
//            qDebug()<<__FUNCTION__<<__LINE__<<routerInfo[row]<<outGainMute[row].gain<<outGainMute[row].mute;

            if(rx)
            {
                body.body8x8.iToOSwitch[rz].swB = routerInfo[row][NODE_SW_L32];
                body.body8x8.outgain[rz].gainB = (outGainMute[row].gain + 100) * 100;
            }else {
                body.body8x8.iToOSwitch[rz].swA = routerInfo[row][NODE_SW_L32];
                body.body8x8.outgain[rz].gainA = (outGainMute[row].gain + 100) * 100;
            }
            if(outGainMute[row].mute)
            {
                body.body8x8.outSwitch |= (1 << row);
            }else {
                body.body8x8.outSwitch &= ~(1 << row);
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = inAndOutLab[i][j].toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
//                qDebug()<<__FUNCTION__<<__LINE__<<body._inAndOutLabel[i][j];
            }
        }
        _mixer_8x8_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
    }
    if(type == CFG_TYPE_MIXER_16X16)
    {
        mixer_x_data body;

        for(int col = 0; col < inNum; col++)
        {
            int z = col / 2;
            int x = col % 2;
//            qDebug()<<__FUNCTION__<<__LINE__<<inGainMute[col].gain<<inGainMute[col].mute;
            if(x)
            {
                body.body16x16.ingain[z].gainB = (inGainMute[col].gain + 100) * 100;
            }else {
                body.body16x16.ingain[z].gainA = (inGainMute[col].gain + 100) * 100;
            }

            if(inGainMute[col].mute)
            {
                body.body16x16.inSwitch |= (1 << col);
            }else {
                body.body16x16.inSwitch &= ~(1 << col);
            }
        }

        for(int row = 0; row < outNum; row++)
        {
            for(int col = 0; col < inNum; col++)
            {
                int z = col / 2;
                int x = col % 2;
                if(x)
                {
                    body.body16x16.iToOGain[row][z].gainB = (routerGain[row][col] + 100) * 100;
//                    qDebug()<<__FUNCTION__<<__LINE__<<"B"<<row<<col<<(body.body16x16.iToOGain[row][z].gainB * 0.01 - 100)<<routerGain[row][col];
                }else {
                    body.body16x16.iToOGain[row][z].gainA =(routerGain[row][col] + 100) * 100;
//                    qDebug()<<__FUNCTION__<<__LINE__<<"A"<<row<<col<<(body.body16x16.iToOGain[row][z].gainA * 0.01 - 100)<<routerGain[row][col];
                }
            }
//            qDebug()<<__FUNCTION__<<__LINE__<<"----------------------------------------------";
        }

        for(int row = 0; row < outNum; row++)
        {
            int rz = row / 2;
            int rx = row % 2;
//            qDebug()<<__FUNCTION__<<__LINE__<<routerInfo[row]<<outGainMute[row].gain<<outGainMute[row].mute;

            if(rx)
            {
                body.body16x16.iToOSwitch[rz].swB = routerInfo[row][NODE_SW_L32];
                body.body16x16.outgain[rz].gainB = (outGainMute[row].gain + 100) * 100;
//                qDebug()<<__FUNCTION__<<__LINE__<<body.body16x16.iToOSwitch[rz].swB
//                       <<routerInfo[row][NODE_SW_L32];
            }else {
                body.body16x16.iToOSwitch[rz].swA = routerInfo[row][NODE_SW_L32];
                body.body16x16.outgain[rz].gainA = (outGainMute[row].gain + 100) * 100;
//                qDebug()<<__FUNCTION__<<__LINE__<<body.body16x16.iToOSwitch[rz].swA
//                       <<routerInfo[row][NODE_SW_L32];
            }
            if(outGainMute[row].mute)
            {
                body.body16x16.outSwitch |= (1 << row);
            }else {
                body.body16x16.outSwitch &= ~(1 << row);
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = inAndOutLab[i][j].toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
//                qDebug()<<__FUNCTION__<<__LINE__<<body._inAndOutLabel[i][j];
            }
        }
        _mixer_16x16_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
    }
    if(type == CFG_TYPE_MIXER_32X32)
    {
        mixer_x_data body;

        for(int col = 0; col < inNum; col++)
        {
//            qDebug()<<__FUNCTION__<<__LINE__<<inGainMute[col].gain<<inGainMute[col].mute;
            body.body32x32._inAndOutGain[col][IN_SELECT] = (inGainMute[col].gain + 100) * 100;

            if(inGainMute[col].mute)
            {
                body.body32x32.chSwIn[NODE_SW_L32] |= (1 << col);
            }else {
                body.body32x32.chSwIn[NODE_SW_L32] &= ~(1 << col);
            }
        }

        for(int row = 0; row < outNum; row++)
        {
            for(int col = 0; col < inNum; col++)
            {
                body.body32x32._nodeGain[row][col] = (routerGain[row][col] + 100) * 100;
            }

        }

        for(int row = 0; row < outNum; row++)
        {
//            qDebug()<<__FUNCTION__<<__LINE__<<routerInfo[row]<<outGainMute[row].gain<<outGainMute[row].mute;
             body.body32x32._nodeSw[row][NODE_SW_L32] = routerInfo[row][NODE_SW_L32];
            body.body32x32._inAndOutGain[row][OUT_SELECT] = (outGainMute[row].gain + 100) * 100;
            if(outGainMute[row].mute)
            {
                body.body32x32.chSwOut[NODE_SW_L32] |= (1 << row);
            }else {
                body.body32x32.chSwOut[NODE_SW_L32] &= ~(1 << row);
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = inAndOutLab[i][j].toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
//                qDebug()<<__FUNCTION__<<__LINE__<<body._inAndOutLabel[i][j];
            }
        }
        _mixer_32x32_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
    }
    if(type == CFG_TYPE_MIXER_64X64)
    {
        mixer_x_data body;

        for(int col = 0; col < inNum; col++)
        {
//            qDebug()<<__FUNCTION__<<__LINE__<<inGainMute[col].gain<<inGainMute[col].mute;
            body.body64x64._inAndOutGain[col][IN_SELECT] = (inGainMute[col].gain + 100) * 100;

            if(col < 32)
            {
                if(inGainMute[col].mute)
                {
                    body.body64x64.chSwIn[NODE_SW_L32] |= (1 << col);
                }else {
                    body.body64x64.chSwIn[NODE_SW_L32] &= ~(1 << col);
                }
            }
            if(col >= 32)
            {
                if(inGainMute[col].mute)
                {
                    body.body64x64.chSwIn[NODE_SW_H32] |= (1 << (col - 32));
                }else {
                    body.body64x64.chSwIn[NODE_SW_H32] &= ~(1 << (col - 32));
                }
            }

        }

        for(int row = 0; row < outNum; row++)
        {
            for(int col = 0; col < inNum; col++)
            {
                body.body64x64._nodeGain[row][col] = (routerGain[row][col] + 100) * 100;

                if(col < 32)
                {
                     body.body64x64._nodeSw[row][NODE_SW_L32] = routerInfo[row][NODE_SW_L32];
                }else {
                     body.body64x64._nodeSw[row][NODE_SW_H32] = routerInfo[row][NODE_SW_H32];
                }
            }

        }

        for(int row = 0; row < outNum; row++)
        {
//            qDebug()<<__FUNCTION__<<__LINE__<<routerInfo[row]<<outGainMute[row].gain<<outGainMute[row].mute;

            body.body64x64._inAndOutGain[row][OUT_SELECT] = (outGainMute[row].gain + 100) * 100;

            if(row < 32)
            {
                if(outGainMute[row].mute)
                {
                    body.body64x64.chSwOut[NODE_SW_L32] |= (1 << row);
                }else {
                    body.body64x64.chSwOut[NODE_SW_L32] &= ~(1 << row);
                }
            }else {
                if(outGainMute[row].mute)
                {
                    body.body64x64.chSwOut[NODE_SW_H32] |= (1 << (row - 32));
                }else {
                    body.body64x64.chSwOut[NODE_SW_H32] &= ~(1 << (row - 32));
                }
            }
        }

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 64; j++)
            {
                QByteArray array = inAndOutLab[i][j].toUtf8();
                body._inAndOutLabel[i][j] = (char *)malloc(array.length() + 1);
                memset(body._inAndOutLabel[i][j], 0, sizeof(array.length()));
                memcpy(body._inAndOutLabel[i][j], array, array.length());
                body._inAndOutLabel[i][j][array.length()] = '\0';
//                qDebug()<<__FUNCTION__<<__LINE__<<body._inAndOutLabel[i][j];
            }
        }
        _mixer_64x64_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
    }
}

