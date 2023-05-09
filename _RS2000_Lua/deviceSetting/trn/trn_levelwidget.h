#ifndef TRN_LEVELWIDGET_H
#define TRN_LEVELWIDGET_H

#include "customWidget/overwriteitem.h"

namespace Ui {
class Trn_LevelWidget;
}
class Trn_LevelWidget : public DevSettingWidget
{
    Q_OBJECT

public:
    explicit Trn_LevelWidget(bool isInput, QWidget *parent = 0);
    ~Trn_LevelWidget();
    void showWidget(quint32 *CurrentVluList);

private slots:
    void spinBoxValueChanged(double vlu);
    void sliderValueChanged(int vlu);

signals:
    void levelChanged(quint8, float);

private:
    Ui::Trn_LevelWidget *_ui;
    bool _isInit;
};

#endif // TRN_LEVELWIDGET_H
