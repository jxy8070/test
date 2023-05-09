#ifndef AMP_MIC_QUAD_XOVER_H
#define AMP_MIC_QUAD_XOVER_H

#include "customWidget/overwriteitem.h"

namespace Ui {
class Amp_Mic_Quad_Xover;
}

class Device_Mic_Quad;
class Amp_Mic_Quad_Xover : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit Amp_Mic_Quad_Xover(Device_Mic_Quad *dev, int ch, QWidget *parent = 0);
    ~Amp_Mic_Quad_Xover();

private slots:
    void pb_clicked(bool enable);
    void valueChanged(int value);
    void currentIndexChanged(int value);

private:
    Ui::Amp_Mic_Quad_Xover *ui;
    Device_Mic_Quad *_dev;
    int _ch;
};

#endif // AMP_MIC_QUAD_XOVER_H
