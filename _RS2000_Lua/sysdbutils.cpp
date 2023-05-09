//#include "sysdbutils.h"
//#include "common.h"
///* 用户信息表 */
//QLatin1String SysDbUtils::_createUserTable =
//        QLatin1String("create table user(num integer primary key, type varchar, name varchar, password varchar)" );
//QLatin1String SysDbUtils::_insertUserInfo = QLatin1String("insert into user"\
//                                                          "(type, name, password) values(?, ?, ?)");
//QLatin1String SysDbUtils::_selectUserInfo = QLatin1String("select * from user");
//QLatin1String SysDbUtils::_updateUserInfo = QLatin1String("update user set password = :password where type = :type AND name = :name");
//QLatin1String SysDbUtils::_deleteUserInfo  = QLatin1String("delete from user where type = :type AND name = :name");

///* 设备表 */
//QLatin1String SysDbUtils::_createDeviceTable =
//        QLatin1String("create table device(mac varchar primary key, ip varchar, " \
//            "type integer, customName varchar, status integer, serial varchar, portInfo text)");
//QLatin1String SysDbUtils::_insertDevice = QLatin1String("insert into device"\
//            "(mac, ip, type, customName, status ) values(?, ?, ?, ?, ?)");
//QLatin1String SysDbUtils::_resetDevice         = QLatin1String("update device set ip = '0.0.0.0',  customName = '', status = 0");
//QLatin1String SysDbUtils::_deleteUnknowDevice  = QLatin1String("delete from device where type >= 65535");
//QLatin1String SysDbUtils::_deleteSpecialDevice = QLatin1String("delete from device where type == 64  or type == 128 or type == 255");
//QLatin1String SysDbUtils::_deleteOfflineDevice = QLatin1String("delete from device where ip = '0.0.0.0'");

///* 报警定义表 */
//QLatin1String SysDbUtils::_createAlarmConfigTable =
//        QLatin1String("create table alarmConfig(mac varchar primary key, " \
//            "enable integer, vch1checkmax integer, vch1maxvalue integer, vch1checkmin integer, vch1minvalue integer, " \
//            "cch1checkmax integer, cch1maxvalue integer, cch1checkmin integer, cch1minvalue integer, " \
//            "vch2checkmax integer, vch2maxvalue integer, vch2checkmin integer, vch2minvalue integer, " \
//            "cch2checkmax integer, cch2maxvalue integer, cch2checkmin integer, cch2minvalue integer, " \
//            "tempcheckmax integer, tempmaxvalue integer, tempcheckmin integer, tempminvalue integer, " \
//            "vlcheckmax integer, vlmaxvalue integer, vlcheckmin integer, vlminvalue integer, " \
//            "vrcheckmax integer, vrmaxvalue integer, vrcheckmin integer, vrminvalue integer)");
//QLatin1String SysDbUtils::_insertAlarmConfig =
//        QLatin1String("insert into alarmConfig"\
//            "(mac, enable, vch1checkmax, vch1maxvalue, vch1checkmin, vch1minvalue, "\
//                          "cch1checkmax, cch1maxvalue, cch1checkmin, cch1minvalue, "\
//                          "vch2checkmax, vch2maxvalue, vch2checkmin, vch2minvalue, "\
//                          "cch2checkmax, cch2maxvalue, cch2checkmin, cch2minvalue, "\
//                          "tempcheckmax, tempmaxvalue, tempcheckmin, tempminvalue, "\
//                          "vlcheckmax, vlmaxvalue, vlcheckmin, vlminvalue, "\
//                          "vrcheckmax, vrmaxvalue, vrcheckmin, vrminvalue)"\
//            "values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
//QLatin1String SysDbUtils::_selectAlarmConfig =
//        QLatin1String("select * from alarmConfig");
//QLatin1String SysDbUtils::_updateAlarmConfig =
//        QLatin1String("update alarmConfig set enable = :enable, "
//                      "vch1checkmax = :vch1checkmax, vch1maxvalue = :vch1maxvalue, vch1checkmin = :vch1checkmin, vch1minvalue = :vch1minvalue, " \
//                      "cch1checkmax = :cch1checkmax, cch1maxvalue = :cch1maxvalue, cch1checkmin = :cch1checkmin, cch1minvalue = :cch1minvalue, " \
//                      "vch2checkmax = :vch2checkmax, vch2maxvalue = :vch2maxvalue, vch2checkmin = :vch2checkmin, vch2minvalue = :vch2minvalue, " \
//                      "cch2checkmax = :cch2checkmax, cch2maxvalue = :cch2maxvalue, cch2checkmin = :cch2checkmin, cch2minvalue = :cch2minvalue, " \
//                      "tempcheckmax = :tempcheckmax, tempmaxvalue = :tempmaxvalue, tempcheckmin = :tempcheckmin, tempminvalue = :tempminvalue, " \
//                      "vlcheckmax = :vlcheckmax, vlmaxvalue = :vlmaxvalue, vlcheckmin = :vlcheckmin, vlminvalue = :vlminvalue, " \
//                      "vrcheckmax = :vrcheckmax, vrmaxvalue = :vrmaxvalue, vrcheckmin = :vrcheckmin, vrminvalue = :vrminvalue "
//                      "where mac = :mac");

///* 报警信息表 */
//QLatin1String SysDbUtils::_createAlarmInfoTable =  //插入时间，采用是默认值default (datetime('now', 'localtime'))
//        QLatin1String("create table alarmInfo ([id] integer PRIMARY KEY autoincrement, "\
//            "[createTime] datetime default (datetime('now', 'localtime')), [deviceMac] varchar(20), "\
//            "[channel] integer, [alarmInfo] varchar (250), [time] datetime) ");
//QLatin1String SysDbUtils::_insertAlarmInfo = QLatin1String("insert into alarmInfo"\
//        "(deviceMac, channel, alarmInfo, time) values(?, ?, ?, ?)");
//QLatin1String SysDbUtils::_deleteExpiryAlarmInfo = QLatin1String("delete from alarmInfo where createTime < ?");

//SysDbUtils::SysDbUtils() {}

//QSqlError SysDbUtils::initDb()
//{
//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("config/sysDb.db");
//    if (!db.open()) return db.lastError();

//    QSqlQuery qry;
//    QStringList tables = db.tables();
//    if(!tables.contains("user", Qt::CaseInsensitive))
//    {
//        if (!qry.exec(_createUserTable)) return qry.lastError();
//    }
//    else if (!tables.contains("device", Qt::CaseInsensitive))
//    {
//        if (!qry.exec(_createDeviceTable)) return qry.lastError();
////        if (!qry.prepare(_insertDevice)) return qry.lastError();
////        insertDevice(qry, "001DC1078271", "0.0.0.0", DEV_TYPE_DNTE_SPK, "", 0);
////        insertDevice(qry, "001DC1078288", "0.0.0.0", DEV_TYPE_DNTE_SPK, "", 0);
////        insertDevice(qry, "001DC107825F", "0.0.0.0", DEV_TYPE_DNTE_SPK, "", 0);
////        insertDevice(qry, "001DC1078260", "0.0.0.0", DEV_TYPE_DNTE_SPK, "", 0);
////        insertDevice(qry, "001DC10DC190", "0.0.0.0", DEV_TYPE_DNTE_TRN, "", 0);
////        insertDevice(qry, "001DC1078278", "0.0.0.0", DEV_TYPE_DNTE_SPK, "", 0);
////        insertDevice(qry, "001DC1078277", "0.0.0.0", DEV_TYPE_DNTE_SPK, "", 0);
////        insertDevice(qry, "001DC107825C", "0.0.0.0", DEV_TYPE_DNTE_SPK, "", 0);
////        insertDevice(qry, "001DC10DBF84", "0.0.0.0", DEV_TYPE_DNTE_TRN, "", 0);
////        insertDevice(qry, "001DC10782D3", "0.0.0.0", DEV_TYPE_DNTE_SPK, "", 0);
////        insertDevice(qry, "001DC10DC18A", "0.0.0.0", DEV_TYPE_DNTE_TRN, "", 0);
////        insertDevice(qry, "001DC10DBF3C", "0.0.0.0", DEV_TYPE_DNTE_TRN, "", 0);
////        insertDevice(qry, "001DC10DC00C", "0.0.0.0", DEV_TYPE_DNTE_TRN, "", 0);
//    }
//    else if(!tables.contains("alarmConfig", Qt::CaseInsensitive))
//    {
//        if (!qry.exec(_createAlarmConfigTable)) return qry.lastError();
//    }
//    else if(!tables.contains("alarmInfo", Qt::CaseInsensitive))
//    {
//        if (!qry.exec(_createAlarmInfoTable)) return qry.lastError();
//    }

//    return QSqlError();
//}

//QSqlError SysDbUtils::insertDevice(QSqlQuery &q, const QString &mac, const QString &ip,
//                       int devType, const QString &customName, int status)
//{
//    q.addBindValue(mac);
//    q.addBindValue(ip);
//    q.addBindValue(devType);
//    q.addBindValue(customName);
//    q.addBindValue(status);
//    if(q.exec()) return q.lastError();
//    else return QSqlError();
//}
