#ifndef CONFIGDELAYNEWDLY_H
#define CONFIGDELAYNEWDLY_H

#include <QDialog>
#include "configmixergainwidget.h"
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>

namespace Ui {
class configDelayNewDly;
}

class configDelayNewDly : public QDialog
{
    Q_OBJECT
public:
    enum{
        NewDlyType_Defaut = 0,
        NewDlyType_Mixer,
    };

public:
    explicit configDelayNewDly(QWidget *parent = nullptr, int type = NewDlyType_Defaut);
    ~configDelayNewDly();


    QDoubleSpinBox *getDoubleSpinBox();
    pieGain *getPieGain();
    void setMaximum(double max);
    void setMinimum(double min);
    void setChecked(bool status);
    inline void setCheckable(bool ch) { _isCheckable = ch; }
    bool isChecked();

protected:
    void resizeEvent(QResizeEvent *) override;
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void enterEvent(QEvent *ev) override;
    void leaveEvent(QEvent *ev) override;

signals:
    void sendMouseClickedSignal(bool status);
    void clicked();
    void gainValueChanged(int);
private slots:
    void onRouterGainChanged(double value);
private:
    Ui::configDelayNewDly *ui;

    double _value;
    bool _isCheckable;
    int _NewDlyType;
};

#endif // CONFIGDELAYNEWDLY_H
