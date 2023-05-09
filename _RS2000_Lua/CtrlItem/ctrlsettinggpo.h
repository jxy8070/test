#ifndef CTRLSETTINGGPO_H
#define CTRLSETTINGGPO_H

#include <QWidget>

class CtrlItemGpo;
class CtrlSettingGpo : public QWidget
{
    Q_OBJECT

public:
    explicit CtrlSettingGpo(CtrlItemGpo* ctrlItem, QWidget *parent = nullptr);
    ~CtrlSettingGpo();

protected slots:
    void setCtrlInfo(unsigned int* pData);
protected:
    void setupUi();
private:
    CtrlItemGpo* _ctrlItem = nullptr;
    QList<QPushButton*> _pinList;
};

#endif // CTRLSETTINGGPO_H
