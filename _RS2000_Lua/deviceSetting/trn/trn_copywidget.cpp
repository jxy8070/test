#include "trn_copywidget.h"
#include "ui_trn_copywidget.h"
#include <QDebug>
#include <QMessageBox>

Trn_CopyWidget::Trn_CopyWidget(QWidget *parent) :
    DevSettingWidget(parent),
    ui(new Ui::Trn_CopyWidget)
{
    ui->setupUi(this);
    hide();

    _inStrList << "Eq" << "Dyn";
    _outStrList << "Eq" << "Split" << "Dyn";

    _from = 0;
    _type = 0;

    _checkBoxList.append(ui->AIn1);
    _checkBoxList.append(ui->AIn2);
    _checkBoxList.append(ui->AIn3);
    _checkBoxList.append(ui->AIn4);
    _checkBoxList.append(ui->AIn5);
    _checkBoxList.append(ui->AIn6);
    _checkBoxList.append(ui->AIn7);
    _checkBoxList.append(ui->AIn8);
    _checkBoxList.append(ui->DIn1L);
    _checkBoxList.append(ui->DIn1R);
    _checkBoxList.append(ui->DIn2L);
    _checkBoxList.append(ui->DIn2R);
    _checkBoxList.append(ui->NetIn1);
    _checkBoxList.append(ui->NetIn2);
    _checkBoxList.append(ui->NetIn3);
    _checkBoxList.append(ui->NetIn4);
    _checkBoxList.append(ui->NetIn5);
    _checkBoxList.append(ui->NetIn6);
    _checkBoxList.append(ui->NetIn7);
    _checkBoxList.append(ui->NetIn8);

    _checkBoxList.append(ui->AOut1);
    _checkBoxList.append(ui->AOut2);
    _checkBoxList.append(ui->AOut3);
    _checkBoxList.append(ui->AOut4);
    _checkBoxList.append(ui->AOut5);
    _checkBoxList.append(ui->AOut6);
    _checkBoxList.append(ui->AOut7);
    _checkBoxList.append(ui->AOut8);
    _checkBoxList.append(ui->DOut1L);
    _checkBoxList.append(ui->DOut1R);
    _checkBoxList.append(ui->DOut2L);
    _checkBoxList.append(ui->DOut2R);
    _checkBoxList.append(ui->NetOut1);
    _checkBoxList.append(ui->NetOut2);
    _checkBoxList.append(ui->NetOut3);
    _checkBoxList.append(ui->NetOut4);
    _checkBoxList.append(ui->NetOut5);
    _checkBoxList.append(ui->NetOut6);
    _checkBoxList.append(ui->NetOut7);
    _checkBoxList.append(ui->NetOut8);
}

Trn_CopyWidget::~Trn_CopyWidget()
{
    delete ui;
}

void Trn_CopyWidget::on_comboBox_source_currentIndexChanged(int index)
{
    ui->comboBox_type->clear();
    if (index < 20)
    {
        ui->comboBox_type->addItems(_inStrList);
        clearSelect(CLEAR_BOTTOM);
        _type = 0;
    }
    else
    {
        ui->comboBox_type->addItems(_outStrList);
        clearSelect(CLEAR_TOP);
        _type = 2;
    }

    for (int i = 0; i < _checkBoxList.count(); ++i)
    {
        _checkBoxList.at(i)->setEnabled(true);
    }
    _checkBoxList.at(index)->setDisabled(true);

    _from = index;
}

void Trn_CopyWidget::on_pushButton_clicked()
{
    int size = 0;

    for (int i = 0; i < _checkBoxList.count(); ++i)
    {
        if (_checkBoxList.at(i)->isChecked())
        {
            _to.resize(size);
            _to.append(i);
            size++;
        }
    }
//    qDebug()<<_from<<_type<<"click";
    emit copyValueSig(_from, _type, _to);

    clearSelect(CLEAR_ALL);
}

void Trn_CopyWidget::on_comboBox_type_currentIndexChanged(int index)
{
    if (_from < 20)
    {
        _type = index;
        if (index == 1)
        {
            clearSelect(CLEAR_NONE);
        }
        else
        {
            clearSelect(CLEAR_BOTTOM);
        }
    }
    else
    {
        _type = index + 2;
        if (index == 2)
        {
            clearSelect(CLEAR_NONE);
        }
        else
        {
            clearSelect(CLEAR_TOP);
        }
    }
}

void Trn_CopyWidget::clearSelect(int flag)
{
    int i = 0;
    switch (flag)
    {
    case CLEAR_NONE:
        for (i = 0; i < _checkBoxList.count(); ++i)
        {
            _checkBoxList.at(i)->setVisible(true);
        }
        break;
    case CLEAR_TOP:
        for (i = 0; i < _checkBoxList.count() / 2; ++i)
        {
            _checkBoxList.at(i)->setVisible(false);
        }
        for (; i < _checkBoxList.count(); ++i)
        {
            _checkBoxList.at(i)->setVisible(true);
        }
        break;
    case CLEAR_BOTTOM:
        for (i = 0; i < _checkBoxList.count() / 2; ++i)
        {
            _checkBoxList.at(i)->setVisible(true);
        }
        for (; i < _checkBoxList.count(); ++i)
        {
            _checkBoxList.at(i)->setVisible(false);
        }
        break;
    case CLEAR_ALL:
        ui->comboBox_source->setCurrentIndex(0);
        _from = 0;

        ui->comboBox_type->clear();
        ui->comboBox_type->addItems(_inStrList);
        _type = 0;

        for (i = 0; i < _checkBoxList.count() / 2; ++i)
        {
            _checkBoxList.at(i)->setChecked(false);
        }
        for (; i < _checkBoxList.count(); ++i)
        {
            _checkBoxList.at(i)->setChecked(false);
            _checkBoxList.at(i)->setVisible(false);
        }
        break;
    default:
        break;
    }
}

void Trn_CopyWidget::showEvent(QShowEvent *event)
{
    clearSelect(CLEAR_ALL);

    QWidget::showEvent(event);
}
