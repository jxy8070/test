#ifndef UCIITEM_PORT_H
#define UCIITEM_PORT_H

#include <QWidget>
#include "common.h"
#include "configItem/configitemuci.h"

//class ConfigItemUCI;
class UciPortWidget;
class UserControlInterface;
class UciItem_port : public QWidget
{
    Q_OBJECT
public:
    explicit UciItem_port(/*CfgType cfgType,*/ ConfigItemUCI *uciItem, int page, QWidget *parent = nullptr);
    enum PinType{
        Analog = 0,
        network,
    };
    int getPage() { return _page; }
    void insertPortData(int pos, ConfigItemUCI::Uci_data_t *data) { _portDataList.insert(pos, data); }
    ConfigItemUCI *getUciItem() { return _uciItem; }
private:
    void initUI(void);
signals:

public slots:
    void onGainAndMuteChanged(int gainValue, bool mute);
    void onGainAndMuteChanged_sumOutput(int gainValue, bool mute);
    void initData(QString ip);
    void setInOutLevel(unsigned int *);
private:
    void createLab(QWidget *wid);
private:
    QLabel *_labTitle;
    UserControlInterface *_uciDialog;
//    CfgType _cfgType;
    ConfigItemUCI *_uciItem;
    int _page;
    QList<UciPortWidget *> _portWidgetList;
    QList<QPushButton *> _sceneBtnList;

    QMap<int, ConfigItemUCI::Uci_data_t *> _portDataList;

    int _sumOut_valueMark = 0;
};

#endif // UCIITEM_PORT_H
