#ifndef LOGSMANAGER_H
#define LOGSMANAGER_H

#include <QListView>

class LogsManager
{
public:
    void addLogs(QString logLevel,QString logType,QString logInfo);
    QTableView* getLogsView() {return &_logsTable;}

protected:
    void updateLogsView();
private:
    QTableView _logsTable;
//////////////////////////////////////////////////
/// 实现Singleton模式相关代码
public:
    static LogsManager *instance();
private:
    LogsManager();
    LogsManager(const LogsManager&);            //禁止拷贝构造函数。
    LogsManager & operator=(const LogsManager&);//禁止赋值拷贝函数。
    static QMutex Mutex;//实例互斥锁。
    static LogsManager* _Instance; //<使用原子指针,默认初始化为0。
};

#endif // LOGSMANAGER_H
