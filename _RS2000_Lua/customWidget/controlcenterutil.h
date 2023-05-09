#ifndef CONTROLCENTERUTIL_H
#define CONTROLCENTERUTIL_H

#include <QWidget>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include "configgainwidget.h"

namespace Ui {
class ControlCenterUtil;
}

struct CenterCtrlCmd
{
    quint32 CmdKey;
    QString CmdName;
    QString GroupName;
    quint32 CmdGroup;
    QString formatType;
    QString parserType;
};

class CmdKeyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CmdKeyModel(QObject *parent = nullptr);
    ~CmdKeyModel();
   int rowCount(const QModelIndex &parent = QModelIndex())const override;
   int columnCount(const QModelIndex &parent = QModelIndex())const override;
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole)const override;
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
   // void setCmdGroup(quint32 cmdGroup = 0xFFFFFFFF);
   // CenterCtrlCmd getCmdByKey(quint32 cmdKey);

   static QList<CenterCtrlCmd> CmdList;

//private:
//   QList<CenterCtrlCmd> _cmdList;
//   static QList<CenterCtrlCmd> SrcCmdList;


private:
    QStringList _headeList;
};

class AES70Device;
class UdpGram;
class ControlCenterUtil : public QWidget
{
    Q_OBJECT

public:
    explicit ControlCenterUtil(QWidget *parent = nullptr);
    ~ControlCenterUtil();

private:
    void addExtraParamUi(QHBoxLayout* exDataLayout, QString paramType);
    QByteArray formatDatagram(UdpGram& sendGram); //根据命令类型获取数据、设置校验并格式化包
    void parserDatagram(UdpGram& recvGram);
private slots:
    void onSendGram();
    void onSelectDevice();
    void onFlashCfgItemModel();
    void onCmdSelected(int index);
    void onModelNameSelected(int index);
    void onDeviceSelected(int row, int column);

private:
    Ui::ControlCenterUtil *ui;
    QList<AES70Device*> _devList;
    CmdKeyModel* _cmdModel = NULL;
    CenterCtrlCmd _curCmd;
    QTableWidget* _devTable = NULL;
    QStringList* splitFilterType;
    QStringList* automixInputMode;
    QStringList* lastMicStatus;
    QStringList* attenuationType;
    QStringList* nrGainType;
    QStringList* rampType;
    QStringList* rampEffect;
    QStringList* specialModal;

    QWidget* _exDataContent = NULL;
    QComboBox* _exData1;
    QComboBox* _exData2;
    QCheckBox* _stateParam;
    ConfigGainWidget* _delayParam;
    QLineEdit* _priorityParam;
    QLineEdit* _maxmicParam;
    ConfigGainWidget* _gainParam;
    ConfigGainWidget* _ratioParam;
    ConfigGainWidget* _timeParam;
    ConfigGainWidget* _fzParam;
    ConfigGainWidget* _qParam;
    ConfigGainWidget* _bwParam;
    ConfigGainWidget* _attParam;

    QSortFilterProxyModel* cmdFilterModel;
};

#endif // CONTROLCENTERUTIL_H
