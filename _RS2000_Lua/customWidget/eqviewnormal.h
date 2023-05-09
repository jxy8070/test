#ifndef EQVIEWNORMAL_H
#define EQVIEWNORMAL_H

#include <QWidget>
#include <QList>


enum EqType;
class QGridLayout;
class EqGraph;
class QPushButton;
class QLabel;
struct EqPara;
class EqviewNormalSeg;
struct EqData;

class EqViewNormal: public QWidget
{
    Q_OBJECT
public:
    EqViewNormal(bool bypass, QList<EqData *> dataList, QWidget *parent = 0);
//    void emitEqDataChanged();
//    void showWidget(bool bypass, QList<EqData *> eqdata);
    void reinitEqData(bool bypass, QList<EqData *> dataList);
    void showWidget(void);
    void setSegBypass(uint32 idx, bool bypass);

signals:
//    void eqDataChanged(quint8, bool, EqData* eqDataArray);
    void eqDataChanged(quint8, bool, int, int, int, int);
    void eqDataBypassClicked(bool);
    void eqDataReset(void);

private slots:
    void onValueChanged(int idx, int fs, int gain, int q, EqType type);
    void onSetBypass(bool isBypass);
    void onResetClicked(void);
    void onSelectSegChanged(int idx);
    void SegBypass_Clicked(bool enable);
    void Seg_ValueChanged(void);

private:
    void setupUi_FangTu();
    void initEqParaUi();
    void connectSignal();
    void disconnectSignal();

    EqType getGraphEqType(int type);
    int getDataEqType(EqType type);
private:
    int _channel;
    EqGraph* _eqGraph;
    QList<EqviewNormalSeg *> _SegList;

    QPushButton *_bypassBtn;
    QPushButton *_reset;
    QLabel * _channelName;
    QGridLayout* _layout;
};


#endif // EQVIEWNORMAL_H
