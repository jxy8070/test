#include "stable.h"
#include "trn_mixerwidget.h"
#include "ui_trn_mixerwidget.h"

Trn_MixerWidget::Trn_MixerWidget(QWidget *parent) :
    DevSettingWidget(parent),
    _ui(new Ui::Trn_MixerWidget)
{
    _ui->setupUi(this);
    hide();

    connect(_ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_2, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_3, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_4, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_5, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_6, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_7, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_8, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_9, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_10, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_11, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_12, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_13, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_14, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_15, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_16, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_17, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_18, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_19, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));
    connect(_ui->doubleSpinBox_20, SIGNAL(valueChanged(double)), this, SLOT(faderSliderChanged(double)));

    connect(_ui->MO1, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO2, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO3, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO4, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO5, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO6, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO7, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO8, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO9, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO10, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO11, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO12, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO13, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO14, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO15, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO16, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO17, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO18, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO19, SIGNAL(clicked()), this, SLOT(faderSliderControl()));
    connect(_ui->MO20, SIGNAL(clicked()), this, SLOT(faderSliderControl()));

    m_pushButtonList.append(_ui->MI1_MO1);
    m_pushButtonList.append(_ui->MI2_MO1);
    m_pushButtonList.append(_ui->MI3_MO1);
    m_pushButtonList.append(_ui->MI4_MO1);
    m_pushButtonList.append(_ui->MI5_MO1);
    m_pushButtonList.append(_ui->MI6_MO1);
    m_pushButtonList.append(_ui->MI7_MO1);
    m_pushButtonList.append(_ui->MI8_MO1);
    m_pushButtonList.append(_ui->MI9_MO1);
    m_pushButtonList.append(_ui->MI10_MO1);
    m_pushButtonList.append(_ui->MI11_MO1);
    m_pushButtonList.append(_ui->MI12_MO1);
    m_pushButtonList.append(_ui->MI13_MO1);
    m_pushButtonList.append(_ui->MI14_MO1);
    m_pushButtonList.append(_ui->MI15_MO1);
    m_pushButtonList.append(_ui->MI16_MO1);
    m_pushButtonList.append(_ui->MI17_MO1);
    m_pushButtonList.append(_ui->MI18_MO1);
    m_pushButtonList.append(_ui->MI19_MO1);
    m_pushButtonList.append(_ui->MI20_MO1);
    m_pushButtonList.append(_ui->MI1_MO2);
    m_pushButtonList.append(_ui->MI2_MO2);
    m_pushButtonList.append(_ui->MI3_MO2);
    m_pushButtonList.append(_ui->MI4_MO2);
    m_pushButtonList.append(_ui->MI5_MO2);
    m_pushButtonList.append(_ui->MI6_MO2);
    m_pushButtonList.append(_ui->MI7_MO2);
    m_pushButtonList.append(_ui->MI8_MO2);
    m_pushButtonList.append(_ui->MI9_MO2);
    m_pushButtonList.append(_ui->MI10_MO2);
    m_pushButtonList.append(_ui->MI11_MO2);
    m_pushButtonList.append(_ui->MI12_MO2);
    m_pushButtonList.append(_ui->MI13_MO2);
    m_pushButtonList.append(_ui->MI14_MO2);
    m_pushButtonList.append(_ui->MI15_MO2);
    m_pushButtonList.append(_ui->MI16_MO2);
    m_pushButtonList.append(_ui->MI17_MO2);
    m_pushButtonList.append(_ui->MI18_MO2);
    m_pushButtonList.append(_ui->MI19_MO2);
    m_pushButtonList.append(_ui->MI20_MO2);
    m_pushButtonList.append(_ui->MI1_MO3);
    m_pushButtonList.append(_ui->MI2_MO3);
    m_pushButtonList.append(_ui->MI3_MO3);
    m_pushButtonList.append(_ui->MI4_MO3);
    m_pushButtonList.append(_ui->MI5_MO3);
    m_pushButtonList.append(_ui->MI6_MO3);
    m_pushButtonList.append(_ui->MI7_MO3);
    m_pushButtonList.append(_ui->MI8_MO3);
    m_pushButtonList.append(_ui->MI9_MO3);
    m_pushButtonList.append(_ui->MI10_MO3);
    m_pushButtonList.append(_ui->MI11_MO3);
    m_pushButtonList.append(_ui->MI12_MO3);
    m_pushButtonList.append(_ui->MI13_MO3);
    m_pushButtonList.append(_ui->MI14_MO3);
    m_pushButtonList.append(_ui->MI15_MO3);
    m_pushButtonList.append(_ui->MI16_MO3);
    m_pushButtonList.append(_ui->MI17_MO3);
    m_pushButtonList.append(_ui->MI18_MO3);
    m_pushButtonList.append(_ui->MI19_MO3);
    m_pushButtonList.append(_ui->MI20_MO3);
    m_pushButtonList.append(_ui->MI1_MO4);
    m_pushButtonList.append(_ui->MI2_MO4);
    m_pushButtonList.append(_ui->MI3_MO4);
    m_pushButtonList.append(_ui->MI4_MO4);
    m_pushButtonList.append(_ui->MI5_MO4);
    m_pushButtonList.append(_ui->MI6_MO4);
    m_pushButtonList.append(_ui->MI7_MO4);
    m_pushButtonList.append(_ui->MI8_MO4);
    m_pushButtonList.append(_ui->MI9_MO4);
    m_pushButtonList.append(_ui->MI10_MO4);
    m_pushButtonList.append(_ui->MI11_MO4);
    m_pushButtonList.append(_ui->MI12_MO4);
    m_pushButtonList.append(_ui->MI13_MO4);
    m_pushButtonList.append(_ui->MI14_MO4);
    m_pushButtonList.append(_ui->MI15_MO4);
    m_pushButtonList.append(_ui->MI16_MO4);
    m_pushButtonList.append(_ui->MI17_MO4);
    m_pushButtonList.append(_ui->MI18_MO4);
    m_pushButtonList.append(_ui->MI19_MO4);
    m_pushButtonList.append(_ui->MI20_MO4);
    m_pushButtonList.append(_ui->MI1_MO5);
    m_pushButtonList.append(_ui->MI2_MO5);
    m_pushButtonList.append(_ui->MI3_MO5);
    m_pushButtonList.append(_ui->MI4_MO5);
    m_pushButtonList.append(_ui->MI5_MO5);
    m_pushButtonList.append(_ui->MI6_MO5);
    m_pushButtonList.append(_ui->MI7_MO5);
    m_pushButtonList.append(_ui->MI8_MO5);
    m_pushButtonList.append(_ui->MI9_MO5);
    m_pushButtonList.append(_ui->MI10_MO5);
    m_pushButtonList.append(_ui->MI11_MO5);
    m_pushButtonList.append(_ui->MI12_MO5);
    m_pushButtonList.append(_ui->MI13_MO5);
    m_pushButtonList.append(_ui->MI14_MO5);
    m_pushButtonList.append(_ui->MI15_MO5);
    m_pushButtonList.append(_ui->MI16_MO5);
    m_pushButtonList.append(_ui->MI17_MO5);
    m_pushButtonList.append(_ui->MI18_MO5);
    m_pushButtonList.append(_ui->MI19_MO5);
    m_pushButtonList.append(_ui->MI20_MO5);
    m_pushButtonList.append(_ui->MI1_MO6);
    m_pushButtonList.append(_ui->MI2_MO6);
    m_pushButtonList.append(_ui->MI3_MO6);
    m_pushButtonList.append(_ui->MI4_MO6);
    m_pushButtonList.append(_ui->MI5_MO6);
    m_pushButtonList.append(_ui->MI6_MO6);
    m_pushButtonList.append(_ui->MI7_MO6);
    m_pushButtonList.append(_ui->MI8_MO6);
    m_pushButtonList.append(_ui->MI9_MO6);
    m_pushButtonList.append(_ui->MI10_MO6);
    m_pushButtonList.append(_ui->MI11_MO6);
    m_pushButtonList.append(_ui->MI12_MO6);
    m_pushButtonList.append(_ui->MI13_MO6);
    m_pushButtonList.append(_ui->MI14_MO6);
    m_pushButtonList.append(_ui->MI15_MO6);
    m_pushButtonList.append(_ui->MI16_MO6);
    m_pushButtonList.append(_ui->MI17_MO6);
    m_pushButtonList.append(_ui->MI18_MO6);
    m_pushButtonList.append(_ui->MI19_MO6);
    m_pushButtonList.append(_ui->MI20_MO6);
    m_pushButtonList.append(_ui->MI1_MO7);
    m_pushButtonList.append(_ui->MI2_MO7);
    m_pushButtonList.append(_ui->MI3_MO7);
    m_pushButtonList.append(_ui->MI4_MO7);
    m_pushButtonList.append(_ui->MI5_MO7);
    m_pushButtonList.append(_ui->MI6_MO7);
    m_pushButtonList.append(_ui->MI7_MO7);
    m_pushButtonList.append(_ui->MI8_MO7);
    m_pushButtonList.append(_ui->MI9_MO7);
    m_pushButtonList.append(_ui->MI10_MO7);
    m_pushButtonList.append(_ui->MI11_MO7);
    m_pushButtonList.append(_ui->MI12_MO7);
    m_pushButtonList.append(_ui->MI13_MO7);
    m_pushButtonList.append(_ui->MI14_MO7);
    m_pushButtonList.append(_ui->MI15_MO7);
    m_pushButtonList.append(_ui->MI16_MO7);
    m_pushButtonList.append(_ui->MI17_MO7);
    m_pushButtonList.append(_ui->MI18_MO7);
    m_pushButtonList.append(_ui->MI19_MO7);
    m_pushButtonList.append(_ui->MI20_MO7);
    m_pushButtonList.append(_ui->MI1_MO8);
    m_pushButtonList.append(_ui->MI2_MO8);
    m_pushButtonList.append(_ui->MI3_MO8);
    m_pushButtonList.append(_ui->MI4_MO8);
    m_pushButtonList.append(_ui->MI5_MO8);
    m_pushButtonList.append(_ui->MI6_MO8);
    m_pushButtonList.append(_ui->MI7_MO8);
    m_pushButtonList.append(_ui->MI8_MO8);
    m_pushButtonList.append(_ui->MI9_MO8);
    m_pushButtonList.append(_ui->MI10_MO8);
    m_pushButtonList.append(_ui->MI11_MO8);
    m_pushButtonList.append(_ui->MI12_MO8);
    m_pushButtonList.append(_ui->MI13_MO8);
    m_pushButtonList.append(_ui->MI14_MO8);
    m_pushButtonList.append(_ui->MI15_MO8);
    m_pushButtonList.append(_ui->MI16_MO8);
    m_pushButtonList.append(_ui->MI17_MO8);
    m_pushButtonList.append(_ui->MI18_MO8);
    m_pushButtonList.append(_ui->MI19_MO8);
    m_pushButtonList.append(_ui->MI20_MO8);
    m_pushButtonList.append(_ui->MI1_MO9);
    m_pushButtonList.append(_ui->MI2_MO9);
    m_pushButtonList.append(_ui->MI3_MO9);
    m_pushButtonList.append(_ui->MI4_MO9);
    m_pushButtonList.append(_ui->MI5_MO9);
    m_pushButtonList.append(_ui->MI6_MO9);
    m_pushButtonList.append(_ui->MI7_MO9);
    m_pushButtonList.append(_ui->MI8_MO9);
    m_pushButtonList.append(_ui->MI9_MO9);
    m_pushButtonList.append(_ui->MI10_MO9);
    m_pushButtonList.append(_ui->MI11_MO9);
    m_pushButtonList.append(_ui->MI12_MO9);
    m_pushButtonList.append(_ui->MI13_MO9);
    m_pushButtonList.append(_ui->MI14_MO9);
    m_pushButtonList.append(_ui->MI15_MO9);
    m_pushButtonList.append(_ui->MI16_MO9);
    m_pushButtonList.append(_ui->MI17_MO9);
    m_pushButtonList.append(_ui->MI18_MO9);
    m_pushButtonList.append(_ui->MI19_MO9);
    m_pushButtonList.append(_ui->MI20_MO9);
    m_pushButtonList.append(_ui->MI1_MO10);
    m_pushButtonList.append(_ui->MI2_MO10);
    m_pushButtonList.append(_ui->MI3_MO10);
    m_pushButtonList.append(_ui->MI4_MO10);
    m_pushButtonList.append(_ui->MI5_MO10);
    m_pushButtonList.append(_ui->MI6_MO10);
    m_pushButtonList.append(_ui->MI7_MO10);
    m_pushButtonList.append(_ui->MI8_MO10);
    m_pushButtonList.append(_ui->MI9_MO10);
    m_pushButtonList.append(_ui->MI10_MO10);
    m_pushButtonList.append(_ui->MI11_MO10);
    m_pushButtonList.append(_ui->MI12_MO10);
    m_pushButtonList.append(_ui->MI13_MO10);
    m_pushButtonList.append(_ui->MI14_MO10);
    m_pushButtonList.append(_ui->MI15_MO10);
    m_pushButtonList.append(_ui->MI16_MO10);
    m_pushButtonList.append(_ui->MI17_MO10);
    m_pushButtonList.append(_ui->MI18_MO10);
    m_pushButtonList.append(_ui->MI19_MO10);
    m_pushButtonList.append(_ui->MI20_MO10);
    m_pushButtonList.append(_ui->MI1_MO11);
    m_pushButtonList.append(_ui->MI2_MO11);
    m_pushButtonList.append(_ui->MI3_MO11);
    m_pushButtonList.append(_ui->MI4_MO11);
    m_pushButtonList.append(_ui->MI5_MO11);
    m_pushButtonList.append(_ui->MI6_MO11);
    m_pushButtonList.append(_ui->MI7_MO11);
    m_pushButtonList.append(_ui->MI8_MO11);
    m_pushButtonList.append(_ui->MI9_MO11);
    m_pushButtonList.append(_ui->MI10_MO11);
    m_pushButtonList.append(_ui->MI11_MO11);
    m_pushButtonList.append(_ui->MI12_MO11);
    m_pushButtonList.append(_ui->MI13_MO11);
    m_pushButtonList.append(_ui->MI14_MO11);
    m_pushButtonList.append(_ui->MI15_MO11);
    m_pushButtonList.append(_ui->MI16_MO11);
    m_pushButtonList.append(_ui->MI17_MO11);
    m_pushButtonList.append(_ui->MI18_MO11);
    m_pushButtonList.append(_ui->MI19_MO11);
    m_pushButtonList.append(_ui->MI20_MO11);
    m_pushButtonList.append(_ui->MI1_MO12);
    m_pushButtonList.append(_ui->MI2_MO12);
    m_pushButtonList.append(_ui->MI3_MO12);
    m_pushButtonList.append(_ui->MI4_MO12);
    m_pushButtonList.append(_ui->MI5_MO12);
    m_pushButtonList.append(_ui->MI6_MO12);
    m_pushButtonList.append(_ui->MI7_MO12);
    m_pushButtonList.append(_ui->MI8_MO12);
    m_pushButtonList.append(_ui->MI9_MO12);
    m_pushButtonList.append(_ui->MI10_MO12);
    m_pushButtonList.append(_ui->MI11_MO12);
    m_pushButtonList.append(_ui->MI12_MO12);
    m_pushButtonList.append(_ui->MI13_MO12);
    m_pushButtonList.append(_ui->MI14_MO12);
    m_pushButtonList.append(_ui->MI15_MO12);
    m_pushButtonList.append(_ui->MI16_MO12);
    m_pushButtonList.append(_ui->MI17_MO12);
    m_pushButtonList.append(_ui->MI18_MO12);
    m_pushButtonList.append(_ui->MI19_MO12);
    m_pushButtonList.append(_ui->MI20_MO12);
    m_pushButtonList.append(_ui->MI1_MO13);
    m_pushButtonList.append(_ui->MI2_MO13);
    m_pushButtonList.append(_ui->MI3_MO13);
    m_pushButtonList.append(_ui->MI4_MO13);
    m_pushButtonList.append(_ui->MI5_MO13);
    m_pushButtonList.append(_ui->MI6_MO13);
    m_pushButtonList.append(_ui->MI7_MO13);
    m_pushButtonList.append(_ui->MI8_MO13);
    m_pushButtonList.append(_ui->MI9_MO13);
    m_pushButtonList.append(_ui->MI10_MO13);
    m_pushButtonList.append(_ui->MI11_MO13);
    m_pushButtonList.append(_ui->MI12_MO13);
    m_pushButtonList.append(_ui->MI13_MO13);
    m_pushButtonList.append(_ui->MI14_MO13);
    m_pushButtonList.append(_ui->MI15_MO13);
    m_pushButtonList.append(_ui->MI16_MO13);
    m_pushButtonList.append(_ui->MI17_MO13);
    m_pushButtonList.append(_ui->MI18_MO13);
    m_pushButtonList.append(_ui->MI19_MO13);
    m_pushButtonList.append(_ui->MI20_MO13);
    m_pushButtonList.append(_ui->MI1_MO14);
    m_pushButtonList.append(_ui->MI2_MO14);
    m_pushButtonList.append(_ui->MI3_MO14);
    m_pushButtonList.append(_ui->MI4_MO14);
    m_pushButtonList.append(_ui->MI5_MO14);
    m_pushButtonList.append(_ui->MI6_MO14);
    m_pushButtonList.append(_ui->MI7_MO14);
    m_pushButtonList.append(_ui->MI8_MO14);
    m_pushButtonList.append(_ui->MI9_MO14);
    m_pushButtonList.append(_ui->MI10_MO14);
    m_pushButtonList.append(_ui->MI11_MO14);
    m_pushButtonList.append(_ui->MI12_MO14);
    m_pushButtonList.append(_ui->MI13_MO14);
    m_pushButtonList.append(_ui->MI14_MO14);
    m_pushButtonList.append(_ui->MI15_MO14);
    m_pushButtonList.append(_ui->MI16_MO14);
    m_pushButtonList.append(_ui->MI17_MO14);
    m_pushButtonList.append(_ui->MI18_MO14);
    m_pushButtonList.append(_ui->MI19_MO14);
    m_pushButtonList.append(_ui->MI20_MO14);
    m_pushButtonList.append(_ui->MI1_MO15);
    m_pushButtonList.append(_ui->MI2_MO15);
    m_pushButtonList.append(_ui->MI3_MO15);
    m_pushButtonList.append(_ui->MI4_MO15);
    m_pushButtonList.append(_ui->MI5_MO15);
    m_pushButtonList.append(_ui->MI6_MO15);
    m_pushButtonList.append(_ui->MI7_MO15);
    m_pushButtonList.append(_ui->MI8_MO15);
    m_pushButtonList.append(_ui->MI9_MO15);
    m_pushButtonList.append(_ui->MI10_MO15);
    m_pushButtonList.append(_ui->MI11_MO15);
    m_pushButtonList.append(_ui->MI12_MO15);
    m_pushButtonList.append(_ui->MI13_MO15);
    m_pushButtonList.append(_ui->MI14_MO15);
    m_pushButtonList.append(_ui->MI15_MO15);
    m_pushButtonList.append(_ui->MI16_MO15);
    m_pushButtonList.append(_ui->MI17_MO15);
    m_pushButtonList.append(_ui->MI18_MO15);
    m_pushButtonList.append(_ui->MI19_MO15);
    m_pushButtonList.append(_ui->MI20_MO15);
    m_pushButtonList.append(_ui->MI1_MO16);
    m_pushButtonList.append(_ui->MI2_MO16);
    m_pushButtonList.append(_ui->MI3_MO16);
    m_pushButtonList.append(_ui->MI4_MO16);
    m_pushButtonList.append(_ui->MI5_MO16);
    m_pushButtonList.append(_ui->MI6_MO16);
    m_pushButtonList.append(_ui->MI7_MO16);
    m_pushButtonList.append(_ui->MI8_MO16);
    m_pushButtonList.append(_ui->MI9_MO16);
    m_pushButtonList.append(_ui->MI10_MO16);
    m_pushButtonList.append(_ui->MI11_MO16);
    m_pushButtonList.append(_ui->MI12_MO16);
    m_pushButtonList.append(_ui->MI13_MO16);
    m_pushButtonList.append(_ui->MI14_MO16);
    m_pushButtonList.append(_ui->MI15_MO16);
    m_pushButtonList.append(_ui->MI16_MO16);
    m_pushButtonList.append(_ui->MI17_MO16);
    m_pushButtonList.append(_ui->MI18_MO16);
    m_pushButtonList.append(_ui->MI19_MO16);
    m_pushButtonList.append(_ui->MI20_MO16);
    m_pushButtonList.append(_ui->MI1_MO17);
    m_pushButtonList.append(_ui->MI2_MO17);
    m_pushButtonList.append(_ui->MI3_MO17);
    m_pushButtonList.append(_ui->MI4_MO17);
    m_pushButtonList.append(_ui->MI5_MO17);
    m_pushButtonList.append(_ui->MI6_MO17);
    m_pushButtonList.append(_ui->MI7_MO17);
    m_pushButtonList.append(_ui->MI8_MO17);
    m_pushButtonList.append(_ui->MI9_MO17);
    m_pushButtonList.append(_ui->MI10_MO17);
    m_pushButtonList.append(_ui->MI11_MO17);
    m_pushButtonList.append(_ui->MI12_MO17);
    m_pushButtonList.append(_ui->MI13_MO17);
    m_pushButtonList.append(_ui->MI14_MO17);
    m_pushButtonList.append(_ui->MI15_MO17);
    m_pushButtonList.append(_ui->MI16_MO17);
    m_pushButtonList.append(_ui->MI17_MO17);
    m_pushButtonList.append(_ui->MI18_MO17);
    m_pushButtonList.append(_ui->MI19_MO17);
    m_pushButtonList.append(_ui->MI20_MO17);
    m_pushButtonList.append(_ui->MI1_MO18);
    m_pushButtonList.append(_ui->MI2_MO18);
    m_pushButtonList.append(_ui->MI3_MO18);
    m_pushButtonList.append(_ui->MI4_MO18);
    m_pushButtonList.append(_ui->MI5_MO18);
    m_pushButtonList.append(_ui->MI6_MO18);
    m_pushButtonList.append(_ui->MI7_MO18);
    m_pushButtonList.append(_ui->MI8_MO18);
    m_pushButtonList.append(_ui->MI9_MO18);
    m_pushButtonList.append(_ui->MI10_MO18);
    m_pushButtonList.append(_ui->MI11_MO18);
    m_pushButtonList.append(_ui->MI12_MO18);
    m_pushButtonList.append(_ui->MI13_MO18);
    m_pushButtonList.append(_ui->MI14_MO18);
    m_pushButtonList.append(_ui->MI15_MO18);
    m_pushButtonList.append(_ui->MI16_MO18);
    m_pushButtonList.append(_ui->MI17_MO18);
    m_pushButtonList.append(_ui->MI18_MO18);
    m_pushButtonList.append(_ui->MI19_MO18);
    m_pushButtonList.append(_ui->MI20_MO18);
    m_pushButtonList.append(_ui->MI1_MO19);
    m_pushButtonList.append(_ui->MI2_MO19);
    m_pushButtonList.append(_ui->MI3_MO19);
    m_pushButtonList.append(_ui->MI4_MO19);
    m_pushButtonList.append(_ui->MI5_MO19);
    m_pushButtonList.append(_ui->MI6_MO19);
    m_pushButtonList.append(_ui->MI7_MO19);
    m_pushButtonList.append(_ui->MI8_MO19);
    m_pushButtonList.append(_ui->MI9_MO19);
    m_pushButtonList.append(_ui->MI10_MO19);
    m_pushButtonList.append(_ui->MI11_MO19);
    m_pushButtonList.append(_ui->MI12_MO19);
    m_pushButtonList.append(_ui->MI13_MO19);
    m_pushButtonList.append(_ui->MI14_MO19);
    m_pushButtonList.append(_ui->MI15_MO19);
    m_pushButtonList.append(_ui->MI16_MO19);
    m_pushButtonList.append(_ui->MI17_MO19);
    m_pushButtonList.append(_ui->MI18_MO19);
    m_pushButtonList.append(_ui->MI19_MO19);
    m_pushButtonList.append(_ui->MI20_MO19);
    m_pushButtonList.append(_ui->MI1_MO20);
    m_pushButtonList.append(_ui->MI2_MO20);
    m_pushButtonList.append(_ui->MI3_MO20);
    m_pushButtonList.append(_ui->MI4_MO20);
    m_pushButtonList.append(_ui->MI5_MO20);
    m_pushButtonList.append(_ui->MI6_MO20);
    m_pushButtonList.append(_ui->MI7_MO20);
    m_pushButtonList.append(_ui->MI8_MO20);
    m_pushButtonList.append(_ui->MI9_MO20);
    m_pushButtonList.append(_ui->MI10_MO20);
    m_pushButtonList.append(_ui->MI11_MO20);
    m_pushButtonList.append(_ui->MI12_MO20);
    m_pushButtonList.append(_ui->MI13_MO20);
    m_pushButtonList.append(_ui->MI14_MO20);
    m_pushButtonList.append(_ui->MI15_MO20);
    m_pushButtonList.append(_ui->MI16_MO20);
    m_pushButtonList.append(_ui->MI17_MO20);
    m_pushButtonList.append(_ui->MI18_MO20);
    m_pushButtonList.append(_ui->MI19_MO20);
    m_pushButtonList.append(_ui->MI20_MO20);

    m_currentOut = CH1;
    m_faderSliderReInit = false;
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            m_MixerSliderValue[i][j] = 0;
            m_MixerSwitch[i][j] = m_pushButtonList.value(i*20+j)->isChecked();
            connect(m_pushButtonList.value(i*20+j), SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
        }
    }

    _lineEditUserName.append(_ui->lineEdit_MI1);
    _lineEditUserName.append(_ui->lineEdit_MI2);
    _lineEditUserName.append(_ui->lineEdit_MI3);
    _lineEditUserName.append(_ui->lineEdit_MI4);
    _lineEditUserName.append(_ui->lineEdit_MI5);
    _lineEditUserName.append(_ui->lineEdit_MI6);
    _lineEditUserName.append(_ui->lineEdit_MI7);
    _lineEditUserName.append(_ui->lineEdit_MI8);
    _lineEditUserName.append(_ui->lineEdit_MI9);
    _lineEditUserName.append(_ui->lineEdit_MI10);
    _lineEditUserName.append(_ui->lineEdit_MI11);
    _lineEditUserName.append(_ui->lineEdit_MI12);
    _lineEditUserName.append(_ui->lineEdit_MI13);
    _lineEditUserName.append(_ui->lineEdit_MI14);
    _lineEditUserName.append(_ui->lineEdit_MI15);
    _lineEditUserName.append(_ui->lineEdit_MI16);
    _lineEditUserName.append(_ui->lineEdit_MI17);
    _lineEditUserName.append(_ui->lineEdit_MI18);
    _lineEditUserName.append(_ui->lineEdit_MI19);
    _lineEditUserName.append(_ui->lineEdit_MI20);
    _lineEditUserName.append(_ui->lineEdit_MO1);
    _lineEditUserName.append(_ui->lineEdit_MO2);
    _lineEditUserName.append(_ui->lineEdit_MO3);
    _lineEditUserName.append(_ui->lineEdit_MO4);
    _lineEditUserName.append(_ui->lineEdit_MO5);
    _lineEditUserName.append(_ui->lineEdit_MO6);
    _lineEditUserName.append(_ui->lineEdit_MO7);
    _lineEditUserName.append(_ui->lineEdit_MO8);
    _lineEditUserName.append(_ui->lineEdit_MO9);
    _lineEditUserName.append(_ui->lineEdit_MO10);
    _lineEditUserName.append(_ui->lineEdit_MO11);
    _lineEditUserName.append(_ui->lineEdit_MO12);
    _lineEditUserName.append(_ui->lineEdit_MO13);
    _lineEditUserName.append(_ui->lineEdit_MO14);
    _lineEditUserName.append(_ui->lineEdit_MO15);
    _lineEditUserName.append(_ui->lineEdit_MO16);
    _lineEditUserName.append(_ui->lineEdit_MO17);
    _lineEditUserName.append(_ui->lineEdit_MO18);
    _lineEditUserName.append(_ui->lineEdit_MO19);
    _lineEditUserName.append(_ui->lineEdit_MO20);
    for (int i = 0; i < _lineEditUserName.count(); ++i)
    {
        connect(_lineEditUserName.at(i), SIGNAL(editingFinished()), this, SLOT(editdName()));
    }

    _ui->pushButton->clicked(false);
}

Trn_MixerWidget::~Trn_MixerWidget()
{
    delete _ui;
}

void Trn_MixerWidget::editdName()
{
    int ch;
    QString newName;

    ch = _lineEditUserName.indexOf(qobject_cast<QLineEdit *>(sender()));
    newName = qobject_cast<QLineEdit *>(sender())->text();

    emit editNameChanged(ch, newName);
}

void Trn_MixerWidget::showWidget(quint32 mixerData[32][32])
{
    for (int i = 0; i < 32; ++i)
    {
        for (int j = 0; j < 32; ++j)
        {
            if (mixerData[j][i] & 0x00200000)//交叉点打开
            {
                m_pushButtonList.value(i*20+j)->setChecked(true);
                m_MixerSliderValue[i][j] = mixerData[j][i] & 0xffdfffff;
                m_MixerSwitch[i][j] = true;
            } else {
                m_pushButtonList.value(i*20+j)->setChecked(false);
                m_MixerSliderValue[i][j] = mixerData[j][i] & 0xffdfffff;
                m_MixerSwitch[i][j] = false;
            }
        }
    }

    _ui->MO1->click();
    show();
}

void Trn_MixerWidget::userNameInit(QStringList newName)
{
    for (int i = 0; i < _lineEditUserName.count(); ++i)
    {
        _lineEditUserName.at(i)->setText(newName.at(i));
    }
}

void Trn_MixerWidget::faderSliderControl()
{
    m_faderSliderReInit = true;

    if (sender() == _ui->MO1)
    {
        _ui->OutLabel->setText(tr("MO1 FAD Decrease(-60~0dB)"));
        m_currentOut = CH1;
    }
    else if (sender() == _ui->MO2)
    {
        _ui->OutLabel->setText(tr("MO2 FAD Decrease(-60~0dB)"));
        m_currentOut = CH2;
    }
    else if (sender() == _ui->MO3)
    {
        _ui->OutLabel->setText(tr("MO3 FAD Decrease(-60~0dB)"));
        m_currentOut = CH3;
    }
    else if (sender() == _ui->MO4)
    {
        _ui->OutLabel->setText(tr("MO4 FAD Decrease(-60~0dB)"));
        m_currentOut = CH4;
    }
    else if (sender() == _ui->MO5)
    {
        _ui->OutLabel->setText(tr("MO5 FAD Decrease(-60~0dB)"));
        m_currentOut = CH5;
    }
    else if (sender() == _ui->MO6)
    {
        _ui->OutLabel->setText(tr("MO6 FAD Decrease(-60~0dB)"));
        m_currentOut = CH6;
    }
    else if (sender() == _ui->MO7)
    {
        _ui->OutLabel->setText(tr("MO7 FAD Decrease(-60~0dB)"));
        m_currentOut = CH7;
    }
    else if (sender() == _ui->MO8)
    {
        _ui->OutLabel->setText(tr("MO8 FAD Decrease(-60~0dB)"));
        m_currentOut = CH8;
    }
    else if (sender() == _ui->MO9)
    {
        _ui->OutLabel->setText(tr("MO9 FAD Decrease(-60~0dB)"));
        m_currentOut = CH9;
    }
    else if (sender() == _ui->MO10)
    {
        _ui->OutLabel->setText(tr("MO10 FAD Decrease(-60~0dB)"));
        m_currentOut = CH10;
    }
    else if (sender() == _ui->MO11)
    {
        _ui->OutLabel->setText(tr("MO11 FAD Decrease(-60~0dB)"));
        m_currentOut = CH11;
    }
    else if (sender() == _ui->MO12)
    {
        _ui->OutLabel->setText(tr("MO12 FAD Decrease(-60~0dB)"));
        m_currentOut = CH12;
    }
    else if (sender() == _ui->MO13)
    {
        _ui->OutLabel->setText(tr("MO13 FAD Decrease(-60~0dB)"));
        m_currentOut = CH13;
    }
    else if (sender() == _ui->MO14)
    {
        _ui->OutLabel->setText(tr("MO14 FAD Decrease(-60~0dB)"));
        m_currentOut = CH14;
    }
    else if (sender() == _ui->MO15)
    {
        _ui->OutLabel->setText(tr("MO15 FAD Decrease(-60~0dB)"));
        m_currentOut = CH15;
    }
    else if (sender() == _ui->MO16)
    {
        _ui->OutLabel->setText(tr("MO16 FAD Decrease(-60~0dB)"));
        m_currentOut = CH16;
    }
    else if (sender() == _ui->MO17)
    {
        _ui->OutLabel->setText(tr("MO17 FAD Decrease(-60~0dB)"));
        m_currentOut = CH17;
    }
    else if (sender() == _ui->MO18)
    {
        _ui->OutLabel->setText(tr("MO18 FAD Decrease(-60~0dB)"));
        m_currentOut = CH18;
    }
    else if (sender() == _ui->MO19)
    {
        _ui->OutLabel->setText(tr("MO19 FAD Decrease(-60~0dB)"));
        m_currentOut = CH19;
    }
    else if (sender() == _ui->MO20)
    {
        _ui->OutLabel->setText(tr("MO20 FAD Decrease(-60~0dB)"));
        m_currentOut = CH20;
    }

    _ui->doubleSpinBox->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH1] / 131072.0));
    _ui->doubleSpinBox_2->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH2] / 131072.0));
    _ui->doubleSpinBox_3->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH3] / 131072.0));
    _ui->doubleSpinBox_4->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH4] / 131072.0));
    _ui->doubleSpinBox_5->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH5] / 131072.0));
    _ui->doubleSpinBox_6->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH6] / 131072.0));
    _ui->doubleSpinBox_7->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH7] / 131072.0));
    _ui->doubleSpinBox_8->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH8] / 131072.0));
    _ui->doubleSpinBox_9->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH9] / 131072.0));
    _ui->doubleSpinBox_10->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH10] / 131072.0));
    _ui->doubleSpinBox_11->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH11] / 131072.0));
    _ui->doubleSpinBox_12->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH12] / 131072.0));
    _ui->doubleSpinBox_13->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH13] / 131072.0));
    _ui->doubleSpinBox_14->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH14] / 131072.0));
    _ui->doubleSpinBox_15->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH15] / 131072.0));
    _ui->doubleSpinBox_16->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH16] / 131072.0));
    _ui->doubleSpinBox_17->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH17] / 131072.0));
    _ui->doubleSpinBox_18->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH18] / 131072.0));
    _ui->doubleSpinBox_19->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH19] / 131072.0));
    _ui->doubleSpinBox_20->setValue(20.0 * log10(m_MixerSliderValue[m_currentOut][CH20] / 131072.0));

    m_faderSliderReInit = false;
}

void Trn_MixerWidget::faderSliderChanged(double vlu)
{
    if (m_faderSliderReInit)
        return;

    int col = 0;
    if (sender() == _ui->doubleSpinBox)
    {
        col = CH1;
    }
    else if (sender() == _ui->doubleSpinBox_2)
    {
        col = CH2;
    }
    else if (sender() == _ui->doubleSpinBox_3)
    {
        col = CH3;
    }
    else if (sender() == _ui->doubleSpinBox_4)
    {
        col = CH4;
    }
    else if (sender() == _ui->doubleSpinBox_5)
    {
        col = CH5;
    }
    else if (sender() == _ui->doubleSpinBox_6)
    {
        col = CH6;
    }
    else if (sender() == _ui->doubleSpinBox_7)
    {
        col = CH7;
    }
    else if (sender() == _ui->doubleSpinBox_8)
    {
        col = CH8;
    }
    else if (sender() == _ui->doubleSpinBox_9)
    {
        col = CH9;
    }
    else if (sender() == _ui->doubleSpinBox_10)
    {
        col = CH10;
    }
    else if (sender() == _ui->doubleSpinBox_11)
    {
        col = CH11;
    }
    else if (sender() == _ui->doubleSpinBox_12)
    {
        col = CH12;
    }
    else if (sender() == _ui->doubleSpinBox_13)
    {
        col = CH13;
    }
    else if (sender() == _ui->doubleSpinBox_14)
    {
        col = CH14;
    }
    else if (sender() == _ui->doubleSpinBox_15)
    {
        col = CH15;
    }
    else if (sender() == _ui->doubleSpinBox_16)
    {
        col = CH16;
    }
    else if (sender() == _ui->doubleSpinBox_17)
    {
        col = CH17;
    }
    else if (sender() == _ui->doubleSpinBox_18)
    {
        col = CH18;
    }
    else if (sender() == _ui->doubleSpinBox_19)
    {
        col = CH19;
    }
    else if (sender() == _ui->doubleSpinBox_20)
    {
        col = CH20;
    }

    m_MixerSliderValue[m_currentOut][col] = pow(10.0, vlu/20.0) * 131072.0;;
//    if (m_pushButtonList.value(m_currentOut*20+col)->isChecked())
//    {
    emit mixerVolumeChanged(col, m_currentOut,
                            m_MixerSliderValue[m_currentOut][col] | (m_MixerSwitch[m_currentOut][col] == 1 ? 0x00200000 : 0));
//    } else {
//        emit mixerVolumeChanged(col, m_currentOut, 129);
//    }

//    qDebug()<<m_currentOut<<col<<m_MixerSliderValue[m_currentOut][col];
}

void Trn_MixerWidget::pushButtonClicked()
{
    int row = m_pushButtonList.indexOf(qobject_cast<QPushButton *>(sender())) / 20;
    int col = m_pushButtonList.indexOf(qobject_cast<QPushButton *>(sender())) % 20;

    m_MixerSwitch[row][col] = qobject_cast<QPushButton *>(sender())->isChecked();

    emit mixerVolumeChanged(col, row,
                            m_MixerSliderValue[row][col] | (m_MixerSwitch[row][col] == 1 ? 0x00200000 : 0));
//    if (qobject_cast<QPushButton *>(sender())->isChecked())
//    {
//        emit mixerVolumeChanged(col, row, m_MixerSliderValue[row][col]);
//    } else {
//        emit mixerVolumeChanged(col, row, 129);
//    }

//    qDebug()<<row<<col<<m_MixerSliderValue[row][col];
}

void Trn_MixerWidget::on_Reset_clicked()
{
    if (_ui->groupBox->isHidden())
    {
        return;
    }

    QMessageBox msgBox(QMessageBox::Question,
                       tr("Save"),
                       tr("This will set MO%1 the input gains of the current output channel to 0dB, are youe sure continue?").arg(m_currentOut+1),
                       NULL);
    msgBox.setWindowFlags(Qt::FramelessWindowHint);
    QPushButton *okButton = msgBox.addButton(tr("Yes"), QMessageBox::AcceptRole);
    QPushButton *cancelButton = msgBox.addButton(tr("No"), QMessageBox::RejectRole);
    msgBox.exec();
    if ((QPushButton*)msgBox.clickedButton() == okButton)
    {
        _ui->doubleSpinBox->setValue(0);
        _ui->doubleSpinBox_2->setValue(0);
        _ui->doubleSpinBox_3->setValue(0);
        _ui->doubleSpinBox_4->setValue(0);
        _ui->doubleSpinBox_5->setValue(0);
        _ui->doubleSpinBox_6->setValue(0);
        _ui->doubleSpinBox_7->setValue(0);
        _ui->doubleSpinBox_8->setValue(0);
        _ui->doubleSpinBox_9->setValue(0);
        _ui->doubleSpinBox_10->setValue(0);
        _ui->doubleSpinBox_11->setValue(0);
        _ui->doubleSpinBox_12->setValue(0);
        _ui->doubleSpinBox_13->setValue(0);
        _ui->doubleSpinBox_14->setValue(0);
        _ui->doubleSpinBox_15->setValue(0);
        _ui->doubleSpinBox_16->setValue(0);
        _ui->doubleSpinBox_17->setValue(0);
        _ui->doubleSpinBox_18->setValue(0);
        _ui->doubleSpinBox_19->setValue(0);
        _ui->doubleSpinBox_20->setValue(0);
    }
    if ((QPushButton*)msgBox.clickedButton() == cancelButton)
    {

    }
}
