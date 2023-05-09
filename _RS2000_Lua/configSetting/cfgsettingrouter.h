#ifndef CFGSETTINGROUTE_H
#define CFGSETTINGROUTE_H

#include<QStackedWidget>
class ConfigItemRouter;
class QGridLayout;
class QButtonGroup;
class QLineEdit;
class CfgSettingRouter: public QWidget
{
    Q_OBJECT
public:
    CfgSettingRouter(ConfigItemRouter *cfgItem,  QWidget *parent = 0);

    void updateUiData();
signals:

public slots:
    void onRouterChanged(bool checked);
    void onPageSelected();
private:
    void setupUi();
    void setupUi_FangTu();
    void setupRoutePage(QWidget* routeWidget, int pagePortNum);
    void updateRoutePage(int pageRowIdx, int pageColIdx);
    void addHeader_FangTu(QGridLayout *mainLayout);
    bool routerChecked(int in, int out);
private:
    ConfigItemRouter *_cfgItemRouter;
    QList<QButtonGroup *> _btnGroupList;    
    QList<QLabel *> _inLabelList;
    QList<QLabel *> _outLabelList;
    QList<QLineEdit *> _btnLabelList;
    int _pageRowIdx = 0;
    int _pageColIdx = 0;
    int _pagePortNum = 32;
    int _rowNum = 0;
    int _colNum = 0;
};

#endif // CFGSETTINGROUTE_H
