#ifndef TRN_METERWIDGET_H
#define TRN_METERWIDGET_H

#include "customWidget/overwriteitem.h"
#include <QLabel>


class VolumeWidget;
namespace Ui {
class Trn_MeterWidget;
}
class Trn_MeterWidget : public DevSettingWidget
{
    Q_OBJECT

public:
    explicit Trn_MeterWidget(bool isInput, QWidget *parent = 0);
    ~Trn_MeterWidget();
public:
    void dispMeter(quint16 *vlu);

private:
    Ui::Trn_MeterWidget *_ui;
    QList<VolumeWidget *> m_dispList;
    QList<QLabel *> m_dispLabelList;
};

#endif // TRN_METERWIDGET_H
