#include "stable.h"
#include "config.h"
#include <QDomDocument>
#include "sysdbutility.h"
#include <QStringList>
#include "Frame/logsmanager.h"
#include "Frame/mainwindow.h"

#define ALARMINFO_SHELFLIFE 7
#define LOG_SHELFLIFE 30
bool Config::ShowProgressDialog = true;
bool Config::ShowSaveDialog = true;

Config::Config(QObject *parent) :
    QObject(parent)
{   
    _initSetting = new QSettings("config/system.ini", QSettings::IniFormat);
    _initSetting->setIniCodec(QTextCodec::codecForName("UTF-8"));
    QVariant var = getInitValue("SOFTWARE", "language");
    if(!var.isValid()){
        _curLang = LANGUAGE_SIMPLIED_CHINESE;
        setLanguage(_curLang);
    }
    else {
        int langInt = var.toInt();
        _curLang = LanguageEnum(langInt);
    }
    _portGroupNum = 4;
    QVariant varNum = getInitValue("SOFTWARE", "portGroupNum");
    if(varNum.isValid()) {
        switch (varNum.toInt()) {
        case 1: _portGroupNum  = 1; break;
        case 2: _portGroupNum  = 2; break;
        case 4: _portGroupNum  = 4; break;
        case 8: _portGroupNum  = 8; break;
        }
    }
    else{
      setInitValue("SOFTWARE", "portGroupNum", 4); //设置默认值
    }
    QVariant varDefIp = getInitValue("SOFTWARE", "defaultIp");
    if(varDefIp.isValid()) {
        _curIp = varDefIp.toString();
    }else {
        _curIp = "0.0.0.0";
        setInitValue("SOFTWARE", "defaultIp", _curIp);
    }
//    QVariant varDefDir = getInitValue("SOFTWARE", "defaultDir");
//    if(varDefDir.isValid()) {
//        _defWorkingDir = varDefDir.toString();
//    }else {
//        _defWorkingDir = QApplication::applicationDirPath();
//        setInitValue("SOFTWARE", "defaultDir", _defWorkingDir);
//    }

    _dbUtility = new SysDBUtility();
    _curSkin = NULL;
}

void Config::init()
{
    QSqlQuery qry;
    /* 报警记录存储有效期为7天 */
    qry.prepare(SysDBUtility::_deleteExpiryAlarmInfo);
    qry.addBindValue(QDateTime::currentDateTime().addDays(-ALARMINFO_SHELFLIFE).toString("yyyy-MM-dd hh:mm:ss.zzz"));
    if(!qry.exec())
    {
        qDebug() << "delete alarm info failed!" << qry.lastError();
    }

    /* 加载用户信息 */
    qry.prepare(SysDBUtility::_selectUserInfo);
    if(qry.exec())
    {
        while(qry.next())
        {
            UserInfo newUser;
            newUser.type     = qry.value(1).toString();
            newUser.name     = qry.value(2).toString();
            newUser.password = qry.value(3).toString();
            _userList.append(newUser);
        }
    }
    else{
        qDebug()<<qry.lastError();
    }

    /* 删除历史日志 */
    qry.prepare(SysDBUtility::_deleteLogHistory);
    qry.addBindValue(QDateTime::currentDateTime().addDays(-LOG_SHELFLIFE));
    if(!qry.exec())
    {
        qDebug()<<qry.lastError();
    }
}

void Config::initManufacturerCfg()
{
#ifdef Logo_SuZhouFitcan
    _curMfrsCfg = MfrsCfg::_Fitcan;
#elif defined Logo_FangTu
    _curMfrsCfg = MfrsCfg::_FangTu;
#elif defined Logo_DongFangJiaLian
    _curMfrsCfg = MfrsCfg::_DongFangJiaLian;
#elif defined Logo_GuangZhouShengTuo
    _curMfrsCfg = MfrsCfg::_GuangZhouShengTuo;
#elif defined Logo_HuShan
    _curMfrsCfg = MfrsCfg::_Hushan;
#elif defined Logo_BeiJingHuiSheng
    _curMfrsCfg = MfrsCfg::_BeiJingHuiSheng;
#elif defined Logo_GuRuiDianZi
    _curMfrsCfg = MfrsCfg::_GuRui;
#elif defined YiChuang
    _curMfrsCfg = MfrsCfg::_YiChuang;
#elif defined Logo_BaiLianDa
    _curMfrsCfg = MfrsCfg::_BaiLianDa;
#elif defined Logo_ControlMax
    _curMfrsCfg = MfrsCfg::_ControlMax;
#elif defined Logo_FTYX
    _curMfrsCfg = MfrsCfg::_FTYX;
#endif

    Q_ASSERT(_curMfrsCfg.SkinTypeList.count() > 0);
    SkinType curSkin = _curMfrsCfg.SkinTypeList[0];
    if(_curMfrsCfg.SkinTypeList.count() > 1)
    {
        QVariant var = getInitValue("SOFTWARE", "skinStyle");
        if(!var.isValid()){
            curSkin = _curMfrsCfg.SkinTypeList[0];
            setInitValue("SOFTWARE", "skinStyle", QVariant(curSkin));
        }
        else {
            curSkin = SkinType(var.toInt());
        }
    }
    //创建当前Skin
    _curSkin = new SkinStyle(curSkin);
    _curMfrsCfg.InitDeviceTree();
}

void Config::setLanguage(LanguageEnum language)
{
    setInitValue("SOFTWARE", "language", QVariant(language));
}


void Config::changUserInfo(int i, UserInfo user)
{
    QSqlQuery qry;

    qry.prepare(SysDBUtility::_updateUserInfo);
    qry.bindValue(":password", user.password);
    qry.bindValue(":type", user.type);
    qry.bindValue(":name", user.name);
    if(!qry.exec())
    {
        qDebug() << "update user failed!" << qry.lastError();
    }
    QString logInfo = QString(tr("%1 changed password of user:%2")).arg(_curUser.name).arg(user.name);
    LogsManager::instance()->addLogs(tr("Info"), tr("UserManager"), logInfo);
    _userList.replace(i, user);
}

void Config::addNewUser()
{
    QSqlQuery qry;

    qry.prepare(SysDBUtility::_insertUserInfo);
//    qry.addBindValue(_userList.count() - 1);
    qry.addBindValue(_userList.last().type);
    qry.addBindValue(_userList.last().name);
    qry.addBindValue(_userList.last().password);
    if(!qry.exec())
    {
        qDebug() << "insert user failed!" << qry.lastError();
    }

    QString logInfo = QString(tr("%1 added a new user:%2")).arg(_curUser.name).arg(_userList.last().name);
    LogsManager::instance()->addLogs(tr("Info"), tr("UserManager"), logInfo);
}

void Config::deleteUser(int i)
{
    QSqlQuery qry;

    qry.prepare(SysDBUtility::_deleteUserInfo);
    qry.addBindValue(_userList.at(i).type);
    qry.addBindValue(_userList.at(i).name);
    if(!qry.exec())
    {
        qDebug() << "insert user failed!" << qry.lastError();
    }
    QString logInfo = QString(tr("%1 deleted a user:%2")).arg(_curUser.name).arg(_userList.last().name);
    LogsManager::instance()->addLogs(tr("Info"), tr("UserManager"), logInfo);
    _userList.removeAt(i);
}

bool Config::hasAdminPriv()
{
    UserInfo curUser = Config::instance()->getCurUser();
    if(curUser.type == "Admin"){
        return true;
    }
    else{
        MainWindow::instance()->showNotice(tr("need Admin privilege"));
        return false;
    }
}

bool Config::hasNormalPriv()
{
    UserInfo curUser = Config::instance()->getCurUser();
    if(curUser.type == "Normal" || curUser.type == "Admin"){
        return true;
    }
    else{
        MainWindow::instance()->showNotice(tr("need Normal or Admin privilege"));
        return false;
    }
}

QVariant Config::getInitValue(QString node, QString key)
{
    return _initSetting->value(QString("/%1/%2").arg(node).arg(key));
}

void Config::setInitValue(QString node, QString key, QVariant value)
{
    _initSetting->setValue(QString("/%1/%2").arg(node).arg(key), value);
}

////////////////////////////////////////////////////////
/// \brief Singleton模式相关代码
QMutex Config::Mutex;               //实例互斥锁。
Config* Config::_Instance = NULL;   //实例指针,默认为NULL。

Config* Config::instance()
{
    //使用双重检测。
    if(!_Instance)//第一次检测
    {
        QMutexLocker locker(&Mutex);//加互斥锁。
        if(!_Instance)//第二次检测。
        {
            _Instance = new Config();
            _Instance->initManufacturerCfg();
        }
    }

    return _Instance;
}

MfrsCfg& Config::curMfrsCfg()
{
    Q_ASSERT(_Instance != NULL);
    return _Instance->_curMfrsCfg;
}

SkinType Config::curSkinType()
{
    Q_ASSERT(_Instance != NULL);
    return _Instance->_curSkin->skinType();
}

SkinStyle* Config::curSkinStyle()
{
    Q_ASSERT(_Instance != NULL);
    return _Instance->_curSkin;
}

void Config::ChangeSkin(SkinType newSkin)
{
    Q_ASSERT(_Instance != NULL);

    if(newSkin != _Instance->_curSkin->skinType()) {
        _Instance->setInitValue("SOFTWARE", "skinStyle", QVariant(newSkin));
        if(_Instance->_curSkin != NULL){
            delete _Instance->_curSkin;
        }
        _Instance->_curSkin = new SkinStyle(newSkin);
    }
}


bool Config::remvoeTsFileVanishedAndObsolete(const QString &srcFileName, const QString &destFileName)
{
    bool isAlter = false;//是否修改翻译文件文件
    QDomDocument doc;
    QString errorMsg;
    int errorLine=0, errorColumn=0;
    //加载翻译文件
    QFile srcFile(srcFileName);
    if(!srcFile.open(QIODevice::ReadOnly)) {
        qDebug() << __FUNCTION__ << __LINE__ << QString("Open the failure(%1)").arg(srcFileName);
        return false;
    }

    QByteArray tsContent = srcFile.readAll();
//    qDebug() << __FUNCTION__ << __LINE__ << tsContent;
    bool bPase = doc.setContent(tsContent, &errorMsg, &errorLine, &errorColumn);
    if(!bPase) {
        qDebug() << __FUNCTION__ << __LINE__ << errorMsg << errorLine << errorColumn;
        return false;
    }
    srcFile.close();

    QDomElement elemntTS = doc.elementsByTagName("TS").at(0).toElement();
    QDomNodeList nlContexts = elemntTS.elementsByTagName("context");

    //加载 手动添加的系统翻译 (SystemTranslation.ts   --包含 QColorDialog以及右键菜单栏、弹窗汉化)
    QStringList sysTr;
    QDomDocument sysTrDoc;
    if(!sysTrDoc.setContent(SystemTranslation(), &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << __FUNCTION__ << __LINE__ << errorMsg << errorLine << errorColumn;
        return false;
    }
    QDomNodeList dnlContext = sysTrDoc.elementsByTagName("TS").at(0).toElement().elementsByTagName("context");
    for(int i = 0; i < dnlContext.count(); i++) {
        QDomNode nodeContext = dnlContext.at(i);
        sysTr.append(nodeContext.firstChildElement("name").text());
    }

    //检索翻译文件
    for(int i = 0; i < nlContexts.count(); i++)
    {
        QDomNode nodeContext = nlContexts.at(i);

        QDomNodeList nlMessages = nodeContext.toElement().elementsByTagName("message");
        int nCount = nlMessages.count();
        for(int j = 0; j < nlMessages.count(); )
        {
            QDomNode nodeMessage = nlMessages.at(j);
            QDomElement elementTranslation = nodeMessage.firstChildElement("translation");
            if (elementTranslation.attribute("type") == "vanished"
                    || elementTranslation.attribute("type") == "obsolete")
            {
                if(sysTr.contains(nodeContext.firstChildElement("name").text())) {
                    elementTranslation.removeAttribute("type");
                }else {
                    nodeContext.removeChild(nodeMessage);
                }
                isAlter = true;
            }else{
                j++;
            }
        }
        nlMessages = nodeContext.toElement().elementsByTagName("message");
        nCount = nlMessages.count();
        nCount = 0;
    }
    if (!isAlter) return true;
    QFile destFile(destFileName);
    if(!destFile.open(QIODevice::ReadWrite|QIODevice::Truncate))
        return false;
    destFile.write(doc.toByteArray());
    destFile.close();
    return true;
}

void Config::setCurNetInfo(QString curIp, QNetworkInterface curNet)
{
    _curIp = curIp;
    _curNet = curNet;
    setInitValue("SOFTWARE", "defaultIp", _curIp);
}

QByteArray Config::SystemTranslation()
{
    QByteArray context;
    QString fileName = QApplication::applicationDirPath() + "/../language/SystemTranslation.ts";
    QFile sysTrFile(fileName);
    if(!sysTrFile.open(QIODevice::ReadOnly)) {
        qDebug() << __FUNCTION__ << __LINE__ << QString("Open the failure(%1)").arg(fileName);
        return context;
    }
    context = sysTrFile.readAll();
    sysTrFile.close();
    return context;
}
