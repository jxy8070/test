#ifndef EQVIEWNORMALSEG_H
#define EQVIEWNORMALSEG_H

#include <QWidget>

namespace Ui {
class EqviewNormalSeg;
}

class EqviewNormalSeg : public QWidget
{
    Q_OBJECT

public:
    explicit EqviewNormalSeg(QWidget *parent = 0);
    ~EqviewNormalSeg();
    void select(void);
    void unselect(void);
    void Set_Title(QString title);
    void Set_Bypass(bool enable);
    void Set_Frequency(int value);
    void Set_Gain(double value);
    void Set_Bandwidth(double value);
    void set_eqType(int type);

    bool Get_bypass(void);
    int Get_Frequency(void);
    double Get_Gain(void);
    double Get_Bandwidth(void);
    int get_eqType(void);

signals:
    void FreqGainBandwidth_ValueChanged(void);
    void Bypass_Clicked(bool);

private slots:
    void on_pushButton_clicked(bool checked);
    void onSpinBoxFreq_valueChanged(int arg1);
    void onSpinBoxGain_valueChanged(double arg1);
    void onSpinBoxQ_valueChanged(double arg1);
    void on_comboBox_EqType_currentIndexChanged(int index);

private:
    Ui::EqviewNormalSeg *ui;
};

#endif // EQVIEWNORMALSEG_H
