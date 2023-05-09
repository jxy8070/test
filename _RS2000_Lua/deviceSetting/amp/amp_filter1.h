#ifndef AMPFILTER1_H
#define AMPFILTER1_H

#include "customWidget/overwriteitem.h"
#include "ui_amp_filter1.h"
#include "protocal/packet.h"
namespace Ui {
class AmpFilter1;
}

// 分频界面
class AmpFilter1 : public DevSettingDlg
{
    Q_OBJECT
public:
    AmpFilter1(quint8 channel, DividerData *dividerData, QWidget *parent = 0);

signals:
    void pfDataChanged(quint8);

public slots:
    void onDataChanged();

private:
    enum
    {
        LINK_RILEY = 0,
        BUTTER,
        BESSEL,
        FIR_BLACKMAN,
    };

    Ui::AmpFilter1 *_ui;
    DividerData *_dividerData;
    quint8 _channel;
};

#endif // AMPPF_H
