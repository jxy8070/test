#ifndef LUAWINDOW_H
#define LUAWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTextDocument>

QT_BEGIN_NAMESPACE
namespace Ui { class LuaWindow; }
QT_END_NAMESPACE

#include "qtluaengine.h"

class CtrlItemLua;
class LuaWindow : public QMainWindow
{
    Q_OBJECT
    enum {LogType_Error = -1, LogType_Info = 0 };
public:
    static LuaWindow* instance();
    ~LuaWindow();
    void addLog(QString msg, int logType = LogType_Info);
    void setLuaScript(QString luaScript);
    void loadLuaScript(QString fileName);    
    void show(CtrlItemLua* luaControl);
    void initLuaEngineAndControls();

private:
    LuaWindow();
    void init();
    bool find(QString text, QTextDocument::FindFlags flags);
    void flashGTable();

private slots:
    void onErrorMessage(QByteArray message);
    void onStartLua();
    void onStopLua();
    void onRestartLua();
    void onClearLogs();
    void onShowGTable();
    void on_brnNext1_clicked(); //Tab查找Next
    void on_btnNext2_clicked(); //Tab替换Next
    void on_btnReplace_clicked();
    void on_btnReplaceAll_clicked();

private:    
    static LuaWindow* _sInstance;
    Ui::LuaWindow *ui;
    QtLuaEngine *_LuaEngine;
    CtrlItemLua* _luaControl;
    QStandardItemModel *logModel;
};
#endif // LUAWINDOW_H
