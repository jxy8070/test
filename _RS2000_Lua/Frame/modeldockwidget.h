#ifndef MODELDOCKWIDGET_H
#define MODELDOCKWIDGET_H

#include "toolkittree.h"
#include "customWidget/overwriteitem.h"
#include "mfrscfg.h"

Q_DECLARE_METATYPE(ConfigScene::ToolkitType);

class ModelDockWidget : public DockWidget
{
    Q_OBJECT

public:
    ModelDockWidget(const QString &title);
    void UpdatePlatformModel(PlatformType platform, int totalPortNum, ConfigScene *pScene);
private:
    void parentExpand(QTreeWidgetItem *item);
    bool FilterModel(CfgType cfgType, QString filter);
signals:
    void sendToolBtnSignal(ConfigScene::ToolkitType type);
private slots:
    void search();
    void onToolBtnCheckedSlot(bool);
public slots:
    void onToolkitBtnStatusChangedSlot(bool);
private:
    ToolKitTree* _modelTree;
    QLineEdit* _SearchText;
    QList<QPushButton *> _toolKitBtnList;
    static QMap<CfgType, QString> _filterMap;
};

#endif // MODELDOCKWIDGET_H
