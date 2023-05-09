#include "labelediter.h"
#include "ui_LabelEditer.h"
#include "Frame/absgraphscene.h"
#include "linelabel.h"


//通过Widget的键盘事件，触发labelName设置及取消
LabelEditer::LabelEditer(AbsGraphScene* scene) : QWidget(nullptr),
    ui(new Ui::LabelEditer)
{
    _scene = scene;
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);   
    setFocusPolicy(Qt::StrongFocus);    
    ui->lineEdit->installEventFilter(this);
    connect(ui->lineEdit, SIGNAL(textEdited(const QString &)),this, SLOT(onTextEdited(const QString &)));
    connect(ui->listLabel, SIGNAL(itemDoubleClicked(QListWidgetItem *)),this, SLOT(onItemDoubleClicked(QListWidgetItem *)));
}

void LabelEditer::init(QString oldName, QPointF pos, QList<LineLabel*> labelList)
{
    setNameDuplicate(false);
    _oldName = oldName;
    _lastName = oldName;
    ui->lineEdit->setText(oldName);
    ui->lineEdit->setFocus();
    _labelList = labelList;
    _isInput = _labelList[0]->isInput();
    _portType = _labelList[0]->getPortType();
    if(_isInput) {
        move(pos.x() - 4 - width(), pos.y() - 1);
    }
    else {
        move(pos.x() + 13, pos.y() - 1);;
    }
    onTextEdited(ui->lineEdit->text());
    show();
}



void LabelEditer::onTextEdited(const QString &text)
{
    QString filterStr = text.trimmed();
    bool isDuplicate = false;
    if(!_isInput){
        isDuplicate = _scene->isLabelNameDuplicate(filterStr);
        setNameDuplicate(isDuplicate);
    }

    if(!isDuplicate){
        QList<LineLabel*> labels = _scene-> filterLabels(_portType, !_isInput, filterStr);
        ui->listLabel->clear();

        QStringList labelList;
        for(LineLabel* label :labels)
        {
           ui->listLabel->addItem(label->name());
        }
    }
}

void LabelEditer::onItemDoubleClicked(QListWidgetItem *item)
{
    QString labelName = item->text().trimmed();
    onTextComplete(labelName);
}

void LabelEditer::setNameDuplicate(bool isDuplicate)
{
    _nameDuplicate = isDuplicate;
    if(_nameDuplicate)
    {
        ui->lineEdit->setStyleSheet("background-color:rgba(255,100,100, 1)");
        ui->listLabel->clear();

        QListWidgetItem* _errItem = new QListWidgetItem("Signal name already in use");
        _errItem ->setBackground(QBrush(QColor(255,150,150)));
        ui->listLabel->addItem(_errItem);
    }
    else
    {
        ui->lineEdit->setStyleSheet("");
    }
}

void LabelEditer::onTextComplete(const QString &labelName)
{
    if(_nameDuplicate) return;
    if(!labelName.isEmpty())
    {
        if(_lastName != labelName)
        {
            _lastName = labelName;
            _scene->addSetLabelCommand(_labelList, labelName);
        }
    }

    this->hide();
}

bool LabelEditer::eventFilter(QObject *target, QEvent *event)
{
    if( target == ui->lineEdit) {
        if( event->type() == QEvent::KeyPress ) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
            {
                QString labelName = ui->lineEdit->text().trimmed();
                onTextComplete(labelName);
                return true;
            }
            if (keyEvent->key() == Qt::Key_Escape)
            {
                this->hide();
                return true;
            }
        }
    }
    return QWidget::eventFilter(target,event);
}

