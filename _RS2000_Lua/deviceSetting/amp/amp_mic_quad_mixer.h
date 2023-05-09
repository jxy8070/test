#ifndef AMP_MIC_QUAD_MIXER_H
#define AMP_MIC_QUAD_MIXER_H

#include "customWidget/overwriteitem.h"
#include "protocal/packet.h"

namespace Ui {
class Amp_Mic_Quad_Mixer;
}

class Mixer_4_4 : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit Mixer_4_4(QStringList in, QStringList out, ConfigData_RT1052_Mixer_4_4 mixer, QWidget *parent = 0);
    ~Mixer_4_4();

private slots:
    void pb_clicked(bool enable);

signals:
    void mixerChanged(int, int, bool);

private:
    Ui::Amp_Mic_Quad_Mixer *ui;
};

#endif // AMP_MIC_QUAD_MIXER_H
