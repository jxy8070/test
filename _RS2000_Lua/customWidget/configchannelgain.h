#ifndef CONFIGCHANNELGAIN_H
#define CONFIGCHANNELGAIN_H

#include <QGroupBox>
#include <QButtonGroup>

namespace Ui {
class ConfigChannelGain;
}

class DeviceItem;
class ConfigChannelGain : public QGroupBox
{
    Q_OBJECT

public:
    explicit ConfigChannelGain(QWidget *parent = 0);
    ~ConfigChannelGain();
    void init(DeviceItem *devItemBase, int channel);

private slots:
    void on_pushButton_clicked(bool checked);
    void onBtnGroupClicked(int i);

private:
    Ui::ConfigChannelGain *ui;
    DeviceItem *_devItemBase;
    QButtonGroup *_btnGroup;
    int _lastCheckId;
    int _channel;
};

#endif // CONFIGCHANNELGAIN_H
