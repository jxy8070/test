#include "newsolution.h"
#include "ui_newsolution.h"
#include "customWidget/overwriteitem.h"
#include "../Frame/mainwindow.h"

NewSolution::NewSolution(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSolution)
{
    ui->setupUi(this);
    ui->label_welcome->setText(tr("Welcome to %1").arg(qApp->applicationName()));
    ui->widget_cfgNewSln->setHidden(true);

    Utils::loadProjectExplorer(_HISTFiles);
    QMap<QDateTime, QString> mapFileTime;
    foreach(QString filePath, _HISTFiles) {
        QFileInfo QFileInfo(filePath);
        mapFileTime.insert(QFileInfo.lastRead(), filePath);
    }

    QMapIterator<QDateTime, QString> iter(mapFileTime);
    iter.toBack();
    while(iter.hasPrevious()) {
        iter.previous();
        CustomListWidgetItem *wid = new CustomListWidgetItem(iter.value());
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(QSize(500,70));
        ui->listWidget_earlerSln->addItem(item);
        ui->listWidget_earlerSln->setItemWidget(item, wid);
    }
}

NewSolution::~NewSolution()
{
    //移除和MainWindow的信号绑定
    this->disconnect(MainWindow::instance());
    delete ui;    
}


void NewSolution::initCfgNewSln()
{
    QString defDir;
    Utils::loadDefaultWorkingPath(defDir);
    QString defSlnName = QString("Untitled");
    QDir dir(defDir);
    int i = 1;
    while(!dir.entryList(QStringList() << defSlnName).isEmpty()) {
        defSlnName = QString("Untitled%1").arg(i++);
    }
    ui->lineEdit_proName->setText(defSlnName);
    ui->lineEdit_slnPath->setText(defDir);
    ui->lineEdit_slnName->setText(ui->lineEdit_proName->text());
}


void NewSolution::on_listWidget_earlerSln_itemDoubleClicked(QListWidgetItem *item)
{
    CustomListWidgetItem *customWid = qobject_cast<CustomListWidgetItem *>(ui->listWidget_earlerSln->itemWidget(item));
    if(!customWid) return;

    QString slnFile = customWid->getFilePath();
    emit solutionOpened(slnFile);
}


void NewSolution::on_listWidget_earlerSln_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu;
    menu.addAction(tr("delete"), this, SLOT(delHistoryFile()));
    menu.exec(QCursor::pos());
}


void NewSolution::on_pushButton_newSln_clicked()
{
    ui->widget_welcome->setHidden(true);
    ui->widget_cfgNewSln->setHidden(false);
    initCfgNewSln();
}


void NewSolution::on_pushButton_openSln_clicked()
{
    QString slnFile = QFileDialog::getOpenFileName(this, tr("Open Solution"), QApplication::applicationDirPath(), QString("file(*.sln)"));
    if(slnFile.isEmpty()) return ;
    emit solutionOpened(slnFile);
}


void NewSolution::on_checkBox_clicked(bool checked)
{
    if(checked)
        ui->lineEdit_slnName->setDisabled(true);
    else
        ui->lineEdit_slnName->setDisabled(false);
}


void NewSolution::on_lineEdit_proName_textEdited(const QString &newText)
{
    if(ui->checkBox->isChecked()) {
        ui->lineEdit_slnName->setText(newText);
    }
}


void NewSolution::on_lineEdit_search_returnPressed()
{
    QMap<QString, QString> containItems;
    foreach (QString item, _HISTFiles) {
        if(item.contains(ui->lineEdit_search->text(), Qt::CaseInsensitive))
            containItems.insert(QFileInfo(item).baseName(), item);

    }
    ui->listWidget_earlerSln->clear();
    foreach (QString value, containItems) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(QSize(500,70));
        ui->listWidget_earlerSln->addItem(item);
        ui->listWidget_earlerSln->setItemWidget(item, new CustomListWidgetItem(value));
    }

}


void NewSolution::on_lineEdit_search_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty()) return ;

    ui->listWidget_earlerSln->clear();
    QMap<QDateTime, QString> mapFileTime;
    foreach(QString filePath, _HISTFiles) {
        QFileInfo QFileInfo(filePath);
        mapFileTime.insert(QFileInfo.lastRead(), filePath);
    }

    QMapIterator<QDateTime, QString> iter(mapFileTime);
    iter.toBack();
    while(iter.hasPrevious()) {
        iter.previous();
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(QSize(500,70));
        ui->listWidget_earlerSln->addItem(item);
        ui->listWidget_earlerSln->setItemWidget(item, new CustomListWidgetItem(iter.value()));
    }
}

void NewSolution::delHistoryFile(QString fileName)
{
    if(fileName.isEmpty()) {
        QWidget *wid = ui->listWidget_earlerSln->itemWidget(ui->listWidget_earlerSln->currentItem());
        if(!wid) return ;
        fileName = qobject_cast<CustomListWidgetItem *>(wid)->getFilePath();
    }
    Utils::removeProjectExplorer(fileName);
    _HISTFiles.removeAll(fileName);
    delete ui->listWidget_earlerSln->takeItem(ui->listWidget_earlerSln->currentIndex().row());
}


void NewSolution::on_btn_lastStep_clicked()
{
    ui->widget_welcome->setHidden(false);
    ui->widget_cfgNewSln->setHidden(true);
}


void NewSolution::on_btn_create_clicked()
{
    QDir dir(ui->lineEdit_slnPath->text());
    if(!dir.exists()) {
        MainWindow::instance()->showNotice(tr("The solution path is invalid!"));
        return ;
    }
    //solution dir
    QString slnPath = ui->lineEdit_slnPath->text() + "/" + ui->lineEdit_slnName->text();
    dir.setPath(slnPath);
    if(!dir.exists()) {
        dir.mkdir(slnPath);
    }
    //project dir
    if(!ui->checkBox->isChecked()) {
        QString proPath = slnPath + "/" + ui->lineEdit_proName->text();
        dir.setPath(proPath);
        if(!dir.exists()) {
            dir.mkdir(proPath);
        }
    }
    //solution file
    slnPath += "/" + ui->lineEdit_slnName->text();
    if(!slnPath.contains(".sln")) slnPath.append(".sln");
    QFile file(slnPath);
    if(file.exists()) {
        if(Utils::MessageBoxQuestion(tr("Warning"),
                                     tr("%1 already exists, Continuing will overwrite it!")
                                     .arg(QFileInfo(slnPath).fileName()),
                                     QMessageBox::Yes | QMessageBox::No)
                == QMessageBox::No) return ;
    }
    bool ok = file.open(QIODevice::ReadWrite);
    file.close();
    if(ok) {
        emit solutionOpened(slnPath);
    }
    //default dir
    if(ui->checkBox_setPath->isChecked()) {
        Utils::saveDefaultWorkingPath(ui->lineEdit_slnPath->text());
    }
}

void NewSolution::on_btn_path_clicked()
{
    QString defPath;
    Utils::loadDefaultWorkingPath(defPath);
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select folder"), defPath);
    if(dir.isEmpty()) return ;
    ui->lineEdit_slnPath->setText(dir);
}
