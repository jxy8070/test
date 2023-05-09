#include "sysdbutility.h"
#include <QtConcurrent/QtConcurrent>

#include <QDebug>
SysDBUtility::SysDBUtility(QObject *parent) : QObject(parent)
{
//    qDebug() << __FUNCTION__ << __LINE__ << thread();

//    insertAlarmInfo_thread *insertAlarmThread = new insertAlarmInfo_thread(_qry,_insert);
//    QThread *thread = new QThread(this);
//    insertAlarmThread->moveToThread(thread);
//    thread->start();
//    connect(this,SIGNAL(insertData_thread(QList<insert_alarmInfo>)),insertAlarmThread,SLOT(on_insertAlarmInfo(QList<SysDBUtility::insert_alarmInfo>)));
//    connect(insertAlarmThread,SIGNAL(timerStop()),this,SLOT(on_timerStop()));
}

void SysDBUtility::insertAlarmInfo(QString Mac, int channel, QString alarmInfo)
{
    AlarmInfoData alarmData;
    alarmData.Mac = Mac;
    alarmData.channel = channel;
    alarmData.alarmInfo = alarmInfo;
    alarmData.time = QDateTime::currentDateTime();//返回当前系统时间

    _mutex.lock();
    _alarmList.append(alarmData);
    _mutex.unlock();
}

void SysDBUtility::on_sendData()
{
//    qDebug()<<"on_sendData";
//    emit insertData_thread(_alarmList);
    while(1)
    {
        QList<AlarmInfoData> alarmList;
        QByteArray alarmInfo("insert into alarmInfo(deviceMac, channel, alarmInfo, time) select ?, ?, ?, ?");
        int Listnum = _alarmList.size();
        qDebug()<<"listnum:"<<Listnum;
        alarmList.clear();
        if(Listnum <= 0)
        {
            return ;
        }

        if(Listnum >= 300)
        {
            for(int i = Listnum-1; i > Listnum-300; i--)
            {
                alarmInfo.append(" union all select ?, ?, ?, ?");
                //        qDebug()<<"alarmInfo:"<<alarmInfo;
                alarmList.append(_alarmList.at(i));
            }
            alarmList.append(_alarmList.at(Listnum-300));
            qDebug()<<"alarmList:"<<alarmList.size();

            for(int j = Listnum-1; j >= Listnum-300; j--)
            {
//                qDebug()<<"remove"<<j;
                _alarmList.removeAt(j);
            }
        }else{
            for(int i = Listnum-1; i > 0; i--)
            {
                alarmInfo.append(" union all select ?, ?, ?, ?");
                //        qDebug()<<"alarmInfo:"<<alarmInfo;
                alarmList.append(_alarmList.at(i));
                //        qDebug()<<"alarmList:"<<alarmList.at(i);
            }
            alarmList.append(_alarmList.at(0));

            for(int j = Listnum-1; j >= 0; j--)
            {
//                qDebug()<<"remove"<<j;
                _alarmList.removeAt(j);
            }
        }

        _insertAlarmInfo = QLatin1String(alarmInfo);
        insert(alarmList);
    }
}

QSqlError SysDBUtility::initDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); //到数据库的连接（使用驱动程序和连接名称将数据库添加到数据库连接列表中）
    QString dbFilePath = QCoreApplication::applicationDirPath() + "/config/sysDb.db";
    if(QFile::exists(dbFilePath))
    {
        db.setDatabaseName(dbFilePath);//设置连接数据库的名称
        if (db.open())
        {
            _qry = new QSqlQuery(db);
            return QSqlError();
        }
    }
    return QSqlError("QSQLITE", dbFilePath, QSqlError::ConnectionError );//如果没有打开，返回错误信息
}

void SysDBUtility::insert(QList<AlarmInfoData> &alarmList)
{
    if(_qry->prepare(_insertAlarmInfo))
    {
        for (AlarmInfoData insert : alarmList)
        {
            _qry->addBindValue(insert.Mac);
            _qry->addBindValue(insert.channel);
            _qry->addBindValue(insert.alarmInfo);
            _qry->addBindValue(insert.time);
        }
        _qry->exec();
    }
    else
    {
        qDebug()<<"QSqlError";
    }
}



/* 用户信息表 */
QLatin1String SysDBUtility::_createUserTable =
        QLatin1String("create table user(num integer primary key, type varchar, name varchar, password varchar)" );
QLatin1String SysDBUtility::_insertUserInfo = QLatin1String("insert into user"\
                                                          "(type, name, password) values(?, ?, ?)");
QLatin1String SysDBUtility::_selectUserInfo = QLatin1String("select * from user");
QLatin1String SysDBUtility::_updateUserInfo = QLatin1String("update user set password = :password where type = :type AND name = :name");
QLatin1String SysDBUtility::_deleteUserInfo  = QLatin1String("delete from user where type = :type AND name = :name");

/* 日志表 */
QLatin1String SysDBUtility::_insertLog = QLatin1String("insert into logsTable"\
                                                       "(logTime, userName, logLevel, logType, logInfo) values(?, ?, ?, ?, ?)");
QLatin1String SysDBUtility::_selectLog = QLatin1String("select * from logsTable order by logTime desc");
QLatin1String SysDBUtility::_deleteLogHistory = QLatin1String("delete from logsTable where logTime < ?");



/* 设备表 */
QLatin1String SysDBUtility::_createDeviceTable =
        QLatin1String("create table device(mac varchar primary key, typeDesp varchar, type integer, " \
            "ip varchar, customName varchar, status integer, serial varchar, portInfo text)");
QLatin1String SysDBUtility::_resetDevice         = QLatin1String("update device set status = 0, serial = null, portInfo = null");
QLatin1String SysDBUtility::_deleteUnknowDevice  = QLatin1String("delete from device where type >= 65535");
QLatin1String SysDBUtility::_deleteSpecialDevice = QLatin1String("delete from device where type == 8  or type == 9 or type == 255");
QLatin1String SysDBUtility::_deleteOfflineDevice = QLatin1String("delete from device where status = 0");

/* 报警定义表 */
QLatin1String SysDBUtility::_createAlarmConfigTable =
        QLatin1String("create table alarmConfig(mac varchar primary key, " \
            "enable integer, vch1checkmax integer, vch1maxvalue integer, vch1checkmin integer, vch1minvalue integer, " \
            "cch1checkmax integer, cch1maxvalue integer, cch1checkmin integer, cch1minvalue integer, " \
            "vch2checkmax integer, vch2maxvalue integer, vch2checkmin integer, vch2minvalue integer, " \
            "cch2checkmax integer, cch2maxvalue integer, cch2checkmin integer, cch2minvalue integer, " \
            "tempcheckmax integer, tempmaxvalue integer, tempcheckmin integer, tempminvalue integer, " \
            "vlcheckmax integer, vlmaxvalue integer, vlcheckmin integer, vlminvalue integer, " \
            "vrcheckmax integer, vrmaxvalue integer, vrcheckmin integer, vrminvalue integer)");
QLatin1String SysDBUtility::_insertAlarmConfig =
        QLatin1String("insert into alarmConfig"\
            "(mac, enable, vch1checkmax, vch1maxvalue, vch1checkmin, vch1minvalue, "\
                          "cch1checkmax, cch1maxvalue, cch1checkmin, cch1minvalue, "\
                          "vch2checkmax, vch2maxvalue, vch2checkmin, vch2minvalue, "\
                          "cch2checkmax, cch2maxvalue, cch2checkmin, cch2minvalue, "\
                          "tempcheckmax, tempmaxvalue, tempcheckmin, tempminvalue, "\
                          "vlcheckmax, vlmaxvalue, vlcheckmin, vlminvalue, "\
                          "vrcheckmax, vrmaxvalue, vrcheckmin, vrminvalue)"\
            "values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
QLatin1String SysDBUtility::_selectAlarmConfig =
        QLatin1String("select * from alarmConfig");
QLatin1String SysDBUtility::_updateAlarmConfig =
        QLatin1String("update alarmConfig set enable = :enable, "
                      "vch1checkmax = :vch1checkmax, vch1maxvalue = :vch1maxvalue, vch1checkmin = :vch1checkmin, vch1minvalue = :vch1minvalue, " \
                      "cch1checkmax = :cch1checkmax, cch1maxvalue = :cch1maxvalue, cch1checkmin = :cch1checkmin, cch1minvalue = :cch1minvalue, " \
                      "vch2checkmax = :vch2checkmax, vch2maxvalue = :vch2maxvalue, vch2checkmin = :vch2checkmin, vch2minvalue = :vch2minvalue, " \
                      "cch2checkmax = :cch2checkmax, cch2maxvalue = :cch2maxvalue, cch2checkmin = :cch2checkmin, cch2minvalue = :cch2minvalue, " \
                      "tempcheckmax = :tempcheckmax, tempmaxvalue = :tempmaxvalue, tempcheckmin = :tempcheckmin, tempminvalue = :tempminvalue, " \
                      "vlcheckmax = :vlcheckmax, vlmaxvalue = :vlmaxvalue, vlcheckmin = :vlcheckmin, vlminvalue = :vlminvalue, " \
                      "vrcheckmax = :vrcheckmax, vrmaxvalue = :vrmaxvalue, vrcheckmin = :vrcheckmin, vrminvalue = :vrminvalue "
                      "where mac = :mac");

/* 报警信息表 */
QLatin1String SysDBUtility::_createAlarmInfoTable =  //插入时间，采用是默认值default (datetime('now', 'localtime'))
        QLatin1String("create table alarmInfo ([id] integer PRIMARY KEY autoincrement, "\
            "[createTime] datetime default (datetime('now', 'localtime')), [deviceMac] varchar(20), "\
            "[channel] integer, [alarmInfo] varchar (250), [time] datetime) ");
QLatin1String SysDBUtility::_deleteExpiryAlarmInfo = QLatin1String("delete from alarmInfo where createTime < ?");
