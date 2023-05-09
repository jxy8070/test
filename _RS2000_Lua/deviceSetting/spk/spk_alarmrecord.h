#ifndef ALARMRECORD_H
#define ALARMRECORD_H

#include "customWidget/overwriteitem.h"


class QTableWidgetItem;
class QSqlTableModel;
class QDateTimeEdit;
class QLineEdit;
class AlarmRecord : public DevSettingDlg
{
    Q_OBJECT
    enum {ALARM_RECORD_MAX = 50}; //最大存储报警记录数
public:
    AlarmRecord(QString mac, QWidget *parent = 0);

signals:

public slots:
    void onSearch();
private:
    void setupUi(QString deviceMac);

private:
    QString _devMac;
    QSqlTableModel *_model;
    QLineEdit *_txtContext;
    QDateTimeEdit *_timeStart;
    QDateTimeEdit *_timeEnd;
};

#endif // ALARMRECORD_H
