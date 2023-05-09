#include "luawindow.h"
#include "ui_luawindow.h"
#include <QSplitter>
#include "qluatextedit.h"
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QSettings>
#include "CtrlItem/ctrlitemlua.h"
#include "luautils.h"
#include "Frame/mainwindow.h"

LuaWindow* LuaWindow::_sInstance = NULL;
LuaWindow* LuaWindow::instance()
{
    static QMutex mutex;
    if (NULL == _sInstance)
    {
        QMutexLocker locker(&mutex);
        if (NULL == _sInstance)
        {
            _sInstance = new LuaWindow();
        }
    }
    return _sInstance;
}

LuaWindow::LuaWindow()
    : QMainWindow() , ui(new Ui::LuaWindow)
{
    ui->setupUi(this);
    _LuaEngine = NULL;
    logModel = new QStandardItemModel(this);
    init();
#ifdef QT_DEBUG
    ui->dockGTable->setVisible(true);
#else
    ui->dockGTable->setVisible(false);
#endif
}

LuaWindow::~LuaWindow()
{
    delete ui;
//    QSettings s;
//    s.setValue("editor/find/caseSensitive", ui->caseSensitive1->isChecked());
//    s.setValue("editor/find/wholeWords", ui->wholeWords1->isChecked());
}

//初始化LuaControl相关的Controls数据
void LuaWindow::initLuaEngineAndControls()
{
    //初始化lua引擎
    _LuaEngine = new QtLuaEngine;
    _LuaEngine->nameObject(_LuaEngine, "qEngine");
    _LuaEngine->pushFunction("print", luaQ_printLog);
    _LuaEngine->pushFunction("test1", test1);
    _LuaEngine->setPrintResults(false);

//   connect(_LuaEngine, SIGNAL(stateChanged(int)), d, SLOT(stateChanged(int)), Qt::QueuedConnection);
    // Causes the emission of the first stateChanged() message.
    QtLuaLocker lock(_LuaEngine);
    lock.setRunning();
    connect(_LuaEngine, SIGNAL(errorMessage(QByteArray)), this, SLOT(onErrorMessage(QByteArray)));

    lua_State* L = _LuaEngine->getLuaState();
    lua_pushlightuserdata(L, (void*)this);
    lua_setfield(L, LUA_REGISTRYINDEX, "pLuaWindow");
    lua_settop(L, 0);

    if(_luaControl != NULL)
    {
        lua_getglobal(L, "Controls"); //重新放置Controls表到栈顶
        lua_newtable(L);
        for(int i = 0; i<_luaControl->gePinOutCount() ; i++) //初始化Outputs表元素
        {
            lua_pushlightuserdata(L, (void*)_luaControl->getPinOut(i));
            luaL_setmetatable(L, "LuaPin");
            lua_rawseti(L, -2, i+1);
        }
        lua_setfield(L, -2, "Outputs"); //添加Outputs表

        lua_newtable(L);
        for(int i = 0; i< _luaControl->gePinInCount(); i++) //初始化Inputs表元素
        {
            lua_pushlightuserdata(L, (void*)_luaControl->getPinIn(i));
            luaL_setmetatable(L, "LuaPin");
            lua_rawseti(L, -2, i+1);
        }
        lua_setfield(L, -2, "Inputs"); //添加Inputs表
        lua_pop(L, 1);
    }
}

void LuaWindow::init()
{
    ui->splitterMain->setChildrenCollapsible(false); //不可以将小部件调整为0
    ui->splitterTop->setChildrenCollapsible(false);

    this->setWindowTitle(tr("Lua Script"));

    ui->luaEditer->setEditorMode("lua");    
    QFont font = SkinStyle::getFont(12);
    ui->luaEditer->setFont(font);
    ui->listView->setModel(logModel);
    ui->listView->setFont(font);

    //工具栏
    connect(ui->actionClearLogs, SIGNAL(triggered()), this, SLOT(onClearLogs()));
    connect(ui->actionStartLua, SIGNAL(triggered()), this, SLOT(onStartLua()));
    connect(ui->actionShowGTable, SIGNAL(triggered()), this, SLOT(onShowGTable()));
    //查询替换 settings
    connect(ui->findEdit1,SIGNAL(returnPressed()), this, SLOT(on_brnNext1_clicked()));
    connect(ui->findEdit2,SIGNAL(returnPressed()), this, SLOT(on_btnNext2_clicked()));

//    QSettings s;
//    bool c = s.value("editor/find/caseSensitive",false).toBool();
//    bool w = s.value("editor/find/wholeWords",true).toBool();
//    ui->caseSensitive1->setChecked(c);
//    ui->wholeWords1->setChecked(w);
//    //快捷键
//    new QShortcut(QKeySequence::FindNext, this, SLOT(findNext()));
//    new QShortcut(QKeySequence::FindPrevious, this, SLOT(findPrevious()));
//    findNextSCut = new QShortcut(QKeySequence::FindNext, editor);
//    findPrevSCut = new QShortcut(QKeySequence::FindPrevious, editor);
//    connect(findNextSCut, SIGNAL(activated()), this, SLOT(findNext()));
//    connect(findPrevSCut, SIGNAL(activated()), this, SLOT(findPrevious()));
}

void LuaWindow::show(CtrlItemLua* luaControl)
{
    _luaControl = luaControl;
    if(_luaControl != NULL){
         QString luaScript =_luaControl->luaScript();
         ui->luaEditer->setPlainText(luaScript);
    }

    //设置窗口置顶
    if (isMinimized()){
        showNormal();
    }
    ::SetWindowPos(HWND(winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    ::SetWindowPos(HWND(winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    QMainWindow::show();
    activateWindow();
}

void LuaWindow::addLog(QString msg, int logType)
{
    QStandardItem *item = new QStandardItem(QString("%1    %2")
                                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(msg));
    if(logType == -1)
    {
        item->setForeground(QBrush(Qt::red));
    }
    logModel->appendRow(item);

    //滚动显示最新位置
    ui->listView->scrollToBottom();
}

void LuaWindow::setLuaScript(QString luaScript)
{
    ui->luaEditer->setPlainText(luaScript);
}

void LuaWindow::loadLuaScript(QString fileName)
{
    ui->luaEditer->readFile(fileName);
}

//执行脚本
void LuaWindow::onStartLua()
{
#ifndef QT_DEBUG
    if(_luaControl != NULL)
    {
        ConfigScene* pScene = qobject_cast<ConfigScene*>(_luaControl->scene());
        if(!pScene->isDebuging()){
            addLog(tr("change debug mode to start lua script"), LogType_Error);
            return;
        }
    }
#endif

    if(_LuaEngine!= NULL) {
        _LuaEngine->stop();
        delete _LuaEngine;
        _LuaEngine = NULL;
    }

    //初始化    
    initLuaEngineAndControls();

    //执行脚本
    QString startMsg = tr("start lua script");
    addLog(startMsg, LogType_Info);

    QString luaScript = ui->luaEditer->toPlainText();
    if(_luaControl != NULL){
        _luaControl->setLuaScript(luaScript);
    }
//    QVariantList varList = _LuaEngine->evaluate(luaScript);
    bool rs = _LuaEngine->eval(luaScript, false);

    flashGTable();
}

//停止脚本
void LuaWindow::onStopLua()
{
    if(_LuaEngine!= NULL) {
        _LuaEngine->stop();
        delete _LuaEngine;
        _LuaEngine = NULL;
    }
    QString startMsg = tr("stop lua script");
    addLog(startMsg, LogType_Info);
}

//重新执行脚本
void LuaWindow::onRestartLua()
{
    onStartLua();
}

void LuaWindow::onClearLogs()
{
    logModel->clear();
}


void LuaWindow::flashGTable()
{
    bool isShow = ui->dockGTable->isVisible();
    if(isShow) //刷新_G
    {
        ui->treeTable->clear();        
        lua_State* L = _LuaEngine->getLuaState();
        QTreeWidgetItem * rootItem = new QTreeWidgetItem(QStringList() <<"root item");
        lua_Format_G(L, rootItem);
        ui->treeTable->addTopLevelItems(rootItem->takeChildren());
        ui->treeTable->sortByColumn(0, Qt::AscendingOrder);
    }
}

void LuaWindow::onShowGTable()
{
#ifdef QT_DEBUG
    if(_LuaEngine == NULL) return;
    bool isShow = !ui->dockGTable->isVisible();
    ui->dockGTable->setVisible(isShow);

    flashGTable();
#endif
}


void LuaWindow::onErrorMessage(QByteArray msgArray)
{
    QString message = QString::fromLocal8Bit(msgArray); //QString message(msgArray); //
    int pos = message.indexOf("\nstack traceback:\n\t");
    if(pos != -1){
        message = message.left(pos);
    }
    addLog(message, LogType_Error);
}


// ========================================
// 查找替换
bool LuaWindow::find(QString text, QTextDocument::FindFlags flags)
{
    if (text.isEmpty())
      return false;
    QLuaTextEdit* editor = ui->luaEditer;
    QTextCursor cursor = editor->textCursor();
    cursor = editor->document()->find(text, cursor, flags);
    if (cursor.isNull())
      cursor = editor->document()->find(text, cursor, flags);
    if (cursor.isNull())
      return false;
    editor->setTextCursor(cursor);
    editor->ensureCursorVisible();
    return true;
}

void LuaWindow::on_brnNext1_clicked()
{
    QTextDocument::FindFlags flags = 0;
    if (ui->searchBackwards1->isChecked())
      flags |= QTextDocument::FindBackward;
    if (ui->caseSensitive1->isChecked())
      flags |= QTextDocument::FindCaseSensitively;
    if (ui->wholeWords1->isChecked())
      flags |= QTextDocument::FindWholeWords;

    QString text = ui->findEdit1->text();
    if(!find(text, flags))
    {
        MainWindow::instance()->showNotice(tr("Search text not found."));
    }
}

void LuaWindow::on_btnNext2_clicked()
{
    QTextDocument::FindFlags flags = 0;
    if (ui->searchBackwards2->isChecked())
      flags |= QTextDocument::FindBackward;
    if (ui->caseSensitive2->isChecked())
      flags |= QTextDocument::FindCaseSensitively;
    if (ui->wholeWords2->isChecked())
      flags |= QTextDocument::FindWholeWords;

    QString text = ui->findEdit2->text();
    if(!find(text, flags))
    {
        MainWindow::instance()->showNotice(tr("Search text not found."));
    }
}

void LuaWindow::on_btnReplace_clicked()
{
    QString searchTxt = ui->findEdit2->text();
    QLuaTextEdit* editor = ui->luaEditer;
    if(searchTxt != editor->textCursor().selectedText())
    {
       on_brnNext1_clicked();
    }
    if(searchTxt == editor->textCursor().selectedText())
    {
        QTextCursor cursor = editor->textCursor();
        cursor.insertText(ui->replaceEdit2->text());
        on_btnNext2_clicked();
    }
}

void LuaWindow::on_btnReplaceAll_clicked()
{
    QTextDocument::FindFlags flags = 0;
    if (ui->searchBackwards2->isChecked())
      flags |= QTextDocument::FindBackward;
    if (ui->caseSensitive2->isChecked())
      flags |= QTextDocument::FindCaseSensitively;
    if (ui->wholeWords2->isChecked())
      flags |= QTextDocument::FindWholeWords;

    QLuaTextEdit* editor = ui->luaEditer;
    QTextDocument *document = editor->document();
    QString fText = ui->findEdit2->text();
    QString rText = ui->replaceEdit2->text();
    QTextCursor cursor(document);

    int n = 0;
    while(! (cursor = document->find(fText, cursor, flags)).isNull()) {
        cursor.insertText(rText);
        n += 1;
    }
    MainWindow::instance()->showNotice(tr("Replaced %n occurrence(s).", 0, n)); //wj2
}



