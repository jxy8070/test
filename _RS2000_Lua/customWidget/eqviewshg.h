#ifndef EQVIEWSHG_H
#define EQVIEWSHG_H

#include <QWidget>
#include <QList>

class QGridLayout;
class EqGraph;
class EqSlider;
class QPushButton;
class QLabel;
struct EqData;
class EqViewSHG: public QWidget
{
    Q_OBJECT
public:
    enum SHG_Type{SHG_15 = 16, SHG_3X3 = 10, SHG_2X3 = 15, SHG_1X3 = 31};
    EqViewSHG(SHG_Type type, bool bypass, QList<EqData *> dataList, QWidget *parent = 0);
    void reinitEqData(bool bypass, QList<EqData *> dataList);
    void setSetBypass(int idx, bool enable);
    void connectSig(void);
    void disconnectSig(void);

signals:
    void eqDataChanged(quint8, int);
    void eqDataSegBypassClicked(int, bool);
    void eqDataBypassClicked(bool);
    void eqDataReset(void);

private slots:
    void onValueChanged(int idx, int fs, int gain, int q);
    void onGainChanged(int idx, int gain);
    void onSegBypassChanged(int idx, bool enable);
    void onSetBypass(bool isBypass);
    void onResetClicked(void);

private:
    void setupUi();
    void setupUi_FangTu();

    QPushButton *_bypassBtn;
    QPushButton *_reset;
    SHG_Type _type;
    QList<EqSlider*> _eqSliderList;
    EqGraph* _eqGraph;
};

#endif // EQVIEWSHG_H
