#ifndef CFGSETTINGNFC_H
#define CFGSETTINGNFC_H

#include <QLabel>
namespace Ui {
  class  CfgSettingNFC;
  class CfgSettingNFC_FangTu;
}
class ConfigItemNFC;

class CfgSettingNFC : public QWidget
{
    Q_OBJECT
public:
    CfgSettingNFC(ConfigItemNFC *cfgItem,  QWidget *parent = 0);
    void setDisp(int idx, int howFreq, int notchDepth);

    void updateUiData();


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void ValueChangedSlot(int d);
    void onOutModePbClicked(void);

private:
    void initUi(void);
    void initUi_FangTu(void);
private:
    Ui::CfgSettingNFC *_uiNFC = NULL;
    Ui::CfgSettingNFC_FangTu * _ui_FangTu = NULL;
    ConfigItemNFC *_cfgItem;
    int _maxFilter;

    QList<QLabel *> _howlFreq;
    QList<QLabel *> _NotchDepth;
};

#endif // CFGSETTINGNFC_H
