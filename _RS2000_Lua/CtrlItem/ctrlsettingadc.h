#ifndef CTRLSETTINGADC_H
#define CTRLSETTINGADC_H

#include <QWidget>

namespace Ui {
class CtrlSettingADC;
}

class CtrlItemADC;
class CtrlSettingADC : public QWidget
{
    Q_OBJECT

public:
    explicit CtrlSettingADC(CtrlItemADC* ctrlItem, QWidget *parent = nullptr);
    ~CtrlSettingADC();

protected slots:
    void setCtrlInfo(unsigned int* pData);
private:    
    CtrlItemADC* _ctrlItem = nullptr;
    Ui::CtrlSettingADC *ui;
};

#endif // CTRLSETTINGADC_H
