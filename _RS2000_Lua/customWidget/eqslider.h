#ifndef EQSLIDER_H
#define EQSLIDER_H

#include <QWidget>
class QVBoxLayout;
class QLabel;
class QSlider;
class QLineEdit;
class QPushButton;
class EqSlider : public QWidget
{
    Q_OBJECT

public:
    EqSlider(int idx, int fx, int gain, bool bypass, bool isShowTick, QWidget *parent = 0);
    void setupUi();
    int gainValue();
    bool getBypass(void);

    void setGainValue(int value);
    void setCurrent(bool isCurrent);
    void setBypass(bool enable);
private:
    void paintEvent(QPaintEvent *e);

signals:
    void gainChanged(int idx, int vlaue);
    void bypassClicked(int, bool);

private slots:
    void onGainChanged();
    void onBypassClicked(bool enable);

private:
    bool _isShowTick;
    int _idx;
    QString _fxString;
    bool _isSelected;

    int _lastGainInt =0;
    QVBoxLayout *_layout;
    QLabel *_upLabel;
    QPushButton *_bypass;
    QSlider *_gainSlider;
    QLabel *_downLabel;
    QLineEdit *_gainEdit;
};

#endif // EQSLIDER_H
