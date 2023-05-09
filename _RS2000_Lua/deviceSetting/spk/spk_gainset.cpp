#include "spk_gainset.h"

GainSet::GainSet(QWidget *parent) :
    DevSettingDlg(parent)
{
    setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);
    setWindowFlags(this->windowFlags() & ~Qt::WindowMaximizeButtonHint);
//    setAttribute(Qt::WA_TranslucentBackground, true);
}

void GainSet::on_closeBtn_clicked()
{
    this->close();
}
