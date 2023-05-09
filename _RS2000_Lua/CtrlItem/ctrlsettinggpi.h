#ifndef CTRLSETTINGGPI_H
#define CTRLSETTINGGPI_H

#include <QWidget>


class CtrlItemGpi;
class CtrlSettingGpi : public QWidget
{
    Q_OBJECT

public:
    explicit CtrlSettingGpi(CtrlItemGpi* ctrlItem, QWidget *parent = nullptr);
    ~CtrlSettingGpi();

protected slots:
    void setCtrlInfo(unsigned int * pData);
protected:
    void setupUi();

private:
    CtrlItemGpi* _ctrlItem = nullptr;
    QList<QPushButton*> _pinList;
};

#endif // CTRLSETTINGGPI_H
