#include "cfgsettingFIRCustom.h"
#include "ui_cfgsettingFIRCustom.h"
#include "common.h"
#include "configItem/configitemFIRCustom.h"
#include <QFileDialog>

CfgSettingFIRCustom::CfgSettingFIRCustom(ConfigItemFIRCustom *cfgItem, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CfgSettingFIRCustom)
{
    ui->setupUi(this);
    _cfgItem = cfgItem;

    ui->eqGraph->initParaList(20, 0, 1*100, EQ_CUSTOM_FIR_CONFIG);
    ui->eqGraph->initParaListEnd();

    ui->pushButton_2->setProperty("form", "Bypass");
    ui->pushButton_2->setChecked(_cfgItem->getBypass());
    ui->eqGraph->setBypass(_cfgItem->getBypass());
}

CfgSettingFIRCustom::~CfgSettingFIRCustom()
{
    delete ui;
}


void CfgSettingFIRCustom::on_pushButton_clicked()
{
    QFileDialog* fd = new QFileDialog(this);
    QString fileName = fd->getOpenFileName(this, tr("Open File"),"./", tr("Excel(*.csv)"));
    if(fileName.isEmpty()) {
        return;
    }

    QDir dir = QDir::current();
    QFile file(dir.filePath(fileName));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"OPEN FILE FAILED";
    }

    QStringList dataList;
    QTextStream filestream(&file);//文本流
    forever {
        QString line = filestream.readLine();
        if (line.isEmpty()) break;

        QStringList row;
        for (const QString& cell: line.split(",")) {
            if (cell.isEmpty()) break;
            dataList.append(cell);
        }
    }
    file.close();//操作完成后记得关闭文件

    _cfgItem->setCoeff(dataList);

    ui->eqGraph->setCoeff(_cfgItem->getCoeff(), _cfgItem->getMaxCoeffSize());
//    ui->eqGraph->initEqParaList(_cfgItem->getCoeff(), dataList.length());
//    ui->eqGraph->initParaListEnd();
//    for (int i = 0; i < 1024; ++i)
//    {
//        QString a = tempbar.value(i);
//        float b = a.toFloat();
//    }
//    qDebug() << __FUNCTION__ << __LINE__ << dataList.count()
//             << dataList;
}

void CfgSettingFIRCustom::on_pushButton_2_clicked(bool checked)
{
    _cfgItem->setBypass(checked);
    ui->eqGraph->setBypass(checked);
}
