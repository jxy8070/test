#ifndef FILTERVIEW_H
#define FILTERVIEW_H

#include <QWidget>
#include "protocal/packet.h"
class EqGraph;
struct EqPara;
class QComboBox;
class QDoubleSpinBox;
class QPushButton;
class QLabel;
class FilterView : public QWidget
{
    Q_OBJECT
public:
    explicit FilterView(QWidget *parent = 0);
    void showWidget(int channel, QString chName, SplitData data);
signals:
    void eqDataChanged(quint8 channel, SplitData data);
public slots:
    void onReset();
    void onSetBypass();
    void onFrequencyChanged();
    void onValueChanged(int idx, int fx, int gain, int q);
private:
    void emitEqDataChanged();
    void setupUi();
    void connectSignal();
    void disconnectSignal();

private:
     int _channel;
     QLabel * _channelName;
     QList<EqPara *> _paraList;
     EqGraph *_eqGraph;
     QComboBox *_typeComb;
     QDoubleSpinBox *_freqSpinHP;
     QPushButton *_bypassBtnHP;
     QDoubleSpinBox *_freqSpinLP;
     QPushButton *_bypassBtnLP;
     QPushButton *_resetBtn;


};

#endif // FILTERVIEW_H
