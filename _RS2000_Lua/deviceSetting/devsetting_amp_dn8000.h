#ifndef DEVSETTING_AMP_DN8000_H
#define DEVSETTING_AMP_DN8000_H

#include "customWidget/overwriteitem.h"
#include <QDialog>

class DeviceItem_Amp_DN8000;
class Device_Amp_DN8000;
struct ChState;
enum DevStatus;
namespace Ui {
class DevSetting_Amp_DN8000;
}
class DevSetting_Amp_DN8000 : public DevSettingDlg
{
    Q_OBJECT
    enum {ChannelCount = 8};
public:
    explicit DevSetting_Amp_DN8000(DeviceItem_Amp_DN8000 *devItem, QWidget *parent = nullptr);
    ~DevSetting_Amp_DN8000();

public slots:
    void onDeviceStateChanged(DevStatus devStatus);
    void onChannelStateChanged(ChState* pData);
    void onAmpParamChanged();
    void onChannelMuteClick(bool checked = false);

private:
    void initChannelUi();
    void sendInitPackets();

private:
    Ui::DevSetting_Amp_DN8000 *ui;
    DeviceItem_Amp_DN8000 *_devItem;
    Device_Amp_DN8000 *_devAmp;
    QList<QPushButton*> _muteBtnList;
    QList<QPushButton*> _faultBtnList;
    QList<QPushButton*> _clipBtnList;
    QList<QPushButton*> _sigBtnList;    
    QList<QPushButton*> _protectBtnList;
    QList<QLineEdit*> _tempEditList;
    QList<QLineEdit*> _valEditList;
    QList<QLineEdit*> _elecEditList;

};

#endif // DEVSETTING_AMP_DN8000_H
