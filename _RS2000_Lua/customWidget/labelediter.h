#ifndef LABELEDITER_H
#define LABELEDITER_H

#include <QWidget>

namespace Ui {
class LabelEditer;
}

class AbsGraphScene;
class LineLabel;
class QListWidgetItem;
enum DevPortType;
class LabelEditer : public QWidget
{
    Q_OBJECT
public:
    explicit LabelEditer(AbsGraphScene* scene);
    void init(QString oldName, QPointF pos, QList<LineLabel*> labelList);

private slots:
    void onTextEdited(const QString &text);
    void onItemDoubleClicked(QListWidgetItem *item);

protected:
    void setNameDuplicate(bool isDuplicate);
    void onTextComplete(const QString &labelName);
    bool eventFilter(QObject *target, QEvent *event) override;

//    void keyPressEvent(QKeyEvent *event) override;
private:
    AbsGraphScene* _scene;
    QList<LineLabel*> _labelList;
    QString _oldName;
    QString _lastName;
    bool _isInput;
    bool _nameDuplicate;
    DevPortType _portType;
    Ui::LabelEditer *ui;
};

#endif // LABELEDITER_H
