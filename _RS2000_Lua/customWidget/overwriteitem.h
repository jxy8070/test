#ifndef OVERWRITEITEM_H
#define OVERWRITEITEM_H

//重写dock栏关闭事件
#include <QDockWidget>

class DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    using QDockWidget::QDockWidget;

protected:
    void closeEvent(QCloseEvent *event) override;
//public:
//    void setVisible(bool visible) override;
signals:
    void closeSignal(bool);

};

//重写tablewidget排序函数
#include <QTableWidgetItem>

class TableWidgetItem : public QTableWidgetItem
{

public:
    using QTableWidgetItem::QTableWidgetItem;

    void addWidget(QWidget *wid);
public:
    virtual bool operator<(const QTableWidgetItem& other) const override;

};

//重写QGraphicsScene代理层 焦点事件
#include <QGraphicsProxyWidget>
class GraphicsProxyWidget : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    using QGraphicsProxyWidget::QGraphicsProxyWidget;
signals:
    void clearFocusSignal();
protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
};


//重写QDiglog 加载qss
#include <QDialog>
class DevSettingDlg : public QDialog
{
    Q_OBJECT
public:
    DevSettingDlg(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
};

//重写QDiglog 加载qss
#include <QDialog>
class DevSettingWidget : public QWidget
{
    Q_OBJECT
public:
    DevSettingWidget(QWidget *parent = nullptr);
};


class CustomRect
{
public:
    //围绕中心点进行归一化处理
    static QPainterPath ScalingPerformedForCenterPointUnchanged(QPainterPath SFOpath, int px);
};

//自定义QListWidgetItem的widget
class CustomListWidgetItem : public QWidget
{
    Q_OBJECT
public:
    CustomListWidgetItem(const QIcon &icon, const QString filePath, QWidget *parent = nullptr);
    CustomListWidgetItem(const QString filePath, QWidget *parent = nullptr);

    inline QString getFilePath() { return _filePath; }
private:
    const QString _filePath;
    bool _isTop = false;    //置顶 未完成
    QLabel *_lab_icon;
    QLabel *_lab_slnName;
    QLabel *_lab_slnPath;
    QLabel *_lab_dateTime;
};


//自定义视图委托
#include <QItemDelegate>
class CustomDelegate : public QItemDelegate
{
  Q_OBJECT
public:
    enum DelegateType{
        ComboBox,
        SpinBox,
        DoubleSpinBox,
    };

public:
  CustomDelegate(DelegateType delegateType, QStringList itemList = QStringList(), QObject *parent = 0);
  CustomDelegate(DelegateType delegateType, double MaxValue, double MinValue, QObject *parent = 0);

  //创建编辑器，当视图需要一个编辑器时会告知委托为被修改的项目提供一个编辑器部件，此时会调用此方法
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)     const override;
  //为编辑器设置数据，将模型中的数据设置到编辑器中
  void setEditorData(QWidget *editor, const QModelIndex &index)                                            const override;
  //将数据写入到模型中，当用户编辑完成后把编辑器中的数据写入到模型中
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)                  const override;
  //更新编辑器几何布局
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
public Q_SLOTS:


private:
  DelegateType _delegateType;
  QStringList _itemList;
  double _maxValue;
  double _minValue;
};
#endif // OVERWRITEITEM_H
