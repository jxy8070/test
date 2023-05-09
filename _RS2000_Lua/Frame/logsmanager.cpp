#include "logsmanager.h"
#include "sysdbutility.h"
#include "config.h"
#include <QStringListModel>

LogsManager::LogsManager()
{
    _logsTable.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _logsTable.setEditTriggers(QAbstractItemView::NoEditTriggers);
    _logsTable.setAttribute(Qt::WA_ShowModal, true);
    _logsTable.setSortingEnabled(false);
    _logsTable.setSelectionMode(QTableWidget::SingleSelection);
    _logsTable.setSelectionBehavior(QTableWidget::SelectRows);
    _logsTable.horizontalHeader()->hide();
}


void LogsManager::addLogs(QString logLevel,QString logType,QString logInfo)
{
    QSqlQuery qry;
    qry.prepare(SysDBUtility::_insertLog);
    qry.addBindValue(QDateTime::currentDateTime());
    qry.addBindValue(Config::instance()->getCurUser().name);
    qry.addBindValue(logLevel);
    qry.addBindValue(logType);
    qry.addBindValue(logInfo);
    if(!qry.exec())
    {
        qDebug() << "insert user failed!" << qry.lastError();
    }
    updateLogsView();
}

//刷新日志信息
void LogsManager::updateLogsView()
{
    QStringList logList;
    QSqlQuery qry;
    qry.prepare(SysDBUtility::_selectLog);
    if(qry.exec())
    {
        while(qry.next())
        {
            QDateTime logTime = qry.value(0).toDateTime();
            QString userName     = qry.value(1).toString();
            QString logLevel     = qry.value(2).toString();
            QString logType     = qry.value(3).toString();
            QString logInfo     = qry.value(4).toString();
            QString logItem = QString("%1    (%2)  %3").arg(logTime.toString("yyyy-MM-dd hh:mm:ss.zzz"))
                    .arg(logLevel).arg(logInfo);
            logList.append(logItem);
        }
    }
    else{
        qDebug()<<qry.lastError();
    }

    QStringListModel* dataModel = new QStringListModel(logList);
    _logsTable.setModel(dataModel);
}

////////////////////////////////////////////////////////
/// \brief Singleton模式相关代码
QMutex LogsManager::Mutex;//实例互斥锁。
LogsManager* LogsManager::_Instance = NULL; //<使用原子指针,默认初始化为0。

LogsManager* LogsManager::instance()
{
    //使用双重检测。
    if(!_Instance)//第一次检测
    {
        QMutexLocker locker(&Mutex);//加互斥锁。

        if(!_Instance)//第二次检测。
            _Instance = new LogsManager();
    }

    return _Instance;
}
