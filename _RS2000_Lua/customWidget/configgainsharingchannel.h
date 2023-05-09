#ifndef CONFIGGAINSHARINGCHANNEL_H
#define CONFIGGAINSHARINGCHANNEL_H

#include <QWidget>

namespace Ui {
class ConfigGainSharingChannel;
}

class ConfigGainSharingChannel : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigGainSharingChannel(QWidget *parent = 0);
    ~ConfigGainSharingChannel();
    void setTitle(QString title);
    void setVu(int value);
    void setPreInGain(int value);
    void setPreInMute(bool checked);
    void initStyle();

signals:
    void PreInGainValueChanged(int);
    void PreInMuteClicked(bool);

private slots:
    void on_dial_valueChanged(int value);
    void on_PB_InputMute_1_clicked(bool checked);

private:
    Ui::ConfigGainSharingChannel *ui;
};

#endif // CONFIGGAINSHARINGCHANNEL_H
