#ifndef TRN_GAINWIDGET_H
#define TRN_GAINWIDGET_H

#include "customWidget/overwriteitem.h"
#include <QPushButton>

namespace Ui {
class Trn_GainWidget;
}
class Trn_GainWidget : public DevSettingWidget
{
    Q_OBJECT
    enum {TOTAL_INPUT = 8};
public:
    explicit Trn_GainWidget(QWidget *parent = 0);
    ~Trn_GainWidget();
    void showWidget(qint8 *inMicGain);

private slots:
    void onGainSelected();

signals:
    void gainChanged(quint8, qint8);

private:
    Ui::Trn_GainWidget *_ui;
};

#endif // TRN_GAINWIDGET_H
