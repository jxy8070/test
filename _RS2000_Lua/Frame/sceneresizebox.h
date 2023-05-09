#ifndef SCENERESIZEBOX_H
#define SCENERESIZEBOX_H

#include <QDialog>
#include "ui_sceneresizebox.h"

class SceneResizeBox : public QDialog, public Ui::SceneResizeBox
{
    Q_OBJECT
public:
    explicit SceneResizeBox(int width, int height, QWidget *parent = 0);


protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // SCENERESIZEBOX_H
