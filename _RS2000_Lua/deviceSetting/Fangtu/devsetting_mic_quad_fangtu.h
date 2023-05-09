#ifndef DEVSETTING_MIC_QUAD_FANGTU_H
#define DEVSETTING_MIC_QUAD_FANGTU_H

#include "customWidget/overwriteitem.h"
#include <QDialog>
#include "Protocal/packet.h"


class AlarmConfig;
class Device_Mic_Quad;
class DeviceItem_Mic_Quad;

namespace Ui {
class DevSetting_Mic_Quad_FangTu;
}

class DevSetting_Mic_Quad_FangTu : public DevSettingDlg
{
    Q_OBJECT
public:
    explicit DevSetting_Mic_Quad_FangTu(DeviceItem_Mic_Quad *devItem, QWidget *parent = nullptr);
    ~DevSetting_Mic_Quad_FangTu();

private slots:
    void displayConfigValue();

    void MicGainChanged(int value);
    void MicPhantomChanged(bool enable);
    void GainChanged(int value);
    void setMute(bool enable);

    void xoverClicked(void);

    void DynClicked(void);
    void onDynChanged(quint8 ch, bool isIn, DynData dynData);

    void EqClicked(void);
    void onEqChanged(quint8 ch, bool bypass, EqData *eqPara);

    void OutPhaseClicked(bool enable);
    void MixerClicked(void);
    void MixerChangedSlot(int out, int in, bool enable);
    void OutGainChanged(int value);

    void DisplaySlot(MsgData_Mic_Quad b);

    void on_save_clicked();

private:
    void connectSigSlots();

private:
    Ui::DevSetting_Mic_Quad_FangTu *ui;
    DeviceItem_Mic_Quad *_devItem;
    Device_Mic_Quad *_dev;

    bool _syncing;
    bool _hasData;
    QTimer *_timer;

};

#endif // DEVSETTING_MIC_QUAD_FANGTU_H
