#ifndef CFGSETTINGSPLITE_H
#define CFGSETTINGSPLITE_H

#include <QPushButton>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>

namespace Ui {
class CfgSettingSplite;
class CfgSettingSplite_FangTu;
}
class ConfigItemSplit;

enum {
    LB_FREQ = 0,
    LB_FILTER_TYPE,
    LB_OUT_LEVEL,
    LB_MUTE,
    LB_INVERT,
    LB_COUNT,
};

const QString LB_FILTERTYPE_STR[12] = {
    "L-R 12", "L-R 24", "L-R 36", "L-R 48",
    "BW 6", "BW 12", "BW 18", "BW 24", "BW 30", "BW 36", "BW 42", "BW 48",
};
class CfgSettingSplite : public QWidget
{
    Q_OBJECT
public:
    CfgSettingSplite(ConfigItemSplit *cfgItem, QWidget *parent = 0);
    ~CfgSettingSplite();
    void updateUiData();
signals:

public slots:

private slots:
    void pointChange(int idx);
    void pointMove(int idx, float fs);
    void processPushButtonSlot(bool flag);
    void processDoubleSpinBoxSlot(double value);
    void processComboBoxSlot(int idx);

    void pointChange_FangTu(int idx);
    void pointMove_FangTu(int idx, float fs);
    void processPushButtonSlot_FangTu(bool flag);
    void processDoubleSpinBoxSlot_FangTu(double value);
    void processComboBoxSlot_FangTu(int idx);
private:
    void initUi();
    void initData();
    void initSignalConnect();
    void initCurrentDisplay();
    void disableSignalConnect();

    void initUi_FangTu();
    void initData_FangTu();
    void initSignalConnect_FangTu();
    void initCurrentDisplay_FangTu();
    void disableSignalConnect_FangTu();


private:
#define CURIDX_NEXT (_curIdx + 1)
#define CURIDX_PRE  (_curIdx - 1)
    Ui::CfgSettingSplite *_uiSplite = NULL;
    Ui::CfgSettingSplite_FangTu *_ui_FangTu = NULL;
    ConfigItemSplit *_cfgItem;
    QList<QLabel *> _displayList;
    int _curIdx;

};

#endif // CFGSETTINGSPLITE_H
