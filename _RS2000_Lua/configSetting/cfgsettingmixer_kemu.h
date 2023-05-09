#ifndef CFGSETTINGMIXER_KEMU_H
#define CFGSETTINGMIXER_KEMU_H

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>


class ConfigGainWidget;
class ConfigItemMixer;
class GainLineEdit;
class QTextEdit;
struct GainMute;
class CfgSettingMixer_Kemu : public QWidget
{
    Q_OBJECT
    enum labType{ IN = 0, OUT = 1, FixW = 42, FixH = 15, PageRows = 16, PageCols = 16};
public:
    CfgSettingMixer_Kemu(ConfigItemMixer *cfgItem,  QWidget *parent = 0);

signals:

public slots:
    void onRouterGainChanged(double value);
    void onInGainValueChanged(double value);
    void onOutGainValueChanged(double value);
    void onRouterMuteChanged();
    void onOutMuteChanged();
    void onInMuteChanged();

    void onLabTextChanged();
    void onInLabelPageChanged();
    void onOutLabelPageChanged();
    void updateUiData();

private:
    void setupUi();
    QVBoxLayout* createNode(int row, int col, double gain, bool mute);
    void routerInfoChanged(int row, int col, bool status);

    void addLabelAreaOut(QGridLayout *mainLayout);
    void addLabelAreaIn(QGridLayout *mainLayout);
    void addlabPage(QGridLayout *mainLayout);

    void updateNodeData();
    void initDisplayRange();
private:
    ConfigItemMixer *_cfgItemMixer;

    QList<GainLineEdit *> _mixGainList;
    QList<GainLineEdit *> _inGainList;
    QList<GainLineEdit *> _outGainList;

    QList<QPushButton *> _mixMuteBtnList;
    QList<QPushButton *> _outMuteBtnList;
    QList<QPushButton *> _inMuteBtnList;

    QList<QLabel *> _InLabelList;
    QList<QLabel *> _outLabelList;
    QList<QTextEdit *> _InLineEdit;
    QList<QTextEdit *> _outLineEdit;

    QList<QPushButton *> _colPageBtnList;
    QList<QPushButton *> _rowPageBtnList;
    int _rowNum;
    int _colNum;
    int _colPosetionHead;
    int _rowPosetionHead;

    int _curRowPage = 0;
    int _curColPage = 0;
};

#endif // CFGSETTINGMIXER_KEMU_H
