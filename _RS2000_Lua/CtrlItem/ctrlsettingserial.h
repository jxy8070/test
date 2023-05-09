#ifndef CTRLSETTINGSERIAL_H
#define CTRLSETTINGSERIAL_H

#include <QWidget>

namespace Ui {
class CtrlSettingSerial;
}

class CtrlSettingSerial : public QWidget
{
    Q_OBJECT

public:
    explicit CtrlSettingSerial(QWidget *parent = nullptr);
    ~CtrlSettingSerial();

private:
    Ui::CtrlSettingSerial *ui;
};

#endif // CTRLSETTINGSERIAL_H
