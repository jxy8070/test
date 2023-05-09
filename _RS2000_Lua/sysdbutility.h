#ifndef SYSDBUTILITY_H
#define SYSDBUTILITY_H

#include <QObject>
#include <QtSql>
#include <QThread>
#include <QMutex>


class SysDBUtility : public QObject
{
    Q_OBJECT
public:  
    struct AlarmInfoData
    {
        QString Mac;
        int channel;
        QString alarmInfo;
        QDateTime time;
    };

    QSqlError initDB(void);
    explicit SysDBUtility(QObject *parent = nullptr);

signals:
    void insertData_thread(QList<AlarmInfoData>);

public slots:
    void insertAlarmInfo(QString Mac, int channel, QString alarmInfo);
    
    void on_sendData();

private:
    void insert(QList<AlarmInfoData>&);
private slots:

private:
    QSqlQuery *_qry;
    QLatin1String _insertAlarmInfo;
    QMutex _mutex;
    QWaitCondition _newAlarmCond;
    QList<AlarmInfoData> _alarmList;

public:
    static QLatin1String _createUserTable;
    static QLatin1String _insertUserInfo;
    static QLatin1String _selectUserInfo;
    static QLatin1String _updateUserInfo;
    static QLatin1String _deleteUserInfo;

    static QLatin1String _insertLog;
    static QLatin1String _selectLog;
    static QLatin1String _deleteLogHistory;

    static QLatin1String _createDeviceTable;
    static QLatin1String _resetDevice;
    static QLatin1String _deleteUnknowDevice;
    static QLatin1String _deleteSpecialDevice;
    static QLatin1String _deleteOfflineDevice;

    static QLatin1String _createAlarmConfigTable;
    static QLatin1String _insertAlarmConfig;
    static QLatin1String _selectAlarmConfig;
    static QLatin1String _updateAlarmConfig;

    static QLatin1String _createAlarmInfoTable;
    static QLatin1String _deleteExpiryAlarmInfo;
};

#endif // SYSDBUTILITY_H
