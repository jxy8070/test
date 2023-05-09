#ifndef SPKROUTER_H
#define SPKROUTER_H

#include "customWidget/overwriteitem.h"

// 音箱路由界面
namespace Ui {
  class Spk_Router;
}
class Device_Spk;
class SpkRouter : public DevSettingDlg
{
    Q_OBJECT
public:
    SpkRouter(Device_Spk *devSpk, QWidget *parent = 0);
    QString getRouterInfo();

signals:

public slots:    
    void onRouterChanged();

private:
    Ui::Spk_Router *_ui;
    Device_Spk *_devSpk;
};

#endif // SPKROUTER_H
