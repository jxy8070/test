#ifndef TRN_SCENEWIDGET_H
#define TRN_SCENEWIDGET_H

#include "customWidget/overwriteitem.h"
#include <QPushButton>

namespace Ui {
class Trn_SceneWidget;
}

//SceneWidget
class Trn_SceneWidget : public DevSettingWidget
{
    Q_OBJECT

public:
    explicit Trn_SceneWidget(QWidget *parent = 0);

    ~Trn_SceneWidget();

private slots:
    void sceneButtonClicked();

signals:
    void sceneSelected(quint8);

private:
    Ui::Trn_SceneWidget *_ui;
    QList<QPushButton *> _sceneList;
};

#endif // TRN_SCENEWIDGET_H
