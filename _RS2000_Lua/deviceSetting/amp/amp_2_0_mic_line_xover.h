#ifndef AMP_2_0_MIC_LINE_XOVER_H
#define AMP_2_0_MIC_LINE_XOVER_H

#include "customWidget/overwriteitem.h"

namespace Ui {
class Amp_2_0_Mic_Line_xover;
}
class Device_Amp_2_0_Mic_Line;
class Amp_2_0_Mic_Line_xover : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit Amp_2_0_Mic_Line_xover(Device_Amp_2_0_Mic_Line *devAmp, quint8 ch, QWidget *parent = 0);
    ~Amp_2_0_Mic_Line_xover();

private slots:
    void pb_clicked(bool enable);
    void valueChanged(int value);
    void currentIndexChanged(int value);

private:
    Ui::Amp_2_0_Mic_Line_xover *ui;
    Device_Amp_2_0_Mic_Line *_devAmp;
    quint8 _ch;
};

#endif // AMP_2_0_MIC_LINE_XOVER_H
