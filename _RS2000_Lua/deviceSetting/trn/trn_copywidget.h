#ifndef TRN_COPYWIDGET_H
#define TRN_COPYWIDGET_H

#include "customWidget/overwriteitem.h"
#include <QCheckBox>
#include <QList>

namespace Ui {
class Trn_CopyWidget;
}

enum {
    CopyType_In_Eq = 0,
    CopyType_In_Dyn,
    CopyType_Out_Eq,
    CopyType_Out_Split,
    CopyType_Out_Dyn,
};

enum {
    CLEAR_NONE = 0,
    CLEAR_TOP,
    CLEAR_BOTTOM,
    CLEAR_ALL,
};

class Trn_CopyWidget : public DevSettingWidget
{
    Q_OBJECT

public:
    explicit Trn_CopyWidget(QWidget *parent = 0);
    ~Trn_CopyWidget();

signals:
    void copyValueSig(int from, int type, QByteArray to);

protected:
    void showEvent(QShowEvent *event);

private slots:
    void on_comboBox_source_currentIndexChanged(int index);
    void on_pushButton_clicked();
    void on_comboBox_type_currentIndexChanged(int index);

private:
    void clearSelect(int flag);

private:
    Ui::Trn_CopyWidget *ui;
    int _from;
    int _type;
    QByteArray _to;

    QList<QCheckBox *> _checkBoxList;

    QStringList _inStrList;
    QStringList _outStrList;
};

#endif // TRN_COPYWIDGET_H
