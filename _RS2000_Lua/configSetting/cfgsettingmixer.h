#ifndef CFGSETTINGMIXER_H
#define CFGSETTINGMIXER_H

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>


class ConfigGainWidget;
class ConfigItemMixer;
class QTextEdit;
struct GainMute;
class CfgSettingMixer : public QWidget
{
    Q_OBJECT
    enum labType{
        IN = 0,
        OUT,
    };
public:
    CfgSettingMixer(ConfigItemMixer *cfgItem,  QWidget *parent = 0);

    void setNodesSwitch();
signals:

public slots:
    void onRouterMuteChanged();
    void onRouterGainChanged(double value);
    void onInGainValueChanged(double value);
    void onOutGainValueChanged(double value);
    void onLabTextChanged();
    void onInLabelPageChanged();
    void onOutLabelPageChanged();
    void updateUiData();

    void onOutMuteChanged();
    void onInMuteChanged();
private:
    void setupUi();
    void setupUi_FangTu();

    void routerInfoChanged(int row, int col, bool status);

    void addLabelAreaOut(QGridLayout *mainLayout);
    void addLabelAreaIn(QGridLayout *mainLayout);
    void addlabPage(QGridLayout *mainLayout);

    void updateNodeData(int minRow, int maxRow, int minCol, int maxCol);
    void initDisplayRange();
private:
    ConfigItemMixer *_cfgItemMixer;

    QList<ConfigGainWidget *> _routerBtnList;
    QList<ConfigGainWidget *> _InBtnList;
    QList<ConfigGainWidget *> _OutBtnList;

    QList<QPushButton *> _outMuteBtnList;
    QList<QPushButton *> _inMuteBtnList;

    QList<QLabel *> _InLabelList;
    QList<QLabel *> _outLabelList;
    QList<QTextEdit *> _InLineEdit;
    QList<QTextEdit *> _outLineEdit;

    QList<QPushButton *> _inLabBtnList;
    QList<QPushButton *> _outLabBtnList;
    int _rowNum;
    int _colNum;
    int _colPosetionHead;
    int _rowPosetionHead;

    int _currentMinRow;
    int _currentMaxRow;
    int _currentMinCol;
    int _currentMaxCol;
};

#endif // CFGSETTINGMIXER_H
