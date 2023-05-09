#ifndef BASE_2_2_MIXER_H
#define BASE_2_2_MIXER_H

#include "protocal/packet.h"
#include "customWidget/overwriteitem.h"

namespace Ui {
class base_2_2_mixer;
}

class base_2_2_mixer : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit base_2_2_mixer(QStringList in,QStringList out,ConfigData_RT1052_Mixer_2_2 mixer,QWidget *parent = 0);
    ~base_2_2_mixer();

    void set_Enabled();

signals:
    void mixerchanged(int,int,bool);
private slots:
    void mixer_clicked(bool);
private:
    Ui::base_2_2_mixer *ui;
};

#endif // BASE_2_2_MIXER_H
