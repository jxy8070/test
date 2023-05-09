#include "stable.h"
#include "trn_scenewidget.h"
#include "ui_trn_scenewidget.h"

Trn_SceneWidget::Trn_SceneWidget(QWidget *parent) :
    DevSettingWidget(parent),
    _ui(new Ui::Trn_SceneWidget)
{
    _ui->setupUi(this);
    hide();

    _sceneList.append(_ui->pushButton);
    _sceneList.append(_ui->pushButton_2);
    _sceneList.append(_ui->pushButton_3);
    _sceneList.append(_ui->pushButton_4);
    _sceneList.append(_ui->pushButton_5);
    _sceneList.append(_ui->pushButton_8);
    _sceneList.append(_ui->pushButton_7);
    _sceneList.append(_ui->pushButton_6);
    _sceneList.append(_ui->pushButton_9);
    _sceneList.append(_ui->pushButton_10);

    for(int i = 0; i < 10; ++i)
    {
        connect(_sceneList.value(i), SIGNAL(clicked()), this, SLOT(sceneButtonClicked()));
    }
}

Trn_SceneWidget::~Trn_SceneWidget()
{
    delete _ui;
}

void Trn_SceneWidget::sceneButtonClicked()
{
    emit sceneSelected(_sceneList.indexOf(qobject_cast<QPushButton *>(sender())));

    close();
}
