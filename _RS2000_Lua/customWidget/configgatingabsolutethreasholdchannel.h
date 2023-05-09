#ifndef CONFIGGATINGABSOLUTETHREASHOLDCHANNEL_H
#define CONFIGGATINGABSOLUTETHREASHOLDCHANNEL_H

#include <QWidget>

namespace Ui {
class ConfigGatingAbsoluteThreasholdChannel;
}

class ConfigGatingAbsoluteThreasholdChannel : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigGatingAbsoluteThreasholdChannel(QWidget *parent = 0);
    ~ConfigGatingAbsoluteThreasholdChannel();
    void setTitle(QString title);
    void setManual(bool checked);
    void setMode(int mode);
    void setPriority(int value);
    void setThreshold(int value);
    void setOpen(bool checked);

signals:
    void ManualClicked(bool);
    void ModeValueChanged(int);
    void PriorityValueChanged(int);
    void ThresholdValueChanged(int);

private slots:
    void on_pushButton_Manual_clicked(bool checked);

    void on_comboBox_Mode_currentIndexChanged(int index);

    void on_spinBox_Priority_valueChanged(int arg1);

    void gainWdg_valueChanged(double value);

private:
    Ui::ConfigGatingAbsoluteThreasholdChannel *ui;
};

#endif // CONFIGGATINGABSOLUTETHREASHOLDCHANNEL_H
