#ifndef UCIPORTWIDGET_H
#define UCIPORTWIDGET_H

#include <QWidget>
#include <QGraphicsOpacityEffect>

namespace Ui {
class UciPortWidget;
}

class UciPortWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UciPortWidget(QWidget *parent = nullptr);
    ~UciPortWidget();

    void setLevelValue(int value);
    void setGainRang(int minValue, int maxValue);
    void setChannel(int ch);
    void setGain(int value);
    void setMute(bool isMute);
    void setDevName(QString name) { _devName = name; }
    void setCfgType(CfgType type) { _cfgType = type; refreshLevelDisplay(); }
    void setOpacity(qreal opacity);
    void setMark(QString mark);
    void setUserLab(QString lab);

    int getChannel();
    int getGainValue();
    bool getMute();
    QString getDevName() { return _devName; }
    CfgType getCfgType() { return _cfgType; }
private:
    void refreshLevelDisplay();
signals:
    void gainAndMuteChanged(int GainValue, bool Mute);
private slots:
    void onGainAndMuteChangedSlot();
    void onGainBtnClickedSlots();
    void onLevelFadeSlot();
private:
    Ui::UciPortWidget *ui;
    QGraphicsOpacityEffect *_opacity;

    QString _devName;
    CfgType _cfgType;
    QTimer *_levelFade;
    int _fadeValue;
};

#endif // UCIPORTWIDGET_H
