#ifndef AMP_4_4_MIXER_H
#define AMP_4_4_MIXER_H

#include "customWidget/overwriteitem.h"

namespace Ui {
class Amp_4_4_Mixer;
}

class Device_Amp_Quad;
class Device_Amp_Module;
class Amp_4_4_Mixer : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit Amp_4_4_Mixer(Device_Amp_Quad *devAmp, QWidget *parent = 0);
    explicit Amp_4_4_Mixer(Device_Amp_Module *devAmp, QWidget *parent = 0);
    ~Amp_4_4_Mixer();

private slots:
    void pb_clicked(bool enable);

private:
    Ui::Amp_4_4_Mixer *ui;
    Device_Amp_Quad *_devAmp;
    Device_Amp_Module *_devAmpModule;
};

#endif // AMP_4_4_MIXER_H
