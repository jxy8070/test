#ifndef NEWSOLUTION_H
#define NEWSOLUTION_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class NewSolution;
}
//class QAbstractScrollArea;
class CustomListWidgetItem;
class NewSolution : public QDialog
{
    Q_OBJECT

public:
    explicit NewSolution(QWidget *parent = nullptr);
    ~NewSolution();
    void initCfgNewSln();

private slots:
    //welcome
    void on_listWidget_earlerSln_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_earlerSln_customContextMenuRequested(const QPoint &pos);
    void on_pushButton_newSln_clicked();
    void on_pushButton_openSln_clicked();
    void on_lineEdit_search_returnPressed();
    void on_lineEdit_search_textChanged(const QString &arg1);
    void delHistoryFile(QString fileName = QString());
    //New Solution
    void on_checkBox_clicked(bool checked);
    void on_lineEdit_proName_textEdited(const QString &newText);
    void on_btn_lastStep_clicked();
    void on_btn_create_clicked();
    void on_btn_path_clicked();
signals:
    void solutionOpened(QString);

private:
    Ui::NewSolution *ui;
    QStringList _HISTFiles;  //fileNames
};


#endif // NEWSOLUTION_H
