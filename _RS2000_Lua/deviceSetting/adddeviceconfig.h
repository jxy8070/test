#ifndef ADDDEVICECONFIG_H
#define ADDDEVICECONFIG_H

#include <QDialog>
#include "ui_adddeviceconfig.h"

class AddDeviceConfig : public QDialog, private Ui::AddDeviceConfig
{
    Q_OBJECT

public:
    explicit AddDeviceConfig(QWidget *parent = 0);
    ~AddDeviceConfig();

protected:
//    void showEvent(QShowEvent *event);
//    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

public slots:
    void ok_clicked();
    void line_edit(QString);
signals:
    void addNewConfig();

//private:
//    void clear();

private:
};

#endif // ADDDEVICECONFIG_H
