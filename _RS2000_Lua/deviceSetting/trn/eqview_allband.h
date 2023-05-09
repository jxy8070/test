#ifndef TRN_EQVIEW3B_H
#define TRN_EQVIEW3B_H

#include "customWidget/overwriteitem.h"
#include <QList>
#include <QResizeEvent>

class ConfigGainWidget;
class QVBoxLayout;
class EqGraph;
class QPushButton;
class QLabel;
struct EqPara;
class QDoubleSpinBox;
class QComboBox;
struct EqData;
class EqView_AllBand: public DevSettingWidget
{
    Q_OBJECT
    struct BandSpinBox{
        ConfigGainWidget *freqSpin;
        ConfigGainWidget *gainSpin;
        ConfigGainWidget *qSpin;
    };
public:
    EqView_AllBand(int paraLen, QWidget *parent = 0);
    void emitEqDataChanged();
    void showWidget(int channel, QString chName, bool bypass, EqData* eqParas);

signals:
    void eqDataChanged(quint8, bool, EqData* eqParas);

public slots:
    void onEqParaChanged();
    void onValueChanged(int idx, int fs, int gain, int q);
    void onSetBypass(bool isBypass);
private:
    void connectSig(void);
    void disconnectSig(void);
    void setupUi();

    void initEqParaUi();

private:
    int _eqDataLen;
    int _channel;
    EqGraph* _eqGraph;

    EqData* _eqArray;
    QList<BandSpinBox *> _spinBoxList;

    QPushButton *_bypassBtn;
    QLabel * _channelName;
    QVBoxLayout* _layout;
};


#endif // TRN_EQVIEW3B_H
