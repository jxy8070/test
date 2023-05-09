#include "stable.h"
#include <QTranslator>
#include "Frame/mainwindow.h"
#include "UserManager/logindialog.h"
#include "device/devicemanager.h"
#include "sysdbutility.h"
#include <stdexcept>
#include <QFile>
#include "UserControlInterface/usercontrolinterface.h"
#include "Windows.h"
#include <DbgHelp.h>

//void myMessageHandle(QtMsgType , const QMessageLogContext& , const QString& msg);

void myMessageOutput(QtMsgType type, const char *msg)
{
    const static QString log_path =  QDir::homePath() + "/log";
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("[%1] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(QString("Debug:")).arg(msg);
        fprintf(stderr, "%s\n", msg);
        break;

    case QtWarningMsg:
        text = QString("[%1] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(QString("Warning:")).arg(msg);
        fprintf(stderr, "%s\n", msg);
        break;

    case QtCriticalMsg:
        text = QString("[%1] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(QString("Critical:")).arg(msg);
        fprintf(stderr, "%s\n", msg);
        break;

    case QtFatalMsg:
        text = QString("[%1] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(QString("Fatal:")).arg(msg);
        fprintf(stderr, "%s\n", msg);
        abort();
        break;

    default:
        break;
    }
    if (!text.isEmpty())
    {
        QString message = text;
        QString log_file = "log_" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".txt";

        QFile file(log_path+ "/" +log_file);
        if (file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QTextStream ts(&file);
            ts << message << endl;
            file.flush();
            file.close();
        }
    }
    mutex.unlock();
}

#ifdef QT_DEBUG
////方便测试，需要测试的功能可写在一下区域
#include "deviceItem/deviceitemfactory.h"
#include "deviceItem/deviceItem.h"
#include "Frame/projectscene.h"
#include "Frame/projectview.h"
#include "LuaIde/luawindow.h"
#include "Lua/Packages/luaengine_dev.h"
#include "customWidget/basewindow.h"
int test(QApplication& app)
{
     qDebug()<< QCoreApplication::applicationDirPath();
//    double time_Start = (double)clock();
//    // 你所要测试的代码块

//    double time_End = (double)clock();
//    qDebug()<<"time= "<<(time_End - time_Start)/1000.0<<"s";
//    cmd.luaL_getValue("Sig_36", 0x00070001, val);

//    LuaWindow* luaWnd = LuaWindow::instance();
//    luaWnd->loadLuaScript("E:\\test.lua");
//    luaWnd->show(nullptr);
     return 0;
}
#endif

#include "Protocal/packetbase.h"
bool checkSingleProcess()
{
    QUdpSocket* udpSock = new QUdpSocket;
    QNetworkInterface curInf = Config::instance()->getCurItr();
    if(!udpSock->joinMulticastGroup(QHostAddress(PacketBase::GROUPCAST_IP), curInf))//加入指定的多播组
    {
        Utils::MessageBoxWarning(QObject::tr("Warning"), QObject::tr("Multicast is used, Only one instance can be started "));
        return false;
    }
    else{
        udpSock->leaveMulticastGroup(QHostAddress(PacketBase::GROUPCAST_IP), curInf);
    }

    return true;
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    //创建 Dump 文件
    QString str =QDateTime::currentDateTime().toString("yyyyMMdd")+ QTime::currentTime().toString("HHmmsszzz") + ".dmp";
    HANDLE hDumpFile = CreateFile(str.toStdWString().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if( hDumpFile != INVALID_HANDLE_VALUE){
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    }
    //这里弹出一个错误对话框并退出程序
    EXCEPTION_RECORD* record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode,16));
    QString errAdr(QString::number((uint)record->ExceptionAddress,16));
    QMessageBox::critical(NULL,QString("program crash"), QString("We sincerely apologize for the error that occurred\n"),
                          QMessageBox::Ok);
    return EXCEPTION_EXECUTE_HANDLER;
}

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
//    qInstallMessageHandler(myMessageHandle);
    try
    {
        QApplication app(argc, argv);
        SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler); //注册异常捕获函数
//#ifndef QT_DEBUG
//        qInstallMessageHandler(customMessageHandler);
//#endif
//#ifdef QT_DEBUG
//            test(app);
//            return 0;
//#endif

        /* 打开数据库 */
        SysDBUtility dbUtils;
        QSqlError err = dbUtils.initDB();
        QSqlError::ErrorType type = err.type();
        if(err.type() != QSqlError::NoError)
        {
            Utils::MessageBoxWarning("Error", "open sqlite failed!");
            return -1;
        }
        Config::instance()->init();

        if(!checkSingleProcess()) return -1;

        //根据客户版本设置app样式、字体等
        SkinStyle::loadAppTranslator(app);
        SkinStyle::loadAppFont();
        app.setStyleSheet(SkinStyle::getAppStyleSheet());

        //遍历翻译文件，删除失效翻译
        QString file = QApplication::applicationDirPath() + "/../language/SimpliedChinese.ts";
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
            file = QApplication::applicationDirPath() + "/../language/FangTu/FangTu_SimpliedChinese.ts";
        Config::instance()->remvoeTsFileVanishedAndObsolete(file, file);

        LoginDialog w(false, 0);
        int result = w.exec();
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu && result == 2) {
            DeviceManager::instance();
            UserControlInterface::instance()->show();
        }
        else if(result == QDialog::Accepted || result == 2) {
            DeviceManager::instance();
            MainWindow::instance()->show();
            MainWindow::instance()->openWelcomeDlg();
        }
        else if(result == QDialog::Rejected) {
            return result;
        }
        int re = app.exec();
        DeviceManager::release();
        return re;
    }
    catch(std::exception ex)
    {
        QMessageBox::information(NULL, "Error", ex.what());
        return -1;
    }
}

//void myMessageHandle(QtMsgType , const QMessageLogContext& , const QString& msg)
//{
//    static QMutex mut; //多线程打印加锁
//    QMutexLocker locker(&mut);
//    QFile file("Debug.txt");
//    QString dbgData = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ") + msg;
//    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
//    {
//        QTextStream stream(&file);
//        stream << dbgData << '\n' << Qt::flush;
//        file.close();
//    }
//}
