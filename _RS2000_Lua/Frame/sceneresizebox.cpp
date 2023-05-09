#include <QKeyEvent>
#include "sceneresizebox.h"

SceneResizeBox::SceneResizeBox(int width, int height, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    widthTxt->setText(QString::number(width));
    heightTxt->setText(QString::number(height));
}


void SceneResizeBox::showEvent(QShowEvent *e)
{
    QDialog::showEvent(e);
}

void SceneResizeBox::closeEvent(QCloseEvent *)
{

}

void SceneResizeBox::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Escape:
        on_pushButton_2_clicked();
        break;
    case Qt::Key_Enter:
        on_pushButton_clicked();
        break;
    default:
        break;
    }

    QDialog::keyPressEvent(event);
}


void SceneResizeBox::on_pushButton_clicked()
{
    accept();
}

void SceneResizeBox::on_pushButton_2_clicked()
{
    reject();
}
