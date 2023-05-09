#ifndef SOLUTIONDOCKWIDGET_H
#define SOLUTIONDOCKWIDGET_H

#include <QTreeWidget>
#include "customWidget/overwriteitem.h"


class SolutionDockWidget: public DockWidget
{
    Q_OBJECT
public:
    SolutionDockWidget();
    void setSolutionDir(QString slnPath);
    void importFile(QString filePath);
    inline QStringList getAllProjectFiles() {return _allProjectList;}
    inline QStringList getAllConfigFiles() {return _allConfigList;}
    inline QString getAbsolutePathOfSln(QString filePath) {return _solutionDir.absoluteFilePath(filePath);}
    inline QString getSolutionFilePath() {return _solutionDir.absoluteFilePath(_slnFileName);}
    bool hasOpendSolution() { return _slnTree->topLevelItemCount() > 0;}

signals:
    void openSubWindow(QString filePath);
    void deleteSubWindow(QString filePath);
    void renameSubWindow(QString oldFilePath, QString newFilePath);

public slots:
    void updateSlnTree();
    void setSlnFileName(QString slnName) { _slnFileName = slnName; }
protected slots:
    void onTreeViewMenuRequested(const QPoint &pos);
    void onItemDoubleClicked(QTreeWidgetItem* item, int column);
    void onDeleteItem();
    void onRenameItem();


private:
    QStringList _allProjectList;
    QStringList _allConfigList;
    QTreeWidget* _slnTree = NULL;
    QDir _solutionDir;   //解决方案路径
    QString _slnFileName = "Solution.sln";//解决方案文件名
};
#endif // SOLUTIONDOCKWIDGET_H
