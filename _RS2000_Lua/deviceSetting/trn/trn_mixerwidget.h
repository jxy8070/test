#ifndef TRN_MIXERWIDGET_H
#define TRN_MIXERWIDGET_H

#include "customWidget/overwriteitem.h"
#include <QPushButton>

enum MixerChannelEnum {
    CH1 = 0,    CH2,    CH3,    CH4,
    CH5,    CH6,    CH7,    CH8,
    CH9,    CH10,    CH11,    CH12,
    CH13,    CH14,    CH15,    CH16,
    CH17,    CH18,    CH19,    CH20,
    CH_NUM
};


namespace Ui {
class Trn_MixerWidget;
}
class Trn_MixerWidget : public DevSettingWidget
{
    Q_OBJECT

public:
    explicit Trn_MixerWidget(QWidget *parent = 0);
    ~Trn_MixerWidget();
    void showWidget(quint32 mixerData[32][32]);
    void userNameInit(QStringList newName);

private slots:
    void faderSliderControl();
    void faderSliderChanged(double vlu);
    void pushButtonClicked();
    void on_Reset_clicked();
    void editdName();

signals:
    void mixerVolumeChanged(int, int, quint32);
    void editNameChanged(int, QString);

private:
    Ui::Trn_MixerWidget *_ui;

    quint32 m_MixerSliderValue[CH_NUM][CH_NUM];
    bool m_MixerSwitch[CH_NUM][CH_NUM];
    MixerChannelEnum m_currentOut;
    bool m_faderSliderReInit;

    QList<QPushButton *>m_pushButtonList;
    QList<QLineEdit *> _lineEditUserName;
};

#endif // TRN_MIXERWIDGET_H
