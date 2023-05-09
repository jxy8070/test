#include "base_ap400_route.h"
#include "ui_base_ap400_route.h"

Base_AP400_Route::Base_AP400_Route(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Base_AP400_Route)
{
    ui->setupUi(this);
}

Base_AP400_Route::~Base_AP400_Route()
{
    delete ui;
}
