#ifndef TRN_DELAYWIDGET_H
#define TRN_DELAYWIDGET_H

#include "customWidget/overwriteitem.h"

namespace Ui {
class Trn_DelayWidget;
}
class Trn_DelayWidget : public DevSettingWidget
{
    Q_OBJECT

public:
    explicit Trn_DelayWidget(bool isInput, QWidget *parent = 0);
    ~Trn_DelayWidget();
    void showWidget(quint16 *delayValueList);

private slots:
    void spinBoxValueChanged(int value);

signals:
    void delayChanged(quint8, quint16);

private:
    void connectSig();
    void disconnectSig();

private:
    Ui::Trn_DelayWidget *_ui;
};

#endif // TRN_DELAYWIDGET_H
