#ifndef AMP_0_2_LINE_MIXER_H
#define AMP_0_2_LINE_MIXER_H


#include "customWidget/overwriteitem.h"

namespace Ui {
class Amp_0_2_Line_Mixer;
}

class Device_Amp_0_2_Line;
class Amp_0_2_Line_Mixer : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit Amp_0_2_Line_Mixer(Device_Amp_0_2_Line *devAmp, QWidget *parent = 0);
    ~Amp_0_2_Line_Mixer();

private slots:
    void pb_clicked(bool enable);

private:
    Ui::Amp_0_2_Line_Mixer *ui;
    Device_Amp_0_2_Line *_devAmp;
};

#endif // AMP_0_2_LINE_MIXER_H
