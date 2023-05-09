#ifndef UCIITEM_MIXER_H
#define UCIITEM_MIXER_H

#include <QWidget>

class ConfigItemUCI;
class QTextEdit;
class ConfigGainWidget;
class UciItem_mixer : public QGraphicsScene
{
    Q_OBJECT
    enum labType{
        IN = 0,
        OUT,
    };
public:
    UciItem_mixer(QString devName, bool nodesSwitch, CfgType type, ConfigItemUCI *uciItem, QGraphicsView *view);

    ~UciItem_mixer();
    ConfigItemUCI *getCfgItemUci() { return _uciItem; };
signals:

public slots:
    void onInLabelPageChanged();
    void onOutLabelPageChanged();
    void updateNodeData(int minRow, int maxRow, int minCol, int maxCol);
    void initDisplayRange();

    void onRouterInfoChanged();
    void onRouterGainChanged(int);
    void onLabTextChanged();
    void initData();
private:
    void initUi(void);
    void addlabPage(QGridLayout *mainLayout);
    void addLabel(QGridLayout *mainLayout, labType type);
private:
    CfgType _cfgType;
    ConfigItemUCI *_uciItem;
    QWidget *_wid;
    QString _devName;
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

    int _currentMinRow = 0;
    int _currentMaxRow = 0;
    int _currentMinCol = 0;
    int _currentMaxCol = 0;

    qreal _scale = 1;
//    int _nodeGain[64][64];//节点增益
    QString _nodeLab[2][64];
    bool _nodeSwitch;
};


#endif // UCIITEM_MIXER_H
