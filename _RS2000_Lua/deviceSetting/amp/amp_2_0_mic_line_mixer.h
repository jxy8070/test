#ifndef AMP_2_0_MIC_LINE_MIXER_H
#define AMP_2_0_MIC_LINE_MIXER_H

#include "customWidget/overwriteitem.h"

namespace Ui {
class Amp_2_0_Mic_Line_Mixer;
}

class Device_Amp_2_0_Mic_Line;
class Amp_2_0_Mic_Line_Mixer : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit Amp_2_0_Mic_Line_Mixer(Device_Amp_2_0_Mic_Line *devAmp, QWidget *parent = 0);
    ~Amp_2_0_Mic_Line_Mixer();

private slots:
    void pb_clicked(bool enable);

private:
    Ui::Amp_2_0_Mic_Line_Mixer *ui;
    Device_Amp_2_0_Mic_Line *_devAmp;
};

#endif // AMP_2_0_MIC_LINE_MIXER_H
