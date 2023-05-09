#ifndef CONFIGGATINGRELATIVETHREASHOLDCHANNEL_H
#define CONFIGGATINGRELATIVETHREASHOLDCHANNEL_H

#include <QWidget>

namespace Ui {
class ConfigGatingRelativeThreasholdChannel;
}

class ConfigGatingRelativeThreasholdChannel : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigGatingRelativeThreasholdChannel(QWidget *parent = 0);
    ~ConfigGatingRelativeThreasholdChannel();
    void setTitle(QString title);
    void setVu(int value);
    void setOpen(bool checked);
    void setPostGateGain(int value);
    void setPostGateMute(bool checked);
    void setDefault(bool checked);
    void setManual(bool checked);
    void initStyle(void);

signals:
    void PostGateGainValueChanged(int);
    void DefaultClicked(bool);
    void ManualClicked(bool);
    void PostGateMuteClicked(bool);

private slots:
    void on_dial_PostGateGain_valueChanged(int value);
    void on_pushButton_Default_clicked(bool checked);
    void on_pushButton_Manual_clicked(bool checked);
    void on_pushButton_PostGateMute_clicked(bool checked);

private:
    Ui::ConfigGatingRelativeThreasholdChannel *ui;
};

#endif // CONFIGGATINGRELATIVETHREASHOLDCHANNEL_H
