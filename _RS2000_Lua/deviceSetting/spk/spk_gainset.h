#ifndef GAINSET_H
#define GAINSET_H

#include "customWidget/overwriteitem.h"
#include "ui_spk_gainset.h"

class GainSet : public DevSettingDlg, private Ui::Spk_GainSet
{
    Q_OBJECT
public:
    explicit GainSet(QWidget *parent = 0);
    QSlider* getGainSlider()
    {
        return gainSlider;
    }
    bool isPhase()
    {
        return PBtn->isChecked();
    }
    bool isMute()
    {
        return MBtn->isChecked();
    }

signals:

public slots:

private slots:
    void on_closeBtn_clicked();
};

#endif // GAINSET_H
