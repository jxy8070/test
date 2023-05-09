#ifndef AMPPF_H
#define AMPPF_H

#include "customWidget/overwriteitem.h"


enum AmpPfTypeEnum {
    AmpPfType_SPK = 0,
    AmpPfType_AMP,
};

namespace Ui {
class Spk_AmpPf;
}
struct DividerData;
// 分频界面
class AmpPf : public DevSettingDlg
{
    Q_OBJECT
public:
    AmpPf(quint8 channel, DividerData *dividerData, AmpPfTypeEnum type, QWidget *parent = 0);
    void setViewMode(bool isView);

signals:
    void pfDataChanged(quint8);
public slots:
    void onDataChanged();

private:
    Ui::Spk_AmpPf *_ui;
    DividerData *_dividerData;
    quint8 _channel;
    AmpPfTypeEnum _type;
};

#endif // AMPPF_H
