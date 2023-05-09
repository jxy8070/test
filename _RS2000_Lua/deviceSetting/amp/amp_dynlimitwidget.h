#ifndef AMP_DYNLIMITWIDGET_H
#define AMP_DYNLIMITWIDGET_H

#include "customWidget/overwriteitem.h"
#include "protocal/packet.h"

namespace Ui {
class amp_dynLimitWidget;
}

class amp_dynLimitWidget : public DevSettingWidget
{
    Q_OBJECT

public:
    explicit amp_dynLimitWidget(QWidget *parent = 0);
    ~amp_dynLimitWidget();

    void showWidget(bool isIn, quint8 ch, QString chName, DynData &dynData);

signals:
    void dynChanged(quint8, bool, DynData);

private slots:
    void processSilderSlot(int value);
    void processLineEditSlot();

private:
    void getDispData();

    Ui::amp_dynLimitWidget *ui;
    int _xValue[4], _yValue[4];
    DynData _dyndata;
};

#endif // AMP_DYNLIMITWIDGET_H
