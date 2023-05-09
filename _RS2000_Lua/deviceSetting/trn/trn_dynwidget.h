#ifndef TRN_DYNWIDGET_H
#define TRN_DYNWIDGET_H

#include "customWidget/overwriteitem.h"
#include "Protocal/packet.h"

namespace Ui {
class Trn_DynWidget;
class Trn_DynWidget_FangTu;
}

enum DynTypeEnum {
    DynType_ALL = 0,
    DynType_Grap,
    DynType_Spin,
};

class Trn_DynWidget : public DevSettingWidget
{
    Q_OBJECT

public:
    explicit Trn_DynWidget(QWidget *parent = 0);
    ~Trn_DynWidget();

    void showWidget(bool isIn, quint8 ch, QString chName, DynData &dynData);

public slots:
    void byPassFunc();
    void getDynValue(int *x, int *y);
    void spinBoxChanged();
    void setDynValue();
    void setSpinBoxLimit();
    void initDynData();
    DynData & getDynData();

signals:
    void dynChanged(quint8, bool, DynData);

private:
    void connectSig(DynTypeEnum type);
    void disconnectSig(DynTypeEnum type);
    void getDispData();
    void display(DynTypeEnum type);

private slots:
    void on_pushButton_clicked();

private:
    float transX(int x)
    {
        return 0.2*x - 90;          // 坐标转换为dbfs
    }
    float transY(int y)
    {
        return -0.2*y + 10;
    }
    int anti_transX(float x)
    {
        return (int)((x + 90)*5);   // dbfs转换为坐标
    }
    int anti_transY(float y)
    {
        return (int)((10 - y)*5);
    }

private:
#if Logo_FangTu
    Ui::Trn_DynWidget_FangTu *_ui;
#else
    Ui::Trn_DynWidget *_ui;
#endif
    bool _isIn;
    quint8 _ch;

    int _xValue[4], _yValue[4];
    DynData _dyndata;
};

#endif // TRN_DYNWIDGET_H
