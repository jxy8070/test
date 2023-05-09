#ifndef CFGSETTINGSNAPSHOTBANK_H
#define CFGSETTINGSNAPSHOTBANK_H

#include "configItem/configitemsnapshot.h"
//class ConfigItemSnapshot;
//enum btnType;
class CfgSettingSnapshotBank : public QWidget
{
    Q_OBJECT
public:
    explicit CfgSettingSnapshotBank(ConfigItemSnapshot *cfgItem, QWidget *parent = nullptr);
    void initUI();
    void connected();
    void disconnected();
    void initData();

signals:

public slots:
    void onBtnClicked_slots();
    void onLineEditEditingFinished_slots();
private:
    void setUISize();
    void removeTheLayout();
    void refreshLoadBtn(QPushButton *btn);
    void setBtnPervOrNext(ConfigItemSnapshot::btnType type);
private:
    typedef struct SnpBtnPair{
        QPushButton *s_loadBtn;
        QPushButton *s_saveBtn;
        QLineEdit   *s_lineEdit;
        SnpBtnPair(QPushButton *loadBtn, QPushButton *saveBtn, QLineEdit *lineEdit) {
            s_loadBtn = loadBtn;
            s_saveBtn = saveBtn;
            s_lineEdit   = lineEdit;
        }
    }SnpBtnPair_t;

    ConfigItemSnapshot *_cfgItem;
    int _snapCount;
    QList<SnpBtnPair_t> _snapBtnList;
};

#endif // CFGSETTINGSNAPSHOTBANK_H
