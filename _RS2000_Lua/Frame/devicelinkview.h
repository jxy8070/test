#ifndef DEVICELINKVIEW_H
#define DEVICELINKVIEW_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class DeviceLinkView;
}

class ProjectScene;
class DeviceLinkView : public QDialog
{
    Q_OBJECT

public:
    DeviceLinkView(ProjectScene *projScene, QWidget *parent = 0);
    ~DeviceLinkView();

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_exit_clicked();

    void on_saveas_clicked();

private:
    void initLink();
    void clear();

private:
    Ui::DeviceLinkView *ui;
    QList<QTableWidgetItem *>m_tableItem;
    ProjectScene *_projScene;
};

#endif // DEVICELINKVIEW_H
