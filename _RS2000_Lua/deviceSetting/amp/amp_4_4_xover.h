#ifndef AMP_4_4_XOVER_H
#define AMP_4_4_XOVER_H

#include "customWidget/overwriteitem.h"

namespace Ui {
class Amp_4_4_xover;
}
class Device_Amp_Quad;
class Device_Amp_Module;
class Amp_4_4_xover : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit Amp_4_4_xover(Device_Amp_Quad *devAmp, quint8 ch, QWidget *parent = 0);
    explicit Amp_4_4_xover(Device_Amp_Module *devAmp, quint8 ch, QWidget *parent = 0);
    ~Amp_4_4_xover();

private slots:
    void pb_clicked(bool enable);
    void valueChanged(int value);
    void currentIndexChanged(int value);

private:
    Ui::Amp_4_4_xover *ui;
    Device_Amp_Quad *_devAmp;
    Device_Amp_Module *_devAmpModule;
    quint8 _ch;
};

#endif // AMP_4_4_XOVER_H
