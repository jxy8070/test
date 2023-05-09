#include "overwriteitem.h"
#include "../Frame/mainwindow.h"
void DockWidget::closeEvent(QCloseEvent *event)
{
//    qDebug()<< event->type();
    emit closeSignal(false);
    event->accept();
}

//void DockWidget::setVisible(bool visible)
//{
//    QDockWidget::setVisible(visible);
//    MainWindow *mainWin = qobject_cast<MainWindow *>(parentWidget());
//    if(!mainWin) return ;
//    if(mainWin->dockWidgetArea(this) == Qt::LeftDockWidgetArea ||
//       mainWin->dockWidgetArea(this) == Qt::RightDockWidgetArea) {
//       int height = mainWin->centralWidget()->height() * 0.5;
//       int width  = this->width();
//       this->resize(width, height);
//    }
//}

#include <QCollator>
bool TableWidgetItem::operator<(const QTableWidgetItem &other) const
{
    QCollator cltr;
    cltr.setNumericMode(true);
    int result = cltr.compare(this->text(), other.text());
    if(result < 0)
        return true;
    else
        return false;
}

void GraphicsProxyWidget::focusInEvent(QFocusEvent *event)
{
    setContentsMargins(0, 0, 0, 0);
    setPalette(QPalette(Qt::transparent));
    if(parentItem()) {
        QRectF rec = parentItem()->boundingRect();
        setPos(rec.left(), rec.top());
        resize(rec.width(), rec.height());
    }
    QGraphicsProxyWidget::focusInEvent(event);
}

void GraphicsProxyWidget::focusOutEvent(QFocusEvent *event)
{
    if(this->isVisible())
        this->setVisible(false);
    emit clearFocusSignal();
    QGraphicsProxyWidget::focusOutEvent(event);
}


#include "config.h"
DevSettingDlg::DevSettingDlg(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    setStyleSheet(SkinStyle::getCfgSettingStyleSheet());
}

DevSettingWidget::DevSettingWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(SkinStyle::getCfgSettingStyleSheet());
}

QPainterPath CustomRect::ScalingPerformedForCenterPointUnchanged(QPainterPath SFOpath, int px)
{
    if(px == 0) return SFOpath;
    QPainterPath path;
    int len = SFOpath.elementCount();
    for(int i = 0; i < len-1; i++) {
        const QPointF p  = SFOpath.elementAt(i);
        const QPointF p1 = SFOpath.elementAt(i == 0 ? len - 2 : i - 1);
        const QPointF p2 = SFOpath.elementAt(i == len - 2 ? 0 : i + 1);

        //归一化处理
        qreal v1x = p1.x() - p.x();
        qreal v1y = p1.y() - p.y();
        qreal n1  = qSqrt(qPow(v1x, 2) + qPow(v1y, 2));
        v1x /= n1;
        v1y /= n1;

        qreal v2x = p2.x() - p.x();
        qreal v2y = p2.y() - p.y();
        qreal n2  = qSqrt(qPow(v2x, 2) + qPow(v2y, 2));
        v2x /= n2;
        v2y /= n2;

        //凹凸性
        qreal mult_cross = (p1.x()-p.x()) * (p2.y()-p1.y()) - (p1.y()-p.y()) * (p2.x()-p.x());
        if(mult_cross < 0) mult_cross = -1;
        else mult_cross = 1;

        qreal v = mult_cross * px / qSqrt((1 - (v1x * v2x + v1y * v2y)) / 2);

        qreal vx = v1x + v2x;
        qreal vy = v1y + v2y;
        qreal n  = v / qSqrt(qPow(vx, 2) + qPow(vy, 2));
        vx *= n;
        vy *= n;

        if(i == 0) {
            path.moveTo(QPointF(vx+p.x(), vy+p.y()));
        }
        path.lineTo(QPointF(vx+p.x(), vy+p.y()));
        if(i == len - 2) {
            path.lineTo(path.elementAt(0));
        }
    }

    return path;
}

CustomListWidgetItem::CustomListWidgetItem(const QIcon &icon, const QString filePath, QWidget *parent)
    : QWidget(parent), _filePath(filePath)
{
    QFileInfo fileInfo(_filePath);
    _lab_icon    = new QLabel;
    _lab_slnName = new QLabel(fileInfo.baseName());
    _lab_slnPath = new QLabel(fileInfo.absolutePath());
    _lab_dateTime= new QLabel(fileInfo.lastRead().toString());

    _lab_dateTime->setAlignment(Qt::AlignRight);
    QFont font;
    font.setPixelSize(15);
    font.setBold(true);
    _lab_slnName->setFont(font);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(6);
    mainLayout->addWidget(_lab_icon, 0, 0);
    mainLayout->addWidget(_lab_slnName, 0, 1);
    mainLayout->addWidget(_lab_dateTime, 0, 2);
    mainLayout->addWidget(_lab_slnPath, 1, 1);
    this->setLayout(mainLayout);
    this->setStyleSheet("background-color: transparent;");

}

CustomListWidgetItem::CustomListWidgetItem(const QString filePath, QWidget *parent)
    : QWidget(parent), _filePath(filePath)
{
    QFileInfo fileInfo(_filePath);
    _lab_icon    = new QLabel;
    _lab_slnName = new QLabel(fileInfo.baseName());
    _lab_slnPath = new QLabel(fileInfo.absolutePath());
    _lab_dateTime= new QLabel(fileInfo.lastRead().toString());
    _lab_dateTime->setAlignment(Qt::AlignRight);
    QFont font;
    font.setPixelSize(15);
    font.setBold(true);
    _lab_slnName->setFont(font);

    _lab_icon->setMaximumWidth(16);
    _lab_icon->setPixmap(QPixmap(":/Image/solution.png"));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(6);
    mainLayout->addWidget(_lab_icon, 0, 0);
    mainLayout->addWidget(_lab_slnName, 0, 1);
    mainLayout->addWidget(_lab_dateTime, 0, 2);
    mainLayout->addWidget(_lab_slnPath, 1, 1);
    this->setLayout(mainLayout);
    this->setStyleSheet("background-color: transparent;");
}


CustomDelegate::CustomDelegate(DelegateType delegateType, QStringList itemList, QObject *parent)
  : QItemDelegate(parent), _delegateType(delegateType), _itemList(itemList)
{
}

CustomDelegate::CustomDelegate(DelegateType delegateType, double MaxValue, double MinValue, QObject *parent)
  : QItemDelegate(parent), _delegateType(delegateType), _maxValue(MaxValue), _minValue(MinValue)
{
}

QWidget *CustomDelegate::createEditor(QWidget *parent,
  const QStyleOptionViewItem & option ,
  const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    if(_delegateType == ComboBox) {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems(_itemList);
        return editor;
    }
    else if(_delegateType == DoubleSpinBox) {
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setRange(_minValue, _maxValue);
        return editor;
    }
    else if(_delegateType == SpinBox) {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setRange(_minValue, _maxValue);
        return editor;
    }
    else {
        return parent;
    }
}

void CustomDelegate::setEditorData(QWidget *editor,
  const QModelIndex &index) const
{
  QString value = index.model()->data(index).toString();
  if(_delegateType == ComboBox) {
      QComboBox *widget = static_cast<QComboBox *>(editor);
      int idx = widget->findText(value);
      widget->setCurrentIndex(idx);
  }
  else if(_delegateType == DoubleSpinBox) {
      QDoubleSpinBox *widget = static_cast<QDoubleSpinBox *>(editor);
      widget->setValue(value.toDouble());
  }
  else if(_delegateType == SpinBox) {
      QSpinBox *widget = static_cast<QSpinBox *>(editor);
      widget->setValue(value.toInt());
  }
}

void CustomDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
  const QModelIndex &index) const
{
    QString value = QString();
    if(_delegateType == ComboBox) {
        QComboBox *widget = static_cast<QComboBox *>(editor);
        value = widget->currentText();
    }
    else if(_delegateType == DoubleSpinBox) {
        QDoubleSpinBox *widget = static_cast<QDoubleSpinBox *>(editor);
        value = QString::number(widget->value());
    }
    else if(_delegateType == SpinBox) {
        QSpinBox *widget = static_cast<QSpinBox *>(editor);
        value = QString::number(widget->value());
    }
    model->setData(index, value);
}

void CustomDelegate::updateEditorGeometry(QWidget *editor,
  const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    editor->setGeometry(option.rect);
}
