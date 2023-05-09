#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QWidget>
#include "Frame/absgraphscene.h"

class PageManager : public QWidget
{
    Q_OBJECT
public:
    explicit PageManager(QWidget *parent = nullptr);
    void initUi();
    void reflashPage(AbsGraphScene* scene);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void onActionTriggered();
    void onPageChanged(int index);
private:
    AbsGraphScene* _scene = NULL;
    QMenu* _contextMenu = NULL;
    QAction* _actionAdd = NULL;
    QAction* _actionDel = NULL;
    QAction* _actionRename = NULL;
    QTabWidget* _tabWidget = NULL;
};

#endif // PAGEMANAGER_H
