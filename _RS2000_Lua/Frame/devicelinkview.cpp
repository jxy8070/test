#include "stable.h"
#include "devicelinkview.h"
#include "ui_devicelinkview.h"
#include "projectscene.h"
#include "deviceItem/deviceItem.h"
#include "device/AES70device.h"

DeviceLinkView::DeviceLinkView(ProjectScene *projScene, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeviceLinkView)
{
    ui->setupUi(this);
    setModal(true);
    _projScene = projScene;
    setAttribute(Qt::WA_DeleteOnClose);
    ui->linkView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->linkView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    initLink();
}

DeviceLinkView::~DeviceLinkView()
{
//    qDebug("~DeviceLink");

    clear();
    delete ui;
}

void DeviceLinkView::initLink()
{
    foreach (QGraphicsItem *item, _projScene->items())
    {
        LineItem* lineItem = qgraphicsitem_cast<LineItem *>(item);
        if(lineItem!= NULL)
        {
            int rows = ui->linkView->rowCount();
            ui->linkView->insertRow(rows);

            QString inPort = lineItem->getStartPort()->portName();
            QString inDevice = qgraphicsitem_cast<DeviceItem *>(lineItem->getStartDevice())->getAES70Device()->getDeviceName();
            QString outPort = lineItem->getEndPort()->portName();
            QString outDevice = qgraphicsitem_cast<DeviceItem *>(lineItem->getEndDevice())->getAES70Device()->getDeviceName();

            m_tableItem.append(new QTableWidgetItem(inPort));
            ui->linkView->setItem(rows, 0, m_tableItem.last());
            m_tableItem.append(new QTableWidgetItem(inDevice));
            ui->linkView->setItem(rows, 1, m_tableItem.last());
            m_tableItem.append(new QTableWidgetItem(outPort));
            ui->linkView->setItem(rows, 2, m_tableItem.last());
            m_tableItem.append(new QTableWidgetItem(outDevice));
            ui->linkView->setItem(rows, 3, m_tableItem.last());

            ui->linkView->item(rows, 0)->setTextAlignment(Qt::AlignCenter);
            ui->linkView->item(rows, 1)->setTextAlignment(Qt::AlignCenter);
            ui->linkView->item(rows, 2)->setTextAlignment(Qt::AlignCenter);
            ui->linkView->item(rows, 3)->setTextAlignment(Qt::AlignCenter);
        }
    }

}

void DeviceLinkView::clear()
{
    qDeleteAll(m_tableItem);
    m_tableItem.clear();

    for (int i = 0; i < ui->linkView->rowCount();)
    {
        ui->linkView->removeRow(i);
    }
}

void DeviceLinkView::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
}

void DeviceLinkView::closeEvent(QCloseEvent *event)
{
    clear();

    QDialog::closeEvent(event);
}

void DeviceLinkView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    default:
        break;
    }

    QDialog::keyPressEvent(event);
}

void DeviceLinkView::on_exit_clicked()
{
    close();
}

void DeviceLinkView::on_saveas_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("save as..."), QDir::currentPath(), tr("file (*.txt)"));
    if(fileName == NULL)
    {
        return;
    }

    QFile f(fileName);
    f.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream ds(&f);

    ds << "In Device\t"
       << "In Channel\t"
       << "Out Device\t"
       << "Out Channel\n";

    for (int rows = 0; rows < ui->linkView->rowCount(); ++rows)
    {
        ds << ui->linkView->item(rows, 1)->text() << "\t" << ui->linkView->item(rows, 0)->text() << "\t\t";
        ds << ui->linkView->item(rows, 3)->text() << "\t\t" << ui->linkView->item(rows, 2)->text() << "\n";
    }

    f.close();
}
